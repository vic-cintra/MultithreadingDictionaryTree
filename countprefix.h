#ifndef COUNTPREFIX_H
#define COUNTPREFIX_H

void* countprefix(void *);//method to countPrefix
dictNode* findEndingNodeOfAStr(const char *, SHARED_DATA *);//finds ending node of a string
void countWordsStartingFromANode(int &, dictNode*);//updates count of words for prefix

#endif