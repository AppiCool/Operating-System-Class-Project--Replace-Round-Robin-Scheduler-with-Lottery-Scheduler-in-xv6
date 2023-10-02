#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
#include "pstat.h" /* This is added by Rohith Jaina (rxj220013)
                    ** to include pstat header file */
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
/* The following code is added by Apeksha Padaliya, axp210162
 ** This is the routine which sets the number of tickets of the calling process */
int
sys_settickets(void)
{
  int no_of_tickets;
  argint(0,&no_of_tickets);
  if(no_of_tickets <= 0)
    return -1;
  proc->tickets = no_of_tickets;
    return 0;
}
/* The following code is added by Rohith Jaina (rxj220013)
 ** This is the routine for gathering information or statistics about all the running processess */
int
sys_getpinfo(void)
{
  struct pstat* procstat;
  argptr(0,(void*)&procstat,sizeof(procstat));
  if(procstat == NULL)
    return -1;
  struct proc* process;
  for(process = ptable.proc; process < &ptable.proc[NPROC]; process++)
{
    const int index = process - ptable.proc;
    if(process->state != UNUSED)
    {
      procstat->inuse[index] = 1;
      procstat->pid[index] = process->pid;
      procstat->tickets[index] = process->tickets;
      procstat->ticks[index] = process->ticks;
    }
  }
  return 0;
}
