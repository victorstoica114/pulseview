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

## Follow-up: one pending IN and omission of the early-stopped capture

2026-11-04 UTC. Two fixed Linux-only controls test separate explanations for
this short Buffer failure. They retain the original Session, the same source
v8/PWM1MHz50% and wires, and USB4-2/dev16/5000Mb/s. Linux is the outer VMware
guest; Windows stays off. There is no hub toggle, reset, firmware operation,
added transition delay or successful retry replacing a failure.

First, a private driver copy changes only `NUM_TRANSFERS` from8 to1 relative to
the retained 477-file source of installed core `84c0c7da`. It inherits the same
three excluded Windows-only WCH lines; these are not promoted. Compiler flags
and `config.h` match the original build. The private core is SHA-256
`9e059c98ae0e183b8f52bb05e7363dd2c998b8df7872ba3fafc16bdfdbdd531f`.
Actual native process mappings select it and its matching C++ library. All122 C
checks, the C++ cleanup/retry check,78 existing frontend regressions and10 reader
contracts pass. This is a private Linux experiment, not a shared-code fix or a
Windows release qualification.

The first fixed cohort repeats the original Stream → early Stop → Buffer
sequence with that private driver, followed by a Stream return with installed
core `84c0c7da`: four captures, zero retries. The second cohort retains installed
core `84c0c7da` throughout and changes only the private fixture loop to omit the
explicitly early-stopped acquisition: complete Stream → Buffer, then Stream
return. Its original Session and every other frontend object are retained.
This is three captures, zero retries. Normal USB retirements at finite capture
completion still occur; the second control does not remove all USB cancellation.

| Control, 5 MS/s/all32 | Saved LOGIC | Outcome and bounded witnesses |
| --- | ---: | --- |
| One-IN Stream, 5M target | 20,000,000 B | complete; D0 bad0; CRC825006–825023 |
| One-IN Stream, 25M target, Stop300ms | 3,145,728 B | stopped; D0 bad0; CRC825029–825031 |
| One-IN Buffer, 5M target | 4,194,304 B | timeout;172 bad D0 periods; no recognized CRC message |
| Installed-eight-IN Stream return, 25M target | 100,000,000 B | complete; D0 bad0; CRC826001–826050 |
| No-early-stop Stream, 5M target | 20,000,000 B | complete; D0 bad0; CRC827006–827024 |
| No-early-stop Buffer, 5M target | 4,194,304 B | timeout;172 bad D0 periods; no recognized CRC message |
| Installed-eight-IN Stream return, 25M target | 100,000,000 B | complete; D0 bad0; CRC828001–828050 |

Two independent UART decoders agree on every passing row's consecutive CRC
messages; they disagree on the failed Buffer rows. Independent all32 RAW
conversion matches all seven actual saved LOGIC payloads, including both partial
failures. Source acknowledgments, finite RAW tails and original failure metadata
are retained. No data is repaired, cropped into success or reordered.

The passive traces record328 events/164 closed URBs with128,974,848 sample bytes
for the one-IN cohort, and334 events/167 closed URBs with125,829,120 sample bytes
for the no-early-stop cohort. Both have zero monitor drops, queued events,
positive-payload truncations or monitor failures. Every saved RAW byte equals its
kernel payload; positive sample completions are in submission order. All other
sample bytes are accounted for. Each capture's four complete2048B commands
(pre-Stop, setup, ARM and Stop) are byte-identical to the matching preceding
eight-IN reference. This comparison does not constitute hardware readback.

The observed maximum pending sample-IN depths are1/1/1/8 and8/8/8 respectively.
With one IN, the four Buffer data requests are sequential, the next submitted
after the preceding completion. The identical first/fourth1MiB relation remains
in their kernel payloads. Multiple concurrently pending sample INs in this Linux
guest are therefore not necessary for this observed failure. Lowering the
transfer count does not repair it. The macro also changes private allocations
and timing; this sequential comparison is not a randomized reliability estimate.
The failure without the early-stopped acquisition likewise shows that preceding
early stop is not necessary. Ordinary finite-completion cleanup remains present.

Offline comparison of the three retained4MiB Buffer RAWs (preceding eight-IN,
new one-IN and new no-early-stop controls) finds exact equality at a49152-byte
shift over each whole file: `RAW[:-49152] == RAW[49152:]`. Each RAW is already
bound byte for byte to its kernel trace. Thus the previously reported3MiB
first/fourth-block equality is consistent with a shorter48KiB pattern:3MiB is
64 times48KiB. This does not prove the minimum period, a3MiB buffer, physical
temporal replay or the originating acquisition. No numbered UART message in
these bad prefixes establishes that time origin. The all-file RAW hashes are:

