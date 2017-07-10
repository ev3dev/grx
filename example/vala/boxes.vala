/*
 * boxes.vala
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
    var x = get_width () / 10;
    var y = get_height () / 10;

    var line_opts = LineOptions () {
        color = Color.WHITE,
        width = 3,
        n_dash_patterns = 2,
        dash_pattern0 = 6,
        dash_pattern1 = 4
    };
    var line_opts0 = LineOptions ();
    var pat = get_checkerboard_pixmap ();
    draw_box (x * 1, y * 1, x * 3, y * 3, Color.WHITE);
    draw_box_with_options (x * 4, y * 1, x * 6, y * 3, line_opts);
    draw_box_with_pixmap (x * 7, y * 1, x * 9, y * 3, line_opts0, pat);

    var img = get_smiley_pixmap (x * 2, y * 2);
    draw_filled_box (x * 1, y * 4, x * 3, y * 6, Color.WHITE);
    draw_filled_box_with_pixmap (x * 4, y * 4, x * 6, y * 6, img);
    draw_filled_box_with_offset_pixmap (x * 2, y, x * 7, y * 4, x * 9, y * 6, img);

    var frame_colors = FramedBoxColors () {
        background = Color.get (224, 223, 228),
        border_top = Color.get (255, 255, 255),
        border_right = Color.get (155, 157, 154),
        border_bottom = Color.get (110, 110, 100),
        border_left = Color.get (241, 239, 226)
    };
    draw_framed_box (x * 1, y * 7, x * 3, y * 9, y / 4, frame_colors);
    draw_rounded_box (x * 4, y * 7, x * 6, y * 9, y / 4, Color.WHITE);
    draw_filled_rounded_box (x * 7, y * 7, x * 9, y * 9, y / 4, Color.WHITE);
}

static int main (string [] args) {
    Environment.set_application_name ("GRX3 Boxes Demo");
    try {
        var app = new SimpleDemoApp ();
        app.activate.connect (activate);
        return app.run (args);
    } catch (GLib.Error err) {
        critical ("%s", err.message);
        return 1;
    }
}
