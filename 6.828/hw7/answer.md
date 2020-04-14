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
  