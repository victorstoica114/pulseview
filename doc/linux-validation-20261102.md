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

## Export worker creation failure

Export startup now reports a worker creation failure through the existing
save-error dialog. Previously the std::thread exception escaped StoreSession's
start path, for both ordinary exports and collections. The regular export's
thread construction is now inside its existing exception handler; collection
startup has the same error-return behavior. No exception hook or allocator
change is added to the application.

On Linux VMware, a standalone fixture interposes pthread_create only in its
own process and returns EAGAIN for the selected export worker. The published
implementation fails six startup contracts; the corrected source passes all14
fault/control cases, covering binary, srzip, collection and a nested collection
worker at2048 and2621441samples. In-memory source bytes stay exact, no failed
export emits success, and fresh exports/replays produce identical data.
Independent ZIP/CRC/SHA/payload and sample metadata checks cover all six normal
controls. All77frontend regressions pass. The real XCB StoreProgress dialog
shows the failure in all three export modes, returns normally and permits a
fresh exact export using the same captured Session.

The complete installed-form GUI also saves and reopens a10MiB-plus-four-byte
srzip recording; all ZIP members, data and metadata are identical. Its SHA256
is `89c993267e326db21a6e30c7ce3deea56fc2c3f56565518a57c63d412721a1b8`.
This is an export-startup exception correction. USB acquisition, segment memory,
file formats and normal worker execution are unchanged. Physical capture is
excepted because the change handles host export startup only; no USB access
occurred in these tests. Windows of this exact tree remains deferred. Isolated
EAGAIN injection does not simulate machine-wide thread or memory exhaustion,
and this change adds no promise of atomic replacement for ordinary exports.
The separate first-data/USB/early-compaction limitations above remain open.

The optional Linux diagnostic reuses a completed pulseview-test build made
with the Unix Makefiles generator. It is not linked into the application:

```sh
python3 contrib/check-store-thread.py /path/to/build /path/to/new-evidence-dir
```

The evidence directory must not exist. Build commands, object hashes, outputs
and results are retained there. The program scans no hardware and requires the
same runtime libraries as the frontend build.


## File-open error reporting, 3 November 2026

Opening a missing or malformed file displayed the cause in set_device(), then
load_file() displayed a second generic warning without further information.
The minimal fix retains the existing cause-bearing dialog and early return,
and removes only the duplicate notification. A regression covers missing sr
and collection files, invalid ZIP and malformed collection JSON. Linked against
the retained old Session object, this exact new regression fails with two
dialogs instead of one; all 78 current frontend regressions pass on Linux.

An isolated native XCB MainWindow/Session test passes 25 collection inputs and
23 immediate recoveries in the failed Session. It covers 17 open-time rejections,
six later-frame failures, v1/v2 and sample origins above2^53. Completed earlier
frames and delivered partial data retain their own outcomes; a separately
loaded healthy Session stays byte-exact. Each failure has one dialog, and the
next valid load restores all three frames, triggers and origins. The complete
installed-form GUI also shows one warning and opens the valid three-frame
collection after one acknowledgment. Core/C++/SDK hashes match the installed
qualified Linux libraries. Inputs are synthetic and no hardware is scanned.

This qualifies only the Linux host file-open path (VMware, Qt5/XCB). Windows
execution remains deferred by the owner. Hardware is excepted because this
change only removes a redundant GUI notification; acquisition, transport,
firmware and file parsing are unchanged. No new acquisition integrity or
corrupt-file fuzzing guarantee is claimed. AUD013/022 and the earlier transport
limits remain open.


## Guest-kernel first-data observation, 3 November 2026

A retained baseline helper using the real Session and native XCB View classes
reproduced the untriggered 50 MS/s startup failure with passive guest usbmon.
It uses core SHA-256 84c0c7da14777f27871587dfbda1a0645231d051bc10d51feb089c5769757890,
C++ 20ec72bc and SDK ff037c43; helper b19a9db3 is the original baseline, not the
private early-trim candidate or a newly qualified current GUI acquisition.
Linux is inside VMware, DLA 1a86:5537 at 4-1/bus4/address9, 5000 Mb/s. Windows
remained off; no explicit USB reset, clear-halt, handover or firmware change.

The first all32 / 1.60 V Stream run delivered 95420416 RAW bytes and stopped during
an active window. Its two overlapping frames (20M complete and 13592899 partial
samples) match independent RAW transposition, archived payload/metadata, D0
periods 49–51 samples and consecutive CRC-checked D22 UART through two decoders.
Guest-kernel IN payload matches every accepted RAW byte; a further 1 MiB completed
before the final OUT Stop but was retired with stopping=true in userspace.
It is retained separately in the kernel trace, not silently merged into RAW.

The next Stream-window start used byte-identical setup/ARM commands, all32 and
no hardware trigger. Eight 1 MiB sample IN requests were submitted to the guest
HCD before ARM. No positive completion followed during the approximately 3.044 s
before final Stop submission; the driver first-data watchdog fired. All eight
original reads finished cancelled with zero bytes, before that final Stop OUT.
The Stop OUT completed successfully; new synchronous drain reads then returned
64 MiB with status0 and no empty read. The bounded drain therefore reported cleanup
failure and the fixture stopped. The64MiB value is the existing host drain
bound, not a measurement of device buffer capacity. The planned finite-return
phase did not run.
The failed accepted RAW remains 0 B and all earlier/partial files are retained.

The drain includes three CRC-checked source messages. Their recorded host times,
approximately 102/203/304 ms after host Stop completion, mark completion of serial
write+flush, not exact electrical emission. Both UART readers agree. The preceding
command had already completed after Stop in each case, so the next write could
only start later: causal lower bounds are approximately 0.320/101.544/202.724 ms.
This freshness argument needs no assumed maximum serial write/flush latency.
Monotonic/realtime anchors before and after the run differ by about 21 us; this
does not bound every intervening clock error or assume a kernel timestamp epoch.
These witnesses rule out interpreting all returned drain bytes as an old
pre-Stop static backlog under the recorded clock mapping.
A successful OUT is still not a firmware execution acknowledgment, and the
observation does not locate the fault in firmware, FPGA, USB bridge, guest HCD
or the outer VMware transport.

Offline review of the full retained drain finds 335554 complete D0 periods,
all 49–51 samples. At the nominal 50 MS/s, the first-byte sample intervals between
the three messages differ from the corresponding host completion intervals by
about 1.683 and 7.358 us. These are bounded D0/D22 witnesses, not arbitrary
continuity or an exact sample-zero time. Cancellation, Stop and replacement
synchronous reads occurred together; the run cannot isolate their effects.

All 370 monitor events form 185 closed URB pairs, with zero reported drops,
queued events, truncation or write failure. All eight 2048-byte commands match
the logged prefixes and expected padding; framing/CRC are independently checked.
Disk diagnostics change timing: this is one instrumented reproduction, not a
failure-frequency measurement. Serial opening emitted source POWERON_RESET boot
text with DTR/RTS false, without a reset command; no cause is attributed to that.

No product fix, added delay, drain-bound increase or automatic retry follows
from this evidence. Further captures require recovery from the non-quiet state.
This new failure has a late-data/non-quiet-drain dimension, unlike the earlier
zero-data/quiet-drain observation; they are not asserted to share an internal
cause. AUD013 stays private, AUD022 remains open, and Windows stays deferred.
