extern unsigned long long sys_getPid();
extern unsigned long long sys_createProcess(void (*pFunction)(int, char **), int argc, char **argv, char priority);
extern void sys_nice(unsigned long long pid, char newPriority);
extern int sys_killPs(unsigned long long pid);
// int64_t my_block(uint64_t pid);
// int64_t my_unblock(uint64_t pid);
extern int sys_togglePsState(unsigned long long pid);
// int64_t my_sem_open(char *sem_id, uint64_t initialValue);
// int64_t my_sem_wait(char *sem_id);
// int64_t my_sem_post(char *sem_id);
// int64_t my_sem_close(char *sem_id);
// int64_t my_wait(int64_t pid);
extern void sys_yield();
extern void * sys_malloc(uint32_t size);
extern void sys_free(void *pr);

