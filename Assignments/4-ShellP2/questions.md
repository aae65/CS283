
1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  _Instead of just using `execvp` directly, `fork` is used to prevent issues with the parent process. It is esentially used as a test to stop possible damage._

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  _If the `fork()` system call fails, no child process is created. My program prevents this by first checking the outputs from `fork()`, then determining the best option. If it fails, the return code is set to an error and the code breaks._

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  _`execvp()` uses the variable (or file) inputted to run the command. It uses the `PATH` enviornment variable to see what directories are listed._

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  _The purpose of calling `wait()` in the parent process after forking is to prevent the parent process from continuing before the status of the child process is given. If it wasn't called, the child could become a zombie process or errors would not be properly handled._

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  _Using `WEXITSTATUS()` means that the code cannot proceed until the child process is finished and has exited. This is important as the code relies on the child process._

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  _My code iterates through the token, removes the quotes, and stores the entire string into the respective variable in the struct. This is necessary to not only preserve strings entered with `echo` but also preserve arguments that require spaces (like a directory or file path that contains spaces)._

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  _I did not have to make too many changes to my logic as I handle the commands after they are entered into the struct. The main issue I came across was how to handle the commands without using a list._

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  _The purpose of a signal in a Linux system is to notify the system. Each signal has a specific purpose. Signals differ from other IPCs as they are mostly used for asynchronous notifications._

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  _`SIGSEGV` is sent when there is a segmentation fault or violation. `SIGQUIT` is used for debugging purposes to help identify why a process quits unexpectedly. `SIGILL` is sent when an instruction cannot be executed._

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  _A process stops when it recieves `SIGSTOP`. It cannot be handled, ignored, or blocked. This is because the process itself does not see the signal. It is meant to be ignored._