- Eight-IN reference: `7f1a90de981502631122059e91ccd9ba44dd9929ccb98e41a48a66ff6e45115b`.
- One-IN: `7210cb35c561e84fbfda73e79e90366b4c3bb7ecab983de77068ed5bbdd0e857`.
- No early stop: `3727e7a5ac00ed2aecf79b1982f809c65df7c75afa5e4051068b428658a8aca4`.

These controls narrow the explanations without identifying a device/FPGA,
Buffer-format, USB, virtualization or host-kernel cause. They do not demonstrate
arbitrary all32 electrical continuity or resolve the separate zero-byte cases.
All1248 product-source files, their HEAD/status/diffs and installed Linux binaries
remain unchanged. Only this validation document is published; the one-IN driver
and AUD-025 candidate remain private. Both cohorts finish with a verified100MB
Stream return and released DLA/source. Buffer/AUD-006 and AUD-025 qualification
remain open.

## Follow-up: compare the preceding mode while holding its target at25M

2026-11-04 UTC. Two further fixed Linux VMware cohorts compare the acquisition
preceding the short Buffer capture. Each is limited to three captures with no
retry and finishes with an installed-driver Stream return. The original Session,
all other frontend objects and installed eight-IN core `84c0c7da` are retained.
Only private diagnostic fixtures change; there is no new product source or
Windows binary. Source v8/PWM1MHz50%, wiring, firmware, hub and USB4-2/dev16 at
5000Mb/s are preserved. Windows stays off, with no simultaneous acquisition.

The first cohort runs Buffer25M → Buffer5M in the same Session. Both complete
and pass their delivered-data witnesses. The second changes only the first
capture's data source and diagnostic label: Stream25M → Buffer5M. The first
target remains25M, the second5M, both at5MS/s/all32, without a trigger. There is
no added delay/gate, reset, fault injection or dummy capture in either cohort.
A prior Stream acquisition precedes these cohorts; first-in-Session does not
mean no device history.

| Cohort/capture at5MS/s | Delivered LOGIC | Outcome and bounded witnesses |
| --- | ---: | --- |
| Buffer predecessor, Buffer25M | 100,000,000 B | complete; D0 bad0; CRC829006–829099,94 consecutive |
| Buffer predecessor, Buffer5M | 20,000,000 B | complete; D0 bad0; CRC829132–829150,19 consecutive |
| Stream return25M | 100,000,000 B | complete; D0 bad0; CRC830001–830050,50 consecutive |
| Stream predecessor, Stream25M | 100,000,000 B | complete; D0 bad0; CRC831006–831099,94 consecutive |
| Stream predecessor, Buffer5M | 4,194,304 B | timeout;172 bad D0 periods; no recognized CRC message |
| Stream return25M | 100,000,000 B | complete; D0 bad0; CRC832001–832050,50 consecutive |

Five of six captures pass the bounded witnesses; one fails. Two independent
UART decoders agree on every passing row and disagree on the failed Buffer.
Independent all32 RAW conversion matches all six actual saved LOGIC payloads,
including the partial failure. Complete source acknowledgments and original
failure metadata remain retained. A diagnostic exit status of zero means its
fixed sequence finished and saved the results; it does not turn the recorded
Buffer timeout into a passing capture.

The first loss-free passive trace contains520 events/260 closed URBs and
224,395,264 sample bytes; the second486 events/243 closed URBs and205,520,896
sample bytes. Both have zero monitor drops, queued events, positive-payload
truncations or failures. All RAW bytes equal the original kernel completion
payloads, positive sample completions match request order, and all positive
sample bytes are accounted for. Measured maximum pending sample-IN depth is8
in each phase. The identical retained readers reuse their ten previously passed
contracts by hash; no changed reader is validated by an old test result.

For the first25M capture, the four complete2048B commands differ between the
two cohorts only in setup: its body changes only the Buffer flag at packet
byte13 (1→0), with the corresponding CRC change at bytes41–44. All other bytes
of setup and all pre-Stop/ARM/Stop commands are identical. Every complete2048B
command for the following Buffer5M is identical across both cohorts, as are the
final Stream-return commands. The first long Buffer's four commands also match
the previous successful Windows/WCH25M control; that historical comparison is
not a new Windows capture or platform qualification.

