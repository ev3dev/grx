#!/usr/bin/env python3
#
# life.py - Conway's Game of Life
#
# Copyright (c) 2016 David Lechner <david@lechnology.com>
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

import gi
gi.require_version('GLib', '2.0')
from gi.repository import GLib
gi.require_version('Grx', '3.0')
from gi.repository import Grx

from random import random

#
# FIXME: This program works, but it is *really* slow!
#


class Life(Grx.Application):
    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()
        self.hold()
        Grx.mouse_set_cursor(None)
        self.source_id = 0
        self.width = Grx.get_width()
        self.height = Grx.get_height()
        self.old_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.new_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.color = (Grx.color_get_black(), Grx.color_get_white())
        self.connect("notify::is-active", self.on_notify_is_active)

    def on_notify_is_active(self, obj, gparamstring):
        """
        This is to handle console switching. Basically, it pauses the program
        when it is not the active console.
        """
        if self.is_active():
            self.source_id = GLib.idle_add(self._draw)
        else:
            GLib.source_remove(self.source_id)

    def do_activate(self):
        """This function overrides Grx.Application.activate"""
        for x in range(self.width):
            for y in range(self.height):
                on = random() > 0.82
                self.old_state[x][y] = on
                if on:
                    Grx.fast_draw_pixel(x, y, self.color[on])

    def do_input_event(self, event):
        """This function overrides Grx.Application.input_event"""
        if event.type != Grx.EventType.POINTER_MOTION:
            self.quit()

    def _update_state(self):
        for x in range(self.width):
            px = x - 1
            if px < 0:
                px += self.width
            nx = x + 1
            if nx >= self.width:
                nx -= self.width

            old_row_prev = self.old_state[px]
            old_row_next = self.old_state[nx]
            old_row = self.old_state[x]
            new_row = self.new_state[x]

            for y in range(self.height):
                py = y - 1
                if py < 0:
                    py += self.height
                ny = y + 1
                if ny >= self.height:
                    ny -= self.height

                live_count = (old_row_prev[py] +
                              old_row[py] +
                              old_row_next[py] +
                              old_row_prev[y] +
                              old_row_next[y] +
                              old_row_prev[ny] +
                              old_row[ny] +
                              old_row_next[ny])
                new_row[y] = (live_count | old_row[y]) == 3

    def _draw(self):
        self._update_state()
        for x in range(self.width):
            new_row = self.new_state[x]
            old_row = self.old_state[x]
            for y in range(self.height):
                if old_row[y] != new_row[y]:
                    Grx.fast_draw_pixel(x, y, self.color[new_row[y]])

        self.old_state, self.new_state = self.new_state, self.old_state

        return GLib.SOURCE_CONTINUE

if __name__ == '__main__':
    GLib.set_prgname('life.py')
    GLib.set_application_name('GRX3 Game of Life Demo')
    life = Life()
    life.run()
