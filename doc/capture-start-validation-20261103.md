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
