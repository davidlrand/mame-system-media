# MK-83 / MK3000 — maintainer correspondence + MK3000 site notes (2026-06-07)

## Email from Enrico (maintainer, vintagesbc.it) — verbatim
> Dear Dave,
> Before moving from Italy to Spain, I sold or gave away all my old computers.
>
> I have archive files, but my MK83 has always been a problem.
> I received it without any boot software, and I tried for a long time to get it to boot by altering
> the boot sector and the addresses in the system traces, with no success.
>
> The Ferguson Bigboard ROM contains 16 bytes that, when powered on, tell the CPU to copy the EPROM to
> RAM at address F000H, then jump to that address and display the prompt. Many pages remain free,
> before the last one at FF00H, which contains the system variables.
>
> In the MK83, the EPROM is copied to F800H; there are many more entry points, and all the related
> addresses are translated. Now the idea was to modify the CPM system traces to point to F800H.
>
> I managed to get the prompt, but too many things still didn't add up.
>
> For example, the Ferguson Bigboard has an FC1771 FDC (SSSD FD), while the MK83, if I remember
> correctly, has an FD1793.
>
> Therefore, depending on the setting copied to the working memory at FF00h, I found different basic
> configurations, such as different number of tracks, sectors per track, and density type (SS DS).
>
> The MK83, in fact, was part of a box capable of altering the geometry of the disks to be read to
> load programs into an IBM 370, so it had different default settings.
> In short, perhaps due to my own incompetence, I never managed to get a CPM from the Ferguson
> Bigboard to load into an MK83 because I didn't have the original MK3000 software.
>
> Other info: https://www.vintagesbc.it/vintage-computer-board/collezione/mk3000/
> Regards, Enrico

## MK3000 page (https://www.vintagesbc.it/vintage-computer-board/collezione/mk3000/), fetched 2026-06-07
- **System:** MK3000, **ADE Elettronica** (Palazzolo Milanese), from **1983**. Purpose: data-entry front-end for **IBM 370** mainframes (e.g. CNR-CUCE, Pisa) — transfer programs to the mainframe via 5.25" floppies instead of punch cards. The MK83 is the MK3000's motherboard ("non-extended bus" variant).
- **CPU:** Z80A.
- **Support chips:** two Z80A PIO (dual printer ports / I-O mapping / keyboard), one Z80A **SIO/0** (two RS232 channels), one Z80A **CTC** (system timer), **FDC 1797** with **FDC9229BT** data separator (FM/MFM).
- **Drive connectors:** IDC **50-pin (8")** + IDC **34-pin (5.25")**.
- **Drives shipped:** BASF AG 6106 5.25" (180 KB, SS/DD) + Shugart **SA465** 5.25" (720 KB, DS/DD).
- **Memory:** EPROM auto-copied to DRAM at startup at **F800H** (vs Ferguson Bigboard I's F000H) — blocks direct software compatibility.
- No disk images / boot software on the site.

## FDC discrepancy to resolve
- MK3000 page: **FD1797** (+ FDC9229BT separator).
- Enrico (from memory, uncertain): **FD1793**.
- Big Board I (Ferguson): FD1771 (SSSD) — both agree the MK83 is a *newer* 179x part.
- Resolve from MK83-1 (Descrizione Hardware) / MK83-2 (Schemi) PDFs already in this dir.
