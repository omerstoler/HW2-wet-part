#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/slab.h>

//#include <sys/types.h>


int sys_is_short(pid_t pid)
{
  task_t* p = find_task_by_pid(pid);

  if (pid < 0 || p == NULL)
	{
		return -1*ESRCH;
	}

  if (p->policy == SCHED_SHORT)
    return 1;
  else
    return 0;
}

int sys_short_remaining_time ( pid_t pid)
{
  task_t* p = find_task_by_pid(pid);

  if (pid < 0 || p == NULL)
  {
    return -1*ESRCH;
  }

  if (p->policy != SCHED_SHORT)
  {
    return -1*EINVAL;
  }

  return (p->short_time_slice * 1000/HZ);
}

int sys_short_place_in_queue(pid_t pid)
{
  task_t* p = find_task_by_pid(pid);
  prio_array_t* array;
  int count = 0;
  list_t *pos, *head;

  if (pid < 0 || p == NULL)
  {
    return -1*ESRCH;
  }

  if (p->policy != SCHED_SHORT)
  {
    return -1*EINVAL;
  }

  array = p->array;
  for (k = 0; k < MAX_PRIO; k++) {

    head = array->queue + k;
    if (array->bitmap[k] == 0)
      continue;

    list_for_each(pos, head){
      if(list_entry(pos, task_t, pid) == pid)
        break;
      count++;
    }
  }
  return count;
}
