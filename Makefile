# repository-manager.mk

LIBD=/home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/

XSB_HOME=/home/stbadmin/repository-manager/XSB/bin

ANTLR_HOME=/home/stbadmin/repository-manager/antlr/libantlr3c-3.4
ANTLR_LIB=$(ANTLR_HOME)/lib
ANTLR_INC=$(ANTLR_HOME)/include

MLIBS=-lcrypto -lantlr3c -lrepoparserlexer
MCFLAGS=-g -Wall -Wextra -Wno-write-strings -lpthread
INCLUDE=-I$(ANTLR_HOME) -I$(ANTLR_INC)
LIBS=-L$(ANTLR_LIB)
BIN=./bin/RepositoryManager
MAIN=./src/manager/RepositoryManager.cpp

all:
	g++ $(MCFLAGS) $(INCLUDE) $(LIBS) $(MAIN) -o $(BIN) $(MLIBS)

parser:
	rm src/parser/*.o
	rm src/parser/librepoparserlexer.so.1.0
	g++ -g -Wall -fPIC -I$(ANTLR_HOME)/include -I$(ANTLR_HOME) -L$(LIBD) -lantlr3c -c *.c
	g++ -shared -Wl,-soname,librepoparserlexer.so.1 -o librepoparserlexer.so.1.0 *.o
	cp librepoparserlexer.so.1.0 /home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/;
	ln -sf $(LIBD)librepoparserlexer.so.1.0 $(LIBD)librepoparserlexer.so.1
	ln -sf $(LIBD)librepoparserlexer.so.1.0 ln -sf $(LIBD)librepoparserlexer.so

parser_clean:

clean:
	rm rlf_tmp.*

clean_all: parser_clean clean
	rm bin/RepositoryManager

