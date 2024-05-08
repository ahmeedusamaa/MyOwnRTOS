# AO-RTOS

## Real-Time Operating System Features

### Create Main Stack
![Main Stack Design](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V1/design_MSP.png)

### Context Switching

> **Save the Context of the Current Task?**
> - Automatically saved by the CPU when entering Handler mode.
> - XPSR
> - PC (Next Task Instruction which should be Run)
> - LR (return register which is saved in CPU while Task1 running before Task Switching)
> - r12
> - r4
> - r3
> - r2
> - r1
> - r0
> - Manually save the additional frame of other general-purpose registers (r5, r6, r7, r8, r9, r10, r11)
>
> **Restore the Context of the next Task**
> - Will restore manually frame of general-purpose registers as these will not be automatically restored by the CPU.
>
> **Update PSP register**
> - XPSR
> - PC (Next Task Instruction which should be Run)
> - LR (return register which is saved in CPU while Task1 running before Task Switching)
> - r12
> - r4
> - r3
> - r2
> - r1
> - r0
> - Automatically restored by the CPU after "BX LR".

### Priority
> - Ensures tasks execute based on their priority levels, optimizing system performance.

### Round-Robin
> - Supports Round-Robin Scheduling when two or more tasks have the same highest priority.
![Round-Robin Scheduling](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V2/Round_robin.png)

### Priority Inversion
> - When a low-priority task holds a resource needed by a high-priority task, causing a delay in the execution of the highest-priority task.
![Priority Inversion](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V4/Priority_inversion.png)

### Solution: Priority Inheritance
> - Temporarily raise the priority of a low-priority task holding a shared resource to match the priority of the high-priority task.
![Priority Inheritance Solution](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V5/priority_Inheritance_Solution.png)

### Mutex Handling
> - Facilitates effective task synchronization through mutex acquisition and release mechanisms.

### Deadlock
> - Occurs when two or more tasks are mutually waiting for each other to release resources they hold.
![Deadlock](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V6/Deadlock.png)

### Deadlock Avoidance
> - Stopping a task from acquiring more than one mutex.
![Deadlock Avoidance Solution](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V6/Deadlock_Solution.png)






