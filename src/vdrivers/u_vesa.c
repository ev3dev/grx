/**
 ** U_VESA.C ---- interface utility functions to VESA BIOS inquiry calls
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Hartmut Schirmer (hsc@xaphod.techfak.uni-kiel.d400.de)
 **/

#include "libgrx.h"
#include "allocate.h"
#include "vesa.h"
#include "mempeek.h"
#include "memfill.h"
#include "int86.h"

#ifndef IREG_ES

int _GrViDrvVESAgetVGAinfo(VESAvgaInfoBlock *ib)
{
        return(FALSE);
}
int _GrViDrvVESAgetModeInfo(int mode,VESAmodeInfoBlock *ib)
{
        return(FALSE);
}

VESApmInfoBlock * _GrViDrvVESAgetPMinfo(void)
{
        return(NULL);
}

#else

int _GrViDrvVESAgetVGAinfo(VESAvgaInfoBlock *ib)
{
        static char  *nmcopy = NULL;
        static short *mdcopy = NULL;
        Int86Regs regs;
        short far *mp;
        short *modes;
        char far *p1;
        char *p2;
        int  ii;
        DECLARE_XFER_BUFFER(1000);
        /*
         * copy VBE 2.0 tag into XFER buffer
         */
        setup_far_selector(LINP_SEL(XFER_BUFFER));
        p1 = (char far *)LINP_PTR(XFER_BUFFER);
        poke_b_f(p1,'V'); ++p1;
        poke_b_f(p1,'B'); ++p1;
        poke_b_f(p1,'E'); ++p1;
        poke_b_f(p1,'2'); ++p1;
        /*
         * set up registers and call interrupt
         */
        sttzero(&regs);
        IREG_AX(regs) = VESA_FUNC + VESA_VGA_INFO;
        IREG_ES(regs) = FP_SEG(XFER_BUFFER);
        IREG_DI(regs) = FP_OFF(XFER_BUFFER);
        int10(&regs);
        if(IREG_AX(regs) != VESA_SUCCESS) return(FALSE);
        /*
         * copy VESA info block into accessible memory
         */
        setup_far_selector(LINP_SEL(XFER_BUFFER));
        p1 = (char far *)LINP_PTR(XFER_BUFFER);
        p2 = (char *)ib;
        for(ii = sizeof(*ib); --ii >= 0; p1++,p2++) *p2 = peek_b_f(p1);
        if(ib->VESAsignature[0] != 'V') return(FALSE);
        if(ib->VESAsignature[1] != 'E') return(FALSE);
        if(ib->VESAsignature[2] != 'S') return(FALSE);
        if(ib->VESAsignature[3] != 'A') return(FALSE);
        /*
         * allocate space and copy mode list into accessible memory
         */
        mp = LINP_PTR(MK_FP(FP86_SEG(ib->VideoModePtr),FP86_OFF(ib->VideoModePtr)));
        p1 = (char far *)mp;
        for(ii = 1; (short)peek_w_f(mp) != (short)(-1); mp++,ii++);
        modes = mdcopy = realloc(mdcopy,ii * sizeof(short));
        if(!modes) return(FALSE);
        ib->VideoModePtr = modes;
        mp = (short far *)p1;
        for( ; --ii >= 0; mp++,modes++) *modes = peek_w_f(mp);
        /*
         * allocate space and copy ID string into accessible memory
         */
        p1 = LINP_PTR(MK_FP(FP86_SEG(ib->OEMstringPtr),FP86_OFF(ib->OEMstringPtr)));
        mp = (short far *)p1;
        for(ii = 1; (char)peek_b_f(p1) != (char)(0); p1++,ii++);
        p2 = nmcopy = realloc(nmcopy,ii * sizeof(char));
        if(!p2) return(FALSE);
        ib->OEMstringPtr = p2;
        p1 = (char far *)mp;
        for( ; --ii >= 0; p1++,p2++) *p2 = peek_b_f(p1);
        return(TRUE);
}

VESApmInfoBlock * _GrViDrvVESAgetPMinfo(void) {
  Int86Regs r;
  static VESApmInfoBlock *ib = NULL;
  unsigned Length, ii;
  char far *p1;
  char *p2;

  sttzero(&r);
  IREG_AX(r) = VESA_FUNC + VESA_PM_INTERF;
  IREG_BX(r) = 0x0000;
  int10(&r);
  if(IREG_AX(r) != VESA_SUCCESS)
    return(NULL);
  /* Now we have : CX         = length of table and routines (bytes)
                   ES:DI = pointer to table
                            ES_DI+00: offset PM set window routine
                                 +02: offset PM set display start
                                 +04: offset PM set primary palette
                                 +06: offset PM description table  */
  Length = (unsigned)IREG_CX(r);
  if( Length == 0) return(NULL);
  ib = realloc(ib, Length + VESApmInfoBlock_BASEOFF);
  if (ib == NULL) return(NULL);

  ib->RealMode_SEG   = IREG_ES(r);
  ib->RealMode_OFF   = IREG_DI(r);
  ib->PhysicalLength = IREG_CX(r);

  setup_far_selector(LINP_SEL(MK_FP( IREG_ES(r),IREG_DI(r))));
  p1 = LINP_PTR( MK_FP( IREG_ES(r),IREG_DI(r) ) );
  p2 = (char *) &(ib->SetWindow_off);
  for(ii = 0; ii < Length; p1++,ii++) *(p2++) = peek_b_f(p1);
  return(ib);
}

int _GrViDrvVESAgetModeInfo(int mode,VESAmodeInfoBlock *ib)
{
        Int86Regs regs;
        char far *p1;
        char *p2;
        int  ii;
        DECLARE_XFER_BUFFER(1000);
        /*
         * set up registers and call interrupt
         */
        sttzero(&regs);
        IREG_AX(regs) = VESA_FUNC + VESA_MODE_INFO;
        IREG_CX(regs) = mode;
        IREG_ES(regs) = FP_SEG(XFER_BUFFER);
        IREG_DI(regs) = FP_OFF(XFER_BUFFER);
        int10(&regs);
        if(IREG_AX(regs) != VESA_SUCCESS) return(FALSE);
        /*
         * copy VESA info block into accessible memory
         */
        setup_far_selector(LINP_SEL(XFER_BUFFER));
        p1 = (char far *)LINP_PTR(XFER_BUFFER);
        p2 = (char *)ib;
        for(ii = sizeof(*ib); --ii >= 0; p1++,p2++) *p2 = peek_b_f(p1);
        return((ib->ModeAttributes & MODE_SUPPORTED) ? TRUE : FALSE);
}

#endif

