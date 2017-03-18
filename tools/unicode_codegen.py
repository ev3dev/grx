#!/usr/bin/env python3
#
# unicode_codegen.py
#
# Copyright (C) 2017 David Lechner <david@lechnology.com>
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
"""Generate source code files from unicode data"""

import sys


def parse_blocks(blocks):
    """Parse unicode Blocks.txt"""
    block_ranges = {}
    for line in blocks:
        line = line.strip()
        if not len(line) or line[0] == '#':
            continue
        fields = [f.strip() for f in line.split(';')]
        block_name = fields[1].replace(' ', '_').replace('-', '_')
        # map block name to unicode range
        block_ranges[block_name] = fields[0]

    return block_ranges


def parse_props(props):
    """Parse unicode PropertyValueAliases.txt"""
    blocks = {}
    scripts = {}
    for line in props:
        line = line.strip()
        if not len(line) or line[0] == '#':
            continue
        if line[0:3] == 'blk':
            fields = [f.strip() for f in line.split(';')]
            # map script name to block name
            blocks[fields[1]] = fields[2]
        elif line[0:2] == 'sc':
            fields = [f.strip() for f in line.split(';')]
            # map ISO 15924 to script name
            scripts[fields[1]] = fields[2]

    return blocks, scripts


def generate_code(block_ranges, blocks, scripts):
    """Generate C source code file from data"""
    script2chars = {}
    for c, s in scripts.items():
        # Create a mapping from ISO 15924 script code to unicode block.
        # Not all scripts have a mapping.
        block = blocks.get(s)
        if not block:
            continue
        chars = block_ranges.get(block)
        if not chars:
            continue
        c0 = script2chars.setdefault(c[0], {})
        c1 = c0.setdefault(c[1], {})
        c2 = c1.setdefault(c[2], {})
        c2.setdefault(c[3], chars)

    code_lines = []

    def add(indent, line):
        if not line:
            code_lines.append('\n')
        else:
            code_lines.append(' ' * 4 * indent + line + '\n')

    add(0, "#include <fontconfig/fontconfig.h>")
    add(0, "#include <glib.h>")
    add(0, "#include <string.h>")
    add(0, "")
    add(0, "FcCharSet *script_to_charset(const gchar *script)")
    add(0, "{")
    add(1, "FcCharSet *set;")
    add(1, "FcChar32 c;")
    add(1, "")
    add(1, "set = FcCharSetCreate();")
    add(1, "")
    add(1, "g_return_val_if_fail(script != NULL, set);")
    add(1, "g_return_val_if_fail(strlen(script) == 4, set);")
    add(1, "")
    add(1, "switch (script[0]) {")
    for c0 in sorted(script2chars):
        add(2, "case '{}':".format(c0))
        add(3, "switch (script[1]) {")
        for c1 in sorted(script2chars[c0]):
            add(3, "case '{}':".format(c1))
            add(4, "switch (script[2]) {")
            for c2 in sorted(script2chars[c0][c1]):
                add(4, "case '{}':".format(c2))
                add(5, "switch (script[3]) {")
                for c3 in sorted(script2chars[c0][c1][c2]):
                    s = c0 + c1 + c2 + c3
                    b = blocks[scripts[s]]
                    r = script2chars[c0][c1][c2][c3].split('..')
                    add(5, "case '{}':".format(c3))
                    add(6, "// {} - {}".format(s, b))
                    add(6, "for (c = 0x{}; c <= 0x{}; c++) {{".format(r[0], r[1]))
                    add(7, "FcCharSetAddChar(set, c);")
                    add(6, "}")
                    add(6, "break;")
                add(5, "}")
                add(5, "break;")
            add(4, "}")
            add(4, "break;")
        add(3, "}")
        add(3, "break;")
    add(1, "}")
    add(1, "")
    add(1, "return set;")
    add(0, "}")

    return code_lines


def main():
    """Main program entry point"""
    blocks_file_name = sys.argv[1]
    props_file_name = sys.argv[2]
    code_file_name = sys.argv[3]

    with open(blocks_file_name, encoding='utf-8') as f:
        blocks = f.readlines()
    with open(props_file_name, encoding='utf-8') as f:
        props = f.readlines()

    block_ranges = parse_blocks(blocks)
    blocks, scripts = parse_props(props)
    code = generate_code(block_ranges, blocks, scripts)

    with open(code_file_name, 'w', encoding='utf-8') as f:
        f.writelines(code)

if __name__ == '__main__':
    main()
