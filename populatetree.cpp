#include "shared.h"
#include "populatetree.h"
#include <cstring>
#include <string.h>
#include <stdio.h>


void *populatetree(void *ptr){
    SHARED_DATA *shared = (SHARED_DATA *) ptr;
    ifstream dictstream(shared->filePath[DICTSRCFILEINDEX]);//first file to read in

    if(!dictstream){//error handling for file
        cout<< "Unable to open <<" << shared -> filePath[DICTSRCFILEINDEX]<< ">>"<<endl;
    }

    string line;
    const char *delimiters = "\n\r !\"#$%&()*+,./0123456789:;<=>?@[\\]^`{|}~";
    while(getline(dictstream, line)){
        shared -> numOfCharsReadFromFile[DICTSRCFILEINDEX] += (line.length() + 1);//counts characters read from file + 1 for end line characters
        char temp[line.length() + 1];
        strcpy(temp, line.c_str());
        char *word = strtok(temp, delimiters);
        while(word != nullptr){
            if(add(word, shared) == false){//if word has not been added, return NULL
                 return NULL;
            }
            word = strtok(NULL, delimiters);//bring up next word in line
            shared -> wordCountInFile[DICTSRCFILEINDEX]++;//increments wordCountInFile
        }
    }
    dictstream.close();
    shared -> taskCompleted[DICTSRCFILEINDEX] = true;//update task to true, allows readprefix and countprefix to begin
    pthread_exit(0);
}

dictNode* children(){//creates node and populates it with null, returning a new node
    dictNode* child = new dictNode();
    for(int i = 0; i < NCHILD; i++){//loops 30 times for the "alphabet", sets to null
        child -> next[i] = NULL;
    }
    return child;
}

bool add(const char * wordBeingInserted, SHARED_DATA *shared){
    int length = strlen(wordBeingInserted);
    dictNode* current = shared -> root;
    for(int i = 0; wordBeingInserted[i] != endChar; i++){
        int index;
        if((wordBeingInserted[i] >= 97) && (wordBeingInserted[i] <= 122)){//checks for a-z
            index = wordBeingInserted[i] - 97;//a-97=0, so a = 0
        }else if((wordBeingInserted[i] >= 65) && (wordBeingInserted[i] <= 90)){//checks for A-Z
            index = wordBeingInserted[i] - 65;
        }else if(wordBeingInserted[i] == 39){//checks for  '
            index = 26;
        }else if(wordBeingInserted[i] == 45){//checks for -
            index = 27;
        }else if(wordBeingInserted[i] == 95){//checks for _
            index = 28;
        }

        if(current -> next[index] == nullptr){//if current is null, creates a new node with children and sets it at new node
            dictNode* newNode = children();
            current -> next[index] = newNode;
            }

        current = current -> next[index];//sets current node at index
        }

        current -> next[29] = children();//checks for end line character and creates new node
        return true;//returns true when word is added to tree
}

