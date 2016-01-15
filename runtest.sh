#!/bin/bash

##
# XSB install
# cd XSB/build
# ./configure --with-aout
# export CC="gcc -lpthread"
# ./makexsb --config-tag=aout
##

##
# XSB
##
export XSB_HOME=/home/stbadmin/repository-manager/XSB/bin
export PATH=$XSB_HOME:$PATH

HAVE_XSB=`which xsb`

echo "HAVE_XSB: $HAVE_XSB"
echo

##
# ANTLR
##
export ANTLR_HOME=/home/stbadmin/repository-manager/antlr/libantlr3c-3.4
export ANTLR_LIB=$ANTLR_HOME/lib
export ANTLR_INC=$ANTLR_HOME/include

##
# Executable
##
export BIN=./bin/RepositoryManager
export MAIN=./src/manager/RepositoryManager.cpp

##
# Path enviornment var
##
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ANTLR_LIB

##
# Lib/Include/Compiler Flags
##
export MLIBS="-lcrypto -lantlr3c -lrepoparserlexer"
export MCFLAGS="-g -Wall -Wextra -Wno-write-strings -lpthread"
export INCLUDE="-I$ANTLR_HOME -I$ANTLR_INC"

echo "Includes:"
echo $INCLUDE
echo

export LIBS="-L$ANTLR_LIB"

echo "Link Libraries:"
echo $LIBS
echo

export BUILD="g++ $MCFLAGS $INCLUDE $LIBS"
export BUILD="$BUILD $MAIN -o $BIN $MLIBS"

BIN_EXISTS=0
if [ -e $BIN ]; then
  BIN_EXISTS=1;
fi

echo "Enter 'r' to run, 'd' to debug, 'c' to compile, or 'q' to exit"

read selection;

if [ "$selection" = "q" ]; then
  echo "Exiting"
  echo
else
  if [ "$selection" = "1" ]; then
    valgrind -v --log-file=valgrind.log --leak-check=full --track-origins=yes --show-reachable=yes $BIN 1983 policy/server_policy2.P
  elif [ "$selection" = "c" ]; then
    if [ "$BIN_EXISTS" = "1" ]; then
      rm $BIN
    fi
    echo
    echo "Compiling With:"
    echo
    echo $BUILD
    echo
    $BUILD
    if [ -e $BIN ]; then
      BIN_EXISTS=1;
    else
      echo "Build failed, exiting..."
    fi
  elif [ "$selection" = "r" -o "$selection" = d ]; then
    if [ "$BIN_EXISTS" = "0" ]; then
      echo "Compiling first"
      echo
      $BUILD
      if [ -e $BIN ]; then
        BIN_EXISTS=1;
      else
        echo "Build failed, exiting..."
      fi
    fi
    if [ "$BIN_EXISTS" = "1" ]; then
      if [ "$selection" = "r" ]; then
        echo "Enter the server policy file (absolute or relative path)"
        echo
        read policy
        echo
        if [ -e $policy ]; then
          echo "Enter the server port"
          echo
          read port
          echo
          echo "Enter 1 to validate the duration of a certificate, otherwise enter another key"
          echo
          read validate
          echo
          echo "Executing With Server Policy $policy"
          echo "Listening On Port $port"
          echo
          if [ "$validate" = "1" ]; then
            $BIN $port $policy $validate
          else
            #valgrind -v --log-file=valgrind.log --leak-check=full --track-origins=yes $BIN $port $policy
            #TODO: $BIN $port $policy #./policy/$POLICY ?
            $BIN $port $policy #./policy/$POLICY ?
          fi
        else
          echo "Server policy $policy does not exist, exiting..."
          echo
        fi
      elif [ "$selection" = "d" ]; then
        echo "Debugging"
        echo
        gdb ./$BIN
      fi
    else
      echo "$BIN does not exists"
      echo
    fi
  else
    echo "$selection is an invalid selection, exiting..."
    echo
  fi
fi
exit 0;
