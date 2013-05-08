GA
==

Implementation of a simple Genetic Algorithm for scheduling optimization

--------------------------------------------------------------------------------

PROGRAM OUTPUT
    - Error messages are currently being printed to stderr
    - All other program output is printed to stdout


TRIVIAL PROBLEM DEFINITION
    - uses the arbitrarily created table of architectures and their performances
        instead of real data from the scheduler

TRIVIAL PERFORMANCE FILE FORMAT
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




FUTURE IMPLEMENTATION IDEAS
    - Allow the user to enter their own seed with an option (eg. -rand_seed=1)
    - Create different ways of running the program (command line vs. GUI?)


QUESTIONS
    - what are the dimensions for area (assumed mm^2 for simplicity)