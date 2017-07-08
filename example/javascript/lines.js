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

const BOUNDS = 200; // How far out of bounds to draw lines
const COUNT = 1000; // number of lines to draw

const DemoApp = new Lang.Class({
    Name: "DemoApp",
    Extends: Grx.Application,

    _init: function() {
        this.parent();
        this.init(null);
        this.hold();

        this._colors = Grx.color_get_ega_colors();
        print(this._colors);
        print(this._colors.length);
        print(this._colors[1]);
    },

    vfunc_activate: function() {
        for (let n = 0; n < COUNT; n++) {
            const x1 = Math.random() * (Grx.get_max_x() + (BOUNDS * 2)) - BOUNDS;
            const y1 = Math.random() * (Grx.get_max_y() + (BOUNDS * 2)) - BOUNDS;
            const x2 = Math.random() * (Grx.get_max_x() + (BOUNDS * 2)) - BOUNDS;
            const y2 = Math.random() * (Grx.get_max_y() + (BOUNDS * 2)) - BOUNDS;
            const c = this._colors[Math.floor(Math.random() * 16)];
            Grx.draw_line(x1, y1, x2, y2, c);
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

GLib.set_prgname('lines.js');
GLib.set_application_name('GRX3 Line Drawing Demo');

// Run the application
let app = new DemoApp();
app.run(ARGV);
