/**
 ** VESA.C ---- the GRX 2.0 VESA BIOS interface: protected mode stuff
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 ** Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 **/

#include <dpmi.h>

/* memory information for linear frame buffer access */
static int           LFB_Selector      = -1;
static unsigned long LFB_LinearAddress = 0;

/* memory information for protected mode     **
** (VESA 2.0) banking with memory mapped i/o */
int           MEM_IO_sel = -1;
unsigned long MEM_IO_la  = 0;

/* memory for portected mode banking, virtual **
** screen handling and palette manipulation   */
static void *PM_base        = NULL;
static int   PM_base_len    = 0;
static void *PM_banking_ptr = NULL;
/*
static void *PM_dstart_ptr  = NULL;
static void *PM_palette_ptr = NULL;
*/

/* ------------------------------------------------------------- */
/* memory access helper functions */
static int map_linear(unsigned long adr, unsigned long len,
                      int *sel, unsigned long *la ) {
  if(*sel<0) {
    *sel = __dpmi_allocate_ldt_descriptors(1);
    if (*sel<0) return FALSE;
  }
  if(!*la) {
    __dpmi_meminfo meminfo;
    meminfo.address = adr;
    meminfo.size    = len;
    if(__dpmi_physical_address_mapping(&meminfo)==-1) {
       return FALSE;
    }
    if(__dpmi_set_segment_base_address(*sel,meminfo.address)==-1) {
       return FALSE;
    }
    if(__dpmi_set_segment_limit(*sel,len-1)==-1) {
       return FALSE;
    }
    *la=meminfo.address;
  }
  return TRUE;
}

static void free_linear(int *sel, unsigned long *la) {
  if(*la) {
      __dpmi_meminfo meminfo;
      meminfo.address = *la;
      __dpmi_free_physical_address_mapping(&meminfo);
      *la = 0;
  }
  if(*sel >= 0) {
      __dpmi_free_ldt_descriptor(*sel);
      *sel = -1;
  }
}

/* ------------------------------------------------------------- */

static void PM_free(void) {
  free_linear(&MEM_IO_sel, &MEM_IO_la);
  if (PM_base != NULL) {
    unsigned long baseaddr;
    __dpmi_meminfo mem;
    __dpmi_get_segment_base_address(_go32_my_ds(), &baseaddr);
    mem.address = baseaddr + (unsigned long)PM_base;
    mem.size    = PM_base_len;
    __dpmi_unlock_linear_region(&mem);
    free(PM_base);
  }
  PM_base = NULL;
}

static int PM_alloc(int len) {
  PM_base_len = len;
  PM_base = malloc(PM_base_len);
  if (PM_base == NULL) return 0;
  _go32_dpmi_lock_data(PM_base, PM_base_len);
  return 1;
}

/* ------------------------------------------------------------- */
/* protected mode banking */
static inline void PM_banking(short BX, short DX) {
  __asm__ volatile (
        "pushal\n\t"
        "call %3\n\t"
        "popal"
        : /* no output */
        : "a" (0x4F05), "b" (BX), "d" (DX),
          "r" (PM_banking_ptr)
  );
}

