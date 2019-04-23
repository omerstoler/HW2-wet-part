#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/slab.h>

//#include <sys/types.h>


int sys_is_short(pid_t pid)
{
  return 0;
}

int sys_short_remaining_time ( pid_t pid)
{
	return 0;
}

int sys_short_place_in_queue(pid_t pid)
{
	return 0;
}
