###############################################################################
# 	Makefile
#
# Genetic Algorithm for Ahmed Al-Watter
# 
# AUTHOR : Jennifer Winer
# Email  : jwiner@uoguelph.ca
#
# CREATED : May 7, 2013
# LAST MODIFIED : May 17, 2013
###############################################################################


CC	    = gcc
CFLAGS	    = -Wall -std=c99 -pedantic -g -O0

CINCLUDES   = -Iinclude
#CINCLUDES  = -I/usr/include/python2.6 -I/usr/include/libxml2
LINCLUDES  = -lm

SRCDIR	    = src/
OBJDIR	    = obj/
BINDIR	    = bin/

SOURCE	    = $(SRCDIR)* 
OBJS	    = $(OBJDIR)*
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
	@rm $(OBJS) $(PROGNAME) *.stackdump

	
run: 
	$(PROGNAME) -ps=16 -mr=0.001 -cr=0.85
