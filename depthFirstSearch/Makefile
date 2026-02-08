#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 2
#
# make                     makes FindPath
# make FindPath            makes FindPath
# make clean               removes all binaries
#------------------------------------------------------------------------------

MAIN           = FindPath
MAINOBJ        = $(MAIN).o
MAINSRC        = $(MAIN).c
TEST           = GraphTest
TESTOBJ        = $(TEST).o
TESTSRC        = $(TEST).c
ADT            = Graph
ADTOBJ         = Graph.o
ADTSRC         = Graph.c
ADT_H          = Graph.h
LIST           = List
LISTOBJ        = List.o
LISTSRC        = List.c
LIST_H         = List.h
COMPILE        = gcc -std=c17 -Wall -c
LINK           = gcc -std=c17 -Wall -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full
INFILE         = in4
OUTFILE        = myout4


$(MAIN) : $(MAINOBJ) $(ADTOBJ) $(LISTOBJ)
	$(LINK) $(MAIN) $(MAINOBJ) $(ADTOBJ) $(LISTOBJ)

$(MAINOBJ) : $(ADT_H) $(LIST_H) $(MAINSRC)
	$(COMPILE) $(MAINSRC)

$(TEST) : $(TESTOBJ) $(ADTOBJ) $(LISTOBJ)
	$(LINK) $(TEST) $(TESTOBJ) $(ADTOBJ) $(LISTOBJ)

$(TESTOBJ) : $(ADT_H) $(LIST_H) $(TESTSRC)
	$(COMPILE) $(TESTSRC)

$(ADTOBJ) : $(ADT_H) $(LIST_H) $(ADTSRC)
	$(COMPILE) $(ADTSRC)

$(LISTOBJ) : $(LIST_H) $(LISTSRC)
	$(COMPILE) $(LISTSRC)

clean :
	$(REMOVE) $(MAIN) $(MAINOBJ) $(TEST) $(TESTOBJ) $(ADTOBJ) $(LISTOBJ)

check$(MAIN) : $(MAIN)
	$(MEMCHECK) $(MAIN) $(INFILE) $(OUTFILE)

check$(TEST) : $(TEST)
	$(MEMCHECK) $(TEST)
