# Mini Scanner Implementation

This implementation scans executable files in the `/.mini.d/` directory (or the directory specified by the `MINI_DIR` environment variable) and executes them using fork & exec.

## Files

- `mini.c`: The main implementation
- `mini`: The compiled executable
- `/.mini.d/`: Directory containing executable files to be scanned and executed (default directory, can be overridden with `MINI_DIR` environment variable)

## Implementation Details

### Directory Scanning
The program uses `opendir()` and `readdir()` functions to iterate through all files in the `/.mini.d/` directory (or the directory specified by the `MINI_DIR` environment variable).

### Executable Detection
The `is_executable()` function checks if a file is executable by:
1. Using `stat()` to get file information
2. Verifying it's a regular file (`S_ISREG`)
3. Checking if it has user execute permission (`S_IXUSR`)

### Process Execution
For each executable file found, the program:
1. Uses `fork()` to create a child process
2. In the child process, uses `execl()` to replace the process with the executable file
3. In the parent process, uses `waitpid()` to wait for the child process to complete
4. Reports the exit status of the child process

## Sample Executables
Three sample files are included in `/.mini.d/`:
1. `script1.sh`: A bash script that prints its arguments
2. `script2.sh`: A bash script that lists the root directory
3. `program`: A compiled C program that prints a message and its process ID
4. `program.c`: The source code for the C program (not executable)

## API Functions

### `int is_executable(const char *filepath)`
Checks if a file is executable.
- Parameters: `filepath` - Path to the file
- Returns: 1 if executable, 0 otherwise

### `void execute_file(const char *filepath)`
Executes a file using fork & exec.
- Parameters: `filepath` - Path to the executable file

### `void scan_and_execute(const char *dir_path)`
Scans a directory for executable files and executes them.
- Parameters: `dir_path` - Path to the directory to scan

## Environment Variables

### `MINI_DIR`
Specifies the directory to scan for executable files. If not set, the program defaults to `/.mini.d/`.

Example:
```bash
# Use custom directory
MINI_DIR=./scripts ./mini

# Use default directory (/.mini.d/)
./mini
```

## Compilation and Execution
```bash
# Compile the scanner
gcc -o mini mini.c

# Run the scanner (uses /.mini.d/ by default)
./mini

# Run the scanner with custom directory
MINI_DIR=./my_scripts ./mini
```
