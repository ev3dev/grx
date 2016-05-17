#!/usr/bin/env python3
#
# Conway's Game of Life
#
# FIXME: This program works, but it is *really* slow!
#

from gi.repository import GLib
from gi.repository import Grx
from random import random


class Life:
    def __init__(self):
        self.source_id = 0
        self.width = 0
        self.height = 0
        self.old_state = None
        self.new_state = None
        self.color = None

    def run(self):
        main_loop = GLib.MainLoop()

        if not Grx.set_mode_default_graphics(True):
            raise IOError('Could not set graphics mode')
        self.width = Grx.get_size_x()
        self.height = Grx.get_size_y()
        self.old_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.new_state = [[0 for y in range(self.height)] for x in range(self.width)]
        self.color = (Grx.color_info_get_black(), Grx.color_info_get_white())

        Grx.set_screen_active_callback(self.screen_active_handler)
        self.screen_active_handler(Grx.is_screen_active())

        self.randomize()
        main_loop.run()

    def randomize(self):
        for y in range(self.height):
            for x in range(self.width):
                on = random() > 0.82
                self.old_state[x][y] = on
                if on:
                    Grx.draw_point_nc(x, y, self.color[on])

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

        self.old_state, self.new_state = self.new_state, self.old_state

    def draw(self):

        self.update_state()
        for y in range(self.height):
            for x in range(self.width):
                Grx.draw_point_nc(x, y, self.color[self.old_state[x][y]])

        return GLib.SOURCE_CONTINUE

    def screen_active_handler(self, active):
        """
        This is to handle console switching. It basically pauses the drawing loop when this is not the active console.

        :param active: True if the screen is active and we can draw on it
        """
        if active:
            self.source_id = GLib.idle_add(self.draw)
        else:
            GLib.source_remove(self.source_id)

if __name__ == '__main__':
    life = Life()
    life.run()
