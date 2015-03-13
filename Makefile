CC = g++
CFLAGS = -std=c++11

#Flags for libPNG
PNGFLAGS = -lm -lpng

#Points to the root of Google Test
GTEST_DIR = /home/michael/Downloads/gtest-1.7.0

#all file dependencies
FILES = linalg.cpp

RM = /bin/rm -f

all: main

main: clean main.cpp
	$(CC) $(CFLAGS) -o main main.cpp $(FILES) $(PNGFLAGS)

clean: 
	$(RM) *.o main *~ output.png test

check: all
	$(CC) $(CFLAGS) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) $(CFLAGS) -isystem ${GTEST_DIR}/include -pthread test.cpp $(FILES) libgtest.a -o test
	./test



