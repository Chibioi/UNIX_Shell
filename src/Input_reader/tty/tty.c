#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios original_terminal_settings;
void reset_terminal_mode(void);
void set_non_canonical_mode(void);

void reset_terminal_mode(void) {
    // Restore the original terminal attributes on exit
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_settings);
}

void set_non_canonical_mode(void) {
    struct termios new_terminal_settings;
    tcgetattr(STDIN_FILENO, &original_terminal_settings);
    atexit(reset_terminal_mode);
    new_terminal_settings = original_terminal_settings;
    new_terminal_settings.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode (ICANON) and local echo (ECHO)
    // Set VMIN and VTIME to control read() behavior
    new_terminal_settings.c_cc[VMIN] = 1;  // Read returns as soon as at least 1 character is available
    new_terminal_settings.c_cc[VTIME] = 0; // No timeout (or set a value in 0.1s for a timeout)

    // Apply the new terminal attributes immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_settings);

}
