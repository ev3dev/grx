/**
 ** LNXINPUT.C ---- mouse and keyboard interface for Linux 
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 ** Contributions by: (See "doc/credits.doc" for details)
 ** Mauro Condarelli (mc5686@mclink.it)
 **/
 
#include <stdio.h>
#include <termio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <vga.h>
#include <vgamouse.h>

#include "libgrx.h"
#include "input.h"
#include "arith.h"
#include "memfill.h"
#include "memcopy.h"
#include "lnxkbd.h"

/*
 * keyboard stuff 
 */
static struct termio kbd_setup;
static struct termio kbd_reset;
static int    kbd_initted = FALSE;
static int    kbd_enabled = TRUE;
static int    kbd_isatty;
static int    kbd_lastchr;
static int    kbd_filedsc;
static enum { normal, test, wait } kbd_mode;

static void kbd_restore(void)
{
        if(kbd_initted && kbd_isatty && (kbd_mode != normal)) {
            ioctl(kbd_filedsc,TCSETA,&kbd_reset);
            kbd_mode = normal;
        }
}

static void kbd_init(void)
{
        if(!kbd_initted) {
            kbd_initted = TRUE;
            kbd_lastchr = EOF;
            kbd_filedsc = fileno(stdin);
            kbd_isatty  = isatty(kbd_filedsc);        
            if(kbd_isatty) {
                ioctl(kbd_filedsc,TCGETA,&kbd_setup);
                ioctl(kbd_filedsc,TCGETA,&kbd_reset);
                kbd_setup.c_lflag &= ~(ICANON | ECHO );
                kbd_setup.c_iflag &= ~(INLCR  | ICRNL);
                atexit(kbd_restore);
                kbd_mode = normal;
            }
        }
}

static
int inkey(void) {
   static int n = 0;
   static char keybuf[10];
   int f = 0, realkey = EOF;
        
   if (n<5) {
      f = read(kbd_filedsc, keybuf+n, 5);
      if (f>0) n += f;
   }
   if (n<=0) return EOF;
#if TEST
   {  int i;putchar('{');
      for(i=0;i<n;i++){
         if(keybuf[i]<' '){putchar('^');putchar(keybuf[i]+'@');}
         else{putchar(keybuf[i]);}
      } putchar('}');}
#endif
   if ((keybuf[0] == KB_ESC) && (keybuf[1] == '[') && (n > 2)) {
      switch (keybuf[2]){
      case 'A':        f=3; realkey=KB_CURUP; break;
      case 'B':        f=3; realkey=KB_CURDN; break;
      case 'C':        f=3; realkey=KB_CURRT; break;
      case 'D':        f=3; realkey=KB_CURLF; break;
      case '[': {
         switch (keybuf[3]) {
         case 'A':        f=4; realkey=KB_F1; break;
         case 'B':        f=4; realkey=KB_F2; break;
         case 'C':        f=4; realkey=KB_F3; break;
         case 'D':        f=4; realkey=KB_F4; break;
         case 'E':        f=4; realkey=KB_F5; break;
         default:        f=4; realkey=KB_ESC;
         }
      } break;
      default:
         if (keybuf[3] == '~') {
            f=4; realkey=KB_ESC;
            switch (keybuf[2]){
            case '1':       realkey=KB_HOME  ; break;
            case '2':       realkey=KB_INSERT; break;
            case '3':       realkey=KB_DELETE; break;
            case '4':       realkey=KB_END   ; break;
            case '5':       realkey=KB_PGUP  ; break;
            case '6':       realkey=KB_PGDN  ; break;
            }
         } else if (keybuf[4] == '~') {
            int i = 10*(keybuf[2]-'0') + (keybuf[3] - '0');
            f=5; realkey=KB_ESC;
            if (17 <= i && i <= 34) {
               static int fnk[] = { KB_F6, KB_F7, KB_F8, KB_F9, KB_F10, KB_ESC,
                                       KB_F11, KB_F12, KB_SHIFT_F3, KB_SHIFT_F4, KB_ESC,
                                       KB_SHIFT_F5, KB_SHIFT_F6, KB_ESC,
                                       KB_SHIFT_F7, KB_SHIFT_F8, KB_SHIFT_F9, KB_SHIFT_F10 };
               realkey = fnk[i-17];
            }
         } else {
            realkey = KB_ESC;
         }
      }
   } else {
      f=1; realkey=keybuf[0];
   }
   {
      int k=0; 
      while (f<n) 
         keybuf[k++]=keybuf[f++];
      n = k;
   }

   return realkey;
}

