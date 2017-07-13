#include "InsertionSort.h"

#define DEBUG


static int cmpInt(const void *a, const void *b) {
	int res = *(int*)a - *(int*)b;
	if (res < 0) return 0;
	return 1;
}

static int cmpFloat(const void *a, const void *b) {
	float res = *(float*)a - *(float*)b;
	if (res < 0) return 0;
	return 1;
}

static int cmpChar(const void *a, const void *b) {
	return 1;
}


//Generic version such as qsort from stdlib
void insertionSort(void *array, const int arraySize, const int elemSize, 
	int (*cmp)(const void *, const void *)) {

	void *currentElem = malloc(elemSize);
	for (int i = 1; i < arraySize; ++i) {
		memcpy(currentElem, (char*)array+elemSize*i, elemSize);
		int j = i - 1;
		while(cmp((char*)array + elemSize*j, currentElem) && j >= 0) {
			memcpy((char*)array + elemSize*(j+1), (char*)array + elemSize*j, elemSize);
			j--;
		}
		memcpy((char*)array+elemSize*(j+1), currentElem, elemSize);
	}
	free(currentElem);
}

#ifdef debug

int main(int argc, char const *argv[]) {
	
	float testArr[] = {5.1, 2.2, 4.5, 6.2, 1.9, 3.53, 2.4};
	insertionSort(testArr, sizeof(testArr)/sizeof(testArr[0]), 
		sizeof(testArr[0]), cmpFloat);

	for (int i = 0; i < 7; ++i) {
		printf("%f ", testArr[i]);
	}
	return 0;
}

#endif