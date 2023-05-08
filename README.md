# Unix-process-monitoring-tool

A command line tool that reports information about running processes including inodes, file descriptors, and more. To compile this program, open a terminal in the directory with all the files in it and run "make". To then execute it, run ./main.out with your desired command line arguments.

Here are the command line arguments it accepts:

--per-process, indicates that only the process file descriptor table will be displayed

--systemWide, indicates that only the system-wide file descriptor table will be displayed

--Vnodes, indicates that the Vnodes file descriptor table will be displayed

--composite, indicates that only the composed table will be displayed

--threshold=X, where X denotes an integer, indicating that processes which have a number of FD assigned larger than X should be flagged in the output.
For this it will list the PID and number of assigned FDs, e.g. PID (FD) and combination of these, i.e. the program can receive multiple of these flags combined.

Default behaviour: if no argument is passed to the program, the program will display the composite table, i.e. same effect as having used the --composite flag

Only one positional argument indicating a particular process id number (PID), if not specified the program will attempt to process all the currently running processes for the user executing the program

