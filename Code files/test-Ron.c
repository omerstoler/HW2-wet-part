#include <sys/types.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
//#include <sched.h>
#include "hw2_syscalls.h"

#include <sys/resource.h>


#define assertTest(expression)\
    do {\
        if (!(expression)) {\
            printf("Assertion failed at %s:%d: "\
                   "in function \"%s\" "\
                   "with assertion \"%s\".\n",\
                   __FILE__, __LINE__, __func__, (#expression));\
        }\
    } while (0)

#define wait_for_all_sons() while(wait(NULL) > 0)

#define forkAndTest(test_function) \
	do {\
		if (fork() == 0) {\
			printf("Starting test: %s\n", #test_function);\
			test_function();\
			printf("Finished test: %s\n", #test_function);\
			printf("--------------------------------\n");\
			exit(0);\
		} else {\
			wait_for_all_sons();\
		}\
	} while(0);

#define STRESS_LEVEL 100

typedef struct sched_param sched_param_t; //======

sched_param_t param1 = {0, 80, 50};
sched_param_t param2 = {5, 80, 50};
sched_param_t param3 = {90000, 80, 50};
sched_param_t param4 = {-2, 80, 50};
sched_param_t param5 = {0, 1, 50};
sched_param_t param6 = {0, 3000, 50};
sched_param_t param7 = {5, 5, 1};
sched_param_t param8 = {5, 5, 139};
sched_param_t param14 = {5, 3000, 139};
sched_param_t param15 = {5, 150, 115};
sched_param_t param16 = {5, 3000, 50};
sched_param_t param17 = {5, 3000, 0};
sched_param_t param18 = {5, 3000, 2};
sched_param_t param19 = {5, 3000, 1};
sched_param_t param20 = {5, 3000, 3};

// these are illegal
sched_param_t param9 = {0, 0, 50};
sched_param_t param10 = {0, -1, 50};
sched_param_t param11 = {0, 3001, 50};
sched_param_t param12 = {0, 5, -1};
sched_param_t param13 = {0, 5, 140};

void stress_test1() {
	int i;
		for (i = 0; i < STRESS_LEVEL; ++i) {
			if (fork() == 0) {
				assertTest(is_short(getpid()) == 0);
				assertTest(short_remaining_time(getpid()) == -1);
				assertTest(short_place_in_queue(getpid()) == -1);
				assertTest(errno = EINVAL);
				assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);
				assertTest(is_short(getpid()) == 1);

				assertTest(short_place_in_queue(getpid()) == 0);

				exit(0);
			} else {
				assertTest(is_short(getpid()) == 0);
			}
		}

		wait_for_all_sons();
}

void stress_test2() {
	int i;
	for (i = 0; i < STRESS_LEVEL; ++i) {
		if (fork() == 0) {
			assertTest(is_short(getpid()) == 0);
			assertTest(short_remaining_time(getpid()) == -1);
			assertTest(short_place_in_queue(getpid()) == -1);
			assertTest(errno = EINVAL);
			assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);
			assertTest(is_short(getpid()) == 1);

			assertTest(short_place_in_queue(getpid()) == 0);

			exit(0);
		} else {
			assertTest(is_short(getpid()) == 0);
		}
	}

	wait_for_all_sons();
}

//   printf("errno = %d\n",errno);

void test1() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test2() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test3() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param3) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test4() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param4) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test5() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param5) == 0);
	sched_setscheduler(getpid(), SCHED_SHORT, &param1); //assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1); ======= think test wrong
	//assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test6() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param6) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test7() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param7) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test8() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param8) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

void test9() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param9) == -1);
	assertTest(errno == EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param9) == -1);
	assertTest(errno == EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param10) == -1);
	assertTest(errno == EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param11) == -1);
	assertTest(errno == EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param12) == -1);
	assertTest(errno == EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param13) == -1);
	assertTest(errno == EINVAL);

	assertTest(sched_setscheduler(-1, SCHED_SHORT, &param1) == -1);
	assertTest(errno = EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT+1, &param1) == -1);
	assertTest(errno = EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, NULL) == -1);

	int pid = fork();
	if (pid) {
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == 0);
		wait_for_all_sons();
	} else {exit(0);}
	assertTest(errno = EINVAL);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);

}

