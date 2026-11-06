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
