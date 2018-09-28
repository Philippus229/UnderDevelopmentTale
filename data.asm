.include "hdr.asm"

.section ".rodata1" superfree

gfxfrisk:
.incbin "frisk.pic"
gfxfrisk_end:

frisksprite:
.incbin "frisk.pal"
frisksprite_end:

.ends

.section ".rodata2" superfree

ruinsgfx: .incbin "start_test.pic"
ruinsgfx_end:

.ends

.section ".rodata3" superfree

ruins: .incbin "start_test.map"
ruins_end:

.ends

.section ".rodata4" superfree

ruinspal: .incbin "start_test.pal"

.ends

.section ".rodata5" superfree

ruinscol: .incbin "start_test_col.clm"
ruinscol_end:

.ends

.section ".rodata6" superfree

ruins2gfx: .incbin "flowey_test.pic"
ruins2gfx_end:

.ends

.section ".rodata7" superfree

ruins2: .incbin "flowey_test.map"
ruins2_end:

.ends

.section ".rodata8" superfree

ruins2pal: .incbin "flowey_test.pal"

.ends

.section ".rodata9" superfree

ruins2col: .incbin "flowey_test_col.clm"
ruins2col_end:

.ends