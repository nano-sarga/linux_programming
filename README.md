# Programming in Linux ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧

A collection of C++ system programming projects demonstrating **file I/O**, **process management**, **inter-process communication**, **multithreading**, and **external algorithms**. Each project includes a Makefile for compilation and testing.


## Project Overview ── ⋆⋅☆⋅⋆ ──

### 🗂 ENV (Environment / Basic Utilities)
| Program | Description | Usage |
|---------|-------------|-------|
| `min_max` | Finds min and max among 3 integers from command line | `./min_max arg1 arg2 arg3` |
| `reverse` | Prints command-line arguments in reverse order | `./reverse arg1 arg2 ...` |
| `Complex` | Demonstrates operations on complex numbers (add, sub, multiply, multiply by constant) and sorts a vector of complex numbers | `./Complex` |

### 🗂 FS (File System) 
| Program | Description | Usage |
|---------|-------------|-------|
| `copy` | Copies files and counts data vs holes | `./copy source dest` |
| `read-file` | Prints file content | `./read-file file` |
| `hacker-rm` | Overwrites and deletes file | `./hacker-rm file` |
| `redirect-cin` | Redirects stdin from a file | `./redirect-cin file` |
| `manual-append` | Appends multiple lines using duplicated file descriptors | `./manual-append file` |

### 🗂 IPC (Inter-Process Communication) 
| Program | Description | Usage |
|---------|-------------|-------|
| `signal-echo` | Handles `SIGUSR1` signals, prints sender info | `./signal-echo` |
| `prime-calculator` | Parent-child pipes for n-th prime calculation | Interactive input |
| `shared_array` | Shared memory array between processes with semaphore sync | Run `first` and `second` |
| `server/client` | Multi-client TCP chat | `./server` + `./client` |

### 🗂 PS (Process & Shell) 
| Program | Description | Usage |
|---------|-------------|-------|
| `do-command` | Executes a command and prints time & exit code | `./do-command ls -l` |
| `interactive-shell` | Minimal shell with operators (`&&`, `||`, `;`), redirection, background tasks | `./interactive-shell` |

### 🗂 TH (Threads) 
| Program | Description | Usage |
|---------|-------------|-------|
| `array-summary` | Sum of array using single & multi-threading | `./array-summary <num_elements> <num_threads>` |
| `parallel_scheduler` | Thread pool executing tasks in parallel | `./demo` |

###  🗂 Backup / Utility 
| Program | Description | Usage |
|---------|-------------|-------|
| `externalSort` | Sorts large file using external memory blocks | Prepare `input.txt`, run executable |


## Build Instructions ⚒

Most projects include a Makefile:

```bash
cd <project-directory>
make        # Build the executable
./<target>  # Run the program
make clean  # Remove compiled files
```
+ Compiler: g++ with -std=c++17 or -Wall -Wextra -O2
+ Dependencies: POSIX system calls, pthread (for threading), sockets (IPC-4), semaphores (shared memory).

## Key Concepts Covered ꄗ

* File I/O, sparse files, redirection, open, read, write
* Signals and sigaction
* Pipes and parent-child process communication
* Shared memory and semaphores
* TCP client-server architecture with pthreads
* Process creation and shell command execution (fork, execvp, waitpid)
* Multithreading, thread pools, synchronization primitives (mutex, condition_variable)
* External memory algorithms and sorting large datasets
