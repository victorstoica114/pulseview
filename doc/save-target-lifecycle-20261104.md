# Save target lifecycle and companion setup paths — 4 November 2026

A successful new hardware capture must ask for a new Save destination. Changing
or importing a device must also discard the previous capture's saved-file
association. A refused capture start must retain the previous data and its Save
target. Loading an srzip file must retain that file as the current Save target.

## Reproduction on the published source

Base: `22502a9ed09c7b3e90bae23da16f3bd92906d67d`.
The Windows native desktop GUI, SHA256
`206d85840def2907c0e04733614c2854afd2ee6533380e7e5dfe2df9fe522f05`,
used only the Demo driver, 32 logic channels, 200 kS/s, in the existing Session.

After a named Save As and a new capture, the Session name reverted to `Session 1`
but its previous save directory remained. Ctrl+S silently wrote that directory's
extensionless `Session 1`, rather than opening Save As. Another capture and
Ctrl+S overwrote this implicit file: its LOGIC payload changed from 8,549,748 to
400,000 bytes. The named exports remained unchanged. Both versions of the
implicit file and all original evidence were retained and independently checked
with Python ZIP CRC, member sizes and SHA256.

The setup path was derived by truncating the entire path at its last dot.
An extensionless filename produced `.pvs` in the working directory. A dot in a
parent directory could instead truncate that directory name and place the setup
outside the capture's directory. The same derivation was used when loading.

## Correction and regression contracts

Session now clears the saved-file association wherever it resets the name after
replacing the data: device replacement and successful hardware worker creation.
The latter remains after the exception-return path, preserving the previous
capture when thread creation fails. File-device capture keeps its association;
load_file and restore_settings still establish the loaded srzip target.

Save and automatic setup loading share one path function. It examines only the
filename's final extension and keeps its directory. Extensionless and hidden
basenames remain intact. Explicitly supplied setup paths remain unchanged.

Three regression cases exercise real Session/StoreSession/Qt paths: hardware
Demo recapture including a rejected no-channel start, device replacement by a
binary input including persisted device identity, and setup save/automatic load
for normal, extensionless, multiple-extension and hidden filenames under dotted
parents. Identical tests fail with six assertions on the base source on both
Linux and Windows, and pass on the corrected source.

The full **81-case** test suite passes on both platforms with Qt offscreen and
hardware opt-ins absent. Linux's initial xcb desktop run retained one geometry
failure: the toolbar received 1270 pixels while the existing layout test required
1524. That failure is preserved separately; it is not reported as a passing
native desktop suite. The three targeted cases also pass with Linux xcb.

A separate private fault injector rejects exactly one real acquisition-worker
allocation after an actual Demo capture. On both platforms the data object,
sample count, acquisition timestamp, outcome, saved flag, name and Save directory
survive. The following successful Demo capture clears the old association.
The injector is absent from the product and the GUI executables.

## Native Windows verification and scope

The corrected full GUI SHA256 is
`705a2fc10aaec5859f3f398ad3468f3ca51cc3774c31fcf960c8777ec1b72b6c`.
In the actual Windows desktop, a 1M-sample Demo capture saved 4,000,000 LOGIC bytes.
A subsequent 100k-sample capture saved 400,000 bytes. Ctrl+S after recapture opened
the native Save dialog. Cancel followed by Ctrl+S opened it again. The original
archive and its setup remained byte-for-byte unchanged. Saving the second
capture to a chosen name established the new target; another Ctrl+S used it
without a dialog. No implicit `Session 1` or orphan `.pvs` was created.
Native Open and Save As of the first file preserved all 4,000,000 payload bytes.
Every archive passed ZIP CRC and member checks. GUI native exit code 0, a child
exit-code self-test of 7 and removal of the temporary task were verified. All
481 Windows originals were copied and SHA256-verified before normal VM shutdown.

Linux full GUI SHA256:
`4f5b74e847c666e9f04f3fa531b01360fe93a8eb0d842eb6e4dc8cc3166ce810`.
Linux is a VMware guest; Windows is a nested KVM guest. Incremental object hashes,
commands, all 408 source-file hashes, before/after failures and native originals
are retained privately. Existing product installations remain unchanged.

This is host Save identity and filesystem-path qualification. It changes no
sample conversion, acquisition control, transport or firmware. No physical DLA
capture, source-port open, hub cycle, reset, flash or wiring change was needed
for this correction; the DLA was never assigned to Windows in this cycle.
It does not extend the qualification of DLA continuity, high rates, free-running
continuous mode or explicit Stop. Those investigations remain open. Private
captures, diagnostic injectors, vendor firmware, VM files and credentials are
not part of this publication.
