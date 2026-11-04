# Windows desktop capture and replay observations — 4 November 2026

## Update: native-output Windows50M continuity failure between Linux controls — 4 November 2026

A separate fixed cohort completed **three physical captures and zero retries**:
Linux50M control → actual native-output Windows GUI50M → the single predefined
Linux5M return. The Linux controls pass the stated bounded D0/D22 checks. The
Windows capture remains **unqualified despite400,000,000 bytes and native
complete/no-error metadata**: one numbered UART packet is absent and two D0
periods violate the predefined tolerance. The preceding Windows25M positive is
retained as its own bounded case; it does not extend to50M.

No product/installation/logger/buffer/firmware change was made. Windows used the
same actual GUI705a/corebf0a/C++a1fc/decoder6287/WCHdb19, in desktop Session1/PID4904,
with exact private C#17d809c7 and direct native STD file handles. The live PID was
published before waiting and its original CreateProcess handle was retained
through GUI exit0. The preparation's initial full-selftest refusal and explicitly
reconstructed subset remain as documented; no retroactive full-suite pass is
claimed. Linux used unchanged helperdb7c255b/core84c0c7da with LC_ALL=C.UTF-8.

| Phase | Rate / samples / saved LOGIC bytes | CRC witness | D0 result |
| --- | --- | --- | --- |
| Linux50M | 50 MS/s / 100 M / 400,000,000 | 20 consecutive,990101–990120 | zero bad; periods49–51; boundaries18/10 samples |
| Windows native GUI50M | 50 MS/s / 100 M / 400,000,000 | 19 valid packets;990514 absent | two bad periods35/66; boundaries22/7 samples |
| Linux5M return | 5 MS/s / 25 M / 100,000,000 | 50 consecutive,990801–990850 | zero bad; periods4–6; boundaries2/4 samples |

All phases used finite Stream,32 channels,1.6 V,Captures1,no hardware/software
trigger, no continuous mode and no Stream windows. The same v8 source retained
its1 MHz/50% D0 clock, D22 UART115200,camera1/SD0 and wiring. Three fresh source
ACKs within500ms preceded each Run;22/22/52 distinct live commands were sent and
all ACKed. The source remained open through real END/quiet/cleanup, then
postflight STATUS/ACK and close. Original metadata/HEADER/END and native
controller/helper exits are valid; no locale normalization or timer-as-END
substitution was needed. MCU uptime across serial connections is not asserted.

### The negative is already present in the exterior-kernel data

Windows D22 contains exactly532 decoded bytes:19 complete, CRC-valid28-byte
messages990501–990513, then990515–990520. Both independent UART decoders agree,
with zero warnings, bad CRCs, repeated numbers or malformed interior lines.
The source independently ACKed every distinct command990501–990522, including
990514. ACK/host flush is not a timestamped electrical emission guarantee.

D0 rising-edge intervals are35 samples starting at64,239,597 and66 samples at
67,108,839, versus the predefined49–51 bound at50M. Their independently mapped
RAW brackets are[256958368,256958560) and[268435328,268435648); the second straddles
the256MiB RAW boundary. These addresses localize observations, not a demonstrated
firmware/memory-capacity cause. The990513→990515 first-data-bit interval is
9,909,567 samples (0.198191340 s at the nominal rate), while the corresponding
host after-flush interval is0.198177494 s. These different observations do not
measure UART latency or prove an exact duration of missing samples.

All saved LOGIC bytes match independent RAW conversion. RAW sizes are400,556,032 /
400,556,032 /100,663,296 bytes. Kernel accounting preserves all bytes and original
order in796 /780 /224 events. Windows401,604,608 positive IN bytes are exactly
RAW plus its retained1,048,576-byte worker-unconsumed sidecar. Therefore the
observed mismatch is not introduced by GUI conversion, archive saving or replay;
it is present in data captured by the exterior Linux kernel. This does not
separate the source/device/bridge/SDK/VMware/KVM layers or identify one cause.

The specific older128MiB-shift replay candidate was checked independently:
RAW[134217728,265782272) versus[268435456,400000000) differs in7,687,738 bytes.
It is not the earlier exact131,564,544-byte repeated range. No repeated numbered
packet is observed here. This inequality does not exclude every smaller replay,
replacement or loss mechanism; periodic/idle channels are not unique time IDs.

### Cleanup, file preservation and dispatch limits

Native Save/Open/Save As preserves400,000,000 bytes and **every archive member
and metadata field exactly**, including the original complete status and the
negative payload. First SR/PVS hashes stay intact. The persistent file observer
records the current HEADER/400MB/END/Stopped, no hardware ARM and exit0.
GUI exit0 comes from the retained original process handle, its interactive task
is removed, and all27 Windows originals were copied/SHA-verified before protected
detach, normal VM shutdown and the reserved Linux return. The return then passed;
no failing phase was retried or replaced.

Original-order linkage accounts for382 accepted worker reads plus one sidecar
read. New native Windows50M has3.007789 s inside the read wrapper and0.058111 s
between calls over3.065900 s (1.8954% outside). The separately qualified native
Windows25M case had3.979658+0.029847 s over4.009505 s (0.7444% outside). Thus the
large PowerShell-output delay is not required for every observed continuity
failure. These elapsed intervals include SDK/memory/queue/scheduler work and
are not isolated CPU, logging or condition-wait measurements.

An independent USBmon sweep matches original accounting. Windows50M spans
3.064435 s ARM-completion→lastRAW-completion, with0.724083 s lacking pending IN,
maximum one pending IN and longest gap7.319 ms. Linux50M keeps pending IN for
all2.249992 s, maximum eight; Linux5M return keeps it for5.036464 s, maximum eight.
The negative's longest gap is smaller than14.117 ms in the preceding qualified
native25M case, reinforcing that a single-pause threshold cannot certify
continuity. These are kernel observation extents, not electrical acquisition
times. Guest/kernel clocks are not synchronized; sequential frontends and rates
do not isolate a transport limit or prove a universal safe rate.

Final read-only state: Windows off, DLA1a86:5537 at4-2/dev16/5000 Mb/s and source
CH3408134 at3-1.2/ttyUSB2 free and unchanged. Linux is VMware; Windows is nested
KVM. QEMU NoNewPrivs1/Seccomp2/guest-reset=false was checked before assignment and
detach. No reset, hub cycle, flash, rewiring, installation, simultaneous capture
or additional agent was used. The physical cohort is closed at its three-attempt
maximum. Both Linux controls provide70 qualified consecutive packets; Windows's
19 nonconsecutive packets are not added as a successful continuity case.

Private evidence under `artifacts/native-output-50M-20261104/` retains the fixed
plan, fresh source/product/hash/identity/resource baselines, original per-phase
RAW/LOGIC/USBmon/metadata/source/END, data/kernel/decoder reviews, negative and
UART-sequence localization, native GUI/config/replay/copy/shutdown/return gates,
dispatch/interval reviews, final review and publication verification. All1,248
product files, HEAD/status/diffs and Linux/Windows installations remain unchanged;
all408 GUI source files and the other415 public files are unchanged. This is a
documentation-only negative result, not a product fix or release qualification.
Commercial SDK/firmware, private captures, VM disks and credentials are excluded.

## Update: actual GUI log path and bounded backpressure review — 4 November 2026

An offline follow-up pinned the retained sources of the actual GUI705a and
corebf0a, not the later private worker-retirement candidate. All477 core runtime
source files match the original build manifest; GUI logging/main/settings match
the qualified408-file source set. It adds **zero physical acquisitions, zero
VM boots and no product changes**. The preceding three-capture cohort stays closed.

The actual call path is synchronous: `sr_log()` checks the selected level, then
PulseView `Logging::log_sr()` calls the previous libsigrok callback before its
own log buffer. That previous callback writes a prefix, timestamp and message
to stderr and calls `fflush(stderr)`. The GUI then formats/buffers the message
under its logging mutex and emits its notification. Qt messages instead use
`Logging::log_pv()` and stdout/`endl`; the paths are not interchangeable.

The WCH I/O worker uses two1MiB slots. Its frontend dispatch claims a completed
result, logs the read, performs RAW tracing/conversion/datafeed, then releases
the claimed slot. When both slots are busy the worker can wait on its condition
until a slot is released. Thus slow synchronous output can delay frontend
release and cause backpressure even though global logging is outside the I/O
thread. This is a verified source mechanism, **not a measured queue wait**:
retained traces do not timestamp release, occupancy or condition-wait intervals.
Increasing buffers or moving release before consumers finish is not justified
by these observations.

Original log order and per-read linkage give382 accepted result groups plus
one preserved sidecar read in both Windows25M cases. Each accepted group has
one read log, RAW payload log and LOGIC log, totaling400,000,000 saved bytes.
Differences between printed timestamps, summed over those382 groups:

| Printed log interval | Native-output positive | PowerShell-output negative |
| --- | --- | --- |
| read log → RAW payload log | 0.039379 s; median78 µs | 2.328280 s; median5000 µs |
| RAW payload log → LOGIC log | 0.240258 s; median551 µs | 2.568516 s; median5603 µs |
| LOGIC log → next read log | 3.728123 s | 3.647471 s |
| Sum across all three intervals | 4.007760 s | 8.544267 s |