void test10() {
	sched_param_t get_result;

	int pid = fork();
	if (pid) {
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == 0);
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == -1);
		assertTest(errno == EPERM);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(get_result.sched_priority == param1.sched_priority);
		assertTest(get_result.requested_time == param1.requested_time);
		assertTest(get_result.sched_short_prio == param1.sched_short_prio);
		wait_for_all_sons();
	} else { exit(0); }
}

void test11() {
	sched_param_t get_result;
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);
	assertTest(sched_getparam(getpid(), &get_result) == 0);
	assertTest(sched_getparam(getpid(), &get_result) == 0);
	assertTest(sched_getparam(getpid(), &get_result) == 0);
	assertTest(get_result.sched_priority == param1.sched_priority);
	assertTest(get_result.requested_time == param1.requested_time);
	assertTest(get_result.sched_short_prio == param1.sched_short_prio);
}


void test12() {
	sched_param_t get_result;

	int pid = fork();
	if (pid) {
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == 0);
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == -1);
		assertTest(errno == EPERM);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(get_result.sched_priority == param1.sched_priority);
		assertTest(get_result.requested_time == param1.requested_time);
		assertTest(get_result.sched_short_prio == param1.sched_short_prio);
		wait_for_all_sons();
	} else { exit(0); }

	pid = fork();
	if (pid) {
		assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == 0);
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == 0);
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == -1);
		assertTest(errno == EPERM);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(sched_getparam(pid, &get_result) == 0);
		assertTest(get_result.sched_priority == param1.sched_priority);
		assertTest(get_result.requested_time == param1.requested_time);
		assertTest(get_result.sched_short_prio == param1.sched_short_prio);
		wait_for_all_sons();
	} else { exit(0); }

	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_getparam(getpid(), &get_result) == 0);
	assertTest(sched_getparam(getpid(), &get_result) == 0);
	assertTest(sched_getparam(getpid(), &get_result) == 0);
	assertTest(get_result.sched_priority == param2.sched_priority);
	assertTest(get_result.requested_time == param2.requested_time);
	assertTest(get_result.sched_short_prio == param2.sched_short_prio);
}

void test13() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);
	assertTest(sched_setparam(getpid(), &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setparam(getpid(), &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setparam(getpid(), &param2) == -1);
	assertTest(errno == EPERM);
}

void test14() {
	int pid = fork();
	if (pid) {
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == 0);
		assertTest(sched_setparam(pid, &param1) == -1);
		assertTest(errno == EPERM);
		assertTest(sched_setparam(pid, &param1) == -1);
		assertTest(errno == EPERM);
		assertTest(sched_setparam(pid, &param2) == -1);
		assertTest(errno == EPERM);
		wait_for_all_sons();
	} else { exit(0); }
}

void test15() {
	assertTest(sched_getscheduler(getpid()) == SCHED_OTHER);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);
	assertTest(sched_getscheduler(getpid()) == SCHED_SHORT);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_getscheduler(getpid()) == SCHED_SHORT);
}

void test16() {
	int pid = fork();
	if (pid) {
		assertTest(sched_getscheduler(getpid()) == SCHED_OTHER);
		assertTest(sched_getscheduler(pid) == SCHED_OTHER);

		assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == 0);
		assertTest(sched_getscheduler(getpid()) == SCHED_SHORT);
		assertTest(sched_getscheduler(pid) == SCHED_OTHER);
		wait_for_all_sons();
	} else { exit(0); }
}

void test17() {
	int pid = fork();
	if (pid) {
		assertTest(sched_getscheduler(getpid()) == SCHED_OTHER);
		assertTest(sched_getscheduler(pid) == SCHED_OTHER);

		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == 0);
		assertTest(sched_getscheduler(getpid()) == SCHED_OTHER);
		assertTest(sched_getscheduler(pid) == SCHED_SHORT);
		wait_for_all_sons();
	} else { exit(0); }
}

static int childFunc(void *arg) {
	return 0;
}

