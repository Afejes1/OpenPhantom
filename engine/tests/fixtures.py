"""Tiny generated PE/COFF fixtures, never derived from a game binary."""
import struct


def pe(code, relocations=(), constants=b"\0\0\0\0"):
    data = bytearray(0x800)
    data[:2] = b"MZ"
    struct.pack_into("<I", data, 60, 0x80)
    data[0x80:0x84] = b"PE\0\0"
    struct.pack_into("<HHIIIHH", data, 0x84, 0x14c, 3, 0, 0, 0, 224, 0x102)
    struct.pack_into("<H", data, 0x98, 0x10b)
    struct.pack_into("<I", data, 0x98 + 28, 0x400000)
    struct.pack_into("<I", data, 0x98 + 92, 16)
    block = b""
    if relocations:
        entries = [0x3000 | offset for offset in relocations]
        if len(entries) % 2:
            entries.append(0)
        block = struct.pack("<II", 0x1000, 8 + 2 * len(entries)) + struct.pack("<" + "H" * len(entries), *entries)
        struct.pack_into("<II", data, 0x98 + 96 + 40, 0x3000, len(block))
    sections = [(b".text", 0x1000, code, 0x200, 0x60000020),
                (b".rdata", 0x2000, constants, 0x400, 0x40000040),
                (b".reloc", 0x3000, block or b"\0" * 4, 0x600, 0x42000040)]
    for i, (name, rva, raw, offset, flags) in enumerate(sections):
        struct.pack_into("<8sIIIIIIHHI", data, 0x178 + i * 40, name, len(raw), rva,
                         len(raw), offset, 0, 0, 0, 0, flags)
        data[offset:offset + len(raw)] = raw
    return bytes(data)


def coff(code, relocations=(), constants=None, symbol="_candidate"):
    constants = constants or {}
    count = 2 if constants else 1
    raw_at = 20 + count * 40
    constant_data = b"".join(constants.values())
    reloc_at = raw_at + len(code) + len(constant_data)
    symbols = [(symbol, 0, 1, 0x20, 2)]
    offset = 0
    for name, value in constants.items():
        symbols.append((name, offset, 2, 0, 3))
        offset += len(value)
    for _, name, _ in relocations:
        if name not in [s[0] for s in symbols]:
            symbols.append((name, 0, 0, 0, 2))
    sym_at = reloc_at + len(relocations) * 10
    strings = bytearray(b"\0" * 4)
    records = bytearray()
    for name, value, section, kind, storage in symbols:
        encoded = name.encode("ascii")
        if len(encoded) > 8:
            encoded = struct.pack("<II", 0, len(strings))
            strings.extend(name.encode("ascii") + b"\0")
        records.extend(struct.pack("<8sIhHBB", encoded, value, section, kind, storage, 0))
    struct.pack_into("<I", strings, 0, len(strings))
    data = bytearray(sym_at)
    struct.pack_into("<HHIIIHH", data, 0, 0x14c, count, 0, sym_at, len(symbols), 0, 0)
    struct.pack_into("<8sIIIIIIHHI", data, 20, b".text", 0, 0, len(code), raw_at,
                     reloc_at, 0, len(relocations), 0, 0x60000020)
    if constants:
        struct.pack_into("<8sIIIIIIHHI", data, 60, b".rdata", 0, 0, len(constant_data),
                         raw_at + len(code), 0, 0, 0, 0, 0x40000040)
    data[raw_at:raw_at + len(code)] = code
    data[raw_at + len(code):reloc_at] = constant_data
    for i, (offset, name, kind) in enumerate(relocations):
        index = next(j for j, sym in enumerate(symbols) if sym[0] == name)
        struct.pack_into("<IIH", data, reloc_at + i * 10, offset, index, kind)
    return bytes(data + records + strings)


def spec(size, bindings=None):
    return {"id": "sample", "address": 0x401000, "size": size, "symbol": "_candidate",
            "source": "src/sample.c", "flags": ["/Od", "/MT"], "bindings": bindings or []}


def target(original, function):
    import hashlib
    return {"schema": 1, "target": "WMAIN", "size": len(original), "image_base": 0x400000,
            "sha256": hashlib.sha256(original).hexdigest(), "functions": [function]}
