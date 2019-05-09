/*Test that you can't change SHORT parameters after they are set.
 * Test they really are not changed (not only that setparam returns -1).*/
void test41() {
	struct sched_param get_result;
	pid_t pid = getpid();
	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param6) == 0);

	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param8) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_getparam(pid, &get_result) == 0);
	assertTest(get_result.requested_time == param6.requested_time);
	assertTest(get_result.sched_short_prio == param6.sched_short_prio);

	assertTest(sched_setparam(pid, &param8) == -1);
	assertTest(errno == EPERM);
	assertTest(sched_getparam(pid, &get_result) == 0);
	assertTest(get_result.requested_time == param6.requested_time);
	assertTest(get_result.sched_short_prio == param6.sched_short_prio);
}

/* Test that timeslice is retained while sleeping */
void test42() {
	pid_t father = getpid(), son;
	int t, i;
	if (son = fork()) {  //father code
		assertTest(sched_setscheduler(father, SCHED_SHORT, &param1) == 0);
		wait_for_all_sons();
		assertTest(short_remaining_time(father) > 0);
	} else {  //son code
		while(!is_short(father));
		t = short_remaining_time(father);
		assertTest(t > 0);
		for (i=0; i<5; ++i)
			assertTest(short_remaining_time(father) == t);
		exit(0);
	}
}

/* short_remaining_time fails if the process could not be set to SHORT */
void test43() {
	pid_t pid = getpid();
	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param9) == -1);
	assertTest(errno == EINVAL);
	assertTest(short_remaining_time(pid) == -1);
	assertTest(errno == EINVAL);
	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param11) == -1);
	assertTest(errno == EINVAL);
	assertTest(short_remaining_time(pid) == -1);
	assertTest(errno == EINVAL);
	assertTest(sched_setscheduler(pid,SCHED_FIFO,&param1_guy) == 0);
	assertTest(sched_setscheduler(pid, SCHED_SHORT, &param1) == -1);
	assertTest(errno == EPERM);
	assertTest(short_remaining_time(pid) == -1);
	assertTest(errno == EINVAL);
}

/*Test short_place_in_queue for many processes together*/
void test44() {
	pid_t me, p40_1, p40_2, p100, p139_1, p139_2;
	/* proc_a is the main process. b1 & b2 are sons of proc_b. proc_b and all
	 * other processes are sons of proc_a. */
	struct sched_param
			rt = {1, 0, 0},
			other = {0, 0, 0},
			pr40 = {0, 3000, 40},
			pr100 = {0, 3000, 100},
			pr139 = {0, 3000, 139};

	me = getpid();
	assertTest(sched_setscheduler(me, SCHED_FIFO, &rt) == 0);  //set myself to realtime
	if (! (p40_1 = fork())) exit(0);
	if (! (p100 = fork())) exit(0);
	if (! (p139_2 = fork())) exit(0);
	if (! (p40_2 = fork())) exit(0);
	if (! (p139_1 = fork())) exit(0);
	assertTest(sched_setscheduler(p139_1, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(p139_2, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(p40_1, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(p100, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(p40_2, SCHED_OTHER, &other) == 0);

	assertTest(sched_setscheduler(p139_1, SCHED_SHORT, &pr139) == 0);
	assertTest(sched_setscheduler(p139_2, SCHED_SHORT, &pr139) == 0);
	assertTest(sched_setscheduler(p40_1, SCHED_SHORT, &pr40) == 0);
	assertTest(sched_setscheduler(p100, SCHED_SHORT, &pr100) == 0);
	assertTest(sched_setscheduler(p40_2, SCHED_SHORT, &pr40) == 0);

	assertTest(short_place_in_queue(p139_1) == 3);
	assertTest(short_place_in_queue(p40_2) == 1);
	assertTest(short_place_in_queue(p139_2) == 4);
	assertTest(short_place_in_queue(p40_1) == 0);
	assertTest(short_place_in_queue(p100) == 2);

	assertTest(short_place_in_queue(p139_1) == 3);
	assertTest(short_place_in_queue(p40_2) == 1);
	assertTest(short_place_in_queue(p139_2) == 4);
	assertTest(short_place_in_queue(p40_1) == 0);
	assertTest(short_place_in_queue(p100) == 2);

	wait(NULL);
	assertTest(short_place_in_queue(p40_2) == 0);
	assertTest(short_place_in_queue(p100) == 1);
	assertTest(short_place_in_queue(p139_1) == 2);
	assertTest(short_place_in_queue(p139_2) == 3);
	wait(NULL);
	assertTest(short_place_in_queue(p100) == 0);
	assertTest(short_place_in_queue(p139_1) == 1);
	assertTest(short_place_in_queue(p139_2) == 2);
	wait(NULL);
	assertTest(short_place_in_queue(p139_1) == 0);
	assertTest(short_place_in_queue(p139_2) == 1);
	wait(NULL);
	assertTest(short_place_in_queue(p139_2) == 0);
	wait_for_all_sons();
}

/*Assert that realtime, SHORT and OTHER processes run in the correct order*/
void test45() {
	pid_t rt1, rt2, rt3, short10_1, short10_2, short105_1, short105_2, other1;
	struct sched_param
			rta = {93, 0, 0},
			rtb = {92, 0, 0},
			rtc = {91, 0, 0},
			short10 = {0, 100, 10},
			short105 = {0, 100, 105},
			other = {0, 0, 0};

	rt1 = getpid();
	assertTest(sched_setscheduler(rt1, SCHED_FIFO, &rta) == 0);
	printf("Should print 1...8 in order:\n");

	if (! (short10_1 = fork())) {printf("4\n"); exit(0);}
	assertTest(sched_setscheduler(short10_1, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(short10_1, SCHED_SHORT, &short10) == 0);

	if (! (rt3 = fork())) {printf("3\n"); exit(0);}
	assertTest(sched_setscheduler(rt3, SCHED_FIFO, &rtc) == 0);

	if (! (short105_1 = fork())) {printf("6\n"); exit(0);}
	assertTest(sched_setscheduler(short105_1, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(short105_1, SCHED_SHORT, &short105) == 0);

	if (! (other1 = fork())) {printf("8\n"); exit(0);}
	assertTest(sched_setscheduler(other1, SCHED_OTHER, &other) == 0);

	if (! (short10_2 = fork())) {printf("5\n"); exit(0);}
	assertTest(sched_setscheduler(short10_2, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(short10_2, SCHED_SHORT, &short10) == 0);

	if (! (short105_2 = fork())) {printf("7\n"); exit(0);}
	assertTest(sched_setscheduler(short105_2, SCHED_OTHER, &other) == 0);
	assertTest(sched_setscheduler(short105_2, SCHED_SHORT, &short105) == 0);

	if (! (rt2 = fork())) {printf("2\n"); exit(0);}
	assertTest(sched_setscheduler(rt2, SCHED_FIFO, &rtb) == 0);

	printf("1\n");
	wait_for_all_sons();
}
