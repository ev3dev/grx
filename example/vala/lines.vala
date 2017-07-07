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

class DemoApp : Grx.Application {
    const int BOUNDS = 200; // How far out of bounds to draw lines
    const int COUNT = 1000; // number of lines to draw

    public DemoApp () throws GLib.Error {
        Object ();
        init ();
        hold ();
    }

    public override void activate () {
        var w = get_width ();
        var h = get_height ();
        for (var n = 0; n < COUNT; n++) {
            var x1 = Random.int_range (-BOUNDS, w + BOUNDS);
            var y1 = Random.int_range (-BOUNDS, h + BOUNDS);
            var x2 = Random.int_range (-BOUNDS, w + BOUNDS);
            var y2 = Random.int_range (-BOUNDS, h + BOUNDS);
            var r = (uint8)Random.int_range (0, 256);
            var g = (uint8)Random.int_range (0, 256);
            var b = (uint8)Random.int_range (0, 256);
            var c = Color.alloc(r, g, b);
            draw_line (x1, y1, x2, y2, c);
        }
    }

    public override bool event (Event event) {
        if (base.event (event)) {
            return true;
        }

        switch (event.type) {
        case EventType.KEY_DOWN:
        case EventType.BUTTON_PRESS:
        case EventType.TOUCH_DOWN:
            quit ();
            break;
        default:
            return false;
        }

        return true;
    }
}

static int main (string [] args) {
    Environment.set_application_name ("GRX3 Lines Demo");
    try {
        var app = new DemoApp ();
        return app.run (args);
    } catch (GLib.Error err) {
        critical ("%s", err.message);
        return 1;
    }
}
