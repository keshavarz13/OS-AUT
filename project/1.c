#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void even_sum(char* result, char* str_mum);

int main()
{
    pid_t pid;
    int pipe_dir1[2], pipe_dir2[2];
    
    if (pipe(pipe_dir1) == -1)
    {
        printf("First pipe creation failed.");
        return 1;
    }
    if (pipe(pipe_dir2) == -1)
    {
        printf("Second pipe creation failed.");
        return 1;
    }

    pid = fork();
    
    if (pid < 0)
    {
        printf("Failed to create a child process\n");
    }
    else if (pid > 0)
    {
        char input[100];
        char pipe_input[100];
        printf("enter number : ");
        scanf("%s", input);
        
        close(pipe_dir1[0]); 
        write(pipe_dir1[1], input, strlen(input) + 1);
        close(pipe_dir1[1]);
        printf("Send data from parent to child\n");

        wait(NULL);
        
        close(pipe_dir2[1]);
        read(pipe_dir2[0], pipe_input, 100);
        printf("Get data from child\n");
        printf("%s\n", pipe_input);
        close(pipe_dir2[0]);

        printf("End process\n");
    }
    else
    {
        int i, x;
        char pipe_input[100];
        char pipe_output[100] = "The sum of even digits in the input number:";
        char sum[100];

        close(pipe_dir1[1]);
        read(pipe_dir1[0], pipe_input, 100);
        printf("Get data from parent\n");

        close(pipe_dir1[0]);

        close(pipe_dir2[0]);
        even_sum(sum, pipe_input);
        x = strlen(pipe_output);
        for (i = 0; i < (int)strlen(sum); i++)
            pipe_output[x + i] = sum[i];
        pipe_output[x + i] = '\0';
        write(pipe_dir2[1], pipe_output, strlen(pipe_output) + 1);
        close(pipe_dir2[1]);
        printf("Send data from child to parent\n");
    }

    return 0;
}

void even_sum(char* result, char* str_num)
{
    const int BASE = 48;
    int sum = 0, x, i;

    for (i = 0; i < strlen(str_num); i++)
    {
        x = (int)str_num[i] - BASE;
        if (x % 2 == 0)
            sum += x;
    }
    sprintf(result, "%d", sum);
}