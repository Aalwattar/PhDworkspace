GA
================================================================================
Implementation of a simple Genetic Algorithm for scheduling optimization



TO RUN:
--------------------------------------------------------------------------------

bin/GA.exe [arguments]
    [arguments] : -ps=XXX   // population size = XXX
                  -mr=XXX   // mutation rate = XXX (where 0 <= XXX <= 1)
                  -cr=XXX   // crossover rate = XXX (where 0 <= XXX <= 1)
                  -g=XXX    // run for XXX generations
                  -arch=XXX // XXX is the filename of the architecture library
                  -dfg=XXX  // XXX is the filename of the DFG

    - These arguments are all optional and can be entered in any order.
        eg. bin/GA.exe -arch=myfile.txt -mr=0.05 -g=1000

    - more options are currently in development


Default Values

    -ps=20
    -mr=0.001
    -cr=0.85
    -g=500
    -arch=architecture_library.txt
    -dfg=DFG.txt // My assumption was that I would be receiving the relevant DFG
                 // information from Napolion


Program Output

    - Error messages are currently being printed to stderr
    - All other program output is printed to stdout



FORMATS:
--------------------------------------------------------------------------------
Archetecture File Format
    - The very first line in the file MUST be as follows:
        Num_Tasks= #
            # = the number different tasks that this file contains
    - The properties of each task must be entered in the following order:
        <NAME> <COLUMNS> <ROWS> <CONFIGURATION_TIME> <EXECUTION_TIME> <CONFIGURATION_POWER> <EXECUTION_POWER>
    - each property must be an integer separated by one or more whitespaces

    - Lines beginning with # are ignored by the parser
    - all implementations of the same task MUST have the exact same name 

FUTURE IMPLEMENTATION IDEAS
--------------------------------------------------------------------------------
    - Allow the user to enter their own seed with an option (eg. -rand_seed=1)
