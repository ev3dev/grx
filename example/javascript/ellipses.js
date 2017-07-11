#!/usr/bin/env gjs

// ellipses.js - Draws ellipses
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

function activate() {
    const x = Grx.get_width() / 8;
    const y = Grx.get_height() / 8;
    const r1 = x * 0.8;
    const r2 = y * 0.8;
    const colors = Grx.color_get_ega_colors();
    const white = colors[Grx.EgaColorIndex.WHITE];
    const checker = Demo.get_checkerboard_pixmap();
    const line_opt0 = new Grx.LineOptions();

    // just a plain old ellipse
    Grx.draw_ellipse(x, y, r1, r2, white);
    Grx.draw_filled_ellipse(x, y * 3, r1, r2, white);
    Grx.draw_ellipse_with_pixmap(x, y * 5, r1, r2, line_opt0, checker);
    Grx.draw_filled_ellipse_with_pixmap(x, y * 7, r1, r2, checker);

    // quarter a ellipses
    Grx.draw_ellipse_arc(x * 3, y, r1, r2, 0, 900, Grx.ArcStyle.OPEN, white);
    Grx.draw_filled_ellipse_arc(x * 3, y * 3, r1, r2, 0, 900, Grx.ArcStyle.OPEN, white);
    Grx.draw_ellipse_arc_with_pixmap(x * 3, y * 5, r1, r2, 0, 900, Grx.ArcStyle.OPEN, line_opt0, checker);
    Grx.draw_filled_ellipse_arc_with_pixmap(x * 3, y * 7, r1, r2, 0, 900, Grx.ArcStyle.OPEN, checker);
    Grx.draw_ellipse_arc(x * 3, y, r1, r2, 1800, 2700, Grx.ArcStyle.CLOSED_CHORD, white);
    Grx.draw_filled_ellipse_arc(x * 3, y * 3, r1, r2, 1800, 2700, Grx.ArcStyle.CLOSED_CHORD, white);
    Grx.draw_ellipse_arc_with_pixmap(x * 3, y * 5, r1, r2, 1800, 2700, Grx.ArcStyle.CLOSED_CHORD, line_opt0, checker);
    Grx.draw_filled_ellipse_arc_with_pixmap(x * 3, y * 7, r1, r2, 1800, 2700, Grx.ArcStyle.CLOSED_CHORD, checker);

    // stretchy Pac-Man
    Grx.draw_ellipse_arc(x * 5, y, r1, r2, 450, 3150, Grx.ArcStyle.CLOSED_RADIUS, white);
    Grx.draw_filled_ellipse_arc(x * 5, y * 3, r1, r2, 450, 3150, Grx.ArcStyle.CLOSED_RADIUS, white);
    Grx.draw_ellipse_arc_with_pixmap(x * 5, y * 5, r1, r2, 450, 3150, Grx.ArcStyle.CLOSED_RADIUS, line_opt0, checker);
    Grx.draw_filled_ellipse_arc_with_pixmap(x * 5, y * 7, r1, r2, 450, 3150, Grx.ArcStyle.CLOSED_RADIUS, checker);

    // line options
    const line_opt = new Grx.LineOptions();
    line_opt.color = colors[Grx.EgaColorIndex.GREEN];
    line_opt.width = 1;
    line_opt.n_dash_patterns = 2;
    line_opt.dash_pattern0 = 2;
    line_opt.dash_pattern1 = 2;
    Grx.draw_ellipse_with_options(x * 7, y, r1, r2, line_opt);
    Grx.draw_ellipse_arc_with_options(x * 7, y * 3, r1, r2, 2250, 1350, Grx.ArcStyle.CLOSED_RADIUS, line_opt);
    Grx.draw_ellipse_arc_with_pixmap(x * 7, y * 5, r1, r2, 2250, 1350, Grx.ArcStyle.CLOSED_RADIUS, line_opt, checker);
}

GLib.set_prgname('ellipses.js');
GLib.set_application_name('GRX3 ellipse Drawing Demo');

let app = new Demo.SimpleDemoApp();
app.connect('activate', Lang.bind(this, activate));
app.run(ARGV);
