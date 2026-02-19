#include "../Queue_buffer/Queue_buffer.h"
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>

volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigwinch_received = 0;

void handle_sigint(int signo) {
  (void)signo;
  sigint_received = 1;
}

void handle_sigwinch(int signo) {
  (void)signo;
  sigwinch_received = 1;
}

void ignore_signal(int signo) { (void)signo; }

void setup_signal_handlers(void) {
  struct sigaction sa;

  // SIGINT
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGINT, &sa, NULL);

  // SIGWINCH
  sa.sa_handler = handle_sigwinch;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGWINCH, &sa, NULL);

  // Ignore SIGTSTP and SIGQUIT
  sa.sa_handler = ignore_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGTSTP, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
}

void reset_input_line(Queue *q) {
  queue_clear(q); // your queue clear function
}

void print_prompt(void) { write(STDOUT_FILENO, "\n$ ", 3); }

void input_loop(void) {
  char c;

  while (1) {

    // React to SIGINT
    if (sigint_received) {
      sigint_received = 0;

      reset_input_line();
      print_prompt();
      continue;
    }

    // React to terminal resize
    if (sigwinch_received) {
      sigwinch_received = 0;
      handle_resize(); // optional: recompute columns, redraw line
    }

    // Read input character
    ssize_t n = read(STDIN_FILENO, &c, 1);

    if (n <= 0)
      continue;

    handle_input_char(c); // your existing editor logic

    if (n == 0) { // read() returns 0
      write(STDOUT_FILENO, "\nexit\n", 6);
      cleanup_and_exit();
    }

    if (fork() == 0) {
      signal(SIGINT, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
      execvp(...);
    }
  }
}
