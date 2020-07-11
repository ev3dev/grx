#!/bin/bash
#
# Maintainer script for publishing releases.

set -e

source=$(dpkg-parsechangelog -S Source)
version=$(dpkg-parsechangelog -S Version)

debuild -d -S

debuild -d -- clean

dput ppa:ev3dev/tools ../${source}_${version}_source.changes

gbp buildpackage --git-tag-only
