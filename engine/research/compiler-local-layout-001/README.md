# VC5 local-layout characterization 001

Six independently authored examples were compiled once as C and once as C++ with the locked VC5 RTM toolchain and the same /Od /MT options. The C++ source uses explicit C linkage. Neither output was executed, and the original executable was not read or compared.

Both frontends produced identical code sections in this sample. Reversing the declarations of `first` and `second` retained their offsets (-4 and -8). Changing the meaningful pair to `prior` and `current` placed them at -8 and -4. A three-local example placed `retained`, `steps`, and `total` at -12, -4, and -8. These observations support identifier-dependent placement in this sample; they do not establish a general algorithm.

The pointer example retained the same ADD form in both frontends. Switching language is therefore unsupported as a remedy for the sampled pointer calculation. Private listings, objects and command receipts are hashed in result.json; only authored sources and metadata are published.

Reproduce with an authorized copy of the pinned compiler: compile probe.c and probe.cpp separately with /c /W4 /WX /Zi /Od /MT /FAcs and distinct /Fo, /Fd and /Fa outputs. Use the repository toolchain environment and lock checks. Original game inputs are unnecessary.

This diagnostic added no accepted functions and did not recompile parked target candidates, reset attempt caps or authorize further blind name trials. Use it to prepare new work and a specific future review question.
