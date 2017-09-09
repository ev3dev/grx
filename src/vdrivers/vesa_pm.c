/**
 ** vesa_pm.c ---- the GRX 2.0 VBE2 BIOS interface: protected mode stuff
 **
 ** Watcom C++ 11.0 DOS4GW by Gary Sands [gsands@stbni.co.uk]
 ** DJGPP v2 by Andrzej Lawa [FidoNet: Andrzej Lawa 2:480/19.77]
 ** and Hartmut Schirmer (hsc@techfak.uni-kiel.de)
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

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

/* ----------------------------------------------------------- DPMI stuff -- */

#ifdef  __DJGPP__
#define WHO_AM_I "DJGPP"
#include <dpmi.h>

/* we do selector based video access with DJGPP.  **
** The selector gives access to full video ram    **
** starting at offset 0                           */
#define LFB_virtualAddr()    ((void *) 0)

#endif

/* ------------------------------------------------------------- */
/* memory access helper functions */
static int map_linear(unsigned long adr, unsigned long len,
	      int *sel, unsigned long *la ) {
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " map_linear\n"));
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
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " free_linear\n"));
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
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " PM_free\n"));
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
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " PM_alloc\n"));
  PM_base_len = len;
  PM_base = malloc(PM_base_len);
  if (PM_base == NULL) return 0;
  _go32_dpmi_lock_data(PM_base, PM_base_len);
  return 1;
}

/* ------------------------------------------------------------- */
/* protected mode banking */

#if defined(__GNUC__) && defined(__i386__)
static INLINE void PM_banking(short BX, short DX) {
  __asm__ volatile (
    " pushal   \n"
    " call *%3 \n"
    " popal      "
    : /* no output */
    : "a" (0x4F05), "b" (BX), "d" (DX),
      "r" (PM_banking_ptr)
  );
}

static INLINE void PM_es_banking(short es, short BX, short DX) {
  __asm__ volatile (
    " pushal             \n"
    " movw %%ax, %%es    \n"
    " movw $0x4f05, %%ax \n"
    " call *%3           \n"
    " popal                "
    : /* no output */
    : "a" (es), "b" (BX), "d" (DX),
      "r" (PM_banking_ptr)
  );
}
#endif /* __GNUC__ && __i386__ */

static void PM_setrwbanks(int rb,int wb) {
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " PM_setrwbanks\n"));
  PM_banking( _GrVidDrvVESAwrbank, wb << _GrVidDrvVESAbanksft);
  PM_banking( _GrVidDrvVESArdbank, rb << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}

static void PM_setbank(int bk) {
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " PM_setbank\n"));
  PM_banking( _GrVidDrvVESAwrbank, bk << _GrVidDrvVESAbanksft);
  if(_GrVidDrvVESArdbank >= 0)
    PM_banking( _GrVidDrvVESArdbank, bk << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}


static void PM_es_setrwbanks(int rb,int wb) {
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " PM_es_setrwbanks\n"));
  PM_es_banking(MEM_IO_sel, _GrVidDrvVESAwrbank, wb << _GrVidDrvVESAbanksft);
  PM_es_banking(MEM_IO_sel, _GrVidDrvVESArdbank, rb << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}

static void PM_es_setbank(int bk) {
  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " PM_es_setbank\n"));
  PM_es_banking(MEM_IO_sel, _GrVidDrvVESAwrbank, bk << _GrVidDrvVESAbanksft);
  if(_GrVidDrvVESArdbank >= 0)
    PM_es_banking(MEM_IO_sel, _GrVidDrvVESArdbank, bk << _GrVidDrvVESAbanksft);
  setup_far_selector(SCRN->gc_selector);
}


static void VBE2ProtMode(void) {
  /* VBE 2.0+ may provide protected mode banking functions */
  VESApmInfoBlock *PMinfo;
  unsigned long   MS_ptr = 0;
  unsigned short  MS_len = 0;

  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " VBE2ProtMode\n"));
  if (PM_base != NULL) return;

  /* Default: INT 10H based bank switching */
  _SETRWBANKS = RM_setrwbanks;
  _SETBANK    = RM_setbank;

  if ( (PMinfo=_GrViDrvVESAgetPMinfo()) == NULL) return;
  if (PMinfo->SubTable_off != 0x0000) {
    GR_int16u *subtab = ptradd(&PMinfo->SetWindow_off,PMinfo->SubTable_off);
    long bytes_left = (long)PMinfo->PhysicalLength
		    - (long)PMinfo->SubTable_off;
    /* Skip over port info but don't leave VBE2PM data */
    while (bytes_left >= 2 && peek_w(subtab) != 0xffff) {
      ptrinc(subtab,2);
      bytes_left -= 2;
    }
    /* skip 0xffff end marker */
    ptrinc(subtab,2);
    bytes_left -= 2;
    /* check for memory mapped IO:             **
    **   4 bytes for memory mapped IO base     **
    **   2 bytes for memory mapped IO length   */
    if (bytes_left >= 4+2 && peek_w(subtab) != 0xffff) {
      /* The memory location area isn't empty. The VESA bios
       * bios requires an additional memory selector set up in ES */
      MS_ptr = peek_l(subtab);
      ptrinc(subtab,4);
      MS_len = peek_w(subtab);
      /* MS_ptr == 0 -> no mem area */
    }
  }
  if (!PM_alloc(PMinfo->PhysicalLength)) return;
  memcpy(PM_base, &PMinfo->SetWindow_off, PMinfo->PhysicalLength);
  PM_banking_ptr = ptradd(PM_base, PMinfo->SetWindow_off);
/*
  PM_dstart_ptr  = ptradd(PM_base, PMinfo->DisplStart_off);
  PM_palette_ptr = ptradd(PM_base, PMinfo->PPalette_off);
*/
  if (MS_ptr && MS_len) {
    if (!map_linear(MS_ptr, MS_len, &MEM_IO_sel, &MEM_IO_la)) return;
    _SETRWBANKS = PM_es_setrwbanks;
    _SETBANK    = PM_es_setbank;
  } else {
    _SETRWBANKS = PM_setrwbanks;
    _SETBANK    = PM_setbank;
  }
}


static int _SETUP(GrVideoMode *mp,int noclear) {
   int res = 0;

   DBGPRINTF(DBG_DRIVER, (WHO_AM_I " _SETUP\n"));
   if ( mp->extinfo->flags&GR_VMODEF_LINEAR  &&
    mp->extinfo->mode != GR_frameText       ) {
     Int86Regs r;
     sttzero(&r);
     IREG_AX(r) = VESA_FUNC + VESA_SET_MODE;
     IREG_BX(r) = (mp->mode & 0x7fff)
	| (noclear ? 0x8000U : 0)
	| 0x4000U;
     DBGPRINTF(DBG_DRIVER, (WHO_AM_I " _SETUP calling int10\n"));
     int10(&r);
     DBGPRINTF(DBG_DRIVER, (WHO_AM_I " _SETUP int10 called\n"));
     res = IREG_AX(r) == VESA_SUCCESS;
   } else
     res = _GrViDrvSetEGAVGAmode(mp,noclear);
   DBGPRINTF(DBG_DRIVER, (WHO_AM_I " _SETUP finished\n"));
   return res;
}

static void reset(void) {
  int i;

  _SETRWBANKS = RM_setrwbanks;
  _SETBANK    = RM_setbank;

  DBGPRINTF(DBG_DRIVER, (WHO_AM_I " reset\n"));
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
