# Welcome to WyShell

Welcome to WyShell
[Jump to Table of Contents](#table-of-contents)

## Getting Started

To run WyShell, you have two convenient options:

### Option 1: Manual Compilation and Run

1. **Clone the Repository:**
   ```sh
   git clone https://github.com/sambuaneesh/wyShell
   cd wyshell
   ```

2. **Compile the Shell:**
   Compile the WyShell binary using the following command:
   ```sh
   make main
   ```

3. **Run WyShell:**
   Run WyShell by executing the compiled binary:
   ```sh
   ./wysh
   ```

### Option 2: One-Step Compilation and Run

You can compile and run WyShell in a single command:

```sh
make run
```

## Cleaning Up

To clean up the compiled files and binaries, you can use the following command:

```sh
make clean
```

This command will remove the WyShell executable, the `pastevents.txt` file, and any other compiled binaries, ensuring a tidy project structure.

## Table of Contents

- [Welcome to WyShell](#welcome-to-wyshell)
  - [Getting Started](#getting-started)
    - [Option 1: Manual Compilation and Run](#option-1-manual-compilation-and-run)
    - [Option 2: One-Step Compilation and Run](#option-2-one-step-compilation-and-run)
  - [Cleaning Up](#cleaning-up)
  - [Table of Contents](#table-of-contents)
  - [Basic System Calls](#basic-system-calls)
    - [Display Requirement](#display-requirement)
    - [Input Requirements](#input-requirements)
  - [Shell Commands](#shell-commands)
    - [`warp [path]`](#warp-path)
    - [`peek [flags] [path/name]`](#peek-flags-pathname)
    - [`pastevents`](#pastevents)
    - [**System Commands**](#system-commands)
      - [Foreground Process](#foreground-process)
      - [Background Process](#background-process)
    - [`proclore [pid]`](#proclore-pid)
    - [`seek [flags] [search] [target_directory]`](#seek-flags-search-target_directory)
  - [Processes, Files, and Miscellaneous](#processes-files-and-miscellaneous)
    - [I/O Redirection](#io-redirection)
    - [Pipes](#pipes)
    - [Redirection along with Pipes](#redirection-along-with-pipes)
    - [Process Management](#process-management)
    - [Neonate Command](#neonate-command)
  - [Networking](#networking)
    - [`iMan [command_name]`](#iman-command_name)

## Basic System Calls

### Display Requirement

The shell prompt is displayed in the following format:

```sh
<Username@SystemName:~>
```

- `<Username>`: Current username
- `@`: Separator
- `SystemName`: System's name
- `~`: Represents the current home directory

**Example:**
```sh
<JohnDoe@SYS:~>
```

### Input Requirements

- The shell supports `;` or `&` separated lists of commands.
- Commands can be separated by random spaces and tabs.
- `;` is used for sequential commands.
- `&` runs the preceding command in the background.

**Example:**
```sh
<JohnDoe@SYS:~> command1 ; command2 & command3
```

## Shell Commands

### `warp [path]`

The `warp` command changes the current directory to the specified path and updates the prompt accordingly. It supports symbols like `.` (current directory), `..` (parent directory), `~` (home directory), and `-` (previous directory).

**Example:**
```sh
<JohnDoe@SYS:~> warp test
/home/johndoe/test
<JohnDoe@SYS:~/test> warp ~
/home/johndoe
<JohnDoe@SYS:~>
```

### `peek [flags] [path/name]`

The `peek` command lists files and directories in the specified directory. It supports `-a` (show hidden files) and `-l` (display extra information) flags.

**Example:**
```sh
<JohnDoe@SYS:~> peek test
# Output
<JohnDoe@SYS:~> peek -al test
# Output with extra information
```

### `pastevents`

The `pastevents` command displays the 15 most recent command statements based on certain constraints. It is persistent across different shell runs.

**Example:**
```sh
<JohnDoe@SYS:~> pastevents
# Output the 15 most recent commands
<JohnDoe@SYS:~> pastevents execute 1
# Execute the command at position 1
<JohnDoe@SYS:~> pastevents purge
# Clear all pastevents
```

### **System Commands**

#### Foreground Process

Foreground processes wait for completion, and their output is displayed in the terminal.

**Example:**
```sh
<JohnDoe@SYS:~> sleep 5
# Executes sleep command in foreground
```

#### Background Process

Background processes run asynchronously. Their PIDs are displayed in the terminal.

**Example:**
```sh
<JohnDoe@SYS:~> sleep 10 &
# Executes sleep command in the background
```

### `proclore [pid]`

The `proclore` command provides information about a process with the given PID.

**Example:**
```sh
<JohnDoe@SYS:~> proclore
# Displays information about the shell process
<JohnDoe@SYS:~> proclore 301
# Displays information about the process with PID 301
```

### `seek [flags] [search] [target_directory]`

The `seek` command searches for a file/directory in the specified target directory. It supports `-d` (look for directories only) and `-f` (look for files only) flags.

**Example:**
```sh
<JohnDoe@SYS:~> seek newfolder ./doe
# Output matching files/directories in ./doe
```

## Processes, Files, and Miscellaneous

### I/O Redirection

The shell supports input/output redirection using `>`, `>>`, and `<` operators.

**Example:**
```sh
<JohnDoe@SYS:~> echo "Hello world" > newfile.txt
# Redirects output to newfile.txt
<JohnDoe@SYS:~> wc < a.txt
# Reads input from a.txt and performs word count
```

### Pipes

The shell supports piping multiple commands.

**Example:**
```sh
<JohnDoe@SYS:~> echo "Lorem Ipsum" | wc
# Pipes output of echo to wc command
```

### Redirection along with Pipes

The shell supports redirection along with pipes.

**Example:**
```sh
<JohnDoe@SYS:~> cat < a.txt | wc | sed 's/ //g' | cat > b.txt
# Redirects input from a.txt, pipes through wc and sed, and outputs to b.txt
```

### Process Management

The shell supports process management commands `fg` and `bg`.

**Example:**
```sh
<JohnDoe@SYS:~> fg 620
# Brings process with PID 620 to foreground
<JohnDoe@SYS:~> bg 620
# Changes process with PID 620 to background
```

### Neonate Command

The `neonate` command prints the Process-ID of the most recently created process at specified intervals until the 'x' key is pressed.

**Example:**
```sh
<JohnDoe@SYS:

~> neonate -n 4
# Prints PID every 4 seconds until 'x' is pressed
11810
11811
11811
11812
11813 # Key 'x' is pressed, terminating the printing
```

## Networking

### `iMan [command_name]`

The `iMan` command fetches man pages from http://man.he.net/ using sockets.

**Example:**
```sh
<JohnDoe@SYS:~> iMan sleep
# Fetches and displays the man page for the 'sleep' command
```