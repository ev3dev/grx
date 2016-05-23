/*
 * UCIRCF.C
 *
 *  Copyright (C) 1992, Csaba Biegl
 *    820 Stirrup Dr, Nashville, TN, 37221
 *    csaba@vuse.vanderbilt.edu
 *
 *  This file is distributed under the terms listed in the document
 *  "copying.grx", available from the author at the address above.
 *  A copy of "copying.grx" should accompany this file; if not, a copy
 *  should be available from where this file was obtained.  This file
 *  may not be distributed without a verbatim copy of "copying.grx".
 *  You should also have received a copy of the GNU General Public
 *  License along with this program (it is in the file "copying");
 *  if not, write to the Free Software Foundation, Inc., 675 Mass Ave,
 *  Cambridge, MA 02139, USA.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *
 *     Copyright (C) 1997, Michael Goffioul
 *
 *       goffioul@emic.ucl.ac.be
 *
 *
 * Note : compiling the library with -DUSR_KEEP_SHAPE makes a circle
 *        looks like a circle on the screen
 */

#ifdef USR_KEEP_SHAPE
#include <grx/draw.h>
#else
#include <grx/user.h>
#endif

#include "globals.h"
#include "libgrx.h"
#include "usercord.h"

/**
 * grx_user_draw_filled_circle:
 * @xc: the X coordinate of the center of the circle
 * @yc: the Y coordinate of the center of the circle
 * @r: the radius of the circle
 * @c: the color
 *
 * Draws a filled circle on the current context centered at the specified
 * coordinates with the specified radius and color.
 */
void grx_user_draw_filled_circle(int xc,int yc,int r,GrxColor c)
{
#ifdef USR_KEEP_SHAPE
        U2SX(xc,CURC);
        U2SY(yc,CURC);
        SCALE(r,r,CURC->x_max,CURC->user_width);
        grx_draw_filled_circle(xc,yc,r,c);
#else
        grx_user_draw_filled_ellipse(xc,yc,r,r,c);
#endif
}