int _GrCheckKeyboardHit(void)
{
        if(!kbd_initted) {
            kbd_init();
        }
        if(!kbd_isatty) {
            return(TRUE);
        }
        if(kbd_lastchr != EOF) {
            return(TRUE);
        }
        if(kbd_mode != test) {
            kbd_setup.c_cc[VMIN]  = 0;
            kbd_setup.c_cc[VTIME] = 0;
            if(ioctl(kbd_filedsc,TCSETAW,&kbd_setup) == EOF) return(FALSE);
            kbd_mode = test;
        }
#if 0
        {
            char buf[10];
            if(read(kbd_filedsc,buf,1) > 0) {
                kbd_lastchr = buf[0];
                return(TRUE);
            }
            return(FALSE);
        }
#else
        kbd_lastchr = inkey();
        return (kbd_lastchr != EOF);
#endif
}

int _GrReadCharFromKeyboard(void)
{
        unsigned char buf[4];
        if(!kbd_initted) {
            kbd_init();
        }
        if(!kbd_isatty) {
            return(getc(stdin));
        }
        if(kbd_lastchr != EOF) {
            buf[0] = kbd_lastchr;
            kbd_lastchr = EOF;
            return(buf[0]);
        }
        if(kbd_mode != wait) {
            kbd_setup.c_cc[VMIN]  = 1;
            kbd_setup.c_cc[VTIME] = 0;
            if(ioctl(kbd_filedsc,TCSETAW,&kbd_setup) == EOF) return(EOF);
            kbd_mode = wait;
        }
#if 0
        if(read(kbd_filedsc,buf,1) > 0) {
            return(buf[0]);
        }
        return(EOF);
#else
        return inkey();
#endif
}

/*
 * Mouse stuff
 */
 
static int  mou_filedsc;
static int  mou_lastxpos;
static int  mou_lastypos;
static int  mou_buttons;
static int  mou_enabled = TRUE;
static long evt_lasttime;

int GrMouseDetect(void)
{
        if(MOUINFO->msstatus == 0) {
            MOUINFO->msstatus = (-1);                /* assume missing */
            mou_filedsc  = mouse_init_return_fd(
                    "/dev/mouse",
                    vga_getmousetype(),
                    MOUSE_DEFAULTSAMPLERATE
            );
            if(mou_filedsc >= 0) {
                    MOUINFO->msstatus = 1;                /* present, but not initted */
                    atexit(mouse_close);
                    mouse_setxrange(0,32767);
                    mouse_setyrange(0,32767);
                    mouse_setwrap(MOUSE_NOWRAP);
                    mouse_setscale(1);
            }
        }
        return((MOUINFO->msstatus > 0) ? TRUE : FALSE);
}

static void uninit(void)
{
        if(MOUINFO->msstatus > 1) MOUINFO->msstatus = 1;
        kbd_restore();
}

void GrMouseInitN(int queue_size)
{
        uninit();
        queue_size = umax(4,umin(256,queue_size));
        init_queue(queue_size);
        kbd_init();
        if(GrMouseDetect()) {
            GrMouseSetSpeed(1,1);
            GrMouseSetAccel(100,1);
            GrMouseSetLimits(0,0,SCRN->gc_xmax,SCRN->gc_ymax);
            GrMouseWarp((SCRN->gc_xmax >> 1),(SCRN->gc_ymax >> 1));
            _GrInitMouseCursor();
               MOUINFO->msstatus = 2;
        }
        GrMouseEventEnable(TRUE,TRUE);
        read_time(evt_lasttime);
        MOUINFO->uninit = uninit;
}

void GrMouseSetSpeed(int spmult,int spdiv)
{
        MOUINFO->spmult = umin(16,umax(1,spmult));
        MOUINFO->spdiv  = umin(16,umax(1,spdiv));
}

void GrMouseSetAccel(int thresh,int accel)
{
        MOUINFO->thresh = umin(64,umax(1,thresh));
        MOUINFO->accel  = umin(16,umax(1,accel));
}

void GrMouseSetLimits(int x1,int y1,int x2,int y2)
{
        isort(x1,x2);
        isort(y1,y2);
        MOUINFO->xmin = imax(0,imin(x1,SCRN->gc_xmax));
        MOUINFO->ymin = imax(0,imin(y1,SCRN->gc_ymax));
        MOUINFO->xmax = imax(0,imin(x2,SCRN->gc_xmax));
        MOUINFO->ymax = imax(0,imin(y2,SCRN->gc_ymax));
}

