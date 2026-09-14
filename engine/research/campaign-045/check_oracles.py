"""Reproduce the authored palette oracle values without game files or a compiler."""
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parent.parent
COLOURS = [(0, 0, 0), (252, 0, 0), (0, 252, 0), (0, 0, 252),
           (252, 252, 0), (252, 0, 252), (0, 252, 252), (252, 252, 252)]
TARGETS = [(0, 0, 0), (255, 128, 63), (17, 93, 211), (252, 252, 252)]

def binary32(value):
    return struct.unpack('<f', struct.pack('<f', value))[0]

def word(value):
    return struct.unpack('<I', struct.pack('<f', value))[0]

def check_array(text, name, expected):
    initializer = text.split(name, 1)[1].split('=', 1)[1].split(';', 1)[0]
    actual = [int(x, 16) for x in re.findall(r'0x([0-9a-fA-F]+)u', initializer)]
    if actual != expected:
        raise ValueError(name + ': literal oracle differs')
    print(name + ': ' + str(len(actual)) + ' authored words reproduced')

mono = []
for target in TARGETS:
    for colour in COLOURS:
        values = [float(x) for x in colour]
        delta = [binary32((t - v) / 63.0) for t, v in zip(target, values)]
        for step in range(63):
            values = [binary32(v + d) for v, d in zip(values, delta)]
            mono.extend(word(v) for v in values)
blend = [word(((2 - numerator) * inner + numerator * outer) / 2.0)
         for numerator in (0, 1, 2, -1, 3)
         for outer_rgb in COLOURS for inner_rgb in COLOURS
         for inner, outer in zip(inner_rgb, outer_rgb)]
for kind, name, values in [('mono', 'mono_expected', mono),
                           ('blend', 'blend_expected', blend)]:
    text = (ROOT / ('campaign-045-colormap-build-' + kind) / 'oracle.h').read_text()
    check_array(text, 'authored_colours', [v for row in COLOURS for v in row])
    check_array(text, name, values)
    if kind == 'mono':
        check_array(text, 'targets', [v for row in TARGETS for v in row])
