#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // You can use print statements as follows for debugging, they'll be visible
  // when running tests printf("Logs from your program will appear here!\n");

  // Uncomment this block to pass the first stage

  // Wait for user input
  char *const_path = getenv("PATH");
  char *commands[] = {"exit", "type", "echo"};
  size_t commands_len = 3;

  int yes = 0;
  while (!yes) {
    char path[strlen(const_path) + 1];
    strcpy(path, const_path);

    printf("$ ");
    fflush(stdout);
    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';

    if (strcmp(input, "exit 0") == 0) {
      return 0;
    } else if (strncmp("echo", input, strlen("echo")) == 0) {
      printf("%s\n", input + strlen("echo "));
    } else if (strncmp("type", input, strlen("type")) == 0) {
      char *arg = input + strlen("type ");
      int flag = 0;
      for (int i = 0; i < commands_len; i++) {
        if (strcmp(arg, commands[i]) == 0) {
          printf("%s is a shell builtin\n", arg);
          flag = 1;
          break;
        }
      }
      char *curr_path = strtok(path, ":");
      while (curr_path != NULL) {
        char *command = curr_path + (strlen(curr_path) - strlen(arg));
        if (strcmp(command, arg) == 0) {
          printf("%s is %s\n", arg, curr_path);
          flag = 1;
          break;
        }
        curr_path = strtok(NULL, ":");
      }

      if (!flag) {
        printf("%s: not found\n", arg);
      }

    } else {
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
