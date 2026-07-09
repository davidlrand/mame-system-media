# Die Wiederbelebung des PC-MX2

*English version: [BRINGUP.md](BRINGUP.md)*

*Ein gekürzter Bericht über die MAME-Inbetriebnahme 2026. Zu keinem Zeitpunkt
stand PC-MX2-Hardware zur Verfügung: keine Schaltpläne, kein Logikanalysator,
kein Oszilloskop. Die Maschine wurde rekonstruiert aus Baugruppenfotos,
ROM-Auslesungen, vierzig Jahre alten Diskettenabbildern, dem deutschen
Handbuchsatz, und logischem Denken.*

## Das Ausgangsmaterial

Die ROMs wurden 2022 ausgelesen (CPUAP-Monitor Rev 3 und Rev 9.0, dazu die
Firmware von SERAD, Storager, OMTI, ExeLAN und SERAG) und zusammen mit
IMD-Abbildern der SINIX-V2.0-Distributionsdisketten (datiert März bis Mai
1986) im OldComputers-Archiv bewahrt. MAME enthielt bereits ein Gerüst des
`pcmx2`-Treibers mit dem Prozessorpaar NS32016/NS32082. Alles Übrige (der serielle
E/A-Prozessor SERAD, der Storager-Plattencontroller, das Terminal 97801)
musste aus den Erwartungen der Firmware selbst aufgebaut werden: die
8085- und 68000-Programme laufen lassen, beobachten, wonach sie tasten, und
liefern, was die Baugruppen geliefert haben müssen.

## Der Aufstieg

**Die Konsole.** Die SERAD-Baugruppe (8085 + SCN2681-DUARTs) spricht mit dem
Rechner über ein Multibus-Postfach. Ihre Firmware synchronisierte die Konsole
nicht, bis ein Taktfehler gefunden war: Der 8085 lief durch eine doppelte
Teilung des 20-MHz-Oszillators mit halber Geschwindigkeit. Eine Zeile behob
es, und zum ersten Mal meldete sich der Kernel: `SINIX-M-C V2.0 (Rev. 266)`.

**Die Platte.** Der Storager (ein intelligenter Controller mit 68000) bedient
Diskette und ESDI-Winchester über IOPB-Postfächer und eine
Scatter-Gather-Registerdatei. Seine Fertigmeldungen mussten aus der
68000-Firmware selbst zurückentwickelt werden; die entscheidende Korrektur
war ein pegelgehaltener Interrupt, verankert an der eigenen
Kanaldeskriptor-Warteschlange der Firmware, gefunden, indem man dem 68000
beim Warten auf sein Fertig-Wort zusah und zurückverfolgte, wer es hätte
schreiben müssen.

**„/: file system full".** Mit Konsole und Platte am Leben bootete SINIX,
und starb: `/: file system full`, `panic: init died`, Sekunden nach jedem
Start. Dieser Fehler verschlang den Großteil der Untersuchung. Das
Wurzeldateisystem der Installationsdiskette hat genau vier freie Blöcke;
irgendetwas fraß sie beim Start auf und tötete init. Die Jagd schloss, durch
Messung, der Reihe nach aus: Schreibfehler auf der Platte, DMA-Platzierung,
MMU-Übersetzung, das Swap-Subsystem (durch eine einkompilierte Sperre
abgeschaltet, auf realer Hardware identisch), die Speichergrößenermittlung
und den Seitenumlagerer (dessen Seitenfehler sich als einwandfrei bedient
erwiesen). Das Dateisystem arbeitete korrekt: Die vier Blöcke verbrauchte der
Kernel selbst, indem er einen Speicherauszug von init schrieb; `SIGSEGV`,
Standardbehandlung, `core()` in ein fast volles Miniwurzel-Dateisystem. Die
Frage wurde: Warum stürzt init auf einer Maschine ab, auf der genau diese
Diskette bootet?

**Den Code lesen.** Die Antwort kam aus dem Archiv, nicht aus dem Emulator.
Kernel und die zugehörige `vmsymbols`-Namensliste wurden aus dem Dateisystem
der SINIX0-Diskette extrahiert (ein FFS nach 4.2BSD-Art mit Verzeichnissen im
V7-Format, eine sehr siemenstypische Mischform), und `/etc/init` selbst
wurde herausgeholt und disassembliert. Seine erste Handlung nach `exec` ist
das Ablegen von `environ` über das **Static-Base-Register**, und SB enthielt
Unsinn. Die Programmdatei trägt ihre eigene
NS32000-Moduldeskriptortabelle bei virtueller Adresse 0x20, im eigenen Text:
korrektes SB = 0x1c00. Der stattdessen gelieferte Wert war, aufs Byte genau,
der Inhalt der *physikalischen* Adresse 0x20, Kernelspeicher.

**Der CPU-Fehler.** Bei RETT/RETI lädt der NS32000 SB neu aus dem
Moduldeskriptor bei MOD. Das Series 32000 Programmer's Reference Manual legt
die Reihenfolge exakt fest (RETT, S. 6-171): PC vom Stapel, MOD vom Stapel,
PSR vom Stapel; *dann* den Deskriptor nach SB kopieren, dann den „durch
Schritt 3 neu ausgewählten Stapelzeiger" anpassen. Das Deskriptor-Lesen
gehört der *Ziel*-Betriebsart. MAMEs Prozessorkern las es im
Systemadressraum; unter SINIX' Dual-Space-MMU-Konfiguration, bei der Rückkehr
in den Benutzermodus, traf das Kernelspeicher statt des Prozessabbilds, und
gab jedem Benutzerprozess bei jeder Systemaufruf-Rückkehr ein zerstörtes SB.
Der Kernel war fehlerfrei; das CPU-Modell nicht.

**Die Korrektur.** Zwei Zeilen: das SB-Deskriptor-Lesen mit dem U-Bit des
wiederhergestellten PSR ausführen und die Dual-Space-Logik der NS32082 selbst
die Seitentabelle wählen lassen. Die gesamte NS32000-Flotte in MAME wurde auf
Rückschritte geprüft (ICM-3216, PC532, Opus und PD-32 booten unverändert),
und der PC-MX2 erwachte:

```
SINIX-M-C V2.0 (Rev. 266)
System 734k User 3362k
using 143 buffers containing 417792 bytes of memory

INSTALLATION EINES SINIX-SYSTEMS
Herzlich Willkommen zur Selbstinstallation Ihres SINIX-Systems
```

Die Speicherwerte stimmen exakt mit der realen 4-MB-Maschine überein. Vierzig
Jahre nachdem die Disketten geschrieben wurden, wartet der
Installationsdialog auf sein `j <CR>`.

## Was der Versuch bewies

Jede zunächst verdächtigte Schicht (Dateisystem, Seitenumlagerer, MMU-Lauf,
Platten-DMA, Swap-Code) wurde am Ende durch Messung *entlastet*, und der
Fehler lag drei Ebenen unter dem Symptom, in zwei Zeilen Prozessorsemantik,
die ein Handbuch von 1984 schlicht und deutlich dokumentiert. Die Disketten
waren die Schaltpläne; der Emulator war der Logikanalysator; die Handbücher
waren die Referenz. Softwarebewahrung hat Hardware-Archäologie möglich
gemacht.
