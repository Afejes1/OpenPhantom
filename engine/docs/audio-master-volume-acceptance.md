# Master volume attenuation

The original function computes signed attenuation, evaluates a clamp to[-10000,-1] but discards that value, and passes the raw result to its volume callback. Reconstruction preserves this observed behavior. Sixteen literal inputs cover signed zero, fractional truncation, one-ULP neighbors of1 and both out-of-clamp directions. Guarded fixtures verify null gating, callback arguments, state mutation and preservation. All51,200 checks use finite values within defined signed conversion range. Shared acceptance pending; whole-executable identity remains unfinished.
