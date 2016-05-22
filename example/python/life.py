#!/usr/bin/env python3
#
# Conway's Game of Life
#
# FIXME: This program works, but it is *really* slow!
#

from gi.repository import GLib
from gi.repository import Grx
from random import random


class Life(Grx.LinuxConsoleApplication):
    def __init__(self):
        super(Grx.LinuxConsoleApplication, self).__init__()
        self.init()
        self.source_id = 0
        self.width = Grx.get_size_x()
        self.height = Grx.get_size_y()
        self.old_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.new_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.color = (Grx.color_info_get_black(), Grx.color_info_get_white())

    def on_notify_is_console_active(self, obj, gparamstring):
        """
        This is to handle console switching. Basically, it pauses the program when it is not the active console.
        """
        if self.is_console_active():
            self.source_id = GLib.idle_add(self.draw)
        else:
            GLib.source_remove(self.source_id)

    def do_activate(self):
        self.randomize()
        self.connect("notify::is-console-active", self.on_notify_is_console_active)
        if self.is_console_active():
            self.notify("is-console-active")

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
