use std::io::{self, Write};
use std::process::{Command, Stdio};

const BUILTIN_COMMANDS: &[&str] = &["cd", "exit", "help", "ls"];

// Please RE-VISIT
fn shell_loop() {
    let mut status = 1;
    while status != 0 {
        print!("> ");
        io::stdout().flush().unwrap();

        let line = shell_readline();
        let arg = shell_splitline(&line);
    }
}

fn shell_readline() -> String {
    let mut buffer = String::new();
    match io::stdin().read_line(&mut buffer) {
        Ok(_) => {
            if buffer.ends_with('\n') {
                buffer.pop();
                if buffer.ends_with('\r') {
                    buffer.pop();
                }
            }
            buffer
        }
        Err(e) => {
            eprintln!("Error: {}", e);
            std::process::exit(1);
        }
    }
}

fn shell_splitline(line: &str) -> Vec<String> {
    let mut tokens = Vec::new();
    let clean_line = match line.find('#') {
        Some(pos) => &line[..pos],
        None => line,
    };

    for tok in clean_line.split_whitespace() {
        tokens.push(tok.to_string());
    }

    tokens
}

fn shell_launch(value: &[String]) -> i32 {
    let mut has_pipe = false;
    let mut pipe_index = 0;

    for (index, val) in value.iter().enumerate() {
        if val == "|" {
            has_pipe = true;
            pipe_index = index;
            break;
        }
    }

    if !has_pipe {
        let mut child = Command::new(&value[0])
            .args(&value[1..])
            .spawn()
            .expect("Failed to execute process");

        child.wait().expect("Failed to wait");
    } else {
        let first_cmd = &value[..pipe_index];
        let second_cmd = &value[pipe_index + 1..];

        // first command (writes to pipe)
        let mut first = Command::new(&first_cmd[0])
            .args(&first_cmd[1..])
            .stdout(Stdio::piped())
            .spawn()
            .expect("Failed to execute first command");

        // second command (reads from the pipe)
        let mut second = Command::new(&second_cmd[0])
            .args(&second_cmd[1..])
            .stdin(first.stdout.take().unwrap())
            .spawn()
            .expect("Failed to execute the second command");

        second.wait().expect("failed to wait for second command");
        first.wait().expect("failed to wait for first command");
    }
    1
}

fn num_shell_builtins() -> usize {
    BUILTIN_COMMANDS.len()
}

fn main() {
    shell_loop();
}
