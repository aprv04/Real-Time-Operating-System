
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h> 
#include "rtai.h"
#include "rtai_sched.h"
#include <rtai_sem.h>
#include <rtai_mbx.h>

MODULE_LICENSE("GPL");

#define BUFSIZE 100
#define MAX_MESSAGES 100
#define STACKSIZE 1024
#define FUNC_ARG 0
#define FPU 0
#define SIGNAL 0
static RT_TASK t1;
static RT_TASK t2;
static MBX mbx1;
SEM sem;

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
	char buff[BUFSIZE];
	memset(buff,'\0',BUFSIZE);
	rt_mbx_send_if(&mbx1,"Recieved Message from task1",strlen("Recieved Message from task1"));
	rt_sem_wait (&sem);
	rt_mbx_receive_if(&mbx1,buff,40);
	rt_printk("TASK1 : %s\n",buff);
}
void taskTwo(long arg)
{
	char buff[BUFSIZE];
	memset(buff,'\0',BUFSIZE);
	rt_mbx_send_if(&mbx1,"Recieved Message from task2",strlen("Recieved Message from task2"));
	rt_mbx_receive_if(&mbx1,buff,40);
	rt_printk("TASK2 : %s\n",buff);
	
	rt_sem_signal(&sem);
}
int init_module(void)
{
	
	rt_set_oneshot_mode();
	start_rt_timer(1);
	rt_typed_sem_init (&sem,0,BIN_SEM);
	rt_typed_mbx_init(&mbx1,MAX_MESSAGES,FIFO_Q);
	tasksinit();
	taskresume();
	return 0;
}
void cleanup_module(void)
{
	rt_mbx_delete(&mbx1);
	rt_sem_delete(&sem);
	stop_rt_timer();
	return;
}
