# Windows validation and USB handover limits — 2026-11-03

Windows hardware testing resumed on November 3. This updates the earlier
owner-deferred Windows status for the checks listed below. It does **not**
qualify arbitrary long Stream captures at 50 MS/s: retained additional failures
returned the requested sample count while skipping and repeating source data.
The failure and the successful controls are both retained.

Linux runs inside VMware. Windows 11 runs in a nested QEMU/KVM VM using the
native WCH transport. The DLA-32 is `1a86:5537`, negotiated at 5000 Mb/s. Tests
were exclusive; Windows and Linux never captured from it simultaneously.
WinUSB and physical, nonvirtualized Windows/Linux hosts were not tested here.

## Versions and frontend checks

All 408 tested PulseView source files match fork master at
`383b8f1fa2718abe7263a1eb20d4088495f7516e`. This publication changes documentation
only. The Windows build was compiled natively with MSYS2/UCRT; source and fixture
hashes were checked before and after testing.

| Windows component | SHA256 |
| --- | --- |
| PulseView GUI | `451a78f770199b509b17953c5f832bd7ddd6228e359317ca1a2960c8de12a74d` |
| Test executable | `247244f03dce08e948e84c76dfdddc57ecff990b3d4ea6cc8c4d22659f146327` |
| libsigrok | `bf0a6c78b1ee4d8f6a3e2c5a0ca234cd2fef283419551023830f40d77bbad3a6` |
| libsigrok C++ | `a1fc7442465f95c7deb5692e15e68df656ab110660fbe62edbe1d579dfb94be6` |
| libsigrokdecode | `628767ce4f2f9d19185860a4ee02917a9f069e28bf7ad2fc666d3cbe633de0b3` |

* All 78 native regression cases pass.
* 25 immutable collection inputs and 23 immediate recovery cases pass through
  the real Qt MainWindow/Session on Windows, using the offscreen platform.
  These check one error dialog, preserved partial data, large sample origins,
  archive errors and an independent healthy session. Fixtures remain unchanged.
* The full desktop GUI opened the real 50 MS/s three-frame collection, zoomed
  and closed normally. The local Windows launcher now uses the GUI identified
  above. This was file playback; it is not an interactive capture qualification.

The commercial WCH DLL was used locally and identified by hash. It is not
included in this repository. These binaries identify a bounded test result,
not a replacement for the independent upstream PR qualifications.

## Captures that passed, and the initial zero-byte failure

The signal source was the unchanged ESP32 v8 generator: D0 carried a 1 MHz
clock and D22 observed UART RX0 commands numbered by the host with CRCs.
Positive cases below passed an independent RAW-to-LOGIC transpose across all
32 channels, bounded D0 periods and consecutive D22 messages. Conversion
agreement across all channels does not establish arbitrary all-channel continuity.

| Case before the VM reset protection | Normalized LOGIC | Result |
| --- | ---: | --- |
| First finite Stream, 5 MS/s | 0 B | Three-second first-data timeout; retained failure |
| Continuous Stream, 5 MS/s | 100 MB | RAW exact, D0 and 50 consecutive messages pass |
| Finite Stream immediately afterward, 5 MS/s | 100 MB | RAW exact, D0 and 50 messages pass |
| Buffer, 50 MS/s | 400 MB | RAW exact, D0 and 20 messages pass |
| Finite Stream, 50 MS/s | 400 MB | RAW exact, D0 and 20 messages pass |

The first failure had four empty SDK reads, accepted Stop and quiet cleanup.
The passive kernel trace contains no positive payload on endpoint 0x81;
the source acknowledged 63 consecutive commands. Setup/ARM payloads match
later successful controls. The continuous/finite controls passed without a reset.
This does not identify a deterministic mode defect or fix the intermittent start.

Native software-trigger testing at 5 MS/s passed 178 assertions: six
HIGH/LOW × less/greater/between OR cases, three frames each, and an impossible
AND case producing no frame. All 18 positive windows match an independent
transpose of the kernel USB payload. The negative control also contains real
D0 pulses. Its Qt timer includes setup; the measured ARM-to-Stop interval is
161595 microseconds, not an assumed 250 ms of electrical acquisition.

