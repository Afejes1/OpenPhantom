# Campaign 062 results

10 exact functions cover 354 complete bytes and 9 verified operands. 269184 focused checks pass for the exact functions. All functions were marked not done in the team manifest.

| Function | Target / candidate bytes | Status | Checks |
| --- | ---: | --- | ---: |
| [audio_com_initialize](../campaign-062-audio-com-initialize/result.json) | 22 / 22 | exact-function-match | 31744 |
| [audio_com_uninitialize](../campaign-062-audio-com-uninitialize/result.json) | 11 / 11 | exact-function-match | 29696 |
| [audio_commit_listener](../campaign-062-audio-commit-listener/result.json) | 37 / 37 | exact-function-match | 22016 |
| [audio_release_buffer](../campaign-062-audio-release-buffer/result.json) | 42 / 42 | exact-function-match | 20096 |
| [audio_set_cooperative](../campaign-062-audio-set-cooperative/result.json) | 29 / 29 | exact-function-match | 30720 |
| [audio_set_frequency](../campaign-062-audio-set-frequency/result.json) | 38 / 38 | exact-function-match | 15104 |
| [audio_stop](../campaign-062-audio-stop/result.json) | 62 / 62 | exact-function-match | 27264 |
| [audio3d_release](../campaign-062-audio3d-release/result.json) | 34 / 34 | exact-function-match | 14720 |
| [audio3d_set_distance_bounds](../campaign-062-audio3d-set-distance-bounds/result.json) | 50 / 50 | exact-function-match | 47104 |
| [audio3d_set_mode](../campaign-062-audio3d-set-mode/result.json) | 29 / 29 | exact-function-match | 30720 |
| [audio3d_set_position](../campaign-062-audio3d-set-position/result.json) | 51 / 45 | research-candidate | 30720 |
| [audio3d_set_velocity](../campaign-062-audio3d-set-velocity/result.json) | 51 / 45 | research-candidate | 30720 |

Ten exact wrappers matched on first compile; all 168 input hashes and 24 reports independently checked including research.

Complete owned object/vtable/vector state and guard snapshots, exact callback ABI/arguments/order and legal NULL domains reviewed.

Distance and stop callbacks change the live vtable and gate; the second call must use the new table despite gate clear.

HRESULT values are recorded where the original does so, and ignored by void wrappers; no fabricated frame.

Position/velocity four forms: struct forwarding 45, double casts 59, float casts 45, array representation 45, against full51 targets. Initial struct source/API/fixture restored together and verified. Parked early without another evidence-backed variant.

Manifest labels for distance-bounds and mode are inconsistent with observed vtable offsets and original SDK ABI; observed contracts govern.

No byte exclusions or new warning waivers. Whole-executable identity remains pending.
