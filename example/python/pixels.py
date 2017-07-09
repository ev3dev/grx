#!/usr/bin/env python3
#
# pixels.py - Draws random pixels
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
import time

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx

from demo import SimpleDemoApp


def activate(app):
    colors = Grx.color_get_ega_colors()
    w = Grx.get_width()
    h = Grx.get_height()
    t = time.monotonic()
    for n in range(0, w*h//4):
        x = int(random.random() * w)
        y = int(random.random() * h)
        if (n & 0xff) == 0:
            # don't change the color so often to speed things up (random is slow)
            c = colors[random.randint(0, 15)]
        # fast_draw_* is only safe when we are sure x and y are in bounds
        Grx.fast_draw_pixel(x, y, c)
    t = time.monotonic() - t
    print('drew', n, 'pixels in', round(t, 3), 'sec')

if __name__ == '__main__':
    GLib.set_prgname('pixels.py')
    GLib.set_application_name('GRX3 Pixel Drawing Demo')
    app = SimpleDemoApp()
    app.connect('activate', activate)
    app.run()
