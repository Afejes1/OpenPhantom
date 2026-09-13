# world_apply_fog

Capture the red, green and blue bytes from world color 0x214 before callbacks. Send
zero-extended full words to the fog-color callback, then send the live float range at
0x218/0x21C. Test the live enable bit at 0x210. The enabled path clears render flag
0x40, sets the clear color using three unsigned-char parameters, then rereads render
flags and sets 0x40. The disabled path writes a zero unsigned-short clear pixel and
clears render flag 0x40. All other render-flag bits are preserved. Verify callback
ordering, captured colors, live range/enable changes, fresh flag reads and argument
widths with owned fixtures; never run the original renderer.

The complete 230-byte original interval and all address operands were independently
checked against the pinned reference. The team manifest reports baplight_applyLevelFog
as not done. The actual calling conventions, callbacks and live field ordering are
recorded in the prepared static review. Synthetic callbacks establish forwarding and
ordering, not real backend execution. No byte exclusions, raw byte insertion, assembly
frames or new warning waivers are permitted.
