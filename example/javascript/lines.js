#!/usr/bin/env gjs

// lines.js - Draws random lines
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
    const max_x = Grx.get_max_x();
    const max_y = Grx.get_max_y();
    const colors = Grx.color_get_ega_colors();

    // draw some horizontal lines
    for (let y = 10; y < 20; y++) {
        Grx.draw_hline(0, max_x, y, colors[y % 16]);
    }

    // and some vertical lines
    for (let x = 10; x < 20; x++) {
        Grx.draw_vline(x, 0, max_y, colors[x % 16]);
    }

    // and angled lines
    for (let x = 30; x < 40; x++) {
        Grx.draw_line(20, x, x, 20, colors[x % 16]);
    }

    // then two connected lines at right angles
    for (let x = 50; x < 60; x++) {
        let p0 = new Grx.Point();
        [p0.x, p0.y] = [20, x];
        let p1 = new Grx.Point();
        [p1.x, p1.y] = [x, x];
        let p2 = new Grx.Point();
        [p2.x, p2.y] = [x, 20];
        // FIXME: there seems to be a gjs bug that prevents this from actually
        // drawing something
        Grx.draw_polyline([p0, p1, p2], colors[x % 16]);
    }

    // try out line options
    let line_opt = new Grx.LineOptions();
    line_opt.color = Grx.color_get_white();
    line_opt.width = 3;
    line_opt.n_dash_patterns = 3;
    line_opt.dash_pattern0 = 3;
    line_opt.dash_pattern1 = 6;
    Grx.draw_line_with_options(70, 30, 110, 30, line_opt);
    // FIXME: gjs array bug - this crashes
    // let points = Grx.generate_points([
    //     70, 40,
    //     90, 40,
    //     90, 50,
    //     110, 50
    // ]);
    // Grx.draw_polyline_with_options(points, line_opt);

    // and pixmaps
    let checker = Demo.get_checkerboard_pixmap();
    let line_opt0 = new Grx.LineOptions();
    Grx.draw_line_with_pixmap(70, 60, 110, 60, line_opt0, checker);
    for (let y = 65; y < 70; y ++) {
        Grx.draw_hline_with_offset_pixmap(y, 0, 70, y, 40, checker);
    }
}

GLib.set_prgname('lines.js');
GLib.set_application_name('GRX3 Line Drawing Demo');

let app = new Demo.SimpleDemoApp();
app.connect('activate', Lang.bind(this, activate));
app.run(ARGV);
