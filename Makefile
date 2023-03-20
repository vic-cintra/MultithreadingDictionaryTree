CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CCFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = populatetree.o readprefix.o countprefix.o main.o 

# Program name
PROGRAM = countprefix

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -pthread -o $(PROGRAM) $(OBJS)

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp

populatetree.o : populatetree.cpp populatetree.h
	$(CC) $(CCFLAGS) populatetree.cpp

readprefix.o: readprefix.cpp readprefix.h
	$(CC) $(CCFLAGS) readprefix.cpp
    
countprefix.o: countprefix.cpp countprefix.h
	$(CC) $(CCFLAGS) countprefix.cpp

clean :
	rm -f *.o *~ $(PROGRAM)


