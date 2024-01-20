# Description

The complete shell is based on the Linux shell and its functionalities

## Runnning Shell  

To run the shell, from the root directory of project run 'make' followed by './a.out'

## Implementations

- **Prompt**  
    - Copy of the command-line prompt in Linux shell
    - 'prompt.c' is its code source

- **Input Requirements**  
    - Delimiters like ';' and '&' have been handled
    - This has been implemented in the 'main.c' file itself since it just required to split the input and pass it for further processing

- **Warp**
    - The 'warp' function is the implementation of the 'cd' command in Linux
    - 'warp.c' is its code source
    - '.', '..', '-' and '~' have also been handled
    - Sequential warp has also been implemented

- **Peek**
    - The 'peek' function is the implementation of the 'ls' command in Linux
    - 'peek.c' is its code source
    - '-a' and '-l' flags have been handled and output is in lexicographic order
    - The folders are printed in blue, executables in green and other files in white

- **Pastevents**
    - The 'pastevents' function is similar to the 'history' command in Linux
    - 'pastevents.c' is its code source
    - Stores/displays maximum 15 commands
    - 'pastevents purge' clears out the stored history
    - 'pastevents execute <index>' executes the command at the specified index
    - Repeated commands are not stored

- **System Commands**
    - All system commands are handling using the 'execvp' function
    - Any background processes (indicated by '&') are implemented in a separate function
    - Code source for the background process handling is 'background.c'
    - On running a background process, its pid is outputted
    - When the background process finishes, its status is outputted along with the next command implementation
    - Running an invalid command outputs an error

- **Proclore**
    - Prints information about the process whose pid is mentioned
    - If no pid is mentioned, then gives information about the shell
    - 'proclore.c' is its code source
    - Information includes  the pid, process status, process group id, virtual memory and the executable path of the process

- **Seek**
    - Seek works as a find function
    - Code source is 'seek.c'
    - The function searches for the specified input in the given directory and its sub-directories
    - Folders are printed in blue and other files are printed in green
    - The '-f', '-d' and '-e' flags have been implemented

# Assumptions

- The home directory (in case of '~') is taken as '/home/user'
- File and directory names shouldn't contain spaces or special characters like ';' and '&'
- The 'pastevents execute' function can only run the system commands and not the user-implemented functions
- 'command_store.txt' file is used to save the history of commands after the shell has exited. It is assumed that shell will be run as a single process and no 2 processes will run the shell concurrently.
- Every user input will be stored as it is in history, given it is not same as the last input, does not have the string 'pastevents' in it and contains at least one character other than space.