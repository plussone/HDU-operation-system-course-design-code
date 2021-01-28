//按列对齐输出系统中所有内核线程的程序名、PID、进程状态、进程优先级、父进程的PID。
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

static int myallkt_init(void)
{
	struct task_struct *p;
	p = NULL;
	printk(KERN_ALERT"myallkt begin\n%*s程序名%*s进程号%*s进程状态%*s进程优先级%*s父进程进程号\n",14," ",0," ",0," ",0," ",0," ");
	for_each_process(p)
	{
		if(p->mm == NULL)
		{
			printk(KERN_ALERT"%20s %6d %8ld %10d %12d\n",p->comm,p->pid,p->state,p->prio,p->parent->pid);
		}
	}
	printk(KERN_ALERT"\n");
	return 0;
}

static void myallkt_exit(void)
{
	printk(KERN_ALERT"myallkt end\n");
	return ;
}

module_init(myallkt_init);
module_exit(myallkt_exit);
MODULE_LICENSE("GPL");
