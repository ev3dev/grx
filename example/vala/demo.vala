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
