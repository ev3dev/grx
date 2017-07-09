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
    unowned Color[] colors = Color.get_ega_colors ();
    var w = get_width ();
    var h = get_height ();
    var count = w * h / 4;
    for (var n = 0; n < count; n++) {
        var x = Random.int_range (0, w);
        var y = Random.int_range (0, h);
        var c = colors[Random.int_range (0, 16)];
        // fast_draw_* is only safe when we are sure x and y are in bounds
        fast_draw_pixel (x, y, c);
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
