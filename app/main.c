#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARGSIZE 100

int is_executable(const char *path) {
  struct stat st;
  return (stat(path, &st) == 0 && st.st_mode & S_IXUSR);
}

void find_executable(const char *const_path, const char *arg, char *file_path) {
  char path[strlen(const_path) + 1];
  strcpy(path, const_path);
  char *dir = strtok(path, ":");
  while (dir != NULL) {
    DIR *dp = opendir(dir);
    if (dp != NULL) {
      snprintf(file_path, PATH_MAX, "%s/%s", dir, arg);
      if (is_executable(file_path)) {
        closedir(dp);
        return;
      }
      closedir(dp);
    }
    dir = strtok(NULL, ":");
  }
  file_path[0] = '\0';
}

void run_executable(const char *path, char **exe_args) {
  pid_t pid = fork();
  if (pid == 0) {
    if (execv(path, exe_args) == -1) {
      perror("exec: ");
    }
  }
  int status;
  waitpid(pid, &status, 0);
}

int main() {
  char *const_path = getenv("PATH");

  char *commands[] = {"exit", "type", "echo", "pwd", "cd"};
  size_t commands_len = 5;

  int yes = 0;
  while (!yes) {

    printf("$ ");
    fflush(stdout);
    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    char *line = input;
    char *rest = input;

    char *command = strtok_r(line, " ", &rest);
    char executable[PATH_MAX];
    find_executable(const_path, command, executable);

    if (strcmp(command, "exit") == 0) {
      exit(0);
    } else if (strncmp("echo", command, strlen("echo")) == 0) {
      printf("%s\n", input + strlen("echo "));
    } else if (strncmp("type", command, strlen("type")) == 0) {
      char *arg;

      while ((arg = strtok_r(NULL, " ", &rest))) {
        int flag = 0;

        for (int i = 0; i < commands_len; i++) {
          if (strcmp(arg, commands[i]) == 0) {
            printf("%s is a shell builtin\n", arg);
            flag = 1;
            break;
          }
        }

        if (!flag) {
          char file_path[PATH_MAX];
          find_executable(const_path, arg, file_path);
          if (file_path[0] != '\0') {
            flag = 1;
            printf("%s is %s\n", arg, file_path);
          }
        }

        if (!flag) {
          printf("%s: not found\n", arg);
        }
      }
    } else if (strcmp(command, "pwd") == 0) {
      char dir[PATH_MAX];
      getcwd(dir, PATH_MAX);
      printf("%s\n", dir);
    } else if (strcmp(command, "cd") == 0) {
      char *const_home = getenv("HOME");
      char home[strlen(const_home) + 1];
      strcpy(home, const_home);

      char *dir = strtok_r(NULL, " ", &rest);
      int status;
      if (dir == NULL) {
        status = chdir(home);
      }

      else if (dir[0] == '~') {
        strcat(home, dir + 1);

        status = chdir(home);
      }

      else {
        status = chdir(dir);
      }

      if (status == -1) {
        printf("cd: %s: No such file or directory\n", dir);
      }
    } else if (executable[0] != '\0') {
      char *exe_args[ARGSIZE];
      exe_args[0] = executable;
      char *arg;

      int i = 1;
      while ((arg = strtok_r(NULL, " ", &rest)) && i < ARGSIZE - 1) {
        exe_args[i] = arg;
      }
      exe_args[i + 1] = NULL;

      run_executable(executable, exe_args);
    } else {
      printf("%s: command not found\n", input);
    }
  }
  return 0;
}
