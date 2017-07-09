#
# demo.py - common module for simple demos
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

import gi
gi.require_version('Grx', '3.0')
from gi.repository import Grx


class SimpleDemoApp(Grx.Application):
    """Base class for simple (non-interactive) demo apps"""
    def __init__(self):
        super(Grx.Application, self).__init__()
        self.init()
        self.hold()

    def do_event(self, event):
        """called when an input event occurs
        overrides Grx.Application.do_event
        """
        if Grx.Application.do_event(self, event):
            return True

        if event.type in (Grx.EventType.KEY_DOWN, Grx.EventType.BUTTON_PRESS,
                          Grx.EventType.TOUCH_DOWN):
            self.quit()
            return True

        return False
