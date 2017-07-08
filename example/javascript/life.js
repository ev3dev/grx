#!/usr/bin/env gjs

// life.js - International text demo
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

        Grx.mouse_set_cursor(null);
        this.source_id = 0
        this.width = Grx.get_width();
        this.height = Grx.get_height();
        this.old_state = new Array(this.width);
        this.new_state = new Array(this.width);
        for (let i = 0; i < this.width; i++) {
            this.old_state[i] = new Array(this.height);
            this.new_state[i] = new Array(this.height);
        }
        this.color = [Grx.color_get_black(), Grx.color_get_white()];
        this.connect("notify::is-active", Lang.bind(this, this.on_notify_is_active));
    },

    vfunc_activate: function() {
        for (let x = 0; x < this.width; x++) {
            let old_row = this.old_state[x];
            for (let y = 0; y < this.height; y++) {
                let on = Number(Math.random() > 0.82);
                old_row[y] = on;
                if (on) {
                    Grx.fast_draw_pixel(x, y, this.color[on]);
                }
            }
        }
    },

    vfunc_event: function(event) {
        if (this.parent(event)) {
            return true;
        }

        const key = event.get_keysym();
        const event_type = event.get_event_type();
        if (this._quit_keys.indexOf(key) >= 0 || this._quit_event_types.indexOf(event_type) >= 0) {
            this.quit();
            return true;
        }

        return false;
    },

    _quit_keys: [Grx.Key.LCASE_Q, Grx.Key.ESCAPE, Grx.Key.BACK_SPACE],
    _quit_event_types: [Grx.EventType.BUTTON_PRESS, Grx.EventType.TOUCH_DOWN],

    on_notify_is_active: function() {
         if (this.is_active) {
            this.source_id = GLib.idle_add(GLib.PRIORITY_DEFAULT_IDLE, Lang.bind(this, this.draw));
         }
         else {
            GLib.source_remove(this.source_id);
         }
    },

    // FIXME: This is a really slow algorithm!
    update_state: function() {
        for (let x = 0; x < this.width; x++) {
            let px = x - 1;
            if (px < 0) {
                px += this.width;
            }
            let nx = x + 1;
            if (nx >= this.width) {
                nx -= this.width;
            }

            let old_row_prev = this.old_state[px];
            let old_row_next = this.old_state[nx];
            let old_row = this.old_state[x];
            let new_row = this.new_state[x];

            for (let y = 0; y < this.height; y++) {
                let py = y - 1;
                if (py < 0) {
                    py += this.height;
                }
                let ny = y + 1;
                if (ny >= this.height) {
                    ny -= this.height;
                }

                let live_count = old_row_prev[py] +
                                 old_row[py] +
                                 old_row_next[py] +
                                 old_row_prev[y] +
                                 old_row_next[y] +
                                 old_row_prev[ny] +
                                 old_row[ny] +
                                 old_row_next[ny];
                new_row[y] = Number((live_count | old_row[y]) == 3);
            }
        }
    },

    draw: function() {
        this.update_state();
        for (let x = 0; x < this.width; x++) {
            let new_row = this.new_state[x];
            let old_row = this.old_state[x];
            for (let y = 0; y < this.height; y++) {
                let new_state = new_row[y];
                if (old_row[y] != new_state) {
                    Grx.fast_draw_pixel(x, y, this.color[new_state]);
                }
            }
        }

        [this.old_state, this.new_state] = [this.new_state, this.old_state];

        return GLib.SOURCE_CONTINUE;
    }
});

GLib.set_prgname('life.js');
GLib.set_application_name('GRX3 Game of Life Demo');

// Run the application
let app = new DemoApp();
app.run(ARGV);
