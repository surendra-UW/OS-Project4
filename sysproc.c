#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "psched.h"

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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);

  if (myproc()->killed)
  {
    release(&tickslock);
    return -1;
  }

  struct proc *p = myproc();
  p->wait_ticks = n;
  sleep(&ticks, &tickslock);
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}



int sys_nice(void) {
  int n;

  if(argint(0, &n) < 0 ) {
    return -1;
  }

  if(n<0 || n>20) {
    cprintf("get set a nice value less than 0 or greater than 20, %d\n", n);
    return -1;
  }

  struct proc *p = myproc();
  int ret_nice = p->nice;
  p->nice = n;
  return ret_nice;
}

int sys_getschedstate(void) {
  struct pschedinfo *pschedinfo;

  if(argptr(0, (void*)&pschedinfo, sizeof(*pschedinfo)) < 0)
    return -1;

  if(pschedinfo == ((void*)0)) {
    return -1;
  }

  build_pschedinfo(pschedinfo);
  return 0;
}