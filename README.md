<center><h1><b>Ashell: A LINUX Shell</b></h1></center>
<br>
<br>
Welcome to Ashell. This is a simple shell made for systems running on some form of LINUX distribution.
<br><br>
Following are the commands that have been built into Ashell:
<br><br>
<ul>
<li><b>cd</b>: <p>
    A command to change the working directory to the one indicated by the user as an argument to this command. If no argument is provided, the current working directory remains unchanged. The target directory's path can also be described relative to the root directory.
    </p>
</li>
<li><b>ls</b>: <p>
    A command to display the contents of the specified directory. Again, the path of this directory can be absolute or relative to the current working directory. The user can also choose to pass flags, to specify if they wish to display all contents of the directory, even the hidden ones. The user can also wish to ask for more information about each of the directory's contents, such as read/write permissions, last modification time, and so on.
    </p>
</li>
<li><b>pwd</b>: <p>
    A simple command to print the absolute path of the current working directory to standard output. This command takes no arguments.
    </p>
</li>
<li><b>echo</b>: <p>
    A simple command to print the string passed as an argument to this command on standard output.
    </p>
</li>
<li><b>clear</b>: <p>
    A command to shift all the commands that have been previously run on Ashell up so that the standard output is "cleared". 
    </p>
</li>
<li><b>history</b>: <p>
    A command to display some of the recently run commands on Ashell. This command can display the 10 most recent commands run on Ashell. If the same command was run more than once consequtively, it will only show up once in the history. The complete history of the 20 most recent commands is stored in a hidden file called ".history.txt".
    </p>
</li>
<li><b>pinfo</b>: <p>
    A command to display information regarding a process. If no argument is provided, the command displays information regarding the current process, i.e. the current instance of the shell. The command however does also accept the pid of any process as input. On receiving a valid pid, this command will display this pid first, followed by the status of the process (R/S/Z) and whether or not it is in the foreground (+). It also shows the amount of virtual memory (in bytes) that the process is currently using, and the absolute path of the executable of the process.
    </p>
</li>
<li><b>discover</b>: <p>
    This command can be used to search for a file or directory within a directory. It can also be used to display all the files and/or directories within a given directory.
    </p>
</li>

Additionally, we can also run processes not directly built into the shell (only in the foreground though :P)
