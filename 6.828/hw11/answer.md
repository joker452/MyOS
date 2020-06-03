# Questions

* Which of the file creation's modifications were written to the disk before the crash, and which were not?
  All changes made in the transaction is written to the disk except that the clear of log head after commit. But the part corresponds to `log.lh.block[0]` is written to the wrong disk block as we change it to 0. After recovery, `cat` invokes syscall `open`, which in turn calls `ilock` and finds that the file's type is 0. This will cause a panic.
* Why was the file empty, even though you created it with echo hi > a?  
    The command contains redirect, so it invokes the `open` syscall to create the file before actually runs the command `echo hi`. As crash happens in `open`, the file is empty after recovery.  
