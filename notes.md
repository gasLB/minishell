
# Minishell preliminary notes

## General instructions

the minishell should: 

- display a prompt when waiting for a new command
- have a working history
- search aand launch the right executable
- at most one global variable to indicate a received signal. Can only provide the number of the received signal.

## Functions

### Readline

- _char *readline(const char *prompt)_: returns a line from terminal using promt as prompt. Need to free the returned line -> compile with *-lreadline*

- _void add_history(const char *line)_ : save the line away in a history list (an internal linked list)

- _void rl_clear_history(void)_: clears the history list by deleting all of the entries and frees data that readline saves in the history list (but not the list line returned from readline)

- _int rl_on_new_line(void)_: reset readline's internal notion of the current cursor position to the beginning of a new line

- _void rl_replace_line(const char *text, int clear_undo)_: replace the content of readline's line buffer with the text, updates the cursor's position, optionnaly clears the undo list if clear_undo is non-zero

### Cwd

In all following functions, path can be either absolute or relative.

- _char *getcwd(char *buf, size_t size)_: get current working directory. if buf is non-NULL -> ppath is copied into it, returns a pointer to the same buffer. if buff is NULL -> allocates dynamic memory for us and store the path in it. /!\ This memory needs to be freed.

- _int chdir(const char *path)_: changes the current working directory to path. /!\ Changes are local to that process only.

### sys/stat.h

the structure **stat** contains a file serial number and device ID that identify a file.

- _int stat(const char *path, struct stat *buf)_: obtains information about the named file and writes it to the stat structure pointed by buf. permissions are not required but file sould be reachable. 

- _int fstat(int fd, struct sat *buf)_: obtains information about an open file associated with a file descriptor.

- _int lstat(const char *path, struct stat *buf)_: same as stat() except when path refers to a symbolic link.

### Directories

In linux, directories are special files with unique properties. A directory is a table that maps filenames to inodes. Each entry (= row) on this list is implemented as a **dirent** (directory entry) C structure. 
An **inode** is a system-level data structure unique for each file (including directories) and contain all metadatas except the file's name and its content. Show them by runnig *ls -i*. 

Directory "example_dir" (inode #5342):
+-----------------+-------------+
| Name            | Inode #     |
+-----------------+-------------+
| .               | 5342        | (self-reference)
| ..              | 1234        | (parent directory)
| file1.txt       | 7890        |
| file2.txt       | 7891        |
| subdirectory    | 7892        |
+-----------------+-------------+

A **stream** is an abstract concept representing a communicaton channel. The C data structure that represents a stream is called **FILE** for file and **DIR** for directories. 
FILE objects are allocated and managed internally by the library and should only be dealt with using pointers *FILE. 
open() or close() are used to access FILE objects. Standard streams (stdin, stdout, stderr) are *FILE objects predefined in the main.
Streams are an alternative to **file descriptors** but those interfaces should not be confused. Streams are built on top of file descriptors.

- _DIR *opendir(const char *name)_: opens a directory stream from name.

- _struct dirent *readdir(DIR *dirp)_: returns a pointer to the next directory entry in the directory stream pointed to by dirp. returns NULL on reaching the end of the stream or error. Does its own parsing of directory structures internally.

- _int closedir(DIR *dirp)_: close the directory stream. If successful, closes all underlying file descriptors associated with dirp.

### Errors

- _char *strerror(int errnum)_: convert error number (errum) to a string. Describes the error and returns a pointer to the string. Useful for manual error handling.

- _void perror(const char *s)_: print formatted error message indicated by s to stderr. Useful for quick error reporting.

### Terminals

A **device driver** is a software program that "translates" the general operations requested by the OS to specific commands for a hardware device.
In linux, a **device file** is the user-friendly interface that links to the device driver. It appear in the file system as an ordinary file. It allows for request-response cycles with the OS via standard file I/O system calls like read() or write(). Device files are typically located in /dev. 
A **TTY** or virtual terminal is represented by a device file. It is a command-line interface that allows user to interact with the OS. It has many more features than a regular file: interprets special characters (Ctrl+D, Ctrl+C), editing capabilities, has dimensions (row/colums), can echo input, etc... 

- _int isatty(int fd)_: tests weither the fd is associated with a terminal.

- _char **ttyname(int fd)_: returns the pathname of the terminal device associated with a fd. for ex: /dev/pts/2

- _int ttyslot(void)_: outdated.

- _int ioctl(int fd, unsigned long op, ...)_: general-purpose system call that is used as the primary interface for operations that are unique to specific device type. fd should be open. op is a device-dependent operation code, usually encode as a **IOCTL** macro. Useful to interact with terminals outside of read / write / seek system calls. It can take more arguments if required by some commands.

### Termios

The termios header includes the IOCTL macros to handling terminals. The **termios** structure store terminal attributes like stripping characters, automatic newline conversion, character size, ....
k

- _int tcsetattr(int fildes, int optional_actions, const struct termios *termios_p)_: set terminal attributes. optional_actions determines when the change take effects.

- _int tcgetattr(int fd, struct termios *termios_p)_: get the parameters associated with the terminal reffered to by fd and stores them into termios_p. 

### Termcap

**tgentent** routines: set of functions included as a conversion aid for programs that use the termcap library.
Alternative to termios

--- 

## GENERAL STEPS

Lexical analysis/tokenization: Breaking input into tokens
Parsing: Converting tokens into a command structure
Expansion: Handling variables, wildcards, etc.
Execution: Running the commands

=> in this order

## Crash Test list

test with bash --posix

- stocker errno dans une variable car valgrind en change la valeur
- overflow environment -> list too long
- input / output in any order -> >out1 echo hello <input 
- delete the current folder -> pwd
- ABC="echo hello" -> hello
- ABC="echo hello >outfile" -> hello >outfile
- ABC=D* -> all files wildcard
- ABC=sD*seioufisu -> no wildcard
- exit: 266666666666666666666: numeric argument required
- exit 257 -> 1
- minishell inside minishell 
