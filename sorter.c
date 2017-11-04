#include "sorter.h"

//declare functions

int main(int argc, char* argv[]) {

	int i, j, k;

	//Check if first argument is -c
	//if (argv[1] != "-c") {
	//	printf("Error");
	//	exit(0);
	//}

	//Check if we have enough arguments
	//if (argc < 2) {
	//	printf("Not enough arguments");
	//	exit(0);
	//}

	//Now, open the file to read
	FILE * input = fopen("movie_metadata.csv", "r");

	//Create one to write out with
	FILE * output = fopen("results.csv", "w");

	/*
	 * CREATING THE HEADERS ARRAY
	 */

	// Step 1) Determine how many columns there are
	char line[10000];
	fgets(line, 10000, input);

	// 1a) Count how many entries divided by commas
	for ( i = 0; line[i] != '\0'; i++ ) {
		cols += (line[i] == ',');
	} cols++; //accounts for last header with no comma

	// Step 2) Placing the headers in an array for reference
	char* headers[cols];
	char* token;
	char* string = strdup(line);

	i = 0;
	while( (token = strsep(&string, ",")) != NULL) {
		headers[i] = token;
		i++;
	}

	/*
	 * INPUT ALL DATA (into structs that contain char* arrs)
	 * Algorithm: create an array of pointers that pointer to
	 *			  structs w/ flexible array members.
	 */
	
	//Step 1) Create an array of pointers that point to structs.
	group **rowArr = malloc(mallocSize*sizeof(group*));
	for (i = 0; i < mallocSize; i++) {
		//Intiialize struct with dynamic array
		rowArr[i] = malloc(sizeof(group) + cols*sizeof(char*));
	}

	//Step 2) strsep() the information into a struct in the array
	// i handles which struct, j handles index in metadata array
	i = 0;
	while ( fgets(line, 10000, input) != NULL) {	//traverse through all rows

		//Increment total number of rows
		rows++;

		//Create char* for strsep()
		string = strdup(line);

		/* CHECK PARSING CASES
		 */

		// 1) Average case: there are no "" in the row.
		if ( strchr(string, '\"') == NULL) {
			
			for ( j = 0; j < cols; j++ ) {
				token = strsep(&string, ",");
				token = stripSpace(token);			
				rowArr[i]->metadata[j] = token;			
			}
		}

		// 2) If we have "" in the line
		else { //strchr returns != NULL

			//printf("inserted into special strsep\n");
			//Figure out how many commas before ""
			int commas = 0;
			int length = strlen(string);
			for (k = 0; i < length; k++) {

				if (string[k] == ',') commas++;

				else if (string[k] == '\"') break;
			}

			//strsep() that many times
			for (j = 0; j < commas; j++) {				
				token = strsep(&string, ",");
				token = stripSpace(token);			
				rowArr[i]->metadata[j] = token;
				//printf("token: %s\n", token);
			}

			//Now deal with the commas
			token = strsep(&string, "\"");
			token = strsep(&string, "\"");

			//put special case into array
			token = stripSpace(token);			
			rowArr[i]->metadata[j] = token;
			//printf("token: %s\n", token);

			j++;

			//skip over next comma
			token = strsep(&string, ",");

			//strsep() the rest of the line
			while ( (token = strsep(&string, ",")) != NULL ) {				
				token = stripSpace(token);
				rowArr[i]->metadata[j] = token;
				//printf("token: %s\n", token);
				j++;
			}

			//printf special case
			//for (j = 0; j < cols; j++) {
			//	printf("%s\n", rowArr[i]->metadata[j]);
			//}

		} //end else special case

		i++; //increment to next struct

	} //end getting lines (while statement)

	/*
	 * PREPPING THE ARRAYS FOR MERGE SORT
	 */
	
	//Determine which header we are going to sort via args
	//int det = targetCol()
	int low = 0;
	int high = rows-1;
	//mergeSort(9, 's', rowArr, low, high);

	//Output results
	printCSV(rowArr);

	//free and close
	for (i = 0; i < mallocSize; i++) {
		free(rowArr[i]);
	} free(rowArr);

	fclose(input);
	fclose(output);
	return 0;
}