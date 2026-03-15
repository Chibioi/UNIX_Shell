use std::io::{self, Write};

static BUILTIN_COMMANDS: &[&str] = &["cd", "exit", "help", "ls"];

fn main() {
    shell_loop();
}

// Please RE-VISIT
fn shell_loop() {
    let mut status = 1;
    while status != 0 {
        print!("> ");
        io::stdout().flush().unwrap();

        let line = shell_readline();
        let arg = shell_splitline();
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
