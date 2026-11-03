# Acquisition worker startup: retained data and qualification limits

2026-11-03 UTC. Linux runs in VMware; Windows runs in nested KVM.
This is a validation report. The Session candidate described below remains
private and unqualified for delivery. No application, library, driver or
firmware was replaced by this work.

## Reproduced data loss

The current Session::start_capture() clears the previous samples, triggers,
frame outcomes and sample origins before constructing its acquisition thread.
If thread creation throws, the error is reported and the Session stops, but
its previous recording is already gone. Its acquisition start time also
changes. This is a confirmed host application defect, independent of USB.

Three fixtures exercise the actual Session and thread runtime: a complete
three-frame collection, a complete collection with source origins above 2^53,
and a collection retaining complete and partial/error frames. One selected
pthread_create call returns EAGAIN. All three preservation contracts fail
with the original Session on each OS. A single error reaches the caller;
the original recording and its metadata are lost. A fresh replay on the same
Session succeeds. This is controlled process-local fault injection, not a
claim about system-wide resource exhaustion.

The minimum private candidate moves the existing data reset into the worker,
restores the previous start time when construction fails, and resets a real
device's display name only after construction succeeds. It adds no worker,
wait gate, transport change or persistent option. The same three contracts
pass on both OSes: bytes, frames, outcomes, errors, triggers, origins, saved
state, name and start time remain exact. Fresh replay succeeds afterward.

Linux injection interposes pthread_create in the diagnostic executable.
Windows injection selects libstdc++'s pthread_create import in that process;
the original import and page protection are restored. No SDK or system DLL
file is modified. The full source snapshot has 408 files and differs from
the current product in Session.cpp only. Candidate Session SHA-256:
`8ff28276a7b1fff20c31f38f73495e37248916e797bf35444925dc06aaba2cab`.

## Software controls

All 78 Linux frontend regression cases pass, including Stop during startup,
repeated captures, software windows, partial outcomes, file replay and export.
Windows builds both the application and tests from the same 408-file snapshot.
One unchanged toolbar visibility case fails six assertions under Qt offscreen.
Relinking with the original Session reproduces exactly the same six assertions;
the rest of the 78-case run has no reported failure. The Windows suite is
therefore not described as wholly passing. This comparison does not qualify
the outstanding Windows toolbar issue or the whole native GUI.

## Physical controls and failures

The DLA-32 is 1a86:5537, outer Linux path 4-2, negotiated 5000 Mb/s. The
existing source remains v8, GPIO32/D0 at 1 MHz/50%, camera enabled, SD disabled;
D22 observes the numbered UART commands. All 32 channels are enabled, threshold
1.6 V, no trigger. Source commands are spaced 50 ms apart in the Session tests.
The source, wires, firmware and hub state are preserved. Linux and Windows
never capture simultaneously. No reset, flash or driver replacement is used.

The fixed candidate cohort contains three captures per OS: finite Stream at
5 MS/s/5M samples, Stream at 5 MS/s/25M target stopped at 300 ms, then Buffer at
5 MS/s/5M samples. After the first capture, refusing a new worker preserves the
exact unsaved hardware recording, its complete outcome, name and start time on
both OSes. The refused start is not counted as a physical capture.

- Windows Stream and cancelled Stream pass the bounded RAW/D0/D22 checks.
  Buffer returns the requested 20 MB and 19 consecutive CRC-valid UART messages,
  but D0 has one three-sample rising-edge period at sample 4,988,160; the allowed
  range for this witness is 4–6 samples. It fails the electrical witness check.
- Linux Stream and cancelled Stream pass. Buffer receives 4 MiB and then reports
  a sample-transfer timeout. That RAW-derived prefix has 343 bad D0 periods,
  no recognized CRC messages and disagreeing UART decoders. It is a failure.
- One additional fixed three-capture Linux control uses the original Session
  and the same sequence. Stream and cancellation pass; Buffer receives 8 MiB
  and times out, with 341 bad D0 periods and no recognized CRC messages. Its
  saved partial LOGIC matches an independent conversion of RAW exactly. The
  startup candidate is thus not required for a Buffer failure in this setup;
  this does not establish the cause of either stall or the Windows anomaly.
- One final Stream return at 5 MS/s/25M samples succeeds with the unchanged
  installed Linux core: 100 MB, independent all32 RAW conversion exact, two UART
  decoders agreeing on messages 822001–822050, zero bad D0 periods, normal
  HEADER/END and quiet cleanup. Windows is off and the DLA/source are released.

Ten physical captures were attempted, with no retries: seven pass the bounded
checks and three fail. All successful saved LOGIC payloads match independent
RAW conversion in original order. Fetched RAW tails remain preserved separately
from delivered data. The first failed Buffer diagnostic did not save its
in-memory LOGIC before exiting: its 4 MiB RAW is original, and a subsequently
derived file is explicitly labelled rather than presented as original LOGIC.
The control diagnostic saves the actual partial LOGIC and timeout metadata.
Source acknowledgments are retained; the original-Session control splits one
acknowledgment across consecutive serial and postflight files, read in order.

