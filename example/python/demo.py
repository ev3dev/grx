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


_demo_pattern_context = None


def get_checkerboard_pixmap():
    """Gets a blue/red checkerboard pattern"""
    global _demo_pattern_context
    if not _demo_pattern_context:
        _demo_pattern_context = Grx.Context.new(32, 32)
        save = Grx.save_current_context()
        Grx.set_current_context(_demo_pattern_context)
        Grx.clear_context(Grx.color_get(0, 0, 255))
        Grx.draw_filled_box(0, 0, 16, 16, Grx.color_get(255, 0, 0))
        Grx.draw_filled_box(17, 17, 32, 32, Grx.color_get(255, 0, 0))
        Grx.set_current_context(save)

    return Grx.Pixmap.new_from_context(_demo_pattern_context)


_demo_image = None


def get_smiley_pixmap(width, height):
    """Gets a smiley face image

    Arguments:
    width: the width of the pixmap
    height: the height of the pixmap
    """
    global _demo_image
    if not _demo_image:
        demo_image_context = Grx.Context.new(27, 27)
        save = Grx.save_current_context()
        Grx.set_current_context(demo_image_context)
        Grx.clear_context(Grx.color_get(0, 0, 255))
        Grx.draw_filled_circle(13, 13, 13, Grx.color_get(255, 255, 0))
        Grx.draw_filled_circle(8, 8, 2, Grx.color_get_black())
        Grx.draw_filled_circle(18, 8, 2, Grx.color_get_black())
        Grx.draw_filled_ellipse_arc(13, 16, 7, 5, 1800, 3600,
                                    Grx.ArcStyle.CLOSED_CHORD,
                                    Grx.color_get_black())
        Grx.set_current_context(save)

        _demo_image = Grx.Pixmap.new_from_context(demo_image_context)

    return _demo_image.stretch(width, height)
