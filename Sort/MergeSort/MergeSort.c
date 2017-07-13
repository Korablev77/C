#include "MergeSort.h"


void mergeSort(int *array, const int leftBorder, const int rightBorder) {
	int middle = (leftBorder + rightBorder)/2;

	if (rightBorder <= leftBorder) return;
	mergeSort(array, leftBorder, middle);
	mergeSort(array, middle+1, rightBorder);
	merge(array, leftBorder, middle, rightBorder);

}

	
void merge(int *array, int leftBorder, int delimiter, int rightBorder) {
	int *helpArray = (int*)malloc(sizeof(int)*(rightBorder-leftBorder+1));
	int leftIndex = leftBorder;
	int rightIndex = delimiter+1;
	for (int i = 0; i < rightBorder-leftBorder+1; ++i) {
		if (leftIndex == delimiter+1) {
			while(rightIndex != (rightBorder+1)) {
				helpArray[i] = array[rightIndex];
				rightIndex++;
				i++;
			}
			break;
		}
		if (rightIndex == rightBorder+1) {
			while(leftIndex != (delimiter+1)) {
				helpArray[i] = array[leftIndex];
				leftIndex++;
				i++;
			}
			break;
		}
		if (array[leftIndex] < array[rightIndex]) {
			helpArray[i] = array[leftIndex];
			leftIndex++;
		}
		else {
			helpArray[i] = array[rightIndex];
			rightIndex++;
		}
	}

	for (int i = leftBorder; i <= rightBorder; ++i) {
		array[i] = helpArray[i-leftBorder];
	}
	free(helpArray);
	helpArray = NULL;
}

#ifdef debug
#include "sortTest.h"

int main(int argc, char const *argv[]) {
	generateArray(10);
	int testArr[] = {5, 2, 4, 6, 1, 3, 9,0, 7};
	if(test(mergeSort) == 1) printf("%s\n", "Tests FAILED");
	else printf("%s\n", "Tests are passed");

	return 0;
}

#endif