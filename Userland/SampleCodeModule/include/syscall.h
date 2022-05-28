extern unsigned long long sys_getPid();
extern unsigned long long sys_createProcess(void (*pFunction)(int, char **), int argc, char **argv, char priority);
extern void sys_nice(unsigned long long pid, char newPriority);
extern int sys_killPs(unsigned long long pid);
extern unsigned long sys_block(uint64_t pid);
extern unsigned long sys_unblock(uint64_t pid);
unsigned long sys_sem_open(char *sem_name, uint64_t initialValue);
unsigned long sys_sem_wait(char *sem_name);
unsigned long sys_sem_post(char *sem_name);
unsigned long sys_sem_close(char *sem_name);
// unsigned long sys_wait(int64_t pid);
extern void sys_yield();
extern void * sys_malloc(uint32_t size);
extern void sys_free(void *pr);

