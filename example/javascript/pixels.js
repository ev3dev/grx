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
    const w = Grx.get_width();
    const colors = Grx.color_get_ega_colors();
    const smile = Demo.get_smiley_pixmap(32, 32);

    // draw the full width of the screen
    for (let x = 0; x < w; x += 3) {
        // draw a band with colored pixels
        for (let y = 10; y < 30; y += 2) {
            Grx.draw_pixel(x + y % 3, y, colors[(y - 1) % 16]);
        }

        // draw a band with pixels from a pixmap
        for (let y = 40; y < 60; y += 2) {
            Grx.draw_filled_pixel_with_pixmap(x + y % 3, y, smile);
        }

        // draw a band with pixels from a pixmap, but offset this time
        for (let y = 70; y < 90; y += 2) {
            Grx.draw_pixel_with_offset_pixmap(16, 0, x + y % 3, y, smile);
        }
    }
}

GLib.set_prgname('pixels.js');
GLib.set_application_name('GRX3 Pixel Drawing Demo');

let app = new Demo.SimpleDemoApp();
app.connect('activate', Lang.bind(this, activate));
app.run(ARGV);
