# Visual idle-time compression

The trace toolbar's **Compress idle** switch folds long constant intervals in
an existing stopped digital capture. **Idle >=** sets the minimum interval;
the initial value is 1 ms. The switch is off by default.

This changes display geometry only. Every recorded sample and timestamp stays
intact. Highlighted bands and break marks identify folded intervals; labels
show their real duration, and hovering over a band gives more precision. At
very narrow zoom levels, a label may be shortened; its full duration remains
available in the tooltip. Cursor deltas, marker times, decoder annotations and
exports continue to use original time, including any folded delay.

Turning the switch off restores the ordinary time axis and the scale/offset
held before enabling compression. It does not require another capture. Saved
session setups retain this ordinary viewport and the idle threshold, so they
reopen with compression off rather than an incomplete display transformation.

Idle detection examines all stored digital inputs, including recorded inputs
whose traces are hidden. Even a one-sample pulse splits an interval. A clock
that runs continuously may therefore leave no intervals to fold. Detection and
map construction run in short, cancellable steps; no samples are discarded.

The initial implementation supports one synchronized digital segment with a
known sample rate. Analog/mixed displays are explicitly rejected. A new capture,
data replacement or segment change cancels compression. Decoder rows use the
same display mapping as their digital inputs; decoder-enabled builds support
the feature as well.

Short unchanged runs remain at the normal scale. Long runs retain up to eight
samples of context on each side. Folded display widths reserve room for readable
duration labels, and are always shorter than the original interval. If a capture
contains more than 100,000 candidate intervals, increase the idle threshold.

This is separate from segmented acquisition. It neither reduces USB traffic nor
infers a precise delay between independently acquired segments without suitable
timing metadata.