void test18() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param14) == 0);
	char* stack = (char*)malloc(5);
	assertTest(fork() == -1);
	assertTest(errno == EPERM);
	assertTest(clone(childFunc, stack, 5, NULL) == -1);
	assertTest(errno == EPERM);
	assertTest(fork() == -1);
	assertTest(errno == EPERM);
	assertTest(clone(childFunc, stack, 5, NULL) == -1);
	assertTest(errno == EPERM);
	free(stack);
}

void test19() {
	int pid = fork();
	if (pid) {
		sched_setscheduler(pid, SCHED_SHORT, &param14);
		assertTest(setpriority(0, pid, 10) == -1);
		assertTest(errno == EPERM);
		assertTest(setpriority(0, pid, 10) == -1);
		assertTest(errno == EPERM);
		assertTest(nice(5) != -1);
		wait_for_all_sons();
	} else {
		sched_setscheduler(getpid(), SCHED_SHORT, &param14);
		assertTest(setpriority(0, getppid(), 10) == 0);
		assertTest(setpriority(0, getpid(), 10) == -1);
		assertTest(errno == EPERM);
		assertTest(nice(5) == -1);
		assertTest(errno == EPERM);
		exit(0);
	}
}

void test20() {
	assertTest(is_short(getpid()) == 0);
	assertTest(is_short(-5) == -1);
	assertTest(errno == ESRCH);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param14) == 0);
	assertTest(is_short(getpid()) == 1);
}

void test21() {
	int pid = getpid();
	assertTest(setpriority(0, pid, 10) == 0);
	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == 0);
	assertTest(getpriority(0, pid) == 10);
	while(is_short(pid) == 1);
	assertTest(getpriority(0, pid) == 17);

	assertTest(setpriority(0, pid, 10) == 0);
	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == 0);
	assertTest(getpriority(0, pid) == 10);
	while(is_short(pid) == 1);
	assertTest(getpriority(0, pid) == 17);

	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == 0);
	assertTest(getpriority(0, pid) == 17);
	while(is_short(pid) == 1);
	assertTest(getpriority(0, pid) == 19);
}

void test22() {
	assertTest(short_remaining_time(getpid()) == -1);
	assertTest(errno == EINVAL);
	assertTest(short_remaining_time(-5) == -1);
	assertTest(errno == ESRCH);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param15) == 0);
	int t = short_remaining_time(getpid());
	assertTest(t >= 0 && t <= param15.requested_time);
}

void test23() {
	int pid = fork();
	if (pid) {
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == 0);
		assertTest(short_remaining_time(pid) != -1);

		wait_for_all_sons();
		assertTest(short_remaining_time(pid) == -1);
		assertTest(errno == ESRCH);
	} else { exit(0); }
}

void test24() {
	int pid = getpid();

	assertTest(is_short(pid) == 0);
	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == 0);
	while (is_short(pid) == 1) {
		int t = short_remaining_time(pid);
		if (t == -1) {
			assertTest(errno == EINVAL);
			assertTest(is_short(pid) == 0);
		} else {
			assertTest(t >= 0 && t <= param15.requested_time);
		}
	}
	assertTest(short_remaining_time(pid) == -1);
	assertTest(errno == EINVAL);

	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == 0);
	while(short_remaining_time(pid) >= 0) {
		int s = is_short(pid);
		if (s == 0) {
			assertTest(short_remaining_time(pid) == -1);
			assertTest(errno == EINVAL);
		} else {
			assertTest(s == 1);
		}
	}
	assertTest(errno == EINVAL);
	assertTest(is_short(pid) == 0);
}

void test25() {
	assertTest(short_place_in_queue(getpid()) == -1);
	assertTest(errno == EINVAL);
	assertTest(short_place_in_queue(-5) == -1);
	assertTest(errno == ESRCH);

	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param15) == 0);
	assertTest(short_place_in_queue(getpid()) == 0);

	while (is_short(getpid()) == 1) {
		assertTest(short_place_in_queue(getpid() == 0));
	}
	assertTest(short_place_in_queue(getpid()) == -1);
	assertTest(errno == EINVAL);
	assertTest(short_place_in_queue(-5) == -1);
	assertTest(errno == ESRCH);
}

