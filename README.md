# xv-DOS

[![forthebadge](https://forthebadge.com/images/badges/no-ragrets.svg)](https://forthebadge.com)

**xv**6 - **D**ependent **O**perating **S**ystem

An extension of the MIT's xv6 Operating System in RISC-V. \
Read the original readme [here](files/README). The original code can be found in the riscv branch.

## Running the OS

```sh
$ make qemu SCHEDULER=[RR/PBS/FCFS] CPUS=[N_CPU]
```

default scheduler is RR (Round-robbin).

# Modifications

Here are the modifications made to the original xv6:

## Modification 1: Syscall Tracing

We aim to intercept and record the system calls which are called by a process during its execution. We define a user program called `strace` which uses the system call `trace`.

### User Program

Running:
```sh
$ strace [mask] [command] [args]
```

Implementation:
```c
void strace(int mask, char *command, char *args[]);
```

First, we fork the current process. The parent process waits for the child process. In the child process, before we execute the command using `exec`, we run the `trace` syscall with mask as a parameter.

### System Call

```c
int trace(int mask);
```

There is a 'trace_mask' defined for the proc struct, which is by default set to 0. Every time a process is forked, the child inherits its parent's trace_mask.

When this syscall is called, it takes the input argument which it sets to the `proc->trace_mask`.

For every set bit, the syscall() function in syscall.c prints the information for the syscall corresponding to that bit.

## Modification 2: Scheduling

### First Come - First Served

We compare the creation time of each process (which is stored in proc->ctime and initialized to 0 when the process is allocated in the table).

Then we schedule the process with the minimum creation time which is currently in the table.

Since this is non-preempted, a condition is added to ignore the yield() when FCFS is defined.

### Priority Based Scheduler

Instead of time, we compare the priorities. Static priority (default to 60) can be changed (explained below) by the user. Dynamic priority is calculated and compared:

```
dynamic_pr = max(0, min(100, static_pr - niceness + 5))
```

where niceness is defined as
```
10*(ntime)/(ntime+rtime)
```

here ntime (nap time) and rtime (run time) are the ticks spent in sleeping state since the last call/running state in total, stored in proc->ntime and proc->rtime.

#### Set Priority

User program:
```sh
$ setpriority [priority] [pid]
```

which calls the `set_priority` system call which sets the static priority to the given value and resets ntime to 0 and niceness to 5.

```c
int set_priority(int new_priority, int proc_id);
```

## Modification 3: procdump

Procdump is triggered when `Ctrl+P` is pressed. It lists out information about all the processes.

For FCFS, procdump acts in the same manner as in the case of RR (default case).

### PBS

```sh
pid priority state rtime wtime nrun
```

pid and state are already defined.

- priority is just the dynamic priority as defined in PBS.
- rtime is the run time as defined in niceness.
- wtime (wait time) is calculated as
```
wtime = ltime - rtime - stime
```
here ltime is the total lifetime of the process and stime is the total sleep time since creation (different from ntime).
- nrun (number of times the process was picked by the scheduler) is stored in proc->nrun initialized to 0 at the creation of the process. Every time the status of a process is changed from runnable to running, it is incremented.


![xv-DOS](files/card.png)

----

*This was built as a part of the Operating Systems and Networks course, Monsoon 2021. The problem statement is given [here](files/Assignment.pdf).*