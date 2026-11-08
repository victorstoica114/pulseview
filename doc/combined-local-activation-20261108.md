# Qualified local activation of Segment completion and Demo Stop fixes

On 8 November 2026, the installed Linux and Windows laboratory launchers were
switched to the complete PulseView GUI containing
[Segment completion compaction](https://github.com/victorstoica114/pulseview/commit/e060ceffb90ce5834410c4826629cdf2bd35aeeb),
with a full libsigrok runtime containing the
[Demo Stop guard](https://github.com/victorstoica114/libsigrok/commit/2c4edd171da43b534eab0c32914c1c6e92601383).
This records a local installation and its acceptance checks, not a new general
binary release or a fix for unresolved DLA transport symptoms.

## Provenance and installed images

The core starts from the exact 477-file source manifest of the previously
qualified full laboratory runtime. Exactly three source files differ: Demo
api.c, protocol.h and driver_all.c, matching the published Demo follow-up.
The physical driver remains unchanged, including the earlier runtime's deliberate
exclusion of three separately pending WCH error-handling lines. The Demo-only
upstream qualification core and diagnostic physical-driver builds were not
installed. The existing qualified C++ wrappers and libsigrokdecode images were
retained. Linux physical-driver object hashes are identical to the baseline.

Installed SHA256 identities:

| Platform | Image | SHA256 |
| --- | --- | --- |
| Linux | normal-main GUI | 3991ef83afc72dcf3fd2d1804ac731082482b7222b043adc12a6077abf59d6c8 |
| Linux | full C core | 6d309a434df80b98e6d7f171aa8fbc5f0bed009f82e45928bb47d4eb59787ebf |
| Linux | C++ wrapper | 20ec72bcb50782fc49e63961823959de286be6696a6b575758a6a9981ea3afad |
| Linux | decoder library | ff037c439927f3a78ec54ea4ac80075afc3e0efb84bf1ff180336980afe8eecb |
| Windows | normal-main GUI | 5fe4e8905489aee1c5e17c545b9aa0e22ba8e0933b638fb4d310ec5fe31dcebb |
| Windows | full C core | 64f89a40bace72fe460ec7071965e2f40cb9d1d877d5d4eb747b85b41f9a16c8 |
| Windows | C++ wrapper | a1fc7442465f95c7deb5692e15e68df656ab110660fbe62edbe1d579dfb94be6 |
| Windows | decoder library | 628767ce4f2f9d19185860a4ee02917a9f069e28bf7ad2fc666d3cbe633de0b3 |

## Acceptance checks

Linux runs inside VMware; Windows runs in the nested KVM guest. Both complete
core builds pass 132 C checks with zero failures/errors and the standalone C++
context-cleanup harness. The fork runtime already includes its separate UCRT
formatting correction. This result does not change the four previously reported
formatting failures of the upstream-based Demo-only qualification.

The exact current frontend suite passes all 85 cases: 35,140,594 assertions on
Linux and 32,161,193 on Windows. With the new full cores, the previously qualified
MainWindow observer completes 20 Demo frames and a separate Stop run retaining
five frames on each OS. HEADER/END counts are 20/20 and 6/6, including the final
empty cancelled attempt. Every incremental payload byte, collection member and
replay matches; all four original native process waits return zero.

The actual normal-main GUI is also launched with explicit no-scan/clean arguments,
first from the shadow runtime and then through each installed launcher. Each
opens a private writable copy of a previously verified 200,000,000-byte logic
capture. Save rewrites that copy; independent ZIP checks compare every entry's
uncompressed bytes and metadata with the unchanged original. Actual loaded GUI,
C core, C++ and decoder image paths/hashes match the installed version. All four
normal-main GUI probes exit zero; the Windows installed CMD process also exits
zero. CLI checks through both shadow and installed paths verify exact 8/32-channel
incremental bytes, healthy exit zero and missing-input exit one on both OSes.

## Refused probes, preservation and limits

The personal Linux desktop was locked and its first XWayland window stayed
unmapped. That software-only probe was closed with its original native wait zero.
Qualification continued on an authenticated private Xvfb server, kept alive until
the GUI exited; the server then exited zero and transient authentication was
removed. A UI helper's geometry query after successful Quit hit the destroyed
window; the original GUI wait, rather than that helper, establishes closure.

The first Windows frontend environment omitted the decoder directory; its
failure is retained. The corrected environment passes. A later proof reader
mistakenly expected Linux's assertion total; the existing successful Windows log
was reviewed without rerunning it. The first normal-main Windows launcher also
passed an empty command line because PowerShell converted a null string argument.
It restored the previous file session, and Save rewrote that historical archive.
The changed container was preserved; all uncompressed entries and metadata match.
The historical SR/PVS files were restored byte-for-byte from verified originals.
The refused run is excluded from acceptance. A fresh run uses an explicit command
line, verified from the live process before UI actions.

Versioned directories and backed-up GUI/CLI launchers retain the former runtimes
for rollback. Windows settings are restored exactly and temporary interactive
tasks removed. All 2,199 Windows original evidence files are copied and verified
before normal ACPI shutdown. No DLA device is assigned to this Windows boot; this
activation makes no physical acquisition, serial command, reset or firmware change.

The existing eight successful physical window cases for the exact Segment source
remain the hardware evidence, with the negative attempts and their limitations
preserved in [the validation record](linux-validation-20261102.md). No new
electrical test is claimed for the Demo-only core delta. This does not establish
arbitrary-channel continuity from limited temporal witnesses, resolve intermittent
transport failures, or turn the observed compaction timings into worst-case CPU
bounds. Those symptoms remain monitored and require new evidence before changes.
