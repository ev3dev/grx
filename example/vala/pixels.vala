/*
 * pixels.vala
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
    var w = get_width ();
    unowned Color[] colors = Color.get_ega_colors ();
    var smile = get_smiley_pixmap (32, 32);

    // draw the full width of the screen
    for (var x = 0; x < w; x += 3) {
        // draw a band with colored pixels
        for (var y = 10; y < 30; y += 2) {
            draw_pixel (x + y % 3, y, colors[(y - 1) % 16]);
        }

        // draw a band with pixels from a pixmap
        for (var y = 40; y < 60; y += 2) {
            draw_filled_pixel_with_pixmap (x + y % 3, y, smile);
        }

        // draw a band with pixels from a pixmap, but offset this time
        for (var y = 70; y < 90; y += 2) {
            draw_pixel_with_offset_pixmap (16, 0, x + y % 3, y, smile);
        }
    }
}

static int main (string [] args) {
    Environment.set_application_name ("GRX3 Pixels Demo");
    try {
        var app = new SimpleDemoApp ();
        app.activate.connect (activate);
        return app.run (args);
    } catch (GLib.Error err) {
        critical ("%s", err.message);
        return 1;
    }
}