These are local emitted-log intervals, separate from the embedded SDK read
spans and kernel spans in the preceding update. The `sr:` prefix is written
before taking the printed timestamp. Each difference includes remaining
callback output, GUI buffering/signals, conversion, RAW disk I/O, datafeed,
scheduling and part of the next log prefix. It is not an isolated duration of
stderr, Qt, CPU work or a condition wait. Cross-cohort timestamps are not aligned.

The core default is WARN; these frequent per-read/payload/LOGIC messages are
DBG and are filtered before callback invocation at the default level. Both
physical25M cohorts deliberately selected loglevel4 and RAW tracing. The retained
installed Windows command launcher, hash2ba8d1a2 confirmed by postflight, invokes
the GUI directly; the old private laboratory launcher used PowerShell
`Start-Process -RedirectStandardOutput/-RedirectStandardError`. Neither launcher
nor product logging was changed by this audit. This does not qualify a new
physical default-WARN run or every possible parent-process output target.

The evidence supports keeping diagnostic capture output on the qualified native
file path, without introducing a speculative product logger, queue or buffer
workaround. It still does not prove the origin of the old temporal replay,
missing packets or0B failures, guarantee25M/all32 electrical channels, or erase
any negative. Evidence under `artifacts/gui-log-path-review-20261104/` pins input
hashes, original-order per-read intervals, build/source and installed-launcher
linkage. All product files, installed artifacts and original captures remain
unchanged; Windows remains off with the DLA/source free. This is documentation
only, with no new release qualification.

## Update: bounded physical capture with native Windows log output — 4 November 2026

A new, fixed cohort completed **three physical captures, zero retries, and no
50 MS/s attempt**: Linux25M control, actual Windows GUI25M with native output,
then the single predefined Linux5M return. All three passed the stated bounded
D0/D22 witness and storage/transport checks. This is one positive Windows25M
case in this nested VM setup. It does not erase the earlier Windows25M replay,
qualify arbitrary channel timing, or establish a universal fix or safe rate.

The only planned Windows intervention was the private launcher STD output
mechanism. The unchanged GUI705a/corebf0a ran in actual desktop Session1/PID4352,
using the previously exercised exact C# source17d809c7. CreateProcess inherited
only the three explicit STD handles, wrote directly to exclusive native files,
published the live PID before waiting, and retained its original process handle
through normal GUI exit0. No PowerShell output redirection, product change,
installation, new product flag, flash, hub cycle, USB reset or rewiring was used.
The preparation cohort's initial full selftest refusal and reconstructed subset
proof remain explicit in the preceding update; this run does not convert that
initial script into a passed full suite.

| Phase | Rate / samples / LOGIC bytes | Fresh consecutive CRC packets | D0 boundary samples / bad periods |
| --- | --- | --- | --- |
| Linux25M | 25 MS/s / 100 M / 400,000,000 | 40, 980101–980140 | 10 / 16; zero |
| Windows native GUI25M | 25 MS/s / 100 M / 400,000,000 | 40, 980501–980540 | 12 / 14; zero |
| Linux5M return | 5 MS/s / 25 M / 100,000,000 | 50, 980801–980850 | 2 / 4; zero |

Every phase used finite Stream, 32 channels, 1.6 V, Captures1, no hardware or
software trigger, no continuous mode and no Stream windows. D0 is the retained
1 MHz/50% clock: periods24–26 samples at25M and4–6 at5M satisfy the predefined
±1-sample bound. Both independent D22 UART decoders agree with zero warnings;
all130 numbered packets are fresh and consecutive within their own phase.
The host source sent41/42/52 distinct live commands, respectively, all ACKed;
commands sent outside the captured interval are not silently counted as samples.
Three fresh preflight ACKs were observed within500ms before each Run. The same
v8 source/PWM/camera1/SD0 and wiring were retained; MCU uptime across serial
connections is not asserted.

Independent RAW conversion matches all saved LOGIC bytes, including the actual
Linux Segment/feed and the native Windows archive. RAW sizes are400,556,032 /
400,556,032 /100,663,296 bytes. Passive exterior-kernel accounting preserves every
positive byte in original order:796 /780 /224 events. Windows has401,604,608
positive IN bytes, exactly RAW plus its retained1,048,576-byte worker-unconsumed
sidecar. Both Linux controls account for all RAW/quiet suffixes as well. USB
success, total length and CRC alone are not used as continuity proof; the temporal
witnesses above are required. Firmware remains unknown.

All three phases have original valid metadata, actual HEADER/END, quiet/cleanup
and native exit0, with the source still open at confirmed END and closed only
after postflight STATUS/ACK. Both Linux children directly observed LC_ALL=C.UTF-8;
no decimal-comma normalization or reconstructed live END proof was needed.
The unchanged Linux helperdb7c255b/core84c0c7da remain private diagnostic paths.
Windows metadata is natively complete with an empty capture error. Native
Save/Open/Save As preserves400,000,000 bytes and every archive member, including
all metadata, exactly. The first SR/PVS hashes remain intact. A persistent file
observer, independent of exec transport, observed the current replay HEADER,
400 MB and END/Stopped with no hardware ARM and exit0. This is file replay only.

All27 Windows originals were copied and SHA-verified before protected detach,
normal VM shutdown and the reserved Linux return. The same original GUI process
handle supplied exit0; its interactive task was removed. Read-only postflight
confirms Windows off and unchanged/free DLA4-2/dev16 at5000 Mb/s and source
CH3408134 at3-1.2/ttyUSB2. Linux is VMware; Windows is nested KVM. QEMU
NoNewPrivs1/Seccomp2 and guest-reset=false were verified before assignment and
detach. No simultaneous acquisition or additional agent was used.

### Timing observations, with clock and causal limits

Offline original-order worker/RAW/sidecar/kernel linkage covers all382 accepted
Windows reads and the sidecar read in both25M cases. The new native-output case
has3.979658 s inside the read wrapper and0.029847 s between calls over4.009505 s
(0.7444% outside). The retained PowerShell-output negative has3.059941 s inside
and5.465104 s between over8.525045 s (64.1065% outside). These are elapsed,
clock-local intervals, not CPU use or an isolated measurement of Qt logging,
a queue wait, mutex, SDK or scheduler cost.

An independent USBmon interval sweep matches the original accounting. New
Windows25M spans4.009037 s from ARM completion to last RAW completion, with
0.689946 s lacking pending IN, maximum one pending IN and longest gap14.117 ms.
The earlier negative spans8.524876 s with6.145891 s lacking pending IN. Current
Linux controls keep pending IN throughout4.008803 /5.036490 s, maximum eight.
Guest and exterior-kernel clocks are not synchronized; their local durations
are not absolute timeline alignment or causal partitions. A pause threshold
still cannot classify continuity: an earlier qualified5M case had a longer
single pause than the earlier negative25M case.

The new positive and the lower between-call time are consistent with the
previously isolated expensive PowerShell output path. They do not establish
that path as the sole cause of the earlier repeated131,564,544-byte interval,
missing packets, old0B failures or other transport limits. Cohorts are sequential,
not randomized; internal buffering/state and scheduling are uncontrolled. The
earlier negative, source commands and all partial/refused probes remain intact.
No firmware/memory-capacity diagnosis or product workaround follows from this
one positive. The physical cohort is closed; further acquisition needs a new
bounded plan, rather than reopening it for a green result.

Evidence is retained privately under `artifacts/native-output-physical-20261104/`:
fixed plan and product baseline, per-phase original metadata/END/source/USBmon,
independent data/kernel/decoder reviews, native launch/config/runtime and file
replay proofs, original copy manifests, shutdown/return gate, dispatch and interval
reviews, final review and publication verification. Product HEAD/status/diffs,
all1,248 product files and installed Linux/Windows artifacts remain unchanged;
all408 GUI source files still match the published product source. This update
publishes documentation only. The private launcher, captures, commercial SDK,
firmware, VM disks and credentials are excluded; no release/rollout is qualified.

## Update: native-output launcher in the actual Windows GUI, Demo only — 4 November 2026

A separate preparation cohort used the unchanged full GUI705a with native file
output in actual desktop Session1/PID5364. **Three Demo acquisitions and zero
physical acquisitions** exercised Stop, controller cancellation, finite END,
native Save/Open/Save As and normal application exit. This qualifies the stated
software observations only. Windows25MS/s continuity and AUD002/AUD006 remain
open; the earlier negative capture and all closed physical cohorts are preserved.

### Startup, retained ownership and refused controls

The private launcher separates Start from Wait. It publishes the PID while the
child is alive and retains the original CreateProcess process HANDLE, rather
than reopening a PID later. Wait timeouts preserve that handle; live close and
exit-code reads are refused. Only three explicitly listed standard handles are
inherited, with exclusive new stdout/stderr files. The interactive task has no
automatic execution-time termination. Its owner was observed alive after Demo
END, with no exit record yet; the GUI subsequently exited0 through that same
native handle, after file replay and normal window close.

The initial property-based exit accessor failed its control: Windows PowerShell
silently yielded null instead of catching its throwing getter. A device-free
control independently reproduced property-catch=false/null=true and explicit-
method-catch=true. The private accessor became an explicit GetExitCode method;
no product code changed. Original source, refusal and partial results remain.

