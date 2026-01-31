#include <stdio.h>
#include <string.h>

char *PolicyCheck(char *argOne);

char *RoundRobinIntegerCheck(char *argTwo);

int FileCheck(char *argthree, char *argtwo, char *scheduleType, char *fileName);

int FileReader(char *fileName, char *path);

int N;

int main(int argc, char *argv[]) 
{
    char *scheduleType;
    char *quantumCheck;
    char fileName[256];
    char path[500];

    //the checks below are to check if the first argument is asking to use Round Robin or First come first serve
    scheduleType = PolicyCheck(argv[1]);
    if (scheduleType == NULL) 
    {
        return 1;
    }

    //this will check the second argument if the schedule type is Round Robin, it will ensure the integer is greater than 0.
    if (strcmp(scheduleType, "Round Robin") == 0)
    {
        quantumCheck = RoundRobinIntegerCheck(argv[2]);
        if (quantumCheck == NULL) 
        {
            return 1;
        }
    }

    //this code will check the schedule type and get the file name from the user based on the scheduler they are using
    if(FileCheck(argv[3], argv[2], scheduleType, fileName) == 1)
    {
        return 1;
    }

    //this code will open the file, print the contents of the file, and close the file
    /*sprintf(path, "/workspaces/CMPT_360_Assignment_2/A2/Workload_Samples/%s", &fileName);
    FILE *fp = fopen(path, "r");
    if (!fp) 
    {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 1;
    }
    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL) 
    {
        printf("%s", line);
    }
    fclose(fp);*/

    if(FileReader(fileName, path) == 1)
    {
        return 1;
    }

    return 0;
}

char *PolicyCheck(char *argOne)
{
    //the checks below are to check if the first argument is asking to use Round Robin or First come first serve
    if(strcmp(argOne, "--policy=RR") == 0)
    {
        return "Round Robin";
    }
    else if(strcmp(argOne, "--policy=FCFS") == 0)
    {
        return "First come first serve";
    }
    else
    {
        fprintf(stderr, "Error: invalid argument, use either --policy=RR OR --policy-FCFS\n");
        return NULL;
    }
}

char *RoundRobinIntegerCheck(char *argTwo)
{
    //this will check the second argument if the schedule type is Round Robin, it will ensure the integer is greater than 0.
        if (sscanf(argTwo, "--quantum=%d", &N) == 1)
        {
            if (N > 0)
            {
                return "True";
            }
            else
            {
                fprintf(stderr, "Error: Quantum must be greater than 0.\n");
                return NULL;
            }
        }
        else
        {
            fprintf(stderr, "Error: Invalid format for quantum. Use --quantum=N\n");
            return NULL;
        }
    
}

int FileCheck(char *argthree, char *argtwo, char *scheduleType, char *fileName)
{
    //this code will check the schedule type and get the file name from the user based on the scheduler they are using
    if(strcmp(scheduleType, "First come first serve")==0)
    {
        if (sscanf(argtwo, "--in=%s", fileName) == 1)
        {
            return 0;
        }
        else
        {
            fprintf(stderr, "Error: Invalid format. Use --in=FILE\n");
            return 1;
        }
    }
    else if(strcmp(scheduleType, "Round Robin")==0)
    {
        if (sscanf(argthree, "--in=%s", fileName) == 1)
        {
            return 0;
        }
        else
        {
            fprintf(stderr, "Error: Invalid format. Use --in=FILE\n");
            return 1;
        }
    }
}

int FileReader(char *fileName, char *path)
{
    sprintf(path, "/workspaces/CMPT_360_Assignment_2/A2/Workload_Samples/%s", fileName);
    FILE *fp = fopen(path, "r");
    if (!fp) 
    {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return 1;
    }
    char line[256];

    while (fgets(line, sizeof(line), fp) != NULL) 
    {
        printf("%s", line);
    }
    fclose(fp);
    return 0;
}