void GrMouseWarp(int x,int y)
{
        MOUINFO->xpos = imax(MOUINFO->xmin,imin(MOUINFO->xmax,x));
        MOUINFO->ypos = imax(MOUINFO->ymin,imin(MOUINFO->ymax,y));
        mou_lastxpos  = 16384;
        mou_lastypos  = 16384;
        mouse_setposition(16384,16384);
        GrMouseUpdateCursor();
}

void GrMouseEventEnable(int enable_kb,int enable_ms)
{
        if(!enable_kb) kbd_restore();        
        kbd_enabled = enable_kb;
        mou_enabled = enable_ms;
}

void _GrUpdateInputs(void)
{
        for( ; ; ) {
            GrMouseEvent ev;
            int gotevt = FALSE;
            if(mou_enabled && (MOUINFO->msstatus == 2) && mouse_update()) {
                    int mx = mouse_getx();
                    int my = mouse_gety();
                    int mb = mouse_getbutton();
                    update_coord(x,(mx - mou_lastxpos));
                    update_coord(y,(my - mou_lastypos));
                    mou_lastxpos = mx;
                    mou_lastypos = my;
                    if(mb != mou_buttons) {
                        fill_mouse_ev(
                                ev,
                                mou_buttons,mb,
                                MOUSE_LEFTBUTTON,
                                MOUSE_MIDDLEBUTTON,
                                MOUSE_RIGHTBUTTON,
                                0
                        );
                    get_dtime(ev.dtime,evt_lasttime);
                        enqueue_event(ev);
                        MOUINFO->moved = FALSE;
                        mou_buttons = mb;
                    }
                    gotevt = TRUE;
            }
            if(kbd_enabled && kbd_isatty && _GrCheckKeyboardHit()) {
                int key = _GrReadCharFromKeyboard();
                fill_keybd_ev(ev,key,0);
                get_dtime(ev.dtime,evt_lasttime);
                enqueue_event(ev);
                MOUINFO->moved = FALSE;
                gotevt = TRUE;
            }
            if(!gotevt) break;
        }
}

void GrMouseGetEventT(int flags,GrMouseEvent *ev,long tout)
{
        int msdraw;        
        if(MOUINFO->msstatus == 0) GrMouseInit();
        msdraw = !MOUINFO->displayed && !(flags & GR_M_NOPAINT);
        if(msdraw) GrMouseDisplayCursor();
        for( ; ; ) {
            fd_set readfds;
            struct timeval wtime;
            int    N;
            _GrUpdateInputs();
            GrMouseUpdateCursor();
            while(MOUINFO->qlength > 0) {
                    dequeue_event((*ev));
                    if(ev->flags & flags) {
                    if(msdraw) GrMouseEraseCursor();
                    return;
                }
            }
            if((flags & GR_M_POLL) || 
               (tout == 0) ||
               (MOUINFO->moved && (flags & GR_M_MOTION))) {
                    fill_mouse_ev(
                        (*ev),
                        mou_buttons,mou_buttons,
                        MOUSE_LEFTBUTTON,
                        MOUSE_MIDDLEBUTTON,
                        MOUSE_RIGHTBUTTON,
                        0
                    );
                    get_dtime(ev->dtime,evt_lasttime);
                MOUINFO->moved = FALSE;
                if(msdraw) GrMouseEraseCursor();
                return;
            }
            FD_ZERO(&readfds);
            N = 0;
            if(kbd_enabled && kbd_isatty) {
                FD_SET(kbd_filedsc,&readfds);
                N = kbd_filedsc + 1;
            }
            if(mou_enabled && (MOUINFO->msstatus == 2)) {
                FD_SET(mou_filedsc,&readfds);
                N = umax(N,(mou_filedsc + 1));
            }
            if(N == 0) {
                    if(tout > 0) usleep(tout * 1000);
                    tout = 0;
                    continue;
            }
            if(tout > 0) {
                    wtime.tv_sec  = (tout / 1000);
                    wtime.tv_usec = (tout % 1000) * 1000;
                    select(N,&readfds,NULL,NULL,&wtime);
                    tout = 0;
                    continue;
            }
            select(N,&readfds,NULL,NULL,NULL);
        }
}

