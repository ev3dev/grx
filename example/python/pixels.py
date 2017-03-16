#!/usr/bin/env python3
#
# lines.py - Draws random pixels
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
gi.require_version('Grx', '3.0')
from gi.repository import Grx


class App(Grx.Application):
    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()

    def do_activate(self):
        """called when the application starts
        overrides Grx.Application.do_activate
        """
        for _ in range(0, Grx.get_max_x()*Grx.get_max_y()//4):
            x = random.randint(0, Grx.get_max_x())
            y = random.randint(0, Grx.get_max_y())
            c = Grx.color_info_alloc_color(
                random.randint(0, 255),
                random.randint(0, 255),
                random.randint(0, 255))
            Grx.draw_pixel(x, y, c)

    def do_input_event(self, event):
        """called when an input event occurs
        overrides Grx.Application.do_input_event
        """
        if event.type == Grx.EventType.KEY_DOWN:
            self.quit()

if __name__ == '__main__':
    app = App()
    app.run()
