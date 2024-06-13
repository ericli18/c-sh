#include <stdio.h>
#include <string.h>

int main() {
  // You can use print statements as follows for debugging, they'll be visible
  // when running tests printf("Logs from your program will appear here!\n");

  // Uncomment this block to pass the first stage

  // Wait for user input

  int yes = 0;
  while (!yes) {
    printf("$ ");
    fflush(stdout);
    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    if (strcmp(input, "exit 0") == 0) {
      return 0;
    }
    if (strncmp("echo", input, sizeof("echo") - 1) == 0) {
      printf("%s\n", input + 5);
    } else {
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
