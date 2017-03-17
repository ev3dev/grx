#!/usr/bin/env python3
#
# fonts.py - Look at all the pretty fonts
#
# Copyright (c) 2017 David Lechner <david@lechnology.com>
# This file is part of the GRX graphics library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

import os
from subprocess import check_output

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx


class App(Grx.Application):
    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()
        self.WHITE = Grx.color_info_get_white()
        self.BLACK = Grx.color_info_get_black()
        fc_list = check_output(['fc-list', '--format=%{file}\n', ':scalable=False'])
        self.font_files = [f for f in fc_list.decode().splitlines()]
        self.font_files.sort()
        self.default_font = Grx.TextOptions.new(
            # Grx.font_load('lucida', 10),
            # Don't want to use the dpi-aware font here so we can cram info on to small screens
            Grx.font_load_full('lucida', 10, -1, Grx.FontWeight.REGULAR,
                               Grx.FontSlant.REGULAR, Grx.FontWidth.REGULAR, False, None),
            self.BLACK, self.WHITE,
            Grx.TextHAlign.LEFT, Grx.TextVAlign.TOP)
        self.v_offset = self.default_font.get_font().get_height()
        self.font_index = 0
        self.next_index = 0
        self.prev_index = []

    def do_activate(self):
        self.show_next_font(0, 0)

    def do_input_event(self, event):
        if event.type == Grx.EventType.KEY_DOWN:
            if event.key.keysym in (Grx.KEY_q, Grx.KEY_BackSpace, Grx.KEY_Escape):
                self.quit()
            elif event.key.keysym in (Grx.KEY_Down, Grx.KEY_m):
                if self.next_index != -1:
                    self.show_next_font(0, self.next_index)
            elif event.key.keysym == Grx.KEY_Up:
                if len(self.prev_index) >= 2 or (len(self.prev_index) == 1 and self.next_index == -1):
                    if self.next_index != -1:
                        self.prev_index.pop()
                    self.show_next_font(0, self.prev_index.pop())
            elif event.key.keysym == Grx.KEY_Home:
                self.show_next_font(-len(self.font_files), 0)
            elif event.key.keysym == Grx.KEY_End:
                self.show_next_font(len(self.font_files), 0)
            elif event.key.keysym == Grx.KEY_Page_Up:
                self.show_next_font(-10, 0)
            elif event.key.keysym == Grx.KEY_Page_Down:
                self.show_next_font(10, 0)
            elif event.key.keysym == Grx.KEY_Left:
                self.show_next_font(-1, 0)
            else:
                self.show_next_font(1, 0)

    def show_next_font(self, step, start_index):
        if step:
            self.prev_index.clear()
            self.font_index += step
            if self.font_index < 0:
                self.font_index = 0
            elif self.font_index >= len(self.font_files):
                self.font_index = len(self.font_files) - 1
        Grx.clear_screen(self.WHITE)
        file_name = self.font_files[self.font_index]
        Grx.draw_text('file: ' + file_name, 10, 10, self.default_font)
        try:
            font = Grx.font_load_from_file(file_name)
            font_info = 'family: {}, style: {}, width: {}, height {}'.format(font.get_family(), font.get_style(), font.get_width(), font.get_height())
            Grx.draw_text(font_info, 10, 10 + self.v_offset, self.default_font)
            dump_context = Grx.Context.new_subcontext(10, 10 + self.v_offset * 2, Grx.get_width() - 10, Grx.get_height() - 10)
            self.next_index = font.dump(dump_context, start_index, self.BLACK, self.WHITE)
            if self.next_index != -1 and self.next_index != start_index:
                self.prev_index.append(start_index)
        except GLib.Error as err:
            Grx.draw_text(str(err), 10, 10 + self.v_offset, self.default_font, self.BLACK, self.WHITE, Grx.TextHAlign.LEFT, Grx.TextVAlign.TOP)

if __name__ == '__main__':
    app = App()
    app.run()
