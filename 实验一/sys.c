//mysetnice系统调用，功能是修改nice和prio值的系统调用功能。
SYSCALL_DEFINE5(mysetnice,pid_t,pid,int,flag,int,nicevalue,void __user*,prio,void __user*,nice){
 	struct pid * kpid;
	struct task_struct * task;
	kpid = find_get_pid(pid);/* 返回pid */
	task = pid_task(kpid, PIDTYPE_PID);/* 返回task_struct */
	int n;
	n = task_nice(task);/* 返回进程当前nice值 */
	int p;
	p = task_prio(task);/*返回进程当前prio值*/
	if(flag == 1)
	{
		printk("Change nice: %d ,prio: %d ",n,p);
		set_user_nice(task, nicevalue);/* 修改进程nice值 */
		n = task_nice(task);/*重新取得进程nice值*/
		p = task_prio(task);/*重新获取进程prio值 这里和参考资料不一样!!! */
		printk("to nice: %d ,prio: %d\n",n,p);
		copy_to_user(nice,&n,sizeof(n));/*将nice值拷贝到用户空间*/
		copy_to_user(prio,&p,sizeof(p));/*将prio值拷贝到用户空间*/
		return 0;
	}
	else if(flag == 0)
 	{
		printk("nice : %d ,prio: %d\n",n,p);
		copy_to_user(nice,&n,sizeof(n));/*将nice值拷贝到用户空间*/
		copy_to_user(prio,&p,sizeof(p));/*将prio值拷贝到用户空间*/
		return 0;
 	}
	return EFAULT;
}

//myshowname系统调用，功能是显示当前系统名称和版本的系统调用。
SYSCALL_DEFINE3(myshowname,void __user*,sysname,void __user*,release,void __user*,version)
{
	struct old_utsname tmp;

	down_read(&uts_sem);
	memcpy(&tmp, utsname(), sizeof(tmp));
	up_read(&uts_sem);

	copy_to_user(sysname,&tmp.sysname,sizeof(tmp.sysname));/*将sysname值拷贝到用户空间*/
	copy_to_user(release,&tmp.release,sizeof(tmp.release));/*将release值拷贝到用户空间*/
	copy_to_user(version,&tmp.version,sizeof(tmp.version));/*将version值拷贝到用户空间*/

        printk("sysname = %s  \n",tmp.sysname);
        printk("release = %s  \n",tmp.release);
        printk("version = %s  \n",tmp.version);

        return 0;
}

//mychangename系统调用，功能是改变主机名称为自定义字符串的系统调用。
SYSCALL_DEFINE2(mychangename,char __user*,name,int,len)
{
    int errno;
    char tmp[__NEW_UTS_LEN];

    if (len < 0 || len > __NEW_UTS_LEN)
	return -EINVAL;
    errno = -EFAULT;
    if (!copy_from_user(tmp, name, len)) {
	struct new_utsname *u;
	down_write(&uts_sem);
	u = utsname();
	memcpy(u->nodename, tmp, len);
	memset(u->nodename + len, 0, sizeof(u->nodename) - len);
	errno = 0;
	uts_proc_notify(UTS_PROC_HOSTNAME);
	up_write(&uts_sem);
	printk("change nodename to %s\n",tmp);
    }
    return errno;
}
