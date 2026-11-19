# DLA-32 Plus: data integrity and measurement limits

Qualification snapshot: 19 November 2026. This note documents observed limits;
it does not introduce a transport workaround, firmware change or new rate limit.
Historical results below are taken from retained October reports, not new
captures or a re-analysis of their original data in this documentation update.

## What is, and is not, established

Correct sample counts, successful USB requests, valid CRCs in retained messages
and exact RAW-to-LOGIC conversion do not by themselves prove continuity. A
capture can have the expected length while containing repeated regions and
missing intervening data. Periodic PWM alone can also hide lost whole periods.
Use known sequence numbers or another independent temporal witness when
continuity matters. Evidence from a few connected inputs does not qualify the
electrical integrity of every enabled channel.

Two distinct observations remain open: long-Stream discontinuities, and bias or
variation in measured HIGH/LOW durations. A duty-cycle discrepancy is not by
itself proof of missing samples, and neither observation establishes one common
firmware cause.

## Long Stream: observed discontinuities

The archived 7 October comparison used the official FNIRSI Windows application
and WCH path, USB3, Stream, 32 channels enabled, a 1.6 V requested threshold,
Normal/Upload with D0 rising and 50% trigger position, RLE disabled and no glitch
filtering. The witnesses were numbered UART TX, SPI MOSI and I2C write/NACK
signals, not arbitrary activity on all inputs.

| Historical official-application case | Observed result |
| --- | --- |
| 50 MS/s, 100 ms | The bounded protocol witnesses passed. |
| 50 MS/s, 20 s | Counter gaps and repeated intervals; 387 counter values absent from all three valid decoded streams within their common observed range, and 373 extra occurrences per stream. |
| 25 MS/s, 20 s | 2,431 consecutive CRC-valid messages per protocol; pass only in that test scope. |

Per-channel native exports matched the saved session data. Independent SPI
edge extraction reproduced the 50 MS/s discontinuities and the clean 25 MS/s
reference. These are historical test results, not a current-release FNIRSI
certification or a guarantee that all 25 MS/s captures are lossless. This summary
does not establish a read-back firmware identity for those sessions.

Separate native-transfer measurements on 8 October used DLA Logic 1.0.3.0 and
WCH 3.4.2023.10 on Windows 11 Pro build 26300. Successful USB completions measured
about 156.95 MB/s at a requested 50 MS/s and 100.27 MB/s at 25 MS/s.
Those timed captures were not fully
decoded for continuity; their throughput must not be presented as a payload
integrity result. Uncompressed 32-channel data requires 200 MB/s at 50 MS/s.

Later Linux/VMware investigations also retained discontinuities, wrong-time-scale
and first-data failures, alongside successful bounded controls. Later Windows
tests ran in KVM nested inside Linux/VMware. The
[Linux observations](linux-validation-20261102.md) and
[Windows observations](windows-desktop-validation-20261104.md) retain those
configurations and their limits. Successful later tests do not erase earlier
negative evidence or qualify arbitrary-duration Stream on every configuration.

The official-application result shows that this class of symptom is not exclusive
to PulseView/libsigrok. It does not isolate device firmware, FPGA, USB transport,
WCH or application behavior, or prove that every observed failure has one cause.

## Other recorded native-application limits

A separate 4 October series in official DLA Logic 1.0.3.0 used unchanged
D0/D1 wiring, Stream at 50 MS/s, 5 million samples per capture, 32 inputs enabled,
a 1.6 V threshold, D0 rising, RLE off and no glitch filtering. Four of ten
captures showed the signal activity on D4/D5 or D16/D17 instead of D0/D1.
All32 native CSV exports matched their saved sessions. That establishes the
observation in the two native representations, not its USB or firmware cause.

The same application also rejected Raw Data / All enabled channels / BIN for
a 32-channel capture with `Parameter exception`. Individual-channel exports
succeeded and could be compared with the saved session. This exporter failure
is separate from corruption or discontinuity in acquired samples; retain the
native session and use relevant per-channel exports for an independent check.

