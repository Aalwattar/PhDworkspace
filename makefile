###############################################################################
# 	Makefile
#
# Genetic Algorithm practice for Ahmed Al-Watter
# 
# AUTHOR : Jennifer Winer
# Email  : jwiner@uoguelph.ca
#
# CREATED : May 7, 2013
# LAST MODIFIED : May 7, 2013
###############################################################################


CC	    = gcc
CFLAGS	    = -Wall -std=c99 -pedantic -g -O0

CINCLUDES   = -Iinclude
#CINCLUDES  = -I/usr/include/python2.6 -I/usr/include/libxml2
LINCLUDES  = -lm

SRCDIR	    = src/
OBJDIR	    = obj/
BINDIR	    = bin/

SOURCE	    = $(SRCDIR)algorithm.c $(SRCDIR)individual.c $(SRCDIR)population.c $(SRCDIR)problem.c 
OBJS	    = $(OBJDIR)algorithm.o $(OBJDIR)individual.o $(OBJDIR)population.o $(OBJDIR)problem.o
PROGNAME    = $(BINDIR)GA.exe
	
#####################################################

default : all

all : prog


prog : link

link : compile
	$(CC) $(OBJS) $(LINCLUDES) -o $(PROGNAME)

compile : $(SOURCE)
	$(CC) $(CINCLUDES) $(CFLAGS) -c $(SOURCE)
	mv *.o $(OBJDIR)



clean :
	rm $(OBJS) $(PROGNAME)
	
	
	
run: 
	$(PROGNAME)

#$(PROGNAME) datafilenamehere