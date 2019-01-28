#include "rtai_sched.h"
#include "rtai.h"
#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");
static RT_TASK task;
void print(long arg)
{
	printk("%lu\n",arg);
	printk("Hello world!\n");
	return;
}
int init_module(void)
{
	int result;
	rt_set_oneshot_mode();
	start_rt_timer(1);
	
	result=rt_task_init(&task,print,10,512,3,0,NULL);
	if(result!=0)
	{
		printk("Task Init Failed\n");
		return result;
	}
	
	result=rt_task_resume(&task);
	if(result!=0)
	{
		printk("task not resume");
		return result;
	}
 return 0;
}
void cleanup_module(void)
{
	stop_rt_timer();
	return;
}
