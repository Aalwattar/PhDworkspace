###############################################################################
# 	Makefile
#
# An off-line Task Scheduler for FPGA
# 
# AUTHOR : Ziad Abuowaimer & Jennifer Winer
#
# CREATED : May 21, 2013
# LAST MODIFIED : May 21, 2013
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
###############################################################################

#compiler options
DEBUG			= 
CC 			= gcc
C_FLAGS 		= -Wall -std=c99 -pedantic -g -O0 $(DEBUG)
C_INCLUDES   		= -I$(INC_DIR)
L_INCLUDES		= -lm

#debugger options
DBGR 			= gdb
DBG_OPTS 		= 

#directory names
SRC_DIR			= src
INC_DIR			= include
OBJ_DIR			= obj
BIN_DIR			= .

GA_DIR       		= GA
NAPOLEON_DIR 		= Napoleon

OBJS			=  $(GA_DIR)/$(OBJ_DIR)/* \
			   $(NAPOLEON_DIR)/$(OBJ_DIR)/*

PROG_NAME		= OfflineScheduler$(BIN_EXTN)

###############################################################################
#      compilation, linking and debugging targets
###############################################################################


all : $(PROG_NAME)
	@ echo "Build updated"
	
	
.PHONY : $(PROG_NAME) 

# FIX -  make the following targets more descriptive
$(PROG_NAME) : 
	cd $(GA_DIR); make build 
	cd $(NAPOLEON_DIR); make build
	$(CC) $(OBJS) $(L_INCLUDES) -o $(PROG_NAME)

debug :
	$(DBGR) $(DBG_OPTS) $(PROG_NAME)

clean : 
	rm -f $(BIN_DIR)/$(PROG_NAME)
	rm -f $(GA_DIR)/$(OBJ_DIR)/*
	rm -f $(NAPOLEON_DIR)/$(OBJ_DIR)/*

