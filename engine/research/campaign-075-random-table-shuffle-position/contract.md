# random_table_shuffle_position

Full original span 0x00470E6C to 0x00470E85, 25 bytes with no tail. All relevant address operands are explicitly bound in target.json.

Random table initialization captures the prior state returned by cdecl int op_gameplay_seed(int), calls cdecl int op_gameplay_random(void) exactly 1117 times, writes consecutive signed int results, resets the global index and returns the captured prior value. Next increments and wraps only when equal to 1117. Shuffle-position stores the unsigned remainder of the signed RNG result; void is used because no reliable explicit return contract is evident, and the quotient left in EAX is incidental. Indexed lookup uses signed index >1117, preserving the equal-1117 adjacent read. Negative or unowned table indexes are not executed. The synthetic unsized extern table is backed by1118 owned elements to observe that boundary without claiming the original allocation extent.

The backend handle accessor uses a live local pointer to a0x102c-byte slot and returns its first signed int. It has no bounds checks. Synthetic slots0..32 and adjacent slot33 are owned; negative/unowned handles are not executed.

Independent callback snapshots verify initialization order, callback mutations and captured return; lookup fixtures verify full table immutability and boundary values. All table values and handles are authored synthetic data. No original program execution or copied game data.
