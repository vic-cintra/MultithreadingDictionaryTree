#include "shared.h"
#include "readprefix.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string.h>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <mutex>

using namespace std;

void *readprefix(void *ptr){
    SHARED_DATA *shared = (SHARED_DATA *)ptr;
    while(shared -> taskCompleted[DICTSRCFILEINDEX] == false){}//do not begin until populatetree is done
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";
    ifstream dictstream(shared -> filePath[TESTFILEINDEX]);//open second file

    if(!dictstream){//file error handling
        cout<< "Unable to open <<" << shared -> filePath[TESTFILEINDEX]<< ">>"<<endl;
    }

    string line;
    while(getline(dictstream, line)){
        char temp[line.length() + 1];
        strcpy(temp, line.c_str());
        char *token = strtok(temp, delimiters);
        while(token != nullptr){
            string token_s = token;
            pthread_mutex_lock(&(shared -> queue_mutex));//lock thread so countprefix cannot affect the queue
            shared -> wordCountInFile[TESTFILEINDEX]++;//increment wordcount in testFileIndex
            shared -> prefixQueue.push(token_s);//push word onto queue
            pthread_mutex_unlock(&(shared -> queue_mutex));//unlock thread
            token = strtok(NULL, delimiters);//get next word
        }
    }
    dictstream.close();
    shared -> taskCompleted[TESTFILEINDEX] = true;//update task so prefix progress bar can begin
    pthread_exit(0);
}