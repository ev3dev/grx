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

const DemoApp = new Lang.Class({
    Name: "DemoApp",
    Extends: Grx.Application,

    _init: function() {
        this.parent();
        this.init(null);
        this.hold();

        this._colors = Grx.color_alloc_ega_colors();
    },

    vfunc_activate: function() {
        const w = Grx.get_width();
        const h = Grx.get_height();
        const count = w * h / 4;
        for (let n = 0; n < count; n++) {
            const x = Math.random() * w;
            const y = Math.random() * h;
            if ((n & 0xff) == 0) {
                // don't change the color so often to speed things up (random is slow)
                const c = this._colors[(Math.floor(Math.random() * 16))];
            }
            // fast_draw_* is only safe when we are sure x and y are in bounds
            Grx.fast_draw_pixel(x, y, c);
        }
    },

    vfunc_event: function(event) {
        if (this.parent(event)) {
            return true;
        }

        const event_type = event.get_event_type();
        if (this._quit_event_types.indexOf(event_type) >= 0) {
            this.quit();
            return true;
        }

        return false;
    },

    _quit_event_types: [Grx.EventType.KEY_DOWN, Grx.EventType.BUTTON_PRESS, Grx.EventType.TOUCH_DOWN]
});

GLib.set_prgname('pixels.js');
GLib.set_application_name('GRX3 Pixel Drawing Demo');

// Run the application
let app = new DemoApp();
app.run(ARGV);
