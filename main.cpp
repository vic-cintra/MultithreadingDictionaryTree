#include <stdio.h>
#include <pthread.h>
#include <string>
#include <fstream>
#include <iostream>
#include "shared.h"
#include <unistd.h>
#include "populatetree.h"
#include "readprefix.h"
#include "countprefix.h"
#include <queue>
#include <sys/stat.h>
#include <mutex>


using namespace std;

void* populateTreeProgress(void *ptr, int numOfMarks, int hashMark){//progress bar to load populate tree
    SHARED_DATA *shared = (SHARED_DATA *) ptr;
    double numCharsPerMark = shared -> totalNumOfCharsInFile[DICTSRCFILEINDEX] / numOfMarks;//takes number of characters in the file and divides it by the number of marks needed, this provides a ratio of how many characters per mark
    for(int i = 0; i < numOfMarks; i++){//keeps track of which mark to print
        while(((double)shared -> numOfCharsReadFromFile[DICTSRCFILEINDEX]) < (numCharsPerMark * (double)(i + 1)));//while the number of characters read is less than the number of characters per the current mark, wait
        if((i + 1) % hashMark == 0){//Checks to see if the hashmark interval has been reached yet
            cout << "#" << flush;
        }else{//else print normal mark
            cout << "-" << flush;
        }
    }
    while (shared -> taskCompleted[DICTSRCFILEINDEX] == false){}//ensures that populatetree.cpp is completed
    cout << "\n" << "There are " << shared -> wordCountInFile[DICTSRCFILEINDEX] << " words in vocabulary.txt" << endl;
}

void* countPrefixProgress(void *ptr, int numOfMarks, int hashMark){//progress bar to load populate tree
    SHARED_DATA *shared = (SHARED_DATA *) ptr;
    while(shared -> taskCompleted[TESTFILEINDEX] == false){}//stalls progress bar loader until readprefix.cpp is complete
    double numWordsPerMark = shared -> wordCountInFile[TESTFILEINDEX] / numOfMarks;//takes number of words in the file and divides it by the number of marks needed, this provides a ratio of how many words per mark
    for(int i = 0; i < numOfMarks; i++){//keeps track of which mark to print
        while(((double)shared -> numOfProcessedPrefixes) < (numWordsPerMark * (double)(i + 1)));//while the number of words read is less than the number of words per the current mark, wait
        if((i + 1) % hashMark == 0){//Checks to see if the hashmark interval has been reached yet
            cout << "#" << flush;
        }else{//else print normal mark
            cout << "-" << flush;
        }
    }
    cout << "\n" << "There are " << shared -> wordCountInFile[TESTFILEINDEX] << " words in testfile1.txt" << endl;//prints message at end of countPrefix progress bar
}

int main(int argc, char **argv){

    SHARED_DATA sharedData;
    sharedData.root = children();
    sharedData.numOfProgressMarks = DEFAULT_NUMOF_MARKS;
    sharedData.hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
    sharedData.minNumOfWordsWithAPrefixForPrinting = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX;
    sharedData.filePath[DICTSRCFILEINDEX] = argv[1];
    sharedData.filePath[TESTFILEINDEX] = argv[2];
    sharedData.prefixQueue = queue<string>();
    sharedData.queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    sharedData.taskCompleted[DICTSRCFILEINDEX] = false;
    sharedData.taskCompleted[TESTFILEINDEX] = false;
    sharedData.wordCountInFile[TESTFILEINDEX] = 0;
    sharedData.wordCountInFile[DICTSRCFILEINDEX] = 0;
    sharedData.numOfCharsReadFromFile[DICTSRCFILEINDEX] = 0;
    sharedData.numOfProcessedPrefixes = 0;
    int option;

    struct stat vocabSize;
    struct stat testSize;

    if((stat(sharedData.filePath[DICTSRCFILEINDEX], &vocabSize) == 0)){
        sharedData.totalNumOfCharsInFile[DICTSRCFILEINDEX] = vocabSize.st_size;//uses stat to return total characters in a file to be used in the equation
    }

    while ((option = getopt(argc, argv, "p:h:n:")) != -1) {//arguments for main
        switch (option) {
            case 'p'://sets number of progress marks
                sharedData.numOfProgressMarks = atoi(optarg);  
            break;
            case 'h'://sets hashmark interval
                sharedData.hashmarkInterval = atoi(optarg);
            break;
            case 'n'://sets prefix minimum
                sharedData.minNumOfWordsWithAPrefixForPrinting = atoi(optarg);
            break;
        default:
            break;
        }
    }

    if(sharedData.numOfProgressMarks < 10){//throws error if progress mark is less than 10
        cout << "Number of progress marks must be a number and at least 10" << endl;
        exit(1);
    }

    if((sharedData.hashmarkInterval <= 0) || (sharedData.hashmarkInterval > 10)){//throws error if hashmark interval is not between 0 and 10
        cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10"  << endl;
        exit(1);
    }


    pthread_t populateTreeThread;
    pthread_t readPrefixThread;
    pthread_t countPrefixThread;
    //create threads and begin
    pthread_create(&populateTreeThread, NULL, &populatetree, &sharedData);
    pthread_create(&readPrefixThread, NULL, &readprefix, &sharedData);
    pthread_create(&countPrefixThread, NULL, &countprefix, &sharedData);
    populateTreeProgress(&sharedData, sharedData.numOfProgressMarks, sharedData.hashmarkInterval);
    countPrefixProgress(&sharedData, sharedData.numOfProgressMarks, sharedData.hashmarkInterval);

    pthread_join(readPrefixThread, NULL);
    pthread_join(countPrefixThread, NULL);
    return(0);
}

//./countprefix vocabulary.txt testfile1.txt -p 40 -h 5 -n 3
//./countprefix vocabulary.txt testfile1.txt