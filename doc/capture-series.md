# Multiple triggered captures per Run

The toolbar's **Captures** field defaults to **1**. Set a larger value for a
finite digital acquisition to retain that many captures from one Run. Each
acquisition uses the current trigger, sample rate, depth and device settings.
Buffer and finite Stream use the same driver start/stop paths as a single
capture. Continuous, analog and mixed acquisitions do not support repetition.

This is software rearming, not autonomous FPGA segmentation. The analyzer is
blind between acquisitions and can miss input events. The exact delay between
captures is unknown; each segment keeps its own sample time axis. Neither the
segment number nor host arrival time establishes a continuous hardware timeline.
The existing Windows/WCH Stream shutdown/settling safeguards still apply.

Stop cancels the series, including startup and the interval before the next
acquisition. Errors, timeouts and device disconnects also stop repetition.
Already received samples remain available, including an incomplete last segment.
The segment selector permits inspecting each capture; its tooltip describes
the capture outcome. A sealed segment is not by itself proof of complete data.

## Saving and export

With multiple segments, **Save** / **Save As** writes all captures to a
**.srcollection** file. This is an explicit PulseView collection format, not a
change to the standard sigrok `.sr` format. A single acquisition still saves
as `.sr`. Other exports, including **Save Selected Range As**, use the segment
currently selected in the main trace view. Exporting one segment does not mark
the whole collection saved.

A collection is a ZIP containing a versioned `pulseview-frames.json` manifest
and `frames/000001.sr`, `frames/000002.sr`, etc. Each member is a standard
single-frame sigrok archive and can be extracted for other readers. The manifest
records sample counts/rates as decimal strings, known relative trigger positions,
per-frame and overall outcomes, member sizes and SHA-256 checksums. Inter-frame
timing is explicitly `unknown`. There is no concatenation or invented idle data.

Reopen collections using the normal Open dialog. Playback restores every frame,
its outcome and known trigger position, once regardless of the toolbar capture
count. Manifest validation, bounded member extraction and checksums reject
malformed/truncated data. A damaged later member can leave earlier received
frames available as a partial load, with an error instead of a success claim.

Saving stages ordinary member archives, then builds the outer ZIP and atomically
replaces the destination. Failure or cancellation preserves an existing target.
Staging requires temporary disk space as well as space for the final file;
buffers remain bounded. Session setup can be saved separately with **Save
Session Setup**; it includes the requested capture count. Collections do not
automatically create a companion setup file.

Visual idle compression is independent: it folds fully recorded constant
intervals without changing samples or their real duration. It cannot reconstruct
events missed during rearming, and currently operates on a single segment.
