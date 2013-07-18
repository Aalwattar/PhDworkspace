###############################################################################
## Author  : Ziad Abuowaimer
## Version : 0.1
## Project : An off-line Task Scheduler for FPGA
## Team    : Ziad Abuowaimer
###############################################################################

###############################################################################
##
## Description:
## This makefile builds the Off-Line Scheduler
##
## targets:
##	1. <default>    : compile incrementally and link
##	2. rebuild	: compile all and link
##
##	Set DEBUG="-g -D__DEBUG" to compile the source files for debugging
##	and enable the test jigs in the code.
##
##
##
###############################################################################

#compiler options
DEBUG			= 
C_COMP 			= gcc
COMP_OPTS 		= -c -fPIC -O3 -MMD -MP -Iheader -I../../include -lcommonInterfaces -lconfuse
OBJ_OPTS		= -o
LINK_OPTS 		= -shared -Wl,-soname=libofflineScheduler.so  -o "libofflineScheduler.so" 
LINK_OPTS_END		= -lm -L../../lib -lcommonInterfaces -lconfuse

#debugger options
DBGR 			= gdb
DBG_OPTS 		= 

#file extensions
SOURCE_LOCN		= ./source
HEADER_LOCN		= ./header
OBJECT_LOCN		= ./object
SOURCE_EXTN		= .c
OBJECT_EXTN		= .o
EXEC_EXTN		= 

HEADER_FILES_IO		= ${HEADER_LOCN}/*.h
HEADER_FILES_MAIN	= ${HEADER_LOCN}/*.h
HEADER_FILES_FUNCTIONS	= ${HEADER_LOCN}/*.h
HEADER_FILES_ILP	= ${HEADER_LOCN}/*.h
HEADER_FILES_NAPOLEON	= ${HEADER_LOCN}/*.h

OBJECT_FILES		= \
			${OBJECT_LOCN}/functions${OBJECT_EXTN} \
			${OBJECT_LOCN}/io${OBJECT_EXTN} \
			${OBJECT_LOCN}/ilp${OBJECT_EXTN} \
			${OBJECT_LOCN}/napoleon${OBJECT_EXTN} \
			${OBJECT_LOCN}/offlineScheduler${OBJECT_EXTN}

BUILD = offlineScheduler.so

#compilation, linking and debugging targets
all: ${BUILD}
	@ echo "Build updated"

offlineScheduler.so: ${OBJECT_FILES}
	${C_COMP}  ${LINK_OPTS} ${OBJECT_FILES}  ${LINK_OPTS_END}
	cp libofflineScheduler.so ../../lib
	cp header/offlineScheduler.h ../../include

debug:
	${DBGR} ${DBG_OPTS} offlinescheduler${EXEC_EXTN}

clean:
	rm -rf ${OBJECT_LOCN}/*
	rm -rf libofflineScheduler.so
	rm -rf outScheduler.txt ilp_equations.lp
	

${OBJECT_LOCN}/functions${OBJECT_EXTN}: \
	${SOURCE_LOCN}/functions${SOURCE_EXTN} ${HEADER_FILES_FUNCTIONS}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/functions${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/functions${OBJECT_EXTN}

${OBJECT_LOCN}/io${OBJECT_EXTN}: \
	${SOURCE_LOCN}/io${SOURCE_EXTN} ${HEADER_FILES_IO}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/io${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/io${OBJECT_EXTN}

${OBJECT_LOCN}/ilp${OBJECT_EXTN}: \
	${SOURCE_LOCN}/ilp${SOURCE_EXTN} ${HEADER_FILES_ILP}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/ilp${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/ilp${OBJECT_EXTN}

${OBJECT_LOCN}/napoleon${OBJECT_EXTN}: \
	${SOURCE_LOCN}/napoleon${SOURCE_EXTN} ${HEADER_FILES_NAPOLEON}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/napoleon${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/napoleon${OBJECT_EXTN}

${OBJECT_LOCN}/offlineScheduler${OBJECT_EXTN}: \
	${SOURCE_LOCN}/offlineScheduler${SOURCE_EXTN} ${HEADER_FILES_MAIN}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/offlineScheduler${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/offlineScheduler${OBJECT_EXTN}

rebuild:
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/functions${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/functions${OBJECT_EXTN}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/io${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/io${OBJECT_EXTN}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/ilp${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/ilp${OBJECT_EXTN}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/napoleon${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/napoleon${OBJECT_EXTN}
	${C_COMP} ${COMP_OPTS} ${SOURCE_LOCN}/offlineScheduler${SOURCE_EXTN} \
		${OBJ_OPTS} ${OBJECT_LOCN}/offlineScheduler${OBJECT_EXTN}
	${C_COMP} ${OBJECT_FILES} ${LINK_OPTS} offlinescheduler${EXEC_EXTN}

