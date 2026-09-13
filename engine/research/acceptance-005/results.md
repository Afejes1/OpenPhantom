# Acceptance005 results

Clock, ambient light and fog are now accepted: three functions,359 complete bytes and14 verified operands. All31 registered functions passed the clean VC5/Docker comparison with zero differences. Source revision8c0d4b045863490af55b4a4fbef0fc06a01fa26c and immutable evidence event25 identify the exact inputs and outputs. Final executable placement remains unverified.

The shared controls fixture passed260 checks; previous world readers488,BGL45 and colormaps321 also passed. The100 engine-tool tests and19 focused-runner tests passed, as did the explicit full GitHub regression. All28 prior target/registry entries and existing test bodies remain unchanged.

The controls use canonical world fields and unchanged candidate bodies. Tests preserve original x87 behavior within an explicit fixture environment, finite clock conversion boundaries, captured RGB versus live fog settings, callback ordering and a shared guarded clock-to-ambient-to-fog sequence. No byte exclusions or warning waivers were added.

One initial successful compile was kept as diagnostic evidence because a generated progress document was unstaged. It was not executed or accepted. After committing the document, a fresh fully clean build passed test/compare/record. There were no failed compilations.

[Full CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34763743517) and [result receipt](result.json).