The successful Buffer RAWs contain100,663,296 B and20,971,520 B; thus663,296 B
and971,520 B exceed their delivered finite targets and remain preserved. Each
Buffer also has1MiB of positive post-Stop data retained separately in its kernel
sidecar, followed by two empty reads. No positive post-Stop bytes occur in the
failed Buffer or either Stream return; finite RAW tails of the Stream captures
remain preserved separately from delivered LOGIC. The passing D0/UART claims concern the
delivered LOGIC only. For example,14 bad D0 periods occur outside the long
Buffer's delivered target in its RAW tail; the separate post-Stop blocks also
have waveform anomalies. Their physical time origins are not established, and
no arbitrary all32 electrical integrity is claimed for these extra bytes.

The failed short Buffer again has the exact49152-byte pattern across its whole
4MiB RAW, already present in kernel payload. The successful Buffer prefixes do
not have that equality. Numbered UART data still does not establish a physical
time origin in the failed prefix; periodic bytes do not prove temporal replay,
queue capacity or a particular firmware mechanism.

Holding the preceding target at25M strengthens the observed association with
the Stream→short-Buffer transition: the short target succeeds after Buffer and
fails after Stream in these two cohorts. This fixed sequential comparison is
not randomized or replicated in reverse order. Prior device state, order and
timing remain possible confounders, and changing mode also changes the real
acquisition, upload and cleanup behavior. It does not isolate a unique device,
format, transport, virtualization or kernel cause. No forced long target,
automatic priming capture or other workaround is added to the product.

All1248 product-source hashes, HEAD/status/diffs and installed Linux binaries
remain unchanged. Both fixed plans are closed; the failures and all extra data
are retained. Windows is off and DLA/source are released. AUD-006/Buffer and the
private AUD-025 qualification remain open; only this documentation is published.

## Follow-up: reverse the cohort order and retain a strict UART-witness failure

2026-11-04 UTC. A separate fixed six-capture plan reverses the preceding pair's
order: Stream25M→Buffer5M→Stream return first, then Buffer25M→Buffer5M→Stream
return. Each target is at5MS/s/all32. The exact previously tested native
executables are reused without rebuilding; original Session, installed eight-IN
driver, source v8/PWM1MHz50%, firmware, wiring, hub and USB4-2/dev16/5000Mb/s are
unchanged. Windows stays off. Both plans close without retries, added waits,
resets or any product change.

| Reversed order/capture | Saved LOGIC | Outcome and strict witness result |
| --- | ---: | --- |
| First cohort, Stream25M | 100,000,000 B | complete; D0 bad0;94 consecutive CRC833006–833099; pass |
| First cohort, Buffer5M | 4,194,304 B | timeout;172 bad D0 periods; no recognized CRC; fail |
| First Stream return25M | 100,000,000 B | complete; D0 bad0;50 consecutive CRC834001–834050; pass |
| Second cohort, Buffer25M | 100,000,000 B | complete; D0 bad0;94 consecutive CRC835006–835099; pass |
| Second cohort, Buffer5M | 20,000,000 B | complete; D0 bad0;18 consecutive CRC835132–835149 agree; strict UART witness fails |
| Second Stream return25M | 100,000,000 B | complete; D0 bad0;50 consecutive CRC836001–836050; pass |

The result is four strict passes and two negatives, not five passes. For the
completed short Buffer, both UART decoders produce515 bytes but differ at byte
indices0 and6. The first recognized complete numbered message begins at decoded
byte11; every subsequent decoded byte is identical. Both decoders recognize the
same18 consecutive CRC-valid messages, with no gap, duplicate or malformed
interior line. The independent decoder also reports two frame warnings at
sample ranges493–536 and3296–3339. These bytes and warnings remain retained.
The fixed acceptance required whole decoded-byte equality and no warnings, so
this recording stays a strict witness failure. No prefix is removed and no
criterion is relaxed retrospectively. Starting inside a UART frame is consistent
with this localized discrepancy but is not asserted as a proven physical cause.

For all six captures, independent all32 RAW conversion matches the actual saved
LOGIC, including the timeout's partial recording. Source acknowledgments are
complete. Two loss-free passive parents contain486 events/243 closed URBs with
205,520,896 sample bytes, then520 events/260 closed URBs with224,395,264 sample
bytes. Monitor drops, queued events, positive truncations and failures are zero.
Every RAW byte equals kernel completion payload, positive completions match
request order, all other positive bytes are accounted for, and maximum pending
IN depth remains8 in every phase. Each capture's four complete2048B commands
match its corresponding earlier control byte for byte. Original RAW tails and
both1MiB post-Stop Buffer sidecars remain separate and preserved; passing witness
claims do not qualify those extra bytes electrically.

