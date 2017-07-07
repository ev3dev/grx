====================
GRX3 Python Tutorial
====================

The GRX3 Python bindings are based on `PyGObject`_. So, if you have not already,
you need to install the Python GObject Introspection module.

On Debian/Ubuntu::

    sudo apt update
    sudo apt install python3-gi

On macOS::

    brew install pygobject3


Hello World!
============

The GRX3 library provides an ``Application`` class that is used for taking care
of basic stuff for you. The following template can (and should) be used as the
basis for any GRX3 Python program. Applications work by creating a main loop
that dispatches events.

.. code-block:: python

    #!/usr/bin/env python3

    # Importing GObject Introspection modules is a bit unusual because we have
    # to specify a version in addition to a name. Technically, you can omit
    # gi.require_version(), but you will get a warning.

    import gi
    gi.require_version('GLib', '2.0')
    from gi.repository import GLib
    gi.require_version('Grx', '3.0')
    from gi.repository import Grx

    # We are subclassing Grx.Application to provide our implementation
    class HelloApp(Grx.Application):
        """A Hello World application"""
        def __init__(self):
            # the usual chaining up to the python superclass
            super(Grx.Application, self).__init__()
            # initialize the GRX graphics library
            self.init()
            # keep a hold on the application so that it does not close (this is
            # a feature of GLib.Application).
            self.hold()

        # GLib.Application requires that we implement (override) the activate
        # method.
        def do_activate(self):
            """called when the application starts
            overrides Grx.Application.do_activate
            """
            # fill the entire screen with white
            Grx.clear_screen(Grx.color_get_white())

            font = Grx.Font.load('fixed', 24)
            to = Grx.TextOptions.new(font, Grx.color_get_black())
            Grx.draw_text('Hello World!', 10, 10, to)

        # Handle input events from the GRX library. Returning True means that
        # the event was handled and that any additional signal handlers will
        # not be called.
        def do_event(self, event):
            """called when an input event occurs
            overrides Grx.Application.do_event
            """
            # Chain up to the superclass method. The superclass handles
            # application events (Grx.EventType.APP_*).
            if Grx.Application.do_event(self, event):
                return True

            # Exit the program on key press, mouse button click or touch
            if event.type in (Grx.EventType.KEY_DOWN, Grx.EventType.BUTTON_PRESS,
                              Grx.EventType.TOUCH_DOWN):
                self.quit()
                return True

            return False

    if __name__ == '__main__':
        # This tells GLib what the name of your program will be, otherwise it
        # will be 'python3'.
        GLib.set_prgname('hello.py')
        # This is used by the desktop version of GRX3 to set the window title.
        GLib.set_application_name('GRX3 Hello World!')
        # Create a new instance of our class
        app = HelloApp()
        # Run the main loop
        app.run()


Additional Resources
====================

* `GRX3 Python API <python-api/index.html>`_
* `GRX3 Python examples <https://github.com/ev3dev/grx/tree/ev3dev-stretch/example/python>`_
* `PyGObject`_ documentation
* `PyGObject API Reference <https://lazka.github.io/pgi-docs/>`_
* `Objects section of Python GTK3 tutorial <https://python-gtk-3-tutorial.readthedocs.io/en/latest/objects.html>`_

.. _`PyGobject`: https://pygobject.readthedocs.io/en/latest/
