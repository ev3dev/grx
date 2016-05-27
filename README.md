GRX 3
=====

GRX 3 is a graphics library for simple graphics displays.

It is developed and maintained by [ev3dev.org](http://www.ev3dev.org) for use
with [LEGO MINDSTORMS EV3](http://mindstorms.lego.com). It also works great
with other displays like Adafruit's PiFTF displays.


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

* Currently, it only runs on Linux.

What display managers does it work with (X11, Wayland, etc.)?

* It doesn't. It currently is used for Linux console applications.


Compiling
---------

Get the code (be aware of the submodule):

    git clone --recursive https://github.com/ev3dev/grx

Get the build dependencies:

    * For now, you have to do this manually:

            head grx/debian/control
            # look for Build-Depends:
            # install the packages listed
            sudo apt-get install <package> <package> ...

    * TODO: once we publish a package, you can use the ev3dev.org package repository:

            sudo apt-add-repository --enable-source "deb http://archive.ev3dev.org/debian jessie main"
            sudo apt-key adv --keyserver pgp.mit.edu --recv-keys 2B210565
            sudo apt-get udpate
            sudo apt-get builddep grx

Now create a build directory, preferably outside of the source directory:

        mkdir build
        cd build

Then build it:

        # If the build directory is not a sibling of the source code directory,
        # replace .. with the actual path to the source code.
        cmake ../grx
        make


Getting Around
--------------

Source code directories:

    addons/   Last remains of some code from GRX 2.x. Not used (but might be useful).
    bindings/ Bindings for other programming languages
    cmake/    Some cmake modules (this is a git submodule)
    debian/   Debian packaging stuff
    doc/      Documentation
    example/  Examples of how to use GRX 3
    fonts/    Font data
    include/  Public header files
    src/      The main library source code
    test/     Old demo program from GRX 2.x

Bugs, support, etc.:

* We're on GitHub. https://github.com/ev3dev.org/grx


License
-------

* See copying.grx for details!
* The library itself is LGPL.
* The tools and examples are GPL.
* Most of the fonts come from X11 and are MIT licensed.

History
-------

GRX is a 2D graphics C library originaly written by Csaba Biegl for
DJ Delorie's DOS port of the GCC compiler in the 1990s.

[GRX 2.x](http://grx.gnu.de/) was actively maintained until about 2012 and was
ported to many different platforms.
