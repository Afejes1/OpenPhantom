# audio_set_pan

Full interval0x00496C6F through0x00496CA0, 49bytes, no tail. Latest team manifest unfinished, absent prior targets/caps. Independent Sol inventory followed by parent disassembly/constant reads; no Ghidra metadata changed. Cdecl wrapper arguments read fromEBP+8/+12, stdcall callbacks; natural VC5 signed casts call original CRT __ftol at49A44C. Pan slot0x40 is recovered in the existing vtable without changing any later offset.

Volume gates on nonzero musicgate and signed nonnegative auxiliarydevice, casts volume*65535.0f to signedint before transporting full32-bit rawbits, then ORs rawbits with the shifted maskedlowhalf. Rawbits are not incorrectly narrowed beforeOR; negative/out-of-unit finite inputs are deliberately covered. Pan gates before dereferencing its buffer, converts pan*2000.0f and stores an ignored HRESULT local matching observed code. Synthetic callbacks inspect capturedarguments and mutate guarded shared state.

Seventeen exact dyadic input values use independent literal integer/word expectations, including negative, zero, fractions,1.5,2 and16. Products stay within signed32 conversion range; NaN/infinity/out-of-range conversions are outside the fixture domain. PC53 round-nearest is saved/restored. Whole-executable or undefined-conversion behavior is not claimed. No original execution, byte exclusions or warning waivers.
