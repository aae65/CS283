1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  _`fgets()` is a good choice for this application because it is easy to use with `stdin`. You can set a limit to how long you want the code to read, making it easy to determine if input is too long. It also returns a string that is saved into the variable provided._ 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  _If we knew how long each command was going to be, we would have a fixed-size array. We do not know how long the input will be, but we do have a maximum, so this is better._


3. In `dshlib.c`, the function `build_cmd_list()` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  _If a command with either a leading or trailing space were to be executed, it would not be able to. This is because the commands are case sensitive. Removing both types of spaces makes executing easier._

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  _- output to stdout to a file: there would not be a huge issue with this one, as you can simply open the file and write to it instead of printing to stdout._ 
    >             _-command to a command: the issue here would most likely be error handling. If the first command had an error, the second command may not be able to understand or handle it._
    >            _-pipe redirection: this could cause many issues as we are working with multiple commands and redirections at once._
- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  _Redirection is taking the output of one command and using it as input for the next. Piping is executing multiple commands at once._

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  _Separating `**stdout**` and `**stderr**` is important as error logging can be crucial. `**stdout**` is only output, so you cannot access it if anything goes wrong with your code._

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  _We could theoretically implement `**stderr**` by logging errors in a separate file. This file would be added to when a new error is logged with the command and error message. The user could type a command to output the most recent errir logged, thus combining them as well._