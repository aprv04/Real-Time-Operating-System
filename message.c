
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h> 
#include "rtai.h"
#include "rtai_sched.h"
#include <rtai_sem.h>
#include <rtai_msg.h>

MODULE_LICENSE("GPL");

#define BUFSIZE 100
#define STACKSIZE 1024
#define FUNC_ARG 0
#define FPU 0
#define SIGNAL 0
static RT_TASK t1;
static RT_TASK t2;

void taskOne(long arg);
void taskTwo(long arg);

void tasksinit(void)
{
	
	rt_task_init(&t1,taskOne,FUNC_ARG,STACKSIZE,0,FPU,SIGNAL);
	rt_task_init(&t2,taskTwo,FUNC_ARG, STACKSIZE,0,FPU,SIGNAL);
}
void taskresume(void)
{
	rt_task_resume(&t1);
	rt_task_resume(&t2);
}
void taskOne(long arg)
{
	unsigned int msg = 112;
	unsigned int res_msg;
	rt_send (&t2,msg);
	rt_receive (&t2,&res_msg);
	rt_printk("TASK1 : %d\n",res_msg);
}
void taskTwo(long arg)
{
	unsigned int msg = 211;
	unsigned int res_msg;
	rt_receive (&t1,&res_msg);
	rt_printk("TASK2 : %d\n",res_msg);
	rt_send (&t1,msg);
}
int init_module(void)
{
	
	rt_set_oneshot_mode();
	start_rt_timer(1);
	tasksinit();
	taskresume();
	return 0;
}
void cleanup_module(void)
{
	stop_rt_timer();
	return;
}
