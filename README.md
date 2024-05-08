# <h1 style="font-size: 24px;">AO-RTOS</h1>

## <h2 style="font-size: 20px;">Real-Time Operating System Features</h2>

### <h3 style="font-size: 18px;">Create Main Stack</h3>
![Main Stack Design](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V1/design_MSP.png)

### <h3 style="font-size: 18px;">Context Switching</h3>

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

### <h3 style="font-size: 18px;">Priority</h3>
Ensures tasks execute based on their priority levels, optimizing system performance.

### <h3 style="font-size: 18px;">Round-Robin</h3>
Supports Round-Robin Scheduling when two or more tasks have the same highest priority.
![Round-Robin Scheduling](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V2/Round_robin.png)

### <h3 style="font-size: 18px;">Priority Inversion</h3>
When a low-priority task holds a resource needed by a high-priority task, causing a delay in the execution of the highest-priority task.
![Priority Inversion](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V4/Priority_inversion.png)

### <h3 style="font-size: 18px;">Solution: Priority Inheritance</h3>
Temporarily raise the priority of a low-priority task holding a shared resource to match the priority of the high-priority task.
![Priority Inheritance Solution](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V5/priority_Inheritance_Solution.png)

### <h3 style="font-size: 18px;">Mutex Handling</h3>
Facilitates effective task synchronization through mutex acquisition and release mechanisms.

### <h3 style="font-size: 18px;">Deadlock</h3>
Occurs when two or more tasks are mutually waiting for each other to release resources they hold.
![Deadlock](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V6/Deadlock.png)

### <h3 style="font-size: 18px;">Deadlock Avoidance</h3>
Stopping a task from acquiring more than one mutex.
![Deadlock Avoidance Solution](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V6/Deadlock_Solution.png)
