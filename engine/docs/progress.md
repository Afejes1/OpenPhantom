# Reconstruction progress

Registered reconstruction work only; not a whole-program census.

Generated from registry.json, target.json and chained verification records. No game bytes are stored here.

Whole executable: **incomplete**. Original linked placement: **unverified**.

| Module | Function / source | Original address | Last verified result | Bytes / address bytes |
|---|---|---|---|---|
| material | [material_mode](../src/material_mode.c) | 0x0040E840 | raw-code-match | 41 / 0 |
| spatial_grid | [grid_cell](../src/grid_cell.c) | 0x00406E22 | relocation-adjusted-match | 24 / 4 |
| map_geometry | [plane_coordinate](../src/plane_coordinate.c) | 0x0040DCEE | relocation-adjusted-match | 384 / 8 |
| view_projection | [update_projection](../src/update_projection.c) | 0x0040EF40 | relocation-adjusted-match | 144 / 48 |

Registered functions: **4**. Last recorded matches with unchanged definitions: **4**.
Current source fingerprint agrees with the latest run: **yes**.
This count is not whole-program coverage and does not certify the modern TPM reconstruction.

Latest build: '20260912-113837-bac8e2c9', source commit 'dc9b79f9a001d73c2e040a1c5d91027f08d46f63'.
History contains 2 verified build records.

## Workarounds and exceptions

- **material_mode**: Retain switch-shaped stack-local code generation under /Od. Exceptions: none.
- **grid_cell**: Evaluate Y before X to reproduce EAX/ECX allocation; Caller bounds and full grid extent remain unestablished. Exceptions: none.
- **plane_coordinate**: Y/Z divisions use explicit double intermediates and float stores; VC5 x87 C3-only comparisons take the early return on unordered input. Exceptions: none.
- **update_projection**: Local float retains the original reciprocal load/divide order; Compare all 15 trailing NOP alignment bytes. Exceptions: none.

## Unresolved program work

- Original CRT startup/heap library revision
- Complete source-module inventory and original translation-unit boundaries
- Retail link order, placements, resources, data, and historical PE metadata
