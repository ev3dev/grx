/*
 * demo.vala - common code for simple demos
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

/**
 * Common class used by simple (non-interactive) demos.
 */
class SimpleDemoApp : Grx.Application {
    public SimpleDemoApp () throws GLib.Error {
        Object ();
        init ();
        hold ();
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

static Context? checkerboard_context;

/**
 * Gets a 16x16 pixel checkerboard pattern.
 * 
 * @return      A new pixmap
 */
public Pixmap get_checkerboard_pixmap () {
    if (checkerboard_context == null) {
        checkerboard_context = Context.new (32, 32);
        var save = save_current_context ();
        set_current_context (checkerboard_context);
        clear_context (Color.get (0, 0, 255));
        draw_filled_box (0, 0, 16, 16, Color.get (255, 0, 0));
        draw_filled_box (17, 17, 32, 32, Color.get (255, 0, 0));
        set_current_context (save);
    }
    return Pixmap.new_from_context (checkerboard_context);
}

static Pixmap smiley_image;

/**
 * Gets a smiley face image.
 *
 * @param width     The width of the image
 * @param height    The height of the image
 * @return          A new pixmap
 */
public Pixmap get_smiley_pixmap (int width, int height) {
    if (smiley_image == null) {
        var smiley_image_context = Context.new (27, 27);
        var save = save_current_context ();
        set_current_context (smiley_image_context);
        clear_context (Color.get (0, 0, 255));
        draw_filled_circle (13, 13, 13, Color.get (255, 255, 0));
        draw_filled_circle (8, 8, 2, Color.BLACK);
        draw_filled_circle (18, 8, 2, Color.BLACK);
        draw_filled_ellipse_arc (13, 16, 7, 5, 1800, 3600, ArcStyle.CLOSED_CHORD, Color.BLACK);
        set_current_context (save);

        smiley_image = Pixmap.new_from_context (smiley_image_context);
    }
    return smiley_image.stretch (width, height);
}
