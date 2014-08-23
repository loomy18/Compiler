
CC      =  gcc
CFLAGS  =  -Wall -g -std=gnu11
PFLAGS  =  -lfl
LIBDIR  =  Source/

all: main

main: flex bison support management semantics
	@$(CC) -o Compiler $(CFLAGS) \
	$(LIBDIR)lex.yy.o \
	$(LIBDIR)compiler.tab.o \
	$(LIBDIR)support.o \
	$(LIBDIR)management.o \
	$(LIBDIR)semantics.o \
	$(PFLAGS)

flex: $(LIBDIR)lex.yy.c
	@$(CC) -c -o $(LIBDIR)lex.yy.o $(CFLAGS) $(LIBDIR)lex.yy.c

bison: $(LIBDIR)compiler.tab.c
	@$(CC) -c -o $(LIBDIR)compiler.tab.o $(CFLAGS) $(LIBDIR)compiler.tab.c

support: $(LIBDIR)support.c $(LIBDIR)support.h
	@$(CC) -c -o $(LIBDIR)support.o $(CFLAGS) $(LIBDIR)support.c

management: $(LIBDIR)management.c $(LIBDIR)management.h
	@$(CC) -c -o $(LIBDIR)management.o $(CFLAGS) $(LIBDIR)management.c

semantics: $(LIBDIR)semantics.c $(LIBDIR)semantics.h
	@$(CC) -c -o $(LIBDIR)semantics.o $(CFLAGS) $(LIBDIR)semantics.c

preclean:
	@$(RM) Compiler *.o $(LIBDIR)*.o
	@$(RM) -rf *.dSYM
	@$(RM) -rf $(LIBDIR)*.dSYM
	@$(RM) -rf *.gch
	@$(RM) -rf $(LIBDIR)*.gch
	@$(RM) -rf $(LIBDIR)lex.yy.c
	@$(RM) -rf $(LIBDIR)compiler.tab.h
	@$(RM) -rf $(LIBDIR)compiler.tab.c

postclean:
	@$(RM) *.o $(LIBDIR)*.o
	@$(RM) -rf *.dSYM
	@$(RM) -rf $(LIBDIR)*.dSYM
	@$(RM) -rf *.gch
	@$(RM) -rf $(LIBDIR)*.gch
	@$(RM) -rf $(LIBDIR)lex.yy.c
	@$(RM) -rf $(LIBDIR)compiler.tab.h
	@$(RM) -rf $(LIBDIR)compiler.tab.c