## Duty cycle: quantization and unresolved threshold dependence

A 1 MHz signal reported as 50% on an oscilloscope was investigated separately.
The oscilloscope's uncertainty was not recorded. At 25 MS/s there are 25 nominal
samples per period: HIGH/LOW counts of 12/13 or 13/12 give 48% or 52%. A fixed
phase relationship can preserve that bias across many periods. Such a result
alone is not a firmware defect.

The 3 October comparison also retained 25 Buffer captures from official
DLA Logic 1.0.3.0, with 32 inputs enabled, 10 ms requested, RLE off and no glitch
filtering. The D0/D1 exports matched the saved native sessions. For the 1 MHz
internal PWM witness:

| Sampling and requested threshold | Official-application observations |
| --- | --- |
| 100 MS/s, 2.5 V | Three of four captures had HIGH/LOW 490/510 ns, or 49% duty; the fourth was about 49.16%. |
| 250 MS/s, 2.5 V | 48.8–49.2% duty across four captures. |
| 250 MS/s, 0.6 V | About 51.47–52.01% across four captures, including the first acquisition. |

The first acquisition after a threshold change also showed HIGH durations
changing within the capture. Retain that first acquisition rather than averaging
it away. Requested threshold values are not calibrated physical switching
voltages. Kingst comparisons also showed threshold-dependent bias and did not
provide a calibrated timing reference.

Not all these observations fit an ideal one-sample rounding explanation.
Input threshold crossings, receiver delays, phase and configuration behavior
remain to be separated. The results do not justify forcing duty to 50%, shifting
channels, discarding initial samples or attributing the cause to firmware alone.
They do not certify the precision of other inputs, frequencies or thresholds.

## Practical interpretation

Keep partial data and explicit failure status. The qualified
[WCH terminal-reporting change](wch-terminal-reporting-20261108.md) improves
reporting after failed or unfinished drain; it does not detect every possible
discontinuity or repair the transport. An apparently successful END cannot
establish continuity of arbitrary unknown signals.

Use the exact mode, rate, channel mask, threshold and duration supported by a
specific verified case. A short Buffer or windowed success is not a general
long-Stream qualification. Preserve original sessions, failed attempts and
source/version information; do not fill, reorder, filter or silently drop data
to make a capture appear correct. Missing arbitrary source information cannot
be reconstructed generically by the host.

## Independent check with the official application

If the official application is already available on a supported system with
stock device firmware, the useful comparison is a bounded 20 s Stream capture
at 50 MS/s and a 20 s reference at 25 MS/s, with the same source, threshold,
trigger/pretrigger settings and 32-channel mask, RLE disabled and no glitch
filtering. Record application, firmware information as available, OS, driver
and negotiated USB speed.

Prefer a source containing increasing UART/SPI sequence numbers or another known
nonperiodic pattern. Check both ordering and missing/repeated values, not just
CRC success or displayed sample count. A square wave alone gives a limited
timing check. Keep the original native session and relevant per-channel exports,
including an incomplete attempt, and report the actual saved extent. A nominal
20 s at 50 MS/s represents 4 GB of uncompressed 32-channel data; use a shorter
declared duration if memory/storage makes the requested case impractical.

An optional separate duty check uses a stable 1 MHz / 50% input, independently
checked at the analyser input: Buffer, 10 ms, 100 or 250 MS/s, first and subsequent
captures at the same threshold. Record HIGH/LOW sample counts, requested
threshold, input reference and any changes between captures. Compare threshold
settings separately and account for sampling quantization.

A reproducible discontinuity in the official application on an independent
stock setup would support a report to FNIRSI for investigation of its device
and software stack. Preserve the exact reproduction and avoid assigning the
fault to one internal component before that evidence exists. Defects demonstrated
in our own conversion, storage or error reporting remain our responsibility.
