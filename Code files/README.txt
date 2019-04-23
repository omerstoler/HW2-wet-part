
Given:

		~/hw2_syscalls.h - GIVEN

Implement:

	Kernel space:

		/kernel/hw2_syscalls.c -> implement syscalls 243-245.

	User space:

		~/hw2_syscalls.c

Update:

		/arch/i386/kernel/entry.S  -> add syscalls 243-245

		/include/linux/sched.h ->

			1) define SCHED_SHORT.
			2) add sched_param's fields.
			3) add task_struct's fields.

		/kernel/fork.c ->

			1) initiate values of new task_struct members.
			2) prevent SCHED_SHORTs from calling it.

		/kernel/sys.c -> maybe to change a little the some first functions corresponds scheduling, later is irrelevant.
						 Understand when to use who = pis and who = uid - in sys_setpriority

		/kernel/sched.c -> all the logic and algorithms. need to get the understanding what to implement and where.

			1) check how RR, FIFO suspends OTHER and implement the same mechanism where: RR, FIFO suspends others, and SHORT suspend OTHER.
			2) short_prio updating with compatibility to the rest of the system. - *Make sure that short < others at first for debug*
			3) implementing a run_queue for SHORTs / check if it's possible to iterate over the
				in original OTHER's, and if it has the wanted behavior.

			maybe build a SCHED_SHORT prio_array_t and connect to it when in find_first_bit() in schedule() it search for next process to run
			- (if first bit is in an index bigger than 99, SCHED_SHORT will be preferred)




Maybe:

		/kernel/exit.c -> maybe not needed (when SCHED_SHORT calls exit(),
						  remaining time slice shall not added to the father) -
						  alternative place = sched_exit() in sched.c.

		learn to use prio_array_t and other kernel data structs
