# parser.mk

ANTLR_HOME=/home/stbadmin/repository-manager/antlr/libantlr3c-3.4
ANTLR_LIB=$(ANTLR_HOME)/lib
ANTLR_INC=$(ANTLR_HOME)/include

all:
	g++ -g -Wall -fPIC -I$(ANTLR_INC) -I$(ANTLR_HOME) -L$(ANTLR_LIB) -lantlr3c -c *.c
	g++ -shared -Wl,-soname,librepoparserlexer.so.1 -o librepoparserlexer.so.1.0 *.o
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
	fi; \
