# Prepared target: cached nearby collision surfaces

Team not-done bapmap_gatherCells at0x0040D03E; existing trusted Ghidra name
collect_b3d_nearby_collision_surfaces retained. Complete555-byte body ends with
RET at0x0040D268; next function0x0040D269, no alignment. Coordinator reviewed full
body/disassembly and independently enumerated every PE base relocation and three
near-call operands, validating all 25 bindings with the existing pinned reader.
Use VC5/Od/MT and natural C. Inspect this function and its callees statically as
needed. Never execute original helpers. Native compilation only; authored fixture
execution in locked Docker only.

cdecl void(world*, position float[3], radius float). The world parameter is not
used; appends use global op_query_active_world at0x008A0060. Keep this behavior.
Natural locals: int num_cells (-4), int added(-8)=0; query_cell cells[25] at-0x134;
int total(-0x138)=0; int i(-0x13C). query_cell is12bytes: int x,y,count. Enumeration
fills x/y; count is ignored there. Small enumerator0x0040CD93 receives(cells,pos,
radius); square0x0040CF79 receives(cells,pos,radius,capacity), with capacity25
as the LAST argument. Caller pushes capacity first because this is cdecl. Use external stubs of those names
in target.json. Return enumeration count is signed int; synthetic domain0..25.

If radius>=2.0f, invoke services->assert(reason,file,0x36E), then continue if the
callback returns. Service-table global at0x00868640 points to a record with cdecl
assert-function pointer at+0x18. External named char arrays stand for diagnostic
strings; bind their pointers, never include extracted game strings in source.
Radius<0.5f uses small enumeration, otherwise square. At original x87 comparison
NaN skips the radius assertion and takes small enumeration; verify under VC5.

Cache hit requires cached_generation == generation and num_cells == cached_count,
num_cells>0, and only the FIRST cell x/y equal cached_cells[0]. This is a real
first-cell-only check, not a full list comparison. Assembly initializes i=0 and
compares i<num_cells; each body path breaks to rebuild or immediately returns,
so the apparent loop never reaches a second entry. Preserve this limitation.
A zero-cell result rebuilds every time. Globals are32-bit signed integers except
the255-byte visited map table. cached_cells is25 natural query_cell records.

On rebuild zero exactly255 bytes at op_query_visited0x008BFAA0 (natural memset).
Then for i=0..num_cells-1 call append(active_world,cells[i].x,cells[i].y,total,0)
at0x0040D3D0, store returned count and x/y into cached_cells[i], add count to total.
The active world is reloaded per call. Do not clear unused cache entries.
After all appends, if total>=4096 invoke services->assert(capacity_reason,file,
0x395). This is a post-call assertion; do not move it before appends. Then assign
polygon_count=total, cached_count=num_cells, cached_generation=generation read
at the end (callbacks may update it). Return void. Avoid signed overflow/invalid
buffer counts in fixture; large totals can be stubbed without writing polygons.

Focused fixture: small/square threshold and exact0.5; radius assertion exact2
with returning authored handler; NaN selection; first-cell cache hit despite a
changed later cell, generation/count/firstx/firsty misses, empty list repeated
rebuild,255-byte zero with adjacent sentinel, append cumulative offsets and0mode,
active-world rather than unused argument, cached fields and untouched tail,
post-append capacity boundary4095/4096, callback-changed generation/world.
Guard authored recording arrays. Use readable multiline fixture code.

Owner scope candidate.c,api.h,behavior.c,README.md,worker-log.json. Coordinator owns
case/target/packet/sharedtools/Git. Remaining assignment must stop by03:30:18 UTC
and also respect15minutes/tencompiler attempts/fiveconsecutive noimprovement.
Preserve useful partials rather than weaken assertions, cache behavior or targets.
