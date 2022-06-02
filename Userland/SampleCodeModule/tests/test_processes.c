#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test_util.h>
#include <syscall.h>

enum State {RUNNING, BLOCKED, KILLED};
#define MAX_PROCESSES 10

typedef struct P_rq{
  int32_t pid;
  enum State state;
}p_rq;

int64_t test_processes(){
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes = MAX_PROCESSES;
  char * argvAux[] = {"endless_loop"};

  // if (argc != 1) return -1;
  // if ((max_processes = satoi(argv[0])) <= 0) return -1;

  p_rq p_rqs[max_processes];
  int j = 0;
  while (1){
    print("=== Vuelta ");
    printInt(j++);
    print(" ===\n");

    //sys_mem();

    // Create max_processes processes
    for(rq = 0; rq < max_processes; rq++){
      // p_rqs[rq].pid = my_create_process("endless_loop", 0, argvAux);
      p_rqs[rq].pid = sys_createProcess(&endless_loop, 1, argvAux, 10);
      if (p_rqs[rq].pid == -1){
        print("test_processes: ERROR creating process\n");
        return -1;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      for(rq = 0; rq < max_processes; rq++){
        action = GetUniform(100) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
              if (sys_killPs(p_rqs[rq].pid) == 0){  
                print("test_processes: ERROR killing process\n");
                return -1;
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;
          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(sys_block(p_rqs[rq].pid) == 0){
                print("test_processes: ERROR blocking process\n");
                return -1;
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }

      // Randomly unblocks processes
      for(rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2){
          if(sys_unblock(p_rqs[rq].pid) == 0){
            print("test_processes: ERROR unblocking process\n");
            return -1;
          }
          p_rqs[rq].state = RUNNING; 
        }
    } 
  }
}
