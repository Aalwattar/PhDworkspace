###############################################################################
# 	Makefile
#
# An off-line Task Scheduler for FPGA
# 
# AUTHOR : Ziad Abuowaimer & Jennifer Winer
#
# CREATED : May 21, 2013
# LAST MODIFIED : May 22, 2013
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
#DEBUG			= -DDEBUG
CC 			= gcc
C_FLAGS 		= -Wall -std=c99 -pedantic -g -O2 $(DEBUG)
C_INCLUDES   		= -Iinclude/Napoleon -Iinclude/GA
L_INCLUDES		= -lm

#debugger options
DBGR 			= gdb
DBG_OPTS 		= 

#directory names
SRC_DIR			= src
OBJ_DIR			= obj
BIN_DIR			= .

NAPOLEON_DIR		= Napoleon
GA_DIR			= GA

OBJS			= $(addprefix $(OBJ_DIR)/, \
                          $(NAPOLEON_DIR)/functions.o \
                          $(NAPOLEON_DIR)/io.o \
                          $(NAPOLEON_DIR)/ilp.o \
                          $(NAPOLEON_DIR)/napoleon.o \
                          $(NAPOLEON_DIR)/main.o \
                          \
                          $(GA_DIR)/fitness.o \
                          $(GA_DIR)/individual.o \
                          $(GA_DIR)/main.o \
                          $(GA_DIR)/population.o \
                          $(GA_DIR)/replacement.o \
                          $(GA_DIR)/selection.o \
			  $(GA_DIR)/util.o)


PROG_NAME		= OfflineScheduler.exe

###############################################################################
#      compilation, linking and debugging targets
###############################################################################


all : $(PROG_NAME)
	@ echo "Build updated"
	
	
.PHONY : $(PROG_NAME) 

# FIX -  make the following targets more descriptive
$(PROG_NAME) : $(OBJS)
	$(CC) $(OBJS) $(L_INCLUDES) -o $(PROG_NAME)


$(OBJ_DIR)/$(NAPOLEON_DIR)/%.o: $(SRC_DIR)/$(NAPOLEON_DIR)/%.c $(OBJ_DIR) 
	$(CC) $(C_INCLUDES) $(C_FLAGS) -c $< -o $@
	
$(OBJ_DIR)/$(GA_DIR)/%.o: $(SRC_DIR)/$(GA_DIR)/%.c $(OBJ_DIR) 
	$(CC) $(C_INCLUDES) $(C_FLAGS) -c $< -o $@
    
$(OBJ_DIR) : 
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/GA
	mkdir -p $(OBJ_DIR)/Napoleon

clean : 
	rm -f $(BIN_DIR)/$(PROG_NAME)
	rm -f $(OBJ_DIR)/$(NAPOLEON_DIR)/*
	rm -f $(OBJ_DIR)/$(GA_DIR)/*



run: 
	./$(PROG_NAME)
			
debug :
	$(DBGR) $(DBG_OPTS) $(PROG_NAME)
