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

let _demo_pattern_context;

/**
 * Gets a blue/red checkerboard pattern.
 */
function get_checkerboard_pixmap() {
    if (_demo_pattern_context == null) {
        _demo_pattern_context = Grx.Context.new(32, 32, null, null);
        let save = Grx.save_current_context(null);
        Grx.set_current_context(_demo_pattern_context);
        Grx.clear_context(Grx.color_get(0, 0, 255));
        Grx.draw_filled_box(0, 0, 16, 16, Grx.color_get(255, 0, 0));
        Grx.draw_filled_box(17, 17, 32, 32, Grx.color_get(255, 0, 0));
        Grx.set_current_context(save);
    }

    return Grx.Pixmap.new_from_context(_demo_pattern_context);
}

let _demo_image;

/**
 * Gets a smiley face image.
 * @param {Number} width 
 * @param {Number} height 
 */
function get_smiley_pixmap(width, height) {
    if (_demo_image == null) {
        let demo_image_context = Grx.Context.new(27, 27, null, null);
        let save = Grx.save_current_context(null);
        Grx.set_current_context(demo_image_context);
        Grx.clear_context(Grx.color_get(0, 0, 255));
        Grx.draw_filled_circle(13, 13, 13, Grx.color_get(255, 255, 0));
        Grx.draw_filled_circle(8, 8, 2, Grx.color_get_black());
        Grx.draw_filled_circle(18, 8, 2, Grx.color_get_black());
        Grx.draw_filled_ellipse_arc(13, 16, 7, 5, 1800, 3600, Grx.ArcStyle.CLOSED_CHORD, Grx.color_get_black());
        Grx.set_current_context(save);

        _demo_image = Grx.Pixmap.new_from_context(demo_image_context);
    }

    return _demo_image.stretch(width, height);
}
