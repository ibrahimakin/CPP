ifdef file
# empty :=
# slash := \ 
# space := $(empty) $(empty)
# foo := $(wildcard $(file)*.cpp)
# bar := ${subst .cpp,,$(wildcard $(file)*.cpp)}
# $(bar): $(foo)				# 1\ -\ Multiples\ of\ 3\ or\ 5.cpp
# 	g++ $(wildcard $(file)*.cpp) -o $(bar)

#Tell make to make one .out file for each .cpp file found in the current directory
file: $(patsubst %.cpp, %, $(wildcard $(file)*.cpp))

#Rule how to create arbitary .out files. 
#First state what is needed for them e.g. additional headers, .cpp files in an include folder...
#Then the command to create the .out file, probably you want to add further options to the g++ call.
%: %.cpp Makefile
	g++ $< -o $@ -std=c++0x

else

CC=g++
CFLAGS=-c -Wall
SOURCES = $(wildcard *.cpp)
EXECS = $(SOURCES:.cpp=)	# shorthand for $(patsubst pattern,replacement,text)

all: $(EXECS)

clean:
	rm $(EXECS)

endif