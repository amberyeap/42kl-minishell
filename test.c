#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        printf("Child process is exiting.\n");
        exit(0); // This only terminates the child
    } else { // Parent process
        printf("Parent process is running.\n");
		pid_t pid2 = fork();

		if (pid2 == 0)
		{
			printf("Child2 process is running.\n");
			exit(0);
		}
        wait(NULL); // Optionally wait for the child to exit
        wait(NULL); // Optionally wait for the child to exit
    }

    printf("Parent process continues running.\n");
    return 0;
}
