==============================
GRX3 Javascript (gjs) Tutorial
==============================

In order to use GRX3 with Javascript, you need a Javascript engine called
`gjs`_. It is based on the Mozilla Spidermonkey engine (sorry for those hoping
for node.js). If you haven't already, you will need to install ``gjs``.

.. _`gjs`: https://wiki.gnome.org/Projects/Gjs

On Debian/Ubuntu::

    sudo apt update
    sudo apt install gjs

On macOS::

    brew install gjs


Hello World!
============

The GRX3 library provides an ``Application`` class that is used for taking care
of basic stuff for you. The following template can (and should) be used as the
basis for any GRX3 Javascript program. Applications work by creating a main loop
that dispatches events. The ``gjs`` engine also provides some infrastructure
for emulating OOP with Javascript.

.. code-block:: javascript

    #!/usr/bin/env gjs

    "use strict";

    const GLib = imports.gi.GLib;
    const Grx = imports.gi.Grx;
    const Lang = imports.lang;

    // We are subclassing Grx.Application to provide our implementation
    const HelloApp = new Lang.Class({
        Name: "HelloApp",
        Extends: Grx.Application,

        _init: function() {
            // chain up to the superclass constructor
            this.parent();
            // initialize the graphics library
            this.init(null);
            // keep the application from automatically closing
            this.hold();
        },

        // override the superclass activate method
        vfunc_activate: function() {
            Grx.clear_screen(Grx.color_get_white());
            const font = Grx.Font.load('fixed', 24)
            const to = Grx.TextOptions.new(font, Grx.color_get_black());
            Grx.draw_text('Hello World!', 10, 10, to);
        },

        // override the superclass event method
        vfunc_event: function(event) {
            // Chain up to the superclass event method. This handles application
            // events (Grx.EventType.APP_*).
            if (this.parent(event)) {
                return true;
            }

            // Quit the application on key press, button click or touch
            const event_type = event.get_event_type();
            if (this._quit_event_types.indexOf(event_type) >= 0) {
                this.quit();
                return true;
            }

            return false;
        },

        _quit_event_types: [Grx.EventType.KEY_DOWN, Grx.EventType.BUTTON_PRESS, Grx.EventType.TOUCH_DOWN]
    });

    // Set the program name, otherwise it will use 'gjs'
    GLib.set_prgname('hello.js');
    // In desktop applications, this will be the window title
    GLib.set_application_name('GRX3 Hello World!');

    // Create a new instance of our class
    let app = new HelloApp();
    // run the main loop
    app.run(ARGV);



Additional Resources
====================

* `GRX3 Javascript API <gjs-api/index.html>`_
* `GRX3 Javascript examples <https://github.com/ev3dev/grx/tree/ev3dev-stretch/example/javascript>`_
* `Gjs`_ documentation