static void PM_setrwbanks(int rb,int wb) {
  PM_banking( _GrVidDrvVESAwrbank, wb << _GrVidDrvVESAbanksft);
  PM_banking( _GrVidDrvVESArdbank, rb << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}

static void PM_setbank(int bk) {
  PM_banking( _GrVidDrvVESAwrbank, bk << _GrVidDrvVESAbanksft);
  if(_GrVidDrvVESArdbank >= 0)
    PM_banking( _GrVidDrvVESArdbank, bk << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}

static inline void PM_es_banking(short es, short BX, short DX) {
  __asm__ volatile (
        "pushal\n\t"
        "movw %%ax, %%es\n\t"
        "movw $0x4f05, %%ax\n\t"
        "call %3\n\t"
        "popal"
        : /* no output */
        : "a" (es), "b" (BX), "d" (DX),
          "r" (PM_banking_ptr)
  );
}

static void PM_es_setrwbanks(int rb,int wb) {
  PM_es_banking(MEM_IO_sel, _GrVidDrvVESAwrbank, wb << _GrVidDrvVESAbanksft);
  PM_es_banking(MEM_IO_sel, _GrVidDrvVESArdbank, rb << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}

static void PM_es_setbank(int bk) {
  PM_es_banking(MEM_IO_sel, _GrVidDrvVESAwrbank, bk << _GrVidDrvVESAbanksft);
  if(_GrVidDrvVESArdbank >= 0)
    PM_es_banking(MEM_IO_sel, _GrVidDrvVESArdbank, bk << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}

#define ADD2PTR(p,o) ((void *) ((char *)(p)+(o)) )

static void VBE2ProtMode(void) {
  /* VBE 2.0+ may provide protected mode banking functions */
  VESApmInfoBlock *PMinfo;
  unsigned long   MS_ptr = 0;
  unsigned short  MS_len = 0;

  if (PM_base != NULL) return;

  /* Default: INT 10H based bank switching */
  _SETRWBANKS = RM_setrwbanks;
  _SETBANK    = RM_setbank;

  if ( (PMinfo=_GrViDrvVESAgetPMinfo()) == NULL) return;
  if (PMinfo->SubTable_off != 0x0000) {
    unsigned short *subtab;
    subtab = ADD2PTR(&PMinfo->SetWindow_off,PMinfo->SubTable_off);
    /* Skip over port info */
    while (*subtab != 0xFFFF) ++subtab;
    if (*(++subtab) != 0xFFFF)
      /* The memory location area isn't empty. The VESA bios
       * bios requires an additional memory selector set up in ES */
      MS_ptr = *((unsigned long *)subtab);
      MS_len = *(subtab+2);
      if ( !MS_ptr || !MS_len) return; /* something's wrong */
  }
  if (!PM_alloc(PMinfo->PhysicalLength)) return;
  memcpy(PM_base, &PMinfo->SetWindow_off, PMinfo->PhysicalLength);
  PM_banking_ptr = ADD2PTR(PM_base, PMinfo->SetWindow_off);
/*
  PM_dstart_ptr  = ADD2PTR(PM_base, PMinfo->DisplStart_off);
  PM_palette_ptr = ADD2PTR(PM_base, PMinfo->PPalette_off);
*/
  if (MS_ptr || MS_len) {
    if (!map_linear(MS_ptr, MS_len, &MEM_IO_sel, &MEM_IO_la)) return;
    _SETRWBANKS = PM_es_setrwbanks;
    _SETBANK    = PM_es_setbank;
  } else {
    _SETRWBANKS = PM_setrwbanks;
    _SETBANK    = PM_setbank;
  }
}
#undef ADD2PTR

static int _SETUP(GrVideoMode *mp,int noclear) {
   int res = 0;

   if ( mp->extinfo->flags&GR_VMODEF_LINEAR  &&
        mp->extinfo->mode != GR_frameText       ) {
     Int86Regs r;
     sttzero(&r);
     IREG_AX(r) = VESA_FUNC + VESA_SET_MODE;
     IREG_BX(r) = (mp->mode & 0x7fff)
                | (noclear ? 0x8000U : 0)
                | 0x4000U;
     int10(&r);
     res = IREG_AX(r) == VESA_SUCCESS;
   } else
     res = _GrViDrvSetEGAVGAmode(mp,noclear);
   return res;
}

static void reset(void) {
  int i;

  _SETRWBANKS = RM_setrwbanks;
  _SETBANK    = RM_setbank;

  for (i=0; i < nexts; ++i) {
     exts[i].setbank    = NULL;
     exts[i].setrwbanks = NULL;
  }

  PM_free();
  PM_banking_ptr = NULL;
/*
  PM_dstart_ptr  = NULL;
  PM_palette_ptr = NULL;
*/
  free_linear(&LFB_Selector, &LFB_LinearAddress);
  _GrViDrvResetEGAVGA();
}
