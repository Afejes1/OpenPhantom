# Sprite frame resolver acceptance

The87-byte exact resolver at0042959d is integrated under its existing canonical caller symbol `op_resolve_zap_material`. One partial sprite view establishes the material pointer at+132; one partial material view establishes frame pointer at+176. No original allocation capacity is inferred from these prefixes.

The retained direct fixture checks NULL outer resource, all selected material/frame combinations, zero/positive/negative prepare results, and callback retargeting of both pointers. The result still addresses the captured frame+44. Owned64-byte frame arrays and full object snapshots bound all accesses.

Connected zap fixtures now provide guarded typed sprite/material/frame storage and invoke the real resolver. The authored prepare backend checks the captured material/frame and mode0 before changing pointers and frame payload. Segment/default capture material before generation; opaque resolves the live sprite after generation and snapshot copy. Success returns the original captured frame even when the backend retargets resource fields. Prepare failure forwardsNULL while retaining callback mutations. Additional NULL-outer wrapper cases execute no prepare callback, including depth0 and depth4; the real geometry and submission still execute.

The scalar geometry tolerance inherited from acceptance017 applies only to independent finite midpoint expectations. All resource objects, frame bytes, guards, passed pointers, counts and bit patterns are exact. The compiled resolver and every prior function must still match their complete original span with all operands verified and zero differences.

This checkpoint isolates frame resolution from the broader acquire/release integration, which changes lifecycle callback counts and post-callback clears across shields, halos and ripples. Those exact research helpers remain queued. Original code was not executed. Whole-executable identity remains pending.
