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

import demo


def activate(app):
    max_x = Grx.get_max_x()
    max_y = Grx.get_max_y()
    colors = Grx.color_get_ega_colors()

    # draw some horizontal lines
    for y in range(10, 20):
        Grx.draw_hline(0, max_x, y, colors[y % 16])

    # and some vertical lines
    for x in range(10, 20):
        Grx.draw_vline(x, 0, max_y, colors[x % 16])

    # and angled lines
    for x in range(30, 40):
        Grx.draw_line(20, x, x, 20, colors[x % 16])

    # then two connected lines at right angles
    for x in range(50, 60):
        points = Grx.generate_points((
            20, x,
            x, x,
            x, 20
        ))
        Grx.draw_polyline(points, colors[x % 16])

    # try out line options
    line_opt = Grx.LineOptions()
    line_opt.color = Grx.color_get_white()
    line_opt.width = 3
    line_opt.n_dash_patterns = 3
    line_opt.dash_pattern0 = 3
    line_opt.dash_pattern1 = 6
    Grx.draw_line_with_options(70, 30, 110, 30, line_opt)
    points = Grx.generate_points((
        70, 40,
        90, 40,
        90, 50,
        110, 50
    ))
    Grx.draw_polyline_with_options(points, line_opt)

    # and pixmaps
    checker = demo.get_checkerboard_pixmap()
    line_opt0 = Grx.LineOptions()
    Grx.draw_line_with_pixmap(70, 60, 110, 60, line_opt0, checker)
    for y in range(65, 70):
        Grx.draw_hline_with_offset_pixmap(y, 0, 70, y, 40, checker)


if __name__ == '__main__':
    GLib.set_prgname('lines.py')
    GLib.set_application_name('GRX3 Line Drawing Demo')
    app = demo.SimpleDemoApp()
    app.connect('activate', activate)
    app.run()
