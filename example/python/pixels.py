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

import time

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx

import demo


def activate(app):
    w = Grx.get_width()
    colors = Grx.color_get_ega_colors()
    smile = demo.get_smiley_pixmap(32, 32)

    # draw the full width of the screen
    for x in range(0, w, 3):
        # draw a band with colored pixels
        for y in range(10, 30, 2):
            Grx.draw_pixel(x + y % 3, y, colors[(y - 1) % 16])

        # draw a band with pixels from a pixmap
        for y in range(40, 60, 2):
            Grx.draw_filled_pixel_with_pixmap(x + y % 3, y, smile)

        # draw a band with pixels from a pixmap, but offset this time
        for y in range(70, 90, 2):
            Grx.draw_pixel_with_offset_pixmap(16, 0, x + y % 3, y, smile)


if __name__ == '__main__':
    GLib.set_prgname('pixels.py')
    GLib.set_application_name('GRX3 Pixel Drawing Demo')
    app = demo.SimpleDemoApp()
    app.connect('activate', activate)
    app.run()
