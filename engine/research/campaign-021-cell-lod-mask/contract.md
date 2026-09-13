# Static contract

cdecl void op_cell_lod_mask(OP_CELL*). NoNULLguard. Cell unsignedbyte count+0,resultlodmask+2, first record+10; records stride40h. Record unsignedshort material_index+0; unsignedbyte lod_mask+38. local unsignedchar mask=255; for signedi0..unsignedcellcount-1, capture typedrecord; if material_index==65535 setmask0 andbreak; else mask &= record->lod_mask. Afterloop storecell->lod_mask only. Count0 therefore255. Need sizeof/offset assertions and maximum255 ownedrecord capacity (never one-element trailing array indexed beyonddeclaredextent). Fixtures count0,1,3,128,255; masksallbits/intersections; sentinel first/middle/last; 0x8000/0xFFFE materialvalues remainvalid; table literalindependentexpectations; fullrecordarray/headerguards unchanged exceptcell+2. No callbacks.

Cell wrapper0041F2E7 one-arg call; full body confirms unsigned word sentinel and unsigned byte masks.
