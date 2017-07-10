#!/usr/bin/env gjs

// boxes.js
//
// Copyright (c) 2017 David Lechner <david@lechnology.com>
// This file is part of the GRX graphics library.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

"use strict";

const GLib = imports.gi.GLib;
const Grx = imports.gi.Grx;
const Lang = imports.lang;

const Demo = imports.demo;

const BOUNDS = 200; // How far out of bounds to draw lines
const COUNT = 1000; // number of lines to draw

function activate() {
    const white = Grx.color_get_white();
    let x = Grx.get_width() / 10;
    let y = Grx.get_height() / 10;

    let line_opts = new Grx.LineOptions();
    line_opts.color = white;
    line_opts.width = 3;
    line_opts.n_dash_patterns = 2;
    line_opts.dash_pattern0 = 6;
    line_opts.dash_pattern1 = 4;
    let line_opts0 = new Grx.LineOptions();
    let pat = Demo.get_checkerboard_pixmap();
    Grx.draw_box(x * 1, y * 1, x * 3, y * 3, white);
    Grx.draw_box_with_options(x * 4, y * 1, x * 6, y * 3, line_opts);
    Grx.draw_box_with_pixmap(x * 7, y * 1, x * 9, y * 3, line_opts0, pat);

    let img = Demo.get_smiley_pixmap(x * 2, y * 2);
    Grx.draw_filled_box(x * 1, y * 4, x * 3, y * 6, white);
    Grx.draw_filled_box_with_pixmap(x * 4, y * 4, x * 6, y * 6, img);
    Grx.draw_filled_box_with_offset_pixmap(x * 2, y, x * 7, y * 4, x * 9, y * 6, img);

    let frame_colors = new Grx.FramedBoxColors();
    frame_colors.background = Grx.color_get(224, 223, 228);
    frame_colors.border_top = Grx.color_get(255, 255, 255);
    frame_colors.border_right = Grx.color_get(155, 157, 154);
    frame_colors.border_bottom = Grx.color_get(110, 110, 100);
    frame_colors.border_left = Grx.color_get(241, 239, 226);
    Grx.draw_framed_box(x * 1, y * 7, x * 3, y * 9, y / 4, frame_colors);
    Grx.draw_rounded_box(x * 4, y * 7, x * 6, y * 9, y / 4, white);
    Grx.draw_filled_rounded_box(x * 7, y * 7, x * 9, y * 9, y / 4, white);
}

GLib.set_prgname('boxes.js');
GLib.set_application_name('GRX3 Boxes Demo');

let app = new Demo.SimpleDemoApp();
app.connect('activate', Lang.bind(this, activate));
app.run(ARGV);