The revised initial script reached a later refusal on the PowerShell process's
global handle count. Its preceding guards therefore completed: fast child exit7,
a short Wait returning false, refused live close/exit read, later exit0 of that
same child, and matching PID/quoted/backslash/Unicode arguments and native
standard-handle types. This is an explicitly reconstructed program-order proof,
not a claimed full pass of that script. Its exact original counter values were
not saved, and the cause of the global increase remains unisolated.

A separate native C# block, after two warmups, performed20 failed starts with
expected file-not-found error: handle count532 before and after. All86 output
files from failed starts reopened exclusively. An existing-output sentinel was
preserved. The remaining unchanged600-message logger child exited0 in22.733ms;
its startup line and all600 timed message texts are retained. There were five
successful selftest child starts across the original and revised phases; no
Demo acquisition was repeated to replace a refusal. A first cleanup-probe
compilation also refused an empty dynamic-assembly location before any child;
the combined-source probe and that failed preparation are both retained.

### Three current Demo runs and file preservation

The same GUI/Session/device used32 channels at200kS/s. Each current observer was
READY before Run and observed exactly one HEADER/END. The host controllers ran
outside exec transport lifetime.

| Demo case | Delivered and natively saved bytes | Observed behavior |
| --- | --- | --- |
| Explicit guarded Stop | 1,162,544 | Stop allowed after current data and matching fresh Stop image; GUI remains alive |
| SIGTERM to the host controller | 622,172 | Signal relayed after data; controller remains through guarded Stop and END |
| Finite100k samples | 400,000 | Natural END; late Stop refused without a click |

The cancellation's first image did not match the approved Stop glyph and was
refused. A later fresh matching image permitted Stop within the same run. No
unverified click or additional acquisition replaced it. The two permitted Stop
checks had FLOW ages50.573/59.887ms and screenshot RTT14.225/11.976ms. These are
UI observation/control bounds, not atomic operations or electrical timestamps.

All three native archives pass ZIP CRC, ordered chunks, expected lengths,
32-probe/4-byte units and200kHz metadata. Demo generated no capture-status/error
fields; their absence is preserved, not relabelled as a hardware success.
Native Open/Save As of the first stopped archive preserves all1,162,544 payload
bytes and every metadata member exactly. Its original archive and setup match
the hashes taken before recapture; no implicit Session1 or orphan setup file was
created. Total unique saved Demo data is2,184,716bytes. The replay's persistent
observer also completed normally with a current HEADER/END.

### Scope and final state

GUI705a, corebf0a6c78, C++a1fc7442 and decoder628767ce module hashes are unchanged.
The revised private launcher source is SHA256
`17d809c772e41db85776864c8fb341a1e741ec602859697594dda86909e892d4`.
Linux runs in VMware, Windows in nested KVM. This is a Windows-specific private
process-launch change; Linux product code/installation is unchanged, with no
new Linux desktop or electrical test claimed. No DLA was assigned to this boot,
no source port was opened, and no reset, hub cycle, firmware or wiring change
occurred. Demo software checks do not qualify real acquisition or data continuity.

GUI native exit0 and task removal were verified. **All135 Windows originals
were copied and SHA256-checked before** normal VM shutdown. The shutdown command
returned0, then the QMP polling connection reset while the VM exited; that
monitor did not finish successfully. A separate read-only postcheck verified
state=null/kvm=null, DLA4-2/address16 at5000Mb/s and free DLA/source ports. No
second shutdown or reset was used to replace that exception.

All1248 product files/HEAD/status/diffs and installed programs remain unchanged.
Only this report is published. Private launcher sources/binaries, captures,
vendor firmware, VM disks and credentials are excluded. The original full
selftest refusals remain explicit; no product rollout or complete suite success
is asserted. Real DLA qualification requires a new bounded physical plan with
fresh witnesses and independent RAW/kernel checks, retaining the old negatives.

## Update: inter-read delays and device-free logging controls — 4 November 2026

The Windows25MS/s negative below remains **failed**, and its physical cohort
remains closed. New read-only analysis and device-free software controls identify
a substantial cost in the laboratory's PowerShell output redirection. There is
no new DLA acquisition, product fix, GUI installation or firmware-cause claim.

### Worker and kernel intervals from unchanged originals

Every positive worker read was aligned in original order by read ID, RAW offset,
size and32-byte prefix with retained RAW/sidecar and exterior-kernel requests.
Earlier independent whole-payload conservation remains required; matching a
prefix alone is not that proof. The linked counts, including the unconsumed1MiB
suffix, are383/192/192 for the three cases below. No sample bytes were altered,
reordered or discarded. Local timing identities were recomputed independently.

| Retained case | Worker read-call total | Time between worker read calls | Worker span, first start to last end |
| --- | --- | --- | --- |
| GUI25MS/s,400MB, failed | 3.059941s /382 accepted reads | 5.465104s /381 intervals | 8.525045s |
| Same GUI705a finite5MS/s,200MB, passed | 9.743593s /191 reads | 0.274539s /190 intervals | 10.018132s |
| Native API5MS/s,200MB, passed | 10.009874s /191 reads | 0.007834s /190 intervals | 10.017708s |

In the negative,64.106% of that worker span is **outside** the read wrapper.
Its median between-call interval is12.816ms, versus40us in the positive GUI case
and the much smaller native-API total above. The wrapper brackets timeout setup,
ReadEndP and error handling. Outside it, the worker still performs publication,
mutex/event operations, possible queue waits and OS/VM scheduling. These are wall
time observations, not CPU attribution or a measurement of one wait primitive.

Exterior-kernel accounting for the negative has6.145891s without IN pending in
8.524876s from ARM completion to last RAW completion, maximum one pending IN;
Linux25MS/s maintained pending IN throughout4.008772s, maximum eight. These are
separate clocks and extents: the6.145891s must not be directly equated to the
5.465104s between worker calls. Clock-local sums are exact; signed worker-minus-
kernel duration observations are retained without inventing synchronized clocks.
The GUI5MS/s positive even has a larger maximum no-IN interval188.307ms than the
negative152.591ms. A single longest pause is not a continuity classifier.
Sequential cases differ in frontend/rate/extent; no failure probability or
universal throughput limit follows from this comparison.

### Reproduced redirection cost and a private correction

A native C fixture calls the **exported default libsigrok log callback**600 times
with three representative message shapes. It does not call sr_init, scan, device,
session, WCH or acquisition APIs, and does not include Qt's additional Logging
path. The same source ran on Linux and Windows. Windows used unchanged core
`bf0a6c78`, Linux unchanged `84c0c7da`; DLL/source/executable hashes were recorded.
Windows ran under SSH/Session0, not the interactive GUI's Session1.

The initial six-process order on each OS was inherited stderr, direct file,
NUL, NUL, direct file, inherited stderr. On Windows the inherited case used the
same PowerShell Start-Process RedirectStandardError mechanism as the earlier GUI
launcher. The child could instead reopen stderr to a file or NUL. Timed600-call
Windows totals were2749.031/2749.778ms inherited,21.311/21.215ms direct file and
16.198/16.592ms NUL. Linux totals were11.445/14.885ms inherited direct-file
handles,3.357/3.387ms reopened file and0.501/0.669ms NUL. The inherited mechanisms
differ between platforms; these figures are not an isolated OS comparison.

A second, predefined Windows intervention kept the fixture binary, core,
message arguments and inherited-stderr mode unchanged (per-run result/output
paths were unique), and changed only the launcher/output mechanism. A private CreateProcessW launcher supplied native
file handles with an explicit three-standard-handle inheritance list, exclusive
new output files and the original process HANDLE for exit verification. The
balanced order was native, PowerShell, PowerShell, native:

| Native file handles | PowerShell redirection | PowerShell redirection | Native file handles |
| --- | --- | --- | --- |
| 24.779ms | 2729.811ms | 2713.330ms | 26.604ms |

Both native totals are over101.98 times smaller than either PowerShell control.
All600 timed message texts after the timestamp prefix are preserved exactly in
order in every non-NUL case, including their arguments, with no callback error. The additional untimed
loglevel-startup line is accounted separately. All16 timed fixture processes exit0;
a separately planned fast child exit2 verifies that the native launcher does not
turn an early nonzero exit into success. There are17 software child processes
and **zero physical acquisitions**. Initial offline reviewer refusals assumed
GUI generation1 and overlooked that startup line; the corrected reviewers keep
the actual generation2 and original logs, without rerunning the refused data.

This intervention demonstrates and avoids the redirection cost **in this
fixture**. It does not measure the complete Qt GUI path, apportion the earlier
5.465104s, prove that logging alone caused temporal replay, or qualify a new GUI
capture. The private launcher is qualified only for these fixture paths/arguments;
it is not installed or promoted into the closed capture cohort. Product logging,
queues, timeouts, sample-rate settings, source and firmware are unchanged.

All44 Windows evidence files were copied and SHA256-checked before normal VM
shutdown. No DLA was assigned or source port opened in this boot. Windows is off;
DLA/source ports are free. The1248 product files/HEAD/status/diffs and installed
programs are unchanged. AUD002/AUD006 remain open. Any subsequent GUI/hardware
qualification needs a new bounded plan with fresh witnesses, native Stop/END and
independent RAW/kernel checks; the negative below must remain preserved. Private
captures, launcher binaries, vendor firmware, VM disks and credentials are not
included in this documentation publication.

## Update: 25 MS/s Windows continuity failure between passing Linux controls — 4 November 2026

