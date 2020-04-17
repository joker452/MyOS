# Questions

* The ticket lock is slightly different than the spinlock in in JOS and xv6. Assuming an invalidation-based coherence scheme as described in the paper does a ticket lock introduce less inter-CPU communication than a JOS/xv6 spinlock?
    Yes. Both locks cost O(n) time when releasing a lock. But JOS/xv6 spinlock may need to keep broadcasting while spinning. Ticket lock only reads while spinning, there is no cost.
