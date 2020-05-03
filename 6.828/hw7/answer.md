# Questions

* Explain in one sentence what happens
  
    ```c
    struct spinlock lk;
    initlock(&lk, "test lock");
    acquire(&lk);
    acquire(&lk);
    ```

    Spinlock in xv6 isn't reentrant. After current process acquires the lock, it will block forever when it tries to acquire the lock for the second time.
* Explain why the kernel panics if interrupt is enabled in iderw in ide.c.  
    After acquiring the lock in `iderw`, if interrupt is enabled, and an interrupt from ide arrives, in `ideintr` a second attempt to acquire the lock will be executed, which fails to pass the holding check in `acquire` and incurs a panic.
* Explain in a few sentences why the kernel didn't panic. Why do file_table_lock and ide_lock have different behavior in this respect?  
    This is because that operations in `filealloc` are rather cheap and fast. And it's less likely that a timer interrupt will occur, which cause the ncli check in `sched` fails. Compared to `filealloc`, operations in `iderw` are slow, and thus be interrupted by a timer event.
* Why does release() clear lk->pcs[0] and lk->cpu before clearing lk->locked? Why not wait until after?  
    When a thread releases the lock, the values of fields, cpu and pcs are obsolete, and they shouldn't be seen by any other threads. To protect this invariant, the lock cannot be released until the thread clears their values.
