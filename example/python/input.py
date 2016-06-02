#!/usr/bin/env python3
#
# input.py
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

from gi.repository import GLib
from gi.repository import Grx


class InputApplication(Grx.LinuxConsoleApplication):
    def __init__(self):
        super(Grx.LinuxConsoleApplication, self).__init__()
        self.init()
        self.color = Grx.color_info_get_white()
        self.device_manager = Grx.LibinputDeviceManager.new()
        self.last_touch = None

    def do_activate(self):
        print("application started")

    def do_input_event(self, event):
        t = event.type
        if t == Grx.InputEventType.KEY_DOWN or t == Grx.InputEventType.KEY_UP:
            key_event = event.key
            print ("key", key_event.key)
            if key_event.key in (Grx.Key.Q, Grx.Key.BACKSPACE, Grx.Key.ESCAPE):
                quit()
        elif t == Grx.InputEventType.BUTTON_PRESS or t == Grx.InputEventType.BUTTON_RELEASE:
            button_event = event.button
            print ("button", button_event.button)
            Grx.clear_screen(Grx.color_info_get_black())
        elif t == Grx.InputEventType.BUTTON_DOUBLE_PRESS:
            button_event = event.button
            print ("button double-press", button_event.button)
        elif t == Grx.InputEventType.TOUCH_DOWN:
            touch_event = event.touch
            Grx.draw_pixel(touch_event.x, touch_event.y, self.color)
            self.last_touch = (touch_event.x, touch_event.y)
            print ("touch", touch_event.x, touch_event.y)
        elif t == Grx.InputEventType.TOUCH_MOTION:
            touch_event = event.touch
            Grx.draw_line(self.last_touch[0], self.last_touch[1], touch_event.x, touch_event.y, self.color)
            self.last_touch = (touch_event.x, touch_event.y)

if __name__ == '__main__':
    app = InputApplication();
    app.run()
