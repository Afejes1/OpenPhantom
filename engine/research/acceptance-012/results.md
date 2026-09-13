# Acceptance012 results

Thirteen exact C27/C28 functions are promoted, raising registered acceptance from69 to82 and immutable evidence from32 to33 events. They cover1119 complete bytes and63 verified operands. All82 original-toolchain comparisons pass.

The shared effects suites pass: halo/overlay13,401 checks; zapline44,310; ripple material7,618; saved effects5,980. These totals include retained prior coverage. The shield lifecycle suite passes17,209 checks: a removed zap-callback stand-in is replaced by actual detach, full-pool checks and endpoint queries, so its check count changes while all input profiles remain. All prior suites,100 tooling tests,19 runner tests and full supplemental CI pass.

Source: `7047248402ec2760716275af5c8bf05988a0ada0`. Build: `20260913-152401-33d3b7bb`. Event: `000033-55ac18bb0d997bea`. All161 source snapshot entries match the accepted commit and current files. Per-artifact hashes and CI links are recorded in result.json.

A preliminary fixture-only compile caught a naming collision between endpoint buffers. Distinct fixture-local names fixed it before the successful full build. The failed diagnostic is retained; candidate behavior and compiler flags did not change. The shared-global binding audit found no address conflicts.

Actual reconstructed functions now link through object teardown, halo drawing, zapline query/reset and overlay reading. External backend callbacks remain authored fixtures. The alignment-tail candidates are excluded from promotion, not from their original comparison spans. No original game execution, byte exclusions, warning waivers or full executable-layout claim.
