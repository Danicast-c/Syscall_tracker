#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

// Const Declaration
#define MAX_SYSCALL_NUM 334

// Var Declaration
int argc_number = 0;
int mode_flag = 0;
int total_syscalled = 0;
int program_index = 2;
char* program;
char* program_args[20];
struct syscall_struct {
  int id;
  char* name;
  int times_called;
};
struct syscall_struct syscall_array[MAX_SYSCALL_NUM];

// Func Declaration
void start_syscall_array();
void sys_call_counter(int syscall_id);
void parse_arguments(char* argv[]);
void print_table();
void print_syscall(int id);

int main(int argc, char* argv[]) {
  argc_number = argc;

  start_syscall_array();
  // manage_arguments(argv);
  parse_arguments(argv);

  int status;
  long sys_called = 0;
  long sys_call_id;
  struct user_regs_struct regs;

  // Create child proces
  pid_t pid_child = fork();
  if (pid_child < 0) {
    perror("PID ERROR");
    abort();
  } else if (pid_child == 0) {
    // Child Process
    printf("The program to execute is '%s'", program);
    if (program_index + 1 < argc_number) {
      printf(" with args ");
      for (int i = 1; i + program_index < argc_number; i++) {
        printf("'%s' ", argv[i + program_index]);
      }
    }
    printf("\n");
    printf(" ____________PROCESS START____________ \n");
    if (!mode_flag) printf("|=ID==========NAME======CALL INSTANCE=|\n");
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    ptrace(PTRACE_SYSCALL, pid_child, NULL, NULL);

    execvp(program, program_args);
  } else {
    // Main Process
    while (1) {
      // Check change on program status
      waitpid(pid_child, &status, 0);

      // Check finished process
      if (WIFEXITED(status) || WIFSIGNALED(status)) {
        break;
      }

      if (sys_called == 0) {
        sys_called = 1;
        // Get the info from the registers
        ptrace(PTRACE_GETREGS, pid_child, NULL, &regs);

        sys_call_id = regs.orig_rax;

        if (sys_call_id == -1){
          printf("There's been a fatal error. The program will end now.\n");
          break;
        }

        sys_call_counter(sys_call_id);

        //#mode_flag = 0 INTERACTIVO
        if (mode_flag == 0) {
          // print_sys_call_info(sys_call_id);
          print_syscall(sys_call_id);
        }

      } else {
        sys_called = 0;
      }
      ptrace(PTRACE_SYSCALL, pid_child, NULL,
             NULL);  // Restart the the process with pid child_pid
    }
    // End of process
    printf(" _____________PROCESS END_____________ \n");
    if (mode_flag == 1) {
      print_table();
    }
  }
  printf("\n");
  return 0;
}

void start_syscall_array() {
  FILE* fd = fopen("syscall.tbl", "r");
  size_t len;
  for (int i = 0; i < MAX_SYSCALL_NUM; i++) {
    struct syscall_struct syscall_element;
    syscall_array[i].id = i;
    getline(&syscall_array[i].name, &len, fd);
    size_t last_idx = strlen(syscall_array[i].name) - 1;
    syscall_array[i].name[last_idx] = '\0';
    syscall_array[i].times_called = 0;
  }
}

void sys_call_counter(int syscall_id) {
  total_syscalled++;
  syscall_array[syscall_id].times_called++;
}

void parse_arguments(char* argv[]) {
  // Flag -i #mode_flag = 0
  if (!strcmp(argv[1], "-i")) {
    printf("Modo Interactivo\n");
    mode_flag = 0;
    program = argv[2];
  }
  // Flag -a #mode_flag = 1
  else if (!strcmp(argv[1], "-a")) {
    printf("Modo Automatico\n");
    mode_flag = 1;
    program = argv[2];

  } else {
    printf("Modo no encontrado\n Utilizando modo interactivo por defecto\n");
    mode_flag = 0;
    program = argv[1];
    program_index = 1;
  }
  for (int i = 0; i + program_index < argc_number; i++)
    program_args[i] = argv[program_index + i];
}

void print_table() {
  printf("\n ____________SYSCALL TABLE____________ \n");
  printf("|=====================================|\n");
  printf("|=ID==========NAME==============CALLS=|\n");
  for (int i = 0; i < MAX_SYSCALL_NUM; i++) {
    if (syscall_array[i].times_called != 0) {
      print_syscall(i);
    }
  }
  printf("|_____________________________________|\n");
  printf("|  Total of System Calls called: %-5d|\n", total_syscalled);
  printf("|=====================================|\n");
}

void print_syscall(int id) {
  printf("| %-12d", syscall_array[id].id);
  printf("%-20s", syscall_array[id].name);
  printf("%-4d|\n", syscall_array[id].times_called);
}