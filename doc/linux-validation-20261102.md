# Linux validation, 2 November 2026

## 7 November 2026 — model OUT timeout before the new control acquisition

A later owner-requested recheck found DLA at a new USB address, 3-2/bus3/dev5 at 5000 Mb/s, with the old device 4 node absent. ESP32 generator was freshly identified at 2-1.2/dev13/ttyUSB0, bcdDevice 8134. The external re-enumerations were observed;  their cause was not established. No hub CYCLE was issued by the agent,  and the CP2102 ESP32 hub controller was still absent from Linux.

A new bounded control plan allowed one finite Stream 5 MS/s, 20 M samples / 80 MB acquisition with zero retries, strict automatic DL32 identification, no warmup/query loop, and fresh witness namespace 942. Twenty-one plan/admission contracts and 444 pinned inputs passed;  executable modes and hashes were checked. The owner/controller/reviewer body was unchanged from the earlier qualified control. The actual outcome is **zero physical acquisitions, one original native attempt, and zero retries**: discovery failed before capture setup or ARM.

The original usbmon scope contains exactly two records, submission/completion of one 512 B bulk OUT to EP02. Its payload is the known nonacquiring model request, SHA ac80c2e3c44f48ab9b195db9492a62fe65174ec45a66cbbc70a4ec6f88f97e78. Completion arrived 1000177 us later with status -2 and zero transferred bytes. The passive reader reports 2 events / 512 retained payload bytes / 0 sample bytes and zero dropped, truncated, queued or failed observer events. The 640 B kernel file has SHA bd40eade565bfb295f3b1bbbdc6865503238ba292bc1da9f3ac2ced962a11233.

There was **no model IN reply, no setup command, no ARM, no HEADER/END and no LOGIC**. Native exit 1 and cleanup_ok=true were retained with Expected exactly one DLA and the trace LIBUSB_ERROR_TIMEOUT. Three fresh ESP32 preflight commands were acknowledged;  no post-ARM witness commands were emitted. All942 IDs remain consumed or reserved. The intended5MS/s setting is not a demonstrated applied hardware configuration.

This is distinct from the earlier 512-zero model replies and from a first-data stall after ARM. The automatic model guard correctly refused to expose a confirmed DLA;  the evidence does not isolate firmware, bridge, USB controller or VMware cause. No further model request or recovery capture was attempted.

The original private no-acquisition closure guard accepted a complete four-event model exchange but refused this closed two-event failed-OUT prefix. Its original refusal and stranded holder evidence remain preserved. A separate private R2 reviewer recognizes only the fully observed empty failed model OUT prefix while retaining the original no-acquisition/ownership/observer checks. **Twenty-six contracts pass**, including the same original scope, older four-event and zero-event cases, and rejection of ARM/data, unknown or successful status without reply, wrong endpoints/ownership, truncation and unread observer events. This is an offline re-review of the same bytes, no new USB operation. R2 was not activated in the original holder and does not qualify a normal live unwind.

After independent actual no-acquisition/request-retirement proof, native absence, Windows off, no DLA USB owner and twice-checked exact holder/parent/argv/executable/fd identities with an open pidfd, only the controlled stranded holder was abandoned. **The original parent Popen wait returned -9;  original tool session 61251 exited 247.** Parent, holder and native are absent and DLA/ESP32 serial owners are free. Missing original run, preARM-closed, END/quiet, monitor-context and source-protocol receipts remain missing; none is fabricated or relabeled as normal close.

The negative attempt and separate offline review are sealed in 64 and 14 evidence files. No acquisition END, Stop/quiet proof, data qualification or DLA recovery is claimed. Linux ran in VMware; Windows KVM remained off. This was private Linux laboratory accounting only, no shared product, installed image, firmware, FPGA, USB request-depth or timeout change, and no private code or raw data upload. ESP32 hub access and an observed hub cycle remain unqualified. AUD027 now also retains this distinct model-OUT-timeout observation; the prior integrity issues and their negative evidence remain open.

## 7 November 2026 — failed limit-case closure and offline first-data review

The interrupted limit-only experiment is now **closed as a failed experiment**, with the original two acquisitions and zero retries unchanged. The 80 MB control passed its declared criteria. The 400 MB phase still has zero accepted LOGIC bytes, native exit1 and no endpoint-quiet proof after its 64 MiB post-Stop drain. Both remaining phases stay canceled. There is no new acquisition, DLA query, drain, hub cycle, firmware change or product fix in this update.

The ESP32 generator was physically disconnected while a USB wiring change was being prepared. Its former USB node disappeared and the old holder's fd3 became a deleted ttyUSB1 descriptor. Native acquisition and the passive reader had already ended, Windows was off and DLA had no USB owner. After checking the original immutable inputs and serial prefixes, the exact controlled holder was checked by PID, startticks, parent, argv, executable and deleted character descriptor, then checked again with a pidfd open. Only this stranded local holder was abandoned through pidfd/SIGKILL. **The original parent Popen wait returned -9; its original tool session exited247.** These abnormal receipts are preserved, not replaced by zero or normal protocol closure.

The original missing run.json, end-confirmed.json, kernel-review.json, monitor-process.json and source-postflight.bin for the failed phase remain missing. Closing the disconnected descriptor does not establish endpoint quietness or qualify any data. The experiment's final136-file evidence set is sealed; the earlier113-file interim snapshot and22 original RAW hashes or serial-prefix checks remain intact. The independently reviewed offline analysis is sealed separately in26 files.

That offline review compares five existing Linux/VMware USB scopes. In every scope, **eight1MiB IN requests were submitted before ARM**. Their full lifetimes must be included; filtering only submissions after ARM would discard these primed requests.

| Existing scope | First positive IN after ARM completion | Active payload including surplus |
| --- | ---: | ---: |
| New50MS/s,20Msamples control | 9399us | 80740352B |
| New50MS/s,100Msamples failure | None | 0B |
| Earlier400MB INFO A1 | 9277us | 400556032B |
| Earlier400MB DEBUG B | 9439us | 400556032B |
| Earlier400MB INFO A2 | 9397us | 400556032B |

In the failed scope, all eight requests remained kernel-visible and outstanding for **3045465us after ARM completion**. They then completed empty/status-2, all before final Stop submission, by533,250,200,141,103,69,35 and9us. The complete ARM-completion-to-Stop-submission interval is3045998us. All positive64MiB belongs to64 new post-Stop reads; it remains diagnostic data, not accepted LOGIC.

Thirty-five original files are pinned to their previous evidence hashes. Two parsers, including an independent fixed-ABI-offset parser, and two independent integrations of request lifetimes agree across all five scopes. Command CRCs, ownership pairing, observer counters and byte conservation agree; the four pre-existing positive kernel reviews also agree on command hashes and active payload totals.

This excludes an absence of submitted kernel-visible IN requests during this particular first-data interval. Outstanding requests do not demonstrate physical bus service, host-controller progress, CPU scheduling or GPIO timing. The three older positive400MB captures are historical context, not newly acquired controls. Firmware, bridge, HCD and VMware causes remain unisolated; neither extending timeouts nor changing request depth is justified by this evidence.

