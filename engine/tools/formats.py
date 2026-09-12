"""Read-only, bounded PE32 and ordinary x86 COFF readers for the first batch.

Unsupported layouts fail closed. No disassembly, extraction, or file rewriting.
"""
from dataclasses import dataclass
from pathlib import Path
import struct


class VerificationError(ValueError):
    pass


def require(condition, message):
    if not condition:
        raise VerificationError(message)


def span(data, offset, size):
    require(offset >= 0 and size >= 0 and offset + size <= len(data),
            "range outside input")
    return data[offset:offset + size]


def unpack(fmt, data, offset):
    return struct.unpack(fmt, span(data, offset, struct.calcsize(fmt)))


def read_binary(path):
    path = Path(path)
    require(path.stat().st_size <= 64 * 1024 * 1024, "input exceeds 64 MiB limit")
    data = path.read_bytes()
    require(len(data) <= 64 * 1024 * 1024, "input changed beyond size limit")
    return data


def cstring(data, offset):
    require(0 <= offset < len(data), "string offset outside table")
    end = data.find(b"\0", offset)
    require(end >= 0, "unterminated string")
    try:
        return data[offset:end].decode("ascii")
    except UnicodeDecodeError as exc:
        raise VerificationError("non-ASCII COFF name") from exc


@dataclass
class Section:
    name: str
    virtual_size: int
    address: int
    size: int
    offset: int
    reloc_offset: int
    reloc_count: int
    flags: int


def sections(data, start, count):
    require(0 < count <= 96, "invalid section count")
    span(data, start, count * 40)
    result = []
    raw_intervals = []
    for index in range(count):
        at = start + index * 40
        values = unpack("<8sIIIIIIHHI", data, at)
        name = values[0].split(b"\0", 1)[0].decode("ascii")
        item = Section(name, *values[1:6], values[7], values[9])
        if item.size:
            require(item.offset >= start + count * 40, "section overlaps headers")
            span(data, item.offset, item.size)
            require(not any(item.offset < end and begin < item.offset + item.size
                            for begin, end in raw_intervals), "overlapping raw sections")
            raw_intervals.append((item.offset, item.offset + item.size))
        result.append(item)
    return result


