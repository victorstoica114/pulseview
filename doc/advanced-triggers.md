# Hardware trigger controls

This fork's `master` includes the generic trigger sequence editor and optional
Count/Continuous and clocked-word controls. Build it with the matching
libsigrok fork's `master`:
<https://github.com/victorstoica114/libsigrok/tree/master>.
The proposed `SR_CONF_TRIGGER_OPTIONS` API is not in upstream libsigrok yet.

Stop capture and use **Trigger...** in the toolbar. Devices without the new
capability retain the generic parallel sequence editor. On DLA-32, Buffer mode
is required for qualified parallel or serial triggers.

A single unqualified stage with multiple channel conditions uses standard AND
triggering and works in both Buffer and Stream with the matching DLA-32 driver.
Multiple stages and Count/Continuous or serial matching still require Buffer.

The pre-trigger capture-ratio control is disabled as **Unavailable** in Stream.
Switching back to Buffer restores its value and its listed range, including
when the configuration popup was originally opened in Stream. Refreshing the
value or bounds does not write a clamped placeholder back to the device.

- **Parallel sequence:** all conditions in a stage must match before advancing.
  Add, duplicate, remove or reorder up to 16 stages. Count/Continuous belong to
  the selected stage and follow it when duplicated or moved. Count is 1–255:
  edge occurrences for edge conditions, nominal nanoseconds for level
  conditions. For levels, Continuous requires uninterrupted matching time;
  otherwise matching time can accumulate. Mixed conditions remain experimental.
- **Serial word:** choose distinct enabled clock/data inputs, the sampling edge,
  1–16 bits and a decimal or `0x` hexadecimal word fitting that width. Matching
  uses the latest sampled bits, MSB first. There is no chip-select or protocol
  framing support. This selects a hardware clocked-word comparator, not a decoder.

Apply validates the complete candidate before replacing the live configuration.
Cancel preserves it. The editor rejects stale changes if the device, ordinary
trigger or hardware options changed while the dialog was open. The simple
per-channel trigger menu is protected for qualified and serial triggers; use
the toolbar editor to edit or disable them.

**Save session setup...** stores both the ordinary stages and hardware options.
**Restore session setup...** validates both before applying them. Unsupported or
malformed hardware settings preserve the previous trigger and produce a warning.
Legacy setups retain the existing ordinary-trigger behavior. Serial mode is
shown on the toolbar and capture enters the awaiting-trigger state.

Validation includes 41 offline test cases, production-object GUI checks with a
DLA-32 on Windows/WCH, real Count and SPI-word captures, overflow rejection,
Cancel, extended setup persistence and explicit disable. Hardware validation on
Linux and calibrated trigger positioning remain outstanding. No precise
synthetic trigger marker or waveform correction is added.

The availability regression reproduces the former missing-control failure.
Additional real DeviceOptions checks cover Stream-first construction, mode
round trips, restored bounds and a successful ratio edit in Buffer.

Restoring a setup after destroying a channel popup also has a regression check.
Its name field is tracked with a Qt guarded pointer, so later name restoration
cannot access a deleted widget.

Crash regressions use the production QApplication and actual visible-widget
focus changes, including condition/threshold columns. Retired condition widgets
are disconnected before deferred deletion so queued input cannot change a
different condition or address a removed row. Session setup restoration skips
unavailable views and invalid references while preserving marker indexes.
Standard exceptions in the capture worker stop acquisition, report an error
and preserve an unsaved error outcome instead of escaping the worker thread.
The tests also cover popup destruction and failed/successful file exports.
