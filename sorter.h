#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


//Global variables
int cols = 0;		//stores number of columns
int rows = 0;		//stores number of rows
int mallocSize = 80000;	//free this amount when done

/* STRUCT PURPOSE: This struct holds a pointer
 * to point to a custom created dynamicaly allocated
 * array. The benefit of this is to associate all
 * entries in array row with each other when merge
 * sorting. 
 */
typedef struct _group{
	int blank;
	char* metadata[];
} group;

//Define functions
char* stripSpace(char* string);
char determineType(int target, group** rowArr);
void mergeSort(int target, char det, group** rowArr, int low, int high);
void merge(int target, char det, group** a, int low, int high, int mid);
void printCSV(group** rowArr);
int checkComma(char* string);

//Write functions
void mergeSort(int target, char det, group** rowArr, int low, int high) {

   if (low < high) {
      int mid = (low + high) / 2;

      mergeSort(target, det, rowArr, low, mid);
      mergeSort(target, det, rowArr, mid+1, high);

      merge(target, det, rowArr, low, high, mid);
   }

}

void merge(int target, char det, group** arr, int low, int high, int mid) {
   int left = low, right = mid+1, i;

   group** sorted = malloc( rows * sizeof(group*) );

   for(i = low; left <= mid && right <= high; i++) {
      
		if (det == 'd') {
	    	
	    	if( atof(arr[left]->metadata[target]) <= atof(arr[right]->metadata[target]) )
	        	sorted[i] = arr[left++];
			else
	         sorted[i] = arr[right++];

	 	}

	 	else if (det == 's') {
	    	
	    	//Set variables for easier reading
	    	char* s1 = arr[left]->metadata[target];
	    	char* s2 = arr[right]->metadata[target];
	    	int cmp = strcmp(s1, s2);

	    	if(cmp <= 0)
	        	sorted[i] = arr[left++];
			else
	        	sorted[i] = arr[right++];	 

	 	}

	}
	
  	while (left <= mid)    
      sorted[i++] = arr[left++];

   	while (right <= high)   
      sorted[i++] = arr[right++];

   	for (i = low; i <= high; i++)
      arr[i] = sorted[i];

}

//Insert a column, and determine the column based some samples
char determineType(int target, group** rowArr) {

	int i = 0, j = 0;
	int strCounter = 0, numCounter = 0;
	int length, strings = 0, numbers = 0;
	char* test;

	/*
	 * To prevent crashing into columsn with numbers as
	 * movie titles, we will take multiple samples from
	 * the column, and then determine based on chances if
	 * the column is a string or a number.
	 */

	//Go through the struct with the given target column
	while ( i < rows || strCounter == 10 || numCounter == 10) {

		test = rowArr[i]->metadata[target];
		length = strlen(test);


		//Check if it's string or number
		for (j = 0; j < length; j++) {

			//Check if there is any prescence of a letter
			if (isalpha(test[j])) {
				strCounter++;
			}

			else if (isdigit(test[j])) {
				numCounter++;
			}
		}

		//If there are more chars than ints, it's a string
		if ( strCounter > numCounter) strings++;
		else { numbers++; }
	}

	//If there are more strings than numbers, return 's'
	if (strings > numbers) return 's';
	else return 'd';

}

//Strips whitespace at the ends of a string.
char* stripSpace(char* string) {
	
	int length = strlen(string);
	char ans[length-1];
	int i;

	//Special Case: if the string is empty
	if (length == 0) {
		//printf("hit special case:\n");
		return string;
	}
	//Check if there are any spaces at all.
	if (string[0] != ' ' && string[length-1] != ' ') {
		return string;
	}

	//Case 1: if we have one one space in the front
	else if(string[0] == ' ' && string[length-1] != ' ') {

		//Start copying the string from the char after the space.
		for (i = 1; i < length; i++) {
			ans[i-1] = string[i];
		}

		//Tack on '\0'
		ans[i-1] = '\0';		
	}

	//Case 2: if we only have one space in the end
	else if(string[0] != ' ' && string[length-1] == ' ') {

		//Start copying the string until the char before the space
		for (i = 0; i < length-1; i++) {
			ans[i] = string[i];
		}

		//Tack on '\0'
		ans[i] = '\0';
	}

	//Case 3: if we have both spaces on the ends
	else {

		//Start copying the string from the char after the space.
		for (i = 1; i < length-1; i++) {
			ans[i-1] = string[i];
		}

		//Tack on '\0'
		ans[i-1] = '\0';
	}

	return strdup(ans);
}

void printCSV(group** rowArr) {

	int i, j, k;
	char* test;

	//for (j = 0; j < cols; j++) {
	//	printf("string: %s\n", rowArr[667]->metadata[j]);
	//}
	//printf("-------------------\n");
	//printf("rows: %d\n", rows);
	for(i = 0; i < rows; i++) {
		
		for (j = 0; j < cols; j++) {
			
			test = rowArr[i]->metadata[j];
			//printf("line: %s\n", test);

			if (test != NULL) {	//bug: last entry sometimes is null

				//bug:front might be blank. check bounds
				//loop through and determine if the front is blank
				//for (k = 0; )

				if (j != cols-1 && !checkComma(test) ) {
					printf("%s,", rowArr[i]->metadata[j]);				
				}
				
				else if (j != cols-1 && checkComma(test) ) {
					printf("\"%s\",", rowArr[i]->metadata[j]);				
				}	

				else if ( checkComma(test) ) {
					printf("\"%s\"", rowArr[i]->metadata[j]);				
				}	

				else {
					printf("%s", rowArr[i]->metadata[j]);
				}
			}

		}
	} //end printf double for loop

}

int checkComma(char* string) {

	int i;
	char* test = strdup(string);
	int length = strlen(test);

	if (test == NULL) {
		return 0;
	}

	for (i = 0; i < length; i++) {

		if (string[i] == ',')
			return 1;
	}

	return 0;
}
