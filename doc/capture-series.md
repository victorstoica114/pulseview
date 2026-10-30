# Multiple triggered captures per Run

The toolbar's **Captures** field defaults to **1**. Set a larger value for a
finite digital acquisition to retain that many captures from one Run. Each
acquisition uses the current trigger, sample rate, depth and device settings.
Buffer and finite Stream use the same driver start/stop paths as a single
capture. Continuous, analog and mixed acquisitions do not support repetition.

With **Stream windows** off, this is software rearming, not autonomous FPGA
segmentation. The analyzer is blind between acquisitions and can miss input
events. The exact delay between
captures is unknown; each segment keeps its own sample time axis. Neither the
segment number nor host arrival time establishes a continuous hardware timeline.
The existing Windows/WCH Stream shutdown/settling safeguards still apply.

Only trigger timestamps explicitly provided to PulseView are saved; missing
timestamps are not inferred from a configured pre-trigger percentage. In the
tested DLA-32 build no trigger-position packets were reported. Short triggered
Stream traces could begin directly in the UART start bit, without preceding
idle, causing a decoder to lose its initial framing. A collection preserves
those exact received samples; it does not reconstruct missing context. The
original finite-capture numbered-payload test therefore also uses a separate arm
message followed by a recorded idle interval. This is a test fixture, not a
capture-boundary fix.

Stop cancels the series, including startup and the interval before the next
acquisition. Errors, timeouts and device disconnects also stop repetition.
Already received samples remain available, including an incomplete last segment.
The segment selector permits inspecting each capture; its tooltip describes
the capture outcome. A sealed segment is not by itself proof of complete data.

## Windows from a continuous Stream

For short, infrequent events, select **Stream**, set a finite sample depth and
a standard digital trigger, then enable **Stream windows**. **Captures** becomes
the number of windows to retain. **Pre %** sets the fraction of each window
before its trigger (1–90%, default 50%). For example, 100 k samples at 1 MS/s
with Pre 50 retains 50 ms before the trigger and 50 ms starting at the trigger.

This mode starts the device once, without a hardware trigger and with its
continuous control enabled. PulseView scans the incoming samples for the
configured trigger and retains windows with a circular pre-trigger history.
It restores the original trigger and finite acquisition limits after Stop.
The initial history must fill before a trigger can be accepted. An edge needs
two received samples; PulseView never invents an idle level before the first
sample. Thus a correctly chosen pre-trigger interval can retain the actual
idle/CS context needed to decode the first message.

All matches within a standard trigger stage are simultaneous (AND). Multiple
stages match consecutive samples, including across USB packet boundaries.
Supported conditions are low, high, rising, falling and either edge; up to
64 enabled digital channel indices and 64 stages are supported. Extended FPGA
trigger qualifiers are not evaluated by this software mode. Devices must
expose readable/writable continuous acquisition controls; analog and mixed
input configurations are rejected. File replay restores the saved windows
rather than applying the extractor again.

While a window is collecting its post-trigger samples, additional matches are
already recorded inside that window. They do not create overlapping windows or
increment the Captures count. Choose a window long enough to include a whole
message, and use a CS/event condition to select transactions when appropriate.
Outside retained windows samples are discarded, while the pre-trigger history
continues updating. This is deliberate selection, not a full recording of all
traffic. The window target counts retained windows, not every input event.

The device is not stopped or rearmed between windows, removing the blind gaps
of finite capture repetition. Normal driver shutdown safeguards apply once at
the end of the Run. USB traffic and host sample-processing cost are unchanged:
this mode does not fix device/firmware data loss or make an unsupported Stream
rate reliable. Testing on DLA-32 covered 32 channels at 1 and 10 MS/s and a
sparse D0+D22 mask at 10 MS/s, including 12 numbered UART messages spaced
250 ms apart. It does not qualify 50 MS/s, 200 MB/s transport or 20 MHz SPI.

Stop, timeout and disconnect retain completed windows and any triggered partial
last window, with their actual sample counts and outcomes. Prehistory alone,
before any accepted trigger, is not saved as an event. If configuration cannot
be restored, PulseView reports the failure; scan/select the device again before
another acquisition. A transport error remains visible even if restoration
also fails.

Each retained window records its origin in the same source sample clock. The
segment selector tooltip shows that origin and the spacing between successive
triggers. These intervals come from received sample positions, not host arrival
times. They remain meaningful only while the device delivers a valid continuous
stream; undetected device-side losses cannot be reconstructed.

## Saving and export

With multiple segments, **Save** / **Save As** writes all captures to a
**.srcollection** file. This is an explicit PulseView collection format, not a
change to the standard sigrok `.sr` format. A single ordinary acquisition still
saves as `.sr`; even a single continuous-Stream window uses `.srcollection` to
preserve its source origin and software trigger. Other exports, including
**Save Selected Range As**, use the segment
currently selected in the main trace view. Exporting one segment does not mark
the whole collection saved.

A collection is a ZIP containing a versioned `pulseview-frames.json` manifest
and `frames/000001.sr`, `frames/000002.sr`, etc. Each member is a standard
single-frame sigrok archive and can be extracted for other readers. The manifest
records sample counts/rates as decimal strings, known relative trigger positions,
per-frame and overall outcomes, member sizes and SHA-256 checksums. Inter-frame
timing is explicitly `unknown` for independent acquisitions (format version 1).
Continuous-Stream windows use version 2 with `inter_frame_timing: source_samples`
and a decimal-string `sample_origin` for each frame at a common sample rate.
This preserves exact 64-bit indices, including positions above 2^53. Readers
that only support version 1 reject version 2 explicitly. There is no
concatenation or invented idle data in either format.

Reopen collections using the normal Open dialog. Playback restores every frame,
its outcome and known trigger position, once regardless of the toolbar capture
count. Manifest validation, bounded member extraction and checksums reject
malformed/truncated data. A damaged later member can leave earlier received
frames available as a partial load, with an error instead of a success claim.

Saving stages ordinary member archives, then builds the outer ZIP and atomically
replaces the destination. Failure or cancellation preserves an existing target.
Staging requires temporary disk space as well as space for the final file;
buffers remain bounded. Session setup can be saved separately with **Save
Session Setup**; it includes the requested capture count, Stream windows option
and pre-trigger percentage. Collections do not
automatically create a companion setup file.

Visual idle compression is independent: it folds fully recorded constant
intervals without changing samples or their real duration. It cannot reconstruct
events missed during rearming, and currently operates on a single segment.
