# Questions

* The paper's Figure 6 shows how Linux uses uses and replaces socket options. Why it safe for read-only access to sock->opts to proceed without holding a lock? Why are read-only operations that are operating on the "old" version of an array not in danger of accessing freed memory?
    Because pointer assignment is atomic in nature. Any reader who calls `rcu_read_lock()` before `rcu_assign_pointer(&sock->opts, new)` will use the old version, and readers come later will use the new version. Since `call_rcu(kfree, old)` is guaranteed not to free the old version until these readers finish, readers are not in danger of accessing freed memory.
