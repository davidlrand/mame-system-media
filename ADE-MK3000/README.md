# ADE Elettronica MK3000 / MK83

Preservation and MAME-emulation tree for the **MK3000**, a 1983 multi-format
disk-gateway system by **ADE Elettronica** (Palazzolo Milanese, Italy), built
around the **MK83** Z80A board — a Ferguson Big Board I derivative (Xerox 820
family) that is *not* software-compatible with it (monitor relocates to F800,
not F000; 256K banked RAM; runtime-reconfigurable drive geometry, the heart of
its multi-format mission: reading foreign-format floppies for IBM 370 ingest,
used e.g. at CNR-CUCE Pisa).

All hardware documentation, schematics, the BIOS disassembly listing and the
photographs were provided by **Enrico (vintagesbc.it)** — the maintainer of
the MK3000 — to whom this preservation effort owes its existence. See
`mk3000-correspondence-and-site.md`.

## Layout

| Item | Contents |
|---|---|
| `docs/` | Hardware description, schematics PDF, user manual, CP/M usage, disk-format tables, BSTAM listings, period advertising, the **full BIOS Z80 disassembly listing**, BIOS command/entry-point notes, board photos (Italian) |
| `schematics/` | The six MK3000 schematic sheets (scans) + original archive |
| `roms/` | The dumped MK83 EPROMs (2732 monitor, 2716), as in the MAME `mk83` set |
| `boot-disk/` | Reserved: the reconstructed bootable CP/M disk (in progress — no original MK3000 media is known to survive; the disk is being rebuilt from the BIOS listing and the disk-format tables, by the same byte-provenance method as the Xerox-820-16-8 boot disk) |
| `INDEX.md`, `MK83-MEMORY.md` | Document index and the living bring-up notes for the MAME `mk83` driver |

## MAME status

**Working.** The `mk83` driver was rewritten from these materials (monitor
relocation to F800, 256K banked RAM, FD1797+FDC9229, memory-mapped video,
console select) and the machine boots the reconstructed CP/M 2.2 disk in
`boot-disk/` to an interactive A> (geometry `U21,27,A,2`, then `B1`).
Submitted upstream as mamedev/mame PR #15489, maker attribution corrected
to ADE Elettronica.

Firmware copyright ADE Elettronica; preserved for emulation and historical
reference.
