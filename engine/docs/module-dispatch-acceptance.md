# Module dispatch acceptance

Six exact dispatch functions add 900 complete function bytes and eleven verified
address operands. Their focused fixtures already execute the accepted module
finder. The shared fixture retains all six case matrices with separate names
and uses the same canonical finder. Existing 267 target and registry entries and
the 36-byte module record remain unchanged. Candidate bodies change only their
header include on integration.

Restore sends event11 and an opaque32-bit payload word; its observed save caller
zero-extends a16-bit value. Save sends event10 and the original requested ID, or
each node's live ID during a forward broadcast. General event dispatch sends the
event and payload words and broadcasts backward. Targeted calls propagate signed
callback returns. Missing IDs return0 for restore/save and1 for general event;
broadcasts return0 irrespective of callback returns.

Suspend and resume conditionally dispatch events8/9 and set/clear bit3 after a
zero callback return. Stop dispatches event4 for bit1-set nodes and clears that
bit after success. Each reads the live status word after its callback, preserving
other callback changes. Stop/suspend traverse forward; resume traverses backward.
Names for these three follow the contributor inventory and observed flag/event
mechanics; no direct callers were recovered, so higher-level naming is provisional.

Guarded owned lists test empty through five nodes, rotation, duplicates, signed
IDs, callback identity/order, extreme returns and arbitrary payload words. Expected
state and visit plans are established before each call. Callbacks can truncate or
skip a live link, change a future ID, mutate node/payload storage and replace both
global list endpoints. Full state is checked inside callbacks and after return.
Observer guards terminate on unexpected visits before indexing. Only authored
bounded acyclic graphs are exercised; no freeing, arbitrary pointer dereferences
or original execution. No warning waivers or byte exclusions are added.

The event tail-global operand starts at46F4EB, offset66. The initial inventory's
65 was rejected before compilation and retained as a failed preflight receipt.
Restore's initial pointer-like spelling was corrected to opaque-word transport
after checking its caller. These are documented review corrections, not exclusions.
Whole-executable identity remains a separate unfinished milestone.
