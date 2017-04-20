// file: DynPipe.cpp
// author: Dylan Yu
// date: 04/02/2017
// purpose: CS3376-502
// description:
// This program takes between two to five commands and pipelines them together. 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <string>
#include <cstring>
int main(int argc, char **argv)
{
    int status;
    int childpid, child2pid, child3pid, child4pid;
    char *arg1[3];
    char *arg2[3];
    char *arg3[3];
    char *arg4[3];
    char *arg5[3];
    if(argc < 3 || argc > 6)
    {
        perror("Error: Invalid number of arguments!");
	exit(1);
    }
    for(int i = 1; i < argc; i++)
    {
        std::istringstream iss(argv[i]);
        std::string cmd, param;
        iss >> cmd;
        char *string = new char[cmd.length() + 1];
        std::strcpy(string, cmd.c_str());
        iss >> param;
        char *string2 = new char[param.length() + 1];
        std::strcpy(string2, param.c_str());
        if(i == 1)
        {
            arg1[0] = string;
            arg1[1] = string2;
            arg1[2] = NULL;
        }
        if(i == 2)
        {
            arg2[0] = string;
            arg2[1] = string2;
            arg2[2] = NULL;
        }
        if(i == 3)
        {
            arg3[0] = string;
            arg3[1] = string2;
            arg3[2] = NULL;
        }
        if(i == 4)
        {
            arg4[0] = string;
            arg4[1] = string2;
            arg4[2] = NULL;
        }
        if(i == 5)
        {
            arg5[0] = string;
            arg5[1] = string2;
            arg5[2] = NULL;
        }
    }
    // create one pipe to send the output of "ls" process to "grep" process
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];

    pipe(pipe1);
    
    if((childpid = fork()) == -1)
    {
        perror("Error creating first child");
	exit(1);
    }
    if(childpid == 0)
    {
        // replace first stdout with write part of 1st pipe
        dup2(pipe1[1], 1);

	//close pipes
	close(pipe1[0]);
	close(pipe1[1]);

	//execute function
	execvp(*arg1, arg1);
	exit(0);
    }
    if(argc == 3)
    {
        //replace stdin with read part of 1st pipe
	dup2(pipe1[0], 0);

	//close pipes
	close(pipe1[0]);
	close(pipe1[1]);

	//execute function
	execvp(*arg2, arg2);
    }
    else
    {
	//open second pipe
	pipe(pipe2);
        if((child2pid = fork()) == -1)
	{
	    perror("Error creating second child");
	    exit(1);
	}
	if(child2pid == 0)
	{
            //replace stdin with read part of 1st pipe
	    dup2(pipe1[0], 0);

	    //close first pipe
	    close(pipe1[0]);
	    close(pipe1[1]);

	    //replace stdout with write part of 2nd pipe
	    dup2(pipe2[1], 1);

	    //close pipes
	    close(pipe2[0]);
	    close(pipe2[1]);

	    execvp(*arg2, arg2);
	    exit(0);
	}
	if(argc == 4)
	{
	    //close first pipe
	    close(pipe1[0]);
	    close(pipe1[1]);

	    //replace stdin with read part of 2nd pipe
	    dup2(pipe2[0], 0);
        
            //close second pipe
	    close(pipe2[0]);
	    close(pipe2[1]);

            execvp(*arg3, arg3);
	}
	else
	{
	    //close first pipe
	    close(pipe1[0]);
	    close(pipe1[1]);
	    //open third pipe
	    pipe(pipe3);
	    if((child3pid = fork()) == -1)
	    {
	        perror("Error creating third child");
		exit(1);
	    }
	    if(child3pid == 0)
	    {
	        //replace stdin with read part of 2nd pipe
		dup2(pipe2[0], 0);

		//close second pipe
		close(pipe2[0]);
		close(pipe2[1]);

		//replace stdout with write part of 3rd pipe
		dup2(pipe3[1], 1);

		//close third pipe
		close(pipe3[0]);
		close(pipe3[1]);

		execvp(*arg3, arg3);
		exit(0);
	    }
	    if(argc == 5)
	    {
			//close second pipe
			close(pipe2[0]);
			close(pipe2[1]);
			
			//replace stdin with read part of third pipe
			dup2(pipe3[0], 0);
			
			//close third pipe
			close(pipe3[0]);
			close(pipe3[1]);
			
			execvp(*arg4, arg4);
	    }
		else
		{
		    //close second pipe
			close(pipe2[0]);
			close(pipe2[1]);
			
			//open fourth pipe
			pipe(pipe4);
			if((child4pid = fork()) == -1)
			{
			    perror("Error creating fourth child");
			    exit(1);
			}
			if(child4pid == 0)
			{
				//replace stdin with read part of 3rd pipe
			    dup2(pipe3[0], 0);

			    //close third pipe
			    close(pipe3[0]);
			    close(pipe3[1]);

			    //replace stdout with write part of 4th pipe
			    dup2(pipe4[1], 1);

			    //close fourth pipe
			    close(pipe4[0]);
			    close(pipe4[1]);

			    execvp(*arg4, arg4);
			    exit(0);
			}
			//close third pipe
			close(pipe3[0]);
			close(pipe3[1]);
			
			//replace stdin with read part of 4th pipe
			dup2(pipe4[0], 0);
			
			//close fourth pipe
			close(pipe4[0]);
			close(pipe4[1]);
			
			execvp(*arg5, arg5);
		}
	}

    }

    return(0);
}
