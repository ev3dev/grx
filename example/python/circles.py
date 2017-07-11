#!/usr/bin/env python3
#
# circles.py - Draws circles
#
# Copyright (c) 2017 David Lechner <david@lechnology.com>
# This file is part of the GRX graphics library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY without even the implied warranty of
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

import demo


def activate(app):
    x = Grx.get_width() // 8
    y = Grx.get_height() // 8
    r = min(x, y) // 2
    colors = Grx.color_get_ega_colors()
    white = colors[Grx.EgaColorIndex.WHITE]
    checker = demo.get_checkerboard_pixmap()
    line_opt0 = Grx.LineOptions()

    # just a plain old circle
    Grx.draw_circle(x, y, r, white)
    Grx.draw_filled_circle(x, y * 3, r, white)
    Grx.draw_circle_with_pixmap(x, y * 5, r, line_opt0, checker)
    Grx.draw_filled_circle_with_pixmap(x, y * 7, r, checker)

    # quarter a circles
    Grx.draw_circle_arc(x * 3, y, r, 0, 900, Grx.ArcStyle.OPEN, white)
    Grx.draw_filled_circle_arc(x * 3, y * 3, r, 0, 900, Grx.ArcStyle.OPEN,
                               white)
    Grx.draw_circle_arc_with_pixmap(x * 3, y * 5, r, 0, 900, Grx.ArcStyle.OPEN,
                                    line_opt0, checker)
    Grx.draw_filled_circle_arc_with_pixmap(x * 3, y * 7, r, 0, 900,
                                           Grx.ArcStyle.OPEN, checker)
    Grx.draw_circle_arc(x * 3, y, r, 1800, 2700, Grx.ArcStyle.CLOSED_CHORD,
                        white)
    Grx.draw_filled_circle_arc(x * 3, y * 3, r, 1800, 2700,
                               Grx.ArcStyle.CLOSED_CHORD, white)
    Grx.draw_circle_arc_with_pixmap(x * 3, y * 5, r, 1800, 2700,
                                    Grx.ArcStyle.CLOSED_CHORD, line_opt0,
                                    checker)
    Grx.draw_filled_circle_arc_with_pixmap(x * 3, y * 7, r, 1800, 2700,
                                           Grx.ArcStyle.CLOSED_CHORD, checker)

    # Pac-Man
    Grx.draw_circle_arc(x * 5, y, r, 450, 3150, Grx.ArcStyle.CLOSED_RADIUS,
                        white)
    Grx.draw_filled_circle_arc(x * 5, y * 3, r, 450, 3150,
                               Grx.ArcStyle.CLOSED_RADIUS, white)
    Grx.draw_circle_arc_with_pixmap(x * 5, y * 5, r, 450, 3150,
                                    Grx.ArcStyle.CLOSED_RADIUS, line_opt0,
                                    checker)
    Grx.draw_filled_circle_arc_with_pixmap(x * 5, y * 7, r, 450, 3150,
                                           Grx.ArcStyle.CLOSED_RADIUS, checker)

    # line options
    line_opt = Grx.LineOptions()
    line_opt.color = colors[Grx.EgaColorIndex.GREEN]
    line_opt.width = 1
    line_opt.n_dash_patterns = 2
    line_opt.dash_pattern0 = 2
    line_opt.dash_pattern1 = 2
    Grx.draw_circle_with_options(x * 7, y, r, line_opt)
    Grx.draw_circle_arc_with_options(x * 7, y * 3, r, 2250, 1350,
                                     Grx.ArcStyle.CLOSED_RADIUS, line_opt)
    Grx.draw_circle_arc_with_pixmap(x * 7, y * 5, r, 2250, 1350,
                                    Grx.ArcStyle.CLOSED_RADIUS, line_opt,
                                    checker)


if __name__ == '__main__':
    GLib.set_prgname('circles.py')
    GLib.set_application_name('GRX3 Circle Drawing Demo')
    app = demo.SimpleDemoApp()
    app.connect('activate', activate)
    app.run()
