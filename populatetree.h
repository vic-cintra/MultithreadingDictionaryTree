#ifndef POPULATETREE_H
#define POPULATETREE_H
#include "shared.h"

void* populatetree(void *);//method to populate tree
bool add(const char *, SHARED_DATA *);//adds node to tree
dictNode* children();//creates node children

#endif