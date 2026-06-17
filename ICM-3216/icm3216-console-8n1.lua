-- ICM-3216 console: pin the console DUART (DUART1 channel B) framing to 8N1.
--
-- NSC UNIX reprograms the console to 7E1 once it goes interrupt-driven.  Real
-- hardware drove a 7E1 terminal (the ICM-3216 console was typically a Wyse set
-- to 7E1), but MAME's built-in serial terminal only displays 8N1, so the console
-- turns to garbage and the system becomes unusable.  This script reinstates the
-- previous driver-side workaround as a pure host-side convenience: it does NOT
-- change the emulated hardware, it only rewrites the framing bits the guest writes
-- to the mode register so the in-MAME terminal stays readable.
--
-- Usage:
--   ./mame icm3216 -bios v1283 -autoboot_script icm3216-console-8n1.lua  <media...>
--
-- (If you attach a real 7E1-capable terminal via -serial3, you do not need this.)

local mr_ptr = 0  -- shadow of the channel-B mode-register pointer (0 = MR1, 1 = MR2)

local space = manager.machine.devices[":cpu"].spaces["program"]

-- writes to the channel-B mode/command registers (0xa00050-0xa00057, low byte)
space:install_write_tap(0xa00050, 0xa00057, "icm_console_8n1",
	function (offset, data, mask)
		if (mask & 0x00ff) == 0 then return end
		local b = data & 0xff
		local reg = (offset - 0xa00050) >> 1
		if reg == 0 then               -- MR1B / MR2B (the MR pointer selects which)
			if mr_ptr == 0 then
				b = (b & 0xe0) | 0x13  -- MR1B: 8 data bits, no parity
				mr_ptr = 1
			else
				b = b & 0xf3           -- MR2B: 1 stop bit
			end
			return (data & 0xff00) | b
		elseif reg == 2 then           -- CRB: command 1 (bits 6:4 = 001) resets the MR pointer
			if ((b >> 4) & 7) == 1 then mr_ptr = 0 end
		end
	end)

-- a read of MR also advances the device's MR pointer -- keep the shadow in sync
space:install_read_tap(0xa00050, 0xa00051, "icm_console_8n1_mr",
	function (offset, data, mask)
		if mr_ptr == 0 then mr_ptr = 1 end
	end)
