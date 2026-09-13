# Static contract

Two callers. Publish active/completed/mode before the clock callback; retain callback changes to those fields, then store the x87 clock result to binary32 and publish duration/hold/four low color bytes. Producer source float precision is not recovered; ST(0) return and caller binary32 store are observed.

Full 96-byte span at `0x00439370`, no trailing gap. Team not_done and no local duplicate; full static instructions, call/data operands and caller references reviewed. Original game is never executed.
