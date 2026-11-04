# Toolbar visibility: fixture sizing on Linux and Windows

On 4 November 2026 the six Windows toolbar assertions previously recorded in
[capture-start preservation](capture-start-preservation-20261104.md) were
reproduced and traced to an undersized test fixture. The application toolbar
implementation remains unchanged. This corrects the regression test, not the
acquisition or transport code.

## Reproduction and acceptance

The old test requested a 2600-pixel-wide QMainWindow and assumed that every
visible action would fit in the toolbar. Its action visibility assertions pass,
but its Pre widget visibility assertion fails six times on Windows, both with
the native `windows` Qt platform and with `offscreen`.

Measured native Windows geometry is 1028 pixels for the window/toolbar, despite
requests for 2600 or 5000 pixels; the available screen is 1024 by768 and the bar's
size hint is1446. Pre's action remains visible but its widget is in the overflow.
With offscreen/Fusion, the requested2600 pixels are retained but the bar's hint
is3575; a private5000-pixel fixture passes. These are fixture/platform observations,
not a fixed DPI or minimum screen requirement for PulseView.

A private native Windows extension probe confirms that Pre appears on the second
row after expansion. An early150ms observation sees it still hidden; at750ms it
is visible with the rest of the second row. Both observations are retained. This
sampling delay belongs only to the diagnostic; it is not a product workaround
or a universal animation duration. Qt5's [toolbar layout implementation](https://github.com/qt/qtbase/blob/5.15/src/widgets/widgets/qtoolbarlayout.cpp)
skips action layout while animating and supports the expanded toolbar layout.

Acceptance requires enough actual layout space before asserting that a visible
action's widget is visible. Disabled mode and file devices must still hide both
controls, and restoring a live source must restore both. No visibility assertion
is removed or weakened.

## Minimal test correction

Only `test/trigger_sequence.cpp` changes in the executable sources. The local
fixture helper processes queued selector/polish events before measuring the
bar's current size hint, sets that as the window's minimum width, resizes it,
processes layout events, and asserts actual bar width is at least its current
hint. It runs after every tested mode and device transition. This handles the
native window size cap and varying fonts/styles without changing the product's
layout, its DPI settings, global screen configuration, or capture behavior.

The first candidate omitted the pre-measurement event processing. A full Linux
suite correctly rejected its sizing precondition (1270px actual vs1524px hint).
That failure and source are retained. The final source includes the event
processing and passes on all four tested platform configurations.

Final test source SHA-256:
`6ed2c4c21784709a978dfe74f13c8a51d735375a48ea50d1a416240258b39503`.
All407 other frontend snapshot files match the previous408-file snapshot from
`b929c183`; no GUI object/source or installed executable is promoted by this
change. Candidate test executables:

- Linux: `7f02647a45b0c5a2f87dd9074474774d6a523eda3281373b79e560a029441db5`.
- Windows: `4ed7787cfb1088b544d51cfe1ea811de1530e018f540156e786d2da5b0ba2854`.

The final full78-case suite passes with exit0 on Linux `xcb` and `offscreen`,
and on Windows `windows` and `offscreen`. Each final variant uses the same
application object files, with only the test object replaced. Negative controls
compile the retained pre-AUD019 MainBar.cpp against the current ABI-compatible
header (its additional unused pointer slots remain present), replacing only
that implementation object. The corrected test still rejects that original
visibility behavior:22 assertions fail/exit201 on Linux X11 and both Windows
platforms. Thus fitting the fixture does not hide the original product defect.

Private before/fit/overflow diagnostics preserve their original exits and
screenshots. The overflow diagnostic still exits201 because it deliberately
retains the old six no-overflow assumptions; its later expansion checks pass.
It is not described as an all-green test run.

## Scope and environment

No physical acquisition, USB assignment, reset, flash, hub operation, source or
wiring change occurred during this unit. Hardware is excepted for this test-only
change: application acquisition code and GUI binaries are byte-identical to the
previously verified versions. This does not create new electrical qualification.
Linux is a VMware guest; Windows is nested KVM. Windows was started only for
software tests, had no DLA assigned and was normally shut down afterwards.

Installed Linux GUI remains `d445d284`, core `84c0c7da`, C++ `20ec72bc`, decoder
`ff037c43`. Native Windows GUI remains `206d8584`; sampled libraries remain core
`bf0a6c78`, C++ `a1fc7442`, decoder `628767ce`. No new Windows GUI is installed.

The six old assertions no longer block the AUD019 toolbar contract. This is not
a claim that every Windows GUI/high-rate capture path is qualified: outstanding
AUD002 acquisition/GUI coverage and the independent Buffer failures remain
open. The earlier eight physical AUD025 preservation/save/replay results retain
their original outcomes, including the Linux Buffer timeout and corrupted data.

Local evidence: `artifacts/toolbar-windows-audit-20261104/`, including original
geometry and overflow screenshots, the failed first candidate, final
`qualification-r2/` logs/object hashes, source snapshots, unchanged installation
proof and the final review. No private captures, firmware, VM images or
credentials are publication inputs.
