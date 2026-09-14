# pack_stream_open

Complete original interval0x004706E5 through0x00470786: 161bytes, including RET, no tail. Cdecl one-pointer parameter; open routines return allocated state or null, plain close returns captured file word after release. Existing allocator/release/fclose aliases are retained.

Guarded typed roots test null files, allocation failure/success, four owned allocation identities, varied pre-fill bytes and callbacks that mutate the caller's source-handle variable. Complete byte snapshots verify exact initialization sizes and every published field, including unaffected storage after the smaller plain-stream allocation. Plain close's release observer mutates the saved file to prove the function returns its pre-release capture. Callback expectations are snapshots captured before candidate execution; post-call actual bytes never define expected state.