A new bounded cohort is closed at **three physical acquisitions, zero retries**.
A Linux25MS/s control passed, the actual Windows desktop GUI at the same requested
geometry failed temporal witnesses, and the predefined Linux5MS/s return passed.
The planned Windows50MS/s phase was cancelled at the first failure. This is a
negative qualification result, with no product fix, workaround or installation.

| Phase | Requested samples/rate | Saved LOGIC bytes | Electrical witness result |
| --- | --- | --- | --- |
| Linux control | 100M /25MS/s | 400,000,000 | Pass:40 fresh consecutive CRC messages, D0 no abnormal periods |
| Windows GUI | 100M /25MS/s | 400,000,000 | **Fail:** missing/repeated CRC messages and6 abnormal D0 periods |
| Windows faster phase | 100M /50MS/s | Not acquired | Cancelled; no retry |
| Linux return | 25M /5MS/s | 100,000,000 | Pass:50 fresh consecutive CRC messages, D0 no abnormal periods |

### Configuration, identity and ownership

All acquired phases used finite Stream,32 enabled channels,1.6V, no hardware or
software trigger, no Stream windows and one capture per Run. Linux is in VMware;
Windows is in nested KVM. The same DLA `1a86:5537` stayed at USB4-2/address16,
negotiated5000Mb/s; its firmware version is unknown. The unchanged CH340 source
`8134` at3-1.2/ttyUSB2 reported v8/PWM1MHz/50%/camera1/SD0. There was no reset,
hub cycle, flash or wiring change. Source connections were separate; continuous
MCU uptime is not claimed.

Windows actual desktop Session1/PID1400 loaded GUI SHA256
`705a2fc10aaec5859f3f398ad3468f3ca51cc3774c31fcf960c8777ec1b72b6c`,
whose408-file source snapshot is published in `62a00c79`. Core/C++/decoder/WCH
module hashes match the preceding qualified5MS/s cohort. QEMU NoNewPrivs1,
Seccomp2 and guest-reset=false were checked before assignment. Native options,
finite100M/25MHz selectors and trigger-off state were inspected; the saved setup
independently confirms32 enabled channels, trigger/windows off and captures1.
All four full2048-byte commands match the Linux25MS/s control exactly; Windows
command framing and CRC were checked separately. These sequential controls use
different frontends/transports and do not isolate an OS, scheduling or firmware
cause, or estimate a failure rate.

Passive USB monitoring and the current GUI observer were ready before Run.
Three fresh source ACKs preceded each Run by at most500ms. Every capture retained
the source through actual backend END/quiet/trace closure and cleanup, then
postflight STATUS/ACK and source close; each live end guard passed directly.
The GUI remained alive after the Windows END. A missing private Python dependency
stopped an initial controller import before any source open or native Run; that
failure and the unchanged dependency correction are retained. It was not a
physical acquisition or a retry of one.

### The Windows negative is in the retained input

Native metadata reports `complete`, no error, and exactly400,000,000 LOGIC bytes.
An independent implementation converts the original400,556,032-byte RAW prefix
to the saved LOGIC exactly. Kernel accounting preserves original request and
completion order:780events,401,604,608 positive IN bytes =400,556,032 RAW
+1,048,576 unconsumed worker-sidecar bytes. No bytes were reordered, silently
discarded or repaired.

Both UART decoders agree, without warnings, on40 CRC-valid packet occurrences.
Counters930514..930526 are absent;930528..930540 appear twice. The host sent each
fresh counter once and all42 live commands were acknowledged. D0 has6 periods
outside25+/-1samples. Their starting samples are30947310,30947328,31088640,
31209452,31209472 and67108851, with periods18,16,16,20,14 and16samples.
The initial/final rising-edge boundaries are10/20samples.

More specifically, original RAW ranges `[134217728,265782272)` and
`[268435456,400000000)` are **byte-for-byte identical over131,564,544bytes**,
at a distance of128MiB. Thirteen complete repeated numbered CRC packets have
identical positions relative to those ranges. This supports temporal replay in
the retained input, beyond similarity of a periodic clock alone. The independently
matched kernel stream already contains these bytes, before guest conversion,
GUI storage and Save. It does not establish device memory capacity or identify
the responsible firmware, USB hardware, host stack or virtualization component.
A correct byte count, valid CRCs and clean END are insufficient continuity proof.

### Linux controls, replay and final state

Linux used the unchanged native C API/actual Segment helper `db7c255b` with
installed core `84c0c7da`. At25MS/s, RAW400,556,032B, feed and actual Segment
preserve400,000,000 output bytes exactly;796USBmon events account for all bytes
in original order. Both decoders agree on930101..930140, no warnings; D0 periods
24..26, boundaries5/20samples. The5MS/s return preserves100,000,000B from
RAW100,663,296B,224events; counters931101..931150, D0 boundaries4/1samples,
no abnormal periods. The original JSON is valid in both runs: child environments
explicitly set `LC_ALL=C.UTF-8` despite inherited Romanian `LC_NUMERIC`.
Native exits, one HEADER/END, cleanup and source-close ordering pass directly;
no decimal normalization or ownership reconstruction was needed in this cohort.

Native Windows Save/Open/Save As preserved all400,000,000 bytes and every archive
metadata member, including the original `complete` status and its continuity
failure. Original archive/setup files are unchanged. The auxiliary host/guest
replay observer was interrupted by exec transport after4MiB (host exit143).
That partial observer and a sharing-mode read refusal are retained, not reported
as a passing live replay gate. Its original prefix matches the full GUI log at
the exact235806-byte READY offset. The full original log subsequently proves
one file HEADER/END/Stopped and400MB, with no hardware ARM; complete native
Save As equality independently confirms payload/metadata preservation. No Open
or acquisition was repeated to replace the failed observer record.

GUI native exit0 and the exit-code7 self-test passed. **All26 Windows originals
were copied and SHA256-verified before** guarded detach and normal VM shutdown.
The reserved Linux return began only after those checks and fresh free-port,
identity and speed checks. Windows is now off, DLA/source ports are free, all1248
product source files/HEAD/status/diffs and installed programs remain unchanged.

The report qualifies these bounded Linux controls and preserves the Windows
negative. Electrical witnesses cover D0/D22 only. It does not qualify arbitrary
32-channel continuity, the cancelled50MS/s case, or Windows25MS/s operation.
AUD002/AUD006 and older zero-byte/gap/replay observations remain open. Private
captures, vendor firmware, VM disks and credentials are excluded from publication.

## Update: free continuous capture, native Stop and finite restoration — 4 November 2026

A new, closed cohort used **three physical acquisitions and zero retries**:
Windows free continuous Stream with an explicit native Stop, a finite capture in
the same GUI/Session/device, then Linux return. All three stored-data comparisons
pass: **403,809,024 LOGIC bytes and 201 fresh consecutive CRC messages**. The Linux
live metadata gate refused a diagnostic formatting error; its separate offline
review and source-ownership reconstruction are described below, rather than
reported as an initially passing live gate.

The tested Windows full GUI is SHA256
`705a2fc10aaec5859f3f398ad3468f3ca51cc3774c31fcf960c8777ec1b72b6c`,
from the 408-file source snapshot published in `62a00c79`. Loaded core/C++/SDK/WCH
hashes were verified. The installed programs and all product source files remain
unchanged in this cohort. Linux runs in VMware; Windows runs in nested KVM.
DLA `1a86:5537` stayed at USB path4-2/address16, negotiated5000Mb/s. Its firmware
version is unknown. Source CH340 `8134` at3-1.2/ttyUSB2 reported v8, PWM1MHz/50%,
camera1 and SD0. There was no reset, hub cycle, flash or wiring change.

### Windows acquisition and evidence

Actual desktop Session1/PID6216 used 32 enabled channels, 1.6V and5MS/s. Native
Continuous sampling was enabled in Stream; hardware/software triggers and Stream
windows were disabled. Captures-per-Run was1. Stop was permitted only after
100,000,000 current LOGIC bytes, with a current FLOW receipt no older than150ms
and a new image matching the approved Stop glyph. At the decision it had
101,711,872 bytes, FLOW age5.156913ms and screenshot RTT16.997375ms. This is a GUI
observation/control bound, not an atomic UI operation or an electrical timestamp.
One native Stop produced one current ARM/HEADER/END, status `stopped`, no error,
and **103,809,024 saved bytes**, equal to the entire independently converted RAW.
D0 had no abnormal periods, with initial/final boundaries2/4samples. Two UART
decoders agreed on51 fresh messages928101..928151, without warnings.

The actual Continuous checkbox was then disabled. Its hidden finite selector
returned and was set to50M samples at5MS/s. Without capture Open/reload/device
change between the acquisitions, the second capture saved **200,000,000 bytes**,
status `complete`, no error. RAW conversion matched exactly. D0 boundaries were
4/0samples with no abnormal periods; both UART decoders agreed on100 fresh
messages928501..928600. All four full2048-byte finite commands match the earlier
finite50M Windows API reference; their framing and CRC were checked separately.
The continuous setup is different and is not claimed to match that reference.

Kernel conservation and original completion order pass. Continuous:
214events/104,857,600 positive IN bytes =103,809,024 RAW +1,048,576 unconsumed
worker sidecar bytes. Finite:398events/201,326,592 positive bytes =200,278,016 RAW
+1,048,576 sidecar. All eight full commands were independently CRC-checked.

