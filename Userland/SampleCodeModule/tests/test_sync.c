#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test_util.h>
#include <syscall.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  uint64_t aux = *p;
  sys_yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(int argc, char *argv[]){
  uint64_t n;
  uint8_t inc;
  int8_t use_sem;

  if (argc != 3) 
    return;

  if ((n = satoi(argv[0])) <= 0) return;
  if ((inc = satoi(argv[1])) == 0) return;
  if ((use_sem = satoi(argv[2])) < 0) return;

  if (use_sem)
    if (!sys_sem_open(SEM_ID, 1)){
      print("test_sync: ERROR opening semaphore\n");
      return;
    }

  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem) 
        sys_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem) 
        sys_sem_post(SEM_ID);
  }

  if (use_sem) 
    sys_sem_close(SEM_ID);
}

uint64_t test_sync(){ //{n, use_sem, 0}
//   uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
//   if (argc != 2) 
//     return -1;
//   char * argvDec[] = {argv[0], "-1", argv[1], NULL};
//   char * argvInc[] = {argv[0], "1", argv[1], NULL};

  // char * argvDec[] = {"my_process_inc", "-1", "1", NULL};
  // char * argvInc[] = {"my_process_inc", "1", "1", NULL};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    // pids[i] = my_create_process("my_process_inc", 3, argvDec);
    // sys_createProcess(&my_process_inc, 4, argvDec, NULL, BACKGROUND);
    // pids[i + TOTAL_PAIR_PROCESSES] = my_create_process("my_process_inc", 3, argvInc);
    // sys_createProcess(&my_process_inc, 4, argvInc, NULL, BACKGROUND);

  }

//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     sys_wait(pids[i]);
//     sys_wait(pids[i + TOTAL_PAIR_PROCESSES]);
//   }

  // print("Final value: %d\n");

  return 0;
}
