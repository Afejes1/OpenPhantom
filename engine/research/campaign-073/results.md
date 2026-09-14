# Campaign 073

Three complete function matches: stream open 86 bytes, packed stream open 161, and stream close 44. Total 291 bytes and four verified call operands, with zero resolved differences. All three first implementation attempts matched. The final native/Docker pairs agree and the guarded matrices pass 17,472 assertions.

Open preserves the original file pointer through allocator mutation. Plain allocation failure leaves the file open; packed allocation failure closes the captured file. Close captures and returns the file before release, even when release mutates the wrapper. Full snapshots verify the exact 0x306c/0xf06c zeroing extents and adjacent guards.

The larger packed-close function remains preparation only: its observed non-null path has no reliable explicit return value. No warning waiver or invented return contract was introduced. Shared checkpoint pending in [Acceptance 053](../acceptance-053/result.json).
