#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define TOK_BUFFSIZE 128
#define TOK_DELIM " \t\r\n\a"

void shell_loop(void);
char *shell_readline(void);
char **shell_splitline(char *line);
int shell_launch(char **args);

int main(int argc, char **argv) {
  // Load config files, if any.

  // Run command loop.
  shell_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

void shell_loop(void) {
  char *line;
  char **arg;
  int status;

  do {
    printf("> ");
    line = shell_readline();
    arg = shell_splitline(line);
    status = shell_launch(arg);
  } while (status);
}

char *shell_readline(void) {
  int buffer_size = BUFF_SIZE;
  int pos = 0;
  char *buffer = malloc(sizeof(char) * buffer_size);
  int c;

  if (!buffer) {
    fprintf(stderr, "shell: buffer allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read the character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[pos] = '\0';
      return buffer;
    } else {
      buffer[pos] = c;
    }
    pos++;

    // If we have exceeded the buffer, reallocate.
    if (pos >= buffer_size) {
      buffer_size *= 2;
      buffer = realloc(buffer, buffer_size);
      if (buffer == NULL) {
        fprintf(stderr, "shell: buffer allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **shell_splitline(char *line) {
  int buffer_size = TOK_BUFFSIZE;
  int pos = 0;
  char **tokens = malloc(buffer_size * sizeof(char *));
  char *tok;
  char *saveptr;

  if (tokens == NULL) {
    fprintf(stderr, "shell: Tokens allocation error\n");
    exit(EXIT_FAILURE);
  }

  char *comments = strchr(line, '#');
  if (comments) {
    *comments = '\0';
  }

  tok = strtok_r(line, TOK_DELIM, &saveptr);
  while (tok != NULL) {

    tokens[pos] = tok; // CHECK BACK LATER
    pos++;

    if (pos >= buffer_size) {
      buffer_size *= 2;
      int size = buffer_size * sizeof(char *);
      tokens = realloc(tokens, size);
      if (tokens == NULL) {
        fprintf(stderr, "shell: error reallocating to buffer\n");
        exit(EXIT_FAILURE);
      }
    }

    tok = strtok_r(NULL, TOK_DELIM, &saveptr);
  }

  tokens[pos] = NULL;
  return tokens;
}

int shell_launch(char **args) {
  pid_t process_id;
  int status;
  int pipefd[2];
  int has_pipe = 0;

  for (int i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i], "|") == 0) {
      has_pipe = 1;
      args[i] = NULL;
      break;
    }
  }

  if (!has_pipe) {
    process_id = fork();
    if (process_id == 0) {
      if (execvp(args[0], args) == -1) {
        perror("shell");
      }
      _exit(EXIT_FAILURE);
    } else if (process_id < 0) {
      perror("fork");
    } else {
      if (waitpid(process_id, &status, 0) == -1) {
        perror("waitpid");
      }
    }
  } else {
    char **args2;
    int i;
    for (i = 0; args[i] != NULL; i++)
      ;

    args2 = args + i + 1;
    pipe(pipefd);
    pid_t first_process_id = fork();

    if (first_process_id == 0) {
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[0]);
      execvp(args[0], args);
      perror("shell");
      _exit(EXIT_FAILURE);
    }

    pid_t second_process_id = fork();
    if (second_process_id == 0) {
      dup2(pipefd[0], STDOUT_FILENO);
      close(pipefd[1]);
      execvp(args2[0], args2);
      perror("shell");
      _exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);
  }

  return 1;
}
