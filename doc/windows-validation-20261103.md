# Windows validation and USB handover limits — 2026-11-03

Windows hardware testing resumed on November 3. This updates the earlier
owner-deferred Windows status for the checks listed below. It does **not**
qualify arbitrary long Stream captures at 50 MS/s: the last additional run
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

## The final additional 50 MS/s run failed integrity checks

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
