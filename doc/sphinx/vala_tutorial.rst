==================
GRX3 Vala Tutorial
==================

Vala is a programming language that compiles to C, so it is a good choice when
performance is an issue. If you have not already, you need to install the Vala
compiler and the GRX3 development files.

On Debian/Ubuntu::

    sudo apt update
    sudo apt install libgrx-3.0-dev valac

On macOS::

    brew install vala


Hello World!
============

The GRX3 library provides an ``Application`` class that is used for taking care
of basic stuff for you. The following template can (and should) be used as the
basis for any GRX3 Vala program. Applications work by creating a main loop
that dispatches events.

.. code-block:: vala

    using Grx;

    class HelloApp : Grx.Application {
        public HelloApp () throws GLib.Error {
            // chain up to the parent class constructor
            Object ();
            // initialize the graphics library
            init ();
            // keep the application from automatically closing
            hold ();
        }

        public override void activate () {
            try {
                clear_screen (Color.WHITE);
                var font = Font.load ("fixed", 24);
                var to = new TextOptions (font, Color.BLACK);
                draw_text ("Hello World!", 10, 10, to);
            }
            catch (GLib.Error e) {
                // Font.load() can thow an exception
                critical ("%s", e.message);
            }
        }

        public override bool event (Event event) {
            // Chain up to the superclass method. The superclass handles
            // application events (EventType.APP_*).
            if (base.event (event)) {
                return true;
            }

            // Quit the application on any key press, button click or touch.
            switch (event.type) {
            case EventType.KEY_DOWN:
            case EventType.BUTTON_PRESS:
            case EventType.TOUCH_DOWN:
                quit ();
                break;
            default:
                return false;
            }

            return true;
        }
    }

    static int main (string [] argv) {
        // In desktop environments, this will be the window title
        Environment.set_application_name ("GRX3 Hello World");
        try {
            // create a new instance of our application
            var app = new HelloApp ();
            // run the main loop
            return app.run ();
        } catch (GLib.Error err) {
            critical ("%s", err.message);
            return 1;
        }
    }



Additional Resources
====================

TODO