A separate native 50 MS/s Stream-windows test passed 55 assertions and produced
three 100 ms windows, 60 MB total, from one physical start. All frame bytes,
source origins, D22 falling triggers, CRC witnesses, exports and replay match
the retained RAW. Gaps or overlaps between windows remain explicit; windows
are not concatenated to infer continuity.

## USB reset is a demonstrated identification-loss trigger

Ordinary QEMU detach left the USB device enumerated but no longer answering
the DL32 identification query. The following controls isolate a trigger in
this VMware/Linux/KVM path, without locating an intrinsic firmware defect:

1. A single successful `USBDEVFS_RESET` on a healthy DLA, with Windows stopped
   and no acquisition, lost DL32 identification.
2. A private QEMU diagnostic omitted exactly its close-time reset for the DLA.
   Attach/detach with the guest paused preserved identification.
3. Without a hub cycle between conditions, ordinary QEMU repeated the same
   paused attach/detach and lost identification again.

[QEMU 11.1.2 closes the device with an unconditional libusb reset](https://github.com/qemu/qemu/blob/v11.1.2/hw/usb/host-libusb.c#L1026-L1055).
Setting `guest-reset=false` alone does not remove that close-time call.
The diagnostic interposer was retired after this control; it is not installed
globally or used by the ordinary VM launcher.

The local dedicated VM now starts under a Linux x86-64 seccomp policy denying
`USBDEVFS_RESET` with EPERM in that QEMU child. Other native syscalls and ioctls
retain their kernel behavior. DLA attachment also sets `guest-reset=false`.
The policy covers USB reset ioctls in that child, rather than selecting a USB
file descriptor; it is not a general-purpose QEMU change or a product driver fix.
It changes no captured bytes, device firmware or FPGA configuration.

Actual syscall contracts and a denied reset on the healthy DLA passed. Paused
handover preserved DL32. Windows 100 MB at 5 MS/s then Linux 100 MB passed.
The final ordinary start/attach/detach path repeated Windows 100 MB followed
by Linux 100 MB at 5 MS/s with the same source connection kept open throughout;
RAW, D0, 50 consecutive messages per OS, cleanup and identification passed.

## Protected-launcher 50 MS/s integrity failure

With that ordinary protected launcher, finite Stream delivered 100 million
samples / 400 MB and reported complete with quiet cleanup. Nevertheless:

* D22 skipped commands 895003–895008 and later repeated 895015–895020 after
  895020. Both UART decoders agree. The source acknowledgements are consecutive.
* Two D0 periods lie outside the allowed ±1 sample bound.
* RAW offsets 134217728 and 268435456 contain **131564544 identical bytes**.
* Independent all-channel RAW conversion matches LOGIC exactly.
* The 400556032-byte retained RAW file is an exact prefix of Linux kernel
  endpoint-0x81 completion payloads. Submission and completion order agree;
  the passive observer exited cleanly, with 383 positive IN completions.

Thus the repeated bytes already exist before Windows WCH decoding, in the
kernel USB data of the outer Linux VM. This narrows the observation point;
it does not distinguish the DLA/bridge from the outer VMware USB path.
The reset protection preserves identification but does not establish Stream
continuity or resolve the previously observed high-rate loss limitation.

After protected detach, Windows shut down normally. A separate Linux 5 MS/s
100 MB control with a newly opened source connection passed RAW, D0, 50
consecutive messages and cleanup. DL32 identification remained valid.
This successful recovery does not invalidate the failed 50 MS/s capture.

## Follow-up: kernel wait and separate SDK calls

An offline comparison of the successful and failed finite 50 MS/s runs above
finds identical runtime hashes and all four command payloads. Each trace has
383 positive sample-IN completions, with at most one request pending. All
401604608 kernel payload bytes equal the retained RAW followed by the separate
1 MiB worker-unconsumed tail, in order.
The prior device/VM states differ; this is a timing comparison of retained
observations, rather than a controlled causal comparison.

| ARM completion to last RAW completion, outer Linux trace | Successful run | Failed run |
| --- | ---: | ---: |
| Total | 2.792663 s | 3.800403 s |
| At least one sample-IN pending | 2.321083 s | 2.322883 s |
| No sample-IN pending | 0.471580 s | 1.477520 s |
| Longest interval without a sample-IN | 2.001 ms | 601.701 ms |

The failed run's second worker read takes 600.871 ms in Windows; its associated
Linux sample-IN takes 6.339 ms. This separates the observed sample-IN lifetime
from the wider call boundary. A pending request is not measured physical bus
occupancy. Windows QPC and Linux usbmon remain separate clock domains.

A fixed private profiling cohort then used a forwarding SDK observer qualified
with a deterministic fixture. It preserves SDK arguments, data, lengths and
LastError; metadata stays in memory until device close. Instrumentation changes
timing, and the following cases do not measure its overhead or a failure rate:

Observer DLL SHA256:
`147debb0aa5ed3b9f1e701758d7ad2afd8c341e6960c371e2a01d4073a77fd54`.

| Physical finite Stream case | LOGIC | Witness result |
| --- | ---: | --- |
| Observed 25 MS/s control | 400 MB | D0 and 40 consecutive CRC messages pass |
| Unobserved 50 MS/s baseline | 400 MB | D0 and 20 messages pass |
| Observed 50 MS/s | 400 MB | Six-message forward gap, backwards sequence and two D0 anomalies; failed |

All three preserve exact independent all32 RAW conversion, complete kernel
payload accounting, quiet cleanup and unconsumed data separately. SDK logs
report no omitted metadata. The same source connection stayed open across the
three Windows cases. The two 50 MS/s cases use identical command bytes.

For the 382 SDK reads represented in accepted RAW, the observed failed run has:

* Timeout setters: 6.20697 ms total, median 13.97 microseconds, maximum 223.02
  microseconds.
* ReadEndP: 3.17839981 s total, median 8.31598 ms, maximum 14.15209 ms.

Removing all measured setter time would address less than 0.2% of the combined
setter/read duration in this instrumented trial. No timeout caching or value
change is justified. The older 600 ms wrapper event did not recur here, so this
does not locate its internal cause.

The new failed packet order is 930001–930012, 930019–930020, then
930015–930020. Both occurrences of 930019 and 930020 are exactly 33554432 samples
apart. At the corresponding 128 MiB RAW distance, a block-aligned suffix of
36086784 bytes is identical, already in the Linux kernel payload. The full
131564544-byte comparison region is **not** identical in this new run; do not
substitute the older complete-region replay result or infer physical memory
capacity from either observation.

The first planned Linux return was refused before hardware acquisition because
the private runner supplied an out-of-range source counter. That refusal is
retained. A separate return with a valid counter and reopened source passed
Linux 5 MS/s / 100 MB, RAW, D0, 50 consecutive CRC messages and cleanup. Windows
shut down normally; DL32 identification stayed valid. No started capture was
retried, and no product source, timeout, queue, driver, firmware or wiring changed.

## Follow-up: bounded guest scheduler profile

A private forwarding observer added native process/thread IDs to each SDK
call. Windows' existing WPR recorded ProcessThread, CSwitch and ReadyThread
events into 64 memory buffers of 1 MiB, without stacks or active ETL disk
writing. Each trace used its own named session. The observer remained confined
to the acquisition helper; the product launcher, driver and timeout values
were unchanged. The tested observer DLL SHA256 is
`37132b4d8d95f5a1a68c112282a125b863e0c0afed95b903c514f7c6da23c2cb`.

Native fixtures without DLA access verified export forwarding, data/counts,
LastError, worker IDs, CPU work, a known wait and a deliberately delayed Ready
thread. Clock, loss, lifetime and ownership rejection cases passed. Raw ETW
timestamps use the same QPC frequency as the SDK observer. Absolute Windows
timestamps are not subtracted from Linux USBmon timestamps.

The first physical control, 5 MS/s and 100 MB, passed RAW conversion, D0,
50 consecutive CRC messages and cleanup. Its strict scheduler analysis refused
ambiguous switch-out states, so the runner stopped **before** starting 50 MS/s.
That refusal and its original sources are retained. Windows was shut down.

The analysis contract was then explicitly revised offline, before another
acquisition: preserve those states as an unresolved off-CPU bin and report
lower/upper bounds, accepting at most one percentage point of unresolved time.
The original strict result remains unqualified. Native calibration and a
masked known-Ready interval verified conservation and conservative bounds;
no ambiguous interval was relabeled Waiting or Ready. PID/TID, complete lifetime,
clock, event-loss and waveform checks remained in force.

The saved 5 MS/s control met this contract without recapture: 0.20956 ms was
unresolved out of 5026.92449 ms in 96 RAW-bearing reads, or 0.004169%.
Its final empty 1014.46328 ms read remains separate, with a real zero-byte
unconsumed tail. Its proportions are not extrapolated to 50 MS/s.

One original planned 50 MS/s/100-million-sample diagnostic then ran, followed
by one Linux return control. The VM and source connection had been reopened
since the saved Windows control; the same source connection was held between
this 50 MS/s run and the Linux return. No acquisition was retried.

The 50 MS/s capture returned 400 MB of normalized LOGIC and quiet cleanup,
but **failed integrity**: messages were `898501`–`898510`, `898518`–`898520`,
then `898515`–`898520`, with two abnormal D0 periods. Independent all32 conversion
still matches RAW. All 383 positive kernel sample-IN completions, in order,
match 400556032 bytes of retained RAW followed by 1 MiB of unconsumed data.
There are no passive-monitor losses or truncated positive payloads. The
131564544-byte comparison at 128 MiB distance differs in 3335467 bytes, while
its final **73835520 block-aligned bytes match exactly**. Each of the three
repeated CRC messages is separated by 33554432 samples. This is another
retained failure, not proof of a physical memory capacity or a firmware cause.

The 30905-event scheduler trace and SDK log have no reported losses. For the
382 reads represented in retained RAW, the bounded analysis gives:

| Component/state | Total |
| --- | ---: |
| Timeout setters, separate from ReadEndP | 6.32307 ms |
| ReadEndP wall time | 3158.24120 ms |
| Guest-scheduled within ReadEndP | 508.99143 ms |
| Waiting | 2605.28892 ms |
| Ready | 36.80810 ms |
| Standby, kept separate | 4.02386 ms |
| Unresolved off-CPU, kept separate | 3.12889 ms (0.099071%) |

Waiting is bounded by **82.4918–82.5908%**. The broader Ready-to-dispatch
category is bounded by **1.1655–1.3919%**, including standby and unresolved
time only in its upper bound. Median ReadEndP is 7.61396 ms, maximum 38.70057 ms;
the older approximately 600 ms call did not recur. The extra 1 MiB read after
the RAW target took 7.54715 ms and is excluded from these acquisition totals.
The longest call has 37.44961 ms Waiting and 0.06193 ms Ready. These observations
do not justify a thread-priority change or timeout-setter optimization.

At the Linux observation point, ARM completion to the last retained-RAW
completion took 3.200787 s: sample-IN requests were pending for 2.328829 s
and absent for 0.871958 s, with at most one pending request and a longest
no-pending interval of 32.805 ms. These are separate clock-domain intervals,
not physical bus utilization or an absolute ETW-to-USB alignment.

After named-WPR cleanup, protected detach and normal Windows shutdown, the
Linux 5 MS/s/100 MB control passed independent RAW conversion, D0 and 50
consecutive CRC messages. DL32 identification remains healthy, with the DLA
and source released. All 1248 product source-file hashes, HEADs, existing
differences and Linux installation hashes remain unchanged.

Guest-scheduled time includes possible DPC/ISR execution and host pauses of
the vCPU; it is not measured physical CPU time. Guest Waiting does not locate
the cause inside WCH, the kernel driver, QEMU, VMware or the device. Stock
tracerpt cannot decode all Thread-v3 metadata here: the retained raw payload's
PID/TID prefix alone is read with its pinned Microsoft schema; CSwitch-v4 and
ReadyThread-v2 use native decoding. Instrumentation changes timing and this
fixed cohort does not estimate its overhead or a failure rate. ETL, SDK logs,
commercial DLLs and captures remain private. This publishes diagnostic evidence,
with AUD-006 still open; no speculative product change is promoted.

Schema/clock references: [Microsoft CSwitch](https://learn.microsoft.com/en-us/windows/win32/etw/cswitch),
[ETW QPC context](https://learn.microsoft.com/en-us/windows/win32/etw/wnode-header),
[Microsoft ThreadTraceData schema](https://github.com/microsoft/perfview/blob/9f071a5cb2e70cdfee13abc510344ab1e8840c90/src/TraceEvent/Parsers/KernelTraceEventParser.cs).

## Follow-up: imported Win32 call boundaries

The retained ETW trace reports the same nonspecific wait-reason code for all
382 RAW-bearing reads. A new private observer therefore brackets the SDK's
LocalAlloc, DeviceIoControl, memcpy and LocalFree imports. It changes only four
import slots in the SDK mapped into the isolated helper process; no instruction
bytes, on-disk commercial DLL, product code or launcher are changed. Logs remain
in memory until Close. The observer stays mapped for the private process's
lifetime. DLL SHA256:
`268f3a97d201258ff8cb34301f045dfacf2fb8aa0e9c776d134aa1bf211e3240`.

Guarded native fixtures verified all seven SDK exports and four imported
function ABIs, arguments, buffers/counts, LastError, partial failure, successful
BOOL value 2, thread IDs, loss rejection and existing-log refusal. A load-only
test of the actual commercial SDK calls no SDK/device export: only those four
import slots change, other imports/instructions and executable-page protections
remain intact. The full wrapper and hashed transfer manifest pass with a fake SDK.

The fixed physical cohort was one Windows 5 MS/s/100 MB control with this new
instrumentation, one 50 MS/s/400 MB diagnostic, then protected detach, normal
Windows shutdown and Linux 5 MS/s/100 MB. The same source connection remained
open within this cohort. No buffer, timeout, priority, firmware or wiring changed;
no acquisition was retried. **All three cases passed their bounded data checks.**
The single 50 MS/s diagnostic has normal D0 periods and 20 consecutive CRC
messages `801501`–`801520`, with exact independent all32 RAW conversion.
This positive observation does not remove the earlier retained failures or
establish a fix, a failure rate or arbitrary all-channel continuity.

For the 382 SDK reads represented in retained RAW at 50 MS/s:

| Observed interval | Total |
| --- | ---: |
| ReadEndP wall time | 2981.35011 ms |
| LocalAlloc bracket | 12.16568 ms |
| DeviceIoControl bracket | 2889.37615 ms (96.9150%) |
| memcpy bracket | 33.07392 ms |
| LocalFree bracket | 22.85785 ms |
| Residual, including observer overhead | 23.87651 ms |

The memory-operation brackets sum to 68.09745 ms, or 2.2841% of this observed
ReadEndP time. This does not demonstrate their causal role in the earlier
failures or qualify an allocator bypass. The synchronous DeviceIoControl
boundary includes work/wait below the user SDK; it does not identify a cause
inside the Windows driver, USB stack, QEMU, VMware or the device. No new ETW is
recorded here, so the earlier Waiting/Ready proportions are not transferred
onto these new calls. All brackets include instrumentation costs; their times
are not physical CPU execution or bus utilization.

Both SDK/API logs are complete and bound by PID/TID/QPC, with no unowned import
calls. All 383 positive kernel sample-IN completions already match retained
RAW 400556032 B followed by the separate 1 MiB unconsumed tail. ARM to last-RAW
completion is 3.024131 s: 2.324913 s with sample-IN pending, 0.699218 s without,
at most one pending request, longest absence 5.022 ms. Absolute timestamps from
the two clock domains are not subtracted. The extra unconsumed read (7.03999 ms)
and the 5 MS/s control's final zero-payload read (998.89807 ms) are excluded from
their RAW-bearing timing totals and preserved separately.

The Linux return passed RAW/D0/50 consecutive CRC messages and cleanup. DL32
remains healthy; Windows is stopped and both DLA and source are released.
All 1248 product files, HEADs, existing differences and installed Linux binaries
remain unchanged. Only this evidence is published; instrumentation, raw traces
and commercial material stay private. AUD-006 remains open beyond the measured
Win32 boundary, without a speculative product change.

API contracts: [DeviceIoControl](https://learn.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-deviceiocontrol),
[module lifetime](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandleexw).

## Follow-up: host request phases

A further fixed cohort at 16:08–16:09 UTC adds native QEMU 11.1.2 USB/xHCI
events to the same private SDK/API observer. Linux is in VMware; Windows is
the nested QEMU/KVM guest. The unchanged VM utility starts QEMU with only
the temporary addition of `-msg timestamp=on`, retaining the qualified reset
guard. Selected events go to private tmpfs during acquisition and are retired
before copying the log. No product, SDK file, firmware, wire, buffer, timeout
or priority is changed.

Before physical use, 21 virtual control/interrupt completions establish the
native timestamp format and packet/transfer relation. Synthetic request-lifetime
and conservation checks pass, including address reuse, plus 11 rejection cases.
QEMU UTC and kernel usbmon realtime timestamps are checked directly for causal
order; host clock-offset bounds are sampled at 100 Hz. These checks do not
guarantee every possible clock adjustment between samples. No absolute Windows
QPC-to-host subtraction or earlier ETW Waiting/Ready attribution is used.

One Windows 5 MS/s/100 MB control, one 50 MS/s/400 MB diagnostic and the subsequent
Linux 5 MS/s/100 MB return all pass independent RAW conversion, D0, consecutive
CRC messages and cleanup. The same source connection stays open through this
cohort, without retry. The 50 MS/s case has 20 consecutive messages
`802501`–`802520`. Earlier failed captures remain failures; this positive case
does not establish a fix or arbitrary all-channel continuity.

All 383 positive kernel sample-IN completions and four full 2048-byte command
transactions match their QEMU request lifetimes and xHCI completions. Kernel
payload is exactly retained RAW 400556032 B followed by the separate 1 MiB
unconsumed tail. The 382 RAW-bearing requests are serialized, with these
host-clock intervals:

| Observed interval | Total |
| --- | ---: |
| xHCI transfer start → backend data-request marker | 6.917 ms |
| Backend marker → kernel submission | 38.974 ms |
| Kernel submission → kernel completion | 2327.368 ms |
| Kernel completion → QEMU callback/copy marker | 142.187 ms |
| Callback/copy marker → xHCI success marker | 7.362 ms |
| Between one xHCI success and the next transfer start | 432.774 ms |
| First xHCI start → last RAW-bearing xHCI success | 2955.582 ms |

The first six rows conserve the complete window exactly; the longest interval
between requests is 5.169 ms. The separate Windows clock records 2915.16430 ms
across 382 RAW-bearing ReadEndP calls, including 2828.02838 ms inside
DeviceIoControl. Their endpoints differ from the host window; these totals are
not subtracted across clocks. The unconsumed read stays outside both RAW totals.

Kernel-pending time is not measured wire utilization. The post-kernel bracket
includes libusb callback dispatch, QEMU copying/logging and scheduling. The xHCI
success marker follows unmapping but precedes guest event reporting; it does
not prove that Windows handled an interrupt. Inter-request gaps can include
guest/driver work, host emulation and scheduling. Instrumentation costs and a
failure rate are not estimated. The native log backend has no global dropped-event
counter: completeness is checked for the matched kernel requests/commands,
without claiming globally zero log loss.

After protected detach and normal Windows shutdown, the Linux return and DL32
query pass; DLA and source are released. All 1248 product files, HEADs, existing
differences and installed Linux binaries remain unchanged. This further bounds
the measured host path but does not isolate a DLA/bridge/VMware cause, prove
physical buffer capacity or justify an allocator/priority change. AUD-006 stays
open. Only this evidence is published; raw traces and instrumentation stay private.

Trace definitions: [QEMU 11.1.2 USB events](https://github.com/qemu/qemu/blob/v11.1.2/hw/usb/trace-events),
[host backend](https://github.com/qemu/qemu/blob/v11.1.2/hw/usb/host-libusb.c),
[xHCI markers](https://github.com/qemu/qemu/blob/v11.1.2/hw/usb/hcd-xhci.c),
[usbmon timestamp source](https://github.com/gregkh/linux/blob/v7.2.9/drivers/usb/mon/mon_bin.c).

## Follow-up: replay boundaries in saved captures

An offline review rechecks three retained Windows 50 MS/s failures and two
positive controls. It reads D0 directly from RAW lane bits and independently
compares every edge with LOGIC, validates the numbered UART packets against
both saved decoders, and rechecks all 383 positive kernel INs against RAW plus
the separate tail in each case. No new acquisition or product change is made.
Six offline fixtures cover chunk-edge transitions, exact suffix boundaries,
partial markers and periodic/static data without distinct marker evidence.

For the three failures, the first abnormal D0 rising-to-rising intervals are:

| Source counters | Closed D0 sample bracket | Aligned exact suffix at 128 MiB distance |
| --- | --- | ---: |
| 895xxx | 7092199–7092251 | 131564544 B |
| 930xxx | 57423871–57423877 | 36086784 B |
| 8985xx | 47986642–47986704 | 73835520 B |

The latter two aligned suffixes start at RAW offsets 229695488 and 191946752,
respectively: both are **57344 B (56 KiB) inside a single 1 MiB kernel
completion**, within the corresponding first D0 bracket. Their preceding
32-byte blocks differ at the same comparison distance. All three first D0
brackets permit this common aligned request-relative position; the older
895xxx bracket does not fix an exact splice. Each second abnormal D0 bracket
contains sample 67108864, the RAW 256 MiB boundary.

The exact suffixes contain six, two and three complete repeated CRC markers,
respectively, each separated by 33554432 samples. This avoids treating periodic
clock/static-lane equality alone as replay evidence. The two positive controls
have no D0 anomaly, no such repeated markers and no aligned suffix in the same
comparison extent. The observations already occur inside individual kernel
completions; reordering whole 1 MiB completions or RAW conversion alone cannot
explain this internal witness boundary. They do not identify its generating layer.

A separately rechecked historical Linux cohort at **480 Mb/s** contains replay
and other internal D0 positions, including a finite zero-pause failure and an
explicit 700 ms paused failure; its below-extent paused control passes. These
are different transport/test conditions, with no saved kernel payload trace
for that cohort. They neither make 56 KiB a universal device boundary nor prove
that both cohorts have the same cause. All eight original captures and their
failures remain unchanged. This supplies a more specific reproduction signature,
without inferring physical buffer capacity, repairing data or closing AUD-006.

## Remaining limits

A separate cold Linux 5 MS/s capture after hub recovery also returned 100 MB
with incorrect D0/D22 witnesses despite exact RAW conversion and 253 source
acknowledgements. Later Windows/Linux 5 MS/s controls pass with the same wires
and firmware. Its cause remains open; no rate metadata or samples were repaired.

The existing [Linux first-data/Stop investigation](linux-validation-20261102.md)
and the Windows initial zero-byte start remain open and distinct from the
reset-triggered identification loss. Private early-trim and pre-Stop diagnostic
candidates are not promoted by these tests. No retries, timing workarounds,
reordering or silent data removal were added to the product.

The independently powered new ESP32 hub-button controller passed four paired
off/on cycles. Hub cycling also affects the source and other USB peripherals;
it is explicit laboratory recovery, never an automatic response to a timeout.
After later cycles, source-only USB reauthorization restored the v8 UART;
electrical capture witnesses were checked separately. Private captures,
commercial DLLs, VM disks and authentication material are not published.
