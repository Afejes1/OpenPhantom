# world_set_ambient

Clamp the float argument below zero and above one, then write world offset 0x60. There
is no NULL guard. The comparisons use double constants 0.0 and 1.0; the assignments
store float values. Preserve negative zero. Under the original x87 instructions and
masked exceptions, a quiet NaN takes the lower clamp to positive zero. Any nonfinite
fixture checks are explicitly specific to VC5; do not claim portable C behavior. Verify
all other guarded world bytes remain unchanged.

The complete 62-byte original interval and all address operands were independently
checked against the pinned reference. The team manifest reports baplight_setAmbient as
not done. The actual calling conventions, callbacks and live field ordering are recorded
in the prepared static review. Synthetic callbacks establish forwarding and ordering,
not real backend execution. No byte exclusions, raw byte insertion, assembly frames or
new warning waivers are permitted.