class PE:
    def __init__(self, data):
        self.data = data
        require(span(data, 0, 2) == b"MZ", "not a DOS/PE image")
        pe = unpack("<I", data, 60)[0]
        require(pe >= 64 and span(data, pe, 4) == b"PE\0\0", "invalid PE header")
        machine, count, _, _, _, opt_size, _ = unpack("<HHIIIHH", data, pe + 4)
        require(machine == 0x14c, "expected x86 PE")
        opt = pe + 24
        span(data, opt, opt_size)
        require(opt_size >= 96 and unpack("<H", data, opt)[0] == 0x10b,
                "expected PE32 optional header")
        self.image_base = unpack("<I", data, opt + 28)[0]
        self.sections = sections(data, opt + opt_size, count)
        virtual_intervals = []
        for item in self.sections:
            end = item.address + max(item.virtual_size, item.size)
            require(end <= 0x100000000, "section exceeds address space")
            require(not any(item.address < other_end and begin < end
                            for begin, other_end in virtual_intervals),
                    "overlapping virtual sections")
            virtual_intervals.append((item.address, end))
        directory_count = unpack("<I", data, opt + 92)[0]
        require(directory_count <= (opt_size - 96) // 8, "truncated data directories")
        self.relocations = set()
        if directory_count > 5:
            rva, size = unpack("<II", data, opt + 96 + 5 * 8)
            require(bool(rva) == bool(size), "inconsistent relocation directory")
            if size:
                self._relocations(self.read_va(self.image_base + rva, size))

    def read_va(self, address, size):
        require(size > 0, "empty image span")
        rva = address - self.image_base
        candidates = [s for s in self.sections
                      if s.address <= rva and rva + size <= s.address + s.size]
        require(len(candidates) == 1, "VA is not backed by one raw section")
        section = candidates[0]
        return span(self.data, section.offset + rva - section.address, size)

    def _relocations(self, data):
        offset = 0
        occupied = set()
        while offset < len(data):
            page, size = unpack("<II", data, offset)
            require(size >= 8 and size % 4 == 0, "invalid relocation block size")
            require(page % 4096 == 0, "unaligned relocation page")
            block = span(data, offset + 8, size - 8)
            for (entry,) in struct.iter_unpack("<H", block):
                kind, within = entry >> 12, entry & 4095
                if kind == 0:
                    continue
                require(kind == 3, "unsupported PE relocation type")
                address = self.image_base + page + within
                self.read_va(address, 4)
                region = set(range(address, address + 4))
                require(not occupied & region, "overlapping PE relocations")
                occupied.update(region)
                self.relocations.add(address)
            offset += size
        require(offset == len(data), "truncated relocation directory")


@dataclass
class Symbol:
    name: str
    value: int
    section: int
    kind: int
    storage: int


class COFF:
    def __init__(self, data):
        self.data = data
        machine, count, _, sym_at, sym_count, opt_size, _ = unpack("<HHIIIHH", data, 0)
        require(machine == 0x14c and opt_size == 0, "expected ordinary x86 COFF object")
        self.sections = sections(data, 20, count)
        require(sym_count > 0, "object has no symbol table")
        span(data, sym_at, sym_count * 18)
        table_at = sym_at + sym_count * 18
        table_size = unpack("<I", data, table_at)[0]
        require(table_size >= 4, "invalid COFF string table")
        table = span(data, table_at, table_size)
        self.symbols = {}
        index = 0
        while index < sym_count:
            name, value, section, kind, storage, aux = unpack("<8sIhHBB", data, sym_at + index * 18)
            if name[:4] == b"\0" * 4:
                string_at = struct.unpack("<I", name[4:])[0]
                require(string_at >= 4, "invalid long symbol name")
                text = cstring(table, string_at)
            else:
                text = name.split(b"\0", 1)[0].decode("ascii")
            require(section <= count and section >= -2, "invalid symbol section")
            require(index + aux < sym_count, "truncated symbol auxiliary records")
            self.symbols[index] = Symbol(text, value, section, kind, storage)
            index += aux + 1

    def function(self, name):
        matches = [s for s in self.symbols.values() if s.name == name]
        require(len(matches) == 1, "missing or ambiguous function symbol")
        symbol = matches[0]
        require(symbol.section > 0 and symbol.kind & 0x20 and symbol.storage == 2,
                "symbol is not an external defined function")
        code = [s for s in self.sections if s.size and s.flags & 0x20]
        require(len(code) == 1, "first-batch objects must contain one code section")
        section = self.sections[symbol.section - 1]
        require(section is code[0] and symbol.value == 0 and section.address == 0,
                "function must occupy the complete code section")
        others = [s for s in self.symbols.values()
                  if s.section == symbol.section and s.kind & 0x20]
        require(len(others) == 1, "multiple functions in object")
        require(not section.flags & 0x1000000, "extended COFF relocation count unsupported")
        relocs = {}
        occupied = set()
        for i in range(section.reloc_count):
            offset, sym_index, kind = unpack("<IIH", self.data, section.reloc_offset + i * 10)
            require(kind == 6, "only x86 DIR32 object relocations supported in first batch")
            require(sym_index in self.symbols, "relocation references missing/auxiliary symbol")
            require(offset + 4 <= section.size, "relocation outside function")
            region = set(range(offset, offset + 4))
            require(not occupied & region, "overlapping COFF relocations")
            occupied.update(region)
            relocs[offset] = self.symbols[sym_index]
        return span(self.data, section.offset, section.size), relocs

    def symbol_bytes(self, symbol, addend, size):
        require(symbol.section > 0, "constant symbol must have a definition")
        section = self.sections[symbol.section - 1]
        require(not section.flags & 0x20, "constant must not point into code")
        at = symbol.value + addend
        require(at >= 0 and at + size <= section.size, "constant outside section")
        return span(self.data, section.offset + at, size)
