# Questions

1. The first instruction after PC powers up is a ljmp. It jumps to an earlier position in BIOS. Then it disables interrupt, and set the string operation direction (cld). It turns on A20. After that, it loads interrupt descriptor table (lidt) and global descriptor table (lgdt). Finally, it sets the lowest bit of CR0 and turns the processor into protected mode.

2. Exercise 3
    1. 3.1

        ```shell
        [   0:7c2d] => 0x7c2d:  ljmp   $0x8,$0x7c32    // ljmp loads %cs, cause processor switch from 16 to 32 bit
        => 0x7c32:      mov    $0x10,%ax               // start executing 32-bit code
        ```
  
    2. 3.2

        ```shell
        => 0x7d61:      call   *0x10018                // last instruction executed by bootloader
        => 0x10000c:    movw   $0x1234,0x472           // first instruction executed by kernel
        ```

    3. first instruction of the kernel is at 0x10000c
    4. The kernel elf file has a field for number of entries in the program header table. Each program segment stores a field for its own page offset, which can be converted to sector number. Then read the corresponding dist sector into sepcified address.
  
3. Exercise 5

    ```shell
    [   0:7c2d] => 0x7c2d:  ljmp   $0x8,$0x7c32    // first broken instruction after change linker address, ljmp uses absolute address
    ```

4. Exercise 6
  
    ```shell
    // when bootloader start
    0x100000:       0x00000000      0x00000000      0x00000000      0x00000000
    0x100010:       0x00000000      0x00000000      0x00000000      0x00000000
    // before bootloader enters kernel, it's the kernel code
    0x100000:       0x1badb002      0x00000000      0xe4524ffe      0x7205c766
    0x100010:       0x34000004      0x0000b812      0x220f0011      0xc0200fd8
    ```
