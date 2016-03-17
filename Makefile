# repository-manager.mk

XSB_HOME=/home/stbadmin/repository-manager/XSB/bin

ANTLR_HOME=./antlr/libantlr3c-3.4
ANTLR_LIB=$(ANTLR_HOME)/lib
ANTLR_INC=$(ANTLR_HOME)/include

LDFLAGS=-L./src/parser -L$(ANTLR_LIB) -lrepoparserlexer
INCLUDE=-I$(ANTLR_HOME) -I$(ANTLR_INC)
LIBS=-L$(ANTLR_LIB)
BIN=./bin/RepositoryManager
PARSER=./src/parser
MAIN=./src/manager/RepositoryManager.cpp

all: manager

manager: parser
	g++ $(MCFLAGS) $(INCLUDE) $(LIBS) $(MAIN) -o $(BIN) $(LDFLAGS) -lpthread -lcrypto -lantlr3c

parser:
	cd $(PARSER) && $(MAKE)
	cd ../../

parser_clean:
	cd $(PARSER) && $(MAKE) clean
	cd ../../

clean:
	if [ -e rlf_tmp.P ] ; then \
		rm rlf_tmp.P ; \
	fi; \
	if [ -e rlf_tmp.xwam ] ; then \
		rm rlf_tmp.xwam ; \
	fi; \
	if [ -e $(BIN) ]; then \
		rm $(BIN) ; \
	fi;

clean_all: clean parser_clean
