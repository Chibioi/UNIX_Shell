# 🐚 Chibioi's Shell

A custom UNIX shell implemented in both **C** and **Rust**, supporting built-in commands, external program execution, and single-pipe piping.

---

## Features

- Interactive REPL loop with a `>` prompt
- Built-in commands: `cd`, `ls`, `help`, `exit`
- External command execution via `fork`/`execvp` (C) and `std::process::Command` (Rust)
- Single pipe (`|`) support between two commands
- Inline comment stripping — anything after `#` on a line is ignored
- Dynamic input buffer that grows as needed (C implementation)

---

## Project Structure

```
.
├── C
│   ├── Makefile
│   ├── my_shell
│   └── shell.c
├── docs
│   └── README.md
└── Rust
    ├── Cargo.lock
    ├── Cargo.toml
    ├── src
    │   └── main.rs
    └── target
```

---

## Getting Started

### Prerequisites

- **C build:** GCC or Clang
- **Rust build:** Rust toolchain (`rustc` + `cargo`) — install via [rustup](https://rustup.rs)

### Building

```bash
# Build the C version
make

# Or build the Rust version
cargo build --release
```

### Running

```bash
# C version
./shell

# Rust version
./target/release/<project-name>
```

---

## Built-in Commands

| Command | Description |
|---------|-------------|
| `cd <path>` | Change the current working directory |
| `ls [path]` | List directory contents (defaults to `.`) |
| `help` | Display help and list all built-in commands |
| `exit` | Exit the shell |

---

## Usage Examples

```bash
# Navigate directories
> cd /home/user/projects

# List files
> ls
> ls /etc

# Pipe output between commands
> ls | grep main

# Run any external program
> echo "Hello, World!"
> cat shell.c

# Inline comments are ignored
> echo hello  # this part is ignored
hello
```

---

## Implementation Notes

### C (`shell.c`)

- Uses `fork()` + `execvp()` for process creation and execution
- Manual memory management with `malloc`/`realloc`/`free`
- Pipe support via `pipe()`, `dup2()`, and two `fork()` calls
- Input buffer doubles in size when capacity is exceeded
- `strtok_r` used for thread-safe line tokenization

### Rust (`main.rs`)

- Uses `std::process::Command` for spawning child processes
- Pipe support via `Stdio::piped()` and `.stdout.take()`
- Leverages Rust's ownership model — no manual memory management needed
- `env::set_current_dir` for `cd`, `fs::read_dir` for `ls`
- Pattern matching replaces the C-style `strcmp` loop for built-in dispatch

---

## Known Limitations

- Only a single `|` pipe is supported per command (no chaining)
- No support for I/O redirection (`>`, `<`, `>>`)
- No background process execution (`&`)
- No command history or tab completion

---
