/*
 * lines.vala
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
    var max_x = get_max_x ();
    var max_y = get_max_y ();
    unowned Color[] colors = Color.get_ega_colors ();
    
    // draw some horizontal lines
    for (var y = 10; y < 20; y++) {
        draw_hline(0, max_x, y, colors[y % 16]);
    }

    // and some vertical lines
    for (var x = 10; x < 20; x++) {
        draw_vline(x, 0, max_y, colors[x % 16]);
    }

    // and angled lines
    for (var x = 30; x < 40; x++) {
        draw_line(20, x, x, 20, colors[x % 16]);
    }

    // then two connected lines at right angles
    for (var x = 50; x < 60; x++) {
        var p0 = Point() { x = 20, y = x };
        var p1 = Point() { x = x,  y = x };
        var p2 = Point() { x = x,  y = 20 };
        draw_polyline({ p0, p1, p2 }, colors[x % 16]);
    }

    // try out line options
    var line_opt = LineOptions () {
        color = Color.WHITE,
        width = 3,
        n_dash_patterns = 3,
        dash_pattern0 = 3,
        dash_pattern1 = 6
    };
    draw_line_with_options (70, 30, 110, 30, line_opt);
    const Point[] points = {
        { 70,  40 },
        { 90,  40 },
        { 90,  50 },
        { 110, 50 }
    };
    draw_polyline_with_options (points, line_opt);

    // and pixmaps
    var checker = get_checkerboard_pixmap ();
    var line_opt0 = LineOptions ();
    draw_line_with_pixmap (70, 60, 110, 60, line_opt0, checker);
    for (var y = 65; y < 70; y ++) {
        draw_hline_with_offset_pixmap (y, 0, 70, y, 40, checker);
    }
}

static int main (string [] args) {
    Environment.set_application_name ("GRX3 Lines Demo");
    try {
        var app = new SimpleDemoApp ();
        app.activate.connect (activate);
        return app.run (args);
    } catch (GLib.Error err) {
        critical ("%s", err.message);
        return 1;
    }
}
