# Acceptance006 results

Animation names, temporary name normalization and clock restart are accepted: three functions, 469 complete bytes and four verified operands. All 34 registered functions passed the clean VC5 comparison with zero differences after verified relocation adjustment. The temporary-name helper is a raw match. Final executable placement remains unverified.

Source revision `937ab726354d4b2e226b2b38e13a593671d5f811` and immutable evidence event 26 identify the exact inputs. The shared names fixture passed 214 checks. Existing readers (488), controls (260), BGL (45) and colormaps (321) passed. The 100 engine-tool tests, 19 focused-runner tests and explicit full CI passed.

The fixture calls the actual restart and temporary-name helper after reading animation names. It checks signed timestamp limits, callback ordering, owned actor/name storage, unchanged unrelated state, and isolation from previous reader callbacks. The temporary-name helper has no externally visible writeback; its local processing is supported by byte equality, not by a claimed observable rename.

The restart mapping probe matched all 58 bytes on its first compilation. Its initial out-of-research packet path was rejected before compilation and corrected without changing the runner. One initial registry test failed because the new documentation was not yet created; the subsequent complete 100-test run passed. No failed compilations, byte exclusions or warning waivers were introduced.

[Full CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34765524052) and [result receipt](result.json).
