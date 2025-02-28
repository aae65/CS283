1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_My code ensures all child processes are complete before the shell continues by keeping track of the amoung of commands and looping through to wait for all of them to finish. If I didn't call this, I would most likely have several zombie processes running and the code wouldn't end unless forcefully quit._

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_If pipes weren't closed, several issues may happen. One of them being a resouce leak, where pipes or files would not be able to be open. Another issue is the process waiting forever for its previous process to finish._

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_`cd` is implemented as a built-in rather than an external command because any external processes are unable to modify the shell's enviornment. If it were external, forking would have to be considered. This would make the process itself more complicated._

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_The way I would implement an arbitrary number of piped commands is to begin with a command maximum. When the maximum is reached, it is increased and memory is reallocated to support the new command. The issue with this is how complex the memory will become, making it difficult to manage._
