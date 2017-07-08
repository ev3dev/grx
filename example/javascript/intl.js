#!/usr/bin/env gjs

// intl.js - International text demo
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

const FONT_TABLE = [
        // [<language code>, <script code>, <hello world>]
        ['en', null, 'Hello world'],
        ['zh-cn', null, '你好，世界'],
        ['ja', null, 'こんにちは世界'],
        ['ko', null, '안녕 세상'],
        ['ru', null, 'Привет мир'],
        ['el', null, 'Γειά σου Κόσμε'],
        ['vi', null, 'Chào thế giới'],
        ['hi', null, 'नमस्ते दुनिया'],
        ['ka', null, 'გამარჯობა მსოფლიო'],
        [null, 'Runr', 'ᚻᛖᛚᛚᚩ ᚹᚩᚱᛚᛞ'],
];
const FONT_SIZE = 12;

const DemoApp = new Lang.Class({
    Name: "DemoApp",
    Extends: Grx.Application,

    _init: function() {
        this.parent();
        this.init(null);
        this.hold();

        this.WHITE = Grx.color_get_white();
        this.BLACK = Grx.color_get_black();
        this.default_text_opt = new Grx.TextOptions(Grx.font_load('lucida', 10), this.BLACK);
    },

    vfunc_activate: function() {
        Grx.clear_screen(this.WHITE);
        try {
            let offset = 0;
            for (let item of FONT_TABLE) {
                let font = Grx.Font.load_full(null, FONT_SIZE, Grx.get_dpi(), Grx.FontWeight.REGULAR,
                    Grx.FontSlant.REGULAR, Grx.FontWidth.REGULAR, false, item[0], item[1]);
                let text_opt = Grx.TextOptions.new_full(
                    font, this.BLACK, this.WHITE, Grx.TextHAlign.CENTER, Grx.TextVAlign.TOP);
                Grx.draw_text(item[2], Grx.get_width() / 2, offset, text_opt);
                offset += font.get_height();
            }
        }
        catch (e) {
            Grx.draw_text(e.message, 10, 10, this.default_text_opt);
            logError(e);
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

GLib.set_prgname('intl.js');
GLib.set_application_name('GRX3 International Hello World Demo');

// Run the application
let app = new DemoApp();
app.run(ARGV);
