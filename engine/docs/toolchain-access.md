# Original compiler access and fingerprinting

Required code generator: Microsoft Visual C++ 5.0 RTM, compiler banner
11.00.7022, from Visual Studio 97 or the standalone VC5 product. The native
Windows driver is an alternative host for the same compiler that upstream ran
through wibo; it is not a replacement with VS2022, VC6, or clang.

Status on 2026-09-12: no VC5 installation or installer was found in the inspected
project/download locations. No original-toolchain build has been performed.

Access research:

- Microsoft's [original product announcement](https://news.microsoft.com/source/1997/02/11/microsoft-announces-visual-c-5-0-professional-edition/)
  identifies VC5 as a standalone product and part of Visual Studio 97.
- Microsoft's [subscription download guidance](https://learn.microsoft.com/en-us/visualstudio/subscriptions/software-download-list)
  points to the authenticated catalog and a public product list. The linked
  public workbook dated 2026-08-19 was inspected on 2026-09-12 and contained no
  exact Visual C++ 5.0 or Visual Studio 97 title. This is not proof that every
  subscriber's authenticated catalog lacks it; Microsoft says suite packaging
  can affect listings.
- Next acquisition step: check an entitled subscriber's authenticated catalog
  for the exact product or obtain legitimately licensed original media. A
  purchase, subscription, or contact with maintainers has not been made.
- No third-party compiler archive was downloaded or treated as authorized merely
  because it is publicly hosted. Original media and tool binaries stay private.

Copy toolchain.example.json to private/toolchain.json and set actual absolute
paths and acquisition provenance. Keep a private original installation tree;
bin_dirs must include its compiler and shared IDE DLL directories. Keep include
and library search order explicit. Do not point these directories at modern SDKs.

`configure` checks the compiler/linker banners and locks every executable/DLL in
the selected binary directories, every file in the include/library directories,
and the configuration itself. It requires the C front end, code-generation back
end, and LIBCMT. `preflight` and each build recheck that inventory. No global PATH,
registry registration, system clock change, binary patch, or automatic compiler
download is part of setup.

The linker check currently requires the original 5.00 family; it records the
exact observed revision instead of asserting that the game used a particular
unverified linker build. Likewise RTM LIBCMT is a starting point, not proof of
the startup/heap library revision. Upstream reports a possible SP1/SP2 mixture.
Keep these differences unresolved until tested; do not fill them with copied
machine code. If the tools cannot run natively, report the failure and revisit
the build host explicitly rather than silently substituting a compiler.
