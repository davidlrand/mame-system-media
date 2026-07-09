# PC-MX2 DIP switch / strap audit

Source of truth: `siemens/storager/mx300-9783_system-manual_u64510-j-7600_1988-89.pdf`
(switch-assignment sections III.3-4/5 Storager, III.3-10 SERAD D279, III.3-11 SERAG D312)
plus `siemens/transdata-970_9780_wartungshandbuch.pdf` (SINIX installation chapter) and the
`pc-mx2_pc2000_9780_logik.pdf` CPUAP sheets. The MX300 manual is later-era; addresses there
carry a 0x2000000 prefix (`2EF7000`) and some defaults differ (noted below).

## CPUAP (S26361-D333)

| Switch | Manual meaning | MAME default | Confidence | Boot impact |
|---|---|---|---|---|
| S7:8 | Open: boot-loader output via diagnostic plug, no SERAD fitted | Off = SERAD console (production) | High | Selects console path; Off is the delivered-system configuration. Comment fixed 2026-07-03 (previously said "On = bring-up default" while defaulting Off). |
| S7:7 | Open: enter monitor after self-test | Off = Disk boot | High | Off = auto-boot (production). |
| S7:1 (loc S7:2) | Open: no reboot after system crash | Off | Medium | Crash-loop behavior only. |
| S7:2-6 | Not identified in OCR'd sheets | Off | Low | Unknown; no observed sensitivity. |
| S8 | IRQ7 routing | ICU IR11 | Medium | Alternative NMI routing unused by the SINIX flow so far. |

## SERAD (S26361-D279)

| Switch group | Manual meaning | MAME model | Confidence | Boot impact |
|---|---|---|---|---|
| S1-S8 | Multibus I/O address (bits F-A); table: board1=1000, 2=1100, 3=1200, 4=1300, 5=0F00 | "I/O Address" DIP, default 1000 | High | Matches the manual's board-1 slot; the kernel probes all five. |
| S18-S33 | Multibus base (mailbox) address; board1=EF7000 (MX300: 2EF7000) | "Base Address" DIP, default EF7000 | High | Matches; kernel probes EF7000..EF3000. |
| S9-S16 | Multibus interrupt level, one closed (S9=INT0..S16=INT7); MX300 = all open (polled) | **"Interrupt Level" DIP added 2026-07-03**, default INT3 | High (mapping), Medium (MX2 default) | The MX2 SINIX driver services the board at ICU IR4 ("vector 4 ipl 5" banner) = Multibus INT3 via the CPUAP's int3->IR4 wiring; INT3 default chosen to match. Manual only states the MX300 (polled) default. |
| S26-S29 | Mailbox size (4KB: S26,S27 closed) | Fixed 4KB | High | Model hardwires the manual's 4KB configuration. |
| S34 | Mailbox selection (position 1-3) | Not modeled | Low | Believed subsumed by the base-address DIP. |
| S17 | pos 1-3 closed (MX300); pos 1-2 = "Console Remote ON, only MX2/MX2+" | Not modeled (behaviorally always-on) | Medium | The MX2 console-through-SERAD option; our firmware ROM behaves as console-capable without a modeled strap. Revisit if a firmware path tests it. |

## Storager (Interphase 3030)

| Strap | Manual meaning | MAME model | Confidence | Boot impact |
|---|---|---|---|---|
| S1:2-8 | I/O address bits 2^9..2^15; Storager1=73F8(7200-73FF), Storager2=75F8 | Fixed: mailbox window 73F8; 8-reg ioreg at 0x800 (MX2 map; kernel banner "sa 0 from 800 to 807") | High | Matches board 1. Second-board support would need the DIP. |
| E24-E38 | INT7..INT0 wire-wrap; E34=INT2 connected to JP8 (level B interrupt) | int_w<2> (= ICU IR3, "vector 3 ipl 5" banner) | High | Matches the documented E34/JP8 strapping exactly. |
| E13-E22 | 16-bit I/O addressing, parallel priority | Implicit in the model | Medium | No observed sensitivity. |
| E42/E43 | 8 I/O register mode | 8-register ioreg modeled | High | Matches. |
| S1:1, S2:2, S2:4-8, JP1-6, JP9-11 | Queue mode operation | Not modeled (IOPB/doorbell mode in use) | Medium | The SINIX V2.0 flow uses the mailbox IOPB path; queue mode unexercised. |
| JP14/JP15 | Both drives ESDI | ESDI assumed | High | — |

## Manual cross-checks that validated the emulation

- Boot output `sr0 found at csr ...ef7000 / sr1..3 not found` (manual p.~3389 OCR):
  absent SERAD slots report absent - confirms the 2026-07-03 XACK-timeout fix
  (empty slots must bus-error, not float).
- "All storager boards must work with the same interrupt number" - single INT2 correct.
- Installation flow (Wartungshandbuch "INSTALLATION EINES SINIX-SYSTEMS"): console
  welcome + self-install via the operator console (SERAD), with a TEST END-only
  fallback when no console processor responds - matching the observed monitor behavior.

## Open items

- S17 "Console Remote" strap: model if a firmware code path is found testing it.
- SERAD interrupt default on a real MX2: manual documents only the MX300 (polled);
  INT3 inferred from the kernel's vector-4 expectation. Verify against an MX2-era
  9780 manual if one surfaces.
- CPUAP S7:2-6 meanings unknown (logik PDF OCR too poor); flag if boot behavior
  ever proves sensitive.
