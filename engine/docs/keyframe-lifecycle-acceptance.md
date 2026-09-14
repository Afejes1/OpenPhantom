# Connected keyframe lifecycle

Five campaign039 exacts connect keyframe load/free/free-entry to actual existing hook setters, plus typed material hook setters. The shared132-byte header now names its node count at56 and node pointer at60; all prior bytes/extents remain fixed. Its44-byte nodes name only the payload at40.

Guarded fixtures connect allocation/load success and failure, free(NULL), custom unload bypass, callback hook mutation, actual nested cleanup and final root release. Every count0..4 and payload mask is covered. Independent direct free-entry tests retain live-count shrink/expand, captured traversal, final live-base retarget, current/next payload replacement and final clear observations. External load-entry and allocation remain authored; the original game is never executed. Material callback ABI remains tied to observed consumers.

Callback releases record observations without real deallocation. Inputs stay within owned bounds; null free-entry and unbounded/malformed node counts are outside evidence. Exact complete functions do not establish whole-executable layout or original resource-data identity.
