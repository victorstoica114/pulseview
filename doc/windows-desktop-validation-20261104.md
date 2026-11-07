# Windows desktop capture and replay observations — 4 November 2026

## 7 November 2026 — matched CAPI INFO/DEBUG/INFO at50M passed; AUD006 not reproduced

A new fixed CAPI comparison passed at **50MS/s,100Msamples/400MB** with INFO, DEBUG and INFO again. The three records contain99,100 and99 fresh consecutive CRC messages, zero D0 anomalies and exact independent original RAW→true feed→Segment equality. **AUD006 remains open.** This lot did not demonstrate a logging dependency, a root cause or a product fix; the earlier negative captures remain negative.

Exactly5 physical acquisitions ran sequentially, with zero retries/GUI/Xvfb/Windows boots: INFO5M20Msamples80MB health → INFO50M100M400MB A1 → DEBUG same geometry B → INFO same geometry A2 → INFO5M80MB return. All used the same private core `8fa09ddef3b43d8c4c014c4852a94bfafbbe0d64085a67f701ba36638fb2cce7`,1MiB8IN, finite Stream/all32/unitsize4/no trigger/1.6V, identical actual per-packet feed writes, storage/Segment objects and event-loop code. Linux7.2.9-1-MANJARO runs in VMware; Windows/KVM stayed off. DLA1a86:5537 remained3-2/bus3/device3 at5000Mb/s; CH340bcd8134/source-v8 remained2-1.3/ttyUSB1 at12Mb/s,1MHz D0 and fresh D22 UART. Wires, source, hub and firmware were preserved: no reset, flash or power cycle; DLA firmware remained unknown/unqueried.

Both fixed-level helpers were rebuilt with the same current GCC16.2.1, headers, flags, linker inputs and unchanged original storage/Segment/moc objects. Their complete ELF files differ only in the GNU build-ID and one executable byte at offset16515:3(INFO) versus4(DEBUG). Executable layout, other code, read-only/data and dynamic-link bytes are identical. Capture-source difference is only fixed `SR_LOG_INFO` versus `SR_LOG_DBG`; no generic product switch was added. INFO helper SHA256 is `6549b73c402219298d9bc92f56bbcd9b60eef1a42f2cf285d30a4f3a2bb05439`; DEBUG is `52163a564d0ded6302cd6d5a9fb693c9a169116916831ffa382bd776382afe33`. Both passed independent20MiB+12byte arithmetic-oracle feed/Segment self-tests before hardware I/O.

The predeclared criteria were D0nominal5/50±1samples and edge gaps≤6/51, two STOP-checking UART decoders agreeing,40±1 fresh consecutive CRCs for low controls and99±3 for high phases, plus all allowed origins of the known44.11904ms window for high phases. High source cadence0.02s/firstdelay0.005s/max300 was unchanged; observed first writes were7.941603/7.910643/7.874892ms after current kernel ARM observation. These are host timestamps, not device-ready ACKs or physical generation times. Ninety-one software contracts, the ELF pair check and both self-tests preceded first I/O;147 source/binary/configuration pins remained unchanged.

| Phase | Accepted bytes | Fresh consecutive CRC messages | D0 periods / edge gaps, samples | Kernel receipt window / pending IN |
| --- | ---: | --- | --- | --- |
| INFO5M health | 80000000 | 40:974101–974140 | 4..6 /3,0 | 4040260us /7..8 |
| INFO50M A1 | 400000000 | 99:975101–975199 | 49..51 /7,13 | 2254868us /6..8 |
| DEBUG50M B | 400000000 | 100:976101–976200 | 49..51 /32,39 | 2255336us /7..8 |
| INFO50M A2 | 400000000 | 99:977101–977199 | 49..51 /4,18 | 2255535us /6..8 |
| INFO5M return | 80000000 | 40:978101–978140 | 4..6 /3,0 | 4040246us /7..8 |

All five passed their original procedure and signal criteria. Every high record covers all97794049 allowed origins of the44.11904ms window. A1 retains an incomplete EOF start at sample99998233: expected Stop100002355/356 lies beyond100000000samples, with no complete SDK DATA byte. It remains unverified; no original waveform samples were removed or repaired. A2 and B have no unchecked EOF frame. Electrical witnesses remain limited to D0/D22; all32 digital storage/conversion equality does not establish arbitrary32-channel electrical continuity.

Each high original kernel file has796 records and382 positive1MiB IN receipts,400556032 active RAW bytes; the556032B surplus is retained beyond the accepted400MB. Each low record retains80740352 active RAW bytes, including740352B surplus, with186 kernel records. All positive original kernel bytes were conserved, without a native WIRE trace. The DEBUG phase actually logged382 SR_DF_LOGIC packets in36325B of stderr; high INFO logged no DEBUG LOGIC packets and347B each.

| High accepted LOGIC payload | SHA256 |
| --- | --- |
| INFO A1 | `fea99e6aebb295da91fcbeac19d035daf8a4bc891b45067367f6f5d4c6b401b4` |
| DEBUG B | `138501465e3e7ccb3d467f01c95cb2107c5b1eac8674e3516b25b4fe3255f020` |
| INFO A2 | `c96debdbcca23dc7edbacd997d33755ffddab2b55764983c9f6c720390a47099` |

Occupancy was computed offline in original event order, from current ARM completion through the completion containing the final accepted RAW byte. High duration histograms were A1:6IN1454us/7IN509091us/8IN1744323us; B:7IN519529us/8IN1735807us; A2:6IN4270us/7IN540363us/8IN1710902us. All began with8 and ended this receipt window with7 pending requests, subsequently retired before source close. There was no0/1 duration or zero-IN interval in these windows. This measures host kernel request ownership, not bus activity, generation, CPU or FIFO occupancy.

Every phase retained actual original native callback HEADER/END/result and process exit0, initial Stop/setup/ARM/final Stop, all retired8IN requests, two sequential quiet reads and monitor original wait0 while source serial stayed owned. Only then did source close; all five original controller Popen waits were0. INFO used current native INFO HEADER/END plus callback counters without inventing a DEBUG packet sum; B additionally used its actual DEBUG packet sum/order. Data integrity was reviewed separately. A demonstrably classified replay would have allowed the comparison to continue without qualifying corrupted data; an inconclusive outcome would have stopped forward phases, with the reserved low return still requiring real closure. No retry or manual recovery Boolean was used.

Private originals, partials, source versions and preparatory refusals are sealed in `capi-logging-ABA-20261107` (307 files). The acquisition/source/hold/END body and independent data reviewer were reused byte-for-byte from the previous frozen lot; only fixed order, original receipt checks and helper selection changed. The actual entry test refused missing qualification before device I/O. A self-test invocation initially refused a pre-existing output directory; the corrected invocation used a new output as required, before any USB enumeration. Neither refusal caused a physical acquisition or a hardware retry.

This is a bounded private Linux/POSIX LAB result, with no shared product/Windows code change or image promotion; the Windows exception is explicit. Fresh checks found USB/serial resources free, Windows off and product1248/HEAD/status/diffs/new-file hashes unchanged after return. It does not qualify global50MS/s integrity, the installed product, continuous acquisition, manual Stop, fault recovery or seat0. The next step is to compare the retained negative and passing receive timelines, configuration and source history offline before proposing another targeted test or changing queue/scheduling behavior. No additional acquisitions are reserved in this closed lot.

## 7 November 2026 — one finite Linux GUI50M INFO3 record passed; AUD006 not reproduced

A new, predeclared native Linux GUI INFO3 capture at **50MS/s, 100Msamples/400MB, all32/unitsize4, finite Stream/no trigger/1.6V** passed its original RAW/archive, D0/D22 and dense temporal-witness contract. The known loss/replay fault did not reproduce in this record. This qualifies this bounded private route and record, not global50MS/s integrity, the installed product, continuous acquisition, manual Stop, fault recovery or an AUD006 fix. Earlier negative captures remain negative; AUD006 remains open.

Exactly3 physical acquisitions ran sequentially, with zero retries, one GUI/private Xvfb and no Windows boot: private CAPI INFO3 at5M20Msamples80MB → GUI INFO3 at50M100M400MB → the same CAPI5M80MB return. Both controls passed independent original RAW/actual feed/Segment equality,40 fresh consecutive CRCs (971101–971140 and973101–973140) and zero D0 anomalies. The low-rate controls establish health; they are not a workload-matched high-rate frontend comparison. Linux runs in VMware, Windows/KVM stayed off, DLA1a86:5537 remained3-2/bus3/device3 at5000Mb/s and CH340bcd8134/source-v8 remained2-1.3/ttyUSB1 at12Mb/s. Source, wires, firmware and hub were preserved, with no flash/reset/power cycle. DLA firmware remained unknown/unqueried.

The private CAPI helper is SHA256 `985e4df02cdd9e69b817c763317f6d1f700e1458120abcbd6f887b05bb19be27`. Its only capture-source change from the existing helper is fixed `SR_LOG_DBG` → `SR_LOG_INFO`; the original storage/Segment/moc objects are unchanged. An independent arithmetic oracle checked both20MiB+12byte self-test files. There were211 software contracts before first I/O. The new INFO3 completion guard joins actual callback HEADER/END/result, original process exit and independent kernel quietness without fabricating a DEBUG LOGIC sum. Both physical CAPI controls then verified that helper at INFO3. CAPI still writes an extra feed file; no causal frontend/logging conclusion follows from comparison with the GUI.

Before the single GUI Run,3 additional software contracts checked one native press/release/sync without the generic UI helper's150ms post-click sleep. That sleep is useful for dialogs but would delay dense source scheduling. The actual first source write was6.561272ms after observed current kernel ARM. This is a host observation, not a device-ready ACK or physical generation timestamp. Source cadence0.02s/firstdelay0.005s/max300,99±3 CRCs, D0nominal50±1/edges≤51 and known44.11904ms origin coverage were fixed before target data. The UI visibly selected100M/50MHz, Stream, finite/all32/no trigger/1.6V. Historical Linux and Windows50M initial Stop/setup/ARM hashes agree and match the current original commands; these are configuration pins, not fresh high-rate reference data.

GUI PID108444/start6717198 used the existing private GUIe4414dad/core8fa09dde/CXX71ef67fa/decoderff037c43. Original PID/start/module/file identities, current stderr prefix/token and GUI liveness were bound/rechecked before GO and at native complete END. Prefix224B +current347B/following0B +later replay/shutdown510B =full1081B conserve exactly. INFO3 has no per-packet FLOW and no native GUI feed/RAW trace claim. Independent current ARM/Stop, retired8IN1MiB requests and two sequential quiet reads were finalized by original monitor wait0 while serial stayed owned; source close followed the joint proof. Original GUI/supervisor/controller/private-server waits were0, with original GUI/server pidfds retained. Empty-authority access was refused, TCP disabled and private auth removed only after original server wait. Personal seat0 remained locked/unqualified.

The original kernel file has792 records and382 positive1MiB receipts:400556032B active RAW. Both native archives have valid full ZIP CRCs,96 chunks and400000000 accepted bytes. Save/Open/SaveAs was completed natively and visibly before native close. All400MB, including the final partial UART waveform, equal an independent bit-plane transpose of the original accepted kernel prefix in both archives. The556032B surplus is retained separately, not appended to accepted data. Metadata and payloads remain equal after close; container hashes differ.

| Evidence | SHA256 |
| --- | --- |
| Accepted payload in both archives | `5429727182cd6d1464e6d5ce1a9afacc1432cb11dde4febbc6cfbf6c299c673f` |
| Metadata in both archives | `d2fe222438f3b4d618a8be9a9ea9ce23020737d6e050c52fc48cadda7a1b2d2f` |
| Active original kernel RAW | `ad094dd1dd9ce0ba9eee8c888c10b30520676b40a3c9a7c5ee16922ed9502afa` |
| First ZIP | `7b25cfb72e799ae445e22a63951326f5ec30f46dea891b5bcc940a181a8c9468` |
| Roundtrip ZIP | `e72478ba4bd06f5f6302b4b9bbb287a04e7a8e1accbff205a6dd0f2f9012d991` |

D0 had zero anomalies, observed periods49..51 and edge gaps49/32 samples. Independent and SDK STOP-checking decoders agree on99 fresh consecutive CRC messages972101–972199. All97794049 allowed origins of the known44.11904ms window are covered. A partial SDK EOF frame beginning at sample99996293 is retained: its expected Stop at100000415/416 lies beyond the100000000sample record. Its DATA annotation0x30 is unverified and excluded only from STOP-checked protocol bytes; no original samples were removed or repaired. The99 complete CRC messages and exact storage conversion do not establish arbitrary electrical continuity on the other30 channels.

Offline kernel occupancy uses original event order from current ARM completion through the completion containing the final accepted RAW byte. The2255277us receipt window began with8 requests and stayed at5..8, with no0/1 duration or zero interval:

| Pending IN | Duration, us |
| ---: | ---: |
| 5 | 760 |
| 6 | 16002 |
| 7 | 520851 |
| 8 | 1717664 |

Eleven offline contracts with1000 duration-oracle domains and4096 frame brackets check that accounting. These are host kernel request durations, not physical bus use, CPU or FIFO measurements. The contrast with the earlier negative's85.0472% at0/1 is consistent with the still-open depletion hypothesis; different frontend, feed I/O, logging and history prevent causal attribution. Neither a queue-size change nor a logging/FIFO fix is justified by this positive record. The next useful experiment is an explicitly matched CAPI INFO3/DEBUG comparison with identical core, storage/feed, geometry, source witnesses and closure gates.

The first data reviewer refused with a missing `witness` alias after retaining its archive/kernel/D0 results. That failed source/output remains intact. R2 changed only the alias and separate output paths, then reread the same original capture under unchanged criteria; no acquisition was repeated. Private originals, source versions, partial EOF and unused RAW are sealed in `linux-GUI-INFO3-50M-20261107` (454 files). Earlier408/624-file cohorts remain closed. Product1248/HEAD/status/diffs/new-file hashes and fresh device/source/VM ownership checks passed after the final return. This is private Linux/POSIX LAB work with no shared product/Windows change, runtime promotion or published private capture/authentication material.

## 7 November 2026 — finite Linux GUI INFO3 qualified on a private Xvfb display

The missing **finite LinuxDLA native GUI INFO3 route passed at 5MS/s, 20Msamples/80MB, all32 channels, unitsize4** on an explicitly owned private Xvfb display. Current native HEADER/complete END, independent kernel completion/quietness, source ownership, native Save/Open/SaveAs, exact original RAW-to-archive conversion and the predeclared D0/D22 witnesses all passed. This qualifies that bounded normal acquisition/storage route. It does not resolve AUD006 or qualify global50MS/s integrity, continuous acquisition, manual Stop, electrical fault recovery, the personal desktop or arbitrary32-channel electrical continuity.

An earlier declared25Msample plan was refused **before BIND/GO/source/monitor/GUI Run**: the native sample-count selector uses a1-2-5 list, not arbitrary typed counts. Typing25 M selected200k;20M was subsequently confirmed visually. Its GUI configuration gate expired without an approved configuration. The original GUI closed with wait0, supervisor/controller1 recorded the pre-Run refusal, and the owned Xvfb closed with wait0/auth removed. Its installed CAPI25M100MB reference and reserved return both passed independent RAW/LOGIC/feed equality,50 fresh consecutive CRCs and zero D0 anomalies. The failed plan, partial logs and true exit evidence remain preserved; it was not relabeled successful or relaunched. Incremental is a Demo Pattern option and was removed from mistakenly copied LAB metadata before any target Run; it is not an external DLA source criterion.

A **separate20M80MB plan** was fixed before data, with at most3 physical acquisitions, zero retries, one GUI/server and no Windows boot. The40±1 CRC criterion follows the declared four-second capture and unchanged0.1s source cadence; it was not chosen after seeing target data. The three sequential phases were:

| Phase | Runtime | Accepted bytes | Fresh consecutive CRCs | D0 anomalies |
| --- | --- | ---: | --- | ---: |
| CAPI+Qt Segment reference | installed Linux core84c0c7da/helperdb7c255b | 80000000 | 963101–963140 (40) | 0 |
| Native Linux GUI INFO3 | private core8fa09dde/CXX71ef67fa, GUIe4414dad | 80000000 | 964101–964140 (40) | 0 |
| CAPI+Qt Segment return | installed Linux core84c0c7da/helperdb7c255b | 80000000 | 965101–965140 (40) | 0 |

Linux runs in VMware. DLA1a86:5537 remained on3-2, bus3/device3, negotiated5000Mb/s; the source was CH340bcd8134 on2-1.3/ttyUSB1 at12Mb/s, firmware-v8 status verified before and after. Stream/finite/all32/no trigger/1.6V and20M at5MHz were checked in the actual native UI. The GUI's original initial Stop/setup/ARM command hashes exactly equal the fresh CAPI reference. Source, wires, hub and firmware were preserved; DLA firmware remained unknown/unqueried, with no flash, FPGA change, hub reset or power cycle. Windows stayed off; it was not a simultaneous owner or capture source.

The original GUI was PID106044/start6528832. Its executable and three mapped modules/file identities, original Popen/pidfd, bound stderr inode/prefix and current run token were checked before GO and again at native END while the GUI remained alive. The closed stderr conserves224B excluded prefix +344B current +0B following +509B later replay/shutdown =1077B. INFO3 has native HEADER/END; no per-packet FLOW or GUI feed trace was invented. Native count alone was not treated as stored data or continuity proof.

The passive original kernel file contains182 records and77 positive1MiB IN receipts:80740352B active RAW, actual maximum depth8. Initial Stop/setup/current ARM/final Stop, every retired request and two sequential post-Stop quiet reads were independently checked. The live complete S/C command records join the finalized original file, native token, PID/start and current source owner. The original monitor wait0/finalized accounting occurred while serial was still held. Only that combined proof authorized serial close; timeout, signal, observer exit or a recovery Boolean did not. Actual normal ordering is native complete END → current kernel/source permission → serial close → later GUI close. Software exception/SIGTERM/timeout tests remain software tests, not physical fault-recovery qualification.

Both native archives have valid full ZIP CRCs,20 logic chunks and80000000 accepted bytes. Save finished visibly before Open; SaveAs finished visibly before native CtrlQ. Both payloads and metadata are exactly equal after close, and all80000000 bytes equal an independent bit-plane transpose of the accepted original kernel RAW prefix. All positive pre/active/post bytes remain retained, including740352B active surplus beyond the accepted prefix; surplus was not appended to accepted data. There is no separate native GUI feed/RAW trace claim.

* Payload SHA256: `d0dd338167825ee1cbfbedfdd5bed3f8b042c6a6b6897fcb0a2e759ccccede1b`.
* Metadata SHA256: `3008fb05a309b5cb46a509eded39532a253e0659bc13fd9b43d4be6bfa508537`.
* Active original RAW SHA256: `89385390b23763ad031ec19dbe8141d5d162fba0f7cf038077baa828bebd5ba9`.
* First ZIP SHA256: `1f11619840382d8ada182bbafb70db0c5c2ee5a8c418358ac36e1b8e8ba5651b`; roundtrip ZIP: `0c9fc13f1b7434741639cafc4c59fffb4b848d498a33d1fafb8598df89344e53`. Container hashes differ; payload/metadata equality is the claim.

D0's declared nominal5±1sample period rule had zero anomalies, observed4..6; GUI edge gaps were5/1 samples, within the6sample bound. Independent and SDK STOP-checking UART decoders agreed on the40 fresh consecutive D22 CRC messages, with no unchecked SDK EOF frame. These temporal/ordering witnesses supplement exact storage conversion and do not prove arbitrary activity on the other30 channels. Each control separately passed its original RAW/LOGIC/feed and same witness contract; CRC count, USB silence or native byte count alone is not the argument.

Original GUI/supervisor/controller/private-server waits were all0. GUI/server original pidfds remained retained; the top-level controller was waited by its original unified exec session, not mislabeled as another Python Popen handle. The authenticated private server stayed alive through original GUI wait, then closed normally. Empty-authority access was actually refused, TCP was disabled and the private cookie was removed after original server wait. Personal seat0 stayed locked/unqualified. No password/cookie or private capture was published.

The20M cohort had180 software component cases before hardware and exactly3 physical captures/one GUI Run. Across both explicitly separated cohorts there were5 physical captures,2 GUI processes/2 private servers and only1 GUI Run, with no hidden capture retry. Original private evidence is sealed locally in `linux-GUI-INFO3-physical-preparation-20261107` (624 files, earlier refusal) and `linux-GUI-INFO3-20M-20261107` (408 files, qualified bounded route). Source/runtime/input hashes and failures are retained. Product1248 source/HEAD/status/diff/installed-Linux/launcher checks passed after the final return, with VM/device/source ownership clear. This is Linux/POSIX LAB qualification with no shared product or Windows image change and no runtime promotion. The earlier Windows finite INFO3 result retains its own scope; AUD006 remains open.

## 7 November 2026 — private Xvfb Demo lifecycle verified; physical Linux INFO3 still open

One new finite native Demo GUI process passed the declared private-Xvfb software route: current process/module/log binding, native END, complete Save/Open/SaveAs payload and metadata equality, and original process waits. There were zero physical captures, source commands, DLA assignments, Windows boots or product changes. Linux runs inside VMware. The personal Wayland seat0 remained locked; this result does not qualify that desktop, physical Linux GUI INFO3, Windows, electrical continuity or a global sample rate. AUD006 remains open.