After reconnecting, the ESP32 generator answered one STATUS with its expected v8 configuration and its serial port closed normally. That is a separate readiness observation, not DLA recovery or a new capture qualification. DLA still requires recovery; the new hub/ESP32 arrangement has not been qualified. Linux ran inside VMware, Windows KVM remained off, and no shared product code, installed image or AUD013 candidate was changed or promoted. AUD022, AUD006, AUD026 and AUD027 remain open.

The following interim section retains the state before this actual closure.


## 7 November 2026 — limit-only comparison blocked by first-data stall and nonquiet Stop

A new bounded comparison,after the qualified post-reconnection5MS/s health case below,has reached **two acquisitions with zero retries**. The first50MS/s20Msample80MB control passes. The next phase keeps50MS/s and increases the requested limit to100Msamples400MB;it receives **zero accepted LOGIC bytes**,times out waiting for first data,and cannot prove endpoint quietness after Stop. **Both remaining phases are canceled. The source cohort is still open only for holder cleanup and evidence preservation.** This is a failed,unqualified large-limit case;no product fix or cause is claimed.

The new plan was fixed before I/O:50M20M→50M100M→50M20M→5M20M,max4captures/0retry,fresh reserved source pairs934/935,936/937,938/939,940/941. Its separate prerequisite is the completed5MS/s health case,not a repeated low-rate acquisition. All new phases retain core8fa09dde/helper52163a56/reader5dfefa65,DEBUG4,noWIRE,finiteStream,no trigger,1.6V,1MiB×8IN,source20ms/first5ms/max1100. The UART ceiling is constant within this plan and allows a wrong-rate diagnostic if needed;it is not reached in the normal short high-rate cases. Forty new plan/admission/real-closure contracts pass with444pinned inputs;executable modes and hashes are checked before launch. The automatic model guard remains mandatory;there is no warmup,explicit-connection bypass or repeated query after failure.

| Evidence | 50MS/s,20Msamples | 50MS/s,100Msamples |
| --- | --- | --- |
| Accepted LOGIC | 80000000B,exact original-RAW-prefix→independent transpose→feed→Segment | 0B,saved and delivered |
| Native outcome | Exit0,complete | Exit1,incomplete,error retained |
| D0/D22 on accepted data | D0all49..51/zero anomalies;19fresh consecutive CRC934101..934119,twoSTOPdecoders,44.11904ms all-origin coverage | No accepted samples to qualify |
| Stop and closure | END/retired8IN/twoquiet/sourceheld/observerfinalized,original waits0,normal source close | HEADER1/END1,then a source hold because independent quiet proof fails |

The large-limit trace reports first-data stall at about3.135s. Eight asynchronous IN requests return no positive sample payload during the run. Stop is submitted and completed,then64new synchronous IN reads each return1MiB:67108864B reaches the predeclared drain bound without two empty reads. The terminal error correctly says the endpoint could not be drained after Stop. The helper's cleanup_ok=true concerns handle cleanup;it does not override this acquisition error or establish endpoint quietness.

All158original observed records pair without overlap or missing completions;observer dropped,truncated,queued and failed counters are0. The original Stop/setup/ARM/Stop commands and framing/CRCs are preserved. All positive sample payload is in post-Stop requests;active positive payload is0. The final two reads each still contain1MiB. Original native exit1 and native absence are verified. No extra query,drain,reset or recovery acquisition is used to seek a preferred result.

The original67,108,864post-Stop RAW bytes are preserved. A separately labeled independent transpose yields16,777,216diagnostic samples,with D0periods49..51 and zero deviations at the50MS/s scale;all periods fail the5MS/s interpretation. Two independent UART STOP decoders agree on16fresh CRC messages936101..936116 from the current phase,with no unchecked EOF frame. These are current-input bytes,not the previous934control. **Derived post-Stop data is not accepted capture data.** This case does not reproduce the earlier5MS/s-scale AUD026 signature. Host command write/flush times do not prove when GPIO emitted the messages relative to Stop;device buffering,FPGA,bridge,HCD and VMware causes remain unisolated.

Independent setup decoding confirms mode0,buffer0,rle0,interval3,rate50000000/index8,threshold160/range2 and trigger position0 in both phases. Only sample limit20M→100M and derived duration400→2000ms/index8→10 plus associated CRC/hash change. Setup SHA256 changes from `9b841b23a4587fe088049ae757e47d626e7673e77e26f28053bf970a085de59d` to `edeb63fd705149d7cd87ddfd6815315fe428db9f28d1b5c2f9127f3f6adc147c`;the latter also occurs in historical positive and negative cases. These are host command bytes,not an applied-device configuration ACK. The symptom resembles AUD022 first-data/Stop failures,and this case needs no GUI/View;that does not establish one shared internal cause.

At the frozen snapshot,the native capture child has exited and no process owns the DLA USB node,but the source controller still holds the CH340 serial descriptor in its protective hold. Its original controller Popen wait is pending,source protocol closure and end-confirmed/quiet receipts are absent,and the normal monitor-context cleanup receipt has not been produced. No missing receipt is fabricated from a timer or a manual Boolean. This snapshot is not a closed or qualified cohort.

A new DLA-only reconnection was requested through the private owner channel after this actual block,keeping CH340,wires and hub connected. Confirmation of this new intervention is pending at the snapshot;the earlier confirmed reconnection and low-rate health result remain valid in their own scope. Deferred holder abandonment requires fresh owner confirmation,actual removal of the old USB node,a new device address,and exact process/start-time/parent/argv/executable/source-fd/pidfd checks. Any later forced closure must retain the real abnormal parent wait and remain abandonment,not normal cleanup or data qualification. No automatic next capture follows it.

The source experiment remains open;only a separate113-file evidence snapshot is sealed. It preserves105completed metadata files,22original raw-file hashes and a verified source-serial prefix that may later be extended in the original hold. This report publishes observations and limits only;private captures,binaries and authentication stay private. Linux is in VMware,Windows/nestedKVM remains off,product1248/four repository states and installed images are unchanged. No hub cycle,firmware/FPGA change,product patch or private AUD013compaction promotion is performed. The0B accepted extent and64MiB diagnostic tail cannot qualify the128MiB AUD006 replay test. AUD022/AUD006/AUD026/AUD027 causes and global high-rate qualification remain open.


## 7 November 2026 — one finite Linux health capture passes after DLA reconnection

After the owner confirmed a DLA-only USB reconnection, **one identification-only query and one finite Linux5MS/s health capture pass in their declared scope**. The DLA reappeared as1a86:5537 at3-2,bus3/device4,5000Mb/s,configuration1,with Windows off and USB/source unowned. The CH340 source remained2-1.3,12Mb/s,with its v8 waveform,wires and hub preserved. **This is a changed diagnostic precondition, not a demonstrated device fix.** Historical AUD027 identification refusals and AUD026/AUD006 high-rate failures remain open.

