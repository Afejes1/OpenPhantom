# Static contract

cdecl float(int slot). Complete67-byte span0043AFD3..0043B015. Signedslot<0 or>=32 returns positive floatzero. Capture tablebase006C9A30+slot*0xB4; zero firstdword returns floatzero, else read radiusfloat+0x4C. Return uses x87ST0. No calls. Verified floatzero004A856C operands18/51 and table operand34. Tabledeclared32 records with exactstride, ownedfixturepointers. Two callers use slotatobject+0x100 and consume x87radius. Whole table preservation, all32slots, invalidsignedbounds, inactivity, finite/signedzero/qNaNunderexplicitmaskedx87 checked.
