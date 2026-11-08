# WCH terminal failure reporting in the current installed runtime

On 8 November 2026 the existing
[three-line WCH fix](https://github.com/victorstoica114/libsigrok/commit/f63c9aea46302b40ce8172d78b02181118beb8bb)
was qualified with the current full fork runtime and activated for the Windows
GUI/CLI launchers. Previously, successful Stop followed by failed or nonquiet
drain could still produce `complete` and CLI exit zero. The change marks that
terminal result `error` (or `disconnected`) before END, preserving delivered data.
It adds no USB operation, retry, delay or sample transformation. This closes the
local reporting integration, not the unexplained transport issues.

## Exact source and executable scope

The baseline is the qualified full 477-file runtime from
[the earlier combined activation](combined-local-activation-20261108.md).
Only fnirsi-dla32/api.c changes, by the three published lines. The candidate's
physical driver and common sources match the earlier full WCH477 candidate;
its only remaining source differences are the separately qualified Demo files
and their tests. Primary repositories and their existing uncommitted work are
preserved.

The installed Windows C core is SHA256
`188bfa73ab2333d82171b5b006a9c347ac1560b03eafcf220b7dced82e8df8c4`,
replacing core64f89 in a new versioned application directory. GUI5fe4, C++a1fc,
decoder6287, CLIe9aa and vendor DLLdb19 are unchanged. Actual GUI module paths
and hashes are checked through the installed CMD launcher. The SYS hash remains
`afa9f44918c7fd2ba3ebe8c1d54a63557d7f434177088dafb40242127519e60b`;
no driver installation occurs.

Linux keeps its previously qualified installation. The changed code is inside
the native Windows worker branch. Preprocessing baseline and candidate with the
actual Linux configuration produces identical 772,312-byte translation units.
This is an explicit platform exception, not a claim that WCH ran on Linux.

## Reproduction, correction and consumers

Windows runs in KVM inside Linux/VMware, without a DLA assigned. The real C core,
API session and CLI run against the deterministic private SDK fixture; actual
core and fake-SDK mappings are recorded. Seven cases cover quiet drain, Stop
failure, drain error, nonquiet drain, disconnect, empty timeout and partial
then empty timeout. Baseline and candidate each run all seven through both API
and CLI. The current baseline reproduces false success for drain-error and
nonquiet; the other five controls agree with the contract. The candidate reports
the expected failure/status and CLI exit one. All delivered 32,000-byte payloads
remain exact; HEADER/END are 1/1, API destruction/close/exit return zero, and all
original child waits are retained. API run success alone is not treated as
capture success.

The full candidate passes 132 C checks, the C++ context-cleanup harness and all
85 current frontend cases (32,168,643 assertions observed). A proof reader's fixed
assertion-total assumption was refused; the existing successful native log was
reviewed without rerunning the suite.

The unchanged normal-main GUI is exercised with each core, with explicit driver
selection and the fake SDK. Both use Buffer, 5 MS/s, 50,000 samples. Baseline
silently retains the 200,000 bytes after failed drain. Candidate raises one
`Capture incomplete` dialog with the cleanup reason and keeps those same bytes
available for saving as a partial capture. Saved payload and samplerate are
checked independently; both original GUI waits return zero. A requested 8,000
sample toolbar selection was not available, so the actual 50,000-sample GUI
geometry is recorded separately from the API/CLI cases.

## Electrical evidence and its limits

No new physical acquisition is claimed. The earlier full WCH477 DLLf244 had
bounded Buffer400MB, finite100MB and later continuous100MB successes, plus a
retained zero-data failure and previous-runtime control. Selected original
receipts are rechecked against their frozen manifests. The new candidate has
the identical generated config.h and CFLAGS. Its fnirsi API object, in both
build copies, is byte-identical after removing debug/unneeded symbols:
`440969798d3630c14fb68e2e014e4fbd86e07a8e2af0844aaaf8cdb4435abd8a`.
This links the existing electrical evidence to the same driver code/data and
relocations; source equality alone is not substituted for a capture.

The old zero-data failure had quiet=1 and did not execute the changed condition.
It remains a failure with unknown cause. The integration is qualified for error
reporting with that electrical evidence reused in its original scope. This is
not new validation of every mode/rate, USB route, firmware or arbitrary-channel
continuity. Limited D0/D22 witnesses do not establish that broader property.

## Installed acceptance and retained failures

All seven fixture cases pass through the actual installed CLI launcher, including
exact bytes and native CMD exit status. With the fake-SDK override removed, the
installed normal-main GUI replays a private copy of a previously verified 200MB
capture. The first ZIP reader overlapped Save; Windows denied a temporary-file
rename and the GUI correctly reported failure. The partial archive and logs are
preserved. A separate private test reproduces rename refusal while Python
ZipFile holds the file and success after its reader closes, supporting observer
interference. No product defect is attributed from that overlap.

One further Save from the same in-memory data completes without a concurrent
reader. After the original GUI/CMD exits return zero, independent ZIP inspection
finds all200MB and every metadata entry identical to the untouched original.
The revised verifier requires those exit receipts before opening the archive;
no acquisition is repeated. The native file dialog's earlier forward-slash path
refusal is retained separately and corrected to a fresh Windows pathname.

Windows settings are restored exactly, temporary interactive tasks are removed,
old packages and both launcher copies remain for rollback, and guest originals
are copied and hash-verified before normal ACPI shutdown. Commercial binaries,
private captures and credentials are not published. No DLA capture, serial
command, reset or flash occurs in this qualification. The previous memory/Demo
activation remains in effect; unexplained transport symptoms remain monitored.
