#!/usr/bin/env python3
#
# boxes.py
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

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx

import demo


def activate(app):
    white = Grx.color_get_white()
    x = Grx.get_width() // 10
    y = Grx.get_height() // 10

    line_opts = Grx.LineOptions()
    line_opts.color = white
    line_opts.width = 3
    line_opts.n_dash_patterns = 2
    line_opts.dash_pattern0 = 6
    line_opts.dash_pattern1 = 4
    line_opts0 = Grx.LineOptions()
    pat = demo.get_checkerboard_pixmap()
    Grx.draw_box(x * 1, y * 1, x * 3, y * 3, white)
    Grx.draw_box_with_options(x * 4, y * 1, x * 6, y * 3, line_opts)
    Grx.draw_box_with_pixmap(x * 7, y * 1, x * 9, y * 3, line_opts0, pat)

    img = demo.get_smiley_pixmap(x * 2, y * 2)
    Grx.draw_filled_box(x * 1, y * 4, x * 3, y * 6, white)
    Grx.draw_filled_box_with_pixmap(x * 4, y * 4, x * 6, y * 6, img)
    Grx.draw_filled_box_with_offset_pixmap(x * 2, y, x * 7, y * 4, x * 9,
                                           y * 6, img)

    frame_colors = Grx.FramedBoxColors()
    frame_colors.background = Grx.color_get(224, 223, 228)
    frame_colors.border_top = Grx.color_get(255, 255, 255)
    frame_colors.border_right = Grx.color_get(155, 157, 154)
    frame_colors.border_bottom = Grx.color_get(110, 110, 100)
    frame_colors.border_left = Grx.color_get(241, 239, 226)
    Grx.draw_framed_box(x * 1, y * 7, x * 3, y * 9, y / 4, frame_colors)
    Grx.draw_rounded_box(x * 4, y * 7, x * 6, y * 9, y / 4, white)
    Grx.draw_filled_rounded_box(x * 7, y * 7, x * 9, y * 9, y / 4, white)

if __name__ == '__main__':
    GLib.set_prgname('boxes.py')
    GLib.set_application_name('GRX3 Boxes Demo')
    app = demo.SimpleDemoApp()
    app.connect('activate', activate)
    app.run()
