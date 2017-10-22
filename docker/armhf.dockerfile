FROM ev3dev/debian-stretch-armhf-cross

RUN sudo apt-get update && \
    DEBIAN_FRONTEND=noninteractive sudo apt-get install --yes --no-install-recommends \
        cmake \
        gir1.2-glib-2.0 \
        gobject-introspection \
        libfontconfig1-dev \
        libfreetype6-dev \
        libgirepository1.0-dev \
        libglib2.0-dev \
        libgudev-1.0-dev \
        libinput-dev \
        libjpeg-dev \
        libpng-dev \
        libudev-dev \
        libxkbcommon-dev \
        pkg-config \
        valac
