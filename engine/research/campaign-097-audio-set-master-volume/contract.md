# Master volume attenuation

Complete124-byte interval00496A46..00496AC2 with7 operands and no tail. One cdecl float argument atEBP+8; no current Ghidra callers. Null primary returns. Otherwise truncate (1-value)^2*-5000 to signed attenuation, evaluate a discarded clamp to[-10000,-1], and pass the original attenuation to primary vtable3C. Callback result is ignored. Constants1/-5000 are PE-verified; __ftol at49A44C is the original conversion.

Sixteen literal finite words include signed zero, fractional truncation, one-ULP values around1 and outputs beyond both clamp limits. Five HRESULT classes, two objects, null gate, callback retarget and full snapshots are bounded synthetic evidence. No overflow, nonfinite conversion, real sound device or original execution is claimed.
