#!/usr/bin/env gjs

// pixels.js - Draws random pixels
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
    const colors = Grx.color_get_ega_colors();
    const w = Grx.get_width();
    const h = Grx.get_height();
    const count = w * h / 4;
    for (let n = 0; n < count; n++) {
        const x = Math.random() * w;
        const y = Math.random() * h;
        if ((n & 0xff) == 0) {
            // don't change the color so often to speed things up (random is slow)
            const c = colors[(Math.floor(Math.random() * 16))];
        }
        // fast_draw_* is only safe when we are sure x and y are in bounds
        Grx.fast_draw_pixel(x, y, c);
    }
}

GLib.set_prgname('pixels.js');
GLib.set_application_name('GRX3 Pixel Drawing Demo');

let app = new Demo.SimpleDemoApp();
app.connect('activate', Lang.bind(this, activate));
app.run(ARGV);
