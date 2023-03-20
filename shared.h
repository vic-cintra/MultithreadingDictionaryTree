#ifndef SHARED_H
#define SHARED_H
#include <pthread.h>
#include <iostream>
#include <ctype.h>
#include <queue>
#include <string>
#include <fstream>

#define NUMOFFILES 2 
#define DICTSRCFILEINDEX 0 
#define TESTFILEINDEX 1  
/* default number of progress marks for representing 100% progress */ 
#define DEFAULT_NUMOF_MARKS 50 
/* minimum number of progress marks for representing 100% progress */ 
#define MIN_NUMOF_MARKS 10
/* place hash marks in the progress bar every N marks */ 
#define DEFAULT_HASHMARKINTERVAL  10 
/* default minimum number of dictionary words starting from a prefix for printing or writing to 
the output */ 
#define DEFAULT_MINNUM_OFWORDS_WITHAPREFIX  1 
/* Common data shared between threads */ 
#define endChar '\0'//signals end of sentence or node
#define NCHILD 30//size of nodes (a-z, A-Z, -, _, *, and /0)

using namespace std;

typedef struct dictNode{//Will initialize next root to have 30 children
    struct dictNode *next[NCHILD];
};

typedef struct SHARED_DATA{ 
  dictNode *root;//root of node
  int numOfProgressMarks;//number of marks needed per progress bar
  int hashmarkInterval;//nuber of hashmarks placed at every interval
  int minNumOfWordsWithAPrefixForPrinting;//checks the minimum acceptable amount of words that must occur to be printed
  const char *filePath[NUMOFFILES];//dictates filepath based off arguments
  long  totalNumOfCharsInFile[NUMOFFILES];//takes number of characters in the file
  long  numOfCharsReadFromFile[NUMOFFILES];//count and updates number of characters read from file
  long  wordCountInFile[NUMOFFILES];//will count and update number of words in the file
  long numOfProcessedPrefixes;//number of prefixes that have been proceessed
  std::queue <std::string>prefixQueue;//a queue of prefixes that are currently being processed
  pthread_mutex_t queue_mutex;//a mutex that will lock the queue to avoid thread race conditions
  bool taskCompleted[NUMOFFILES];//boolean to tell when threads are to start
};

#endif