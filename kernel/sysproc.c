#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"

uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0; // not reached
}

// const cinfo info_exit = {.name = "exit", .argc = 1};

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

// const cinfo info_getpid = {.name = "getpid", .argc = 0};

uint64
sys_fork(void)
{
  return fork();
}

// const cinfo info_fork = {.name = "fork", .argc = 0};

uint64
sys_wait(void)
{
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

// const cinfo info_wait = {.name = "wait", .argc = 1};

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

// const cinfo info_sbrk = {.name = "sbrk", .argc = 1};

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// const cinfo info_sleep = {.name = "sleep", .argc = 1};

uint64
sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// const cinfo info_kill = {.name = "kill", .argc = 1};

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// const cinfo info_uptime = {.name = "uptime", .argc = 0};

uint64
sys_trace(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;

  myproc()->trace_mask = n;
  return 0;
}

// const cinfo info_trace = {.name = "trace", .argc = 0};