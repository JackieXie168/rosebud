#include <sys/types.h>
#include <unistd.h>

void POS33_C() {
  pid_t pid = fork(); // correct
  if (pid == 0) /* child */ {
    execve("filename", NULL, NULL);
  }
}
