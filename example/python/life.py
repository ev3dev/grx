#!/usr/bin/env python3
#
# life.vala - Conway's Game of Life
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
        self.source_id = 0
        self.width = Grx.get_width()
        self.height = Grx.get_height()
        self.old_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.new_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.color = (Grx.color_info_get_black(), Grx.color_info_get_white())
        self.connect("notify::is-active", self.on_notify_is_active)

    def on_notify_is_active(self, obj, gparamstring):
        """
        This is to handle console switching. Basically, it pauses the program when it is not the active console.
        """
        if self.is_active():
            self.source_id = GLib.idle_add(self.draw)
        else:
            GLib.source_remove(self.source_id)

    def do_activate(self):
        self.randomize()

    def do_input_event(self, event):
        self.quit()

    def randomize(self):
        for y in range(self.height):
            for x in range(self.width):
                on = random() > 0.82
                self.old_state[x][y] = on
                if on:
                    Grx.fast_draw_pixel(x, y, self.color[on])

    def update_state(self):
        for y in range(self.height):
            for x in range(self.width):
                px = x - 1
                if px < 0:
                    px += self.width
                nx = x + 1
                if nx >= self.width:
                    nx -= self.width
                py = y - 1
                if py < 0:
                    py += self.height
                ny = y + 1
                if ny >= self.height:
                    ny -= self.height
                live_count = (self.old_state[px][py] +
                              self.old_state[x][py] +
                              self.old_state[nx][py] +
                              self.old_state[px][y] +
                              self.old_state[nx][y] +
                              self.old_state[px][ny] +
                              self.old_state[x][ny] +
                              self.old_state[nx][ny])
                self.new_state[x][y] = (live_count | self.old_state[x][y]) == 3

    def draw(self):
        self.update_state()
        for y in range(self.height):
            for x in range(self.width):
                if self.old_state[x][y] != self.new_state[x][y]:
                    Grx.fast_draw_pixel(x, y, self.color[self.new_state[x][y]])

        self.old_state, self.new_state = self.new_state, self.old_state

        return GLib.SOURCE_CONTINUE

if __name__ == '__main__':
    life = Life()
    life.run()
