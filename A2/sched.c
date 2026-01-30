#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    char *scheduleType;
    char *quantumCheck;
    int N;
    char *fileName;
    char path[500];

    //the checks below are to check if the first argument is asking to use Round Robin or First come first serve (clean this and add it to a seperate function called PolicyCheck)
    if(strcmp(argv[1], "--policy=RR") == 0)
    {
        scheduleType = "Round Robin";
    }
    else if(strcmp(argv[1], "--policy=FCFS") == 0)
    {
        scheduleType = "First come first serve";
    }
    else
    {
        fprintf(stderr, "Error: invalid argument, use either --policy=RR OR --policy-FCFS\n");
        return 1;
    }

    //this will check the second argument if the schedule type is Round Robin, it will ensure the integer is greater than 0. (clean this, add to a seperate function called RoundRobinIntegerCheck)
    if (strcmp(scheduleType, "Round Robin") == 0)
    {
        if (sscanf(argv[2], "--quantum=%d", &N) == 1)
        {
            if (N > 0)
            {
                quantumCheck = "True";
            }
            else
            {
                fprintf(stderr, "Error: Quantum must be greater than 0.\n");
                return 1;
            }
        }
        else
        {
            fprintf(stderr, "Error: Invalid format for quantum. Use --quantum=N\n");
            return 1;
        }
    }

    //this code will check the schedule type and get the file name from the user based on the scheduler they are using (to clean the code put this in a seperate function called FileCheck)
    if(strcmp(scheduleType, "First come first serve")==0)
    {
        if (sscanf(argv[2], "--in=%s", &fileName) == 1)
        {
            //continue with the program, if this was in another function just return the filename
        }
        else
        {
            fprintf(stderr, "Error: Invalid format. Use --in=FILE\n");
            return 1;
        }
    }
    else if(strcmp(scheduleType, "Round Robin")==0)
    {
        if (sscanf(argv[3], "--in=%s", &fileName) == 1)
        {
            //continue with the program, if this was in another function just return the filename
        }
        else
        {
            fprintf(stderr, "Error: Invalid format. Use --in=FILE\n");
            return 1;
        }
    }

    //this code will open the file and close the file
    sprintf(path, "/workspaces/CMPT_360_Assignment_2/A2/Workload_Samples/%s", &fileName);
    FILE *fp = fopen(path, "r");
    if (!fp) 
    {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 1;
    }
    printf("Opened file: %s\n", path);
    fclose(fp);

    return 0;
}