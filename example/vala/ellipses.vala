/*
 * ellipses.vala
 *
 * Copyright (c) 2017 David Lechner <david@lechnology.com>
 * This file is part of the GRX graphics library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

using Grx;

static void activate () {
    var x = get_width () / 8;
    var y = get_height () / 8;
    var r1 = x * 8 / 10;
    var r2 = y * 8 / 10;
    unowned Color[] colors = Color.get_ega_colors ();
    var white = colors[EgaColorIndex.WHITE];
    var checker = get_checkerboard_pixmap ();
    var line_opt0 = LineOptions ();

    // just a plain old ellipse
    draw_ellipse (x, y, r1, r2, white);
    draw_filled_ellipse (x, y * 3, r1, r2, white);
    draw_ellipse_with_pixmap (x, y * 5, r1, r2, line_opt0, checker);
    draw_filled_ellipse_with_pixmap (x, y * 7, r1, r2, checker);

    // quarter a ellipses
    draw_ellipse_arc (x * 3, y, r1, r2, 0, 900, ArcStyle.OPEN, white);
    draw_filled_ellipse_arc (x * 3, y * 3, r1, r2, 0, 900, ArcStyle.OPEN, white);
    draw_ellipse_arc_with_pixmap (x * 3, y * 5, r1, r2, 0, 900, ArcStyle.OPEN, line_opt0, checker);
    draw_filled_ellipse_arc_with_pixmap (x * 3, y * 7, r1, r2, 0, 900, ArcStyle.OPEN, checker);
    draw_ellipse_arc (x * 3, y, r1, r2, 1800, 2700, ArcStyle.CLOSED_CHORD, white);
    draw_filled_ellipse_arc (x * 3, y * 3, r1, r2, 1800, 2700, ArcStyle.CLOSED_CHORD, white);
    draw_ellipse_arc_with_pixmap (x * 3, y * 5, r1, r2, 1800, 2700, ArcStyle.CLOSED_CHORD, line_opt0, checker);
    draw_filled_ellipse_arc_with_pixmap (x * 3, y * 7, r1, r2, 1800, 2700, ArcStyle.CLOSED_CHORD, checker);

    // stretchy Pac-Man
    draw_ellipse_arc (x * 5, y, r1, r2, 450, 3150, ArcStyle.CLOSED_RADIUS, white);
    draw_filled_ellipse_arc (x * 5, y * 3, r1, r2, 450, 3150, ArcStyle.CLOSED_RADIUS, white);
    draw_ellipse_arc_with_pixmap (x * 5, y * 5, r1, r2, 450, 3150, ArcStyle.CLOSED_RADIUS, line_opt0, checker);
    draw_filled_ellipse_arc_with_pixmap (x * 5, y * 7, r1, r2, 450, 3150, ArcStyle.CLOSED_RADIUS, checker);

    // line options
    var line_opt = LineOptions ();
    line_opt.color = colors[EgaColorIndex.GREEN];
    line_opt.width = 1;
    line_opt.n_dash_patterns = 2;
    line_opt.dash_pattern0 = 2;
    line_opt.dash_pattern1 = 2;
    draw_ellipse_with_options (x * 7, y, r1, r2, line_opt);
    draw_ellipse_arc_with_options (x * 7, y * 3, r1, r2, 2250, 1350, ArcStyle.CLOSED_RADIUS, line_opt);
    draw_ellipse_arc_with_pixmap (x * 7, y * 5, r1, r2, 2250, 1350, ArcStyle.CLOSED_RADIUS, line_opt, checker);

    // reusable points
    var points = generate_ellipse (x * 7, y * 7, r1, r2);
    draw_polygon (points, white);
}

static int main (string [] args) {
    Environment.set_application_name ("GRX3 ellipse Drawing Demo");
    try {
        var app = new SimpleDemoApp ();
        app.activate.connect (activate);
        return app.run (args);
    } catch (GLib.Error err) {
        critical ("%s", err.message);
        return 1;
    }
}
