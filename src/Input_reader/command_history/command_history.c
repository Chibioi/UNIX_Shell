#include <string.h>
// Using a fixed size circular buffer
#define MAX_HISTORY 500
#define MAX_CMD_LEN 1024

typedef struct {
  char commands[MAX_HISTORY][MAX_CMD_LEN];
  int start; // index of oldest command
  int count; // number of commands stored
  int nav_index;
} CommandHistory;

void init_history(CommandHistory *history) {
  history->start = 0;
  history->count = 0;
  history->nav_index = 0;
}

void add_command(CommandHistory *history, const char *command) {
  int index;

  if (history->count < MAX_HISTORY) {
    index = (history->start + history->count) % MAX_HISTORY;
    history->count++;
  } else {
    // Buffer full → overwrite oldest
    index = history->start;
    history->start = (history->start + 1) % MAX_HISTORY;
  }

  strncpy(history->commands[index], command, MAX_CMD_LEN - 1);
  history->commands[index][MAX_CMD_LEN - 1] = '\0';
  history->nav_index = history->count;
}

const char *history_prev(CommandHistory *history) {
  if (history->count == 0)
    return NULL;

  if (history->nav_index > 0)
    history->nav_index--;

  int index = (history->start + history->nav_index) % MAX_HISTORY;
  return history->commands[index];
}

const char *history_next(CommandHistory *history) {
  if (history->count == 0)
    return NULL;

  if (history->nav_index < history->count)
    history->nav_index++;

  if (history->nav_index == history->count)
    return ""; // empty input line

  int index = (history->start + history->nav_index) % MAX_HISTORY;
  return history->commands[index];
}

void history_search_reset(CommandHistory *history) {
  history->nav_index = history->count;
}

// REVERSE SEARCH
const char *history_search_prev(CommandHistory *history, const char *query) {
  if (history->count == 0 || !query || *query == '\0')
    return NULL;

  for (int i = history->nav_index - 1; i >= 0; i--) {
    int index = (history->start + i) % MAX_HISTORY;

    if (strstr(history->commands[index], query)) {
      history->nav_index = i;
      return history->commands[index];
    }
  }

  return NULL; // no match
}

// FORWARD SEARCH
const char *history_search_next(CommandHistory *history, const char *query) {
  if (history->count == 0 || !query || *query == '\0')
    return NULL;

  for (int i = history->nav_index + 1; i < history->count; i++) {
    int index = (history->start + i) % MAX_HISTORY;

    if (strstr(history->commands[index], query)) {
      history->nav_index = i;
      return history->commands[index];
    }
  }

  return NULL;
}