The persistent observer was READY before the source opened; each Run had three
fresh acknowledged witness commands. The source stayed open through actual
retirement/trace closure/END/quiet/Stopped, then status/ACK and close. The74 and102
live commands were all acknowledged; commands after the sampled interval are
not claimed to be captured. Source connections were separate, without an MCU
uptime claim.

Native Save after recapture asked for the new target, exercising the published
Save-context correction on hardware. After both physical acquisitions, native
Open and Save As of each archive preserved every payload byte and all capture
metadata, including `stopped` versus `complete`. ZIP CRC, ordered chunks and
member hashes passed on Windows and independently on Linux. Original archives
and their setup files remained unchanged. An unbound shortcut was mistakenly
used during replay; its UI effects and failed attempt were retained. Qualified
resaves used the actual Save As menu and full comparisons. No acquisition was
repeated. An early launcher screenshot and unsuccessful checkbox click were
also retained; only subsequently verified native states qualified the Run.

GUI native exit0, a child exit-code self-test of7 and removal of the temporary
tasks were verified. **All58 Windows originals were copied and SHA256-checked
before** guarded detach and normal VM shutdown. QEMU retained NoNewPrivs1,
Seccomp2 and guest-reset=false.

### Linux return and diagnostic exception

The unchanged `db7c255b` native C API/actual Segment helper, with installed core
`84c0c7da`, captured25M samples at5MS/s. This is transport/storage return evidence,
not a Linux desktop Save test. RAW100,663,296B, feed and actual Segment preserve
100,000,000 output bytes exactly under independent conversion. USBmon224events
account for all positive bytes in original order. D0 has no abnormal periods,
boundaries4/1samples; two UART decoders agree on50 fresh messages928801..928850,
without warnings. The helper exited0 with one HEADER/END, accepted Stop, drain,
trace closure and session/device/context cleanup.

The service inherited `LC_NUMERIC=ro_RO.UTF-8` without an overriding LC_ALL.
The helper's locale-sensitive formatter wrote four timing values with decimal
commas, which strict JSON parsing rejected after the native process had exited.
A device-free QCoreApplication/glib formatting control reproduces the invalid
number in that environment and valid JSON with `LC_ALL=C.UTF-8`. The private
launcher now sets that environment for future cohorts; this capture was not
repeated and the native helper/product binaries were not changed.

The original malformed result, exception and controller flags are retained.
A separately labelled derived metadata view changes exactly four decimal
separators, preserving the numeric values and every other byte; RAW/feed/Segment
and trace inputs remain unchanged links to the originals. Backend completion is
verified offline from those same originals. Retained controller/protocol order
proves the serial context remained open when STOP_READY was observed after
session/device/context closure, and unwound only at the later JSON exception.
The exact port-close timestamp was not recorded. The initial live END parser
and its postflight STATUS did not complete; they are not reported as passing.
A later, separate read-only STATUS connection verified the unchanged source
configuration and closed normally. This reconstruction is distinct from the
direct live END/source-close measurements of the two Windows phases.

The cohort is closed. Windows is off and DLA/source ports are free. These results
qualify the stated5MS/s continuous-Stop-to-finite case and bounded D0/D22 witness
data. They do not establish arbitrary32-channel electrical continuity, high-rate
behavior, a firmware cause or repair for older zero-byte/gap/replay observations,
or a complete GUI rollout. AUD002/AUD006 remain open for those scopes. No new
product workaround or GUI installation is promoted. Private captures, diagnostic
executables, vendor firmware, VM files and credentials are not published.

## Update: internally stopped Stream window and finite restoration

A new fixed cohort is closed after three physical acquisitions, zero retries.
It starts from the separately verified Linux return in the preceding cohort;
that closed cohort is not rerun. The starting evidence hash, current DLA/source
identities, exclusive owners and negotiated 5000 Mb/s are checked before the new
guarded Windows assignment. Firmware, source v8/PWM1MHz50%, camera1/SD0, wiring
and hub are unchanged. Linux is in VMware; Windows is a nested KVM guest.

This tests one specific route in the actual full GUI206d, interactive desktop
Session1, PID420. Stream windows require a finite **UI** depth: the product
then temporarily enables continuous input in the backend and restores its
limits on exit. The UI Continuous checkbox stays off. This distinction was
clarified before any physical capture; the original plan is retained.
It is not a manual free-running-continuous or explicit-Stop qualification.

| Phase, all at 5 MS/s | Saved data | Retained RAW | Fresh consecutive CRC messages | Kernel records |
| --- | ---: | ---: | --- | ---: |
| Windows, one internally stopped window | 20,000,000 B | 20,971,520 B | 926101–926110 (10) | 56 |
| Windows, subsequent finite 50M samples | 200,000,000 B | 200,278,016 B | 926501–926600 (100) | 398 |
| Installed Linux return, 25M samples | 100,000,000 B | 100,663,296 B | 926801–926850 (50) | 224 |

The first phase selects one 5-million-sample window, D0 rising, Pre1% and
Captures1. It stops internally when that window is complete, without a timed
Run/Stop toggle. Its native collection contains one complete frame with no
error, source origin1 and trigger50000 relative samples (0.01s), exactly 1%
real pre-trigger history. At that trigger, D0 changes from low to high. All
20 MB match the independently converted RAW slice starting at source sample1.
Both UART decoders agree with zero warnings on all10 fresh CRC messages.
D0 has no period outside4..6 samples; first/last-rise boundaries are2/3 samples.

The second phase keeps the same GUI process, Session and DLA. No capture-file
Open, reload, device switch or VM restart intervenes between acquisitions.
Native Restore Session Setup disables only Stream windows in the previously
saved setup; the normal sample selector then chooses50M at5MS/s. The software
condition is retained but inactive without Stream windows. This qualifies that
native setup-restoration route, not a pointer toggle of an overflow-hidden
checkbox. Driver Continuous reads off, and all four complete2048B
Stop/SETUP/ARM/Stop commands match the previously verified finite50M API case,
including SHA and CRC. The stored200MB, independent RAW conversion, all100 fresh
CRC messages and D0 pass; D0 boundaries are3/3 samples. Complete/error-empty
metadata is preserved. The earlier window export is not overwritten.

After both physical Windows acquisitions, native Open and Save As replay both
exports. Every20MB window byte and200MB finite byte, capture metadata, frame
order/status/error, source origin and trigger are exact. ZIP CRCs and collection
member lengths/SHA are checked. The original native exports retain their
original SHA; different ZIP timestamps need not produce identical containers.

Passive kernel evidence accounts for every positive byte in original
completion/submission order, without loss, truncation, sorting or repair.
The window has20,971,520B RAW plus a separate1,048,576B worker suffix, exactly
22,020,096B kernel data. The finite phase has200,278,016B RAW plus1,048,576B
suffix, exactly201,326,592B kernel data. All eight complete Windows commands
have independently verified framing/CRC. Raw surplus, source-origin prefix and
worker suffixes remain separate from the intended saved frames; none is erased.
The Linux return has100,663,296B RAW/kernel, exact100MB feed/actualSegment,
50 fresh CRC messages and quiet Stop/END/cleanup. Electrical witnesses remain
limited to D0/D22; stored32-value conversion is not an arbitrary all-channel
continuity guarantee.

Persistent current-run observers are READY before three fresh source ACKs and
native Run. The foreground source stays owned through accepted Stop, quiet
drain, worker retirement, trace closure, END and Stopped; post-status/ordered
ACK checks precede serial close. Window/finite live commands are15/102; later
ACKs beyond the saved interval are not claimed as captured messages. The GUI
remains alive through native saves and replay, then exits normally with native
code0 through the retained-handle reader. Its child-exit7 control also passes.
All34 guest originals are copied and SHA-verified before normal VM shutdown;
temporary tasks are removed. The Linux return and final DL32 identity scan
pass. Windows is off; DLA at4-2/address4.16 and source CH3408134 at3-1.2 are free.

A packaging check initially refused the already confirmed END because this
PowerShell runtime interprets String.Split's separator as characters, not one
literal substring. The error, script and empty output directory are retained;
no RAW had moved. The corrected check counts escaped literal regex matches,
and filenames use explicit suffix construction. Originals are then copied and
verified. This is a packaging-tool correction after capture, not a product
change, altered acceptance criterion or repeated physical acquisition.

The actual GUI/runtime binaries and all1,248 local product source hashes,
HEAD/status/diffs, installed GUI/launcher/libraries retain their baseline.
QEMU's existing per-child reset guard and guest-reset=false remain active; no
reset, power cycle, flash or wiring change is used. This update contains only
the reviewed evidence report. Private captures/tools, commercial firmware,
VM and credentials are not published. This positive5MS/s route does not repair
or explain earlier0B cases, manual continuous/Stop failures or high-rate gaps/
replay. AUD002/AUD006 remain open; GUI206d is not installed or promoted.

## Update: matched Windows API and interactive GUI geometry

A separate fixed cohort is closed with four physical acquisitions and zero
retries. It keeps the source v8/PWM1MHz50%, wiring, firmware and hub unchanged.
Linux runs in VMware; Windows is a nested KVM guest. The DLA is
`1a86:5537`, negotiated 5000 Mb/s at Linux path `4-2`/address `4.16`.
The source CH3408134 remains at `3-1.2`; DTR/RTS are false. Its serial port is
opened separately for each phase, so continuous MCU uptime is not claimed.

