# Mesh submission reconstruction evidence

This research group follows consumers of the matched projection and clip-code
routines. Its complete original source-module boundaries are not established.

## Render-Thing and opacity setup

Function 0x0040F1B0, original analysis name FUN_0040f1b0.
Reconstructed symbol: op_set_mesh_render_thing.
Behavior confidence: HIGH.

The routine publishes its argument to 0x005B8E68. A non-null argument supplies
the float at +0x150, multiplied by 255.0 and stored at 0x005B8E64. A null argument
publishes null and stores 255.0 directly. There are no calls, imports, strings,
clamps, conversions to integer, or other conditional paths in the body.

The incoming call at 0x00456DFB is in queue_projectile_fragment_meshes, immediately
before queue_model_mesh_faces. The caller obtains the render Thing from the
projectile's associated object. The ordinary model setup routine 0x0040FE70
independently stores the same two globals and its recovered RD_THING_TPM layout
identifies +0x150 as render_opacity. The mesh submitter reads the scaled opacity
at 0x0040FAF1 and 0x0040FB3E while preparing the queued face's byte at +0x16.
These independent caller, sibling, layout, and consumer observations support
the semantic names without assuming the rest of the Thing layout.

The body is 43 bytes. Five NOP alignment bytes at 0x0040F1DB through 0x0040F1DF
are independently confirmed in Ghidra; all 48 bytes are compared. Four DIR32
operands cover 16 bytes: the active-Thing store, a verified 255.0 float constant,
and both scaled-opacity stores. No diagnostic waiver or byte exclusion is used.

Synthetic tests cover null reset, normal opacity, negative and above-one values
(no clamping), signed zero, infinity, and NaN propagation. Only the observed
+0x150 field is declared in the partial reconstructed structure.

## Remaining work

Reconstruct queue_model_mesh_faces and its clipping/queue allocation consumers.
This requires extending the comparator with proven relative-call relocations,
reconstructing call contracts, and preserving the current matches. The separate
ordinary model setup path also contains culling, pose construction, light
selection, and hierarchy traversal; it is not complete merely because this
small shared state setup now has a matching candidate.