/** void test26() {
	int pid = fork();
	if (pid) {
		assertTest(short_place_in_queue(pid) == -1);
		assertTest(errno == EINVAL);
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == 0);
		printf("place: %d\n", short_place_in_queue(pid));
		printf("errno: %d\n", errno);
		assertTest(short_place_in_queue(pid) == 1);
		assertTest(short_place_in_queue(getpid()) == -1);
		assertTest(errno == EINVAL);
		wait_for_all_sons();
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == -1);
		assertTest(errno == ESRCH);
	} else { while(1); }
}
*/
void test26() {
	int pid = fork();
	if (pid) {
		assertTest(short_place_in_queue(pid) == -1);
		assertTest(errno == EINVAL);
		assertTest(short_place_in_queue(getpid()) == -1);
		assertTest(errno == EINVAL);

		assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param15) == 0);
		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param16) == 0);
		assertTest(is_short(pid) == 1);
		wait_for_all_sons();
		assertTest(short_place_in_queue(getpid()) == 0);
	} else {
		int ppid = getppid();
		assertTest(is_short(ppid) == 1);
		assertTest(short_place_in_queue(ppid) == 1);
		assertTest(is_short(ppid) == 1);
		exit(0);
	}
}

void test27() {
	int pid = fork();
	if (pid) {
		assertTest(short_place_in_queue(pid) == -1);
		assertTest(errno == EINVAL);
		assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param15) == 0);
		assertTest(short_place_in_queue(getpid()) == 0);
		assertTest(short_place_in_queue(pid) == -1);
		assertTest(errno == EINVAL);

		wait_for_all_sons();

		assertTest(sched_setscheduler(pid, SCHED_SHORT, &param15) == -1);
		assertTest(errno == ESRCH);
	} else { exit(0); }
}

void test28() {
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param17) == 0);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param2) == -1);
	assertTest(errno == EPERM);
}

/**
sched_param_t param18 = {5, 3000, 2};
sched_param_t param19 = {5, 3000, 1};
sched_param_t param20 = {5, 3000, 3}; */

void test29() {
	int pid1, pid2, pid3;
	pid1 = fork();
	if (pid1) {
		pid2 = fork();
		if (pid2) {

			pid3 = fork();
			if (pid3) {
				assertTest(sched_setscheduler(getpid(), SCHED_SHORT, &param17) == 0);
				assertTest(sched_setscheduler(pid1, SCHED_SHORT, &param18) == 0);
				assertTest(sched_setscheduler(pid2, SCHED_SHORT, &param19) == 0);
				assertTest(sched_setscheduler(pid3, SCHED_SHORT, &param20) == 0);

				assertTest(short_place_in_queue(pid1) == 2);
				assertTest(short_place_in_queue(pid2) == 1);
				assertTest(short_place_in_queue(pid3) == 3);
				assertTest(short_place_in_queue(getpid()) == 0);

			}else {
				exit(0);
			}
		} else {
			exit(0);
		}


		wait_for_all_sons();
		assertTest(short_place_in_queue(pid1) == -1);
		assertTest(errno == ESRCH);
		assertTest(short_place_in_queue(pid2) == -1);
		assertTest(errno == ESRCH);
		assertTest(short_place_in_queue(pid3) == -1);
		assertTest(errno == ESRCH);
		assertTest(short_place_in_queue(getpid()) == 0);
	} else {

		exit(0);
	}
}
int main() {
	printf("Starting tests!\n");
	printf("--------------------------------\n");
	forkAndTest(test1);
	forkAndTest(test2);
	forkAndTest(test3);
	forkAndTest(test4);
	forkAndTest(test5);
	forkAndTest(test6);
	forkAndTest(test7);
	forkAndTest(test8);
	forkAndTest(test9);
	forkAndTest(test10);
	forkAndTest(test11);
	forkAndTest(test12);
	forkAndTest(test13);
	forkAndTest(test14);
	forkAndTest(test15);
	forkAndTest(test16);
	forkAndTest(test17);
	forkAndTest(test18);
	forkAndTest(test19);
	forkAndTest(test20);
	forkAndTest(test21);
	forkAndTest(test22);
	forkAndTest(test23);
	forkAndTest(test24);
	forkAndTest(test25);
	forkAndTest(test26);
	forkAndTest(test27);
	forkAndTest(test28);
	forkAndTest(test29);

	printf("Finished testing: don't forget to check for kernel oops with dmesg.\n");
	return 0;
}
