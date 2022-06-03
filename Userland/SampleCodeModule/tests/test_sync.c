#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test_util.h>
#include <syscall.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

#define SYNCHRO "1"
#define NO_SYNCHRO "0"

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  sys_yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

void my_process_inc(int argc, char *argv[]){
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  /* if (argc != 3) 
    return; */

  if ((n = satoi(argv[1])) <= 0){ 
    print("MALARDO 1\n");
    return;
  }
  if ((inc = satoi(argv[2])) == 0) {
    print("MALARDO 2\n");
    return;
  }
  if ((use_sem = satoi(argv[3])) < 0){
    print("MALARDO 3\n");
    return;
  }

  //printInt(sys_getPid());
  //printWithColor(": A\n", RED_BLACK);

  /* printInt(n);
  print(" ");
  printInt(inc + 1);
  print(" ");
  printInt(use_sem);
  print("\n"); */

  if (use_sem){
    if (sys_sem_open(SEM_ID, 1) == 0){
      print("test_sync: ERROR opening semaphore\n");
      return;
    }
  }
  uint64_t i;
  for (i = 0; i < n; i++){
    if (use_sem) {
      if(sys_sem_wait(SEM_ID) == 0)
        printWithColor("ERROR EN WAIT\n", RED_BLACK);
    }
    slowInc(&global, inc);
    if (use_sem) {
      if(sys_sem_post(SEM_ID) == 0)
        printWithColor("ERROR EN POST\n", RED_BLACK);   
    }
  }

  if (use_sem) 
    sys_sem_close(SEM_ID);
}

uint64_t test_sync(int mode){ //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  //char * argvDec[] = {argv[0], "-1", argv[1], NULL};
  //char * argvInc[] = {argv[0], "1", argv[1], NULL};

  char * argvDec[] = {"my_process_inc", "10", "-1", mode ? SYNCHRO : NO_SYNCHRO};
  char * argvInc[] = {"my_process_inc", "10", "1", mode ? SYNCHRO : NO_SYNCHRO};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    //pids[i] = my_create_process("my_process_inc", 3, argvDec);
    pids[i] = sys_createProcess(&my_process_inc, 4, argvDec, NULL, FOREGROUND);
    // pids[i + TOTAL_PAIR_PROCESSES] = my_create_process("my_process_inc", 3, argvInc);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_createProcess(&my_process_inc, 4, argvInc, NULL, FOREGROUND);

  }

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    sys_wait(pids[i]);
    sys_wait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  print("Final value: ");
  printInt(global);
  print("\n");

  global = 0;

  return 0;
}
