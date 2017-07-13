#include <time.h>
#include <stdlib.h>


int cmp(const void *a, const void *b) {
	return *(int*)a - *(int*)b;
}

int* generateArray(const int size) {
	int *randomArray = (int*)malloc(sizeof(int)*size);
	for (int i = 0; i < size; ++i) {
		randomArray[i] = rand();
	}

	return randomArray;
}

int test(void(*sort)(int*, int, int)) {
	short isCorrect = 0;
	srand(time(NULL));
	for (int i = 0; i < 10; ++i) {
		int arraySize = rand() % 100;
		int *correctArray = generateArray(arraySize);
		int *testArray = (int*)malloc(sizeof(int)*arraySize);
		for (int i = 0; i < arraySize; ++i) {
			testArray[i] = correctArray[i];
		}
		qsort(correctArray, arraySize, sizeof(correctArray[0]), cmp);
		sort(testArray, 0, arraySize-1);
		for (int i = 0; i < arraySize; ++i) {
			if (correctArray[i] != testArray[i]) {
				printf("Not equal:%d %d\n", correctArray[i], testArray[i]);
				isCorrect = 1;
			}
		}
		free(testArray);
		free(correctArray);
	}

	return isCorrect;
}

