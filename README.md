GRX 3
=====

GRX 3 is a graphics library for simple graphics displays. It also includes
keyboard, mouse, joystick and touchscreen input support.

It is developed and maintained by [ev3dev.org](http://www.ev3dev.org) for use
with [LEGO MINDSTORMS EV3](http://mindstorms.lego.com). It also works great
with other displays like Adafruit's PiTFT displays.


About/FAQ
---------

Why revive an old graphics library instead of using a "modern" library like SDL?

* Other libraries do not handle 1-bit-per-pixel displays (at least not very well).
  LEGO MINDSTORMS EV3 has a monochrome display, so this is a necessity.
* Other libraries rely on floating point heavily. GRX 3 uses integers mostly.
  This is important for performance because LEGO MINDSTORMS EV3 does not have
  a hardware floating point processor.

Why use GLib?

* Using GLib allows other programming languages to use this library with minimal
  effort using [GObject Introspection].

[GObject Introspection]: https://wiki.gnome.org/Projects/GObjectIntrospection

What OSes does it run on?

* Currently, it runs on Linux and macOS.


Getting Around
--------------

On the web:

* Docs: http://docs.ev3dev.org/projects/grx/
* Bugs/Questions: https://github.com/ev3dev/grx/issues/
* Wiki: https://github.com/ev3dev/grx/wiki/


Source code directories:

    addons/     Last remains of some code from GRX 2.x - not used (but might be useful)
    bindings/   Bindings for other programming languages
    cmake/      Some CMake modules (this is a git submodule)
    data/       Various data files
    doc/        Documentation
    docker/     Dockerfiles for cross-compiling
    example/    Examples of how to use GRX 3
    include/    Public header files
    plugins/    Pluggable components, such as video drivers
    scripts/    Scripts used for development
    src/        The main library source code
    test/       Old demo/test program from GRX 2.x
    tools/      Utility programs

License
-------

* See copying.grx for details!
* The library itself is LGPL.
* The tools and examples are GPL.

History
-------

GRX was originally written in the 1990s by Csaba Biegl for
DJ Delorie's DOS port of the GCC compiler.

[GRX 2.x](http://grx.gnu.de/) was actively maintained until about 2012 and was
ported to many different platforms.
