# Hardware trigger controls

This fork's `master` includes the generic trigger sequence editor and optional
Count/Continuous and clocked-word controls. Build it with the matching
libsigrok fork's `master`:
<https://github.com/victorstoica114/libsigrok/tree/master>.
The proposed `SR_CONF_TRIGGER_OPTIONS` API is not in upstream libsigrok yet.

Stop capture and use **Trigger...** in the toolbar. Devices without the new
capability retain the generic parallel sequence editor. On DLA-32, Buffer mode
is required for qualified parallel or serial triggers.

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

Validation includes 31 offline test cases, production-object GUI checks with a
DLA-32 on Windows/WCH, real Count and SPI-word captures, overflow rejection,
Cancel, extended setup persistence and explicit disable. Hardware validation on
Linux and calibrated trigger positioning remain outstanding. No precise
synthetic trigger marker or waveform correction is added.

Restoring a setup after destroying a channel popup also has a regression check.
Its name field is tracked with a Qt guarded pointer, so later name restoration
cannot access a deleted widget.
