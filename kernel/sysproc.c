#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
void vmprint(pagetable_t pagetable);
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  struct proc *p;
  uint64 addr;
  int sz;
  uint64 abits_addr;
  int abits_temp;
  pte_t *pte;

  p = myproc();
  argaddr(0, &addr);
  argint(1, &sz);
  argaddr(2, &abits_addr);

  // printf(">>> pgaccess START\n");
  // printf(">>> addr = %p, sz = %d, abits_addr = %p\n", addr, sz, abits_addr);
  // addr = PGROUNDDOWN(addr);
  // vmprint(p->pagetable);
  // printf(">>>\n");

  abits_temp = 0;
  for (int i=0; i<sz; i++) {
    pte = walk(p->pagetable, addr, 0);
    // printf(">>>>>> pte = %p\n", *pte);
    if (pte && (*pte & PTE_A)) {
      abits_temp |= (1<<i);
      *pte &= ~PTE_A;
    }
    addr += PGSIZE;
  }

  // vmprint(p->pagetable);
  // printf(">>> abits = %x\n", abits_temp);
  // printf(">>> pgaccess END\n\n");
  copyout(p->pagetable, abits_addr, (char*) &abits_temp, sizeof abits_temp);
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

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
