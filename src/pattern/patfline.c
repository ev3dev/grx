/**
 ** PATFLINE.C
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **
 **  Copyright (C) 1992, Csaba Biegl
 **    820 Stirrup Dr, Nashville, TN, 37221
 **    csaba@vuse.vanderbilt.edu
 **
 **  This file is distributed under the terms listed in the document
 **  "copying.cb", available from the author at the address above.
 **  A copy of "copying.cb" should accompany this file; if not, a copy
 **  should be available from where this file was obtained.  This file
 **  may not be distributed without a verbatim copy of "copying.cb".
 **  You should also have received a copy of the GNU General Public
 **  License along with this program (it is in the file "copying");
 **  if not, write to the Free Software Foundation, Inc., 675 Mass Ave,
 **  Cambridge, MA 02139, USA.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **/

#include "libgrx.h"
#include "clipping.h"
#include "shapes.h"

void GrPatternFilledLine(int x1,int y1,int x2,int y2,GrPattern *p)
{
        union { GrFrame *c; unsigned char *b; } src;
        int dx,dy,sy,ymajor;
        int pw,ph,px,py;
        int ispixmap;
        GrColor fgc = 0, bgc = 0;
        int points,error;
        int by1, by2;

        if (x1 > x2) {
                iswap(x1,x2);
                iswap(y1,y2);
        }
        if(y1 == y2) {
            clip_hline(CURC,x1,x2,y1);
            mouse_block(CURC,x1,y1,x2,y1);
            _GrFillPattern(x1,y1,(x2 - x1 + 1),p);
            mouse_unblock();
            return;
        }
        if(x1 == x2) {
            clip_vline(CURC,x1,y1,y2);
        }
        else {
            clip_line(CURC,x1,y1,x2,y2);
        }
        dx = x2 - x1;
        dy = y2 - y1;
        if(dy >= 0) {
            by1 = y1;
            by2 = y2;
            sy = 1;
        }
        else {
            by1 = y2;
            by2 = y1;
            dy = (-dy);
            sy = (-1);
        }
        mouse_block(CURC,x1,by1,x2,by2);
        if((ispixmap = p->gp_ispixmap) != FALSE) {
            pw = p->gp_pxp_width;
            ph = p->gp_pxp_height;
            px = (x1 + CURC->gc_xoffset) % pw;
            py = (y1 + CURC->gc_yoffset) % ph;
            src.c = &p->gp_pxp_source;
        }
        else {
            pw = 8;
            ph = p->gp_bmp_height;
            px = (x1 + CURC->gc_xoffset) & 7;
            py = (y1 + CURC->gc_yoffset) % ph;
            src.b = (unsigned char *) p->gp_bmp_data;
            fgc = p->gp_bmp_fgcolor;
            bgc = p->gp_bmp_bgcolor;
        }
        if(dy > dx) {
            points = dy + 1;
            error  = dy >> 1;
            ymajor = TRUE;
        }
        else {
            points = dx + 1;
            error  = dx >> 1;
            ymajor = FALSE;
        }
        while(--points >= 0) {
            (*CURC->gc_driver->drawpixel)(x1,y1,
                ispixmap ?
                    (*src.c->gf_driver->readpixel)(src.c,px,py) :
                    (src.b[py] & (0x80U >> px)) ? fgc : bgc
            );
            if(ymajor) {
                if((error -= dx) < 0) error += dy,x1++,px++;
                y1 += sy,py += sy;
            }
            else {
                if((error -= dy) < 0) error += dx,y1 += sy,py += sy;
                x1++,px++;
            }
            if((unsigned)py >= (unsigned)ph) {
                if(py < 0) py += ph;
                else       py -= ph;
            }
            if(px >= pw) px = 0;
        }
        mouse_unblock();
}

void _GrPatternFilledLine(int x1,int y1,int dx,int dy,GrPattern *p)
{
        GrPatternFilledLine(x1,y1,x1 + dx,y1 + dy,p);
}
