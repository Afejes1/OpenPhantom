# Acceptance031 results

Four canvas/face functions are accepted, bringing the baseline from 162 to 166. Their 389 complete bytes match with eight verified operands and no exclusions or warning waivers. Clean build20260913-224942-6db72bbc at6ca2f1f0576bcf998402dcc733c636110e3c517b passes all166 comparisons and shared authored behavior. [Event54](../../evidence/runs/000054-59e46badca984d72.json) preserves its source snapshot; [full CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34800750019) succeeds on the same source.

Canvas lifecycle passes 7,334 checks, face construction 864. Actual canvas constructor/initializer/free are connected with literal center-bit oracles, guarded rasters and allocation failure. Inner cleanup remains an authored observer because its four-byte tail is unresolved. Face construction connects the actual initializer and cleanup chain, preserving all four unspecified words and verifying optional array releases before the captured root.

Two targeted fixture compiles exposed helper-name collisions with the font fixture. New observer names were corrected to fctor_/FCTOR_ before the successful targeted compile and clean build; failed receipts remain and no candidate changed. Independent review approved the final files. Whole-executable identity remains pending.