A private [Xvfb virtual framebuffer server](https://www.x.org/docs/man/man.pdf) supplied the test display. The exact distribution package xorg-server-xvfb21.1.24-1 was privately extracted, with repository SHA256 and the original package signature verified under the installed Manjaro public signing key. Nothing was installed into the system. The first mirror connection refusal and initial gpgv refusal of the ASCII-armored public keyring remain preserved; the existing public keyring was dearmored locally and signature verification then passed. No signature check was disabled or binary run before that verification. The package executable is SHA5bfd315a; dependencies match the installed xorg-server-common21.1.24-1.

The new display had its own cookie file0600 and no TCP listener. A client using an empty authority file was actually refused with exit3. The cookie was never placed in argv/logs or retained in the sealed evidence; it was removed only after the original server wait. Eight software display-closure contracts refuse a timer, mismatched process or secondary exit observation as a replacement for the original GUI wait. The actual private server's original Popen/pidfd stayed owned and alive through native GUI exit, then closed with original wait0. The server was not used to disconnect a live GUI. The former failed seat0 Demo and its missing original wait remain refused; the separate staged seat0 launcher remains unused. This is a new, explicitly changed display route, not a hidden retry or a spoofed unlocked session.

The unchanged native GUIe441 loaded private core8fa/CXX71ef and existing decoderff03. Copies of the native observer/controller/X11 helpers are byte-identical to frozen R3; the existing77 observer contracts,15 real software-child cases and one X11 BadMatch negative are reused by SHA, not rerun or presented as new GUI tests. LinuxDemo remains distinct from LinuxDLA and cannot grant physical source release. The GUI's original PID102210/start6087279, parent/session/UID/boot, exact image/argv and all three actual mapped module paths/disk identities/hashes were bound before GO and rechecked at END. No DLA or serial descriptor was present.

The actual UI selected Incremental with all32 channels enabled, finite1MHz/1Msamples/unitsize4. One native Run followed current OBSERVING. Exactly one HEADER,982 LOGIC packets totaling4000000B, END and Stopped were received in order while the original GUI was alive. Existing prefix4561B is excluded; current87965B and following0B are conserved exactly. Full closed stderr retains1987 later replay/shutdown bytes separately. Demo uses its actual standard DEBUG4 packets; there is no invented Demo INFO3 DLA probe, live physical FLOW or kernel-quiet claim.

Native Save, Open and the actual SaveAs menu action preserve4000000B in one logic chunk, all probe names, unitsize, samplerate and complete metadata. Both srzip files pass ZIP CRC; every entry's uncompressed bytes match. Payload SHA256 is fc144b696f5976a13b338638a21a04a4977b2da733c3f5b000b4151f6870d748. Whole ZIP hashes differ and are retained separately; their ZipInfo timestamps differ, so whole-container identity is not claimed. Both archive hashes remain unchanged after GUI close. Save and SaveAs completion were visually checked with no active save/progress dialog before native CtrlQ. The earlier report's15%-progress limitation is not transferred to this new result. No acquisition or Save retry occurred.

An independent byte-index formula verifies every byte of both archives: each32-bit sample i has all four byte lanes equal to i mod256, starting at0. It calls neither the producer nor the observer implementation and agrees with the pinned Demo source contract. **This pattern repeats every256samples:** it cannot locate whole-cycle replay/drop and is not an electrical or arbitrary32-channel integrity witness. This result validates the declared software data/storage path, not AUD006 continuity or high-rate USB behavior.

Original GUI Popen/pidfd wait0, supervisor actual wait0, controller original wait0 and Xvfb original Popen/pidfd wait0 are all retained. No cancellation signal was sent to the GUI. Unused gpib/iec/max7219 startup decoder warnings and XKB F23/F24 warnings remain in the original logs; those decoders/key mappings are not qualified here. Product verification reconfirms1248 primary files/HEAD/status/diffs, installed Linux GUI/modules and launchers. Windows remains off; passive snapshots show the DLA at3-2/5000Mb/s and source2-1.3/ttyUSB1 free. No image was promoted. Windows/hardware are explicitly excepted for this private Linux X11 software control, with no changed shared product code or new electrical test.

The next gate is a separately planned physical Linux GUI INFO3 runner with an explicitly chosen display route, original LinuxDLA identity and independent current kernel ARM/Stop/quiet plus source-held finalization. This Demo alone cannot release a source or qualify that missing physical path. Closed electrical cohorts and historical failures keep their verdicts. Local evidence is `artifacts/private-X11-Demo-20261107/README.md`, a frozen131-file closure manifest and original native outputs/archives; only this report is published, excluding private captures, binaries, VM/authentication material.


## 7 November 2026 — Linux finite INFO observer preparation; native GUI prerequisite refused

This is a private Linux laboratory-helper result, **not a product fix or a qualified Linux GUI INFO3 acquisition path**. No product source, installed binary, driver configuration or firmware changed. There were zero physical captures, zero source commands, zero DLA assignments and zero Windows boots. One unchanged native Linux GUI process was launched with Demo only; it never reached Run. Linux is inside VMware; the nested Windows KVM was not used. AUD006 remains open.

The new observer gives Linux its own explicit roles instead of reusing a Windows/software tag for physical evidence. It retains the original native Popen and pidfd, joins current boot/UID/parent/startticks/session, checks the unchanged native GUI image and exact argv, and verifies actual mapped module paths, disk identities and hashes. It binds the log file's device/inode, existing prefix and EOF before GO; old lines cannot confirm the new acquisition. Current bytes, old prefix, rejected bytes and coalesced following bytes, including an incomplete UTF8 tail, remain separate. Full original stderr is also retained, including bytes not yet consumed when an observation fails.

The first real software-child test was correctly refused before binding: Btrfs reported device0:46 for stat and device0:28 for the same mapped inode438756. Linux Btrfs getattr uses a per-subvolume anonymous device, whereas proc maps and mountinfo use the superblock device; the relevant primary implementations are [btrfs_getattr](https://github.com/torvalds/linux/blob/master/fs/btrfs/inode.c), [show_map_vma](https://github.com/torvalds/linux/blob/master/fs/proc/task_mmu.c) and [show_mountinfo](https://github.com/torvalds/linux/blob/master/fs/proc_namespace.c). The corrected helper checks the same mount namespace/root, exact map_files link, inode, longest matching mountinfo record, expected disk device/inode and file hash. Only Btrfs permits this specific stat-versus-superblock distinction; a mismatched mapping device is still refused. No elevated map_files access was requested. The original refusal, original exit0 and original helper revision remain preserved.

Final software validation passes **77 contracts, 15 actual Linux child-process cases and one actual X11 BadMatch negative case**. These are explicitly software evidence, not physical acquisitions. Cases include stale identity/image/module path/hash, missing/deleted mappings, exit37, old log exclusion, partial-tail conservation, coalesced BIND+GO, EOF, timeout, cancellation and evidence-write failure. In the added containment case, a controller proxy dies with exit23 and closes its pipes; an independent supervisor keeps the original producer alive, preserves the EOF refusal, then records the original Popen wait0 with pidfd retained. This tests that failure path, not immunity to SIGKILL or host/session shutdown.

The native Demo attempt is **refused, not qualified**. X11 discovery found the original GUI window, but X_SetInputFocus returned BadMatch and terminated the controller before BIND or Run. The observer preserved EOF-before-BIND and no native END. Subsequently the supervisor was absent while the GUI remained alive and reparented; the exact external termination mechanism is not established. There is no original Popen wait receipt for this attempt. The active Wayland seat then reported LockedHint=yes, consistent with unavailable GUI focus; the precise X11 mapping cause was not isolated. Native WM_DELETE_WINDOW closed the same PID/start/window, with exit observed through a new pidfd only. That later exit observation does not repair the missing original wait or qualify the run. No Save/Open/SaveAs or current HEADER/LOGIC/END evidence was obtained.

Minimal private containment corrections make Xlib errors catchable, wait for a viewable target and verify actual focus before keys. The controller starts the supervisor in its own session; events are durable before unbuffered pipe delivery, and a broken controller pipe cannot cause a second logging exception or discard original native-process ownership. A real X_SetInputFocus against an unmapped fixture X window reproduces error8/request42 and now becomes a catchable refusal while the Python process survives. This fixture launches no native GUI process and changes no other application's focus. The failed attempt and its executed source revisions remain separate from the corrected, software-tested revision. No second native Demo GUI was launched in this lot.

DLA INFO3 contains native HEADER/END counters, not per-packet FLOW. Demo has no such INFO3 probe and requires its separate standard DEBUG4 HEADER/LOGIC/END/Stopped path. Demo, a timer or process exit cannot release a physical source. A future physical runner must independently join the current kernel ARM, Stop, retired requests/quiet and finalized monitor while the source remains held; even that grants ownership release, not data-integrity qualification. This preparation supervisor refuses physical LinuxDLA launches outright.

The **Linux GUI INFO3 path remains unqualified**. Existing finite private Windows INFO3 evidence keeps its previous scope and is not transferred to Linux. Windows/electrical tests were excepted here because only private Linux proc/pidfd/X11 helpers changed; frozen common binding code and all product files remain unchanged. Verification reconfirmed 1248 primary files/HEAD/status/diffs, installed Linux GUI/modules and launchers. No image was promoted. Once desktop access is available, the corrected helper needs a separately bounded native Demo regression with finite END and Save/Open/SaveAs equality before a distinct physical Linux INFO3 plan. Closed electrical cohorts are not reopened or reclassified.

Local evidence: `artifacts/linux-info-observer-20261107/REVIEW.md`, original refusals and revisions, final r3 software receipts, and a frozen657-file manifest. Only this document is published; private binaries, captures, VM material and authentication data are excluded.


## 7 November follow-up: eight configured INs still depleted in a replay trace

The retained multi-IN counterexample is now checked directly, without another
capture, source command, GUI run, Windows boot or product edit. Both observations
use the db7 CAPI+Qt Segment helper/84 core,50MS/s/100Msamples, eight initial1MiB
IN requests before ARM and381 resubmissions in the full active scope. The product
already configures8 transfers; setting8 is not a new fix.

Every original request closes. Kernel event-order accounting covers exactly ARM
completion through the last accepted native RAW completion:382reads/400556032B,
including the whole final request, for400000000 accepted LOGIC bytes. Later
processing, unused positive results, cancellation, quiet and shutdown are outside
this receiver window. It is not sampling time, CPU time, bus utilization or FIFO
occupancy.

| Outstanding sample INs | Fresh negative duration(ms) | Historical derived-V2 control duration(ms) |
|---|---:|---:|
| 0 | 1455.928 | 0 |
| 1 | 1732.349 | 0 |
| 2..5 | 132.554 | 0 |
| 6 | 40.669 | 3.365 |
| 7 | 139.749 | 622.056 |
| 8 | 247.583 | 1624.878 |
| Total | 3748.832 | 2250.299 |

The failure spends85.0472% of that window at depth0/1 despite initial/peak8.
The control stays6..8. At the final accepted completion they have0 versus7
requests left. A buffer holding a completed result is not an outstanding request
or spare read capacity; zero pending does not mean the application buffers are
free. Configured capacity and effective kernel occupancy must remain distinct.

The historical control is *derived V2 only*: its original R1 live qualification
and cancelled Windows forward phase remain refused. It is not a retroactive
live pass. Different boots/USB paths/history also prevent a causal performance
comparison: the historical path is4-2/bus4device16/source3-1.2, the fresh one
3-2/bus3device3/source2-1.3; both negotiated5000Mb/s in VMware Linux. Firmware is
unknown/unqueried. No new Windows/common product code is affected by this
Linux-only offline analysis.

All four original D0 faults are re-derived and mapped to complete32B RAW-frame
brackets. Their containing completions change outstanding count1→0. The third
bracket spans two completions and both are retained. Before the first faulty
receipt there are151 nonzero-duration empty-queue intervals, totalling531.036ms,
longest31.423ms. This trace cannot rule out an earlier depletion role. The largest
559.303ms empty interval starts once all four fault-containing receipts have
occurred, preserving the previous ordering conclusion. Receipt times are not
physical sample-production times.

Direct comparison of original kernel bytes re-verifies61808640B repeated at
exactly+134217728B, SHA24f5f247ca8d5c7f0b91dfb37dd51fc45b661aad03772fc9595861d4f686e849
for each full copy. Only59155456B lie within both400MB accepted LOGIC prefixes.
The failure's2097152B unused positive RAW stays separately classified. Accepted
native RAW/kernel-prefix hashes also match in both cases; prior all32
conversion/storage equality and D0/D22 electrical-witness limits are preserved.

15 contracts pass, including1000 nonempty duration-oracle scopes,4096 edge-frame
comparisons and unique old-schema command bindings. An independent midpoint
interval oracle reproduces all756/763 real duration intervals and event states,
plus each fault-prefix histogram. All35 original inputs and25 frozen analysis
files remain intact; product1248 and installed Linux binaries/launchers are
unchanged. The first analysis refused missing command record ordinals in the
older metadata. Its source/refusal are preserved; unique original byte-hash/time
S/C bindings now supply those ordinals without rewriting the old reviews.

A single *configured* IN is not necessary for this replay, and configuring8 is
not sufficient to preserve integrity or maintain8 pending. This does not disprove
a dynamic low/empty-queue condition: it was present before the first fault.
No receiver redesign, buffer change, causal fix or new rate qualification follows.
AUD006 remains open. Private evidence: `artifacts/multi-in-occupancy-20261107/REVIEW.md`.


## 7 November follow-up: one kernel IN does not match receiver buffering

A read-only audit of five retained50MS/s/400MB captures distinguishes kernel
request depth from RAW-result buffering and frontend coupling. Zero new captures,
serial commands, native GUI executions or Windows boots. The existing source,
firmware uncertainty and VMware Linux/nested-KVM Windows context are preserved.

The WCH source has two RAW-result slots and one synchronous SDK read at a time.
It publishes a result FIFO and can read into the other slot while main processes
the previous result; a full two-slot queue applies backpressure. In the private
one-IN Linux diagnostic cores, the callback decodes, synchronously sends LOGIC
and returns from consumers before resubmitting its RAW transfer. These are RAW
slots, not total application allocations. The INFO candidate leaves the worker,
logger and session source bytes unchanged.

The old Windows DEBUG/no-WIRE GUI evidence has383 consecutive native reads,
382 LOGIC packets totalling400000000B and one1048576B unconsumed result. Counts
and32B prefixes match the ordered active kernel requests. The same-process
worker and default logger use GLib monotonic time; the logger subtracts one
fixed startup integer E. Read-return-before-announcement and two-slot FIFO reuse
give E=[101794904,101800990]us, without aligning Windows time to Linux kernel time.
The reuse bound follows because read i+2 cannot start before the previous
buffer's synchronous send/release has finished.

Across all6087 feasible common epochs, the next SDK read had started before the
previous LOGIC announcement at all382 boundaries; at381 it was still active at
that timestamp. Read331 remains ambiguous as to activity at the LOGIC instant.
All382 next-read intervals overlap some of the main path between current read
and LOGIC announcements. The joint summed marker overlap is112114..112782us
using one E throughout; independently minimized rows give only a looser112094us
bound. These timestamps precede completed logger/Qt/storage callbacks. They do
not measure storage overlap, CPU cost, speedup or physical USB utilization.
The newer INFO capture lacks per-read timestamps: overlap there is not newly
measured or fabricated from missing records.

All four full2048B Stop/setup/ARM/finalStop commands, including framing, CRC and
zero padding, are byte-identical across the five observations. Entire captured
USB scopes differ: the Linux scopes also retain a512B identification request
and512BDL32 reply before initial Stop. Those originals are saved separately;
their absence from the recorded Windows Run slices says nothing about earlier
GUI discovery outside those slices. Equal capture commands do not prove equal
device state, physical arrival timing, mode history or FIFO occupancy.

| Retained case | Active reads | First IN submit minus ARM completion(us) | IN pending at ARM completion | Preserved witness result |
|---|---:|---:|---:|---|
| Linux1MiB | 382 | -567 | 1 | positive,99CRC |
| Linux512KiB DEBUG | 763 | -415 | 1 | replay,110CRC occurrences/78unique |
| Linux512KiB WARN | 763 | -315 | 1 | positive,100CRC |
| Windows DEBUG | 383 | +1042 | 0 | positive,99CRC |
| Windows INFO | 383 | +824 | 0 | positive,99CRC |

The start ordering agrees with source: Linux submits before ARM; WCH launches
its reader after accepted ARM. Original electrical witnesses remain D0/D22 and
all earlier strict-profile refusals retain their verdicts. These observations
also differ in frontend, quantum, logging, trace and history; they are not a
paired causal intervention or a new cross-OS throughput comparison.

19 contracts pass:12 parser/epoch/overlap contracts with1000 exhausted small
domains, plus7 command-original/mutation checks. An independent checker exhausts
all6087 actual epochs at every one of382 boundaries, reproducing the per-row
bounds and the joint aggregate. The first script refused the extra Linux
identification request rather than silently dropping it; its source/failure
and the explicit classification correction are retained. All52 pinned original
inputs and46 frozen analysis files are intact; product1248 and installed Linux
binaries/launchers are unchanged. Windows files are retained originals, not
rechecked through a new boot. Pure offline analysis runs on Linux; no common or
Windows product code changes require a new platform/hardware run here.

Kernel maximum depth1 is insufficient to match application pipelines. Future
experiments need explicit RAW-result capacity and next-read/ARM/frontend
accounting. This finding neither attributes replay to buffering nor justifies
a Linux reader redesign. No closed cohort is repeated, no private image is
promoted, and no cause, product fix or new rate qualification is claimed.
AUD006 remains open. Private details: `artifacts/receiver-pipeline-20261107/REVIEW.md`.


## 6 November follow-up: finite INFO3 GUI route qualified; AUD006 remains open

The private INFO lifecycle probe from the preceding software report now has a qualified finite
Windows GUI route. Exactly three new captures, zero retries and one Windows
boot were completed: private-core Linux5M control, Windows50M GUI, installed-core
Linux5M return. The sourcev8, wires and DLA USB3 path stayed unchanged. Linux is
in VMware; Windows22000 is in nested KVM. DLA firmware remains unknown/unqueried.
No flash, FPGA change, USB reset, hub action or product install was performed.

The observer binds the original CreateProcess HANDLE identity, PID/creation
FILETIME, executable hash, session1 and the four actually mapped module hashes.
The original native-file launcher implementation is unchanged; an appended
helper queries its existing HANDLE. The read-only observer uses synchronized
process waits, so an actual exit259 cannot masquerade as STILL_ACTIVE. Its DLA
profile requires the unchanged51b GUI and private a17 core/75aa CXX, plus the
existing decoder and WCH SDK hashes; synthetic Python producers cannot qualify
as hardware evidence.

Before Run it binds the log file HANDLE/volume/index and EOF, retaining the old
prefix. GO refuses intervening append, replacement, truncation or prefix changes.
Current bytes and coalesced following bytes are conserved separately, including
rejected chunks. Thirty-seven Linux binding contracts and twelve native Windows
software cases pass, covering stale frames, identity/hash failures, EOF, timeout,
partial tails and exit259. The Windows positives are explicitly synthetic software
fixtures with the real private DLL loaded, zero acquisition/serial access.
An initial PowerShell-path refusal happened before launching a test child;
its evidence and the corrected absolute-path harness are retained.

Twenty-six further controller checks exercise its actual reader/finalizer with
injected failures. Source release requires both current backend END and independent
original kernel Stop/quiet in the same ARM scope. Reader/process exit, a deadline,
synthetic evidence, an old process or GUI exit alone cannot release the source.
There is no live per-packet FLOW at INFO3 and no blind Run/Stop-toggle action on
timeout. These are software guards, not electrical fault-recovery qualification.

The physical Windows run used finite Stream50MS/s/100Msamples400MB, all32 channels,
1.6V, no trigger, one1MiB IN, INFO3 and no WIRE. READY bound the72B pre-Run prefix;
the997 new bytes contain exactly one current HEADER/Stop/worker/END/Stopped.
The real following tail is0B; synthetic tests separately cover partial tails.
The full original stderr, including subsequent file-replay output, is retained.
Native END reports100000000 samples, complete/error-free/result0, pending0 and
two quiet reads. The source remains owned through native END and independent
kernel finalization. All253live+3preflight ACKs are unique and ordered; the
original GUI and controller exit with zero.

Native Save/Open/SaveAs preserve400000000B,96chunks and metadata exactly.
Independent all32 conversion from401604608B of original active kernel data
equals both saved payloads. The entire1604608B active surplus is retained;
the worker's1048576B unconsumed counter is distinct. D0 has zero deviations,
periods49..51 and edge distances43/45 samples. Both STOP-aware decoders agree
on99 consecutive CRC778101..778199. All97794049 origins of the fixed44.119040ms
window contain a complete message. A final partial UART frame extends beyond
capture EOF and is preserved/excluded, not counted as a valid STOP/CRC message.
All32 equality validates conversion/storage; electrical continuity witnesses
remain limited to D0/D22.

The last SaveAs screenshot still showed15% progress; CtrlQ preceded a separate
visual-completion check. The original exit0 and closed archive were obtained
before reading it, and ZIP CRC, full400MB payload and exact metadata equality
subsequently passed. Visual SaveAs completion is not claimed; no Save or capture
retry was used. Twenty-six closed Windows originals are copied and size/SHA/
archive verified before protected detach and normal shutdown.

Both Linux5M/100MB controls pass all32 kernel/feed/Segment equality, zero D0
deviations and50 consecutive CRC each:777101..777150 and779101..779150.
Each retains100663296 active kernel bytes including663296B surplus. The reference
uses the original CAPI+Qt Segment helper/private8fa core and checks both INFO
records against accepted native counts; its helper still uses DEBUG4. It is
not a Linux INFO-only GUI test and does not load libsigrokcxx. The return uses
the installed84 core/original helper. END/quiet and finalized monitors precede
source close; original native/controller exits are zero.

Windows ARM-to-last-active-RAW completion is2786.746ms:2331.308ms with an IN
pending and455.438ms without one, longest gap11.640ms. This excludes final
callback, settle/cleanup and Save; it is not CPU cost, physical USB utilization
or FIFO occupancy. The existing2s retirement hold remains visible and unchanged.

The325 physical-cohort and438 adapter files are frozen; all1667 original prepared
inputs, additional Windows pins and the prior161-file software preparation remain
intact. Eight installed Windows files,1248 primary product records and installed
Linux binaries/launchers are unchanged. Windows is off, DLA3-2 is still5000Mb/s,
and the source2-1.3/ttyUSB1 is free. Details are retained privately in
`artifacts/gui-info-physical-20261106/REVIEW.md` and
`artifacts/gui-info-adapter-20261106/REVIEW.md`.

Only the declared private finite INFO3 GUI lifecycle route is qualified.
Continuous mode, manual Stop and electrical fault recovery require separate
gates. This single positive is not a paired causal experiment: it does not show
that logging causes, prevents or fixes the earlier128MiB replay. Older failures
and strict20/30ms profiles retain their verdicts. No private image is promoted,
no product fix or global high-rate qualification is claimed; AUD006 remains open.


## 6 November follow-up: replay without a long initial-prefix fputs call

The failed parent-polling observation was reproduced in software: while the
controller did no polling during a blocked-pipe fixture, the old sampler
captured zero states and a separate passive process captured 49 fully
contained pipe-wait states. Nine contracts and the existing helper's
20 MiB + 12 byte feed/Segment integration test pass. An incomplete ready-JSON
publication was caught before launching that helper, preserved and fixed by
atomic exclusive publication with an interleaving regression. Six additional
phase/recovery/coverage contracts passed before hardware I/O.

A new plan completed exactly two acquisitions, zero retries and zero Windows
boots: one finite Stream 50 MS/s / 400,000,000 B observation with the same
512 KiB/one-IN core, prefix observer and CAPI/storage helper, followed by the
reserved 5 MS/s / 100 MB return with the installed core and no diagnostics.
The independent sampler starts before the original helper process, binds
PID/starttime/UID/parent/executable, reads only mmap and guest `/proc`, stores
bounded RAM records and writes once after normal target exit. It sends no
target signals and inherits no USB/serial descriptors. All timing and denied
fields are preserved; observer/sampler load can affect the run.

The target still fails integrity: three D0 periods 30/35/34 instead of
50 ± 1, 99 valid CRC occurrences but 92 unique numbers. The sequence jumps
772159→772194 then 772200→772168; 772194–772200 repeat. Both global STOP
decoders agree without an unchecked EOF frame. Accepted RAW
`[238280704,265782272)` equals `[372498432,400000000)`:
**27,501,568 B at +134,217,728 B**, SHA256 each
`fa91dc3dfd6b349aa791f92a7c2cc1571ddfd553112a7b10794731473ad21c18`.
The first two fresh pairs, 772194/772195, separately pass local independent
and SDK full-final-STOP checks in both copies. All 400,031,744 B kernel/RAW
and the accepted all-channel independent conversion/feed/Segment equality
are retained; the surplus is not discarded or appended to accepted data.

All 2,315 initial-prefix fputs calls and 763 callbacks have complete telemetry.
The maximum actual initial-prefix fputs duration is **2.229495 ms** across
all log categories. From ARM through the final active RAW completion, the
largest Linux-observed interval without a pending IN request is **7.654 ms**.
Thus an individual initial-prefix fputs call of at least 20 ms, or a Linux
no-pending-IN interval of at least 20 ms in that acquisition window, is not
necessary for this corroborated replay. This does not exclude
cumulative logging cost, other logger/RAW/storage stages, pending-request
waits or device/bridge/VM behavior, and does not establish a root cause.

The independent sampler retained **705 observations**, maximum sample-start
gap **5.706965 ms**, zero skipped slots, and was bound and recording at least
158.989692 ms before actual ARM. Readiness uses original command/completion/
read clock brackets without a fitted outlier or assumed sample-zero epoch;
kernel realtime mapping assumes no incompatible step. Guest syscall access
was denied in all observations and is not claimed as a syscall trace.
Startup and cadence are verified on hardware. No initial-prefix call reached
the predeclared 20 ms threshold, so blocking-site coverage for such a call
remains unqualified. Missing state from the older 172/327 ms events is not
filled retroactively, and the cohort is not retried to seek the old event.

The return passes D0 and 50 consecutive CRCs, 773101–773150, with exact
kernel-prefix/feed/Segment equality. All source commands have exactly one
ordered ACK. Actual END, final Stop, sequential quiet reads, finalized
observers, source-close ordering and original process waits of zero are
retained. DLA USB 3-2 / 5,000 Mb/s, source v8, firmware, wiring and hub remain
unchanged; USB/source are free and Windows KVM is off. Linux runs in VMware.
No product sources or installed binaries changed. The sampler/core/observer
remain private LAB diagnostics; no Windows or arbitrary 32-channel electrical
qualification. AUD006 remains open, with no logging or Segment fix justified.

## 6 November follow-up: measured fputs duration; live thread state remains unqualified

A new bounded Linux observation used the unchanged private 512 KiB/one-IN
core and existing native CAPI/storage helper, with an explicitly hashed
private preload around the actual `fputs("sr: ", stderr)` call. It preserves
output, return value and errno, calling the real function once. A preallocated
1,310,848 B mmap records wall/thread CPU/rusage; there is no per-prefix trace
write or allocation. The observer still changes timing. Eight software
contracts and four phase/recovery/clock contracts passed before device I/O,
including the actual logger, blocked pipe, pause before entry, overflow,
exclusive refusal and the existing feed/Segment ABI selftest.

The fixed plan completed exactly two acquisitions, zero retries and zero
Windows boots: one observed finite Stream capture at 50 MS/s, 100 million
samples / 400,000,000 LOGIC bytes, then the reserved 5 MS/s / 100 MB return
with the installed core and no observer. DLA `1a86:5537`, USB `3-2`, negotiated
5,000 Mb/s, source v8, firmware, wiring and hub were unchanged. Linux is VMware;
Windows KVM remained off. The previously closed cohorts were not rerun.

The target again fails integrity despite native completion: five D0 faults,
99 CRC occurrences but 78 unique numbers, with 768180–768200 repeated.
The malformed UART fragment and global SDK STOP/framing refusal are retained.
Accepted RAW `[181605536,265782272)` equals `[315823264,400000000)`:
**84,176,736 B at +134,217,728 B**, SHA256 each
`155d39fd3c7417661b3a8492625643b76b54a7320026c7c8a45d8e93db4e1784`.
The deterministic first two fresh pairs, 768180/768181, separately pass local
independent and SDK decoding with full final STOP high for both copies.
Original-order kernel/RAW contains 400,031,744 B; the accepted prefix converts
independently to exactly feed and actual Qt Segment storage. Surplus is kept.

All 2,315 prefix records and 763 callbacks match the original stderr/kernel
ordinals in the native main thread, without telemetry overflow or errors.
At callback 252, RAW offset 132,120,576 B, the actual real-fputs interval is
**172.231798 ms**, with thread CPU clock span **0.086960 ms**. The broader
rusage bracket records one voluntary, zero involuntary switches and one
minor/zero major faults; it includes observation boundaries and is not an
isolated syscall trace. Time before observer entry is bounded 0–13.029 us;
argument-to-logger timestamp is 172.271899–172.294867 ms. This new delay is
inside the measured call. It does not assign the older 327 ms interval.
Same-packet RAW/conversion/feed/Segment work follows the call; no ten-MiB
chunk fills in this callback. Wall duration includes waiting/descheduling.

The call lies inside the original 173.695 ms no-pending-IN gap. The first
faulty completion arrives **395.846663–395.913286 ms after the call returns**,
so no immediate splice or direct corruption cause is claimed. Clock origins
are bounded from all original command/completion/synchronous-read constraints,
without fitting the outlier or assuming an electrical sample-zero epoch.
Kernel realtime mapping assumes no incompatible clock step; the measured
call duration itself uses a monotonic clock.

**Live thread-state attribution remains unqualified:** no `/proc` samples
were obtained during the delay, despite the software sampler test and no
sampler exception. The source parent's 187.223529 ms command-start gap spans
the call, without proving why sampling missed it. There is no wchan witness
for a blocked write, FILE lock, disk, scheduler or outer VMware cause. The
instrumented run and its incomplete coverage are retained without retry.

The 5 MS/s return passes D0 and 50 consecutive CRCs, 769101–769150, with exact
kernel-prefix/feed/Segment equality. All source commands have one ordered
ACK. Both phases retain actual END, final Stop, sequential quiet reads,
finalized observer, source-close ordering and original process waits of zero;
USB/source owners are free. Ninety-four preparation files, 145 physical
artifacts and 1,587 prepared inputs are frozen privately. Product sources,
dirty work and installed binaries are unchanged. No observer/core promotion,
logger/Segment fix, new Windows qualification or arbitrary 32-channel
continuity claim. The fputs duration is qualified; root-cause attribution
is incomplete and AUD006 remains open.

## 6 November follow-up: the 512 KiB stall precedes conversion and storage

Read-only analysis of the closed USB-quantum pair accounts for all 382
one-MiB and 763 half-MiB callbacks, exact original kernel/RAW packet order,
counts and offsets, all 400,000,000 accepted LOGIC bytes per case, and all
38 ten-MiB Segment chunk-fill paths per capture. Four clock/order contracts
pass; 27 source/trace inputs are pinned. There were zero new acquisitions,
source I/O, native capture processes or VM boots. The captures were not rerun.

The largest callback argument-to-default-logger timestamp interval is
2.850–3.130 ms in the positive one-MiB control and **327.711–328.013 ms**
in the negative half-MiB control. Upper bounds for all other callbacks are
0.915 ms and 1.724 ms respectively. Bounds use source order and the same
process monotonic clock, not an assumed alignment to electrical sample zero.

The half-MiB outlier is callback 355, zero based, RAW offset 186,122,240 B.
It advances LOGIC to 186,646,528 B and does not fill a ten-MiB chunk.
In `receive_transfer`, the argument clock is sampled before calling the
logger. Default `sr_logv` writes the initial `sr: ` prefix with `fputs` before
sampling its timestamp. Status handling, payload logging, RAW output,
conversion, LOGIC logging, feed write and Segment append follow that call.
Thus the long interval precedes that packet's conversion and storage.
It includes logging dispatch/initial prefix output and possible descheduling;
it is not measured CPU time or an isolated blocked-write, FILE-lock, disk,
scheduler, allocator or VMware diagnosis. Background pressure is not excluded.

The corresponding kernel completion-to-resubmission gap is 329.254 ms.
Both D0 fault brackets arrive after the outlier logger timestamp; the first
faulty completion arrives 3.899–4.067 ms later. Kernel clock origins use all
completion, command and synchronous-read brackets and reject empty
intersections without fitting the outlier. Kernel realtime mapping assumes
no incompatible realtime step; the monotonic delay bounds are separate.
This chronology is compatible with a service delay preceding corruption,
without proving the replay's cause or identifying an internal physical splice.
The older eight-IN negative has different fault/gap chronology and is not
reclassified by this one-IN analysis.

The result rules out same-packet Segment chunk growth as an explanation for
time already elapsed before its conversion/feed/append. It does not justify
a Segment workaround, logger fix or firmware claim. AUD006 remains open.
Linux VMware trace analysis changes no common product code or installed
binary and adds no Windows or arbitrary 32-channel electrical qualification.
The previous END/quiet/source-close proofs and 5 MS/s return remain intact;
15 analysis artifacts and the original 245 evidence files are frozen locally.

## 6 November follow-up: 512 KiB requests also reproduce the 128 MiB replay

A new Linux-only necessity experiment asks whether a 1 MiB active USB IN
request is required for the observed RAW replay at a 128 MiB displacement.
Two private cores were built from the 477 verified sources of the currently
used runtime. Both have one pending IN request; their only source difference
is `TRANSFER_SIZE` 1 MiB versus 512 KiB. Neither core was installed or promoted.
GNU checks, two existing helper ABI/feed/Segment selftests and 15 lab guard
contracts passed before I/O. The detector was first checked against the
original negative, including four local independent/SDK full-STOP anchors.

The fixed plan allowed at most four acquisitions, no retries and no Windows
boots: normal 1 MiB, normal 512 KiB, a 700 ms injected pause only if both
normal controls passed, then the reserved 5 MS/s return. There were exactly
three acquisitions. The normal 512 KiB capture already reproduced the replay,
so the injected pause was omitted. Linux ran in VMware; Windows KVM stayed
off. The same DLA `1a86:5537`, USB `3-2`, negotiated 5,000 Mb/s, source v8,
1 MHz D0, numbered CRC UART D22, wiring, firmware and hub were retained.

| Phase | Pending IN / request size | LOGIC bytes | D0 faults | Fresh CRC result |
| --- | --- | ---: | ---: | --- |
| Normal 50 MS/s | 1 / 1,048,576 B | 400,000,000 | 0 | 99 consecutive: 764101–764199 |
| Normal 50 MS/s | 1 / 524,288 B | 400,000,000 | 2 | 109 occurrences, 79 unique; replay |
| Reserved 5 MS/s return | 8 / 1,048,576 B | 100,000,000 | 0 | 50 consecutive: 767101–767150 |

High-rate geometry was finite Stream, 100 million samples, 32 channels,
1.6 V, no trigger/windows. Native RAW exactly equals the original-order
active kernel payload: 400,556,032 B for 1 MiB and 400,031,744 B for 512 KiB.
Surpluses beyond accepted LOGIC were retained. Independent all-channel RAW
conversion equals feed and actual Qt Segment storage in every phase. The
negative is already present in kernel payloads, before that conversion and
storage; this does not distinguish DLA, bridge or virtualized host USB path.

For 512 KiB, accepted RAW `[186703872,265782272)` equals
`[320921600,400000000)`: **79,078,400 B at +134,217,728 B**. Both ranges hash to
`c3f32f1f1ebc4566c1c38141344c568e91bf5026abd6364622a2b7a63147c4c9`.
Thirty fresh CRC pairs occur in this interval. The deterministic first two,
765171 and 765172, additionally pass local independent and SDK decoding with
full final STOP high for both copies. The full decoders also agree: the
sequence jumps 765146→765171, then 765200→765168; 765171–765200 occur twice.
D0 periods 14 and 35 samples violate 50 ± 1. The first D0 bracket is inside
an original 512 KiB completion, around offset 57,344 B; the other crosses
the 256 MiB RAW boundary. These brackets do not identify an exact internal
physical splice. A 329.254 ms no-pending-IN gap precedes the first faulty
completion, but its cause was not isolated and no pause was injected.

All source commands, including three preflight and 148/173/51 live commands,
have exactly one ordered CRC ACK per phase. Each phase retains real native
HEADER/END, cleanup, final Stop, two sequential quiet reads, all USB requests
closed and a finalized observer before source ownership was released.
Original native and controller process waits both returned zero. Device identity and free ownership were
rechecked. Raw captures, commercial firmware and credentials remain private.

**Limited conclusion:** a 1 MiB request is not necessary for this
128 MiB-displacement replay in the tested setup. The positive 1 MiB control
is not a fix, and the failing 512 KiB variant must not be promoted. AUD006
and general high-rate qualification remain open. This Linux lab experiment
changes no common product code and adds no Windows qualification. D0/D22
are electrical witnesses, not proof of arbitrary 32-channel continuity.
The predeclared necessity-test contract includes a full fresh CRC in every
contained 44.119040 ms window; it does not override historical 20/30 ms
profile refusals. Product sources, dirty work and installed binaries remain
unchanged; 245 evidence files and 1,475 prepared inputs are frozen locally.

## 6 November follow-up: the matched WIRE-on capture also passes

A new bounded comparison changed the existing Windows WIRE instrumentation
setting from the qualified no-WIRE case to ON. It retained the exact GUI
`51b1e1cc42272626f186c6d38b44e321e66ff56a54c52f4a9a09f4c8405b65e5`,
the same loaded core/C++ wrapper/decoder/WCH hashes, native C# process-handle
and file-output launcher, 50 Hz witness schedule, and capture configuration.
All four USB command packet hashes, including setup and ARM, matched the
qualified no-WIRE capture. WIRE enables its existing synchronous native
RAW/worker-sidecar output and conditional debug records; those observation
costs are part of the intended intervention.

The separately frozen plan allowed three acquisitions, zero retries and one
Windows boot: Linux native CAPI 50 MS/s reference, one Windows GUI 50 MS/s
capture with WIRE, then Linux native CAPI 5 MS/s return. All three completed.
Fresh source numbers were checked before I/O. No closed cohort was restarted,
and no product source, installed binary, transport setting, firmware, FPGA,
hub power or wiring change was introduced. Linux ran in VMware and Windows
in nested KVM; only one OS owned the same `1a86:5537` DLA at a time, USB path
`3-2`, negotiated 5,000 Mb/s. The source remained v8 with D0 = 1 MHz, D22
numbered CRC UART, camera enabled and SD disabled.

Both 50 MS/s acquisitions requested 100 million samples / 400,000,000 LOGIC
bytes, finite Stream, 32 channels, 1.6 V, no trigger, one capture and no stream
windows. Both used a 20 ms source-command period. The Linux return requested
25 million samples / 100,000,000 bytes at 5 MS/s and the established 100 ms
source period. The Linux CAPI controls used the unchanged pinned helper and
core; this does not add Linux GUI 50 MS/s qualification.

| New cohort phase | LOGIC bytes | D0 periods | D0 bad periods | Consecutive CRC messages | STOP-checked UART bytes |
| --- | ---: | --- | ---: | --- | ---: |
| Linux CAPI reference, 50 MS/s | 400,000,000 | 49–51 samples | 0 | 99: 704101–704199 | 2,772 |
| Windows GUI, 50 MS/s, WIRE ON | 400,000,000 | 49–51 samples | 0 | 99: 705101–705199 | 2,772 |
| Linux CAPI return, 5 MS/s | 100,000,000 | 4–6 samples | 0 | 50: 706101–706150 | 1,400 |

The acceptance bands stayed at 99 ± 3 CRC messages for 50 MS/s and 50 ± 1 for
the return. Two independent decoders agreed on every STOP-checked byte,
without an unchecked EOF frame. D0 edge bounds were 32/11, 36/7 and 1/0 samples
respectively. All source commands and ACKs were retained in order, including
250 live Windows commands through retirement. The source port stayed owned
until current native END and finalized independent kernel ownership/quiet
proof both passed. ARM permission came only from the complete original
kernel Stop/setup/ARM prefix and successful ARM completion.

The Windows native decoder-input RAW contained 400,556,032 bytes and the
worker-unconsumed sidecar contained 1,048,576 bytes. Together they matched
every byte of the original-order active kernel payload, 401,604,608 bytes.
The accepted 400,000,000-byte prefix independently converted to exactly the
native Save data. Native Open and Save As preserved every data byte and
metadata in both 96-chunk exports. Archive readers ran after Save progress
closed. The original Save was unchanged, Open had one current HEADER and
END without a new DLA Run, and the actual GUI closed with native exit zero
through its original process handle. All 23 guest originals were copied
and SHA256 verified before normal DLA detach and Windows shutdown. The six
installed Windows file hashes remained unchanged.

All positive kernel payload remained in its original pre-ARM, active and
post-Stop categories; surplus beyond accepted data was not used to repair
or extend the capture. Linux reference and return also passed independent
kernel-prefix/feed/Segment-storage conversion equality, native exit zero
and cleanup. Their kernel positive totals were 400,556,032 and 100,663,296
bytes. The final VM was off and USB/source owners were empty. The 21 frozen
control/analysis inputs and 107 retained original files were hash-checked.
The primary 1,248-file baseline, 477 private runtime source files, 416 GUI
candidate files and installed binaries remained unchanged.

The existing host reader and finalization/recovery nodes were retained.
Before I/O, 102 offline contracts passed: 64 protocol/recovery cases, 25
unchanged 50 Hz profile cases, and 13 WIRE pin/RAW-conservation cases. The
latter executed the actual analysis code on a retained real 200 MB RAW plus
worker sidecar and on explicitly synthetic corruptions. The guest observer
changed only its fixed WIRE-presence pin; it still obtains END from native
lifecycle records and never authorizes source release from RAW-file size.

Additional full-message analysis on this Windows capture passed the
unchanged Linux reference profile: initial full START 29.44342 ms, final
full STOP gap 5.73098 ms and maximum full message envelope 2.41790 ms.
A window without a fully contained message was bounded by 31.86088 ms,
below the known 44.119040 ms historical replay interval. The preceding
no-WIRE capture's additional strict-edge check remains refused at
30.79734 ms versus the Linux-only 30 ms limit. Neither the limit nor that
previous result was changed; these extra checks do not rewrite the frozen
Windows acceptance criteria.

| Matched Windows observation | WIRE absent | WIRE ON |
| --- | ---: | ---: |
| Accepted LOGIC bytes | 400,000,000 | 400,000,000 |
| Consecutive valid CRC messages | 99 | 99 |
| D0 bad periods | 0 | 0 |
| Positive kernel payload bytes | 401,604,608 | 401,604,608 |
| Active 1 MiB kernel reads | 383 | 383 |
| ARM completion to Stop completion | 2.851663 s | 2.829225 s |
| Median native read duration | 6,080 µs | 6,076 µs |
| Median idle interval between reads | 1,156 µs | 1,124 µs |

Both acquisitions pass under the current exact-GUI/50 Hz configuration.
Enabling WIRE alone did not reproduce corruption in this one matched pair.
Fresh counters, separate VM boots and scheduling variation remain nuisance
differences; these timing observations do not establish a general performance
improvement or universal cause. The earlier Windows 50 MS/s electrical
failures remain failures, and AUD006's cause and broader applicability remain
open. This does not qualify arbitrary electrical continuity on all channels,
continuous operation, every rate or unrelated mode transitions. No speculative
data repair, transport workaround or firmware change is justified by this
result. Private captures, vendor runtimes, VM material and credentials remain
outside the publication.

## 6 November follow-up: one Windows 50 MS/s capture passes without WIRE tracing

A new, separately bounded cohort completed after publication of the samplerate
snapshot correction. Its order was Linux native CAPI reference at 50 MS/s,
one actual Windows GUI capture at 50 MS/s, then Linux native CAPI return at
5 MS/s. All three acquisitions completed, with zero retries and one Windows
boot. The existing closed negative cohorts were not restarted.

The Windows GUI was the exact qualified executable
`51b1e1cc42272626f186c6d38b44e321e66ff56a54c52f4a9a09f4c8405b65e5`,
with the published samplerate snapshot source correction. Loaded core
`bf0a6c78b1ee4d8f6a3e2c5a0ca234cd2fef283419551023830f40d77bbad3a6`,
C++ wrapper, decoder and WCH DLL hashes matched their pinned values. The
original native process handle was retained. WIRE's optional environment
setting was absent, and no diagnostic RAW file or worker-unconsumed sidecar
was created. Native file logging at level 4 and passive kernel observation
remained enabled; this was not a test with every observation cost removed.

The hardware remained the same `1a86:5537` DLA, USB path `3-2`, negotiated
5,000 Mb/s. Windows ran in nested KVM; Linux ran in VMware. The source v8,
D0 = 1 MHz, D22 numbered CRC UART, camera enabled, SD disabled and wiring
remained unchanged. No reset, hub power cycle, FPGA or firmware change was
performed. Both 50 MS/s acquisitions requested 100 million samples and
400,000,000 LOGIC bytes in finite Stream mode, 32 enabled channels, 1.6 V,
no trigger and one capture. Their source command period was 20 ms (50 Hz).
The return requested 25 million samples at 5 MS/s and 100,000,000 bytes,
with the established 100 ms witness period. The Linux CAPI controls used
the unchanged pinned native helper and core; they are not Linux GUI
50 MS/s qualification.

Before hardware I/O, the lab's legacy controller was shown to ignore the
new observer's `NATIVE_END` event: it still expected `END_CONFIRMED` and an
ARM event from the log. The private integration now consumes native END
and obtains source permission only from the complete original kernel
Stop/setup/ARM sequence and successful ARM completion, using hashes pinned
from the new Linux reference. The log observer cannot authorize ARM or
source release. The observer itself is byte-identical to the one already
used in the twelve qualified Windows Demo acquisitions on this GUI.
Sixty-four offline integration contracts covered retained Demo transcripts,
original positive and negative physical scopes, fragmented native logs,
missing proof, mixed ARM scope, and five injected finalization failures
that retained the source owner. No product option or transport patch was
introduced for this integration.

| Phase | Native LOGIC bytes | D0 periods | D0 bad periods | Consecutive CRC messages | STOP-checked UART bytes |
| --- | ---: | --- | ---: | --- | ---: |
| Linux CAPI reference, 50 MS/s | 400,000,000 | 49–51 samples | 0 | 99: 683101–683199 | 2,772 |
| Windows GUI, 50 MS/s, no WIRE | 400,000,000 | 49–51 samples | 0 | 99: 684101–684199 | 2,772 |
| Linux CAPI return, 5 MS/s | 100,000,000 | 4–6 samples | 0 | 50: 685101–685150 | 1,400 |

The count acceptance bands were fixed before I/O: 99 ± 3 messages for the
50 MS/s acquisitions and 50 ± 1 for the return. Two independent UART
decoders agreed on all STOP-checked bytes, with no unchecked EOF frame.
All source commands and ACKs were retained in order, including 250 live
commands through Windows retirement. D0 capture-edge bounds were 50/41,
33/8 and 1/1 samples respectively, within the corresponding nominal
period plus one sample.

All accepted bytes matched an independent conversion of the original-order
kernel RAW prefix. Linux additionally compared native feed against actual
Segment storage. Windows compared native Save against the separate native
Open/Save As export, with identical data and metadata: 96 chunks and exactly
400,000,000 bytes in both files. Save progress had closed before any archive
inspection. The original Save remained unchanged, Open produced one current
HEADER and END without a new DLA Run, and the GUI closed with native exit
zero. All 21 guest originals were copied and SHA256 verified. The six
installed Windows file hashes remained unchanged.

The kernel positive payload totals were 400,556,032 bytes on Linux reference,
401,604,608 on Windows and 100,663,296 on Linux return. Every positive byte
was retained in its original pre-ARM, active or post-Stop category; active
surplus beyond accepted LOGIC was not used to repair the capture. The Windows
native worker reported 1,048,576 unconsumed bytes, pending zero, two empty
reads, quiet state and native thread exit. The independent kernel record
showed the pinned single-read, 1 MiB WCH geometry and finalized ownership
and quietness. The source stayed owned until both native END and kernel
quiet were proved. Linux native processes also ended with exit zero and
cleanup complete before source close. DLA detach and Windows shutdown were
normal; the final source and USB owners were empty.

The Linux reference passed the previously fixed full-message witness profile:
its full-message coverage bound was 25.55126 ms, below the known historical
44.119040 ms replay interval. An additional offline application of that
**entire Linux reference profile to Windows was refused**: the first full
message START occurred at 30.79734 ms, exceeding the Linux-only 30 ms edge
limit. That refusal is preserved; the limit was not changed. The frozen
Windows criteria required its stated D0, ordered CRC count, independent
conversion, native storage/replay and lifecycle checks, which passed.
The actual Windows messages still bound a window lacking a fully contained
message to 33.21424 ms, below 44.119040 ms. Their observed start gaps were
16.0007–22.10416 ms, and full message envelopes were at most 2.41844 ms.
Windows is not promoted to the stricter 30 ms reference profile.

This is one qualified Windows no-WIRE 400 MB capture under the stated
configuration. The earlier Windows 50 MS/s continuity/replay failure remains
a failure. The earlier negative used a different GUI build, optional WIRE
tracing and a different witness schedule; these differences prevent causal
attribution to any single change. AUD006's cause and broader applicability
remain open. D0/D22 temporal evidence and all-32-channel storage/conversion
equality do not prove arbitrary electrical continuity on every channel.
No installed GUI, core or driver was replaced, and no private capture,
vendor binary, VM image or authentication material is published here.

## 6 November follow-up: protect the samplerate snapshot during capture replacement

The existing-mutex correction in `Session::get_samplerate()` has completed
its Linux and Windows validation gates. The getter now holds the same
recursive `data_mutex_` used when the capture worker clears and appends
segment containers. This prevents the unprotected segment-container copy
identified in the debugger investigation below. The product change is two
added lines, including its comment; a 38-line regression exercises 64 real
Demo capture replacements while the main thread reads the samplerate.
No destructor, transport, firmware, FPGA, cancellation or END behavior is
changed. The existing vector allocation and traversal remain; the getter
can wait for the existing capture-data critical section. This is not a
claim that every segment-container API is generally thread-safe.

The retained before-regression failures on Linux and Windows and the
original Windows `EINVAL` throw remain failures. With the correction,
all 12 `CrashRecovery` cases passed on each OS. The exact Windows GUI
passed twelve Demo acquisitions in four Stop / external-SIGTERM-cancel /
finite cycles, with the finite Run at least 61.5 seconds after the preceding
cancel END. The exact Linux GUI additionally passed native Stop
(13,052,108 LOGIC bytes), external controller SIGTERM followed by guarded
Stop (16,019,868 bytes), and a finite 20,000,000-byte capture after the same
61.5-second gate. The late Stop after each finite END was refused without
a click. Both actual desktop processes closed with native exit zero.
An earlier Linux preflight stopped before any Run because the desktop was
locked; its failed preflight is preserved separately from the resumed GUI's
native exit and successful Demo results.

A separate fixed hardware cohort then used exactly three acquisitions,
with zero retries and one Windows boot: Linux control, Windows, Linux
return. The plan was amended from 25 to 50 million samples **before any
physical I/O**, because the native GUI exposes fixed sample-count presets.
All three requested 50 million samples at 5 MS/s: finite Stream mode,
32 channels, 1.6 V threshold, one capture, no trigger and no stream windows.
The DLA remained `1a86:5537`, USB path `3-2`, negotiated 5,000 Mb/s. Linux
ran in VMware; Windows ran in its nested KVM guest using the WCH driver.
Only one OS owned the DLA at a time. There was no reset, hub power cycle,
rewiring or firmware change. The existing source v8 remained D0 = 1 MHz
and D22 = ordered CRC UART, with 10 Hz commands during acquisition.

| Physical phase | Native LOGIC bytes | D0 bad periods | Consecutive CRC messages | Independently STOP-checked UART bytes |
| --- | ---: | ---: | --- | ---: |
| Linux control | 200,000,000 | 0 | 99: 653101–653199 | 2,772 |
| Windows | 200,000,000 | 0 | 100: 654101–654200 | 2,800 |
| Linux return | 200,000,000 | 0 | 99: 655101–655199 | 2,772 |

The acceptance criterion was fixed at 100 ± 1 CRC messages before the runs.
D0 periods were 4–6 samples on all three captures. Both independent UART
decoders agreed on every STOP-checked byte, with no unchecked EOF frame.
The observed message-start interval ranges were 95.8832–102.1978 ms on
Linux control and 95.3706–103.6762 ms on Windows; these observations did
not retune the acceptance criteria.

Each acquisition retained 200,278,016 decoder-input RAW bytes, including
278,016 bytes beyond the accepted LOGIC length. Original-order kernel
payload accounting matched all RAW bytes; on Windows it also accounted
for the separate 1,048,576-byte worker-unconsumed payload, for a total of
201,326,592 bytes. An independent RAW-to-LOGIC implementation matched
every byte of the native 200 MB export on all 32 channels. The surplus
and the worker-unconsumed data were retained separately, not silently
used to repair or extend the accepted capture. D0 and D22 provide the
stated temporal witnesses; this does not establish arbitrary electrical
continuity on every channel.

The source serial port stayed owned until current native END and finalized
kernel quiet/ownership checks both passed. Windows source commands began
only after observation of the original complete kernel Stop/setup/ARM
prefix and successful ARM completion, checked against the predeclared
packet hashes. HEADER, a timer or the WIRE ARM log did not authorize it.
The unchanged Windows core's two-second Stream retirement hold is not a
device-ready acknowledgment. These captures used WIRE tracing for retained
RAW and the legacy native END observer; they do not qualify the separate
controller without WIRE tracing.

Native Save, Open and Save As were checked on the same retained capture
on each platform. Every qualified export and re-export contained 48 chunks
and exactly 200,000,000 LOGIC bytes, with byte-identical data and metadata.
The Open log had one current HEADER and END and no new DLA Run. All three
physical GUI processes closed with native exit zero through their retained
original process owner/handle. All 34 Windows guest originals were copied
and hash-verified before normal DLA detach and VM shutdown. The source and
USB owners were then verified free. Installed binaries were left unchanged.

**Windows export failure retained:** the first Windows Save reported
`Renaming temporary file failed: Permission denied` and left a valid but
partial ZIP containing 117,440,512 LOGIC bytes. A private lab observer was
opening that ZIP while Save was still active. A controlled Win32 test on
copies confirmed that an ordinary ZIP reader can block replacement rename
with error 5; the precise handle responsible for the original failure was
not traced. A GUI diagnostic using an already-existing copied destination
failed differently (`File already exists`), so it is not a clean reproduction
of the initial rename failure. Both failures and their partial files remain
preserved. A distinct Save As to a fresh destination, with no reader until
the native progress dialog closed, produced the complete verified 200 MB
export and successful native Open/Save As round trip. No acquisition was
repeated and the partial original was not overwritten or counted as a
successful Save. A separate live-log .NET reader also refused sharing;
the frozen original log was independently checked after native GUI exit.
The samplerate mutex change does not claim to fix these file-access cases.

The exact qualified GUI executables are:

| Platform | SHA256 |
| --- | --- |
| Linux | `e4414dad2e80e1162f0b2cd5f55741be4dc761e90e47961d6a7c6c39141d248d` |
| Windows | `51b1e1cc42272626f186c6d38b44e321e66ff56a54c52f4a9a09f4c8405b65e5` |

Loaded core/C++ wrapper/decoder modules and the Windows WCH module matched
their pinned hashes. All 416 candidate source files remained unchanged
from the builds through final review. The primary working tree's 1,248-file
baseline, the private core's 477-file source manifest, and the six installed
Windows files remained unchanged. Review covered the shared recursive lock,
RAII exits, callback/join lifetime, allocation cost, cancellation and retained
negative cases. Private captures, vendor binaries, VM material and credentials
are not part of the source publication.

This follow-up supersedes the pending Linux/physical qualification status
in the historical entry below. It qualifies this samplerate snapshot fix
and the stated 5 MS/s controls. It does **not** resolve AUD006, the preserved
Windows 50 MS/s continuity/replay failure, or the pending physical controller
without WIRE tracing. The installed GUI executables have not been replaced.

## 6 November update: the Demo restart throw is localized; the snapshot candidate remains unqualified

The restart failure reported below was subsequently captured in the original
private Windows GUI executable. No DLA was assigned to the guest and the
serial source was never opened. Three initial Demo acquisitions under the
debugger completed without reproducing the throw; that observation did not
reclassify the earlier failure. A separate bounded soak on the same running
GUI completed its first Stop and cancellation, then stopped at the throw on
the following finite Run. All later cycles were cancelled, with no retry.

The captured GUI-thread stack, before throwing, was:

```text
std::__throw_system_error(int)
pv::data::LogicSegment::~LogicSegment()
shared_ptr release
pv::Session::get_samplerate() const
pv::views::trace::View::update_hover_point()
pv::views::trace::View::eventFilter(QObject*, QEvent*)
```

The error argument was 22 (`EINVAL`). Disassembly of the same executable
shows the destructor's `pthread_mutex_lock` failure leading to that throw.
The capture thread was in `sr_session_run`, reached from
`Session::sample_thread_proc`. After inspection, only the paused acquisition
was resumed: the GUI aborted with native exit `0xC0000409`, verified through
the original retained process handle. The function names came from the
executable's COFF symbols; source-line debug information was not available.
Raw register and heap observations are retained without asserting an
unverified object/control-block layout or a firmware cause.

The source exposes concurrent container access on this path:
`get_samplerate()` copies each signal's segment container without
`data_mutex_`, while the acquisition worker clears the previous containers
under that mutex. A private candidate adds a guard using the existing
recursive mutex around the getter. It keeps the segment destructor, data,
return values and capture/Stop behavior. There is no catch hiding the
exception, samplerate cache, new option, transport change or reset.
The existing traversal and vector allocation remain; the read now waits
for the same critical section used by capture replacement and append.

A new regression repeatedly starts a real 32-channel Demo capture while
the main thread reads the samplerate and processes events. It requests 64
captures, alternates 200 kS/s and 1 MS/s, checks allowed old/new/fallback
rates during replacement, joins each capture, and verifies the final rate,
1,024 samples and absence of a reported capture error. The before/after
executables use the same new test object; only the Session object differs.
The old Session version failed on both systems: Linux exited with signal 11
and a heap-corruption diagnostic; Windows exited with `0xC0000005`.
These are preserved memory-corruption failures, not a claim that the
original `EINVAL` throw is deterministically reproduced on every run.
The candidate passed all 12 `CrashRecovery` cases on Linux and Windows,
including completion of the 64-restart case on each. Tests ran serially.

The exact candidate Windows GUI then passed a separate, fixed 12-Demo
full-desktop cohort on one process, without a debugger or WIRE tracing:
four cycles of native Stop after fresh LOGIC data, external controller
SIGTERM followed by guarded native Stop and current END, and a finite
5-million-sample capture at 1 MS/s. Each finite Run was requested at least
61.5 seconds after host observation of the preceding cancellation's END,
and each delivered exactly 20,000,000 LOGIC bytes. Each late Stop request
was refused without a click. Every acquisition had one current HEADER and
one current END; the GUI stayed alive throughout all twelve acquisitions.
It subsequently closed with native exit zero. All 37 guest originals were
copied and SHA256 verified before normal VM shutdown. The six existing
Windows installed-file hashes remained unchanged.

The candidate GUI SHA256 values are:

| Platform | Exact GUI SHA256 |
| --- | --- |
| Linux | `e4414dad2e80e1162f0b2cd5f55741be4dc761e90e47961d6a7c6c39141d248d` |
| Windows | `51b1e1cc42272626f186c6d38b44e321e66ff56a54c52f4a9a09f4c8405b65e5` |

Windows ran in the nested KVM guest; Linux runs in VMware. Loaded Windows
GUI/core/C++ wrapper/decoder module hashes matched their pinned values.
The original GUI's throw and both before-regression failures remain failures.
The successful candidate Demo cohort does not qualify a physical capture.

**The candidate is not released or installed.** Full-desktop Linux GUI
validation and a separately bounded Linux/Windows/Linux-return physical
cohort on the exact candidate binaries are still pending. The reserved
physical controls are finite Stream captures at 5 MS/s, with unchanged D0
and D22 sources, native Save/replay, independent RAW-to-LOGIC comparison,
original kernel order/accounting, temporal witnesses and verified END,
quiet cleanup and close. No physical capture or source I/O was performed
for this diagnosis and candidate validation. This report update changes
no product source or installed binary. It does not resolve AUD-006,
qualify the pending no-WIRE physical controller, or claim arbitrary
32-channel electrical continuity.

## 6 November: Windows no-WIRE preparation is refused after a Demo restart crash

Preparation for the next AUD-002/AUD-006 Windows comparison exposed a GUI
restart failure before any DLA use. This cohort is **not qualified**: zero
physical captures, zero source I/O, one Windows boot without DLA assignment,
and exactly three distinct planned Demo acquisitions with no retry. The
preceding Linux 50 Hz control remains qualified in its stated scope; it does
not substitute for the failed Windows preparation. Product code, firmware,
wiring and installed binaries were not changed.

The old private GUI observer depended on optional WIRE ARM/RAW/sidecar
records. That dependence was reproduced on preserved Windows originals,
including an explicitly derived log with conditional diagnostics removed.
The original Linux kernel guard also correctly refused WCH's different
geometry. Neither old tool was used to start a physical no-WIRE capture.

New private lab checks separate three facts: successful original kernel ARM
completion, current native GUI END/worker retirement, and independent WCH
kernel ownership/quiet. The live ARM prefix reader requires the complete
original successful Stop/setup/ARM command sequence, valid command CRCs,
pinned packet hashes and device identity. A partial record, elapsed time,
GUI HEADER or observer exit cannot assert ARM. The WCH kernel check requires
serial 1 MiB reads, complete original-order payload accounting, accepted Stop,
closed ownership and two final sequential empty reads submitted after Stop
completion. SDK retirement is checked separately in the unconditional native
WCH terminal record; host empty reads alone are not an SDK ready ACK.

The revised native log observer emits `NATIVE_END`, never ARM or permission
to release a physical source. Release requires both current native END and
the independently reviewed WCH kernel proof. It no longer stats optional
RAW/sidecar files. Data qualification and native Save/replay remain separate.
The host physical controller is not yet integrated or hardware qualified.

Offline checks passed 43 WCH/END contracts, 27 original ARM-prefix contracts
and 20 incremental log contracts. Both the preserved Windows 25 MS/s positive
and 50 MS/s electrical-negative captures have valid completion proofs; their
data verdicts are unchanged. Additional checks reject concurrent WCH reads
and missing END from the new real crash. Initial offline fixture/expected
refusal mismatches are retained; they did not cause hardware retries.

The real Windows tests used the same private, uninstalled GUI `705a2fc1`,
core `bf0a6c78`, C++ binding `a1fc7442` and decoder `628767ce`, in Session 1.
The unchanged C# launcher `17d809c7` retained the original CreateProcess
handle and native stdout/stderr files. WIRE was absent; no WCH DLL was loaded
for Demo. QEMU retained NoNewPrivs=1/Seccomp=2 and had no DLA peripheral;
Windows also reported no present DLA. Linux is in VMware, Windows in nested
KVM. The existing private credential opened the actual desktop without a
password change or a request for personal intervention.

| Planned Demo case | Observed result |
| --- | --- |
| Native Stop, 10 million samples at 200 kHz maximum | Stop was sent after fresh LOGIC and a verified Stop glyph. The initial observer then refused the valid 1,146,916-byte stopped capture because it incorrectly required a multiple of 32 bytes in Demo. This initial live test remains failed. |
| SIGTERM cancellation, same finite Demo configuration | After the contract correction, external SIGTERM followed actual FLOW. The controller remained alive, sent a guarded native Stop and observed one current HEADER/END with 16,721,972 bytes. This distinct live case passes; the GUI remained alive. |
| Finite 5 million samples at 1 MHz, expected 20,000,000 bytes, then late-Stop refusal | On the same GUI process, after changing the finite count/rate, the next Run produced one new HEADER, zero LOGIC bytes and no END. The GUI terminated. Neither the finite result nor late-Stop check passed. |

The minimal observer correction is specific to Demo: its 32-channel LOGIC
unit is 4 bytes and native Stop may end at any sample count. The pinned DLA
32-byte RAW-frame requirement is unchanged. The corrected Demo rule was
checked against the retained first stopped capture and four alignment cases,
then exercised live by the cancellation case. That offline derivation does
not turn the original live refusal into a passed test, and the first capture
was not rerun.

The third attempt's original stderr contains an unhandled `std::system_error`
with `Invalid argument`; the retained native process handle reports exit
`3221226505` / `0xC0000409`. Windows Application Error/Reporting events 1000
and 1001 agree on the process and failure. The full stderr suffix after the
third observer's recorded offset independently has one HEADER and no END;
the observer's shorter retained log is an exact prefix of that suffix.
Its previous capture's END was not reused. The guard correctly refused to
report completion or permission to release a physical source.

The WER `Report.wer` was retained; the temporary dump was already unavailable
when checked. There is no symbolized throwing frame or confirmed root cause.
The private GUI's recorded `pv/session.cpp` is byte-identical to the current
preserved product source, but that does not locate the throw or demonstrate
the installed GUI's behavior. No source fix, mutex/thread workaround or
firmware explanation is inferred from this exception message.

All 19 guest original files were copied and verified by size/SHA before
normal VM shutdown. The six installed Windows file hashes are unchanged.
The abnormal native exit is preserved explicitly; shutdown is not reported
as a successful GUI exit. The GUI process and scheduled task had ended,
the test task was removed, and DLA had never been assigned. Windows is off;
the Linux DLA/source remain free with the same identities. All 1,248 local
product files, repository HEAD/status/diffs and Linux installations, plus
477 active runtime source files, remain intact.

Further physical Windows work is cancelled. The next step is a separately
bounded Demo-only diagnosis that obtains a native throwing frame/backtrace
for the restart sequence before another DLA comparison. This preparation
does not resolve AUD-002/AUD-006, qualify Windows no-WIRE acquisition, or pass
the global release gate. Private originals and lab checks are retained under
`artifacts/windows-passive-preparation-20261106/`; captures, crash report,
private helper/launcher and credentials are not published.

## 6 November: a separately defined 50 Hz reference passes the Linux control

This is a new reference profile for AUD-002/AUD-006, defined and checked before
I/O. The preceding 100 Hz cohort remains refused under its original 20 ms
limits. No thresholds or results in that closed cohort were changed. The new
profile reduces the nominal numbered-command rate from 100 Hz to 50 Hz; it
does not change the acquisition rate, firmware, D0 clock, camera/SD setting,
wiring or product code. There were exactly two finite Linux captures, no
physical retry and no Windows boot in this cohort.

The reference has a specific coverage target. The earlier Windows 50 MS/s
repetition had 8,823,808 bytes present in both accepted LOGIC copies: 2,205,952
samples, or 44.119040 ms at the nominal rate. A message start gap of at most
40 ms plus a complete-message envelope of at most 3 ms leaves a full numbered
CRC witness in any contained window of that duration, with capture boundaries
checked separately. This does **not** guarantee a numbered witness in every
1 MiB / 5.243 ms interval or prove arbitrary-channel temporal integrity.

The fixed new limits were 99 ± 3 fresh consecutive CRC messages in the 2 s
capture, GPIO start gaps 2..40 ms with mean 18..22 ms, scoped host write gaps
5..40 ms, full-message envelope at most 3 ms, initial full START within 30 ms
and final full STOP within 20 ms of the capture edge. The envelope includes
the last nominal STOP bit, whose GPIO interval must remain high; it is not
just the DATA annotation span. Twenty-five private offline contracts checked
acceptance, refusals, host scope, full STOP and an independent interval oracle.
The existing 48 Linux kernel/END contracts and their unchanged guard were
reused. The first new offline fixture expected a boundary refusal but first
violated a GPIO gap; that test failure and the corrected fixture are retained.
The real 198-message 100 Hz input is refused by the new profile's count, and
its original 20 ms refusal is also reproduced unchanged.

| Finite Linux capture | Accepted LOGIC / positive kernel bytes | Electrical witnesses | Result |
| --- | --- | --- | --- |
| 50 MS/s, 100 million samples, new 50 Hz reference | 400,000,000 / 402,653,184 | D0: zero bad periods, 49..51 samples, boundary offsets 45/42 samples; 99 consecutive valid CRCs `610101..610199` | New reference profile passes |
| Reserved 5 MS/s return, 25 million samples, 10 Hz reference | 100,000,000 / 100,663,296 | D0: zero bad periods, 4..6 samples, boundary offsets 5/1 samples; 50 consecutive valid CRCs `611101..611150` | Return passes |

For the new 50 Hz capture, all 98 GPIO start intervals were
16.002860..22.001600 ms, mean 19.998498 ms. The 99 scoped host intervals
(`610101..610200`, captured span plus the available adjacent command) were
16.008803..22.004105 ms, mean 20.012804 ms. All 99 complete-message envelopes
were 2.415960..2.417440 ms. Initial full START / final full STOP coverage was
23.783840 / 13.947080 ms. Maximum GPIO gap plus maximum envelope was
24.419040 ms, below the fixed 44.119040 ms target; a separate integer endpoint
oracle checked every inter-message and capture-boundary interval. These are
nominal sample coordinates, not a clock alignment or emission-latency claim.

Two independent valid-STOP UART decoders agreed on all 2,772 bytes at 50 MS/s
and all 1,400 bytes on the return; neither had an unchecked EOF frame or
interior warning. There were 117 live host commands `610101..610217` and 52
return commands `611101..611152`, plus three fresh preflight commands per
phase. Every command had exactly one in-order ACK. Host commands and ACKs
are not electrical emission counts. Native ARM-return to last LOGIC was
2.281032 s / 5.037345 s respectively; this is not isolated CPU timing or a
causal performance comparison.

The exact same helper `db7c255b`, active Linux core `84c0c7da`, actual Segment
source `b03fa259`, DBG4, 32 channels, 1.6 V and finite Stream/no-trigger mode
were used. Optional `FNIRSI_DLA32_WIRE_TRACE` remained absent, verified in the
child environment. Passive usbmon, exact-feed logging, Segment storage and
ordinary debug logging remained active. All four original USB command packet
hashes per phase match the preceding cohort. Independent NumPy conversion of
the original-order kernel prefix equals every accepted feed/Segment byte on
all 32 channels. The 2,653,184 / 663,296 surplus active kernel bytes are kept
in explicitly derived RAW files and are never appended to accepted LOGIC;
there are zero positive pre-ARM/post-Stop bytes. Without native RAW tracing,
the accepted-prefix boundary is not asserted to be the native consumption
boundary.

Both captures completed with exact requested counts, one native HEADER/END,
no reported error/watchdog, cleanup and exit 0. The source owner remained
open through real native END/exit, observer closure and independent kernel
proof: successful Stop, all requests closed and two sequential empty reads
submitted after Stop completion. Only then were postflight STATUS/ACK and
source close allowed. Timer or observer exit alone cannot release the source;
missing proof leaves the controller in an explicit hold. This quiet policy
is specific to the reviewed Linux transport, not a WCH/Windows ready ACK.

Linux is in VMware; Windows, unused here, is nested KVM. DLA `1a86:5537` was
USB `3-2`, bus 3/address 3 at 5,000 Mb/s. The source was CH340 `1a86:7523`,
revision `8134`, USB `2-1.3`, bus 2/address 9, `/dev/ttyUSB1` at 12 Mb/s;
both identities and speeds were recorded before each phase and afterward.
Source v8 / 1 MHz D0 / 50% duty / camera=1 / SD=0 was confirmed by STATUS;
DLA firmware revision was not interrogated or changed. Both devices are free
and Windows is off. All 1,248 product files, repository HEAD/status/diffs,
installed Linux binaries and 477 active runtime source files remain intact.
The active runtime API is still distinct from the preserved repository API.

The new Linux reference and its reserved return are qualified for this
bounded setup. No Windows result, root cause, global 50 MS/s fix or release
qualification follows. AUD-002/AUD-006 and the earlier negative captures
remain open. A Windows comparison needs a separate plan and a review of its
actual GUI/worker/END observation path before hardware use. Private evidence
is under `artifacts/reference50Hz-linux-20261106/`; no private capture,
firmware, helper binary, VM or authentication material is published.

## 6 November: bounded Linux control without synchronous WIRE tracing

A new cohort performed **two Linux acquisitions, zero retries and zero Windows
boots**. The only configured intervention was leaving the existing optional
`FNIRSI_DLA32_WIRE_TRACE` unset. The exact same helper `db7c255b`, installed core
`84c0c7da`, Segment, finite Stream geometry, 32 channels, 1.6 V, source firmware,
wiring and command cadence were retained. The helper still used DBG4. Disabling
that option removes both synchronous RAW-file writes and conditional WIRE debug
messages together; it is not an isolated measurement of either cost, or a
product change. Passive usbmon, exact-feed logging, Segment storage and ordinary
session debug logging remained enabled.

The 50 MS/s capture has a **positive bounded D0/CRC/storage result**, while its
**overall dense-input qualification is refused**. The original 20 ms pacing
limits were exceeded by one host interval and one measured UART interval. The
thresholds and refusal are preserved; the capture was not retried, and Windows
was not started. The one reserved 5 MS/s return passed. Earlier negative results
remain negative; this run does not resolve AUD-002/AUD-006 or establish a general
high-rate guarantee.

Linux was Manjaro 7.2.9-1-MANJARO inside VMware; DLA `1a86:5537` stayed at
`3-2`, bus/address `3/3`, negotiated 5 Gb/s. CH340 source `bcdDevice=8134` stayed
at `2-1.3`. Source STATUS confirmed unchanged v8, D0 1 MHz/50%, camera enabled
and SD disabled. DTR/RTS remained false. There was no reset, hub cycle, flash,
firmware update, wiring change or Windows credential access. The DLA firmware
revision was not interrogated. Both devices were free before and after phases.

The private Linux END/accounting guard was qualified **before physical I/O**:
48 contracts cover actual retained kernel inputs, metadata/lifecycle errors,
missing or failed Stop, unmatched/duplicated requests, missing quiet reads,
observer loss, reordered/truncated payloads and corrupted storage. A separate
bit-by-bit oracle validates the compact conversion fixture. Kernel-prefix
conversion also matches the original feed and actual Segment for the retained
400 MB negative capture and 100 MB positive return; recognizing their clean END
does not turn the earlier electrical failure into a pass. The legacy guard's
dependency on optional WIRE log lines is reproduced. An initial 47-contract run
is retained; the final version additionally accepts a clean END with a shorter
saved prefix after a reported storage failure, while refusing complete-data
qualification for it. No product code or executable was changed for this guard.

During each new run, native HEADER/END/Stopped, exit and cleanup are checked
while the source remains open. Only after actual native END does the passive
observer close; independent parsing then verifies successful Stop, all kernel
requests closed, and the last two sequential, empty sample reads submitted
after Stop completion. These are bounded host quietness observations, not a
device-ready ACK. The source is closed only after that proof and postflight
STATUS/ACK verification. A missing END/quiet proof holds the source controller
alive; elapsed time, observer exit and cancellation alone cannot release it.

Without a native WIRE file, the original passive kernel trace supplies the RAW
reference. Every positive payload remains in its original category and order.
The reference prefix corresponding to each accepted LOGIC byte is converted
independently and compared with both exact feed and actual Segment storage.
Derived `kernel-pre/active/post` files are explicitly labeled; surplus bytes
remain outside accepted samples. No native callback timestamps or decoded-input
tail boundary are invented when conditional tracing is absent.

| Phase | Accepted LOGIC | Original positive kernel bytes | Witness result |
| --- | --- | --- | --- |
| 50 MS/s, 100M samples | 400,000,000 B | 402,653,184 B | D0 periods 49–51, zero anomalous periods, boundaries 41/45 samples; 198 consecutive valid CRC messages `600101..600298`, both STOP-checked UART decoders equal |
| Reserved 5 MS/s, 25M samples | 100,000,000 B | 100,663,296 B | D0 periods 4–6, zero anomalous periods, boundaries 1/0 samples; 50 consecutive valid CRC messages `601101..601150`, both decoders equal; overall return passes |

Both natives report complete/exact count, no error/watchdog, one HEADER/END,
cleanup and exit 0. Independent all32 kernel-prefix/feed/Segment comparison is
exact. Pre-ARM and post-Stop positive payloads are zero in both phases. The
active surplus beyond accepted LOGIC is 2,653,184 B for 50 MS/s and 663,296 B for
the return, retained separately from accepted samples. Without the native WIRE
trace these are not asserted to be precise native-decoder/unconsumed boundaries.
The UART decoders agree on all 5,544 and 1,400 valid-STOP bytes respectively,
with no unchecked EOF frame. This verifies storage conversion on all channels
and electrical witnesses on D0/D22, not arbitrary32 temporal integrity.

The dense source qualification remains **false**. Within the captured-ID host
window plus its available adjacent command (`600101..600299`), host write-start
gaps are 5.335511–20.177397 ms, mean 10.018137 ms. The `600241 -> 600242` gap
exceeds the predefined 20 ms ceiling; the preceding write/flush itself took
0.194726 ms. In accepted sample coordinates, UART message-start gaps are
5.332940–20.222900 ms, mean 10.013705 ms; that maximum also exceeds the original
20 ms ceiling. First/final UART data bounds are 23.227100/1.673380 ms. All 236
live commands `600101..600336` plus three preflight commands were acknowledged
once in order. All source commands, failures and timing outliers remain
retained. Host writes/ACKs and nominal sample coordinates do not align an
electrical emission clock or prove emission counts. The specific delay is not
isolated to serial reads, disk logging, scheduling or a firmware operation.

The native ARM-return-to-last-LOGIC interval was 2.307998 s for the no-WIRE
50 MS/s run; the preceding traced negative run observed 3.823052 s. These are
whole-path wall observations in separate runs, not isolated CPU/RAW/logger
costs or proof of causality. A historical traced capture also had positive
bounded D0/CRC data under its derived review. The new observation therefore
neither proves that tracing caused all prior faults nor makes 50 MS/s safe in
every configuration. The strict input-cadence refusal is not waived for a
favorable storage/witness result.

Evidence `passive-kernel-control-20261106` retains the pre-I/O plan and unused-ID
check, both offline guard rounds, all original kernel/source/native files,
explicitly derived RAW references, independent conversion/UART reviews, exact
timing violation, real END/kernel quiet/source-close records and final review.
The cohort is closed at two acquisitions/zero retries; it cannot be reopened.
All 1,248 product files, local HEAD/status/diffs, installed Linux hashes and the
477-source active runtime manifest remain preserved. Windows stays off. Only
this report is published. Dense-source, Windows high-rate and global release
qualification remain open; no private guard, controller, capture or commercial
firmware is promoted into the product.

## 6 November: host service boundaries in the retained Linux traces

A separate **read-only comparison** of the fresh negative dense capture and the
historical dense input qualified by the corrected V2 review localizes host-side
delays without another acquisition, native probe process, source command or Windows
boot. The historical R1 original live verdict remains negative; its later derived
qualification is not substituted for the fresh failure. Both inputs used the
same helper `db7c255b`, active core `84c0c7da` and exact current Segment source
`b03fa259`, but came from different boots/USB paths. This is an observational
comparison, not a controlled performance intervention or CPU profile.

The review independently parses every original USB record, validates all
382 accepted 1 MiB callback ordinals/offsets against retained accounting and
checks the eight initial requests plus 381 resubmissions in FIFO callback
order. It retains every packet row. All 38 packets crossing a 10 MiB Segment
boundary are classified by byte count; the other 343 resubmitted packets are
ordinary. The final, non-resubmitted packet and teardown are excluded from the
following service totals.

The driver calls `wire_trace_write`, conversion and `sr_session_send` before
resubmitting each transfer. In this private helper the LOGIC consumer writes the
exact feed and appends to Segment synchronously. Segment growth allocates the
next chunk and a temporary twice-chunk headroom block, touches that temporary
block, then publishes the next chunk. Those source paths are verified, but the
recorded traces do **not** time allocation, feed I/O or Segment independently.

| Recorded host wall-time boundary | Historical derived-positive input | Fresh negative input |
| --- | --- | --- |
| Callback argument evaluation to resubmission, sum of 381 service intervals | 405.533–429.917 ms | 3330.830–3351.404 ms |
| RAW-payload log timestamp to LOGIC log timestamp, median of 343 ordinary packets | 0.414 ms | 2.940 ms |
| LOGIC log timestamp to resubmission, median of 38 growth packets | 2.986–3.220 ms | 29.946–30.192 ms |
| LOGIC log timestamp to resubmission, median of 343 ordinary packets | 0.099–0.333 ms | 2.596–2.842 ms |

The ranges conservatively bound clock origins using synchronous-read brackets,
command-submit and callback-completion inequalities, with outward microsecond
rounding. The wire-epoch constraint intersections are 64 µs wide for the old run
and 54 µs for the fresh run. Default log-clock origin uncertainty is retained;
no fit to a large delay is used. The model assumes no inconsistent host realtime
step within either run. The values are host wall time, not CPU time or electrical
sample time. LOGIC-to-resubmit includes the remaining log operation, consumer,
core/libusb work and possible descheduling. Packet completion-to-callback
latencies overlap across queued requests and must not be summed as CPU work.
More than one stage is slower in the fresh run; the table does not establish
that the temporary allocation alone caused loss or justify removing it.

One interval can be narrowed further using the two clocks already present in
each log line. Callback 249 (zero based), RAW offset `261095424`, evaluated its completion-log
argument at wire-relative `2359268 µs`; the default logger's displayed timestamp
is `00:02.910585`. The next RAW-payload argument is `2909829 µs`, giving a
550.561 ms argument-to-payload interval. Of this, **550.205–550.510 ms occurred
before the default logger acquired the timestamp**, with every other accepted
callback's corresponding upper bound at most 0.379 ms. The historical input's
largest such upper bound is 0.405 ms.

The verified default logger calls `fputs("sr: ", stderr)` before acquiring that
timestamp. Thus this particular long interval precedes RAW writing, conversion,
LOGIC delivery, Segment growth, and log-body formatting/flush after the
timestamp. It includes logger dispatch, the initial stderr prefix operation and
possible descheduling. It does not identify a blocked write, FILE lock, scheduler
or VM pause. Measuring only the subsequent RAW/LOGIC timestamps would misplace
this delay in the pipeline.

The time ordering must distinguish this logger interval from the later period
with no pending USB requests. The first two anomalous D0 periods were already
received approximately 505 ms before callback 249 evaluated its argument. The
third anomalous-period bracket spans a completion before that argument and one
5.326–5.380 ms after it; the fourth is in that later completion. All those
completions precede the end of the logger interval. The previously reported
559.303 ms interval **without pending IN requests begins at that later
completion**, after all four fault-containing completions. Therefore the long
logger interval cannot explain the first fault; the evidence does not establish
its causal relationship to the later faults. Kernel receipt times are not
electrical production times. An initial extra check assuming all four faults
preceded the logger argument was correctly rejected and remains preserved; the
final review retains this more precise ordering. The preceding report's
statement about the zero-pending interval remains unchanged and consistent.

Private evidence `host-service-boundaries-20261106` contains the frozen plan and
input hashes, complete per-packet CSVs, source/build checks, both clock-bound
sets, all 382 prefix intervals per input, the initial refused ordering check and
the independent final review. Original capture artifacts, all 1,248 product
files/HEAD/status/diffs and installed Linux hashes are unchanged. Windows remains
off; this Linux-only evidence makes no Windows claim. Only documentation is
published. No workaround, buffering option, allocation policy or product logging
change is promoted. The fresh 50 MS/s failure and the passing 5 MS/s return
remain the latest physical results; AUD-002/AUD-006 and release remain
unqualified.

## 6 November: fresh Linux control rejects before Windows; low-rate return passes

After the DLA reappeared in the Linux guest, a **new, bounded Linux cohort**
performed exactly two acquisitions, with zero physical retries and zero Windows
boots. The fresh 50 MS/s control failed integrity, so the proposed Windows work
was canceled. The one reserved 5 MS/s return passed. Earlier closed cohorts and
their original verdicts remain unchanged; no product fix or release
qualification follows from this run.

The environment was Manjaro Linux 7.2.9-1-MANJARO inside VMware. DLA
`1a86:5537` was at USB `3-2`, bus/address `3/3`, negotiated 5 Gb/s; the separate
CH340 source was identified by `bcdDevice=8134` at `2-1.3`, not a generic serial
alias. Both were free before and after each phase; Windows stayed off. The
unchanged v8 source reported 1 MHz/50% D0, camera enabled and SD disabled. D22
carried numbered, CRC-protected UART messages at nominal 100 Hz for the dense
phase and 10 Hz for the return. The controller kept the source open through
actual END/quiet cleanup/native process exit, then checked STATUS and closed it.
There was no reset, hub cycle, flash, firmware or wiring change; the DLA firmware
revision was not interrogated in this run.

The fixed Linux helper `db7c255b` used the unchanged installed core `84c0c7da`,
32 channels, 1.6 V, finite Stream and no trigger. Independent conversion of the
RAW prefix covering each accepted LOGIC byte matched feed and actual Segment
storage. Kernel parsing
verified original completion/submission order and conservation of every positive
IN payload, including separately quarantined surplus. Native completion and
byte counts therefore describe storage and shutdown, not electrical continuity.

| Phase | Accepted LOGIC / RAW bytes | Electrical result |
| --- | --- | --- |
| Linux 50 MS/s, 100M samples | 400,000,000 / 400,556,032 | **Rejected:** four D0 anomalies, UART frame errors and numbered-message loss/replay |
| Reserved Linux 5 MS/s, 25M samples | 100,000,000 / 100,663,296 | Passed: D0 periods 4–6, initial/final boundaries 3/0 samples, 50 consecutive CRC messages `999101..999150`, independent decoders equal |

Both native captures reported complete, one HEADER/END, quiet cleanup, no error,
exit 0 and source close after END. In the dense phase D0 instead had periods
39, 33, 31 and 47 at samples 51,656,665; 51,656,704; 67,108,833; and 67,108,864
(nominal period 50, permitted deviation one). Both independent UART decoders
recovered the **same 197 valid CRC records but only 168 unique identifiers**:
the forward step is `997201 -> 997270`, then `997299 -> 997233` goes backward.
Identifiers `997270..997298` occur twice; `997202..997232` never appear within
the observed identifier span. Their decoded byte streams differ around damaged
frames; SDK frame-error annotations at samples `51660610..51661045` and
`51674772..51675207` and all malformed material remain retained. Neither the
original strict review nor the preplanned STOP-checked V2 contract passes.
All 388 live host commands `997101..997488`, plus three preflight commands, were
acknowledged once in order. Host writes/ACKs are not measurements of electrical
emission count or exact source timing.

A separate read-only review used the already specified displacement **128 MiB**,
32-byte RAW frames and retained every maximal equal run of at least 1 MiB.
It found exactly one such run:

```
RAW[206626816, 268435456) == RAW[340844544, 402653184)
length: 61,808,640 bytes; displacement: 134,217,728 bytes
SHA256 of each range:
24f5f247ca8d5c7f0b91dfb37dd51fc45b661aad03772fc9595861d4f686e849
```

Of those bytes, **59,155,456 have both copies in accepted LOGIC storage**. The
remainder of the second range is surplus RAW plus a separate 2,097,152-byte
unconsumed-active kernel payload; it is a virtual read-only extension, never
appended to accepted samples. The range ends at the limit of the retained
positive stream, not a proven end of a longer repetition. The preceding full
32-byte frame differs, with eight trailing bytes equal. The first full-frame
boundary falls in the first two anomalous D0 brackets. There are **29 complete
same-numbered CRC pairs**, `997270..997298`, separated by 33,554,432 samples.
A separate `struct/mmap/hashlib` parser verifies both selected range digests
directly in the original 384 positive kernel IN completions, totaling
402,653,184 bytes (796 records/398 closed URBs). The repeated bytes precede
conversion, storage and derived-file analysis.

Kernel request accounting observed maximum depth eight, a 3.748832 s
ARM-to-last-accepted-RAW window and 1.455928 s without pending IN requests in
that window. Its longest such interval was 559.303 ms, **after the completions
already containing all four D0 anomalies**. No deliberate read pause was added.
These host request intervals do not align electrical sample time and do not
isolate observer/storage/scheduling cost or the internal loss/replay mechanism.
The 128 MiB equality does not determine physical copy direction, buffer capacity,
source/FPGA/bridge/VMware cause, or integrity of arbitrary signals on all channels.

The initial preparation refusal is preserved: after the reboot `usbmon` was
absent, so the passive reader failed before ARM or any witness command. Loading
the existing kernel module repaired that prerequisite before the first physical
attempt. A later offline analysis initially assumed the Windows sidecar filename;
that failed attempt and script remain retained, and only the derived analysis
was corrected to the preserved Linux sidecar. No acquisition, native helper or
SDK process was repeated for these corrections.

Private evidence is retained in `fresh-linux-control-20261106`: the fixed plan,
initial preflight and repair, original manifests, both capture/END/source/kernel
records, strict and V2 refusals, low-rate return, anomaly review, fixed-displacement
review, independent kernel digests and final review. All 1,248 product files,
HEAD/status/diffs and Linux installed hashes remain unchanged; all 408 frontend
sources match the public base. Only this report is published. AUD-002/AUD-006,
Windows 50 MS/s and the global release gate remain open/unqualified. This cohort
is closed; it cannot be retried or used as a passed dense-source qualification.

## Isolated OR/PW evaluator checked on Windows and Linux — 6 November 2026

AUD-004 now has a bounded Windows **file-replay** check of the current
software-trigger evaluator, paired with a fresh Linux run. All seven fixed
configurations preserve every expected output byte, window origin, trigger
offset, complete/partial status and length on both platforms. This closes an
isolated host-test gap; it does not qualify USB acquisition, the GUI, concurrent
decoders, 64 conditions or a sustained device rate. AUD002/AUD006 and the earlier
Windows 50 MS/s continuity failures remain open.

The input is the retained, normalized 32-channel Linux/VMware recording from
the earlier 50 MS/s checks: 20,000,000 bytes / 5,000,000 samples, SHA256
`b98c24075c7127668c3b69a8a5489c49549ba4e7efed284235084441a5658e70`.
Its prior D0/D22 witnesses are historical evidence, not a new electrical test.
The exact current `softwaretrigger.hpp` and `streamwindows.hpp` are frozen,
hashed and compiled directly into a private standalone helper. No acquisition
library or Qt is linked. The private harness combines the seven previously
measured modes and uses native CPU clocks; no product code or installed binary
is changed.

Before execution, the plan fixed zero physical acquisitions/source I/O, one
Windows boot without DLA assignment, seven evaluator processes per OS and zero
retries. Each process makes one verification pass and three timing trials;
each trial processes the same 5 M-sample input 100 times, resetting the engine
between iterations: 500 M samples per trial, 10.5 billion replayed samples per
OS across the seven modes. These are repeated in-memory samples, not newly
captured or continuously acquired data. Configuration is unit=4, depth=2048,
pre=1024, target=10000, nominal rate=50 MS/s and 1 MiB packets.

The stage is compiled once before measurement. Timing includes evaluator
construction, matching, allocation and output-window copying; the timed
callback counts frames/bytes and a byte checksum. Input loading, verification
file output and timing-log writes are outside the measured interval. A NumPy
oracle independently derives GPIO transitions, pulse-run lengths and window
scheduling, then compares every emitted byte against the original input slice.

| Configuration | Complete + partial windows per OS | Linux M samples / process-CPU second | Windows M samples / process-CPU second |
| --- | ---: | ---: | ---: |
| Falling D0 | 4761 + 1 | 2044.3–2245.0 | 1684.2–1777.8 |
| OR, D0 HIGH 400–600 ns plus impossible HIGH >100 µs | 4761 + 1 | 129.4–131.9 | 115.5–120.3 |
| OR, D0 LOW 400–600 ns plus impossible HIGH >100 µs | 4761 + 1 | 132.4–134.7 | 118.5–119.4 |
| OR, D0 HIGH/LOW >100 µs | 0 | 81.6–82.0 | 83.8–85.3 |
| AND, D0 HIGH 400–600 ns and HIGH >100 µs | 0 | 81.3–82.6 | 85.6–86.0 |
| OR, D0 HIGH >100 µs or D22 HIGH >10 s | 0 | 76.1–76.6 | 77.7–79.2 |
| AND, D0 HIGH 400–600 ns and D22 HIGH >10 s | 0 | 77.3–77.9 | 80.0–81.0 |

Ranges are the three observed trials. Linux uses `CLOCK_PROCESS_CPUTIME_ID`;
Windows uses `GetProcessTimes` kernel+user, represented in 100 ns units. All
21 Windows CPU durations are integer multiples of **15.625 ms** in this run.
That observed quantization especially limits precision for the fast falling
case; it is not a claim about every Windows timer or installation. Steady-clock
wall intervals are retained separately. Linux runs in VMware with GCC 16.2.1;
Windows 11 runs in nested KVM with MSYS2 GCC 16.2.0, noninteractive Session 0.
The OS tests run sequentially. These virtualized observations are not an OS
ranking or a bare-metal benchmark.

Per OS, the verification pass preserves 14,283 complete windows and three
partial windows, totaling **117,019,240 output bytes**. All payload hashes agree
between OSs. Metadata fields agree; native CSV line endings are preserved.
All 14 evaluator processes exit natively with code 0, all 42 timing trials
process their full input, and timing frame/byte/checksum counts match the
independent verification. The four negative configurations emit no invented
windows. No measured case here is below 50 M samples per process-CPU second,
but the remaining acquisition/frontend costs are excluded, so this is not a
50 MS/s operating guarantee or a reason to infer the cause of earlier faults.
These seven profiles do not justify a product optimization.

The first Linux extraction of the Windows archive **failed verification**:
Windows `Compress-Archive` wrote backslash entry names and the initial
`ZipFile.extractall` preserved literal backslashes on Linux. The native Windows
job itself had completed all seven modes successfully. The original archive,
initial extraction and supervisor failure remain retained. A separate derived
extraction normalizes relative path separators, rejects duplicate/traversal
paths and verifies every original byte, size and hash directly against the
unchanged archive and its 53-file manifest. No native helper was rerun. Both
original refusal and corrected offline copy verification are explicit.

All 53 Windows originals are copied/hash-verified before normal VM shutdown;
native process HANDLEs provide actual exit codes. The six installed Windows
file hashes match both pre/postflight and the earlier verified runtime. Final
review confirms Windows off, DLA still absent, all 1248 product files and local
HEAD/status/diffs plus Linux installed hashes unchanged. The cohort is closed
at 14 evaluator processes, zero retries, zero physical acquisitions/source I/O
and one Windows boot. DLA reconnection remains necessary for the next fresh
Linux control and bounded physical Windows plan.

Private evidence: `trigger-replay-windows-20261106` contains the fixed plan,
source/input/header hashes, compiler flags and binaries, original native logs,
payloads, independent oracle results, archive-layout refusal/recovery, copy
manifest, shutdown, final review and cohort closure. Only this report is
published. Product rollout, Windows50M capture and global release qualification
remain false; no private helper, capture, SDK, VM disk or credential is published.

## Dense Linux witness: original refusal retained, private review corrected — 6 November 2026

The dense-witness cohort on 4 November stopped after its first qualification
failure. It is **closed at two physical Linux acquisitions, zero retries and
zero Windows boots**: one dense 50 MS/s acquisition and the reserved 5 MS/s
return. The planned Windows acquisition was cancelled. A subsequent, separately
specified offline review qualifies the retained Linux input under a corrected
private comparison contract. **This is a derived qualification, not a pass of
the original live plan or a product fix.** The original failed reviews and all
raw inputs remain unchanged.

The source, wiring and product were unchanged: source STATUS v8,
1 MHz/50% PWM, camera=1, sd=0; 32 channels, 1.6 V, finite Stream, no trigger or
Stream windows, Captures=1. Linux runs in VMware; the planned Windows test is
in nested KVM. At acquisition, DLA `1a86:5537` was at USB 4-2/dev16, negotiated
5000 Mb/s; the bcdDevice=8134 CH340 source was at 3-1.2. The exact source
firmware is unavailable here; STATUS and command ACKs do not establish the
number or latency of electrical emissions.

| Retained phase | LOGIC / RAW bytes | Numbered CRC messages | Original verdict |
| --- | --- | --- | --- |
| Linux dense, 50 MS/s, 100 M samples, 10 ms commands | 400,000,000 / 400,556,032 | 197 consecutive, 992101–992297 | Qualification refused |
| Reserved Linux return, 5 MS/s, 25 M samples, 100 ms commands | 100,000,000 / 100,663,296 | 50 consecutive, 994101–994150 | Passed |

Both acquisitions preserve independent RAW conversion equal to the entire
accepted LOGIC feed and actual Segment, and exact kernel completion order and
byte conservation: 796 and 224 kernel records, respectively. D0 has no rejected
periods: 49–51 samples with initial/final fragments 43/32 at 50 MS/s, and
4–6 with fragments 4/1 at 5 MS/s. Both have one header and real END, native
exit 0, complete metadata with empty error, quiet cleanup and source closure
after END. These storage checks and D0/D22 witnesses do not certify arbitrary
electrical integrity on all 32 channels.

The original dense refusal exposed two private reviewer assumptions:

* **DATA annotation is not a validated STOP.** The independent decoder retained
  5542 bytes, whereas the SDK UART decoder emitted 5543 DATA annotations. The
  first 5542 bytes are identical. The extra `0x32` belongs to the final unfinished
  UART frame: START edge at sample 99,996,249, expected STOP sample
  100,000,371–100,000,372, beyond the 100,000,000-sample input. The SDK emits DATA
  before it samples STOP. The corrected private checker compares bytes only
  after an independently observed valid STOP, retains every SDK annotation and
  the unchecked EOF frame, and excludes that frame from complete CRC messages.
  It rejects warnings, interior unfinished frames, byte disagreement and an
  alleged EOF frame whose STOP should have been observable. No captured sample,
  byte, annotation or decoder implementation was changed.
* **Host pacing was checked beyond the observed identifier span.** All 232 live
  commands 992101–992332 were distinct, ordered and ACKed. The original
  all-command check retained a 34.677226 ms host write-start gap between
  992324 and 992325. Neither identifier is in the observed 992101–992297 span.
  The revised contract checks that span plus one available adjacent command at
  each boundary, here 992101–992298, while retaining and reporting every outside
  gap. This identifies scope by command number; it does not align the host
  clock to electrical capture time or measure UART emission latency.

Under this explicit revised contract, both decoders agree on 5542
STOP-checked bytes, including the retained incomplete textual suffix, and all
197 complete CRC messages remain fresh, known and consecutive. GPIO message
start gaps are 5.5049–13.6315 ms, mean 10.016528 ms; corresponding host write-start
gaps are 5.498378–13.607653 ms, mean 10.020963 ms. The first and last UART DATA
boundaries are 23.44996 ms from the beginning and 10.91184 ms from the end.
The original numerical thresholds remain unchanged: 195–201 complete messages,
GPIO gaps 2–20 ms/mean 9–11 ms, host gaps 5–20 ms, and boundary coverage
30/20 ms. The changed contracts and original refusal are recorded separately.

Private offline qualification covers nine UART cases (six actual SDK software
fixtures and three derived annotation faults) and ten host-cadence cases.
It rejects interior loss, repeated or unknown identifiers, blind boundaries,
in-span or adjacent host gaps, bad STOP and missing interior STOP. The original
all-command refusal is reproduced. A single additional SDK decode of the
original Linux input supplies START/DATA/STOP annotations. None of these
offline processes opens the source or a capture device. The separate
source-only batch of 100 commands, all ACKed, remains control-port evidence
only; it is not itself electrical cadence qualification.

On resumption on 6 November, the prepared separate Windows-dense plan had
started **zero acquisitions and zero VM boots**. Fresh read-only enumeration
found no `1a86:5537`; the visible `77a1:01a2` is Kingst. USB paths have changed,
so the old identity preflight cannot authorize acquisition. The owner was
asked to reconnect DLA through the private channel. No hub cycle, reset,
firmware change or source I/O was performed during this resume. Windows dense
cadence and continuity remain untested; the earlier Windows 50 MS/s negative,
partial RAW repetition and AUD002/AUD006 remain unresolved. Windows50M and
release qualification remain false.

Evidence is retained privately under `dense-witness-20261104` (original plan,
source-only/physical runs, failed reviews, closure and original manifest),
`dense-witness-review-v2-20261104` (fixed offline plan, fixtures, stop proof and
explicit derived qualification), and `dense-witness-report-20261106` (fresh
inventory, frozen-input verification and publication review). The prepared
`dense-witness-v2-20261104` physical tools were not executed. Final review
rechecks all 1248 product files, local HEAD/status/diffs and Linux installed
hashes. Windows installed files were last checked in the prior completed
cohort; this offline resume does not claim a new Windows runtime check.
Only this report is published; the private checker, captures, commercial
firmware/SDK, VM disks and credentials are not publication material.

## Update: offline128MiB overlap localization in the native50M negative — 4 November 2026

A fixed offline follow-up finds a **smaller exact RAW equality interval** in the
preceding native-output Windows50M negative. The earlier comparison correctly
rejected equality of the whole old131,564,544-byte range; it did not exclude a
shorter matching interval. This review adds zero physical captures, VM boots,
source/USB I/O or product changes. Both physical cohorts remain closed.

The plan scans the complete original positive RAW streams at exactly128MiB
displacement, retaining every maximal32-byte-frame-aligned equality run of at
least1MiB. It fixes four cases before execution: native50M negative, native25M
positive, old PowerShell25M negative and the Linux50M positive control. Existing
worker-unconsumed sidecars are virtual read-only extensions, never appended to
accepted LOGIC. Full-range independent SHA digests verify every retained pair.

| Retained case | Exact RAW+sidecar equality run | Portion with both ranges inside accepted LOGIC | Repeated numbered CRC pairs |
| --- | --- | --- | --- |
| Windows50M native, negative | 10,428,416 bytes | 8,823,808 bytes | zero |
| Windows25M native, qualified | no run≥1MiB | none | none at this displacement |
| Windows25M PowerShell, negative | 133,169,152 bytes | 131,564,544 bytes | 13 |
| Linux50M control, qualified | no run≥1MiB | none | none at this displacement |

The native50M pair is precisely:

```text
first  RAW [256958464,267386880)
second RAW [391176192,401604608)
distance   134217728 bytes
length      10428416 bytes
```

Each full range has SHA256
`d4098a84a768769170a7d2a2a94947b67f3370bbd92b0054f742b3a66c99d7b0`.
The first range's aligned start lies inside the independently derived RAW bracket
[256958368,256958560) of the first D0 anomaly at sample64,239,597/period35.
The preceding32-byte comparison differs;29 trailing bytes of that preceding
frame also match. The retained aligned run continues to the end of the available
positive kernel stream on its second side, so this review does not establish
where a longer underlying match might end.

Exactly8,823,808 bytes of the equality have both sides inside accepted LOGIC.
The remaining
1,604,608 bytes on the second side are556,032 fetched RAW bytes beyond the finite
LOGIC limit plus the original1,048,576-byte worker-unconsumed sidecar. They remain
quarantined original evidence. No samples were padded, appended, reordered or
promoted into the saved400MB. The old25M pair similarly extends to133,169,152 RAW
bytes only when including these retained suffixes; its accepted LOGIC overlap
remains the previously reported131,564,544 bytes.

A separate standard-library verifier parses every original USBmon submission/
completion in order, accounts for401,604,608 positive IN bytes in each Windows
case and400,556,032 in Linux50M, and hashes the selected ranges directly from
completion payloads. Those digests equal the RAW pair digests. Thus the equality
already exists in exterior Linux kernel data, including the retained sidecar;
it is not introduced by archive replay or concatenating a derived LOGIC file.
Original captures, annotations, reviews and kernel files retain their hashes.

### Equality, temporal support and remaining uncertainty

The old25M pair contains13 complete numbered messages930528–930540 on each side,
with exact corresponding data-bit positions33554432 samples apart. The host issued each of those numbered source commands once, with ACKs. The
paired identifiers supply temporal support in addition to whole-range equality;
host commands/ACKs alone do not measure the number of electrical emissions.

The new50M pair contains **no complete numbered UART message on either side**
within the accepted annotation coverage. Its equality and D0 boundary relation
are verified, but it lacks that repeated-identifier proof. The existing50M
failure remains independently established by absent990514 and D0 periods35/66;
no new confidence in arbitrary channel timing follows. Sidecar beyond accepted
LOGIC has no archive UART annotation witness in this review.

The Linux50M positive has289,901 matching individual32-byte frames at the same
displacement, all isolated single-frame runs, while the native25M positive has
zero matching full frames. This illustrates why matching periodic/idle data
alone is not a universal continuity detector. The1MiB cutoff is the predeclared
offline reporting threshold, not a new product flag, corruption classifier or
safe-rate rule. No arbitrary displacement was searched to manufacture a match.

These results localize a partial equality interval already in kernel data; they
do not distinguish device/bridge/SDK/VMware/KVM/source behavior, identify memory
capacity, prove the direction or cause of a copy, or certify every matched
sample's true time. The two stored ranges have an order; that is not a measured
physical copy direction. A future bounded probe seeking stronger temporal
support would need denser unique witnesses after qualifying the source cadence,
rather than assuming100ms-spaced messages identify a shorter suspect interval.
This audit starts no such capture and changes no firmware, buffer or logger.

Evidence is retained under `artifacts/raw-overlap-review-20261104/`: fixed plan,
original input hashes, every retained run and boundary, paired-message coverage,
independent kernel plan/parser/digest proofs, product baseline, final review and
publication verification. All1,248 product files, HEAD/status/diffs and installed
artifacts remain unchanged. Windows remains off with the DLA4-2/dev16/5000 and
source CH3408134/3-1.2/ttyUSB2 free. This update publishes documentation only;
Windows50M and release qualification remain false. Private captures, SDK/firmware,
VM disks and credentials are excluded.

## Update: native-output Windows50M continuity failure between Linux controls — 4 November 2026

A separate fixed cohort completed **three physical captures and zero retries**:
Linux50M control → actual native-output Windows GUI50M → the single predefined
Linux5M return. The Linux controls pass the stated bounded D0/D22 checks. The
Windows capture remains **unqualified despite400,000,000 bytes and native
complete/no-error metadata**: one numbered UART packet is absent and two D0
periods violate the predefined tolerance. The preceding Windows25M positive is
retained as its own bounded case; it does not extend to50M.

No product/installation/logger/buffer/firmware change was made. Windows used the
same actual GUI705a/corebf0a/C++a1fc/decoder6287/WCHdb19, in desktop Session1/PID4904,
with exact private C#17d809c7 and direct native STD file handles. The live PID was
published before waiting and its original CreateProcess handle was retained
through GUI exit0. The preparation's initial full-selftest refusal and explicitly
reconstructed subset remain as documented; no retroactive full-suite pass is
claimed. Linux used unchanged helperdb7c255b/core84c0c7da with LC_ALL=C.UTF-8.

| Phase | Rate / samples / saved LOGIC bytes | CRC witness | D0 result |
| --- | --- | --- | --- |
| Linux50M | 50 MS/s / 100 M / 400,000,000 | 20 consecutive,990101–990120 | zero bad; periods49–51; boundaries18/10 samples |
| Windows native GUI50M | 50 MS/s / 100 M / 400,000,000 | 19 valid packets;990514 absent | two bad periods35/66; boundaries22/7 samples |
| Linux5M return | 5 MS/s / 25 M / 100,000,000 | 50 consecutive,990801–990850 | zero bad; periods4–6; boundaries2/4 samples |

All phases used finite Stream,32 channels,1.6 V,Captures1,no hardware/software
trigger, no continuous mode and no Stream windows. The same v8 source retained
its1 MHz/50% D0 clock, D22 UART115200,camera1/SD0 and wiring. Three fresh source
ACKs within500ms preceded each Run;22/22/52 distinct live commands were sent and
all ACKed. The source remained open through real END/quiet/cleanup, then
postflight STATUS/ACK and close. Original metadata/HEADER/END and native
controller/helper exits are valid; no locale normalization or timer-as-END
substitution was needed. MCU uptime across serial connections is not asserted.

### The negative is already present in the exterior-kernel data

Windows D22 contains exactly532 decoded bytes:19 complete, CRC-valid28-byte
messages990501–990513, then990515–990520. Both independent UART decoders agree,
with zero warnings, bad CRCs, repeated numbers or malformed interior lines.
The source independently ACKed every distinct command990501–990522, including
990514. ACK/host flush is not a timestamped electrical emission guarantee.

D0 rising-edge intervals are35 samples starting at64,239,597 and66 samples at
67,108,839, versus the predefined49–51 bound at50M. Their independently mapped
RAW brackets are[256958368,256958560) and[268435328,268435648); the second straddles
the256MiB RAW boundary. These addresses localize observations, not a demonstrated
firmware/memory-capacity cause. The990513→990515 first-data-bit interval is
9,909,567 samples (0.198191340 s at the nominal rate), while the corresponding
host after-flush interval is0.198177494 s. These different observations do not
measure UART latency or prove an exact duration of missing samples.

All saved LOGIC bytes match independent RAW conversion. RAW sizes are400,556,032 /
400,556,032 /100,663,296 bytes. Kernel accounting preserves all bytes and original
order in796 /780 /224 events. Windows401,604,608 positive IN bytes are exactly
RAW plus its retained1,048,576-byte worker-unconsumed sidecar. Therefore the
observed mismatch is not introduced by GUI conversion, archive saving or replay;
it is present in data captured by the exterior Linux kernel. This does not
separate the source/device/bridge/SDK/VMware/KVM layers or identify one cause.

The specific older128MiB-shift replay candidate was checked independently:
RAW[134217728,265782272) versus[268435456,400000000) differs in7,687,738 bytes.
It is not the earlier exact131,564,544-byte repeated range. No repeated numbered
packet is observed here. This inequality does not exclude every smaller replay,
replacement or loss mechanism; periodic/idle channels are not unique time IDs.

### Cleanup, file preservation and dispatch limits

Native Save/Open/Save As preserves400,000,000 bytes and **every archive member
and metadata field exactly**, including the original complete status and the
negative payload. First SR/PVS hashes stay intact. The persistent file observer
records the current HEADER/400MB/END/Stopped, no hardware ARM and exit0.
GUI exit0 comes from the retained original process handle, its interactive task
is removed, and all27 Windows originals were copied/SHA-verified before protected
detach, normal VM shutdown and the reserved Linux return. The return then passed;
no failing phase was retried or replaced.

Original-order linkage accounts for382 accepted worker reads plus one sidecar
read. New native Windows50M has3.007789 s inside the read wrapper and0.058111 s
between calls over3.065900 s (1.8954% outside). The separately qualified native
Windows25M case had3.979658+0.029847 s over4.009505 s (0.7444% outside). Thus the
large PowerShell-output delay is not required for every observed continuity
failure. These elapsed intervals include SDK/memory/queue/scheduler work and
are not isolated CPU, logging or condition-wait measurements.

An independent USBmon sweep matches original accounting. Windows50M spans
3.064435 s ARM-completion→lastRAW-completion, with0.724083 s lacking pending IN,
maximum one pending IN and longest gap7.319 ms. Linux50M keeps pending IN for
all2.249992 s, maximum eight; Linux5M return keeps it for5.036464 s, maximum eight.
The negative's longest gap is smaller than14.117 ms in the preceding qualified
native25M case, reinforcing that a single-pause threshold cannot certify
continuity. These are kernel observation extents, not electrical acquisition
times. Guest/kernel clocks are not synchronized; sequential frontends and rates
do not isolate a transport limit or prove a universal safe rate.

Final read-only state: Windows off, DLA1a86:5537 at4-2/dev16/5000 Mb/s and source
CH3408134 at3-1.2/ttyUSB2 free and unchanged. Linux is VMware; Windows is nested
KVM. QEMU NoNewPrivs1/Seccomp2/guest-reset=false was checked before assignment and
detach. No reset, hub cycle, flash, rewiring, installation, simultaneous capture
or additional agent was used. The physical cohort is closed at its three-attempt
maximum. Both Linux controls provide70 qualified consecutive packets; Windows's
19 nonconsecutive packets are not added as a successful continuity case.

Private evidence under `artifacts/native-output-50M-20261104/` retains the fixed
plan, fresh source/product/hash/identity/resource baselines, original per-phase
RAW/LOGIC/USBmon/metadata/source/END, data/kernel/decoder reviews, negative and
UART-sequence localization, native GUI/config/replay/copy/shutdown/return gates,
dispatch/interval reviews, final review and publication verification. All1,248
product files, HEAD/status/diffs and Linux/Windows installations remain unchanged;
all408 GUI source files and the other415 public files are unchanged. This is a
documentation-only negative result, not a product fix or release qualification.
Commercial SDK/firmware, private captures, VM disks and credentials are excluded.

## Update: actual GUI log path and bounded backpressure review — 4 November 2026

An offline follow-up pinned the retained sources of the actual GUI705a and
corebf0a, not the later private worker-retirement candidate. All477 core runtime
source files match the original build manifest; GUI logging/main/settings match
the qualified408-file source set. It adds **zero physical acquisitions, zero
VM boots and no product changes**. The preceding three-capture cohort stays closed.

The actual call path is synchronous: `sr_log()` checks the selected level, then
PulseView `Logging::log_sr()` calls the previous libsigrok callback before its
own log buffer. That previous callback writes a prefix, timestamp and message
to stderr and calls `fflush(stderr)`. The GUI then formats/buffers the message
under its logging mutex and emits its notification. Qt messages instead use
`Logging::log_pv()` and stdout/`endl`; the paths are not interchangeable.

The WCH I/O worker uses two1MiB slots. Its frontend dispatch claims a completed
result, logs the read, performs RAW tracing/conversion/datafeed, then releases
the claimed slot. When both slots are busy the worker can wait on its condition
until a slot is released. Thus slow synchronous output can delay frontend
release and cause backpressure even though global logging is outside the I/O
thread. This is a verified source mechanism, **not a measured queue wait**:
retained traces do not timestamp release, occupancy or condition-wait intervals.
Increasing buffers or moving release before consumers finish is not justified
by these observations.

Original log order and per-read linkage give382 accepted result groups plus
one preserved sidecar read in both Windows25M cases. Each accepted group has
one read log, RAW payload log and LOGIC log, totaling400,000,000 saved bytes.
Differences between printed timestamps, summed over those382 groups:

| Printed log interval | Native-output positive | PowerShell-output negative |
| --- | --- | --- |
| read log → RAW payload log | 0.039379 s; median78 µs | 2.328280 s; median5000 µs |
| RAW payload log → LOGIC log | 0.240258 s; median551 µs | 2.568516 s; median5603 µs |
| LOGIC log → next read log | 3.728123 s | 3.647471 s |
| Sum across all three intervals | 4.007760 s | 8.544267 s |

These are local emitted-log intervals, separate from the embedded SDK read
spans and kernel spans in the preceding update. The `sr:` prefix is written
before taking the printed timestamp. Each difference includes remaining
callback output, GUI buffering/signals, conversion, RAW disk I/O, datafeed,
scheduling and part of the next log prefix. It is not an isolated duration of
stderr, Qt, CPU work or a condition wait. Cross-cohort timestamps are not aligned.

The core default is WARN; these frequent per-read/payload/LOGIC messages are
DBG and are filtered before callback invocation at the default level. Both
physical25M cohorts deliberately selected loglevel4 and RAW tracing. The retained
installed Windows command launcher, hash2ba8d1a2 confirmed by postflight, invokes
the GUI directly; the old private laboratory launcher used PowerShell
`Start-Process -RedirectStandardOutput/-RedirectStandardError`. Neither launcher
nor product logging was changed by this audit. This does not qualify a new
physical default-WARN run or every possible parent-process output target.

The evidence supports keeping diagnostic capture output on the qualified native
file path, without introducing a speculative product logger, queue or buffer
workaround. It still does not prove the origin of the old temporal replay,
missing packets or0B failures, guarantee25M/all32 electrical channels, or erase
any negative. Evidence under `artifacts/gui-log-path-review-20261104/` pins input
hashes, original-order per-read intervals, build/source and installed-launcher
linkage. All product files, installed artifacts and original captures remain
unchanged; Windows remains off with the DLA/source free. This is documentation
only, with no new release qualification.

## Update: bounded physical capture with native Windows log output — 4 November 2026

A new, fixed cohort completed **three physical captures, zero retries, and no
50 MS/s attempt**: Linux25M control, actual Windows GUI25M with native output,
then the single predefined Linux5M return. All three passed the stated bounded
D0/D22 witness and storage/transport checks. This is one positive Windows25M
case in this nested VM setup. It does not erase the earlier Windows25M replay,
qualify arbitrary channel timing, or establish a universal fix or safe rate.

The only planned Windows intervention was the private launcher STD output
mechanism. The unchanged GUI705a/corebf0a ran in actual desktop Session1/PID4352,
using the previously exercised exact C# source17d809c7. CreateProcess inherited
only the three explicit STD handles, wrote directly to exclusive native files,
published the live PID before waiting, and retained its original process handle
through normal GUI exit0. No PowerShell output redirection, product change,
installation, new product flag, flash, hub cycle, USB reset or rewiring was used.
The preparation cohort's initial full selftest refusal and reconstructed subset
proof remain explicit in the preceding update; this run does not convert that
initial script into a passed full suite.

| Phase | Rate / samples / LOGIC bytes | Fresh consecutive CRC packets | D0 boundary samples / bad periods |
| --- | --- | --- | --- |
| Linux25M | 25 MS/s / 100 M / 400,000,000 | 40, 980101–980140 | 10 / 16; zero |
| Windows native GUI25M | 25 MS/s / 100 M / 400,000,000 | 40, 980501–980540 | 12 / 14; zero |
| Linux5M return | 5 MS/s / 25 M / 100,000,000 | 50, 980801–980850 | 2 / 4; zero |

Every phase used finite Stream, 32 channels, 1.6 V, Captures1, no hardware or
software trigger, no continuous mode and no Stream windows. D0 is the retained
1 MHz/50% clock: periods24–26 samples at25M and4–6 at5M satisfy the predefined
±1-sample bound. Both independent D22 UART decoders agree with zero warnings;
all130 numbered packets are fresh and consecutive within their own phase.
The host source sent41/42/52 distinct live commands, respectively, all ACKed;
commands sent outside the captured interval are not silently counted as samples.
Three fresh preflight ACKs were observed within500ms before each Run. The same
v8 source/PWM/camera1/SD0 and wiring were retained; MCU uptime across serial
connections is not asserted.

Independent RAW conversion matches all saved LOGIC bytes, including the actual
Linux Segment/feed and the native Windows archive. RAW sizes are400,556,032 /
400,556,032 /100,663,296 bytes. Passive exterior-kernel accounting preserves every
positive byte in original order:796 /780 /224 events. Windows has401,604,608
positive IN bytes, exactly RAW plus its retained1,048,576-byte worker-unconsumed
sidecar. Both Linux controls account for all RAW/quiet suffixes as well. USB
success, total length and CRC alone are not used as continuity proof; the temporal
witnesses above are required. Firmware remains unknown.

All three phases have original valid metadata, actual HEADER/END, quiet/cleanup
and native exit0, with the source still open at confirmed END and closed only
after postflight STATUS/ACK. Both Linux children directly observed LC_ALL=C.UTF-8;
no decimal-comma normalization or reconstructed live END proof was needed.
The unchanged Linux helperdb7c255b/core84c0c7da remain private diagnostic paths.
Windows metadata is natively complete with an empty capture error. Native
Save/Open/Save As preserves400,000,000 bytes and every archive member, including
all metadata, exactly. The first SR/PVS hashes remain intact. A persistent file
observer, independent of exec transport, observed the current replay HEADER,
400 MB and END/Stopped with no hardware ARM and exit0. This is file replay only.

All27 Windows originals were copied and SHA-verified before protected detach,
normal VM shutdown and the reserved Linux return. The same original GUI process
handle supplied exit0; its interactive task was removed. Read-only postflight
confirms Windows off and unchanged/free DLA4-2/dev16 at5000 Mb/s and source
CH3408134 at3-1.2/ttyUSB2. Linux is VMware; Windows is nested KVM. QEMU
NoNewPrivs1/Seccomp2 and guest-reset=false were verified before assignment and
detach. No simultaneous acquisition or additional agent was used.

### Timing observations, with clock and causal limits

Offline original-order worker/RAW/sidecar/kernel linkage covers all382 accepted
Windows reads and the sidecar read in both25M cases. The new native-output case
has3.979658 s inside the read wrapper and0.029847 s between calls over4.009505 s
(0.7444% outside). The retained PowerShell-output negative has3.059941 s inside
and5.465104 s between over8.525045 s (64.1065% outside). These are elapsed,
clock-local intervals, not CPU use or an isolated measurement of Qt logging,
a queue wait, mutex, SDK or scheduler cost.

An independent USBmon interval sweep matches the original accounting. New
Windows25M spans4.009037 s from ARM completion to last RAW completion, with
0.689946 s lacking pending IN, maximum one pending IN and longest gap14.117 ms.
The earlier negative spans8.524876 s with6.145891 s lacking pending IN. Current
Linux controls keep pending IN throughout4.008803 /5.036490 s, maximum eight.
Guest and exterior-kernel clocks are not synchronized; their local durations
are not absolute timeline alignment or causal partitions. A pause threshold
still cannot classify continuity: an earlier qualified5M case had a longer
single pause than the earlier negative25M case.

The new positive and the lower between-call time are consistent with the
previously isolated expensive PowerShell output path. They do not establish
that path as the sole cause of the earlier repeated131,564,544-byte interval,
missing packets, old0B failures or other transport limits. Cohorts are sequential,
not randomized; internal buffering/state and scheduling are uncontrolled. The
earlier negative, source commands and all partial/refused probes remain intact.
No firmware/memory-capacity diagnosis or product workaround follows from this
one positive. The physical cohort is closed; further acquisition needs a new
bounded plan, rather than reopening it for a green result.

Evidence is retained privately under `artifacts/native-output-physical-20261104/`:
fixed plan and product baseline, per-phase original metadata/END/source/USBmon,
independent data/kernel/decoder reviews, native launch/config/runtime and file
replay proofs, original copy manifests, shutdown/return gate, dispatch and interval
reviews, final review and publication verification. Product HEAD/status/diffs,
all1,248 product files and installed Linux/Windows artifacts remain unchanged;
all408 GUI source files still match the published product source. This update
publishes documentation only. The private launcher, captures, commercial SDK,
firmware, VM disks and credentials are excluded; no release/rollout is qualified.

## Update: native-output launcher in the actual Windows GUI, Demo only — 4 November 2026

A separate preparation cohort used the unchanged full GUI705a with native file
output in actual desktop Session1/PID5364. **Three Demo acquisitions and zero
physical acquisitions** exercised Stop, controller cancellation, finite END,
native Save/Open/Save As and normal application exit. This qualifies the stated
software observations only. Windows25MS/s continuity and AUD002/AUD006 remain
open; the earlier negative capture and all closed physical cohorts are preserved.

### Startup, retained ownership and refused controls

The private launcher separates Start from Wait. It publishes the PID while the
child is alive and retains the original CreateProcess process HANDLE, rather
than reopening a PID later. Wait timeouts preserve that handle; live close and
exit-code reads are refused. Only three explicitly listed standard handles are
inherited, with exclusive new stdout/stderr files. The interactive task has no
automatic execution-time termination. Its owner was observed alive after Demo
END, with no exit record yet; the GUI subsequently exited0 through that same
native handle, after file replay and normal window close.

The initial property-based exit accessor failed its control: Windows PowerShell
silently yielded null instead of catching its throwing getter. A device-free
control independently reproduced property-catch=false/null=true and explicit-
method-catch=true. The private accessor became an explicit GetExitCode method;
no product code changed. Original source, refusal and partial results remain.

The revised initial script reached a later refusal on the PowerShell process's
global handle count. Its preceding guards therefore completed: fast child exit7,
a short Wait returning false, refused live close/exit read, later exit0 of that
same child, and matching PID/quoted/backslash/Unicode arguments and native
standard-handle types. This is an explicitly reconstructed program-order proof,
not a claimed full pass of that script. Its exact original counter values were
not saved, and the cause of the global increase remains unisolated.

A separate native C# block, after two warmups, performed20 failed starts with
expected file-not-found error: handle count532 before and after. All86 output
files from failed starts reopened exclusively. An existing-output sentinel was
preserved. The remaining unchanged600-message logger child exited0 in22.733ms;
its startup line and all600 timed message texts are retained. There were five
successful selftest child starts across the original and revised phases; no
Demo acquisition was repeated to replace a refusal. A first cleanup-probe
compilation also refused an empty dynamic-assembly location before any child;
the combined-source probe and that failed preparation are both retained.

### Three current Demo runs and file preservation

The same GUI/Session/device used32 channels at200kS/s. Each current observer was
READY before Run and observed exactly one HEADER/END. The host controllers ran
outside exec transport lifetime.

| Demo case | Delivered and natively saved bytes | Observed behavior |
| --- | --- | --- |
| Explicit guarded Stop | 1,162,544 | Stop allowed after current data and matching fresh Stop image; GUI remains alive |
| SIGTERM to the host controller | 622,172 | Signal relayed after data; controller remains through guarded Stop and END |
| Finite100k samples | 400,000 | Natural END; late Stop refused without a click |

The cancellation's first image did not match the approved Stop glyph and was
refused. A later fresh matching image permitted Stop within the same run. No
unverified click or additional acquisition replaced it. The two permitted Stop
checks had FLOW ages50.573/59.887ms and screenshot RTT14.225/11.976ms. These are
UI observation/control bounds, not atomic operations or electrical timestamps.

All three native archives pass ZIP CRC, ordered chunks, expected lengths,
32-probe/4-byte units and200kHz metadata. Demo generated no capture-status/error
fields; their absence is preserved, not relabelled as a hardware success.
Native Open/Save As of the first stopped archive preserves all1,162,544 payload
bytes and every metadata member exactly. Its original archive and setup match
the hashes taken before recapture; no implicit Session1 or orphan setup file was
created. Total unique saved Demo data is2,184,716bytes. The replay's persistent
observer also completed normally with a current HEADER/END.

### Scope and final state

GUI705a, corebf0a6c78, C++a1fc7442 and decoder628767ce module hashes are unchanged.
The revised private launcher source is SHA256
`17d809c772e41db85776864c8fb341a1e741ec602859697594dda86909e892d4`.
Linux runs in VMware, Windows in nested KVM. This is a Windows-specific private
process-launch change; Linux product code/installation is unchanged, with no
new Linux desktop or electrical test claimed. No DLA was assigned to this boot,
no source port was opened, and no reset, hub cycle, firmware or wiring change
occurred. Demo software checks do not qualify real acquisition or data continuity.

GUI native exit0 and task removal were verified. **All135 Windows originals
were copied and SHA256-checked before** normal VM shutdown. The shutdown command
returned0, then the QMP polling connection reset while the VM exited; that
monitor did not finish successfully. A separate read-only postcheck verified
state=null/kvm=null, DLA4-2/address16 at5000Mb/s and free DLA/source ports. No
second shutdown or reset was used to replace that exception.

All1248 product files/HEAD/status/diffs and installed programs remain unchanged.
Only this report is published. Private launcher sources/binaries, captures,
vendor firmware, VM disks and credentials are excluded. The original full
selftest refusals remain explicit; no product rollout or complete suite success
is asserted. Real DLA qualification requires a new bounded physical plan with
fresh witnesses and independent RAW/kernel checks, retaining the old negatives.

## Update: inter-read delays and device-free logging controls — 4 November 2026

The Windows25MS/s negative below remains **failed**, and its physical cohort
remains closed. New read-only analysis and device-free software controls identify
a substantial cost in the laboratory's PowerShell output redirection. There is
no new DLA acquisition, product fix, GUI installation or firmware-cause claim.

### Worker and kernel intervals from unchanged originals

Every positive worker read was aligned in original order by read ID, RAW offset,
size and32-byte prefix with retained RAW/sidecar and exterior-kernel requests.
Earlier independent whole-payload conservation remains required; matching a
prefix alone is not that proof. The linked counts, including the unconsumed1MiB
suffix, are383/192/192 for the three cases below. No sample bytes were altered,
reordered or discarded. Local timing identities were recomputed independently.

| Retained case | Worker read-call total | Time between worker read calls | Worker span, first start to last end |
| --- | --- | --- | --- |
| GUI25MS/s,400MB, failed | 3.059941s /382 accepted reads | 5.465104s /381 intervals | 8.525045s |
| Same GUI705a finite5MS/s,200MB, passed | 9.743593s /191 reads | 0.274539s /190 intervals | 10.018132s |
| Native API5MS/s,200MB, passed | 10.009874s /191 reads | 0.007834s /190 intervals | 10.017708s |

In the negative,64.106% of that worker span is **outside** the read wrapper.
Its median between-call interval is12.816ms, versus40us in the positive GUI case
and the much smaller native-API total above. The wrapper brackets timeout setup,
ReadEndP and error handling. Outside it, the worker still performs publication,
mutex/event operations, possible queue waits and OS/VM scheduling. These are wall
time observations, not CPU attribution or a measurement of one wait primitive.

Exterior-kernel accounting for the negative has6.145891s without IN pending in
8.524876s from ARM completion to last RAW completion, maximum one pending IN;
Linux25MS/s maintained pending IN throughout4.008772s, maximum eight. These are
separate clocks and extents: the6.145891s must not be directly equated to the
5.465104s between worker calls. Clock-local sums are exact; signed worker-minus-
kernel duration observations are retained without inventing synchronized clocks.
The GUI5MS/s positive even has a larger maximum no-IN interval188.307ms than the
negative152.591ms. A single longest pause is not a continuity classifier.
Sequential cases differ in frontend/rate/extent; no failure probability or
universal throughput limit follows from this comparison.

### Reproduced redirection cost and a private correction

A native C fixture calls the **exported default libsigrok log callback**600 times
with three representative message shapes. It does not call sr_init, scan, device,
session, WCH or acquisition APIs, and does not include Qt's additional Logging
path. The same source ran on Linux and Windows. Windows used unchanged core
`bf0a6c78`, Linux unchanged `84c0c7da`; DLL/source/executable hashes were recorded.
Windows ran under SSH/Session0, not the interactive GUI's Session1.

The initial six-process order on each OS was inherited stderr, direct file,
NUL, NUL, direct file, inherited stderr. On Windows the inherited case used the
same PowerShell Start-Process RedirectStandardError mechanism as the earlier GUI
launcher. The child could instead reopen stderr to a file or NUL. Timed600-call
Windows totals were2749.031/2749.778ms inherited,21.311/21.215ms direct file and
16.198/16.592ms NUL. Linux totals were11.445/14.885ms inherited direct-file
handles,3.357/3.387ms reopened file and0.501/0.669ms NUL. The inherited mechanisms
differ between platforms; these figures are not an isolated OS comparison.

A second, predefined Windows intervention kept the fixture binary, core,
message arguments and inherited-stderr mode unchanged (per-run result/output
paths were unique), and changed only the launcher/output mechanism. A private CreateProcessW launcher supplied native
file handles with an explicit three-standard-handle inheritance list, exclusive
new output files and the original process HANDLE for exit verification. The
balanced order was native, PowerShell, PowerShell, native:

| Native file handles | PowerShell redirection | PowerShell redirection | Native file handles |
| --- | --- | --- | --- |
| 24.779ms | 2729.811ms | 2713.330ms | 26.604ms |

Both native totals are over101.98 times smaller than either PowerShell control.
All600 timed message texts after the timestamp prefix are preserved exactly in
order in every non-NUL case, including their arguments, with no callback error. The additional untimed
loglevel-startup line is accounted separately. All16 timed fixture processes exit0;
a separately planned fast child exit2 verifies that the native launcher does not
turn an early nonzero exit into success. There are17 software child processes
and **zero physical acquisitions**. Initial offline reviewer refusals assumed
GUI generation1 and overlooked that startup line; the corrected reviewers keep
the actual generation2 and original logs, without rerunning the refused data.

This intervention demonstrates and avoids the redirection cost **in this
fixture**. It does not measure the complete Qt GUI path, apportion the earlier
5.465104s, prove that logging alone caused temporal replay, or qualify a new GUI
capture. The private launcher is qualified only for these fixture paths/arguments;
it is not installed or promoted into the closed capture cohort. Product logging,
queues, timeouts, sample-rate settings, source and firmware are unchanged.

All44 Windows evidence files were copied and SHA256-checked before normal VM
shutdown. No DLA was assigned or source port opened in this boot. Windows is off;
DLA/source ports are free. The1248 product files/HEAD/status/diffs and installed
programs are unchanged. AUD002/AUD006 remain open. Any subsequent GUI/hardware
qualification needs a new bounded plan with fresh witnesses, native Stop/END and
independent RAW/kernel checks; the negative below must remain preserved. Private
captures, launcher binaries, vendor firmware, VM disks and credentials are not
included in this documentation publication.

## Update: 25 MS/s Windows continuity failure between passing Linux controls — 4 November 2026

A new bounded cohort is closed at **three physical acquisitions, zero retries**.
A Linux25MS/s control passed, the actual Windows desktop GUI at the same requested
geometry failed temporal witnesses, and the predefined Linux5MS/s return passed.
The planned Windows50MS/s phase was cancelled at the first failure. This is a
negative qualification result, with no product fix, workaround or installation.

| Phase | Requested samples/rate | Saved LOGIC bytes | Electrical witness result |
| --- | --- | --- | --- |
| Linux control | 100M /25MS/s | 400,000,000 | Pass:40 fresh consecutive CRC messages, D0 no abnormal periods |
| Windows GUI | 100M /25MS/s | 400,000,000 | **Fail:** missing/repeated CRC messages and6 abnormal D0 periods |
| Windows faster phase | 100M /50MS/s | Not acquired | Cancelled; no retry |
| Linux return | 25M /5MS/s | 100,000,000 | Pass:50 fresh consecutive CRC messages, D0 no abnormal periods |

### Configuration, identity and ownership

All acquired phases used finite Stream,32 enabled channels,1.6V, no hardware or
software trigger, no Stream windows and one capture per Run. Linux is in VMware;
Windows is in nested KVM. The same DLA `1a86:5537` stayed at USB4-2/address16,
negotiated5000Mb/s; its firmware version is unknown. The unchanged CH340 source
`8134` at3-1.2/ttyUSB2 reported v8/PWM1MHz/50%/camera1/SD0. There was no reset,
hub cycle, flash or wiring change. Source connections were separate; continuous
MCU uptime is not claimed.

Windows actual desktop Session1/PID1400 loaded GUI SHA256
`705a2fc10aaec5859f3f398ad3468f3ca51cc3774c31fcf960c8777ec1b72b6c`,
whose408-file source snapshot is published in `62a00c79`. Core/C++/decoder/WCH
module hashes match the preceding qualified5MS/s cohort. QEMU NoNewPrivs1,
Seccomp2 and guest-reset=false were checked before assignment. Native options,
finite100M/25MHz selectors and trigger-off state were inspected; the saved setup
independently confirms32 enabled channels, trigger/windows off and captures1.
All four full2048-byte commands match the Linux25MS/s control exactly; Windows
command framing and CRC were checked separately. These sequential controls use
different frontends/transports and do not isolate an OS, scheduling or firmware
cause, or estimate a failure rate.

Passive USB monitoring and the current GUI observer were ready before Run.
Three fresh source ACKs preceded each Run by at most500ms. Every capture retained
the source through actual backend END/quiet/trace closure and cleanup, then
postflight STATUS/ACK and source close; each live end guard passed directly.
The GUI remained alive after the Windows END. A missing private Python dependency
stopped an initial controller import before any source open or native Run; that
failure and the unchanged dependency correction are retained. It was not a
physical acquisition or a retry of one.

### The Windows negative is in the retained input

Native metadata reports `complete`, no error, and exactly400,000,000 LOGIC bytes.
An independent implementation converts the original400,556,032-byte RAW prefix
to the saved LOGIC exactly. Kernel accounting preserves original request and
completion order:780events,401,604,608 positive IN bytes =400,556,032 RAW
+1,048,576 unconsumed worker-sidecar bytes. No bytes were reordered, silently
discarded or repaired.

Both UART decoders agree, without warnings, on40 CRC-valid packet occurrences.
Counters930514..930526 are absent;930528..930540 appear twice. The host sent each
fresh counter once and all42 live commands were acknowledged. D0 has6 periods
outside25+/-1samples. Their starting samples are30947310,30947328,31088640,
31209452,31209472 and67108851, with periods18,16,16,20,14 and16samples.
The initial/final rising-edge boundaries are10/20samples.

More specifically, original RAW ranges `[134217728,265782272)` and
`[268435456,400000000)` are **byte-for-byte identical over131,564,544bytes**,
at a distance of128MiB. Thirteen complete repeated numbered CRC packets have
identical positions relative to those ranges. This supports temporal replay in
the retained input, beyond similarity of a periodic clock alone. The independently
matched kernel stream already contains these bytes, before guest conversion,
GUI storage and Save. It does not establish device memory capacity or identify
the responsible firmware, USB hardware, host stack or virtualization component.
A correct byte count, valid CRCs and clean END are insufficient continuity proof.

### Linux controls, replay and final state

Linux used the unchanged native C API/actual Segment helper `db7c255b` with
installed core `84c0c7da`. At25MS/s, RAW400,556,032B, feed and actual Segment
preserve400,000,000 output bytes exactly;796USBmon events account for all bytes
in original order. Both decoders agree on930101..930140, no warnings; D0 periods
24..26, boundaries5/20samples. The5MS/s return preserves100,000,000B from
RAW100,663,296B,224events; counters931101..931150, D0 boundaries4/1samples,
no abnormal periods. The original JSON is valid in both runs: child environments
explicitly set `LC_ALL=C.UTF-8` despite inherited Romanian `LC_NUMERIC`.
Native exits, one HEADER/END, cleanup and source-close ordering pass directly;
no decimal normalization or ownership reconstruction was needed in this cohort.

Native Windows Save/Open/Save As preserved all400,000,000 bytes and every archive
metadata member, including the original `complete` status and its continuity
failure. Original archive/setup files are unchanged. The auxiliary host/guest
replay observer was interrupted by exec transport after4MiB (host exit143).
That partial observer and a sharing-mode read refusal are retained, not reported
as a passing live replay gate. Its original prefix matches the full GUI log at
the exact235806-byte READY offset. The full original log subsequently proves
one file HEADER/END/Stopped and400MB, with no hardware ARM; complete native
Save As equality independently confirms payload/metadata preservation. No Open
or acquisition was repeated to replace the failed observer record.

GUI native exit0 and the exit-code7 self-test passed. **All26 Windows originals
were copied and SHA256-verified before** guarded detach and normal VM shutdown.
The reserved Linux return began only after those checks and fresh free-port,
identity and speed checks. Windows is now off, DLA/source ports are free, all1248
product source files/HEAD/status/diffs and installed programs remain unchanged.

The report qualifies these bounded Linux controls and preserves the Windows
negative. Electrical witnesses cover D0/D22 only. It does not qualify arbitrary
32-channel continuity, the cancelled50MS/s case, or Windows25MS/s operation.
AUD002/AUD006 and older zero-byte/gap/replay observations remain open. Private
captures, vendor firmware, VM disks and credentials are excluded from publication.

## Update: free continuous capture, native Stop and finite restoration — 4 November 2026

A new, closed cohort used **three physical acquisitions and zero retries**:
Windows free continuous Stream with an explicit native Stop, a finite capture in
the same GUI/Session/device, then Linux return. All three stored-data comparisons
pass: **403,809,024 LOGIC bytes and 201 fresh consecutive CRC messages**. The Linux
live metadata gate refused a diagnostic formatting error; its separate offline
review and source-ownership reconstruction are described below, rather than
reported as an initially passing live gate.

The tested Windows full GUI is SHA256
`705a2fc10aaec5859f3f398ad3468f3ca51cc3774c31fcf960c8777ec1b72b6c`,
from the 408-file source snapshot published in `62a00c79`. Loaded core/C++/SDK/WCH
hashes were verified. The installed programs and all product source files remain
unchanged in this cohort. Linux runs in VMware; Windows runs in nested KVM.
DLA `1a86:5537` stayed at USB path4-2/address16, negotiated5000Mb/s. Its firmware
version is unknown. Source CH340 `8134` at3-1.2/ttyUSB2 reported v8, PWM1MHz/50%,
camera1 and SD0. There was no reset, hub cycle, flash or wiring change.

### Windows acquisition and evidence

Actual desktop Session1/PID6216 used 32 enabled channels, 1.6V and5MS/s. Native
Continuous sampling was enabled in Stream; hardware/software triggers and Stream
windows were disabled. Captures-per-Run was1. Stop was permitted only after
100,000,000 current LOGIC bytes, with a current FLOW receipt no older than150ms
and a new image matching the approved Stop glyph. At the decision it had
101,711,872 bytes, FLOW age5.156913ms and screenshot RTT16.997375ms. This is a GUI
observation/control bound, not an atomic UI operation or an electrical timestamp.
One native Stop produced one current ARM/HEADER/END, status `stopped`, no error,
and **103,809,024 saved bytes**, equal to the entire independently converted RAW.
D0 had no abnormal periods, with initial/final boundaries2/4samples. Two UART
decoders agreed on51 fresh messages928101..928151, without warnings.

The actual Continuous checkbox was then disabled. Its hidden finite selector
returned and was set to50M samples at5MS/s. Without capture Open/reload/device
change between the acquisitions, the second capture saved **200,000,000 bytes**,
status `complete`, no error. RAW conversion matched exactly. D0 boundaries were
4/0samples with no abnormal periods; both UART decoders agreed on100 fresh
messages928501..928600. All four full2048-byte finite commands match the earlier
finite50M Windows API reference; their framing and CRC were checked separately.
The continuous setup is different and is not claimed to match that reference.

Kernel conservation and original completion order pass. Continuous:
214events/104,857,600 positive IN bytes =103,809,024 RAW +1,048,576 unconsumed
worker sidecar bytes. Finite:398events/201,326,592 positive bytes =200,278,016 RAW
+1,048,576 sidecar. All eight full commands were independently CRC-checked.

The persistent observer was READY before the source opened; each Run had three
fresh acknowledged witness commands. The source stayed open through actual
retirement/trace closure/END/quiet/Stopped, then status/ACK and close. The74 and102
live commands were all acknowledged; commands after the sampled interval are
not claimed to be captured. Source connections were separate, without an MCU
uptime claim.

Native Save after recapture asked for the new target, exercising the published
Save-context correction on hardware. After both physical acquisitions, native
Open and Save As of each archive preserved every payload byte and all capture
metadata, including `stopped` versus `complete`. ZIP CRC, ordered chunks and
member hashes passed on Windows and independently on Linux. Original archives
and their setup files remained unchanged. An unbound shortcut was mistakenly
used during replay; its UI effects and failed attempt were retained. Qualified
resaves used the actual Save As menu and full comparisons. No acquisition was
repeated. An early launcher screenshot and unsuccessful checkbox click were
also retained; only subsequently verified native states qualified the Run.

GUI native exit0, a child exit-code self-test of7 and removal of the temporary
tasks were verified. **All58 Windows originals were copied and SHA256-checked
before** guarded detach and normal VM shutdown. QEMU retained NoNewPrivs1,
Seccomp2 and guest-reset=false.

### Linux return and diagnostic exception

The unchanged `db7c255b` native C API/actual Segment helper, with installed core
`84c0c7da`, captured25M samples at5MS/s. This is transport/storage return evidence,
not a Linux desktop Save test. RAW100,663,296B, feed and actual Segment preserve
100,000,000 output bytes exactly under independent conversion. USBmon224events
account for all positive bytes in original order. D0 has no abnormal periods,
boundaries4/1samples; two UART decoders agree on50 fresh messages928801..928850,
without warnings. The helper exited0 with one HEADER/END, accepted Stop, drain,
trace closure and session/device/context cleanup.

The service inherited `LC_NUMERIC=ro_RO.UTF-8` without an overriding LC_ALL.
The helper's locale-sensitive formatter wrote four timing values with decimal
commas, which strict JSON parsing rejected after the native process had exited.
A device-free QCoreApplication/glib formatting control reproduces the invalid
number in that environment and valid JSON with `LC_ALL=C.UTF-8`. The private
launcher now sets that environment for future cohorts; this capture was not
repeated and the native helper/product binaries were not changed.

The original malformed result, exception and controller flags are retained.
A separately labelled derived metadata view changes exactly four decimal
separators, preserving the numeric values and every other byte; RAW/feed/Segment
and trace inputs remain unchanged links to the originals. Backend completion is
verified offline from those same originals. Retained controller/protocol order
proves the serial context remained open when STOP_READY was observed after
session/device/context closure, and unwound only at the later JSON exception.
The exact port-close timestamp was not recorded. The initial live END parser
and its postflight STATUS did not complete; they are not reported as passing.
A later, separate read-only STATUS connection verified the unchanged source
configuration and closed normally. This reconstruction is distinct from the
direct live END/source-close measurements of the two Windows phases.

The cohort is closed. Windows is off and DLA/source ports are free. These results
qualify the stated5MS/s continuous-Stop-to-finite case and bounded D0/D22 witness
data. They do not establish arbitrary32-channel electrical continuity, high-rate
behavior, a firmware cause or repair for older zero-byte/gap/replay observations,
or a complete GUI rollout. AUD002/AUD006 remain open for those scopes. No new
product workaround or GUI installation is promoted. Private captures, diagnostic
executables, vendor firmware, VM files and credentials are not published.

## Update: internally stopped Stream window and finite restoration

A new fixed cohort is closed after three physical acquisitions, zero retries.
It starts from the separately verified Linux return in the preceding cohort;
that closed cohort is not rerun. The starting evidence hash, current DLA/source
identities, exclusive owners and negotiated 5000 Mb/s are checked before the new
guarded Windows assignment. Firmware, source v8/PWM1MHz50%, camera1/SD0, wiring
and hub are unchanged. Linux is in VMware; Windows is a nested KVM guest.

This tests one specific route in the actual full GUI206d, interactive desktop
Session1, PID420. Stream windows require a finite **UI** depth: the product
then temporarily enables continuous input in the backend and restores its
limits on exit. The UI Continuous checkbox stays off. This distinction was
clarified before any physical capture; the original plan is retained.
It is not a manual free-running-continuous or explicit-Stop qualification.

| Phase, all at 5 MS/s | Saved data | Retained RAW | Fresh consecutive CRC messages | Kernel records |
| --- | ---: | ---: | --- | ---: |
| Windows, one internally stopped window | 20,000,000 B | 20,971,520 B | 926101–926110 (10) | 56 |
| Windows, subsequent finite 50M samples | 200,000,000 B | 200,278,016 B | 926501–926600 (100) | 398 |
| Installed Linux return, 25M samples | 100,000,000 B | 100,663,296 B | 926801–926850 (50) | 224 |

The first phase selects one 5-million-sample window, D0 rising, Pre1% and
Captures1. It stops internally when that window is complete, without a timed
Run/Stop toggle. Its native collection contains one complete frame with no
error, source origin1 and trigger50000 relative samples (0.01s), exactly 1%
real pre-trigger history. At that trigger, D0 changes from low to high. All
20 MB match the independently converted RAW slice starting at source sample1.
Both UART decoders agree with zero warnings on all10 fresh CRC messages.
D0 has no period outside4..6 samples; first/last-rise boundaries are2/3 samples.

The second phase keeps the same GUI process, Session and DLA. No capture-file
Open, reload, device switch or VM restart intervenes between acquisitions.
Native Restore Session Setup disables only Stream windows in the previously
saved setup; the normal sample selector then chooses50M at5MS/s. The software
condition is retained but inactive without Stream windows. This qualifies that
native setup-restoration route, not a pointer toggle of an overflow-hidden
checkbox. Driver Continuous reads off, and all four complete2048B
Stop/SETUP/ARM/Stop commands match the previously verified finite50M API case,
including SHA and CRC. The stored200MB, independent RAW conversion, all100 fresh
CRC messages and D0 pass; D0 boundaries are3/3 samples. Complete/error-empty
metadata is preserved. The earlier window export is not overwritten.

After both physical Windows acquisitions, native Open and Save As replay both
exports. Every20MB window byte and200MB finite byte, capture metadata, frame
order/status/error, source origin and trigger are exact. ZIP CRCs and collection
member lengths/SHA are checked. The original native exports retain their
original SHA; different ZIP timestamps need not produce identical containers.

Passive kernel evidence accounts for every positive byte in original
completion/submission order, without loss, truncation, sorting or repair.
The window has20,971,520B RAW plus a separate1,048,576B worker suffix, exactly
22,020,096B kernel data. The finite phase has200,278,016B RAW plus1,048,576B
suffix, exactly201,326,592B kernel data. All eight complete Windows commands
have independently verified framing/CRC. Raw surplus, source-origin prefix and
worker suffixes remain separate from the intended saved frames; none is erased.
The Linux return has100,663,296B RAW/kernel, exact100MB feed/actualSegment,
50 fresh CRC messages and quiet Stop/END/cleanup. Electrical witnesses remain
limited to D0/D22; stored32-value conversion is not an arbitrary all-channel
continuity guarantee.

Persistent current-run observers are READY before three fresh source ACKs and
native Run. The foreground source stays owned through accepted Stop, quiet
drain, worker retirement, trace closure, END and Stopped; post-status/ordered
ACK checks precede serial close. Window/finite live commands are15/102; later
ACKs beyond the saved interval are not claimed as captured messages. The GUI
remains alive through native saves and replay, then exits normally with native
code0 through the retained-handle reader. Its child-exit7 control also passes.
All34 guest originals are copied and SHA-verified before normal VM shutdown;
temporary tasks are removed. The Linux return and final DL32 identity scan
pass. Windows is off; DLA at4-2/address4.16 and source CH3408134 at3-1.2 are free.

A packaging check initially refused the already confirmed END because this
PowerShell runtime interprets String.Split's separator as characters, not one
literal substring. The error, script and empty output directory are retained;
no RAW had moved. The corrected check counts escaped literal regex matches,
and filenames use explicit suffix construction. Originals are then copied and
verified. This is a packaging-tool correction after capture, not a product
change, altered acceptance criterion or repeated physical acquisition.

The actual GUI/runtime binaries and all1,248 local product source hashes,
HEAD/status/diffs, installed GUI/launcher/libraries retain their baseline.
QEMU's existing per-child reset guard and guest-reset=false remain active; no
reset, power cycle, flash or wiring change is used. This update contains only
the reviewed evidence report. Private captures/tools, commercial firmware,
VM and credentials are not published. This positive5MS/s route does not repair
or explain earlier0B cases, manual continuous/Stop failures or high-rate gaps/
replay. AUD002/AUD006 remain open; GUI206d is not installed or promoted.

## Update: matched Windows API and interactive GUI geometry

A separate fixed cohort is closed with four physical acquisitions and zero
retries. It keeps the source v8/PWM1MHz50%, wiring, firmware and hub unchanged.
Linux runs in VMware; Windows is a nested KVM guest. The DLA is
`1a86:5537`, negotiated 5000 Mb/s at Linux path `4-2`/address `4.16`.
The source CH3408134 remains at `3-1.2`; DTR/RTS are false. Its serial port is
opened separately for each phase, so continuous MCU uptime is not claimed.

The earlier 25-million-sample API control did not match the GUI's 50-million
sample request. This cohort requests finite Stream, 5 MS/s, 50 million samples,
32 channels, 1.6 V and no trigger in **both Windows paths**. Linux recovery and
return controls each request 25 million samples at 5 MS/s. Equal requested
geometry does not match hidden device state or establish GUI/API causality.

| Phase | Actual LOGIC/storage | Retained RAW | Fresh consecutive CRC messages | Kernel records |
| --- | ---: | ---: | --- | ---: |
| Linux recovery | 100,000,000 B | 100,663,296 B | 924201–924250 (50) | 224 |
| Windows native API/Segment | 200,000,000 B | 200,278,016 B | 924501–924600 (100) | 406 |
| Windows interactive GUI | 200,000,000 B | 200,278,016 B | 924801–924900 (100) | 398 |
| Linux return | 100,000,000 B | 100,663,296 B | 925201–925250 (50) | 224 |

Every phase passes independent RAW-to-LOGIC conversion. Both API paths also
match their delivered feed and actual Segment bytes. The actual GUI's native
Save, Open and Save As round trip preserves all 200 MB, every ordered ZIP LOGIC
chunk and the complete metadata/error state exactly. The archives report
`complete`, no capture error, 32 stored channels and 5 MHz. ZIP CRCs and all
payload lengths are checked; differing container timestamps are not data loss.
The original export's SHA is unchanged after replay/reexport.

Two independent UART decoders agree with zero warnings in every phase. The
captured CRC ranges above are fresh, known and consecutive, not merely serial
ACKs. D0 has no period outside 4..6 samples; initial/final rise boundaries are
2/1, 4/3, 2/4 and 4/3 samples respectively. These electrical witnesses cover
D0/D22 only, not arbitrary signals on every channel. The 32-value conversion
check concerns stored data, not a general electrical continuity guarantee.

Passive USB evidence is loss-free and accounts for every positive byte in
original submission/completion order. Each Windows phase retains 200,278,016 B
of decoder-input RAW **plus a separate 1,048,576 B worker-unconsumed suffix**,
exactly matching all 201,326,592 positive kernel bytes. Neither suffix nor
quantized RAW surplus is discarded or fed to the saved 200 MB recording.
Linux's complete positive kernel payload is 100,663,296 B per phase. All four
complete 2048 B Stop/SETUP/ARM/Stop commands have verified framing and CRC;
the Windows API and GUI command hashes match exactly. The Linux count differs
by design, so its SETUP is not required to match the Windows 50M request.

A supplementary review initially required zero worker suffix, inherited from
the earlier 100 MB control. The existing independent WCH accounting already
qualified the 1 MiB suffix in original order. That additional assumption was
rejected and the review corrected to require complete ordered conservation.
The rejected review, its source and every original byte are retained; no
physical acquisition was repeated. This is a review-tool correction, not a
product fix or a change to the cohort's acceptance contract.

Before hardware, the current-run GUI END guard passes 27 offline controls,
including retained positive, zero-data-timeout and long-timeout logs plus
missing/duplicate/stale markers and mismatched lengths. It confirms termination,
not capture success. An actual interactive Demo run then verifies that a
persistent SSH observer can read the live GUI log, signal READY before Run and
observe the current HEADER/END/Stopped while the GUI stays alive. Demo has no
physical DLA or serial source. Its native exit is independently verified as 0;
the retained-handle exit reader also passes a child-exit-7 control.

For each physical phase, three fresh source ACKs precede Run. The actual GUI
Run is 121.420384 ms after the latest acknowledged preflight command. The
persistent observer is already READY; no cold SSH query or timed Run/Stop
toggle intervenes. Foreground source ownership lasts through current-run
HEADER/ARM, accepted Stop, quiet drain, worker retirement, trace closure,
END and Stopped. Source status/ACK checks precede serial close. The API helper
also exits natively before source close; the GUI stays alive for Save and is
closed normally only after native replay/reexport. The actual GUI exit is 0,
verified through its retained native handle. No fabricated GUI-exit marker is
used to confirm acquisition END.

Source control emits 51/102/102/52 live commands, plus three preflight commands
per phase; all have ordered ACKs. Later ACKs outside the captured interval are
not counted as captured data. Linux API first LOGIC arrives 57.940/58.144 ms
after ARM returns; Windows API is 58.013 ms. These are callback observations,
not timestamps of the first electrical sample.

The exact unmodified binaries are Linux helper `db7c255b`, Windows helper
`d0ac9c4c`, Linux core `84c0c7da`, Windows core `bf0a6c78`, and actual full GUI
`206d8584` in Victor's interactive desktop Session 1. Runtime DLL paths/hashes
are checked before Run. This is broader evidence than a Session-0 API helper,
but it does not qualify the continuous-to-finite transition, software-window
controls with fresh physical counters, high rates or earlier intermittent
zero-data captures. **AUD002/AUD006 remain open; no product correction or GUI
installation is promoted.** The earlier failures remain unchanged.

All 13 API, 14 GUI and 8 Demo guest originals are copied and SHA-verified before
normal VM shutdown. Temporary tasks are removed; GUI/launcher/runtime hashes
are unchanged. QEMU retains its existing per-child reset guard and
`guest-reset=false`; no reset, power cycle, flash or rewiring is used. A real
Linux return and final DL32 identity scan pass. Windows is off; DLA and source
have no owners. All 1,248 local product source hashes, repository HEAD/status/
diffs and installations retain their baseline. This update changes only this
evidence report; captures, test tools, vendor firmware, VM and credentials stay
private.

## Update: native API controls with source closure after confirmed END

A separate plan was capped at three physical acquisitions: an installed Linux
baseline, native Windows API capture, and installed Linux return. Each required
independent qualification of the preceding acquisition. No physical acquisition
was repeated. Both hosts used the already qualified, unchanged C API/actual
Segment helper; this path does not run Session, MainWindow or View.

The laboratory completion check requires the native process to finish, exactly
one HEADER/END and closed-session marker, RAW trace closure and quiet cleanup.
Elapsed time, ARM or a successful SSH launch alone cannot satisfy it. Thirty
offline controls using both hosts' retained logs reject missing, stale, duplicated
or conflicting evidence, while distinguishing a cleanly ended failure from a
successful acquisition. A first predicate mistake admitting exit 1 with complete
metadata was caught and retained before physical testing. The corrected checker
was also exercised by the real Windows bridge.

The guest bridge stays connected and reports READY before source preflight.
After three fresh acknowledged commands, GO travels through that same pipe.
The source remains open through native process exit and confirmed END; it is
then checked and closed. No timed Run/Stop toggle controls these captures.

All three finite Stream captures used 32 channels, 1.6 V, no trigger,
5 MS/s and a 25-million-sample target:

| Physical acquisition | Stored bytes / captured fresh CRC packets | First LOGIC after returned ARM | Passive USB records |
| --- | --- | --- | --- |
| Linux baseline | 100,000,000 / 50 | 58.224 ms | 224 |
| Windows C API | 100,000,000 / 50 | 59.084 ms | 216 |
| Linux return | 100,000,000 / 50 | 58.022 ms | 224 |

In each case, the preserved RAW prefix converts independently to the original
feed and actual Segment bytes exactly on all 32 stored channel values. D0
periods are four to six samples, with zero outliers including boundary checks.
Two UART decoders agree on all 50 consecutive CRC packets without warnings.
All 52 live serial commands and three preflight commands were acknowledged;
ACKs beyond the capture interval are control evidence, not additional samples.
Electrical witnesses remain limited to D0/D22.

Each original RAW contains 100,663,296 bytes. The suffix beyond the requested
100,000,000 LOGIC bytes is retained. Every RAW byte matches passive outer-Linux
USB completions in original completion/submission order, with no lost or
truncated monitor events and all positive sample bytes accounted. Windows has
96 positive 1 MiB completions; the worker-unconsumed sidecar is empty. All four
complete 2048-byte Stop/SETUP/ARM/Stop commands have valid framing/CRCs and are
identical across the three acquisitions. These observations are not a measured
physical bus utilization or a proof of arbitrary all-channel continuity.

One Windows loader preparation was rejected before `sr_init`: the bridge passed
an expected DLL path with backslashes while the unchanged helper compares a
slash-normalized path. Exit 2 produced no RAW, configuration or ARM files.
An offline DLL mapping check reproduces the mismatch; only the bridge's
environment string changed to canonical slashes. The rejected launch and its
source preflight are preserved. Only the previously unstarted acquisition phases
continued; the DLA assignment was kept without a reset. It is not a repeated
physical capture or an unrecorded successful preparation.

Windows ran through SSH Session0 in nested KVM; Linux runs in VMware, on the
same DLA `1a86:5537`, USB path `4-2`, address `4.16`, negotiated 5000 Mb/s.
Source v8, PWM 1 MHz / 50%, camera
and wiring were unchanged, SD disabled; DLA firmware version remains unknown.
Serial connections were separate per phase, so uninterrupted MCU uptime is not
claimed. All 18 Windows original files were copied and SHA256-verified before
normal VM shutdown. The guarded handover, Linux return, final DL32 identification
and released USB/serial owners passed. No hub cycle, reset, firmware write,
product-source change or GUI installation was performed.

This qualifies these low-rate API controls and the laboratory source-completion
rule. It does not repair or invalidate the earlier zero-byte starts or replay
failures. The earlier interactive GUI timeout requested 50 million samples;
this API control requests 25 million, so it is not a matched GUI/API causal test.
Interactive desktop, physical continuous-to-finite and broader high-rate Windows
qualification remain open. The GUI candidate remains uninstalled.

## Update: host-only automatic count Stop check

The old laboratory controller was reproduced unchanged with every hardware,
serial, QMP and SSH interface replaced. A simulated flow query delayed by
2.964 seconds makes it return success and close its source while the simulated
acquisition is still active. No physical acquisition was started in this check.
That controller remains retired; its closed physical cohort was not resumed.

A private entry point then exercised the existing MainWindow, Session and View
objects on Linux XCB and the Windows platform. The Windows test ran in SSH
Session 0, not Victor's interactive desktop. The application source snapshot
and the original GUI objects were retained; no product binary was installed.
Configuration uses Session/device APIs; toolbar editing and displayed selector
refresh are outside this check.

The standard Demo driver lacks CONTINUOUS GET/SET. A probe-only Device adapter
therefore temporarily changes its sample bound from 5 million to 100 million.
The existing frontend extracts one 5-million-sample window at 1 MS/s, with a D0
rising trigger and 1% pretrigger, and stops the source when that window finishes.
This adapter is restricted to Demo and is not product code or a DLA workaround.
The main Qt/observer thread was deliberately paused for seven seconds.

| Host platform | Internal window Stop after Run | Following finite capture |
| --- | --- | --- |
| Linux XCB | 5.047586 s, before the observer resumed | Exactly 5 million samples / 20,000,000 bytes |
| Windows, Session 0 | 5.039201 s, before the observer resumed | Exactly 5 million samples / 20,000,000 bytes |

Both returned to Run without an external Stop toggle. The sample limit and
trigger were restored before the following finite capture in the same session.
All four payloads match an independent Gray-to-binary inverse yielding counters
1 through 5,000,000 on the saved 32-bit values. The archives, member SHA256/CRCs,
chunk order, source origins and triggers passed, and saved replay matched.
The automatic window has explicit `complete` status. The ordinary finite Demo
source emits no outcome metadata: its empty in-memory status is serialized as
`unknown` and read back as such; it is not relabelled complete by this check.

Initial fixture failures are retained: unsupported native Demo controls,
rejected zero sample limit, and an incorrect expectation that empty Demo status
round-trips as an empty string. The final fixture checks the format's explicit
`unknown` representation. These are laboratory assumptions, not product fixes.

This verifies the frontend count Stop under a stalled observer. It does not
qualify DLA CONTINUOUS restoration, USB/electrical behavior, a physical
continuous-to-finite transition or the earlier unrestricted continuous capture.
There were zero physical captures, no DLA assignment and no serial source opens.
The separate failed timing attempt remains unqualified. Product sources and
installed launchers remain unchanged; the GUI candidate is still uninstalled.

## Update: finite first-data deadline experiment

A separate Linux-only diagnostic tested whether a finite Stream acquisition can
legitimately deliver its first data after the existing three-second deadline.
A review of seven retained starts found that the failing 5 MS/s finite requests
encoded five or ten seconds while timing out at about three seconds. However,
identical finite setups had also delivered data promptly, and a 20 MS/s failure
encoded only 1.25 seconds. These observations motivate a test, not a universal
mode, firmware or transport explanation.

Only a private copy of `api.c` changed: finite Stream first-data allowance became
the requested duration plus the existing three-second margin, under a Linux-only
guard. Continuous operation, Buffer, later stall handling, command fields and
ownership were unchanged. The 477-file runtime snapshot, configuration, build,
make check and private install were verified. Four synthetic processes covered
eight attempts, including recovery on the same open device: delayed first data,
no data and a stall after partial data. The synthetic delayed-first case shows
that the changed deadline has the intended effect; it is not hardware evidence.

The physical plan was capped at three captures, zero retries, with a fresh source
preflight before each start. All three completed:

| Linux 5 MS/s control | Independent result | First data after logged ARM |
| --- | --- | --- |
| Installed runtime, continuous host-count Stop | 100,663,296 bytes | 58.198 ms |
| Private runtime, finite 25 million samples | 100,000,000 bytes | 58.118 ms |
| Installed runtime, finite 25 million samples | 100,000,000 bytes | 58.302 ms |

Each passed RAW-to-feed-to-actual-Segment comparison, all 50 fresh CRC packets
in order with two UART decoders, D0 periods of four to six samples, loss-free
kernel request/completion ordering and byte conservation, Stop, drain, END and
resource closure. Electrical witnesses remain limited to D0 and D22. Linux ran
in VMware on the unchanged USB3 path; no reset, hub cycle, firmware or wiring
change was made. This private diagnostic did not modify or run Windows code.

**The additional deadline was never used in any physical capture. The experiment
does not support promoting a longer timeout.** Immediate success with both
private and installed runtimes does not explain the earlier zero-byte failures.
The private runtime was not installed in the normal launcher or published.

## Update: Windows continuous-to-finite cohort stopped after a timing failure

A new, separate plan allowed a native continuous GUI capture followed by a finite
capture in the same device session, then an installed Linux return control. Each
later phase required the previous phase to pass. Only the first physical
acquisition started; no retry or remaining phase was run.

The actual Windows GUI and runtime hashes are the same as those recorded below.
It used Stream, 5 MS/s, continuous sampling, all 32 channels, threshold 1.6 V,
software windows disabled and no hardware trigger. Windows ran in nested KVM,
with exclusive DLA ownership, the reset guard and `guest-reset=false`. The source
preflight passed three fresh ACKs from a live producer; the first 50 commands
892201–892250 were acknowledged.

The laboratory controller was supposed to request native Stop at six to eight
seconds. Its remote flow query took 2.964 seconds, exceeding its 1.5-second guard,
so it skipped the toggle. It then wrongly exited successfully and closed its
source connection after about 13 seconds without confirming acquisition END.
The later manual Stop request was 129.035 seconds after the Run request. Driver
logs show a sample-transfer timeout before backend Stop, two empty drain reads,
worker retirement and END. The source process was not kept alive throughout the
unplanned long acquisition. This is a failed laboratory timing/control attempt,
not a successful transition test or evidence of a product fix. The cause of the
later data stall is not isolated.

The original 2,478,833,664 RAW bytes (619,708,416 samples), zero-byte unconsumed
sidecar and logs were retained. Native Save finished after the error and wrote
an archive containing 591 ordered LOGIC chunks. Its 2,478,833,664 decoded bytes
match an independent RAW bit-plane conversion on all 32 channels, byte for byte;
ZIP CRCs, lengths and original guest-to-host SHA256 hashes passed. The archive
retains `capture status=timeout` and the actual `Capture incomplete` error.
Storage preservation does not make the acquisition complete. No GUI replay of
this large partial archive is claimed.

D0 has 123,945,656 rising-edge periods, all four to six samples, with only four
samples before the first rise and one after the last. Two independent UART
decoders agree on 1,407 bytes, including all 50 fresh CRC packets in order,
without decoder warnings. Those packets witness the initial commanded interval;
they do not establish source-owner liveness or arbitrary all-channel continuity
through the entire unplanned long run.

The GUI then closed with native process exit code zero. The exit-handle wrapper
had first checked an independent process exiting with code seven, avoiding the
old missing/nullable exit-code record. The temporary task was removed. All 11
original files were copied and hash-verified before normal VM shutdown. A final
non-acquiring identification check found the same DLA address 4.16 / 5000 Mb/s;
USB and serial owners were absent. Both installed launchers and all 1,248 local
source hashes, HEADs and dirty differences remained unchanged. The GUI candidate
remains uninstalled; the first-data/replay and broader Windows qualification
issues remain open. Only this report is published; raw data, desktop and VM
material remain private.

## Update: controlled UART witness cohort

A separate, fixed two-acquisition cohort addressed the demonstrated source
preflight gap. It did not repeat the closed five-acquisition cohort. The old
readiness file was still present with a dead owner and zero ECHO commands. A
private pre-start check now requires a live owner, ACKs for three consecutive
ECHO commands, and latest command activity no older than 500 ms. It rejects
the old artifact, missing or
partial acknowledgements, a dead owner and expired activity; a current ordered
control is accepted. This check does not by itself prove electrical capture.

The source was owned by the same bounded foreground orchestrator as each Run,
with commands, received serial bytes, acknowledgements and final closure kept.
There was no detached source waiting during GUI configuration. Neither product
code nor installed binaries changed. The limit was two physical acquisitions,
zero retries; a failure ended the remaining cohort. Serial connections were
opened separately per OS; no uninterrupted MCU uptime is claimed.

| New control | Verified outcome |
| --- | --- |
| Linux Stream, 5 MS/s, continuous with host-count Stop at 25,165,824 samples | Complete 100,663,296 bytes (100 MiB); RAW, original feed and actual Segment storage identical; D0 periods 4–6 with zero outliers; all 50 fresh CRC packets 876601–876650 in order, two UART decoders identical, zero decoder warnings and all source ACKs |
| Actual Windows GUI, Stream, 5 MS/s, finite 50-million-sample request | First-data timeout with zero RAW and zero worker-unconsumed bytes; source pre-start gate passed, producer stayed alive, all 90 serial commands 876801–876890 acknowledged; no captured CRC packets or Windows continuity qualification |

The Linux observer recorded 224 events with exact request/completion order and
byte conservation, no lost or truncated events. HEADER, END, Stop, drain and
close passed. The last preflight command preceded the first USB OUT by at most
285.559 ms, cross-checked using the retained host clock pair. The new continuous
control deliberately differs from the earlier failing finite SETUP. It does
not retest or repair that finite timeout.

The Windows SDK made four ordered 1 MiB reads, each returning zero bytes with
status zero / Windows error zero. The first-data deadline was reported at
approximately 3 s; the worker then accepted Stop, obtained two empty drain
reads, retired and closed both traces without a diagnostic failure. The GUI
showed `Capture incomplete`. A native Save attempt reported that a logic channel
without data cannot be saved; no export file was created. No empty payload or
replay result was fabricated.

Windows serial command emission spanned approximately 0.5–9.4 s after the native
Run request. Some commands followed the early acquisition Stop; the 90 ACKs are
control-port evidence, not 90 captured packets or an electrical D22 claim. This
attempt excludes a missing source process as its explanation. It does not
establish the reason for the original producer termination or the acquisition
failure. A Windows kernel USB trace was not recorded for this new attempt.

All three logged pre-Stop / SETUP / ARM 54-byte prefixes match the corresponding
prefixes of the earlier successful Windows 200 MB acquisition. This compares
the logged active fields, not full kernel-observed 2048-byte transfers. Different
outcomes with these same fields do not establish a deterministic setup defect,
a firmware cause or a transport fix. The first-data and replay issues remain
open, and the GUI candidate remains uninstalled.

Cleanup also retained a laboratory sequencing failure: the initial archive step
found the GUI process still running, and VM shutdown was requested before copying
finished. All original guest files were subsequently recovered after booting
without DLA assignment; their hashes were verified and the temporary task was
removed. The launcher exit record is absent, so orderly GUI exit zero is not
claimed. Backend Stop, quiet drain and worker retirement had already completed.
A final normal VM shutdown left USB and serial owners absent; the DLA remained
at address 4.16 / 5000 Mb/s and answered the DL32 identification query. There was
no DLA reset, hub cycle, firmware write or extra acquisition during recovery.

Raw data, serial and USB traces, desktop/authentication evidence and VM material
remain private. Only this reviewed report is published. The earlier results
below retain their original scope and failures.

## Earlier desktop cohort: five acquisitions, zero retries

The real Windows desktop GUI completed four planned acquisitions and preserved
all captured data through native Save As, file reopening and another Save As.
This is **partial qualification, not release approval**: the Windows UART source
producer stopped before emitting any numbered messages, and the final Linux
return control timed out without sample data. No new GUI was installed.

## Scope and fixed plan

Linux runs in VMware; Windows runs in a nested KVM VM. The device is the
FNIRSI DLA-32 (`1a86:5537`), Linux USB path `4-2`, address `4.16`, negotiated
5000 Mb/s. All 32 channels were enabled, threshold 1.6 V, Stream mode. The DLA
firmware version is unknown. Wiring and firmware were unchanged. The source
preflight reported v8, PWM 1 MHz / 50%, camera enabled, SD disabled.

The cohort was capped at five physical acquisitions and zero retries. Windows
owned the DLA exclusively for its four acquisitions. Linux acquired it only
after the Windows hardware session closed and the guarded USB handover finished.
The QEMU reset guard and `guest-reset=false` were retained. There was no hub
power cycle, DLA reset, firmware write or simultaneous acquisition.

The actual desktop executable was SHA256
`206d85840def2907c0e04733614c2854afd2ee6533380e7e5dfe2df9fe522f05`.
Its 407 application snapshot files match the current source snapshot; the one
changed regression test was already qualified separately. Runtime hashes were:

| Component | SHA256 |
| --- | --- |
| Windows libsigrok | `bf0a6c78b1ee4d8f6a3e2c5a0ca234cd2fef283419551023830f40d77bbad3a6` |
| Windows libsigrokcxx | `a1fc7442465f95c7deb5692e15e68df656ab110660fbe62edbe1d579dfb94be6` |
| Windows libsigrokdecode | `628767ce4f2f9d19185860a4ee02917a9f069e28bf7ad2fc666d3cbe633de0b3` |
| Linux libsigrok | `84c0c7da14777f27871587dfbda1a0645231d051bc10d51feb089c5769757890` |

## Desktop observations

The software trigger used OR between D0 HIGH pulse <600 ns and D22 HIGH pulse
>10 s. Windows contained 200,000 samples with 50% pretrigger. Hardware triggering
was disabled. The original setup was restored through the normal GUI command
and inspected in the editor before capture. A native Save Session Setup kept
the same trigger conditions, OR, window/pretrigger and capture-count values.
This does not claim a second GUI restore of the saved output.

| Acquisition | Requested | Retained result |
| --- | --- | --- |
| Windows 1 | 5 MS/s, three windows | Three complete 200,000-sample windows; 2,097,152 RAW bytes |
| Windows 2 | 50 MS/s, three windows | Three complete 200,000-sample windows; 2,097,152 RAW bytes |
| Windows 3 | 50 MS/s, 10,000-window target; native Stop within 1 s | Stop requested after 0.292209366 s; seven windows retained, global status `stopped`; last window has 186,137 samples and status `stopped`; 5,242,880 RAW bytes |
| Windows 4 | 5 MS/s, finite 50 million samples | Complete 200,000,000-byte saved LOGIC payload; 200,278,016 RAW bytes |
| Linux return | 5 MS/s, finite 25 million samples | First-data timeout after approximately 3 s; zero RAW and LOGIC bytes; no retry |

Windows 4 was amended **before starting** from the planned 25 million samples
(100 MB) to the supported native preset of 50 million (200 MB). The GUI sample
count selector is not editable; attempts to enter 25 M did not start a capture.
The original plan and the amendment are both retained. Linux's requested
25-million-sample return control was unchanged.

Each Windows trace closed with no decoder-input tail or trace failure. Each
Stop was accepted, the worker completed its quiet drain and retirement, and
its unconsumed RAW sidecar was retained separately. No sidecar byte was silently
inserted into the saved data.

Independent bit-plane conversion of original RAW equals every saved 32-bit
word. Each of the 13 windows equals its declared RAW slice, including overlapping
windows and the partial last window. Origins, frame order and rates are exact.
Every targeted complete window places the trigger at sample 100,000. The D0
trigger marks the falling edge closing a HIGH pulse shorter than 600 ns.
D0 rising-edge periods stay within 4–6 samples at 5 MS/s and 49–51 at 50 MS/s;
the full 50-million-sample return capture has no D0 period outside 4–6.

The same executable reopened all four files in the user desktop with device
scanning disabled. Native Save As created new outputs. All 13 window payloads
and the 200 MB finite payload match byte for byte after replay; metadata,
origin, trigger, order, status and error values are preserved. ZIP CRCs, member
lengths and manifest SHA256 values were independently checked. Container hashes
may differ because ZIP timestamps change. Original exports remained unchanged.
The stopped partial window remains stopped after this round trip.

## Qualification failures and limits

The independent serial source process was externally terminated (exit 143).
Its preflight and initial serial bytes survive, but its ECHO command journal is
empty and neither its postflight nor its final-result file exists. The reason
for termination is unknown. A readiness file was incorrectly treated as enough
to proceed; it proved preflight only, not ongoing emission. Future source checks
must verify actual command emission and process liveness before each capture.

Consequently **none of these Windows captures qualifies fresh D22 counter
continuity**. The long capture contains zero numbered CRC packets. Two UART
decoders agree, but agreement without fresh packets does not meet the planned
acceptance criterion. The independent RAW/storage comparison and the limited
D0 witness remain valid; neither proves arbitrary electrical continuity across
all 32 channels. No replacement Windows acquisition was made.

The final Linux return used a synchronous producer: 31 fresh commands
900001–900031 were acknowledged by the unchanged source, but no sample bytes
arrived. HEADER, error metadata, END, Stop, two empty drain reads and close were
retained; cleanup completed without the probe watchdog. A loss-free Linux USB
observer recorded 34 events / 17 closed URBs, zero sample payload and eight
pending IN requests. The 133 microseconds without pending IN occurred after
request retirement, immediately before Stop; it does not explain three seconds
without data. This observer is inside Linux VMware, not a physical bus analyzer.

All four command packet hashes match a previous successful Linux 5 MS/s,
100 MB control. Different outcomes with identical commands do not establish a
firmware cause, a deterministic setup defect or a transport fix. The failing
return remains a failure; Linux recovery is not claimed. Stream replay and
first-data failures remain open. No rate filter, retry, delay or reset was
added to product code.

The desktop GUI processes closed and both temporary GUI tasks were removed.
The replay task returned zero. The first hardware launch wrapper recorded a
null exit code, so its exit code is not claimed as zero; its process was absent
before handover. USB and serial ownership were released. Existing product
sources, dirty work, installed binaries and the Windows launcher were preserved.
Raw captures, source logs, USB traces, desktop images and VM credentials stay
private; this publication contains only the reviewed report.

## 6 November: cumulative one-IN receiver service from four closed captures

An offline ledger now accounts for all **2671 callbacks and 2667 resubmission
gaps** in the four original one-IN, 50 MS/s, 400 MB accepted-LOGIC captures.
There were **zero new acquisitions, source commands or VM boots**. These are
Linux VMware observations; Windows KVM was not used and no common product code
changed. The original DLA path was 3-2 at 5000 Mb/s, with the same v8 source
and wires. Firmware was unknown/unqueried. The helper is the db7 C API plus Qt
Segment helper; it does not load libsigrokcxx. Private cores remain 064f
(1 MiB) and f497 (512 KiB), with b889 initial-prefix observation in the last
two cases and the separate sampler only in the fourth.

The receiver window is **kernel ARM completion to the final positive active
RAW completion**. It partitions exactly into intervals with an IN request
pending and completion-to-next-submit gaps:

| Original case | Callbacks | Receiver window ms | IN pending ms | No IN pending ms |
| --- | ---: | ---: | ---: | ---: |
| 1 MiB necessity control, witnesses pass | 382 | 2918.709 | 2315.473 | 603.236 |
| 512 KiB control, replay negative | 763 | 3413.370 | 2343.160 | 1070.210 |
| 512 KiB prefix observer, replay negative | 763 | 3337.916 | 2340.692 | 997.224 |
| 512 KiB independent sampler, replay negative | 763 | 3156.482 | 2336.832 | 819.650 |

Original request order, depth one and absence of hidden zero-byte active IN
requests are checked. Full RAW totals remain 400556032 B and 400031744 B,
respectively; surplus is preserved. The last callback's service lies outside
this receiver window and is retained explicitly, never assigned zero. Setup,
shutdown and save time are outside it as well. There is no proportional
trimming of the last request's duration.

Each closed gap is divided in source order: **K**, kernel completion to
callback timestamp argument; **A**, callback argument to payload argument;
**PL**, payload argument to payload logger timestamp; **B**, payload logger
timestamp to LOGIC logger timestamp; **C**, LOGIC timestamp to next submit.
All entries below are cumulative elapsed milliseconds:

| Phase | 1 MiB | 512 KiB | Prefix observer | Independent sampler |
| --- | ---: | ---: | ---: | ---: |
| K | 184.359–203.409 | 182.244–231.774 | 192.591–234.501 | 182.654–229.136 |
| A | 18.603 | 357.865 | 212.854 | 37.662 |
| PL | 1.686–20.736 | 2.362–54.940 | 5.340–51.822 | 5.139–57.717 |
| B | 160.753 | 211.457 | 232.886 | 241.172 |
| C | 218.785–237.835 | 263.704–316.282 | 307.071–353.553 | 300.445–353.023 |

**The extrema in different cells are correlated and must not be added as
simultaneous bounds.** Unknown wire/logger epochs remain a joint feasible
polygon constrained by all callback, command and synchronous-read brackets
and source GLib order. Aggregate affine forms are summed before bounding;
epoch coefficients cancel exactly, conserving each gap and the entire
window. Cross-kernel quantization allowance is 1 µs. This constant-offset
clock model is compatible with all retained brackets; it is not an independent
guarantee against every clock adjustment. Existing prefix-observer epoch
brackets agree but do not selectively narrow only the observed cases.

A contains the callback logging/dispatch path; B contains RAW writing,
conversion and the next logger prefix; C contains LOGIC logging, feed/Segment
storage and return/resubmit. These intervals **do not isolate CPU, allocation,
logging or write-syscall cost**. All 38 Segment chunk-fill callbacks per case
remain identified. Conservative budgets through the first containing D0-fault
completion exclude service of that callback; they do not establish an exact
electrical splice or a host-clock sample-zero epoch.

The passing necessity control also receives over 2.918709 s, longer than the
nominal 2 s sampling duration. Consequently, receipt slope below nominal
sample payload rate alone does not diagnose loss or imply a FIFO capacity or
overflow threshold. Pending-request time includes possible device, bridge,
kernel and VM waits; it is **not physical USB wire utilization**. In the
independent-sampler replay, the previously bounded individual no-IN gaps
(at most 7.654 ms) nevertheless total 819.650 ms. Cumulative service remains
an open hypothesis, not a demonstrated cause. Different quanta and auxiliary
observers make this an observational comparison, not an isolated intervention.

Seven contracts pass, including an independent exhaustive small-domain clock
oracle and a correlated-sum counterexample; the read-only closure guard checks
CSV/byte continuity and exact conservation. A generator-reuse error caught
by the first contract run was corrected before actual-data analysis, with
the failed run preserved. All 51 pinned inputs, original 245/145/157-file
cohort manifests and the 1248-file product/installation baseline are intact.
Local evidence is `artifacts/receiver-service-ledger-20261106/` (23 frozen
files); captures remain private. There is **no product fix, new rate
qualification or arbitrary 32-channel electrical guarantee; AUD006 remains
open**. A distinct DEBUG-logging necessity test first needs a quiet helper
path that preserves the RAW oracle and ARM/source/END/quiet/closure evidence.

## 6 November: native quiet lifecycle witness and conditional DEBUG/WARN pair

A distinct Linux VMware cohort is closed: **three acquisitions, zero retries
and zero Windows boots**. This tests DEBUG text emission separately from
unchanged unbuffered RAW writing. The DLA remained on USB 3-2 at 5000 Mb/s;
the v8 source, wires, firmware and hub were unchanged. DLA firmware remained
unknown/unqueried. There was no injected pause, interposer or live sampler.

The old LAB guard first reproduced a dependency on DEBUG lifecycle text.
A private copy of the db7 C API plus Qt Segment helper now records six scalar
callback-order counters and an invalid-order flag. It adds no per-callback
file writes, allocations or timestamps; its shared nonzero CPU overhead was
not isolated. The two helpers compile identical source and original storage
objects with only `SR_LOG_DBG` versus `SR_LOG_WARN`. Both high-rate phases
use the same private f497 core, one 512 KiB IN, finite 50 MS/s/400 MB,
32 channels/1.6 V/no trigger, and the predeclared 50 Hz fresh source markers.
They do not load libsigrokcxx. WARN was conditional on a corroborated current
DEBUG replay, with no retry. The final reserved phase uses original db7 and
installed core 84 at 5 MS/s/100 MB.

| Phase | Accepted LOGIC bytes | D0 faults | CRC occurrences / unique | Outcome |
| --- | ---: | ---: | ---: | --- |
| New witness, DEBUG | 400000000 | 2 | 110 / 78 | Corroborated replay |
| Same witness, WARN | 400000000 | 0 | 100 / 100 | Fixed finite witness contract passes |
| Installed original, 5 MS/s return | 100000000 | 0 | 50 / 50 | Reserved return passes |

The DEBUG failure retains D0 periods 15/36 and **90088448 B of accepted RAW
repeated at +134217728 B**: `[175693824,265782272)` equals
`[309911552,400000000)`, SHA256
`112948a23f2262f4f2cb6d7f70dbc552b465077d5e43113acefb2b1974687bcf`.
Fresh pairs 774169/774170 pass local independent/SDK full-STOP validation in
both copies. Global STOP decoders also agree, with no unchecked EOF frame.
The 32 duplicate CRC occurrences remain failures, not repaired data.

WARN preserves **100 consecutive CRC messages, 775101..775200**, D0 periods
49..51, equal independent/SDK STOP output and no unchecked EOF frame. All
97794049 contained origins of the fixed 44.119040 ms witness window are
covered. Historical 20 ms profiles are not replaced or reclassified.
**WARN stderr is exactly 0 B; DEBUG stderr is 338527 B. RAW writing remains
enabled in both.** Each full RAW file contains 400031744 B, including the
31744 B surplus, and equals the complete original active kernel payload in
order. Independent all32 conversion equals feed and actual Segment storage;
this does not certify arbitrary 32-channel electrical continuity.

Both new helpers report HEADER ordinal 1, 763 LOGIC packets at ordinals
2..764, and unique terminal END ordinal 766, with no invalid order; effective
native levels are 4 and 2. Quiet closure does not fabricate DEBUG text. The
original reserved-return guard remains strict. The 5 MS/s return preserves
50 consecutive CRC messages 776101..776150, conversion and END/quiet.
All source ACKs are unique/in order: 177/151/51 live commands respectively,
plus three preflight commands per phase. Native/controller original waits
return zero; END, kernel quiet and observer finalization precede source close.
DLA/source are free and Windows is off.

| Kernel ARM to final active RAW completion | Window ms | IN pending ms | No-IN sum ms | Largest no-IN gap ms |
| --- | ---: | ---: | ---: | ---: |
| DEBUG | 3503.854 | 2341.961 | 1161.893 | 310.488 |
| WARN | 2970.198 | 2335.187 | 635.011 | 5.721 |

These are recorded kernel budgets, not isolated logging/CPU cost, physical
USB utilization or FIFO occupancy. Terminal callback work, setup/shutdown
and save remain outside the window. No fputs interposer was used here: the
310 ms gap is not an isolated function duration.

The native witness is qualified on these Linux captures. Preparation passes
33 guards using ten actual callback/Segment sequences, both 20 MiB + 12 B
storage ABI checks and eight policy guards. The 60 preparation files, 190
physical evidence files and 1552 inputs are frozen; product 1248 and installed
modules/launchers are unchanged. Two manifest path-normalization errors,
before I/O and after closure, are preserved and corrected without changing
captures or criteria. Private evidence remains local under
`artifacts/quiet-debug-{preparation,physical}-20261106/`.

**This conditional single ordered pair does not establish DEBUG necessity,
causal sufficiency or a product fix.** Scheduling, VM/device state and source
phase remain possible influences. WARN passes this finite necessity-test
contract; no general high-rate or arbitrary-channel qualification follows.
The private helper/core is not promoted. Windows exception: Linux/POSIX LAB
diagnostic and original kernel oracle, no common product change; Windows KVM
is untested here. **AUD006 remains open.** The next prerequisite is auditing
the existing Windows quiet/no-WIRE path with the qualified GUI and independent
WCH evidence, rather than transferring the Linux result across platforms.

## 6 November: INFO3 GUI/WCH prerequisite, software only

The Windows evidence path can reduce per-packet DEBUG emission while keeping
rare native lifecycle records at **INFO3**. **WARN2 remains unsupported by
this LAB observer**, since it hides the existing WCH Stop/retirement records.
Product logging defaults are unchanged.

A private copy of the original 477-source core adds **two INFO record sites**
in `fnirsi-dla32/api.c`: after successful HEADER delivery and after the END
delivery call returns. HEADER reports the configured rate/mask/unitsize/limit;
END reports the existing driver sample counter, status/error flag and actual
return code. A record label alone is not success: the guard requires result
zero. There is no new state, per-packet work, recorder or runtime option.
These logger calls can block or reenter; zero overhead is not assumed.
The source change is confined to private builds and is not installed.

The finite all32 guard requires unique current HEADER/Stop/worker/END/Stopped
records in order, correct configuration/counts and successful native WCH
retirement. Independent original kernel ownership/quiet and unconsumed-byte
bounds remain mandatory before releasing the source. The backend sample
counter **does not prove GUI acceptance or electrical integrity**; native
Save and independent kernel-to-LOGIC comparison remain separate requirements.

**47 protocol/ownership/framing contracts pass on each OS**, including every
byte split, missing/duplicated/reordered records, failed delivery, partial
timeout closure and preservation of coalesced later GUI bytes. Their positive
fixture is explicitly synthetic, not a native hardware observation. The
observer has no live per-packet FLOW progress and does not establish freshness
without a recorded pre-Run offset and original process/module identity.

Linux VMware and Windows KVM builds use the same 477-source manifest, with
only the API probe additions. **122 native C checks per OS and both C++ context
cleanup suites pass**; intentional negative cleanup messages are retained.
Three actual Linux Demo cases and four distinct Windows Demo cases preserve
independent counter bytes. The Windows 2 Msamples/8 MB case verifies the actual
live loaded private core, SHA256
`a17b662763d7e74e6e046a5e6b5c945c826c914cc160e329335aba2f8876fafc`.
Demo output cannot satisfy the DLA native guard.

The initial Windows bootstrap missed UCRT64's `pkg.m4`; correcting
`ACLOCAL_PATH` preserved the same source. Two errors in my ABI command
(separator, then global/channel-group scope) were correctly refused and
preserved before the corrected run passed. Windows totals are twelve CLI
starts: ten successful Demo runs and two rejected commands, not physical
retries. Guards and acceptance criteria were not relaxed.

There were **zero physical acquisitions, source operations, DLA assignments
or native GUI executions**. One Windows boot was used for build/tests and
normally shut down. All 52 copied Windows originals match SHA256/size and ZIP
CRC; eight installed Windows GUI/DLL/launcher files and the Linux 1248-file
product/install baseline remain unchanged. DLA/source are free. Local evidence
is `artifacts/gui-info-lifecycle-20261106/` (161 frozen files); private images
and captures are not published.

**Actual DLA emission and GUI/WCH integration remain UNQUALIFIED.** The next
step is the finite GUI adapter with fresh original PID/image/module hashes
and log offset, retained trailing bytes, current kernel ARM/Stop/quiet and
native Save evidence, before a distinct fixed physical qualification plan.
No private image is promoted, no rate is newly qualified and **AUD006 remains
open**.
