# Linux validation, 2 November 2026

This fork update adds OR and sampled HIGH/LOW pulse-width conditions to
Stream windows. Width comparisons use whole nanoseconds; `<` and `>` are
strict and Between includes both bounds. A pulse triggers at its closing
sampled edge; an initial partial pulse is ignored. Disabled software
conditions retain the existing trigger sequence. The editor and session
setup preserve the conditions and pretrigger fraction.

It also fixes hidden toolbar controls: Stream windows could be enabled while
Software trigger and Pre remained outside the toolbar layout. Their
QWidgetActions now follow the mode and device/file transitions. The update
includes the previously reviewed startup log-level and decoder ownership/
progress corrections, plus rejection of analog idle compression and safe
edge snapping when an analog signal has no converted logic data.

## Verified scope

The final source content passes all 77 frontend regressions with Qt 5.15.19
on Linux in VMware. The new shown-toolbar contract has 114 assertions;
substituting the old MainBar implementation fails 22 of them. It covers
initially enabled/disabled mode, repeated toggles and device/file transitions.

Real DLA-32 USB3 captures at 5 and 50 MS/s exercise the actual Session path:
20 acquisitions, 36 positive windows across HIGH/LOW and the three width
comparisons, plus eight negative OR/AND intervals followed by Stop. The
negative intervals receive data for 1.900–2.101 seconds and retain 19–21
consecutive numbered/CRC-checked UART messages on D22. Independent RAW
transposition matches every stored logic byte; the D0 clock witness is
checked with a one-sample timing tolerance.

The native GUI separately configures OR conditions, captures three frames
of 200000 samples at each rate, saves/restores the setup and reopens both
collections. All six frames match independently transposed RAW on all 32
channels and replay byte-for-byte through the SDK. At Pre50%, the marker is
at sample100000 on the closing HIGH edge, with widths2samples at5MS/s and
25samples at50MS/s. The 50MS/s GUI capture uses the installed-form executable
whose SHA256 is `08cae7aae20ab5cf44fc30b7824fddbb3418967b1c30a44bc40c8776f8e2872e`.
The menu launch and saved-collection display also pass on native Wayland.

The runtime uses libsigrok/C++ and the EOF-corrected decoder SDK; installed
library hashes were checked in the running process. This publication changes
frontend source and documentation. It includes no private captures, firmware,
VM disks, vendor runtime or authentication material.

## Limits

The current complete frontend tree is qualified for the Linux cases above.
Windows validation of this exact tree remains deferred; earlier Windows
results for individual fixes do not qualify the current combined update.
This is not a claim of arbitrary-channel continuity, every rate/condition
combination, 64 simultaneous conditions, or concurrent decoder throughput.
The short GUI frames supplement rather than replace the longer UART checks.

Measured evaluator cost on a retained physical50MS/s input is129–132million
samples/CPU-second for the positive OR/pulse cases and72–75million for the
two-channel negative cases. These exclude USB and GUI work. One live50MS/s
two-channel OR interval uses1.976CPU seconds over2.337wall seconds including
optional RAW diagnostics; it is not a general throughput guarantee.

The separate high-rate Stream discontinuities, USB handover failure and
experimental segment-memory candidate are not resolved or promoted here.
The `--clean --settings` startup path without an input file was not validated;
the GUI checks use Restore Session Setup explicitly. Hardware sample rate
and depth are configured separately from the saved frontend setup.
