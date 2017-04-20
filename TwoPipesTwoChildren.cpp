// file: TwoPipesTwoChildren.cpp
// author: Dylan Yu
// date: 04/02/2017
// purpose: CS3376-502
// description:
// This gives the output of the command "ls -ltr | grep 3376 | wc -l" using two pipes
// and two children

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(int argc, char **argv)
{
    int status;
    int childpid, child2pid;
    char *cat_args[] = {"ls", "-ltr", NULL};
    char *grep_args[] = {"grep", "3376", NULL};
    char *wc_args[] = {"wc", "-l", NULL};
    // create one pipe to send the output of "ls" process to "grep" process
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    // fork the first child (to execute cat)
    if((childpid = fork()) == -1)
    {
        perror("Error creating a child process");
        exit(1);
    }      
    if (childpid == 0) 
    {   
        // replace cat's stdout with write part of 1st pipe
        dup2(pipe1[1], 1);

        // close all pipes (very important!); end we're using was safely copied
        close(pipe1[0]);
	close(pipe1[1]);

	execvp(*cat_args, cat_args);
	exit(0);
    }
        pipe(pipe2);
	if((child2pid = fork()) == -1)
	{
	    perror("Error creating second child process");

	    close(pipe1[0]);
	    close(pipe1[1]);
            close(pipe2[0]);
	    close(pipe2[1]);

	    exit(1);
	}
	
	if(child2pid == 0)
	{
	    //replace grep's stdin with read end of 1st pipe
	    dup2(pipe1[0], 0);

	    close(pipe1[0]);
	    close(pipe1[1]);

	    //replace grep's stdout with write end of 2nd pipe
	    dup2(pipe2[1], 1);

	    //close pipes
	    close(pipe2[0]);
	    close(pipe2[1]);

            execvp(*grep_args, grep_args);

            exit(0);
	}
	else
	{
	    //close pipes
	    close(pipe1[0]);
	    close(pipe1[1]);

	    //replace wc's stdin with read end of 2nd pipe
	    dup2(pipe2[0], 0);

            //close pipes
	    close(pipe2[0]);
	    close(pipe2[1]);

	    execvp(*wc_args, wc_args);
        }
    return(0);
}
