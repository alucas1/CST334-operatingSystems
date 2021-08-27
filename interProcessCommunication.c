/*
 Name: Alberto Lucas
 Date: 03/23/2021
 Title: Lab3 – task 3
 Description: This program provides an example as to how output gets piped
 into input. Specifically, this program replicates the behavior of the 
 "cat /etc/passwd | grep root" command. To do this, a child process executes
 the cat command and copies its upstream into stdout. The other child then 
 copies the downstream into stdin and runs the grep command.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//main
int main()
{
    int fds[2];
    pipe(fds);

    if (fork() == 0)
    { /*child duplicates upstream into stdout */
        dup2(fds[1], 1);
        close(fds[0]);
        execlp("cat", "cat", "/etc/passwd", (char *)NULL);
    }
    else if (fork() == 0)
    { /*child duplicates downstream into stdin */
        dup2(fds[0], 0);
        close(fds[1]);
        execlp("grep", "grep", "root", (char *)NULL);
    }
    else
    { /*parent closes both ends and wait for children*/
        close(fds[0]);
        close(fds[1]);
        wait(0);
        wait(0);
    }
    return 0;
}