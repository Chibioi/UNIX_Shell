use std::env;
use std::fs;
use std::io::{self, Write};
use std::path::Path;
use std::process::{Command, Stdio};

const BUILTIN_COMMANDS: &[&str] = &["cd", "exit", "help", "ls"];

// Please RE-VISIT
fn shell_loop() {
    let status = 1;
    while status != 0 {
        print!("> ");
        io::stdout().flush().unwrap();

        let line = shell_readline();
        let args = shell_splitline(&line);
        let proc = shell_execute(&args);

        if proc == 0 {
            break;
        }
    }
    println!("Goodbye!!!");
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

fn shell_cd(value: &[String]) -> i32 {
    match value.get(1) {
        None => {
            eprintln!("Expected argument to \"cd\"");
        }
        Some(path_str) => {
            let new_path = Path::new(path_str);
            if let Err(e) = env::set_current_dir(new_path) {
                // Equivalent to perror("shell")
                eprintln!("shell: {}", e);
            }
        }
    }
    1
}

fn shell_help() -> i32 {
    let builtin_commands = BUILTIN_COMMANDS;
    println!("Chibioi's Shell");
    println!("Type program names and arguments, and hit enter.");
    println!("The following are built in:");

    for var in builtin_commands {
        println!("{}", var);
    }

    println!("Use the man command for information on other programs.");
    1
}

fn shell_exit() -> i32 {
    0
}

fn shell_ls(args: &[String]) -> i32 {
    // 1. Get the path from args[1], default to "." if not provided
    // args[0] is usually the command name itself
    let path_str = args.get(1).map(|s| s.as_str()).unwrap_or(".");
    let path = Path::new(path_str);

    // 2. Attempt to read the directory
    match fs::read_dir(path) {
        Ok(entries) => {
            // 3. Iterate through each entry
            for entry in entries {
                if let Ok(entry) = entry {
                    // entry.file_name() returns an OsString, so we convert to string for printing
                    println!("{}", entry.file_name().to_string_lossy());
                }
            }
            1 // Return success code
        }
        Err(e) => {
            // 4. Handle errors (similar to perror)
            eprintln!("ls: {}", e);
            1 // Return 1 even on error to keep the shell running
        }
    }
}

fn shell_execute(args: &[String]) -> i32 {
    // 1. Check if an empty command was entered
    let command = match args.get(0) {
        Some(cmd) => cmd.as_str(),
        None => {
            println!("An Empty command was entered");
            return 1;
        }
    };

    // 2. Check for built-in commands using a match statement
    // This replaces the C loop and strcmp logic
    match command {
        "cd" => shell_cd(args),
        "help" => shell_help(),
        "exit" => shell_exit(),
        "ls" => shell_ls(args),

        // 3. If it's not a built-in, try to launch it as an external process
        _ => shell_launch(args),
    }
}

fn main() {
    shell_loop();
}
