#!/usr/bin/env python3
#
# lines.py - Simple paint program
#
# Copyright (c) 2017 David Lechner <david@lechnology.com>
# This file is part of the GRX graphics library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

from collections import OrderedDict

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx


class App(Grx.Application):
    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()  # sets up graphics driver/mode
        self.hold()  # keeps the application running
        self.colors = OrderedDict([
            ('red', Grx.color_get(255, 0, 0)),
            ('green', Grx.color_get(0, 255, 0)),
            ('blue', Grx.color_get(0, 0, 255)),
            ('yellow', Grx.color_get(255, 255, 0)),
            ('cyan', Grx.color_get(0, 255, 255)),
            ('magenta', Grx.color_get(255, 0, 255)),
            ('black', Grx.color_get_black()),
            ('white', Grx.color_get_white()),
        ])
        self.color_lookup = []
        self.fg_color = self.colors['white']
        self.bg_color = self.colors['black']
        self.pen_x, self.pen_y = 0, 0
        self.pen_down = False
        self.color_picker_y = Grx.get_height() // 8

    def do_activate(self):
        """called when the application starts
        overrides Grx.Application.do_activate
        """
        Grx.clear_screen(self.bg_color)

        count = len(self.colors)
        remaining = Grx.get_width()
        x0, y0 = 0, 0
        y1 = self.color_picker_y
        for c in self.colors:
            x1 = x0 + remaining // count
            Grx.draw_filled_box(x0, y0, x1, y1, self.colors[c])
            Grx.draw_box(x0, y0, x1, y1, self.fg_color)
            count -= 1
            remaining -= x1 - x0
            x0 = x1
            self.color_lookup.append(c)
        Grx.set_clip_box(0, y1 + 1, Grx.get_max_x(), Grx.get_max_y())

    def do_event(self, event):
        """called when an input event occurs
        overrides Grx.Application.do_input_event
        """

        # First call the base method to handle APP_* events.
        if Grx.Application.do_event(self, event):
            return True

        # If the event was not handled by the base method, then handle it here
        if (event.type == Grx.EventType.KEY_DOWN and
                event.key.keysym in (Grx.Key.LCASE_Q, Grx.Key.BACK_SPACE,
                                     Grx.Key.ESCAPE)):
            self.quit()
        elif event.type == Grx.EventType.TOUCH_DOWN:
            self.handle_pen_down(event.touch.x, event.touch.y)
        elif event.type == Grx.EventType.TOUCH_MOTION:
            self.handle_pen_move(event.touch.x, event.touch.y)
        elif event.type in (Grx.EventType.TOUCH_UP, Grx.EventType.TOUCH_CANCEL):
            self.handle_pen_up()
        elif event.type == Grx.EventType.BUTTON_PRESS:
            self.handle_pen_down(self.pen_x, self.pen_y)
        elif event.type == Grx.EventType.BUTTON_RELEASE:
            self.handle_pen_up()
        elif event.type == Grx.EventType.POINTER_MOTION:
            self.handle_pen_move(event.motion.x, event.motion.y)
        else:
            return False

        # If there were any other signal handlers, returning True would prevent
        # them from being called (but there aren't any in this app).
        return True

    def handle_pen_down(self, x, y):
        if y <= self.color_picker_y:
            idx = x * len(self.colors) // Grx.get_width()
            self.fg_color = self.colors[self.color_lookup[idx]]
        else:
            self.pen_x, self.pen_y = x, y
            self.pen_down = True
            Grx.draw_pixel(self.pen_x, self.pen_y, self.fg_color)

    def handle_pen_move(self, x, y):
        if self.pen_down:
            Grx.draw_line(self.pen_x, self.pen_y, x, y, self.fg_color)
        self.pen_x, self.pen_y = x, y

    def handle_pen_up(self):
        self.pen_down = False

if __name__ == '__main__':
    GLib.set_prgname('paint.py')
    GLib.set_application_name('GRX3 Paint Program Demo')
    app = App()
    app.run()
