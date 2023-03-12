        AREA M0CODE, DATA, READONLY, PREINIT_ARRAY, ALIGN=3
		EXPORT m0_image_start
		EXPORT m0_image_end
m0_image_start
        INCBIN ..\\multicore_m0_blinky_ram\\m0_blinky_ram.bin
m0_image_end
        END
