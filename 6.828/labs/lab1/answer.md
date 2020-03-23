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

5. Exercise 7  
    1. 7.1

        ```shell
        // before movl %eax %cr0
        (gdb) x/8x 0x00100000
        0x100000:       0x1badb002      0x00000000      0xe4524ffe      0x7205c766
        0x100010:       0x34000004      0x0000b812      0x220f0011      0xc0200fd8
        (gdb) x/8x 0xf0100000
        0xf0100000 <_start+4026531828>: 0x00000000      0x00000000      0x00000000      0x00000000
        0xf0100010 <entry+4>:   0x00000000      0x00000000      0x00000000      0x00000000
        // after movl %eax %cr0
        (gdb) x/8x 0x00100000
        0x100000:       0x1badb002      0x00000000      0xe4524ffe      0x7205c766
        0x100010:       0x34000004      0x0000b812      0x220f0011      0xc0200fd8
        (gdb) x/8x 0xf0100000
        0xf0100000 <_start+4026531828>: 0x1badb002      0x00000000      0xe4524ffe      0x7205c766
        0xf0100010 <entry+4>:   0x34000004      0x0000b812      0x220f0011      0xc0200fd8
        ```

    2. `movl %eax, %cr0 line` is the first broken instruction. Because paging is not enabled, this address is interpeted as a physical address, and since there is no RAM  oinsidef it, qemu crashes.

6. Exercise 8
    1. `console.c` deals with low level interactions with console. `printf.c` uses functions it exposed to print data according to the format specified. `console.c` exports `void cputchar(int c)`, `int getchar(void)`, and `int iscons(int fdnum)`. `printf.c` uses `cputchar` to print a char to the console.
    2. 8.2

        ```c
        1      if (crt_pos >= CRT_SIZE) {                                                              // position exceeds the end
        2              int i;
        3              memmove(crt_buf, crt_buf + CRT_COLS, (CRT_SIZE - CRT_COLS) * sizeof(uint16_t)); // all rows moves one row forward
        4              for (i = CRT_SIZE - CRT_COLS; i < CRT_SIZE; i++)                    // last row is empty character with black on white
        5                      crt_buf[i] = 0x0700 | ' ';
        6              crt_pos -= CRT_COLS;                                                            // move current position backward
        7      }
        ```

    3. 8.3

        ```shell
        // assembly code for cprintf
        (gdb) x/10i
        0xf0100972 <cprintf+1>:      mov    %esp,%ebp
        0xf0100974 <cprintf+3>:      sub    $0x18,%esp
        0xf0100977 <cprintf+6>:      lea    0xc(%ebp),%eax          # get ap
        0xf010097a <cprintf+9>:      mov    %eax,0x4(%esp)
        0xf010097e <cprintf+13>:     mov    0x8(%ebp),%eax          # get format string
        0xf0100981 <cprintf+16>:     mov    %eax,(%esp)
        0xf0100984 <cprintf+19>:     call   0xf010093e <vcprintf>
        0xf0100989 <cprintf+24>:     leave
        0xf010098a <cprintf+25>:     ret
        0xf010098b <stab_binsearch>: push   %ebp

        // format points to format string, ap points to stack pointer
        (gdb) si
        => 0xf010093e <vcprintf>:       push   %ebp
        vcprintf (fmt=0xf0101972 "x %d, y %x, z %d\n", ap=0xf010ffe4 "\001") at kern/printf.c:18

        // execution order is vcprintf -> cons_putc to output char on console
        // -> find some %escape sequence -> process flags etc. and use va_arg to get value
        // -> ap moves up along the stack to get next value
        // -> cons_putc the value -> continue parsing until finish (repeat previous steps)
        ```

    4. 8.4

        ```c
        unsigned int i = 0x00646c72;
        cprintf("H%x Wo%s", 57616, &i);         // output He110 World

        // 57616 -> interpreted as octal is 110
        // i as an address in x86 (little endian) -> 72 6c 64 00 -> r l d \n
        // if x86 were big-endian, 57616 no need to change, because its specifier is %x,
        // but i change to 726c6400
        ```

    5. 8.5

        ```c
        cprintf("x=%d y=%d", 3);
        // according to the given implementation of vprintfmt, whatever after after the first argument on the stack that ap
        // points to gets printed. validate this by putting this statement after those in 8.3, y gets the old value of y.
        // I guess if it references an illegal position, then a segment fault will occur
        ```

    6. If GCC pushed arguments on the stack in declaration order, we change cprintf interface to put format string at last to support variable length arugments. Key point is put what we know (format string) on the top of the stack, then we can get number of arguments by parsing it.
