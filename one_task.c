#include "rtai_sched.h"
#include "rtai.h"
#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");
static RT_TASK task;
void print(long arg)
{
	while(1)
	{
		printk("%lu\n",arg);
		printk("Hello task periodic!\n");
		rt_task_wait_period();
	}
	return;
}
int init_module(void)
{
	int result;
	RTIME i;
	i= nano2count(5e9);
	rt_set_oneshot_mode();
	start_rt_timer(1);
	
	result=rt_task_init(&task,print,10,512,3,0,NULL);
	if(result!=0)
	{
		printk("Task Init Failed\n");
		return result;
	}
	result=rt_task_make_periodic(&task,rt_get_time(),i);
	if(result!=0)
	{
		printk("task not resume");
		return result;
	}
 return 0;
}
void cleanup_module(void)
{
	rt_task_delete(&task);
	stop_rt_timer();
	return;
}
