
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h> 
#include <linux/errno.h>
#include "rtai.h"
#include "rtai_sched.h"
#include <rtai_sem.h> 
MODULE_LICENSE("GPL");
#define ITER 10
static RT_TASK t1;
static RT_TASK t2;
void taskOne(long arg);
void taskTwo(long arg);
int global = 0;
void tasks(void)
{
	int retval;

	retval = rt_task_init(&t1,taskOne, 0, 1024, 0, 0, 0);
	retval = rt_task_init(&t2,taskTwo, 0, 1024, 0, 0, 0);
	retval = rt_task_resume(&t1);
	retval = rt_task_resume(&t2);
}
void taskOne(long arg)
{
	int i;
	for (i=0; i < ITER; i++){	
		rt_printk("I am taskOne and global = %d................\n", ++global);
	}
}
void taskTwo(long arg)
{
	int i;
	for (i=0; i < ITER; i++){
	rt_printk("I am taskTwo and global = %d----------------\n", --global);
	}
}
int init_module(void)
{
	printk("Start of init_module\n");
	rt_set_oneshot_mode();
	start_rt_timer(1);
	tasks();
	return 0;
}
void cleanup_module(void)
{
	stop_rt_timer();
	return;
}