Windows records the actual loaded DLL paths to the existing product prefixes;
DLL hashes were not newly sampled during this cohort. Historical runtime hashes
are not presented as a new runtime hash measurement. Linux loaded paths and
unchanged installed library hashes are checked. No arbitrary all32 electrical
continuity, high-rate qualification, queue capacity, deadline guarantee or
new firmware conclusion follows from these tests.

The candidate stays private. Buffer transition/integrity, AUD-006 and first-data
failures remain open; the new partial-data stalls are distinct from the earlier
zero-byte cases. No failure was hidden by a successful retry or transport reset.

## Follow-up: short Buffer repetition is present in the kernel trace

2026-11-03, after the fixed cohorts above. A new Linux-only control adds passive
usbmon to the original Session's unchanged Stream → cancellation → Buffer
sequence, followed by one Stream return. There is no added wait/gate between
the three Session captures. The installed core remains `84c0c7da`, the frontend
Session remains the original `64e41f8d`, USB remains 5000 Mb/s at 4-2/dev16.
The source, wires, firmware and hub state are preserved; Windows stays off.
This is four additional captures with zero retries, not a positive replacement
for any of the earlier failures. The complete retained local source snapshot
has 477 core files, repo HEAD `016c813c` with its existing local modifications;
this measurement is scoped to that snapshot and installed binary.

The passive device-filtered parent trace contains 370 events/185 closed URBs,
128,974,848 sample-payload bytes, and zero dropped, queued, truncated or failed
monitor events. Four contiguous derived scopes preserve every record in order.
Their counters are explicitly derived from that loss-free parent, not presented
as four new monitor measurements. Discovery/control bytes remain preserved.
All saved RAW matches the corresponding kernel completion bytes exactly; sample
completion order also matches submission order in every scope. No sorting or
sample repair is used.

| Capture, 5 MS/s/all32 | Saved LOGIC | Result |
| --- | ---: | --- |
| Finite Stream, 5M target | 20,000,000 B | D0 bad0; CRC823006–823024, both UART decoders agree |
| Stream, 25M target, Stop300ms | 3,145,728 B | stopped; D0 bad0; CRC823029–823031, decoders agree |
| Buffer, 5M target | 4,194,304 B | timeout;257 bad D0 periods, no recognized CRC message, decoders disagree |
| Finite Stream return, 25M target | 100,000,000 B | D0 bad0; CRC824001–824050, decoders agree |

Independent all32 RAW conversion matches all four saved LOGIC payloads,
including the failed Buffer's actual partial recording. Fetched RAW beyond the
finite LOGIC target remains in the original trace: 971,520 B for the first Stream
and 663,296 B for the return. There are no other positive sample bytes outside
the saved RAW. Source acknowledgments are complete. The final return, END,
quiet cleanup, released DLA/source and powered-off Windows are verified.

The Buffer's first and fourth 1 MiB blocks are identical byte for byte, at RAW
offsets 0 and 3,145,728. The same equality is present in two distinct initial
kernel IN requests, both submitted before ARM completed. Their payload SHA-256
is `2595719cc93e32cdec1ecbcb0b8189b160aac9dee1623ed7a10da55b70dfa9c9`.
All four accepted requests complete successfully with 1 MiB each. The first data
arrives 677,180 µs after ARM completion, the fourth at 694,719 µs. Then no more
positive sample data arrives before cleanup.

The last data completion precedes the first cancellation retirement by
3,045,450 µs and the Stop submission by 3,046,252 µs. IN requests remain pending
throughout the interval before cleanup, with maximum depth 8. After the last
cancellation, only 80 µs has no pending IN before Stop. Thus this stall does not
show a missing-read-request gap before cleanup. Two post-Stop reads are empty;
there is no unaccounted late sample payload.

The retained reader passes ten contracts covering byte mismatch, missing or
cross-scope ownership, trace truncation/loss, command CRC, cancelled partial
tails, empty RAW and an intentionally out-of-order completion fixture. That
fixture is reported out of order rather than sorted into a passing sequence.
An initial analysis incorrectly required pending reads until the Stop packet;
the corrected report includes the measured 80 µs cleanup gap explicitly.

The repeated payload that fails the current waveform checks is already visible at the outer Linux VMware guest's
kernel observation, before frontend conversion and storage. This excludes a
frontend copy or conversion error as a necessary explanation for these specific
repeated bytes. It does not separate device/FPGA, USB, hypervisor or host-kernel
causes, or establish whether Buffer output obeys the assumed format in this
transition. With no recognized numbered UART message in the bad prefix, it also
does not identify the physical times or acquisition from which bytes originated.
No queue capacity, universal deadline, native physical-Linux qualification or
arbitrary all32 electrical integrity is inferred. No source correction is
promoted; AUD-006/Buffer and the private AUD-025 qualification remain open.
