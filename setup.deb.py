#!/usr/bin/env python3
"""Install build dependencies"""

from subprocess import check_call

from debian import deb822

s = deb822.Sources(open('debian/control'))
build_deps = [d[0]['name'] for d in s.relations['build-depends']]

check_call(['sudo', 'apt', 'update'])
check_call(['sudo', 'apt', 'install'] + build_deps)
