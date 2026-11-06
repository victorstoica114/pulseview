# Qualified local GUI activation — 6 November 2026

The Linux and Windows lab launchers now use the exact GUI binaries qualified for
[capture replacement and samplerate reads](windows-desktop-validation-20261104.md).
The source fix was already merged through [PR #1](https://github.com/victorstoica114/pulseview/pull/1),
commit `5dfe65ff96788fe32f71db6b8ce6f3806a66a84b`. This activation changes no
acquisition, conversion, transport or firmware implementation.

## Reviewed delivery

| Platform | Previous installed GUI SHA256 | Activated GUI SHA256 |
| --- | --- | --- |
| Linux | `d445d284114cfd6f30a1f0bacca6c375d6df4466afab3be86530276823ad178d` | `e4414dad2e80e1162f0b2cd5f55741be4dc761e90e47961d6a7c6c39141d248d` |
| Windows | `451a78f770199b509b17953c5f832bd7ddd6228e359317ca1a2960c8de12a74d` | `51b1e1cc42272626f186c6d38b44e321e66ff56a54c52f4a9a09f4c8405b65e5` |

The source comparison against each installed build changes only Session,
StoreSession, util.cpp and util.hpp in the application. These are the already
qualified capture-start preservation, Save target/path and samplerate-lock
fixes. Test changes cover their regressions, including the corrected toolbar
fixture; the additional snapshot files are documentation. The 416-file candidate
snapshot and build/evidence anchors remain unchanged.

Linux replaces only the installed `bin/pulseview`; every other installed file
is unchanged. Its existing user wrapper and desktop launcher select that binary.
Windows retains both old GUI files and adds the qualified executable under
`Applications/pulseview-qualified-20261106/pulseview.exe`. Only the `task_gui`
line changes in `run-pulseview.cmd`; its new SHA256 is
`167305bc3886bbe15bf7f214a60f2ed51c7456aa7490c68c6d4d7e4e3c1b9c77`.
The workspace launchers select these installations. Backups and rollback targets
are retained privately. No library, WCH DLL or decoder file is replaced.

The primary checkout was then aligned with the published samplerate fix: exactly
Session's existing-mutex guard and the 64-restart regression were copied from the
qualified source. The other 1,246 product snapshot files, all checkout HEADs and
existing status paths remain unchanged. The prior two files and complete diff
are backed up. All 408 shared primary frontend files now match the qualified
snapshot; the local HEAD is still not the public merged commit. This source-path
alignment changes neither the tested source content nor the activated binaries.

## Actual installed-launcher verification

Each real installed launcher opened a retained 200,000,000-byte LOGIC capture
and used native Save As to a fresh destination. Independent ZIP CRC, member size
and SHA256 checks show identical payload and metadata, with the original archive
unchanged. Each current log contains exactly one file-replay HEADER, END and
Stopped sequence, 200,000,000 LOGIC bytes and no DLA acquisition start.

The loaded core, C++ and decoder libraries match the respective physically
qualified runtime hashes. Both native GUI exits are zero. Windows additionally
retains the command process's original CreateProcess HANDLE and the GUI's live
process HANDLE; its command exit is also zero. All 29 guest originals were
copied and SHA256-verified. The temporary task was removed and the pre-test
Windows settings restored exactly. Linux used an isolated test settings path.

Two preparation failures are preserved: the first Linux launch used incorrect
X11 session authorization/display and aborted before opening the GUI or file;
the first Windows helper quoted CMD options incorrectly and never started the
GUI. The corrected private CMD adapter passed an expected native exit-code-7
self-test before the actual replay. Neither correction enters the application.

## Scope and remaining limits

This unit used one normal Windows boot, zero physical captures, zero DLA
assignments and no source-port I/O, hub operation, reset, flash or wiring change.
Windows shut down normally; the DLA and source are free, with the DLA still
negotiated at 5,000 Mb/s. Linux runs in VMware and Windows in nested KVM.

No new physical qualification was needed for placing these byte-identical,
already physically qualified binaries at their installed paths. Native replay
checks the new launch paths, loaded modules and storage behavior. It does not
extend electrical continuity coverage or resolve AUD006, Buffer failures or
high-rate generality. The failed partial Windows Save and the separate strict
no-WIRE timing refusal retain their original outcomes.

The existing Windows launcher still defaults to its frontend Python decoder
folder. It differs from the private qualification SDK folder in `parallel/pd.py`
only; both trees are preserved. This GUI-only activation and file replay do not
qualify that Python decoder behavior. The loaded decoder DLL itself is identical.

A new private maintenance baseline records the intentional installed-GUI and
primary-source changes. Historical baselines and closed cohorts remain frozen;
their old installed-GUI expectations must not be rewritten or rerun for green
results. Private captures, diagnostic helpers, registry/settings backups,
commercial files, VM data and credentials are not publication inputs.

## Follow-up: direction of the Parallel decoder difference

A file-path audit on 6 November resolves the earlier decoder mismatch. The
installed Linux decoder and the Windows launcher's frontend default already
contain the published [Parallel width correction](https://github.com/sigrokproject/libsigrokdecode/pull/140),
commit `cddc02f3339aee7dc260402d2b572f25c1efbf2c`. Their `parallel/pd.py` SHA256 is
`5168e5254af9b359358a46e306ffbb81b6932c65cbbed3752a7c1f223ab4c511`, verified
against that public source. The separate private EOF qualification SDK prefix
retains the older one-byte implementation, SHA256
`e2622edf990d08c48e03d9b8a190646f17853e3491c9750152f66bd36f5ddee3`.
The only source difference is sizing binary items from the configured bus width
instead of always using one byte. No product decoder or launcher change is
needed; copying the SDK folder's old decoder into the frontend would regress it.

The Windows before/after deployment manifests sampled both trees and confirmed
this was their only non-cache file difference. Retained real-Windows SDK
compatibility evidence uses the exact current decoder source and decoder DLL
`628767ce4f2f9d19185860a4ee02917a9f069e28bf7ad2fc666d3cbe633de0b3`.
Its 23 corrected cases cover widths1–16, four sparse maps, leading zeroes on a
16-bit bus and both word endianness settings. The saved input, expected output,
actual output and stderr files were rechecked against the recorded results.
This reuses frozen Windows evidence; it is not a new guest execution or a new
native GUI decoder-path qualification.

A new file-only run used the actual installed Linux decoder folder and decoder
library `ff037c439927f3a78ec54ea4ac80075afc3e0efb84bf1ff180336980afe8eecb`,
with the qualified CLI. All 23 outputs exactly match the independent expected
bytes and the retained Windows outputs; all exits are zero with empty stderr.
A separate 16-bit control selecting the old private SDK decoder, with the same
CLI and library, reproduces OverflowError and exits1. That negative is preserved.

The product sources, installed files and launchers remain unchanged. There was
no Windows boot, physical capture or source-port I/O in this audit. The VM stays
off and no electrical qualification is added. The original activation evidence,
its preparation failures and the AUD006/Buffer/high-rate limits remain intact.

## Follow-up: Linux native continuous Stop — 6 November 2026

The installed Linux GUI was tested at 50 MS/s in continuous Stream mode, with
32 channels, 1.6 V, one capture and no trigger or stream windows. This is a new
Linux ViewQt/Stop investigation, separate from the closed Linux CAPI and Windows
finite cohorts. Linux runs in VMware; the DLA stayed at USB3-2/5000Mb/s. The
source, wiring, firmware, installed GUI and libraries were unchanged. Optional
RAW tracing and a passive kernel observer were enabled. No Windows boot occurred.

The plan allowed at most three acquisitions, zero retries: continuous native
Stop after 200 MB, then finite 400 MB in the same device/session only if the first
data gate passed, followed by a reserved CAPI 5 MS/s return after GUI close.
Eight lifecycle/Stop/order contracts and 25 profile checks passed;28 preparation
inputs were frozen before I/O. Source commands began 20 ms after the controller
observed the actual current kernel ARM completion. The source remained owned
through native END and finalized kernel ownership/quiet, then was closed.

Native Stop was requested at 200,278,016 delivered bytes. Native Save contains
202,375,168 LOGIC bytes in 49 chunks, with stopped status and empty error.
Kernel 203,423,744 bytes equal the native RAW plus 1,048,576 retained active
worker bytes, in original order. Independent conversion matches all saved
bytes on all 32 channels. D0 has zero bad periods (49–51 samples); two independent
STOP-aware UART decoders agree on 49 consecutive valid CRC messages,
754101–754149. The partial EOF frame remains preserved as unchecked.

**The high-rate data gate failed:** the first complete UART message starts at
30.94888 ms, exceeding the predeclared 30 ms boundary by 0.94888 ms. The final
message boundary passes at 16.17286 ms. Supplemental full-message coverage is
bounded by 33.36520 ms, below the historical 44.119040 ms target; this does not
override the failed initial boundary. No criterion was relaxed and no attempt
was repeated. This refusal alone does not demonstrate lost/replayed data or
identify a firmware, transport or GUI cause.

The finite 400 MB phase was therefore **omitted**. The same-device continuous
to finite transition remains unqualified. The historical zero-data case did
not occur in this continuous attempt, but remains unexplained. Native Open/Save
As preserved all members and metadata of the 202 MB archive, with the original
unchanged; the GUI's original process handle returned exit 0.

After GUI close and free-resource checks, the reserved CAPI 5 MS/s/25 Msample
return completed 100,000,000 bytes. Independent kernel-prefix/feed/storage
conversion agrees; D0 has zero bad periods (4–6 samples), and both UART decoders
agree on 50 consecutive CRC messages,756101–756150. Native helper/controller
exit 0, END/quiet and source close passed. The cohort closed at **two physical
acquisitions, zero retries and zero Windows boots**, with USB/serial resources
free and the current product source/runtime checks unchanged. No reset, flash,
hub pulse, installation or production-code change was made.

This result adds bounded Stop, storage, replay and recovery evidence. It does
not qualify the withheld transition, close AUD002/AUD022/AUD006, or establish
arbitrary electrical continuity on 32 channels: D0/D22 remain the only temporal
witnesses. The negative and partial evidence are retained privately. A new
Windows/common-product qualification is not claimed for this Linux-specific
diagnostic. No speculative product correction is justified by this result.

## Follow-up: source timing and the actual Linux transition — 6 November 2026

The Linux GUI has now completed continuous 50 MS/s → native Stop → finite
50 MS/s / 400 MB in one hardware Session. Storage and replay are exact, but
**the full predeclared finite witness profile remains unqualified**: its last
complete UART message ends 21.13752 ms before the capture boundary, exceeding
the fixed 20 ms limit. The refusal is retained without changing its criterion
or repeating the capture. No product implementation or installation changed.

Two separately bounded cohorts distinguish source timing and a LAB preparation
error. Both used the same installed GUI/core/libraries, 32 channels, 1.6 V,
Stream, one capture, no trigger/windows and unchanged source/wiring/firmware.
Linux runs in VMware; the DLA stayed on USB 3-2 at 5,000 Mb/s. Windows stayed off.

The first cohort changed only the GUI witness's first-command scheduling floor
from 20 to 5 ms after observation of the current kernel ARM completion. Its
prediction, fixed before I/O, required the first full UART message to move
10–20 ms earlier and track the host-command shift within 5 ms. The first START
moved from the preserved 30.94888 ms to 15.37486 ms, earlier by 15.57402 ms;
host/GPIO shift disagreement was 0.695465 ms. Its 202 MB continuous capture
and reserved 100 MB / 5 MS/s return passed. This supports source-phase
sensitivity across runs; it establishes neither absolute propagation latency
nor a device/GUI cause for the historical failures.

Its finite Run was refused **before HEADER, ARM or USB requests**: optional
WIRE tracing creates an exclusive file, and the LAB adapter had left its
previous pathname occupied. This was a LAB preparation error. The native GUI
returned exit 0, two finalized passive scopes contained zero requests/loss,
and the source remained owned until verified recovery. That cohort closed
without retry: three native Run requests including the refusal, two actual
ARM acquisitions and zero Windows boots. Its negative remains preserved.

The minimal correction changes only LAB file management. After proven END,
kernel quiet, source close and an exact RAW copy, it verifies that the native
descriptor is closed, preserves the original inode/hash under a fresh name,
and frees only the exclusive pathname. Product `O_EXCL` stays unchanged.
Nineteen lifecycle, phase and real filesystem/CLI contracts plus 25 unchanged
witness-profile checks passed before the corrected adapter's physical test.
All 41 preparation inputs were frozen. The second cohort used exactly three
actual acquisitions, zero retries and zero Windows boots:

| Phase | Saved LOGIC | Consecutive valid CRC | Result |
| --- | --- | --- | --- |
| Continuous 50 MS/s, native Stop | 202,375,168 B | 50, `761101..761150` | Original data profile passed; first START 14.70646 ms. |
| Same-session finite 50 MS/s | 400,000,000 B | 99, `762101..762199` | Native complete, exact storage; strict final boundary failed at 21.13752 ms > 20 ms. |
| Reserved CAPI return, 5 MS/s | 100,000,000 B | 50, `763101..763150` | Data, END/quiet, source close and native exit passed. |

Independent kernel/RAW conversion matches every accepted byte on all 32
channels. Continuous kernel payload is 203,423,744 B: RAW 202,375,168 B plus
1,048,576 retained active worker bytes. Finite kernel and RAW are both
400,556,032 B; the 556,032 B beyond the accepted prefix are retained. D0 has
zero bad periods in all phases. Two independent STOP-aware UART decoders agree.
The first diagnostic inode and archive remain unchanged after the finite Run.

The finite tail contains the partial prefix `ECHO USBSEQ-76220`; its next
observed byte cannot complete STOP validation before EOF. Candidate command `762200` follows
source order and matches the prefix, but its full counter/CRC is unverified.
It is **not counted as a 100th valid message**. The observed last-message
start gap, 22.007440 ms, is consistent with the corresponding host-write gap,
22.026781 ms. This explains the stricter final-boundary gate's sensitivity
to the paced witness; it does not demonstrate acquisition loss.

A separate integer containment analysis checks every possible origin of the
historical 44.119040 ms target window. Each of the 97,794,049 contained finite
windows includes a complete fresh CRC message, as do all 48,387,841 origins in
the continuous capture. Exact-edge and one-sample-hole negatives and an
independent exhaustive small-case oracle pass. This supplemental result does
not override the failed 20 ms profile or guarantee arbitrary electrical
continuity on all channels. D0/D22 remain the electrical witnesses.

Native Open/Save As preserves every payload and metadata member of the new
202 MB and 400 MB archives and a copy of the earlier source-phase archive;
all originals remain unchanged. The original GUI process returned exit 0.
Sources were held through each actual END and finalized kernel ownership/quiet.
The DLA/source are free and Windows is off. No reset, flash or hub operation
occurred. The corrected LAB handoff and native transition lifecycle are
verified; the strict finite profile and general AUD006/AUD022 remain open.
No new Windows/common-product qualification or transport correction is claimed.
