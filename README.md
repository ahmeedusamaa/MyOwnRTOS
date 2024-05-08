# AO-RTOS

## Real-Time Operating System Features:

###Create Main Stack
![design_MSP](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V1/design_MSP.png)

###Context Switching:
	/*
	 * Save the Context of the Current Task?
	 * Automatically saved by the CPU when entering Handler mode.
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 * manually save the addition frame of other general purpose registers
	 * r5, r6 , r7 ,r8 ,r9, r10,r11
	 */
	 
	/*
	 * Restore the Context of the next Task
	 * will restore manually frame of general purpose registers
	 * as these will not be automatically restored by the CPU.
	 */

	/*
	 * Update PSP register
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 * Automatically restored by the CPU after ("BX LR").
	 */


###Priority: Ensures tasks execute based on their priority levels (Highest Priority Should Run First), optimizing system performance.

###Round-Robin: Support Round-Robin Scheduling when two or more tasks have the same highest priority.
![Round_robin](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V2/Round_robin.png)

###Priority inversion? : When a low-priority task holds a resource needed by a high-priority task, causing a delay in the execution of the highest-priority task.
![Priority_inversion](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V4/Priority_inversion.png)

###Solution -> Priority Inheritance: temporarily raise the priority of a low-priority task, which is currently holding a shared resource, to match the priority of the high-priority task that requires the resource.
![priority_Inheritance_Solution](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V5/priority_Inheritance_Solution.png)

###Mutex Handling: Facilitates effective task synchronization through mutex acquisition and release mechanisms.

###Deadlock? : A deadlock occurs when two or more tasks are mutually waiting for each other to release resources they hold.
![Deadlock](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V6/Deadlock.png)

###Deadlock avoidance by Stopping a task from Acquiring more than one Mutex
![Deadlock_Solution](https://github.com/ahmeedusamaa/MyOwnRTOS/blob/main/V6/Deadlock_Solution.png)






