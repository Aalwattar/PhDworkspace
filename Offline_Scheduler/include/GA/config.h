/*******************************************************************************
 * Author   : Jennifer Winer
 * 
 * Project  : A DFG Off-Line Task Scheduler for FPGA
 *              - The Genetic Algorithm for determining the ideal implementation
 *                  for each task's operation
 * 
 * Created  : June 17, 2013
 * Modified : June 17, 2013
 ******************************************************************************/

/*******************************************************************************
 * Filename : config.h
 * 
 * Purpose  : a configuration file (to contain all defines)
 ******************************************************************************/


#ifndef CONFIG_H
#define	CONFIG_H


// locations of input files
#define DEFAULT_ARCH_FILENAME "input/architecture_library.txt"
#define DEFAULT_AIF_FILENAME  "input/B1_10_5.aif"


// FIX - normalize power + runtime to make them 75:25 instead
// Fitness weights
#define RUNTIME_WEIGHT 0.875
#define POWER_WEIGHT (1.0 - RUNTIME_WEIGHT)


// Population Parameters
#define DEFAULT_STOP_CONDITION 500
#define DEFAULT_POP_SIZE 50

// Genetic operator rates
#define DEFAULT_CROSSOVER_RATE 0.85
#define DEFAULT_MUTATION_RATE  0.005

// misc
#define BUFF_SIZE 500

#endif	/* CONFIG_H */

