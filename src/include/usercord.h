/**
 ** USERCORD.H
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

#include "usrscale.h"

#define U2SX(x,c) \
    SCALE(x,((x) - (c)->gc_usrxbase),(c)->gc_xmax,(c)->gc_usrwidth)
#define U2SY(y,c) \
    SCALE(y,((y) - (c)->gc_usrybase),(c)->gc_ymax,(c)->gc_usrheight)

#define S2UX(x,c) do {                                          \
    SCALE(x,x,(c)->gc_usrwidth,(c)->gc_xmax);                   \
    (x) += (c)->gc_usrxbase;                                    \
} while(0)

#define S2UY(y,c) do {                                          \
    SCALE(y,y,(c)->gc_usrheight,(c)->gc_ymax);                  \
    (y) += (c)->gc_usrybase;                                    \
} while(0)
