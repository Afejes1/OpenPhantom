# frame_prepare

Five ordered callbacks: prepare(current world), traverse(reloaded current world), prepare surfaces(reloaded world,current camera pointer+8,0), prepare graphics submission(), submit gathered surfaces(). First three calls reload globals after prior callback. The caller pushes three arguments for surface preparation even though its current decompiler view only uses world; preserve all three stack arguments. World/camera pointees stay opaque and fixture-owned. No claimed complete rendering implementation.

Team manifest: not_done. Original complete body reaches the next function with no intervening padding. Exact spans and every operand must match under pinned VC5/Od/MT. Native compile only; authored fixtures run isolated in Docker. Ten attempts/fifteen minutes, stop after five non-improving attempts.
