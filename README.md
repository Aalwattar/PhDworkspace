/*************************************************************************/
// Author  : Ziad Abuowaimer
// Version : 0.1
// Project : OfflineScheduler - A DFG off-line Task scheduler for FPGA
// Team    : Ziad Abuowaimer
/*************************************************************************/

/*************************************************************************/
// Filename: README
// Description:
// README file
/*************************************************************************/

Usage:
offlinescheduler <options>
<options>: -aif <aif file>
	   -res_file <resource file>

Example: 
offlinescheduler -aif input/iir.aif -res_file input/res.dat 
The command shown above takes “./input/iir.aif” as the input aif file and “./input/res.dat” 
as the resource file.

Compilation:
Gcc was used to compile the source c codes. 

To create the executable, "offlinescheduler", run: 
 $> make 

To clean up the object files and the executable: 
 $> make clean 

To force compilation of all the files and creating the executable, "offlinescheduler", run: 
 $> make rebuild 

Compile with -D__DEBUG option for enabling debugging jigs in the code. 
 $> make rebuild DEBUG=-D__DEBUG


List of header files:
	bounds.h
	ecodes.h
	functions.h
	io.h
	types.h
	ilp.h
	napoleon.h
	main.h

List of source files:
	functions.c
	io.c
	ilp.c
	napoleon.c
	main.c


Directory Structure
-------------------

header  	= all header files and the configuration file (.h files)
input		= all input files (architecture libraries, DFGs and prr files)
Makefile  	= Builds and cleans Napoleon as an independant program
makefile  	= Builds and cleans Napoleon as a shared library
nbproject	= A configuration folder for Netbeans Projects (my IDE)
obj		= where the object files are stored to create the library 
README.md	= All information regarding Napoelon **** INCOMPLETE ****
src		= source code (.c files)

