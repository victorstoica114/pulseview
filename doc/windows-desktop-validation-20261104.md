# Windows desktop capture and replay observations — 4 November 2026

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