7. Exercise 9

    ```shell
    (gdb) si
    => 0xf0100034 <relocated+5>:    mov    $0xf0110000,%esp     // initialize stack here

    // stack is from 0xf0108000 to 0xf0110000, because KSTKSIZE is 8*PGSIZE (defined in memlayout.h)
    // %esp points to stack top the highest address in this case, because stack grows towards lower address,
    // we should put initial value at highest address

    // in entry.S, kernel reserves its stack as follows
    .data
    ###################################################################
    # boot stack
    ###################################################################
        .p2align    PGSHIFT     # force page alignment
        .globl      bootstack
    bootstack:
        .space      KSTKSIZE
        .globl      bootstacktop
    bootstacktop:
    ```

8. Exercise 10

    ```shell
    // The disassembly code for test_backtrace is as follows:
    0xf0100040 <test_backtrace>:         push   %ebp                # save ebp
    0xf0100041 <test_backtrace+1>:       mov    %esp,%ebp
    0xf0100043 <test_backtrace+3>:       push   %ebx                # save ebx
    0xf0100044 <test_backtrace+4>:       sub    $0x14,%esp          # reserve space for next function call parameters
    0xf0100047 <test_backtrace+7>:       mov    0x8(%ebp),%ebx
    0xf010004a <test_backtrace+10>:      mov    %ebx,0x4(%esp)
    0xf010004e <test_backtrace+14>:      movl   $0xf0101920,(%esp)
    0xf0100055 <test_backtrace+21>:      call   0xf0100971 <cprintf>
    0xf010005a <test_backtrace+26>:      test   %ebx,%ebx
    0xf010005c <test_backtrace+28>:      jle    0xf010006b <test_backtrace+43>
    0xf010005e <test_backtrace+30>:      lea    -0x1(%ebx),%eax
    0xf0100061 <test_backtrace+33>:      mov    %eax,(%esp)
    0xf0100064 <test_backtrace+36>:      call   0xf0100040 <test_backtrace>
    0xf0100069 <test_backtrace+41>:      jmp    0xf0100087 <test_backtrace+71>
    0xf010006b <test_backtrace+43>:      movl   $0x0,0x8(%esp)
    0xf0100073 <test_backtrace+51>:      movl   $0x0,0x4(%esp)
    0xf010007b <test_backtrace+59>:      movl   $0x0,(%esp)
    0xf0100082 <test_backtrace+66>:      call   0xf01007df <mon_backtrace>
    0xf0100087 <test_backtrace+71>:      mov    %ebx,0x4(%esp)
    0xf010008b <test_backtrace+75>:      movl   $0xf010193c,(%esp)
    0xf0100092 <test_backtrace+82>:      call   0xf0100971 <cprintf>
    0xf0100097 <test_backtrace+87>:      add    $0x14,%esp
    0xf010009a <test_backtrace+90>:      pop    %ebx
    0xf010009b <test_backtrace+91>:      pop    %ebp
    0xf010009c <test_backtrace+92>:      ret
    0xf010009d <i386_init>:      push   %ebp
    0xf010009e <i386_init+1>:    mov    %esp,%ebp
    0xf01000a0 <i386_init+3>:    sub    $0x18,%esp
    0xf01000a3 <i386_init+6>:    mov    $0xf0112940,%eax
    0xf01000a8 <i386_init+11>:   sub    $0xf0112300,%eax
    0xf01000ad <i386_init+16>:   mov    %eax,0x8(%esp)

    // %ebp of different levels of test_backtrace has a difference of 32 byte, this is because the function reserves
    // enough space for nested function calls' parameters (for test_backtrace with one arg and mon_backtrace with three args),
    // also it saves %eip, %ebp and %ebx on stack
    ```

9. See source code for solutons to Exercise 11 and 12
