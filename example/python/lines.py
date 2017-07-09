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

from demo import SimpleDemoApp

BOUNDS = 200
"""How far out of bounds to draw lines"""


def activate(app):
    """called when the application starts
    overrides Grx.Application.do_activate
    """
    colors = Grx.color_get_ega_colors()
    for _ in range(0, 1000):
        x1 = random.randint(-BOUNDS, Grx.get_max_x() + BOUNDS)
        y1 = random.randint(-BOUNDS, Grx.get_max_y() + BOUNDS)
        x2 = random.randint(-BOUNDS, Grx.get_max_x() + BOUNDS)
        y2 = random.randint(-BOUNDS, Grx.get_max_y() + BOUNDS)
        c = colors[random.randint(0, 15)]
        Grx.draw_line(x1, y1, x2, y2, c)

if __name__ == '__main__':
    GLib.set_prgname('lines.py')
    GLib.set_application_name('GRX3 Line Drawing Demo')
    app = SimpleDemoApp()
    app.connect('activate', activate)
    app.run()
