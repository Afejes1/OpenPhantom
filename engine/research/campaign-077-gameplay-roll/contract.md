# gameplay_roll

Complete original span 0x00470DA1 to 0x00470DE5, 68 bytes, no tail. Shared signed RNG state is4B7950. Cdecl signatures are int next(void), int seed(int), int roll(int count,unsigned sides). All relevant operands are explicit in target.json.

Next uses signed Schrage division/remainder by127773, coefficients16807/2836 and adds INT_MAX when the trial value is nonpositive. Its independent fixture uses signed64-bit multiplication and remainder by INT_MAX, tests edge states and256 authored state patterns, then follows32 state transitions each.

Seed returns the captured prior state, makes a negative seed positive, subtracts INT_MAX at/above that boundary, maps zero to1 and stores the result. Input INT_MIN is excluded from executed C negation; an INT_MIN prior state is safely covered. The oracle computes unsigned magnitude/modulo and zero normalization.

Roll accumulates one plus unsigned RNG remainder for signed iterations1..count. Nonpositive counts make no calls, including with sides0. Positive counts are bounded at16 with nonzero unsigned sides; positive INT_MAX iteration overflow and zero divisors are not executed. Crafted high-bit callback results and unsigned accumulation wrap are covered. Observers preserve ordered RNG state side effects.

No original program execution or copied game data. Fixtures are authored; future shared integration must use the canonical RNG chain where present while retaining focused callback evidence separately.
