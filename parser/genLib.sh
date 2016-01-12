#!/bin/bash

rm *.o;

rm librepoparserlexer.so.1.0;

#g++ -g -Wall -fPIC -I/home/stbadmin/repository-manager/antlr/libantlr3c-3.4/include -L/home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib -lantlr3c -c *.c;

g++ -g -Wall -fPIC -I/home/stbadmin/repository-manager/antlr/libantlr3c-3.4/include -I/home/stbadmin/repository-manager/antlr/libantlr3c-3.4 -L/home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib -lantlr3c -c *.c;

g++ -shared -Wl,-soname,librepoparserlexer.so.1 -o librepoparserlexer.so.1.0 *.o;

mkdir /home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/

cp librepoparserlexer.so.1.0 /home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/;

ln -sf /home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/librepoparserlexer.so.1.0 /home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/librepoparserlexer.so.1;

ln -sf /home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/librepoparserlexer.so.1.0 /home/stbadmin/repository-manager/antlr/libantlr3c-3.4/lib/librepoparserlexer.so;

exit 0;
