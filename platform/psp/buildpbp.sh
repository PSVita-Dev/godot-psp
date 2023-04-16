psp-fixup-imports -o test-94.elf godot 
psp-strip test-94.elf -o strip.elf
pack-pbp EBOOT.PBP PARAM.SFO NULL          NULL NULL NULL          NULL  strip.elf NULL