The separate native identification tool sent one512B request and read one512B reply, with no preread,retry,reset or capture setup. Original native and observer exits are0; cleanup is verified. The strict reply contains DL32 and matches the earlier valid reply SHA256 `667f781b187b19a9170a30672ff5607feab64424616637492e699aea2a70a0b1`; request SHA256 is `ac80c2e3c44f48ab9b195db9492a62fe65174ec45a66cbbc70a4ec6f88f97e78`. Exactly4original USB records preserve1024payload bytes,with zero samples,queued/dropped/truncated/error records. The original RealLab.monitor method,actual reader command/readiness and dependencies were verified with private authorization consumed only in memory/stdin.

The frozen R1 identification reviewer initially rejected reuse of the OUT transfer's tag by the later IN. This was a private reviewer defect: the kernel binary ABI defines an ID's submission/callback lifetime,and the tag is normally an URB address. [Linux usbmon documentation](https://docs.kernel.org/usb/usbmon.html) supports this lifetime-based accounting. The original records show completed OUT146µs,then56µs gap,then IN104µs,with no overlapping request. R1's failed collector verdict is preserved. Private R2 removes only the cross-lifetime inequality,retaining exact event order,pair IDs,times,bytes,status,cleanup and exit checks. All29offline contracts pass,including reused-tag positive and overlap negatives. Re-review uses the same original records/native reply; **no identification query is repeated**. This does not identify the cause of the earlier512-zero replies.

The subsequent health plan admits at most one finite5MS/s,20Msample,80MB CAPI acquisition. Its own automatic strict model guard remains enabled,adding one model exchange; there is no explicit-connection bypass. Thus the two stages contain two model exchanges total. It uses unchanged private core8fa09dde,helper52163a56 at DEBUG4,reader5dfefa65,1MiB×8IN,noWIRE,no trigger and1.6V threshold. Fresh source block933,20ms cadence,first delay5ms and maximum240live commands are fixed before acquisition. Source v8 remains D0at1MHz/50%,D22UART,camera1/sd0.

| Finite health evidence | Result |
| --- | --- |
| Accepted logic/feed/Segment | 80000000B,20000000samples,all bytes exact against independent original-RAW transposition |
| Original active RAW retained | 80740352B;740352B surplus preserved |
| Fresh consecutive CRC messages | 197,IDs933101 through933297 |
| D0 witness | All periods4..6samples,zero anomalies;edge distances2/0 |
| UART STOP checks | Two independent decoders agree;unchecked EOF frame is null |
| Same-marker host/GPIO span | 19605978samples/3.921383336host seconds,about4999760.625samples per host second |
| Passive scope | 186records,zero dropped,truncated,queued or failed records |
| Procedure/ownership | Current ARM,actual END/Stop,retired8IN,two quiet reads and finalized observer while source held;original native/controller/observer waits0,then normal source protocol/serial close |

The host span is a scale check,not a calibrated source-emission timestamp or applied-configuration ACK. Requested metadata stays5MS/s. Independent RAW→LOGIC equality covers all32stored bits; electrical witnesses remain limited toD0/D22. The observed IN occupancy is7..8,not a device FIFO,bus-capacity or CPU-cause measurement.80MB cannot test the historical128MiB replay displacement. No accepted samples,surplus or failed observations are silently removed.

A first health preparation was refused before the native CAPI helper started: private binary copies had correct hashes but mode0644,so the observer command was not executable. Its original monitor/controller exits1,zero native processes/queries/acquisitions and empty numbered source-command lists are retained. The serial source was opened for STATUS only and closed;source_protocol_completed remains false,END/quiet remain false. That failed plan is closed and block932 stays reserved. A new directory preserves executable modes with copy2,checks X_OK and hashes before launch,and uses fresh933IDs. It passes21new one-phase admission contracts with444pins. **Only one native health acquisition occurs,with zero physical retries.** Namespace collisions930/931 and other read-only preparation refusals remain preserved.

The identity cohort is sealed as41files,the pre-native preparation refusal as71files,and the successful health cohort as102files. Private raw captures,binaries and authorization are not uploaded. Product1248/four repository states,installed Linux images/launchers and existing sources remain unchanged. Linux runs in VMware; Windows/nestedKVM stays off. This private Linux/POSIX/usbmon diagnostic changes no common product or Windows image;it does not replace a Windows acquisition test. No hub cycle,reset,flash,FPGA or firmware change was performed,and the commercial DLA firmware remains unknown.

The result qualifies this single finite low-rate health case after reconnection. It does not qualify50MS/s,Windows,continuous/manualStop/fault-recovery,arbitrary32-channel electrical integrity or the private AUD013compaction candidate. Further AUD026/AUD006 diagnosis requires a distinct bounded plan with fresh IDs; canceled prior limit phases remain closed.


## 7 November 2026 — AUD013 current decoder and collection software checks pass

The unchanged private compaction candidate now passes the declared **current-source DecodeSignal/UART and file-consumer software matrix on Linux and Windows**. Four native helper runs, baseline and candidate on each OS, exit normally without retries. Each run passes9UART and11consumer cases:36UART cases and44consumer cases in total. **The candidate remains private and unpromoted.** No physical driver is scanned and no hardware acquisition is performed.

The409 current primary product files and existing GUI/Segment objects are hash-pinned to the preceding current-consumer qualification. Only the QA main is newly compiled and relinked; baseline/candidate product difference remains Segment.cpp SHA256 `7116da5202701b294e4e3a2b64ed954db09d5b73eb950535f0682a24fa17520c` for the candidate. Current Session/decoder code, all existing objects and installed images stay unchanged. Linux runs in VMware; Windows runs in nested KVM with one software-only boot and no DLA assignment.

The fixed matrix imports8/16/24/32-bit synthetic binary files at2048 and262144samples, plus32-bit2621441samples crossing the real10MiB boundary. Real DecodeSignal channel assignment/mux and the UART decoder process D7 for8/16-bit inputs and D22 for24/32-bit inputs. The declared source is1MS/s,10kbaud,8data bits/no parity/one stop bit. The helper checks actual SDK binary-output chunks, not only progress counters, and exports those observed bytes separately:280bytes per run,1120bytes total.

An independent NumPy generator reconstructs every source byte from the declared seeds. A separate Python UART decoder finds falling edges and samples bit centers in the actual input; its output and the independently expected bytes both equal every observed SDK byte. All36 UART cases pass these checks. For each source, ordered srzip logic chunks, full binary export and selected range `[3,count-2)` are byte-exact. The real Session also imports/replays each archive and checks sample count, unit size, completion, rate and outcome.

Each run writes and then resaves a collection of three distinct32-bit frames. All eight collection archives preserve exact payloads and metadata:2048/262144/2621441samples,1MS/s,complete status,trigger0.0001 and integer origins `9007199254740993 + i*4000000`, above the exact-integer range of binary64. Independent review checks every nested payload, member size/hash and string-encoded origin. Baseline/candidate source and UART bytes match for all9matrix cases; all9cross-OS payload pairs match. The two same-OS1100×500 offscreen32-channel View image pairs are pixel-identical. ZIP container bytes and pixels across different OSes are not asserted identical.

For the largest case, the UART stack is installed before import and all four runs observe three actual Session input notifications. This verifies the declared preinstalled-stack/multiple-input path. Per-SDK send chronology, arbitrary reader concurrency and electrical/acquisition timing are not proved by those notifications.

The first copy verifier raised KeyError because it expected an older runner's timeout-field name. It failed before writing final guest receipts or archives. Its source/error remain preserved; QA-only R2 reads the actual `deadline_kill` field and still requires original native exit0 and no forced timeout termination. No native test is rerun and no product assertion is relaxed.

All538 original guest file hashes and the157078806B result archive were verified after copying, before normal ACPI shutdown. Windows is now off. Eight installed Windows files, the1248 primary product files/four repository states, Linux images/launchers and all prior/new objects remain intact. Private evidence is sealed as704files under `artifacts/aud013-current-decoders-20261107/`; it and the binaries are not uploaded. The preceding678-file consumer cohort remains unchanged.

There are zero physical acquisitions, source/model USB operations, resets, hub cycles, firmware actions or new source-ID reservations. This closes the bounded current UART/mux/file-consumer software subtask. Full interactive GUI, physical Windows, acquisition copy-cost/50MS/s timing, fault recovery, global OOM and arbitrary concurrency remain unqualified. Historical View/first-data failures and AUD022/AUD006/AUD026/AUD027 stay unchanged. The existing end-series baseline cleanup/memory-lifecycle findings remain valid, and the owner-confirmed physical reconnection gate remains pending.


## 7 November 2026 — AUD013 current Session and consumer software checks pass

The unchanged private compaction candidate now passes the bounded **current-source Session and consumer software checks on Linux and Windows**. Each variant on each OS passes82 frontend regressions, a64-frame Demo series, a between-frames Stop retaining7frames, and Math/A2L/export/replay/real offscreenView at1,1024 and2621441analog samples. **The candidate remains private and unpromoted.** These checks extend the standalone results below; they do not qualify physical acquisition or the full interactive application.

All409 current primary product files match the qualified samplerate-snapshot source manifest. Existing native GUI/test objects are hash-pinned before and after reuse; new native helpers replace main with an isolated fixture and Segment with either the current baseline or unchanged candidate SHA256 `7116da5202701b294e4e3a2b64ed954db09d5b73eb950535f0682a24fa17520c`. Current Session.cpp is SHA256 `0cf6baa211ba6815c8beafd8ad6a7a4412b88f3248b34fcd9bdb0cba5543a225`. No existing object, primary source or installed image is changed. Linux runs in VMware; Windows runs in the nested KVM VM, with one software-only boot for this task and no DLA assignment. Only the Demo driver is scanned; replay uses saved session files.

Both64-frame series retain65536 exact incremental bytes; every Stop retains7168 exact bytes from7completed frames with overall stopped status. C++ checks frame counts, completion/outcome and every byte; an independent Python oracle checks the exported stored bytes again. All four64-frame exports have SHA256 `7daca2095d0438260fa849183dfc67faa459fdf4936e1bc91eec6b281b27e4c2`; all four Stop exports have SHA256 `c7d59719997c773047af17c551fa0ab7ed3f5bbbe1afc85da17aad6d5eef53fa`.

For each analog size and variant, real MathSignal computes `source*2+0.25`, A2L uses the declared threshold, StoreSession writes srzip, and Session replays the saved analog data byte-exactly. Independent NumPy verifies every derived float and threshold byte; ordered archive chunks concatenate exactly to the source export. The largest source is10485764B, crosses the actual10MiB production boundary and is stored in3analog archive chunks. All12 analog cases pass. Baseline/candidate payloads match, all six same-OS render pairs are pixel-identical at1100×600, and all nine cross-OS payload comparisons match. Pixel equality is claimed within each OS; no cross-OS pixel identity or broad interactive GUI qualification is inferred.

The current Session lifecycle bounds the memory benefit:

| Actual64-frame Session measurement | Baseline | Candidate |
| --- | --- | --- |
| Windows private commit before start | 37900288B | 37896192B |
| Windows private commit at direct Stopped signal, before worker cleanup | 719417344B | 46755840B |
| Windows private commit after sampling-thread join | 46682112B | 46686208B |
| Linux virtual-address increase at direct Stopped signal | 747003904B | 75673600B |
| Linux RSS increase at direct Stopped signal | 1478656B | 9400320B |

Current Session emits Stopped, then calls its existing free_unused_memory traversal before returning from the worker; stop_capture joins that worker. **Baseline already releases the reserve after join**, while all64frames remain readable in both variants. The candidate reduces reservation before that end-of-series cleanup. This observation does not demonstrate a persistent post-join leak. Linux RSS rises more for the candidate in this trial, so this case shows no Linux resident-RAM saving. Private commit, VMS and RSS remain distinct; no isolated copy-cost, CPU or physical throughput result is claimed. Synchronous optional allocation/copy of up to10MiB at completion still needs acquisition qualification.

The first Linux runner failed on an incorrect report-format assumption after the native baseline had already exited0 with82/82cases and all assertions passed. R1 source/logs remain failed overall. QA-only R2 checks the complete detailed Boost report and reuses that original baseline result without rerunning it; the other seven Linux processes and all eight Windows processes then exit normally. No product assertion is relaxed. Windows preparation also retains its original PowerShell parse refusal and missing-PATH refusal; correction uses the documented absolute Python path in the verified empty private directory. No existing directory is deleted.

Eight installed Windows files remain identical. All526 original guest file hashes and the25372298B result archive were verified before normal ACPI shutdown; Windows is now off. The1248 primary sources/four repository states, Linux images/launchers, current409-source tree and reused/new objects remain intact. Evidence is sealed as678 files under private `artifacts/aud013-current-consumers-20261107/`; private evidence and binaries are not uploaded.

There are zero physical acquisitions, physical driver scans, source/model USB operations, resets, hub cycles, firmware actions or new source-ID reservations. This task covers declared finite Demo, between-frames Stop and single-analog offscreen consumers. Current decoder-consumer coverage, full interactive GUI, physical Windows,50MS/s acquisition timing, arbitrary concurrency, global OOM and fault recovery remain open. Historical View/first-data failures and AUD022/AUD006/AUD026/AUD027 remain unchanged; no candidate code or image is promoted. The pending owner-confirmed physical reconnection gate is unchanged.


## 7 November 2026 — AUD013 standalone checks pass on current Linux/Windows headers

The unchanged rebased completion-compaction candidate passes the declared **standalone Segment software checks on Linux and Windows** with the current headers. Each platform passes53 allocation/completion contracts and three64-frame storage cases. **AUD013 remains a private, partially qualified candidate.** This work does not qualify Windows acquisition, the full Session/View paths or an installed image; no product code or binary is promoted.

Current Segment.cpp is byte-identical to the earlier rebased baseline, SHA256 `b03fa2599caf1e2e40afeb655fa0483fdd8abb2f74c592a4b653a0c278013aef`. The candidate remains `7116da5202701b294e4e3a2b64ed954db09d5b73eb950535f0682a24fa17520c`: optional compaction at completion, nonthrowing replacement allocation and an unused-capacity guard. The current copied headers include SegmentMutex's MinGW destructor behavior. Older Windows results from a different candidate/header set do not replace this qualification. MOC and native C++17 objects were built separately on both OSes with Qt5.15.19.

The frame fixture retains64 completed segments with1024 used bytes each. Baseline observe-mode preserves all data and reports zero compacted tails; baseline require-trim mode exits1 for that specific unmet criterion. The candidate satisfies it with64 compacted tails. Every case exports65536 actual stored bytes; independent Python checks every byte against frame-specific fixture data. All six exports have SHA256 `ffd2a840627958ab0308401a4b4906c9873e5f0a27d6fa319af3d4993cc6bed9`. Metadata, completion state and frame IDs also match. These are generated class inputs, not physical source samples.

| Measurement at64 retained completed frames | Baseline | Candidate |
| --- | --- | --- |
| Windows private-commit increase from process preallocation point | 672751616B, about641.586MiB | 180224B,176KiB |
| Windows handles, before → retained → after frame destruction | 126 →190 →126 | 126 →190 →126 |
| Linux virtual-address-space increase | 671485952B | 21102592B |
| Linux RSS increase in this process | 454656B | 266240B |

The Windows commit deltas meet the predefined baseline≥512MiB /candidate≤128MiB criterion for this small-frame case. PrivateUsage is private commit, not working set. Linux virtual address space and RSS remain separate observations; this is not a promised application RSS saving. Handle release is measured before the helper process exits. The separately linked memory helpers use the normal allocator, without fault-injection overrides.

The53 fault contracts comprise31 compaction/append allocation/ownership cases and22 completion, two-held-iterator, allocation-failure, retry and idempotence cases. Accepted bytes and sample counts remain intact, notification counts match and iterator pointers remain valid until release. The fault fixture uses32-byte test chunks to exercise deque boundaries cheaply. The64-frame memory fixture uses production10MiB chunks with only1KiB used in each; this plan does not test every production-sized crossing or unit width.

The first Windows R1 run remains failed overall: its three memory/export cases pass and31 allocation cases report zero failures, then the completion fixture exits1. An isolated default-nothrow routing probe observes the injected throwing-array override called once with its fault consumed on Linux, but zero calls with the fault still armed on this Windows runtime. R2 adds only explicit QA nothrow-array allocation/deallocation overloads that delegate to that injected allocator. The completion assertions are unchanged. Both platforms then pass all53 contracts. Only the QA object is rebuilt; the exact original candidate Segment and MOC objects are reused and hash-checked. R1 source, diagnostics and failure receipts are retained, rather than relabeled a passing trial. Supporting allocator-routing observations and test-process override diagnostics do not establish a general runtime allocator defect or real system-wide OOM recovery.

Linux runs in VMware; Windows runs in nested KVM. There was one Windows boot, with no physical USB passthrough/DLA assignment in the recorded QMP states and guest checks. The standalone helpers invoke no Session, device API or sigrok context, and their direct imports contain no sigrok/libusb/WCH capture library. There were **zero physical acquisitions and zero source operations**. Eight original Windows installed files, all1248 primary source hashes/repository states, installed Linux modules and launchers remain unchanged. Windows was closed normally by ACPI and confirmed off; its original native results were copied and verified before shutdown.

Optional completion compaction still performs an allocation and up to a tail chunk's10MiB copy under the existing mutex, or defers it while iterators are held. This software plan does not measure its isolated CPU cost or sustained acquisition throughput. It does not requalify current full Session/GUI/Math/A2L/decoder consumers, arbitrary concurrent readers, or the earlier failed View50M/first-data trials. Earlier consumer and hardware results retain their original source/configuration scope. Physical Windows and full-path qualification are still required before promoting this candidate. AUD013, AUD022 and the rate/continuity/identification issues remain open; see the [current capture observations](windows-desktop-validation-20261104.md). Private immutable evidence is retained in188 files under `aud013-rebased-software-20261107`; only this verified documentation is published.


Windows testing resumed on November 3. See the
[Windows validation and USB handover report](windows-validation-20261103.md)
for the completed checks and the additional failed 50 MS/s continuity test.
The owner-deferred Windows statements below describe the earlier test state.

This fork update adds OR and sampled HIGH/LOW pulse-width conditions to
Stream windows. Width comparisons use whole nanoseconds; `<` and `>` are
strict and Between includes both bounds. A pulse triggers at its closing
sampled edge; an initial partial pulse is ignored. Disabled software
conditions retain the existing trigger sequence. The editor and session
setup preserve the conditions and pretrigger fraction.

It also fixes hidden toolbar controls: Stream windows could be enabled while
Software trigger and Pre remained outside the toolbar layout. Their
QWidgetActions now follow the mode and device/file transitions. The update
includes the previously reviewed startup log-level and decoder ownership/
progress corrections, plus rejection of analog idle compression and safe
edge snapping when an analog signal has no converted logic data.

## Initial fork validation

The initial fork source content passes all 77 frontend regressions with Qt 5.15.19
on Linux in VMware. The new shown-toolbar contract has 114 assertions;
substituting the old MainBar implementation fails 22 of them. It covers
initially enabled/disabled mode, repeated toggles and device/file transitions.

Real DLA-32 USB3 captures at 5 and 50 MS/s exercise the actual Session path:
20 acquisitions, 36 positive windows across HIGH/LOW and the three width
comparisons, plus eight negative OR/AND intervals followed by Stop. The
negative intervals receive data for 1.900–2.101 seconds and retain 19–21
consecutive numbered/CRC-checked UART messages on D22. Independent RAW
transposition matches every stored logic byte; the D0 clock witness is
checked with a one-sample timing tolerance.

The native GUI separately configures OR conditions, captures three frames
of 200000 samples at each rate, saves/restores the setup and reopens both
collections. All six frames match independently transposed RAW on all 32
channels and replay byte-for-byte through the SDK. At Pre50%, the marker is
at sample100000 on the closing HIGH edge, with widths2samples at5MS/s and
25samples at50MS/s. The initial 50MS/s GUI capture uses the installed-form executable
whose SHA256 is `08cae7aae20ab5cf44fc30b7824fddbb3418967b1c30a44bc40c8776f8e2872e`.
The menu launch and saved-collection display also pass on native Wayland.

The runtime uses libsigrok/C++ and the EOF-corrected decoder SDK; installed
library hashes were checked in the running process. This publication changes
frontend source and documentation. It includes no private captures, firmware,
VM disks, vendor runtime or authentication material.

## Limits

The current complete frontend tree is qualified for the Linux cases above.
Subsequent transition stress revealed the intermittent first-data failures
described below; they remain an open qualification limit.
Windows validation of this exact tree remains deferred; earlier Windows
results for individual fixes do not qualify the current combined update.
This is not a claim of arbitrary-channel continuity, every rate/condition
combination, 64 simultaneous conditions, or concurrent decoder throughput.
The short GUI frames supplement rather than replace the longer UART checks.

Measured evaluator cost on a retained physical50MS/s input is129–132million
samples/CPU-second for the positive OR/pulse cases and72–75million for the
two-channel negative cases. These exclude USB and GUI work. One live50MS/s
two-channel OR interval uses1.976CPU seconds over2.337wall seconds including
optional RAW diagnostics; it is not a general throughput guarantee.

The separate high-rate Stream discontinuities, USB handover failure and
experimental segment-memory candidate are not resolved or promoted here.
The `--clean --settings` startup path without an input file was not validated;
the GUI checks use Restore Session Setup explicitly. Hardware sample rate
and depth are configured separately from the saved frontend setup.

## Storage allocation follow-up

Two host storage ownership defects were reproduced with controlled allocation
failures. Compacting the final chunk removed and reinserted its deque entry;
if reinsertion allocated a node and failed, the final stored samples became
unreachable. Growing a chunk could delete the still-owned previous chunk when
the first allocation failed, or leak a newly allocated chunk when deque
publication failed. Both bulk and single-sample append paths are covered.

Compaction now replaces the existing pointer directly. Append paths keep the
new array in a local unique_ptr until the container accepts it, then transfer
ownership. The temporary memory reserve, chunk sizes, padding, sample-count
commit point and compaction timing remain unchanged. This does not enable
the separate experimental early compaction of completed frames.

The final Linux tree passes77frontend regressions and31standalone fault cases.
The original compaction fails at64/128/1024deque entries on this libstdc++;
separate negative controls reproduce deletion of retained storage and leaks
after failed publication in each append path. The test uses small chunks
with normal production append/read methods and verifies every accepted byte,
sample/chunk counts and allocation ownership. Negative controls stop before
reading or destroying known-invalid storage. These are isolated test-process
failures, not induced machine-wide memory exhaustion.

With the final source, real Linux captures at5/50MS/s each retain three event
windows, export and replay. All six frames (66MB total) match independent
all32 RAW transposition, archive payloads and Session replay. Numbered UART
messages agree with a second decoder and remain consecutive in the observed
windows; D0 periods stay within one sample. The exact installed-form GUI also
loads, switches frames, saves and reopens a60MB collection with unchanged
data, origins and trigger metadata. That installed executable has SHA256
`fa9c85c3d99edd9e1e468bc13496bd2bd2bef2c5c675857663e0002942845aab`;
its menu launch is also checked on native Wayland. Explicit `--settings` with
an input file restores the labelled channel as documented by the man page.

Windows for this follow-up remains deferred. Normal hardware captures check
the surrounding storage/export flow; they do not reproduce electrical faults
or actual memory exhaustion. Earlier Stream/USB handover limits remain open.

The optional diagnostic runs in its own process and does not change the
application allocator. From the source root with Qt5 development tools:

```sh
task_tmp=$(mktemp -d)
moc "$PWD/pv/data/segment.hpp" -o "$task_tmp/moc_segment.cpp"
c++ -std=c++17 -O1 -fPIC -I. contrib/check-segment-compaction.cpp \
    pv/data/segment.cpp "$task_tmp/moc_segment.cpp" \
    $(pkg-config --cflags --libs Qt5Widgets) -o "$task_tmp/check-segment"
"$task_tmp/check-segment"
```

## Subsequent Stream startup observations

Further Linux VMware stress at50MS/s used a native XCB Qt View while capturing,
stopped during an active window, repeated Stream windows and returned to a
finite Stream capture. One finite hardware-triggered start produced no RAW
bytes before the test's20second Stop. A control using the published storage
implementation, without private early compaction, then stalled at an
untriggered continuous start after the partial-frame Stop. The driver reported
its first-data timeout after approximately3seconds; all eight transfers retired
with zero bytes and END was delivered. The completed and partial windows before
the failed starts remain preserved and independently checked against RAW.

Both runs used the active Linux libsigrok whose SHA256 begins84c0c7da, with the
same C++ wrapper and decoder SDK. DL32 identification still worked after both
failures. A subsequent published-implementation control retained three event
windows at50MS/s (60MB), with independent RAW conversion, archive/replay and
bounded D0/D22 UART/timing checks, without USB reset, replug or VM handover.

These are intermittent no-data startup observations, not evidence that the
private memory candidate, WCH code or firmware caused them. The triggered
finite observation and untriggered timeout may have different internal causes.
They also differ from the USB handover case in which DL32 identification fails.
The exact cause remains open, and the additional memory candidate remains
private and unqualified. This documentation update changes no product code,
installed executable, USB behavior or firmware. Windows remains deferred.

## Export worker creation failure

Export startup now reports a worker creation failure through the existing
save-error dialog. Previously the std::thread exception escaped StoreSession's
start path, for both ordinary exports and collections. The regular export's
thread construction is now inside its existing exception handler; collection
startup has the same error-return behavior. No exception hook or allocator
change is added to the application.

On Linux VMware, a standalone fixture interposes pthread_create only in its
own process and returns EAGAIN for the selected export worker. The published
implementation fails six startup contracts; the corrected source passes all14
fault/control cases, covering binary, srzip, collection and a nested collection
worker at2048 and2621441samples. In-memory source bytes stay exact, no failed
export emits success, and fresh exports/replays produce identical data.
Independent ZIP/CRC/SHA/payload and sample metadata checks cover all six normal
controls. All77frontend regressions pass. The real XCB StoreProgress dialog
shows the failure in all three export modes, returns normally and permits a
fresh exact export using the same captured Session.

The complete installed-form GUI also saves and reopens a10MiB-plus-four-byte
srzip recording; all ZIP members, data and metadata are identical. Its SHA256
is `89c993267e326db21a6e30c7ce3deea56fc2c3f56565518a57c63d412721a1b8`.
This is an export-startup exception correction. USB acquisition, segment memory,
file formats and normal worker execution are unchanged. Physical capture is
excepted because the change handles host export startup only; no USB access
occurred in these tests. Windows of this exact tree remains deferred. Isolated
EAGAIN injection does not simulate machine-wide thread or memory exhaustion,
and this change adds no promise of atomic replacement for ordinary exports.
The separate first-data/USB/early-compaction limitations above remain open.

The optional Linux diagnostic reuses a completed pulseview-test build made
with the Unix Makefiles generator. It is not linked into the application:

```sh
python3 contrib/check-store-thread.py /path/to/build /path/to/new-evidence-dir
```

The evidence directory must not exist. Build commands, object hashes, outputs
and results are retained there. The program scans no hardware and requires the
same runtime libraries as the frontend build.


## File-open error reporting, 3 November 2026

Opening a missing or malformed file displayed the cause in set_device(), then
load_file() displayed a second generic warning without further information.
The minimal fix retains the existing cause-bearing dialog and early return,
and removes only the duplicate notification. A regression covers missing sr
and collection files, invalid ZIP and malformed collection JSON. Linked against
the retained old Session object, this exact new regression fails with two
dialogs instead of one; all 78 current frontend regressions pass on Linux.

An isolated native XCB MainWindow/Session test passes 25 collection inputs and
23 immediate recoveries in the failed Session. It covers 17 open-time rejections,
six later-frame failures, v1/v2 and sample origins above2^53. Completed earlier
frames and delivered partial data retain their own outcomes; a separately
loaded healthy Session stays byte-exact. Each failure has one dialog, and the
next valid load restores all three frames, triggers and origins. The complete
installed-form GUI also shows one warning and opens the valid three-frame
collection after one acknowledgment. Core/C++/SDK hashes match the installed
qualified Linux libraries. Inputs are synthetic and no hardware is scanned.

This qualifies only the Linux host file-open path (VMware, Qt5/XCB). Windows
execution remains deferred by the owner. Hardware is excepted because this
change only removes a redundant GUI notification; acquisition, transport,
firmware and file parsing are unchanged. No new acquisition integrity or
corrupt-file fuzzing guarantee is claimed. AUD013/022 and the earlier transport
limits remain open.


## Guest-kernel first-data observation, 3 November 2026

A retained baseline helper using the real Session and native XCB View classes
reproduced the untriggered 50 MS/s startup failure with passive guest usbmon.
It uses core SHA-256 84c0c7da14777f27871587dfbda1a0645231d051bc10d51feb089c5769757890,
C++ 20ec72bc and SDK ff037c43; helper b19a9db3 is the original baseline, not the
private early-trim candidate or a newly qualified current GUI acquisition.
Linux is inside VMware, DLA 1a86:5537 at 4-1/bus4/address9, 5000 Mb/s. Windows
remained off; no explicit USB reset, clear-halt, handover or firmware change.

The first all32 / 1.60 V Stream run delivered 95420416 RAW bytes and stopped during
an active window. Its two overlapping frames (20M complete and 13592899 partial
samples) match independent RAW transposition, archived payload/metadata, D0
periods 49–51 samples and consecutive CRC-checked D22 UART through two decoders.
Guest-kernel IN payload matches every accepted RAW byte; a further 1 MiB completed
before the final OUT Stop but was retired with stopping=true in userspace.
It is retained separately in the kernel trace, not silently merged into RAW.

The next Stream-window start used byte-identical setup/ARM commands, all32 and
no hardware trigger. Eight 1 MiB sample IN requests were submitted to the guest
HCD before ARM. No positive completion followed during the approximately 3.044 s
before final Stop submission; the driver first-data watchdog fired. All eight
original reads finished cancelled with zero bytes, before that final Stop OUT.
The Stop OUT completed successfully; new synchronous drain reads then returned
64 MiB with status0 and no empty read. The bounded drain therefore reported cleanup
failure and the fixture stopped. The64MiB value is the existing host drain
bound, not a measurement of device buffer capacity. The planned finite-return
phase did not run.
The failed accepted RAW remains 0 B and all earlier/partial files are retained.

The drain includes three CRC-checked source messages. Their recorded host times,
approximately 102/203/304 ms after host Stop completion, mark completion of serial
write+flush, not exact electrical emission. Both UART readers agree. The preceding
command had already completed after Stop in each case, so the next write could
only start later: causal lower bounds are approximately 0.320/101.544/202.724 ms.
This freshness argument needs no assumed maximum serial write/flush latency.
Monotonic/realtime anchors before and after the run differ by about 21 us; this
does not bound every intervening clock error or assume a kernel timestamp epoch.
These witnesses rule out interpreting all returned drain bytes as an old
pre-Stop static backlog under the recorded clock mapping.
A successful OUT is still not a firmware execution acknowledgment, and the
observation does not locate the fault in firmware, FPGA, USB bridge, guest HCD
or the outer VMware transport.

Offline review of the full retained drain finds 335554 complete D0 periods,
all 49–51 samples. At the nominal 50 MS/s, the first-byte sample intervals between
the three messages differ from the corresponding host completion intervals by
about 1.683 and 7.358 us. These are bounded D0/D22 witnesses, not arbitrary
continuity or an exact sample-zero time. Cancellation, Stop and replacement
synchronous reads occurred together; the run cannot isolate their effects.

All 370 monitor events form 185 closed URB pairs, with zero reported drops,
queued events, truncation or write failure. All eight 2048-byte commands match
the logged prefixes and expected padding; framing/CRC are independently checked.
Disk diagnostics change timing: this is one instrumented reproduction, not a
failure-frequency measurement. Serial opening emitted source POWERON_RESET boot
text with DTR/RTS false, without a reset command; no cause is attributed to that.

No product fix, added delay, drain-bound increase or automatic retry follows
from this evidence. Further captures require recovery from the non-quiet state.
This new failure has a late-data/non-quiet-drain dimension, unlike the earlier
zero-data/quiet-drain observation; they are not asserted to share an internal
cause. AUD013 stays private, AUD022 remains open, and Windows stays deferred.


## 8 November 2026 — observed hub recovery; finite 50MS/s progress stall remains open

One authorized hub cycle was observed in the new setup, with ESP32 generator
(CH340/bcd8134) outside the switched hub and ESP32 hub (CP2102/0001, GPIO23)
independently connected. Linux is still a VMware guest; Windows nested KVM
remained off throughout. DLA firmware version is unknown; no firmware, FPGA,
product code, transfer depth, timeout, wiring or generator waveform was changed.

Two host preparation refusals are retained. The first hub test failed with
`Permission denied` before opening UART or writing CYCLE: its original wait is1
and `cycle_write_attempted=false`. A Linux `uaccess` rule matching only the
CP2102/0001 tty was installed; rule bytes and effective user ACL were checked.
A separate capture preparation then found `/dev/usbmon3` absent after the Linux
reboot. It ended before native run/model query/ARM/numbered witness commands;
original controller/observer waits are1, the generator STATUS-only session
closed, and normal postflight/END/quiet were not fabricated. Loading the
existing `usbmon` module restored the observer character node without USB reset.
The fresh admission checks that node and keeps the earlier refused cohort closed.

The single actual CYCLE write completed with the same ESP32 hub boot, exactly
two reported falling edges and final idle/released GPIO23. DLA disappeared and
returned from bus3/dev3 to dev4 at5000Mb/s. ESP32 generator stayed bus2/dev17 and
ESP32 hub stayed bus2/dev18 in all9 sampled USB states, with no kernel USB event
for either board in the cycle interval. UART did not disconnect; original host
wait is0. Pulse widths and uninterrupted electrical waveform output during the
cycle were not measured. No ambiguous CYCLE was resent.

A NEW finite5MS/s20Msamples/80MB health capture then passed:80,000,000 accepted
bytes are the exact independently transposed prefix of80,740,352 original
kernel RAW bytes, matching feed and Segment;740,352 surplus bytes are retained.
It contains197 fresh consecutive CRC witnesses944101..944297, D0 periods4..6
with zero anomalies, and matching independent STOP decoders. Actual END, Stop,
retired8IN, two quiet reads and finalized observer were confirmed while the
generator UART remained owned; native/controller/observer waits are0 before
normal postflight and serial closure.186 observer records have zero loss,
truncation, queued events or reader errors. This qualifies the stated finite
5M health following the observed cycle, without claiming an acquisition fix.

The next NEW bounded comparison kept50MS/s constant while changing only the
sample limit20M→100M→20M; its final5M health was preplanned. All high phases
use the same DEBUG helper52163a56, core8fa09dde, passive reader5dfefa65,
1MiB×8 reads, no WIRE,1.6V/no trigger, and generator20ms/first5ms/max1100.
The same frozen controller/source-owner/RAW-review bodies are used.40 admission
and closure contracts passed before I/O;444 source/runtime/plan pins and
executable copy modes were checked. No extra model warmup or connection bypass
was used; the strict automatic DL32 guard remains mandatory.

| Actual phase | Requested | Accepted | Fresh CRC witnesses | Native/original controller wait | Result |
| --- | --- | --- | --- | --- | --- |
| First50M |20Msamples/80MB |80,000,000B |19,945101..945119 |0/0 |Qualified in this scope |
| Large50M |100Msamples/400MB |83,886,080B (80MiB) |20,947101..947120 |1/1 |Incomplete, unqualified |
| Final5M |20Msamples/80MB |80,000,000B |197,951101..951297 |0/0 |Qualified in this scope |

There were3 actual captures and zero retries in this comparison. The dependent
second50M20M phase was permanently canceled after the large failure. All actual
phases have real END/Stop/retired8IN/twoquiet/source-held proof, observer wait0
and normal protocol/serial closure. There was no forced abandonment or second
hub cycle. All945..952 witness blocks remain reserved, including canceled IDs.
The preceding separate5M health makes4 physical acquisitions in this work,
with zero physical retries; the refused preparation adds zero acquisitions.

The large capture correctly reports `Capture incomplete: sample data did not
arrive within the transfer timeout.` Its original194-record kernel scope has
80 complete1MiB positive sample completions. The first arrived9360us after
ARM completion and the last at474951us. After this progress, all8 submitted
host IN requests stayed pending for3,044,757us without sample completions.
Stop was submitted3,049,398us after the last positive completion. All requests
retired, with zero positive post-Stop bytes and two quiet reads. Two independent
binary parsers agree on the original event framing and order; no new I/O was
used for this review.

Every saved prefix matches original kernel RAW→independent transpose→feed→
Segment exactly. The failed large prefix has D0 periods49..51/zero interior
period anomalies and20 fresh ordered CRC messages, with matching STOP decoders;
its partial D0 edges40/18 and unchecked EOF frame are retained. It remains
unqualified for the requested400MB. Its witnesses support the requested50M
scale rather than the earlier diagnostic5M-scale negative. It also differs
from the prior0B-before-first-data case with64MiB after Stop and no quietness.
These distinct failures are preserved; a common internal cause is not shown.
The partial80MiB scope cannot test128MiB replay, and the later positive5M
control does not validate or repair the failed high-rate capture.

AUD028 records this progress-then-stall observation separately. AUD006,
AUD022, AUD026 and AUD027 remain open; no firmware/bridge/HCD/VMware cause,
timeout/depth workaround or global integrity claim follows. D0/D22 are limited
electrical witnesses, while all32 storage conversion is checked independently.
Only finite Linux routes in the stated sizes are covered; Windows, continuous
capture, manual Stop, arbitrary inputs and general fault recovery are not
qualified by this work. No shared product source or Windows image changed;
1248 primary source/HEAD/status/diff and installed Linux/launcher checks passed.
Offline Windows images were not re-read.

Evidence is closed in `hub-recovery-20261108` (34 files), refused preparation
`hub-recovered-health-20261108` (53), qualified low health
`hub-recovered-health-ready-20261108` (86), and negative comparison
`limit-only-after-hub-20261108` (196). Original errors, waits, partial data,
surplus and EOF records remain preserved. Only this documentation is published;
private raw traces, firmware, credentials, binaries and host configuration are
not uploaded. Closed cohorts and canceled phases must not be rerun.


## 8 November 2026 — offline review of the final LOGIC/storage callback

The preceding AUD028 failure remains incomplete at80MiB; no capture was
repeated. This follow-up adds zero hardware I/O, native executions, VM boots,
model queries, serial commands or witness IDs. It reviews the current negative,
the current small positive and a historical400MB DEBUG positive on their
original bytes.41 direct input pins,477 compiled core source hashes, actual
loaded core8fa/helper5216 and the linked storage/Segment/moc objects are checked.
Two independent parsers agree on the original USB event framing and order.

The negative has80 positive1MiB sample completions and80 synchronous DEBUG
LOGIC deliveries totaling83,886,080B. There are eight sample reads actually
outstanding across ARM and80 subsequent sample submissions before Stop. A
separate, already-completed empty EP81 read before ARM is excluded from the
primed set. The final positive completion is record178; a new sample submission
at record179 follows3673us later. All8 IN requests are then pending for
3,044,757us without a completion.

The compiled route is `decode_transfer` → `sr_session_send` → the synchronous
helper LOGIC callback → `QFile.write` and `Segment::append_samples` → return →
`libusb_submit_transfer`. The final synchronous LOGIC/storage callback had
therefore returned before that new submission. This excludes a still-blocked
last storage append as the immediate state during the observed stall. It does
not assert that the enclosing libusb callback or submission ioctl had already
returned at the USB submission timestamp. Earlier I/O/CPU pressure and
subsequent kernel/HCD/VMware/device/bridge behavior remain unisolated. A returned
`QFile.write` is not proof of disk durability or absence of I/O pressure. No
storage option, cache, priority, timeout or queue-depth workaround is justified.

The host ESP32 generator writer records178 numbered write/flush operations
over3.537556904s, with a maximum start gap46.630073ms and maximum write-through-
flush duration0.756387ms. These show host writer liveness only; the retained
ACKs have no per-packet reception timestamps, and the numbers are not GPIO
emission timestamps or clock calibration. They do not locate the USB cause.

The historical DEBUG record completed400,000,000B with the exact same
helper/core/storage objects. It is a counterexample to an unconditional80MiB
software cap, not a new paired causal control. Its generator command ceiling300
differs from the current1100; history and physical setup differ as well. No
qualification of the new negative or general high-rate integrity is transferred.

R1 of this offline reviewer counted the completed pre-ARM empty read as a ninth
primed request and refused. Its exact source, exit1 and stderr are retained. R2
requires an initial read still be outstanding across current ARM; data, order
and request ownership criteria are unchanged.14 contracts cover legal sequential
request-tag reuse, live reuse, unowned terminals, changed identity/count/clock/
order, unretired requests, the real pre-ARM empty read and a missing final
resubmission. The completed small record correctly receives no last-resubmission
proof. No original trace, capture or failed verdict was rewritten.

The10-file `progress-stall-callback-review-20261108` evidence is closed. This is
an offline audit of Linux sources and evidence: no shared source, image or
installed product changed, and no new Windows hardware qualification is claimed.
AUD028 and the earlier acquisition failures remain open. The next investigation
needs a new bounded transport/VM comparison; a storage rewrite or repeated
unchanged capture would not isolate the remaining cause. Only this documentation
is published, without raw data, private binaries, credentials or firmware.
