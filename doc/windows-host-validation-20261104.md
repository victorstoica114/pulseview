# Windows host-side storage and file-recovery validation

4 November2026. This qualifies the specific host-memory and file-load contracts
below for the current frontend sources. No application source, GUI executable,
library or installed launcher changes. User-desktop acquisition at5/50MS/s remains
pending under AUD002; no physical acquisition was performed in this work unit.
The combined Windows frontend is not newly installed or declared fully qualified.

## Source and executable identity

Public baseline `a0371074` has the current408-file frontend snapshot. The retained
native Windows build matches407 files exactly; the one difference is the
[toolbar regression fixture](toolbar-layout-validation-20261104.md), already
qualified separately with its corrected test executable. All application files
match. GUI identities remain Linux `d445d284` and native Windows `206d8584`.
The prior full78-case suites passed on Linux X11/offscreen and Windows native/
offscreen; those unchanged checks are reused, not reported as new executions.

Windows actual file-load diagnostics record module paths to core `bf0a6c78`,
C++ `a1fc7442` and decoder `628767ce`; hashes sampled after the tests are unchanged.
Linux actual mappings remain core `84c0c7da`, C++ `20ec72bc`, decoder `ff037c43`.
Linux is a VMware guest; native Windows runs in nested KVM. No DLA was assigned
during these host tests. No source, wiring, hub, firmware or FPGA change was made.

## AUD020/AUD021: storage under allocation failure

The unchanged native Windows Segment source passes all31 existing compact/append
contracts. They check exact initialized bytes, sample count, chunk count, tail
reachability and array ownership at deque boundaries. Faults cover replacement
array allocation, reserve allocation, and object/container insertion on bulk and
single-sample append. Small test chunks make those boundaries cheap; the product
chunk size, reserve strategy and compaction timing are unchanged.

Four negative executables retain and reproduce the old behaviors:

| Retained implementation / diagnostic | Native Windows result |
|---|---|
| Original compaction,16-case contract | exit1; tail ownership fails at64,128,1024 chunks |
| Original bulk append | exit1; first replacement-array fault deletes retained storage |
| Compaction-only correction, bulk node fault | exit1 at63 chunks, bulk insertion leaks the new chunk |
| Bulk/compaction correction, single node fault | exit1 at63 chunks, single insertion leaks the new chunk |

The append negatives exit after recording unsafe ownership, before reading a
freed retained pointer or invoking a second delete. They do not complete31
passing cases; the original compaction diagnostic independently completes its16
cases with three failures. All logs and partial results are retained.

The current31-case program exits0 with zero failures. It overrides allocation
only inside its own diagnostic process, with thread-local arming around the
actual storage operation. This does not claim machine-wide OOM or an electrical
fault combined with memory exhaustion.

Source SHA-256:
`b03fa2599caf1e2e40afeb655fa0483fdd8abb2f74c592a4b653a0c278013aef`.
Existing public contract SHA-256:
`8a94f09565ef437152f721cee506c8008415f6265aa9ddaf66072685aab8cf6a`.
The source/contract are byte-identical to the retained31-case Linux qualification,
which is reused after hash checks. The fresh standalone Windows executable is
`b4efb2358871af900bd514c2b76a48ddbbaf190065d1e37dad97dd0c73e902db`.

A second native Windows program links the actual optimized Segment object used
by the GUI. The31 same contracts also pass there. Recompiling the current source
with its recorded GUI compiler flags yields an object byte-identical to the
actual GUI object:
`82258f49428ecb3ecb4e4e8aef092461917000016b1cdfd064823eb16451ea22`.
That object and the GUI remain unchanged. The diagnostic executable is
`36e36dfeb5bb42954c983ceb865ac5daa4d786536d30fc2141e26801454d5e6a`.
These are the same31 unique contracts in two compilation/link configurations,
not62 different contracts. No diagnostic hook is linked into the GUI.

## AUD024: malformed collections and recovery

The actual MainWindow/Session file-open path is exercised with25 immutable
synthetic entries, including complete collections, origins above2^53, invalid
ZIP/JSON/manifest/layout/counts, member path traversal, CRC/checksum errors,
truncated members and later-frame corruption.23 failed entries are followed by
a fresh valid replay in the same Session. A separate healthy reference remains
exact through every case. Thus each positive run has48 entry/recovery results.

All48 pass in each fresh run: Linux X11, Windows native Qt, Windows offscreen.
They check exact expected frame bytes, counts, triggers, origins, per-frame/global
outcomes, retained partial data and one error dialog. The private cross-platform
diagnostic changes only process-module recording for Windows and maps fixture
paths to the local immutable copies. Input bytes are hash-verified before use.

The original Session is independently rejected on both OSes: the invalid ZIP
produces two visible dialogs, first the concrete open error and then the generic
load error. The current Session produces one. Negative diagnostics stop at that
failed contract with exit2; they are not described as completing all48 checks.

Current Session SHA-256:
`8ff28276a7b1fff20c31f38f73495e37248916e797bf35444925dc06aaba2cab`.
The Linux diagnostic links the actual full GUI object set, replacing its entry
point; Windows uses the verified current native frontend objects. Native Qt
windows/dialogs are driven by the diagnostic in its test session. Interactive
user-desktop capture and its controls remain outside this software result.
Diagnostic executable identities:

- Linux current: `e4fafe9d6ddca92125e2a3fa71789e1fbceb1874e852c4d0fe963e526568fd12`.
- Windows current: `30b78651fd636549b420ff17fced33668f00213a9a42b22470ebbb1fc8594fbd`.

## Remaining scope

These results complete the Windows software checks for the named storage and
file-recovery contracts. Hardware is excepted for this host-only follow-up:
application acquisition code and executables are unchanged, and no new physical
or electrical claim is made. Previous bounded physical captures and their
failures retain their original verdicts. In particular, the Linux Buffer timeout/
corrupted D0/UART recording remains a failure, and broad GUI/high-rate transport
qualification remains open. No reset, automatic retry or transport workaround
was introduced.

Local evidence: `artifacts/windows-segment-qualification-20261104/` and
`artifacts/windows-collection-qualification-20261104/`, including source/fixture
hashes, compiler/link commands, original negative programs, stdout/stderr,
module mappings, object identity and qualification manifests. Private desktop,
VM images, authentication and commercial firmware are not publication inputs.
