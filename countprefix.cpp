#include <fstream>
#include <string.h>
#include <iostream>
#include <queue>
#include "shared.h"
#include "countprefix.h"
#include "populatetree.h"
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <mutex>

using namespace std;

void *countprefix(void *ptr){
    SHARED_DATA *shared = (SHARED_DATA *)ptr;
    while(shared -> taskCompleted[DICTSRCFILEINDEX] == false){}//wait for populate tree to be done
    ofstream outputFile;
    outputFile.open("countprefix_output.txt");
    int count = 0;//counter for words
    string line;
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";
    while((!shared -> prefixQueue.empty()) || (shared -> taskCompleted[TESTFILEINDEX] == false)){//if queue is empty and task is completed, then we know we are done with count prefix
        if(!shared -> prefixQueue.empty()){//if queue is not empty then enter, if it is, continue to wait in the while loop until queue is filled
            string first = shared -> prefixQueue.front();//look at first word in queue
            char temp[first.length() + 1];
            strcpy(temp, first.c_str());
            dictNode* wordCheck = findEndingNodeOfAStr(temp, shared);//send to find endingNodeOfAStr
            if(wordCheck != nullptr){//if not null, we can check count
                countWordsStartingFromANode(count, wordCheck);
            }
            if(count >= shared -> minNumOfWordsWithAPrefixForPrinting){//checks if word appears minimum number of times, if it does then output it to the file
                outputFile << temp << " " << count << "\n";
            }
            count = 0;
            pthread_mutex_lock(&(shared -> queue_mutex));//lock thread
            shared -> numOfProcessedPrefixes++;//update numOfProcessedPrefixes
            shared -> prefixQueue.pop();//pop from queue
            pthread_mutex_unlock(&(shared -> queue_mutex));//unlock thread
        }else{}
    }
    outputFile.close();
    pthread_exit(0);
}

dictNode* findEndingNodeOfAStr(const char *strBeingSearched, SHARED_DATA *shared){
    dictNode* current = shared -> root;//sets current node at root
    int length = strlen(strBeingSearched);
    for(int i = 0; i < length; i++){
        int index;
        if((strBeingSearched[i] >= 97) && (strBeingSearched[i] <= 122)){//checks for a-z
            index = strBeingSearched[i] - 97;
        }else if((strBeingSearched[i] >= 65) && (strBeingSearched[i] <= 90)){//checks for A-Z
            index = strBeingSearched[i] - 65;
        }else if(strBeingSearched[i] == 39){//checks for  '
            index = 26;
        }else if(strBeingSearched[i] == 45){//checks for -
            index = 27;
        }else if(strBeingSearched[i] == 95){//checks for _
            index = 28;
        }

        if(current -> next[index] == nullptr){//returns null if pointer is null, else, current is set to next
            return nullptr;
        }else{
            current = current -> next[index];
        }
    }
    
    if(current != nullptr){//check if last char is null, if not return current
            return current;
        }

    return nullptr;
}

void countWordsStartingFromANode(int &count, dictNode* temp){
    dictNode* current = temp;
    if(current == nullptr){//if currentnode is null, we can just return
        return;
    }

    if(current -> next[29] != nullptr){//if the current node is the endChar, we know we have reached the end and can increase the count
        count++;
    }

    for(int i = 0; i < NCHILD; i++){
        if(current -> next[i]){
            countWordsStartingFromANode(count, current -> next[i]);//recursive method to run down the string
        }
   }
}