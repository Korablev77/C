#ifndef INSERTION_SORT
#define INSERTION_SORT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//time:   O(n^2)
//memory: O(1)

static int cmpInt(const void*, const void*);

static int cmpFloat(const void*, const void*);

static int cmpChar(const void*, const void*);

void insertionSort(void *array, const int arraySize, const int elemSize, 
	int (*cmp)(const void *, const void *));

#endif