The Buffer timeout after Stream again has the exact49152-byte pattern across
its whole4MiB RAW, already in kernel payload. Thus the acquisition outcomes
(timeout after Stream, complete target after Buffer) persist when the prior
cohort order is reversed. Full positive witness acceptance after Buffer is not
replicated because of the recorded UART-prefix discrepancy. AB followed by BA
is still a fixed sequential control, not randomized reliability qualification
or a unique root-cause proof. Earlier device history, mode timing, upload and
cleanup effects remain possible factors. Temporal replay, queue capacity and
arbitrary all32 electrical continuity are not established.

All1248 product-source hashes, HEAD/status/diffs, retained477-file runtime source
and installed Linux binaries remain unchanged. Both final100MB Stream returns
pass, and DLA/source are released with Windows off. No workaround, forced long
target, priming capture or private AUD-025 candidate is promoted. Buffer/AUD-006
and AUD-025 qualification remain open; only this documentation is published.

## Follow-up: offline UART-boundary reproduction, without changing the verdict

2026-11-04 UTC. No new hardware capture is made. The completed20MB Buffer with
its strict UART-prefix failure is analysed from its original RAW/LOGIC, D22
projection, annotations and source records. All input hashes remain exact, and
an offline SDK re-decode reproduces the original annotations byte for byte.
The actual loaded core `84c0c7da`, SDK `ff037c43`, CLI and unchanged UART decoder
source are checked. USB scanning is disabled; Windows stays off. No product,
installed binary, decoder behavior or capture-acceptance policy changes.

The first complete recognized message,835132, supplies an independent sampling
width calibration: its184 observed transitions fit the known emitted8N1 packet
with43.14616 samples/bit and maximum residual0.786 samples. Holding this width
fixed, the initial69 transitions are compared with suffixes of all170 commanded
and acknowledged source packets, fitting only their offset. There is one match:
packet835131, with maximum residual1.000 sample. The fitted capture start is
bit160.659 of that packet, inside the start bit of byte16 (zero-based). The
predicted suffix ends near sample5149. The diagnostic uses an explicit2-sample
edge bound and a±2% nominal-width interval; these are model limits, not hardware
specifications or a relaxed version of the old acceptance rule.

A known-valid synthetic8N1 waveform generated with the fitted sampling/phase
values reproduces the exact first11 decoded bytes of the real recording:

| Decoder | Initial11 bytes, hex |
| --- | --- |
| Simple independent decoder | `8a 6a aa 82 92 32 39 30 32 38 0a` |
| SDK UART decoder | `a6 6a aa 82 92 32 e5 30 32 38 0a` |

The synthetic input also produces two SDK frame warnings, at samples493–536
and3297–3340, versus493–536 and3296–3339 in the real record. The second warning
is shifted by one sample. Both decoders then recover the next complete packets
and agree. A generated control with initial idle instead decodes identically
without warnings. The SDK emits a data annotation before checking its stop bit;
the simple decoder tests the stop level before accepting a byte. With a record
starting inside a frame and no observed initial falling edge, these different
resynchronization paths can therefore produce the observed prefix discrepancy
without corrupting the generated waveform. This calibrated reproduction is
not a second electrical measurement or a unique physical-cause proof.

Thirteen offline contracts pass, including separate rejection of a changed
prefix bit, missing immediate-next or interior messages, duplicates, reordering,
bad CRC, malformed interior bytes, bad stop bit, no full calibration packet,
stale packets and duplicated source commands. The diagnostic does not merely
ignore everything before a later good CRC: it requires a unique initial suffix,
a consecutive next counter and no unexplained intervening events. On retained
real controls, it recognizes the earlier clean Buffer and rejects the4MiB
failed Buffer, which has no complete emitted message for calibration.

This is additional signal/model evidence consistent with a capture-boundary
explanation for the completed Buffer's two differing leading bytes. Every real
byte, warning and original outcome is retained. The original fixed plan remains
four strict passes and two negatives; the completed Buffer is not retrospectively
reclassified. Complete-message checks do not prove arbitrary all32 electrical
continuity, and this analysis does not explain or repair the separate Buffer
timeout. AUD-006/Buffer and the private AUD-025 qualification remain open.