The earlier 25-million-sample API control did not match the GUI's 50-million
sample request. This cohort requests finite Stream, 5 MS/s, 50 million samples,
32 channels, 1.6 V and no trigger in **both Windows paths**. Linux recovery and
return controls each request 25 million samples at 5 MS/s. Equal requested
geometry does not match hidden device state or establish GUI/API causality.

| Phase | Actual LOGIC/storage | Retained RAW | Fresh consecutive CRC messages | Kernel records |
| --- | ---: | ---: | --- | ---: |
| Linux recovery | 100,000,000 B | 100,663,296 B | 924201–924250 (50) | 224 |
| Windows native API/Segment | 200,000,000 B | 200,278,016 B | 924501–924600 (100) | 406 |
| Windows interactive GUI | 200,000,000 B | 200,278,016 B | 924801–924900 (100) | 398 |
| Linux return | 100,000,000 B | 100,663,296 B | 925201–925250 (50) | 224 |

Every phase passes independent RAW-to-LOGIC conversion. Both API paths also
match their delivered feed and actual Segment bytes. The actual GUI's native
Save, Open and Save As round trip preserves all 200 MB, every ordered ZIP LOGIC
chunk and the complete metadata/error state exactly. The archives report
`complete`, no capture error, 32 stored channels and 5 MHz. ZIP CRCs and all
payload lengths are checked; differing container timestamps are not data loss.
The original export's SHA is unchanged after replay/reexport.

Two independent UART decoders agree with zero warnings in every phase. The
captured CRC ranges above are fresh, known and consecutive, not merely serial
ACKs. D0 has no period outside 4..6 samples; initial/final rise boundaries are
2/1, 4/3, 2/4 and 4/3 samples respectively. These electrical witnesses cover
D0/D22 only, not arbitrary signals on every channel. The 32-value conversion
check concerns stored data, not a general electrical continuity guarantee.

Passive USB evidence is loss-free and accounts for every positive byte in
original submission/completion order. Each Windows phase retains 200,278,016 B
of decoder-input RAW **plus a separate 1,048,576 B worker-unconsumed suffix**,
exactly matching all 201,326,592 positive kernel bytes. Neither suffix nor
quantized RAW surplus is discarded or fed to the saved 200 MB recording.
Linux's complete positive kernel payload is 100,663,296 B per phase. All four
complete 2048 B Stop/SETUP/ARM/Stop commands have verified framing and CRC;
the Windows API and GUI command hashes match exactly. The Linux count differs
by design, so its SETUP is not required to match the Windows 50M request.

A supplementary review initially required zero worker suffix, inherited from
the earlier 100 MB control. The existing independent WCH accounting already
qualified the 1 MiB suffix in original order. That additional assumption was
rejected and the review corrected to require complete ordered conservation.
The rejected review, its source and every original byte are retained; no
physical acquisition was repeated. This is a review-tool correction, not a
product fix or a change to the cohort's acceptance contract.

Before hardware, the current-run GUI END guard passes 27 offline controls,
including retained positive, zero-data-timeout and long-timeout logs plus
missing/duplicate/stale markers and mismatched lengths. It confirms termination,
not capture success. An actual interactive Demo run then verifies that a
persistent SSH observer can read the live GUI log, signal READY before Run and
observe the current HEADER/END/Stopped while the GUI stays alive. Demo has no
physical DLA or serial source. Its native exit is independently verified as 0;
the retained-handle exit reader also passes a child-exit-7 control.

For each physical phase, three fresh source ACKs precede Run. The actual GUI
Run is 121.420384 ms after the latest acknowledged preflight command. The
persistent observer is already READY; no cold SSH query or timed Run/Stop
toggle intervenes. Foreground source ownership lasts through current-run
HEADER/ARM, accepted Stop, quiet drain, worker retirement, trace closure,
END and Stopped. Source status/ACK checks precede serial close. The API helper
also exits natively before source close; the GUI stays alive for Save and is
closed normally only after native replay/reexport. The actual GUI exit is 0,
verified through its retained native handle. No fabricated GUI-exit marker is
used to confirm acquisition END.

Source control emits 51/102/102/52 live commands, plus three preflight commands
per phase; all have ordered ACKs. Later ACKs outside the captured interval are
not counted as captured data. Linux API first LOGIC arrives 57.940/58.144 ms
after ARM returns; Windows API is 58.013 ms. These are callback observations,
not timestamps of the first electrical sample.

The exact unmodified binaries are Linux helper `db7c255b`, Windows helper
`d0ac9c4c`, Linux core `84c0c7da`, Windows core `bf0a6c78`, and actual full GUI
`206d8584` in Victor's interactive desktop Session 1. Runtime DLL paths/hashes
are checked before Run. This is broader evidence than a Session-0 API helper,
but it does not qualify the continuous-to-finite transition, software-window
controls with fresh physical counters, high rates or earlier intermittent
zero-data captures. **AUD002/AUD006 remain open; no product correction or GUI
installation is promoted.** The earlier failures remain unchanged.

All 13 API, 14 GUI and 8 Demo guest originals are copied and SHA-verified before
normal VM shutdown. Temporary tasks are removed; GUI/launcher/runtime hashes
are unchanged. QEMU retains its existing per-child reset guard and
`guest-reset=false`; no reset, power cycle, flash or rewiring is used. A real
Linux return and final DL32 identity scan pass. Windows is off; DLA and source
have no owners. All 1,248 local product source hashes, repository HEAD/status/
diffs and installations retain their baseline. This update changes only this
evidence report; captures, test tools, vendor firmware, VM and credentials stay
private.

## Update: native API controls with source closure after confirmed END

A separate plan was capped at three physical acquisitions: an installed Linux
baseline, native Windows API capture, and installed Linux return. Each required
independent qualification of the preceding acquisition. No physical acquisition
was repeated. Both hosts used the already qualified, unchanged C API/actual
Segment helper; this path does not run Session, MainWindow or View.

The laboratory completion check requires the native process to finish, exactly
one HEADER/END and closed-session marker, RAW trace closure and quiet cleanup.
Elapsed time, ARM or a successful SSH launch alone cannot satisfy it. Thirty
offline controls using both hosts' retained logs reject missing, stale, duplicated
or conflicting evidence, while distinguishing a cleanly ended failure from a
successful acquisition. A first predicate mistake admitting exit 1 with complete
metadata was caught and retained before physical testing. The corrected checker
was also exercised by the real Windows bridge.

The guest bridge stays connected and reports READY before source preflight.
After three fresh acknowledged commands, GO travels through that same pipe.
The source remains open through native process exit and confirmed END; it is
then checked and closed. No timed Run/Stop toggle controls these captures.

All three finite Stream captures used 32 channels, 1.6 V, no trigger,
5 MS/s and a 25-million-sample target:

| Physical acquisition | Stored bytes / captured fresh CRC packets | First LOGIC after returned ARM | Passive USB records |
| --- | --- | --- | --- |
| Linux baseline | 100,000,000 / 50 | 58.224 ms | 224 |
| Windows C API | 100,000,000 / 50 | 59.084 ms | 216 |
| Linux return | 100,000,000 / 50 | 58.022 ms | 224 |

In each case, the preserved RAW prefix converts independently to the original
feed and actual Segment bytes exactly on all 32 stored channel values. D0
periods are four to six samples, with zero outliers including boundary checks.
Two UART decoders agree on all 50 consecutive CRC packets without warnings.
All 52 live serial commands and three preflight commands were acknowledged;
ACKs beyond the capture interval are control evidence, not additional samples.
Electrical witnesses remain limited to D0/D22.

Each original RAW contains 100,663,296 bytes. The suffix beyond the requested
100,000,000 LOGIC bytes is retained. Every RAW byte matches passive outer-Linux
USB completions in original completion/submission order, with no lost or
truncated monitor events and all positive sample bytes accounted. Windows has
96 positive 1 MiB completions; the worker-unconsumed sidecar is empty. All four
complete 2048-byte Stop/SETUP/ARM/Stop commands have valid framing/CRCs and are
identical across the three acquisitions. These observations are not a measured
physical bus utilization or a proof of arbitrary all-channel continuity.

One Windows loader preparation was rejected before `sr_init`: the bridge passed
an expected DLL path with backslashes while the unchanged helper compares a
slash-normalized path. Exit 2 produced no RAW, configuration or ARM files.
An offline DLL mapping check reproduces the mismatch; only the bridge's
environment string changed to canonical slashes. The rejected launch and its
source preflight are preserved. Only the previously unstarted acquisition phases
continued; the DLA assignment was kept without a reset. It is not a repeated
physical capture or an unrecorded successful preparation.

Windows ran through SSH Session0 in nested KVM; Linux runs in VMware, on the
same DLA `1a86:5537`, USB path `4-2`, address `4.16`, negotiated 5000 Mb/s.
Source v8, PWM 1 MHz / 50%, camera
and wiring were unchanged, SD disabled; DLA firmware version remains unknown.
Serial connections were separate per phase, so uninterrupted MCU uptime is not
claimed. All 18 Windows original files were copied and SHA256-verified before
normal VM shutdown. The guarded handover, Linux return, final DL32 identification
and released USB/serial owners passed. No hub cycle, reset, firmware write,
product-source change or GUI installation was performed.

