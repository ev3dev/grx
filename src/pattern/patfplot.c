/**
 ** PATFPLOT.C
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

void GrPatternFilledPlot(int x,int y,GrPattern *p)
{
        int xp,yp;

        clip_dot(CURC,x,y);
        mouse_block(CURC,x,y,x,y);
        if(p->gp_ispixmap) {
            xp = (x + CURC->gc_xoffset) % p->gp_pxp_width;
            yp = (y + CURC->gc_yoffset) % p->gp_pxp_height;
            (*CURC->gc_driver->drawpixel)(x,y,
                (*p->gp_pxp_source.gf_driver->readpixel)(&p->gp_pxp_source,xp,yp)
            );
        }
        else {
            xp = (x + CURC->gc_xoffset) & 7;
            yp = (y + CURC->gc_yoffset) % p->gp_bmp_height;
            (*CURC->gc_driver->drawpixel)(x,y,
                (p->gp_bmp_data[yp] & (0x80U >> xp)) ? p->gp_bmp_fgcolor : p->gp_bmp_bgcolor
            );
        }
        mouse_unblock();
}

