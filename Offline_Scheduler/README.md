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


DEFAULT VALUES
    -ps=20
    -mr=0.001
    -cr=0.85
    -g=500
    -arch=architecture_library.txt
    -dfg=DFG.txt // My assumption was that I would be receiving the relevant DFG
                 // information from Napolion


PROGRAM OUTPUT
    - Error messages are currently being printed to stderr
    - All other program output is printed to stdout



FORMATS:
--------------------------------------------------------------------------------
TRIVIAL ARCHITECTURE DEFINITION
    - uses the arbitrarily created table of architectures and their performances
        instead of real data from the scheduler

TRIVIAL ARCHITECTURE FILE FORMAT
    - ALL FOUR OPERATIONS ARE ASSUMED TO BE PRESENT IN THE FOLLOWING ORDER
    - {a, s, m, d} <number of architectures of that operation type>
        eg. a 6
        eg. d 4

    - this is immediately followed by each architecture in the following format
        <speed in ns>    <power consumption in milli Watts>    <Area mm^2>

    - There should be a newline between each operation
    - ALL WHITESPACE MUST BE EXACTLY ONE SPACE!!!

    - my dummy file multiplies each operation's runtime by the following
        operator, but keeps the ratio of the runtime to area, and the runtime
        to the power consumption constant
            a = 1x
            s = 1.5x
            m = 4x
            d = 5x

TRIVIAL DFG FILE FORMAT
    - The first line of the file must contain how many nodes follow

    - it must contain only the operation that each node will take
    - each line is one operation



FUTURE IMPLEMENTATION IDEAS
--------------------------------------------------------------------------------
    - Allow the user to enter their own seed with an option (eg. -rand_seed=1)