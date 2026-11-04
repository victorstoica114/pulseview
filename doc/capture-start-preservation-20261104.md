# Preserve the previous capture when worker creation fails

Qualified scope, 4 November 2026: the Session source correction below and the
Linux frontend built from it. The same preservation contracts pass natively on
Windows. Delivery of the combined Windows frontend remains unqualified because
one unchanged toolbar visibility test fails six assertions with either Session
version (AUD019). This change does not repair the separate Buffer transport or
electrical failures documented in [the earlier investigation](capture-start-validation-20261103.md).

## Defect and correction

`Session::start_capture()` cleared the previous samples, trigger timestamps,
frame counts, per-frame outcomes and origins before constructing its acquisition
thread. If `std::thread` construction threw, the error was reported but the
previous capture was already lost, together with its acquisition name/time.

The correction clears the previous acquisition at the beginning of the worker,
under the existing data mutex. A refused worker therefore never clears it.
Hardware capture names reset only after successful thread construction; the
previous acquisition start time is restored before reporting a construction
failure. Startup remains visible to Stop before the worker starts. There is no
additional worker, product injection hook, factory, gate, delay or USB change.
The existing worker already clears signal sample data under the same mutex.

The acceptance contract is exact preservation after a refused worker: samples,
frame count, trigger timestamps, origins, global and frame outcomes/errors,
name, saved/unsaved state and acquisition start time. Stopped/error data retain
their actual status. A subsequent accepted acquisition still replaces old data.

## Software and source identity

Three contracts fail with the original Session and pass with the correction on
each OS: complete multi-frame collections, origins above 2^53, and partial/error
collections. The same Session recovers on a fresh replay after each refusal.
The Linux checks were relinked from the fresh full GUI's actual object files;
Windows checks use the verified native snapshot. The diagnostic rejects exactly
one actual acquisition `pthread_create` call with EAGAIN. On Windows its
process-local libstdc++ import slot and original protection are restored and
verified. This is an injected construction refusal, not a claim of system-wide
thread or memory exhaustion.

All 408 frontend source files match the tested snapshot; only `pv/session.cpp`
differs from fork master `1965b38`. Corrected Session SHA-256:
`8ff28276a7b1fff20c31f38f73495e37248916e797bf35444925dc06aaba2cab`.
Original Session: `64e41f8d15e4e90be11afd823f9c44fb974441c154667b9f4268c8a1778eda25`.
The fresh Linux build passes all 78 frontend regression cases, including startup
Stop, repeated captures, Stream windows, collection replay and saving. Native
Windows retains the six identical baseline toolbar assertions mentioned above;
it is not a fully passing Windows suite.

Linux GUI SHA-256:
`d445d284114cfd6f30a1f0bacca6c375d6df4466afab3be86530276823ad178d`.
Native Windows GUI snapshot:
`206d85840def2907c0e04733614c2854afd2ee6533380e7e5dfe2df9fe522f05`.
Private Linux/Windows physical diagnostic executables respectively:
`b7811a0cd0bde16310af56a4ec92d972799754eb84042bd741034f6135c87282` and
`52163c14424b6e2d7115caa7bf143b256c8b75eaaa4ff1f9733e68bcd38a33a0`.
Diagnostics are not linked into the installed GUI.

## Physical acceptance, without concurrent captures

Closed plan: four acquisitions on Linux, then four on native Windows, no retry.
All32, 5 MS/s, threshold 1.6 V, no trigger; source v8, D0 1 MHz/50% PWM,
D22 numbered CRC UART at 115200 8N1, camera=1/sd=0. DLA32 `1a86:5537` remained
at 5000 Mb/s on Linux USB 4-2; Linux runs in VMware and Windows in nested KVM.
Windows handover uses the existing reset guard and guest-reset=false. No hub
cycle, firmware, FPGA, source or wiring change was made.

After every physical acquisition the diagnostic injects one worker refusal,
compares the actual unsaved data and metadata, saves a collection, and replays
that collection. All eight preservation and save/replay checks pass, including
the actual partial timeout data. RAW and actual LOGIC were saved before any
acceptance assertion; independent all32 conversion matches all eight captures.

| OS | Acquisition before refusal | Actual LOGIC bytes | Outcome | D0 bad periods | Complete CRC messages |
|---|---|---:|---|---:|---|
| Linux | Stream 5M samples | 20,000,000 | complete | 0 | 19: 842006–842024 |
| Linux | Stream 25M target, Stop at 300 ms | 3,145,728 | stopped | 0 | 3: 842039–842041 |
| Linux | Buffer 5M target | 4,194,304 | timeout | 172 | 0 |
| Linux | Stream 25M return | 100,000,000 | complete | 0 | 94: 842124–842217 |
| Windows | Stream 5M samples | 20,000,000 | complete | 0 | 19: 843034–843052 |
| Windows | Stream 25M target, Stop at 300 ms | 3,145,728 | stopped | 0 | 3: 843112–843114 |
| Windows | Buffer 5M samples | 20,000,000 | complete | 0 | 18: 843165–843182 |
| Windows | Stream 25M return | 100,000,000 | complete | 0 | 95: 843221–843315 |

For all five complete captures and both requested Stop prefixes, two independent
UART decoders agree, with zero frame warnings and consecutive fresh CRC messages;
source ACKs are complete. D0 periods use the established 4–6 sample tolerance.
The Linux Buffer timeout remains a failed capture: 172 D0 anomalies, zero complete
CRC messages, disagreeing UART decoders and frame warnings. Its bytes and timeout
error survive the refused start and collection replay unchanged. Preserving a
failure does not make its samples valid or qualify Buffer recovery generally.

One passive Linux usbmon parent contains 366 events, zero loss/truncation/error,
and four contiguous capture scopes. RAW matches the complete positive sample
completions in both completion and request order; all unconsumed RAW and kernel
sidecars are retained. It contains exactly four physical acquisition command
groups, including the planned Stop. The failed worker attempts add no acquisition.
The ten existing parser contracts were reused only after byte-identity checks.

Actual Linux mappings remain core `84c0c7da`, C++ `20ec72bc`, decoder `ff037c43`.
Windows recorded the actual runtime paths; hashes sampled after a software-only
normal VM restart are core `bf0a6c78`, C++ `a1fc7442`, decoder `628767ce` and
libstdc++ `4e4822dc`. This is post-capture sampling, not simultaneous DLL hashing.
The fresh full Linux GUI also renders the saved hardware collection and exits
normally with the same mapped libraries. Windows is shut down and DLA/source
ownership is released after testing.

D0/D22 are bounded electrical witnesses. Neither exact byte counts, all32 format
conversion, CRCs nor kernel equality proves arbitrary continuity on all channels.
Private captures, commercial firmware, VM images and credentials are not
publication inputs. Local evidence: `artifacts/capture-preservation-qualification-20261104/`
(`final-review.json`, `data-review.json`, `kernel-review.json`, source/build proofs,
original RAW/LOGIC, collections and before/after fault snapshots).
