# Windows desktop capture and replay observations — 4 November 2026

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
