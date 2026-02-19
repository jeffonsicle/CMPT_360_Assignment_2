# CMPT_360_Assignment_2

Student Name: Jeffrey Moniz
Student ID: 3148591
Submission Date: February 20th 2026
File Name: README.md

# 1. Academic Integrity
“I certify that this submission represents entirely my own work.”

# 2. Overview
This program is designed to take a input from the user in the terminal, and the user can decide on two different policies: Round Robin or First Come First Serve. if the user picks FCFS the user only needs to input the file name: --policy=FCFS --in=W1. But if the user picks the Round Robin policy then the user will also need to pick a quantum number: --policy=RR --quantum=2 --in=W1. The program will then either enter the FCFS function and print the file using the FCFS schedule policy, or the program will enter the RR function and print the file using the RR schedule policy. The program will also print the first run, completion, TAT and RESP of each of the processes. The program will also print the number of context switches, average turnaround time and average response time of the program

# 3. Build
In order to run the program you need to type 'make' in the command line, then type ./sched --policy=FCFS --in=(file name) or type ./sched --policy=RR --quantum=(number) --in=(file name) to get the program to run and print the desired results. typing 'make clean' will get rid of the output files. 

# 4. Status
The program also runs perfectly fine in both Git and the student server