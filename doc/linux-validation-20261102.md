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

## Initial fork validation

The initial fork source content passes all 77 frontend regressions with Qt 5.15.19
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
25samples at50MS/s. The initial 50MS/s GUI capture uses the installed-form executable
whose SHA256 is `08cae7aae20ab5cf44fc30b7824fddbb3418967b1c30a44bc40c8776f8e2872e`.
The menu launch and saved-collection display also pass on native Wayland.

The runtime uses libsigrok/C++ and the EOF-corrected decoder SDK; installed
library hashes were checked in the running process. This publication changes
frontend source and documentation. It includes no private captures, firmware,
VM disks, vendor runtime or authentication material.

## Limits

The current complete frontend tree is qualified for the Linux cases above.
Subsequent transition stress revealed the intermittent first-data failures
described below; they remain an open qualification limit.
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

## Storage allocation follow-up

Two host storage ownership defects were reproduced with controlled allocation
failures. Compacting the final chunk removed and reinserted its deque entry;
if reinsertion allocated a node and failed, the final stored samples became
unreachable. Growing a chunk could delete the still-owned previous chunk when
the first allocation failed, or leak a newly allocated chunk when deque
publication failed. Both bulk and single-sample append paths are covered.

Compaction now replaces the existing pointer directly. Append paths keep the
new array in a local unique_ptr until the container accepts it, then transfer
ownership. The temporary memory reserve, chunk sizes, padding, sample-count
commit point and compaction timing remain unchanged. This does not enable
the separate experimental early compaction of completed frames.

The final Linux tree passes77frontend regressions and31standalone fault cases.
The original compaction fails at64/128/1024deque entries on this libstdc++;
separate negative controls reproduce deletion of retained storage and leaks
after failed publication in each append path. The test uses small chunks
with normal production append/read methods and verifies every accepted byte,
sample/chunk counts and allocation ownership. Negative controls stop before
reading or destroying known-invalid storage. These are isolated test-process
failures, not induced machine-wide memory exhaustion.

With the final source, real Linux captures at5/50MS/s each retain three event
windows, export and replay. All six frames (66MB total) match independent
all32 RAW transposition, archive payloads and Session replay. Numbered UART
messages agree with a second decoder and remain consecutive in the observed
windows; D0 periods stay within one sample. The exact installed-form GUI also
loads, switches frames, saves and reopens a60MB collection with unchanged
data, origins and trigger metadata. That installed executable has SHA256
`fa9c85c3d99edd9e1e468bc13496bd2bd2bef2c5c675857663e0002942845aab`;
its menu launch is also checked on native Wayland. Explicit `--settings` with
an input file restores the labelled channel as documented by the man page.

Windows for this follow-up remains deferred. Normal hardware captures check
the surrounding storage/export flow; they do not reproduce electrical faults
or actual memory exhaustion. Earlier Stream/USB handover limits remain open.

The optional diagnostic runs in its own process and does not change the
application allocator. From the source root with Qt5 development tools:

```sh
task_tmp=$(mktemp -d)
moc "$PWD/pv/data/segment.hpp" -o "$task_tmp/moc_segment.cpp"
c++ -std=c++17 -O1 -fPIC -I. contrib/check-segment-compaction.cpp \
    pv/data/segment.cpp "$task_tmp/moc_segment.cpp" \
    $(pkg-config --cflags --libs Qt5Widgets) -o "$task_tmp/check-segment"
"$task_tmp/check-segment"
```

## Subsequent Stream startup observations

Further Linux VMware stress at50MS/s used a native XCB Qt View while capturing,
stopped during an active window, repeated Stream windows and returned to a
finite Stream capture. One finite hardware-triggered start produced no RAW
bytes before the test's20second Stop. A control using the published storage
implementation, without private early compaction, then stalled at an
untriggered continuous start after the partial-frame Stop. The driver reported
its first-data timeout after approximately3seconds; all eight transfers retired
with zero bytes and END was delivered. The completed and partial windows before
the failed starts remain preserved and independently checked against RAW.

Both runs used the active Linux libsigrok whose SHA256 begins84c0c7da, with the
same C++ wrapper and decoder SDK. DL32 identification still worked after both
failures. A subsequent published-implementation control retained three event
windows at50MS/s (60MB), with independent RAW conversion, archive/replay and
bounded D0/D22 UART/timing checks, without USB reset, replug or VM handover.

These are intermittent no-data startup observations, not evidence that the
private memory candidate, WCH code or firmware caused them. The triggered
finite observation and untriggered timeout may have different internal causes.
They also differ from the USB handover case in which DL32 identification fails.
The exact cause remains open, and the additional memory candidate remains
private and unqualified. This documentation update changes no product code,
installed executable, USB behavior or firmware. Windows remains deferred.
