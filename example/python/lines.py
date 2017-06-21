#!/usr/bin/env python3
#
# lines.py - Draws random lines
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

import random

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx

BOUNDS = 200
"""How far out of bounds to draw lines"""


class App(Grx.Application):
    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()
        self.hold()

    def do_activate(self):
        """called when the application starts
        overrides Grx.Application.do_activate
        """
        for _ in range(0, 1000):
            x1 = random.randint(-BOUNDS, Grx.get_max_x() + BOUNDS)
            y1 = random.randint(-BOUNDS, Grx.get_max_y() + BOUNDS)
            x2 = random.randint(-BOUNDS, Grx.get_max_x() + BOUNDS)
            y2 = random.randint(-BOUNDS, Grx.get_max_y() + BOUNDS)
            c = Grx.color_alloc(
                random.randint(0, 255),
                random.randint(0, 255),
                random.randint(0, 255))
            Grx.draw_line(x1, y1, x2, y2, c)

    def do_input_event(self, event):
        """called when an input event occurs
        overrides Grx.Application.do_input_event
        """
        if event.type == Grx.EventType.KEY_DOWN:
            self.quit()

if __name__ == '__main__':
    GLib.set_prgname('lines.py')
    GLib.set_application_name('GRX3 Line Drawing Demo')
    app = App()
    app.run()
