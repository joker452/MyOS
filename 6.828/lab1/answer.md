# Questions

1. The first instruction after PC powers up is a ljmp. It jumps to an earlier position in BIOS. Then it disables interrupt, and set the string operation direction (cld). It turns on A20. After that, it loads interrupt descriptor table (lidt) and global descriptor table (lgdt). Finally, it sets the lowest bit of CR0 and turns the processor into protected mode.
