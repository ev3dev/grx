#!/bin/bash
#
# Maintainer script for publishing releases.

set -e

source=$(dpkg-parsechangelog -S Source)
version=$(dpkg-parsechangelog -S Version)

./release-doc.sh ${version} stable

OS=debian DIST=stretch ARCH=amd64 pbuilder-ev3dev build
OS=debian DIST=stretch ARCH=i386 PBUILDER_OPTIONS="--binary-arch" pbuilder-ev3dev build
OS=debian DIST=stretch ARCH=armhf PBUILDER_OPTIONS="--binary-arch" pbuilder-ev3dev build
OS=debian DIST=stretch ARCH=armel PBUILDER_OPTIONS="--binary-arch" pbuilder-ev3dev build
OS=raspbian DIST=stretch ARCH=armhf pbuilder-ev3dev build

debsign ~/pbuilder-ev3dev/debian/stretch-amd64/${source}_${version}_amd64.changes
debsign ~/pbuilder-ev3dev/debian/stretch-i386/${source}_${version}_i386.changes
debsign ~/pbuilder-ev3dev/debian/stretch-armhf/${source}_${version}_armhf.changes
debsign ~/pbuilder-ev3dev/debian/stretch-armel/${source}_${version}_armel.changes
debsign ~/pbuilder-ev3dev/raspbian/stretch-armhf/${source}_${version}_armhf.changes

dput ev3dev-debian ~/pbuilder-ev3dev/debian/stretch-amd64/${source}_${version}_amd64.changes
dput ev3dev-debian ~/pbuilder-ev3dev/debian/stretch-i386/${source}_${version}_i386.changes
dput ev3dev-debian ~/pbuilder-ev3dev/debian/stretch-armhf/${source}_${version}_armhf.changes
dput ev3dev-debian ~/pbuilder-ev3dev/debian/stretch-armel/${source}_${version}_armel.changes
dput ev3dev-raspbian ~/pbuilder-ev3dev/raspbian/stretch-armhf/${source}_${version}_armhf.changes

gbp buildpackage --git-tag-only
