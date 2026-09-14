# Acceptance034 results

Six exact functions bring the accepted baseline from176 to182. Their complete446 bytes match with18 verified address operands and no byte exclusions or VC5 warning waivers. Clean source5e169cb90d889ff0c2ad3a1cf3cc1231ad1c3e59 builds as20260914-011600-8ba6f9c4. All182 comparisons and shared authored behavior pass. [Event57](../../evidence/runs/000057-6fcfb46647e55bd0.json) preserves the source snapshot. [Full supplemental CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34808976085) passes on that source.

Normal construction passes3,841 checks. Connected palette load/read/fallback/free passes31,824; name/get passes2,880; actual colormap/material installation passes3,168. The fallback bridge passes5,302 checks. Owned records, callback order and facade globals are checked throughout real dependency chains. Independent review approved the applied code after strengthening full name and pool preservation checks.

The initial supplemental build failed modern MSVC C4996 for historical strcpy. Its [failed run](https://github.com/Afejes1/OpenPhantom/actions/runs/34808479610) and original build receipt remain recorded. The supplemental target now defines _CRT_SECURE_NO_WARNINGS, a target-wide CRT deprecation suppression, while retaining /W4 /WX. Original VC5 source and options were unchanged.

All six additions are team-not_done. Euler construction and the palette release alignment tail remain research. Authored observers and static pools do not establish real heap/file/device behavior or whole-executable identity.