This qualifies these low-rate API controls and the laboratory source-completion
rule. It does not repair or invalidate the earlier zero-byte starts or replay
failures. The earlier interactive GUI timeout requested 50 million samples;
this API control requests 25 million, so it is not a matched GUI/API causal test.
Interactive desktop, physical continuous-to-finite and broader high-rate Windows
qualification remain open. The GUI candidate remains uninstalled.

## Update: host-only automatic count Stop check

The old laboratory controller was reproduced unchanged with every hardware,
serial, QMP and SSH interface replaced. A simulated flow query delayed by
2.964 seconds makes it return success and close its source while the simulated
acquisition is still active. No physical acquisition was started in this check.
That controller remains retired; its closed physical cohort was not resumed.

A private entry point then exercised the existing MainWindow, Session and View
objects on Linux XCB and the Windows platform. The Windows test ran in SSH
Session 0, not Victor's interactive desktop. The application source snapshot
and the original GUI objects were retained; no product binary was installed.
Configuration uses Session/device APIs; toolbar editing and displayed selector
refresh are outside this check.

The standard Demo driver lacks CONTINUOUS GET/SET. A probe-only Device adapter
therefore temporarily changes its sample bound from 5 million to 100 million.
The existing frontend extracts one 5-million-sample window at 1 MS/s, with a D0
rising trigger and 1% pretrigger, and stops the source when that window finishes.
This adapter is restricted to Demo and is not product code or a DLA workaround.
The main Qt/observer thread was deliberately paused for seven seconds.

| Host platform | Internal window Stop after Run | Following finite capture |
| --- | --- | --- |
| Linux XCB | 5.047586 s, before the observer resumed | Exactly 5 million samples / 20,000,000 bytes |
| Windows, Session 0 | 5.039201 s, before the observer resumed | Exactly 5 million samples / 20,000,000 bytes |

Both returned to Run without an external Stop toggle. The sample limit and
trigger were restored before the following finite capture in the same session.
All four payloads match an independent Gray-to-binary inverse yielding counters
1 through 5,000,000 on the saved 32-bit values. The archives, member SHA256/CRCs,
chunk order, source origins and triggers passed, and saved replay matched.
The automatic window has explicit `complete` status. The ordinary finite Demo
source emits no outcome metadata: its empty in-memory status is serialized as
`unknown` and read back as such; it is not relabelled complete by this check.

Initial fixture failures are retained: unsupported native Demo controls,
rejected zero sample limit, and an incorrect expectation that empty Demo status
round-trips as an empty string. The final fixture checks the format's explicit
`unknown` representation. These are laboratory assumptions, not product fixes.

This verifies the frontend count Stop under a stalled observer. It does not
qualify DLA CONTINUOUS restoration, USB/electrical behavior, a physical
continuous-to-finite transition or the earlier unrestricted continuous capture.
There were zero physical captures, no DLA assignment and no serial source opens.
The separate failed timing attempt remains unqualified. Product sources and
installed launchers remain unchanged; the GUI candidate is still uninstalled.

## Update: finite first-data deadline experiment

A separate Linux-only diagnostic tested whether a finite Stream acquisition can
legitimately deliver its first data after the existing three-second deadline.
A review of seven retained starts found that the failing 5 MS/s finite requests
encoded five or ten seconds while timing out at about three seconds. However,
identical finite setups had also delivered data promptly, and a 20 MS/s failure
encoded only 1.25 seconds. These observations motivate a test, not a universal
mode, firmware or transport explanation.

Only a private copy of `api.c` changed: finite Stream first-data allowance became
the requested duration plus the existing three-second margin, under a Linux-only
guard. Continuous operation, Buffer, later stall handling, command fields and
ownership were unchanged. The 477-file runtime snapshot, configuration, build,
make check and private install were verified. Four synthetic processes covered
eight attempts, including recovery on the same open device: delayed first data,
no data and a stall after partial data. The synthetic delayed-first case shows
that the changed deadline has the intended effect; it is not hardware evidence.

The physical plan was capped at three captures, zero retries, with a fresh source
preflight before each start. All three completed:

| Linux 5 MS/s control | Independent result | First data after logged ARM |
| --- | --- | --- |
| Installed runtime, continuous host-count Stop | 100,663,296 bytes | 58.198 ms |
| Private runtime, finite 25 million samples | 100,000,000 bytes | 58.118 ms |
| Installed runtime, finite 25 million samples | 100,000,000 bytes | 58.302 ms |

Each passed RAW-to-feed-to-actual-Segment comparison, all 50 fresh CRC packets
in order with two UART decoders, D0 periods of four to six samples, loss-free
kernel request/completion ordering and byte conservation, Stop, drain, END and
resource closure. Electrical witnesses remain limited to D0 and D22. Linux ran
in VMware on the unchanged USB3 path; no reset, hub cycle, firmware or wiring
change was made. This private diagnostic did not modify or run Windows code.

**The additional deadline was never used in any physical capture. The experiment
does not support promoting a longer timeout.** Immediate success with both
private and installed runtimes does not explain the earlier zero-byte failures.
The private runtime was not installed in the normal launcher or published.

## Update: Windows continuous-to-finite cohort stopped after a timing failure

A new, separate plan allowed a native continuous GUI capture followed by a finite
capture in the same device session, then an installed Linux return control. Each
later phase required the previous phase to pass. Only the first physical
acquisition started; no retry or remaining phase was run.

The actual Windows GUI and runtime hashes are the same as those recorded below.
It used Stream, 5 MS/s, continuous sampling, all 32 channels, threshold 1.6 V,
software windows disabled and no hardware trigger. Windows ran in nested KVM,
with exclusive DLA ownership, the reset guard and `guest-reset=false`. The source
preflight passed three fresh ACKs from a live producer; the first 50 commands
892201–892250 were acknowledged.

The laboratory controller was supposed to request native Stop at six to eight
seconds. Its remote flow query took 2.964 seconds, exceeding its 1.5-second guard,
so it skipped the toggle. It then wrongly exited successfully and closed its
source connection after about 13 seconds without confirming acquisition END.
The later manual Stop request was 129.035 seconds after the Run request. Driver
logs show a sample-transfer timeout before backend Stop, two empty drain reads,
worker retirement and END. The source process was not kept alive throughout the
unplanned long acquisition. This is a failed laboratory timing/control attempt,
not a successful transition test or evidence of a product fix. The cause of the
later data stall is not isolated.

The original 2,478,833,664 RAW bytes (619,708,416 samples), zero-byte unconsumed
sidecar and logs were retained. Native Save finished after the error and wrote
an archive containing 591 ordered LOGIC chunks. Its 2,478,833,664 decoded bytes
match an independent RAW bit-plane conversion on all 32 channels, byte for byte;
ZIP CRCs, lengths and original guest-to-host SHA256 hashes passed. The archive
retains `capture status=timeout` and the actual `Capture incomplete` error.
Storage preservation does not make the acquisition complete. No GUI replay of
this large partial archive is claimed.

D0 has 123,945,656 rising-edge periods, all four to six samples, with only four
samples before the first rise and one after the last. Two independent UART
decoders agree on 1,407 bytes, including all 50 fresh CRC packets in order,
without decoder warnings. Those packets witness the initial commanded interval;
they do not establish source-owner liveness or arbitrary all-channel continuity
through the entire unplanned long run.

The GUI then closed with native process exit code zero. The exit-handle wrapper
had first checked an independent process exiting with code seven, avoiding the
old missing/nullable exit-code record. The temporary task was removed. All 11
original files were copied and hash-verified before normal VM shutdown. A final
non-acquiring identification check found the same DLA address 4.16 / 5000 Mb/s;
USB and serial owners were absent. Both installed launchers and all 1,248 local
source hashes, HEADs and dirty differences remained unchanged. The GUI candidate
remains uninstalled; the first-data/replay and broader Windows qualification
issues remain open. Only this report is published; raw data, desktop and VM
material remain private.

## Update: controlled UART witness cohort

A separate, fixed two-acquisition cohort addressed the demonstrated source
preflight gap. It did not repeat the closed five-acquisition cohort. The old
readiness file was still present with a dead owner and zero ECHO commands. A
private pre-start check now requires a live owner, ACKs for three consecutive
ECHO commands, and latest command activity no older than 500 ms. It rejects
the old artifact, missing or
partial acknowledgements, a dead owner and expired activity; a current ordered
control is accepted. This check does not by itself prove electrical capture.

The source was owned by the same bounded foreground orchestrator as each Run,
with commands, received serial bytes, acknowledgements and final closure kept.
There was no detached source waiting during GUI configuration. Neither product
code nor installed binaries changed. The limit was two physical acquisitions,
zero retries; a failure ended the remaining cohort. Serial connections were
opened separately per OS; no uninterrupted MCU uptime is claimed.

| New control | Verified outcome |
| --- | --- |
| Linux Stream, 5 MS/s, continuous with host-count Stop at 25,165,824 samples | Complete 100,663,296 bytes (100 MiB); RAW, original feed and actual Segment storage identical; D0 periods 4–6 with zero outliers; all 50 fresh CRC packets 876601–876650 in order, two UART decoders identical, zero decoder warnings and all source ACKs |
| Actual Windows GUI, Stream, 5 MS/s, finite 50-million-sample request | First-data timeout with zero RAW and zero worker-unconsumed bytes; source pre-start gate passed, producer stayed alive, all 90 serial commands 876801–876890 acknowledged; no captured CRC packets or Windows continuity qualification |

