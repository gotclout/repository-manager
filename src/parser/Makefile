# parser.mk

ANTLR_HOME=../../antlr/libantlr3c-3.4
ANTLR_LIB=$(ANTLR_HOME)/lib
ANTLR_INC=$(ANTLR_HOME)/include

all:
	gcc -g -Wall -O2 -fPIC -I$(ANTLR_INC) -I$(ANTLR_HOME) -c *.c
	g++ -shared -Wl,-soname,librepoparserlexer.so.1 -L$(ANTLR_LIB) -lantlr3c -o librepoparserlexer.so.1.0 *.o
	cp librepoparserlexer.so.1.0 $(ANTLR_LIB)
	ln -sf $(ANTLR_LIB)/librepoparserlexer.so.1.0 $(ANTLR_LIB)/librepoparserlexer.so.1
	ln -sf $(ANTLR_LIB)/librepoparserlexer.so.1.0 $(ANTLR_LIB)/librepoparserlexer.so

clean:
	if [ -e RepositoryManagerLexer.o ] ; then \
		rm RepositoryManagerLexer.o ; \
	fi; \
	if [ -e RepositoryManagerParser.o ] ; then \
		rm RepositoryManagerParser.o ; \
	fi; \
	if [ -e librepoparserlexer.so.1.0 ] ; then \
		rm librepoparserlexer.so.1.0 ; \
	fi;
