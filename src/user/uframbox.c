/**
 ** UFRAMBOX.C
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
#include "arith.h"
#include "usercord.h"

void GrUsrFramedBox(int x1,int y1,int x2,int y2,int wdt,GrFBoxColors *c)
{
        int w1,w2;

        U2SX(x1,CURC);
        U2SY(y1,CURC);
        U2SX(x2,CURC);
        U2SY(y2,CURC);
        SCALE(w1,wdt,CURC->gc_xmax,CURC->gc_usrwidth);
        SCALE(w2,wdt,CURC->gc_ymax,CURC->gc_usrheight);
        wdt = (iabs((int)w1) + iabs((int)w2)) >> 1;
        if(wdt == 0)
            GrFilledBox(x1,y1,x2,y2,c->fbx_intcolor);
        else GrFramedBox(x1,y1,x2,y2,wdt,c);
}
