//
// demo.js - common module for simple demos
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

const Grx = imports.gi.Grx;
const Lang = imports.lang;

const SimpleDemoApp = new Lang.Class({
    Name: "SimpleDemoApp",
    Extends: Grx.Application,

    _init: function() {
        this.parent();
        this.init(null);
        this.hold();
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

    _quit_event_types: [
        Grx.EventType.KEY_DOWN,
        Grx.EventType.BUTTON_PRESS,
        Grx.EventType.TOUCH_DOWN
    ]
});