The Linux observer recorded 224 events with exact request/completion order and
byte conservation, no lost or truncated events. HEADER, END, Stop, drain and
close passed. The last preflight command preceded the first USB OUT by at most
285.559 ms, cross-checked using the retained host clock pair. The new continuous
control deliberately differs from the earlier failing finite SETUP. It does
not retest or repair that finite timeout.

The Windows SDK made four ordered 1 MiB reads, each returning zero bytes with
status zero / Windows error zero. The first-data deadline was reported at
approximately 3 s; the worker then accepted Stop, obtained two empty drain
reads, retired and closed both traces without a diagnostic failure. The GUI
showed `Capture incomplete`. A native Save attempt reported that a logic channel
without data cannot be saved; no export file was created. No empty payload or
replay result was fabricated.

Windows serial command emission spanned approximately 0.5–9.4 s after the native
Run request. Some commands followed the early acquisition Stop; the 90 ACKs are
control-port evidence, not 90 captured packets or an electrical D22 claim. This
attempt excludes a missing source process as its explanation. It does not
establish the reason for the original producer termination or the acquisition
failure. A Windows kernel USB trace was not recorded for this new attempt.

All three logged pre-Stop / SETUP / ARM 54-byte prefixes match the corresponding
prefixes of the earlier successful Windows 200 MB acquisition. This compares
the logged active fields, not full kernel-observed 2048-byte transfers. Different
outcomes with these same fields do not establish a deterministic setup defect,
a firmware cause or a transport fix. The first-data and replay issues remain
open, and the GUI candidate remains uninstalled.

Cleanup also retained a laboratory sequencing failure: the initial archive step
found the GUI process still running, and VM shutdown was requested before copying
finished. All original guest files were subsequently recovered after booting
without DLA assignment; their hashes were verified and the temporary task was
removed. The launcher exit record is absent, so orderly GUI exit zero is not
claimed. Backend Stop, quiet drain and worker retirement had already completed.
A final normal VM shutdown left USB and serial owners absent; the DLA remained
at address 4.16 / 5000 Mb/s and answered the DL32 identification query. There was
no DLA reset, hub cycle, firmware write or extra acquisition during recovery.

Raw data, serial and USB traces, desktop/authentication evidence and VM material
remain private. Only this reviewed report is published. The earlier results
below retain their original scope and failures.

## Earlier desktop cohort: five acquisitions, zero retries

The real Windows desktop GUI completed four planned acquisitions and preserved
all captured data through native Save As, file reopening and another Save As.
This is **partial qualification, not release approval**: the Windows UART source
producer stopped before emitting any numbered messages, and the final Linux
return control timed out without sample data. No new GUI was installed.

## Scope and fixed plan

Linux runs in VMware; Windows runs in a nested KVM VM. The device is the
FNIRSI DLA-32 (`1a86:5537`), Linux USB path `4-2`, address `4.16`, negotiated
5000 Mb/s. All 32 channels were enabled, threshold 1.6 V, Stream mode. The DLA
firmware version is unknown. Wiring and firmware were unchanged. The source
preflight reported v8, PWM 1 MHz / 50%, camera enabled, SD disabled.

The cohort was capped at five physical acquisitions and zero retries. Windows
owned the DLA exclusively for its four acquisitions. Linux acquired it only
after the Windows hardware session closed and the guarded USB handover finished.
The QEMU reset guard and `guest-reset=false` were retained. There was no hub
power cycle, DLA reset, firmware write or simultaneous acquisition.

The actual desktop executable was SHA256
`206d85840def2907c0e04733614c2854afd2ee6533380e7e5dfe2df9fe522f05`.
Its 407 application snapshot files match the current source snapshot; the one
changed regression test was already qualified separately. Runtime hashes were:

| Component | SHA256 |
| --- | --- |
| Windows libsigrok | `bf0a6c78b1ee4d8f6a3e2c5a0ca234cd2fef283419551023830f40d77bbad3a6` |
| Windows libsigrokcxx | `a1fc7442465f95c7deb5692e15e68df656ab110660fbe62edbe1d579dfb94be6` |
| Windows libsigrokdecode | `628767ce4f2f9d19185860a4ee02917a9f069e28bf7ad2fc666d3cbe633de0b3` |
| Linux libsigrok | `84c0c7da14777f27871587dfbda1a0645231d051bc10d51feb089c5769757890` |

## Desktop observations

The software trigger used OR between D0 HIGH pulse <600 ns and D22 HIGH pulse
>10 s. Windows contained 200,000 samples with 50% pretrigger. Hardware triggering
was disabled. The original setup was restored through the normal GUI command
and inspected in the editor before capture. A native Save Session Setup kept
the same trigger conditions, OR, window/pretrigger and capture-count values.
This does not claim a second GUI restore of the saved output.

| Acquisition | Requested | Retained result |
| --- | --- | --- |
| Windows 1 | 5 MS/s, three windows | Three complete 200,000-sample windows; 2,097,152 RAW bytes |
| Windows 2 | 50 MS/s, three windows | Three complete 200,000-sample windows; 2,097,152 RAW bytes |
| Windows 3 | 50 MS/s, 10,000-window target; native Stop within 1 s | Stop requested after 0.292209366 s; seven windows retained, global status `stopped`; last window has 186,137 samples and status `stopped`; 5,242,880 RAW bytes |
| Windows 4 | 5 MS/s, finite 50 million samples | Complete 200,000,000-byte saved LOGIC payload; 200,278,016 RAW bytes |
| Linux return | 5 MS/s, finite 25 million samples | First-data timeout after approximately 3 s; zero RAW and LOGIC bytes; no retry |

Windows 4 was amended **before starting** from the planned 25 million samples
(100 MB) to the supported native preset of 50 million (200 MB). The GUI sample
count selector is not editable; attempts to enter 25 M did not start a capture.
The original plan and the amendment are both retained. Linux's requested
25-million-sample return control was unchanged.

Each Windows trace closed with no decoder-input tail or trace failure. Each
Stop was accepted, the worker completed its quiet drain and retirement, and
its unconsumed RAW sidecar was retained separately. No sidecar byte was silently
inserted into the saved data.

Independent bit-plane conversion of original RAW equals every saved 32-bit
word. Each of the 13 windows equals its declared RAW slice, including overlapping
windows and the partial last window. Origins, frame order and rates are exact.
Every targeted complete window places the trigger at sample 100,000. The D0
trigger marks the falling edge closing a HIGH pulse shorter than 600 ns.
D0 rising-edge periods stay within 4–6 samples at 5 MS/s and 49–51 at 50 MS/s;
the full 50-million-sample return capture has no D0 period outside 4–6.

The same executable reopened all four files in the user desktop with device
scanning disabled. Native Save As created new outputs. All 13 window payloads
and the 200 MB finite payload match byte for byte after replay; metadata,
origin, trigger, order, status and error values are preserved. ZIP CRCs, member
lengths and manifest SHA256 values were independently checked. Container hashes
may differ because ZIP timestamps change. Original exports remained unchanged.
The stopped partial window remains stopped after this round trip.

## Qualification failures and limits

The independent serial source process was externally terminated (exit 143).
Its preflight and initial serial bytes survive, but its ECHO command journal is
empty and neither its postflight nor its final-result file exists. The reason
for termination is unknown. A readiness file was incorrectly treated as enough
to proceed; it proved preflight only, not ongoing emission. Future source checks
must verify actual command emission and process liveness before each capture.

Consequently **none of these Windows captures qualifies fresh D22 counter
continuity**. The long capture contains zero numbered CRC packets. Two UART
decoders agree, but agreement without fresh packets does not meet the planned
acceptance criterion. The independent RAW/storage comparison and the limited
D0 witness remain valid; neither proves arbitrary electrical continuity across
all 32 channels. No replacement Windows acquisition was made.

The final Linux return used a synchronous producer: 31 fresh commands
900001–900031 were acknowledged by the unchanged source, but no sample bytes
arrived. HEADER, error metadata, END, Stop, two empty drain reads and close were
retained; cleanup completed without the probe watchdog. A loss-free Linux USB
observer recorded 34 events / 17 closed URBs, zero sample payload and eight
pending IN requests. The 133 microseconds without pending IN occurred after
request retirement, immediately before Stop; it does not explain three seconds
without data. This observer is inside Linux VMware, not a physical bus analyzer.

All four command packet hashes match a previous successful Linux 5 MS/s,
100 MB control. Different outcomes with identical commands do not establish a
firmware cause, a deterministic setup defect or a transport fix. The failing
return remains a failure; Linux recovery is not claimed. Stream replay and
first-data failures remain open. No rate filter, retry, delay or reset was
added to product code.

The desktop GUI processes closed and both temporary GUI tasks were removed.
The replay task returned zero. The first hardware launch wrapper recorded a
null exit code, so its exit code is not claimed as zero; its process was absent
before handover. USB and serial ownership were released. Existing product
sources, dirty work, installed binaries and the Windows launcher were preserved.
Raw captures, source logs, USB traces, desktop images and VM credentials stay
private; this publication contains only the reviewed report.
