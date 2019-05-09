//	Author : Aykut Sahin
//	Contact: sahinayk@fit.cvut.cz
//
//	Program explanation:
//		In a summary, the program gets an input contains a puzzle of hidden words and a list of
//	words to search for in the puzzle. Task is to find all the words and struck out the unmatched
//	characters of the puzzle.
//
//	Algorithm explanation (Enhanced Brute Force):
//		Once the first letter of a word is found, try to match the rest of the word in all
//	directions. However, if the length of the word is too long to fit in between the position where
//	the first letter was found and the bound of the puzzle in regarding direction or if there is a
//	period on the way to the last letter of the word then stop and move for the next step.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char** create2DArrayForPuzzle				(int maxRows, int maxColumns);
char** createArrayOfStringsForListOfWords		(int numCities, int cityLength);
void emptyLineCheckF					(int realRows, int emptyLineCheck, int freeGrid, int freeWords, char** grid, char* words[]);
int** create2DArrayPuzzleBitmask			(int realRows, int realColumns);
int* createWordCheckBitmask				(int realNumberCities);
void solvePuzzle					(char** grid, char** words, int** isMatched, int* wordCheck, int realNumberCities, int realRows, int realColumns);
void detectionNotFoundWord				(int* wordCheck, int realNumberCities, char* words[], int** isMatched, char** grid, int realRows, int freeGrid, int freeWords);
void struckOut						(int** isMatched, char** grid, int realRows, int realColumns);
void exitWithoutError					(int** isMatched, char** grid, char* words[], int* wordCheck, int realRows, int freeGrid, int freeWords);
void invalidInputExitFunction				(int freeGrid, int freeWords, char** grid, char* words[]);

int main (void)
{
	// Variables, are to work with 2d array of puzzle. In case the puzzle grid is bigger than
	// 100x100 the array boundaries will be upscaled by n*maxRows and m*maxColumns
	int rows = 0, maxRows = 100;
	int columns = 0, maxColumns = 100;

	// Variables, are to work with array of words to search for. The array boundary will be upscaled
	// by n*numCities
	int numCities = 100, realNumberCities = 0;
	int cityLength = 50;

	// Variables, are to be used only for freeing memory that is allocated
	int freeWords = numCities, freeGrid = maxRows;

	// Variables, are to work with arrays within actual array boundaries
	int realColumns = 0, realRows = 0;

	// Variable, is to be set to the count of empty line(s) between the puzzle and words
	int emptyLineCheck = 0;

	// Declare 2D array a.k.a the puzzle
	char** grid = create2DArrayForPuzzle(maxRows, maxColumns);

	// Declare list of the words
	char** words = createArrayOfStringsForListOfWords(numCities, cityLength);

	// Get the input, fill the puzzle and words arrays and validate the input
	char c, prevChar = 0;

// ################################# Input handling starts #################################

	printf("Enter your wordfind puzzle:\n");
	while ((c = getchar()) != '\n')
	{
		// VALIDATION OF INPUT
		if (((int)c >= 97 && (int)c <= 122) || c == '.' )
		{
			// UPSCALING "MAXCOLUMNS", MORE THAN 100*N CHARS IN 1 ROW
			if (columns == maxColumns)
			{
				maxColumns = maxColumns + 100;
				for (int i = 0; i < maxRows; i++)
				{
					grid[i] = (char*)realloc(grid[i], sizeof(**grid) * maxColumns);
				}
			}
			// STORING INDIVIDUAL CHARS AT CERTAIN POSITIONS OF THE 2D ARRAY
			grid[rows][columns] = c;
			columns++;
		}
		else
		{
			invalidInputExitFunction(freeGrid, freeWords, grid, words);
		}
	}
	rows++;
	realColumns = columns; //THE ACTUAL SIZE OF COLUMNS GETS SET HERE AND MUST REMAIN THE SAME
	columns = 0;	// EACH TIME THE ROWS INCREASES, "COLUMNS" IS TO SET TO 0
	prevChar = '\n';

	while ((c = getchar()) != EOF)
	{
		// VALIDATION OF IMPUT
		if (((int)c >= 97 && (int)c <= 122) || c == '\n' || c == '.')
		{
			if (columns > realColumns)
			{
				invalidInputExitFunction(freeGrid, freeWords, grid, words);
			}

			// DETECTION OF A NEWLINE. "ROWS" IS TO BE INCREMENTED BY 1
			if (c == '\n')
			{
				// DETECTION OF AN EMPTY LINE. 
				if (prevChar == '\n')
				{
					// DETECTION OF AN EMPTY PUZZLE
					if (realColumns == 0)
					{
						invalidInputExitFunction(freeGrid, freeWords, grid, words);
					}

					emptyLineCheck = 1;
					realRows = rows; // ACTUAL SIZE OF ROWS GETS SET HERE.
					rows = 0;
					columns = 0;

					// START GETTING THE LIST OF WORDS
					while ((c = getchar()) != EOF)
					{
						// VALIDATION OF INPUT
						if (((int)c >= 97 && (int)c <= 122) || c == '\n')
						{
							// UPSCALE THE COUNT OF WORDS
							if (rows == numCities)
							{
								numCities = numCities + 100;
								freeWords = numCities;
								words = (char**)realloc(words, sizeof(*words) * numCities);
								for (int i = numCities - 100; i < numCities; i++)
								{
									words[i] = (char*)malloc(sizeof(char*) * cityLength);
								}
							}
							// UPSCALE THE LENGTH OF WORDS
							if (columns == cityLength)
							{
								cityLength = cityLength + 100;
								for (int i = numCities - 100;  i < numCities; i++)
								{
									words[i] = (char*)realloc(words[i], sizeof(char*) * cityLength);
								}
							}

							// NEXT WORD
							if(c != '\n')
							{
								words[rows][columns] = c;
								columns++; 
							}
							else
							{
								c = '\0';
								words[rows][columns] = c;
								rows++;
								columns = 0;
							}
						}
						else
						{
							invalidInputExitFunction(freeGrid, freeWords, grid, words);
						}
					}
					realNumberCities = rows; // ACTUAL COUNT OF WORDS GETS SET HERE
				}
				else
				{
					// DETECTING IF PUZZLE IS NOT IN A RECTANGULAR SHAPE
					if (columns != realColumns)
					{
						invalidInputExitFunction(freeGrid, freeWords, grid, words);
					}

					// UPSCALING "MAXROWS" MORE THAN 100*N LINES OF PUZZLE
					if (rows + 1 == maxRows)
					{
						maxRows = maxRows + 100;
						freeGrid = maxRows;
						grid = (char**)realloc(grid, sizeof(*grid) * maxRows);
						for (int i = maxRows - 100; i < maxRows; i++)
						{
							grid[i] = (char*)malloc(sizeof(**grid) * realColumns);	
						}
					}
					rows++;
					columns = 0;
				}
			}
			else
			{
				grid[rows][columns] = c;
				columns++;
			}
		}
		else
		{
			invalidInputExitFunction(freeGrid, freeWords, grid, words);
		}

		prevChar = c;
	}
// ################################# Input handling finishes #################################

	// Check for an empty line between puzzle and the words
	emptyLineCheckF(realRows, emptyLineCheck, freeGrid, freeWords, grid, words);

	// Declaring bitmasks for puzzle and the list of the words
	int** isMatched = create2DArrayPuzzleBitmask(realRows, realColumns);
	int* wordCheck = createWordCheckBitmask(realNumberCities);

	// Solve the puzzle
	solvePuzzle(grid, words, isMatched, wordCheck, realNumberCities, realRows, realColumns);

	// Detect not-found word
	detectionNotFoundWord(wordCheck, realNumberCities, words, isMatched, grid, realRows, freeGrid, freeWords);

	// Struck out non-matched characters from the puzzle
	struckOut(isMatched, grid, realRows, realColumns);

	// Terminate without error
	exitWithoutError(isMatched, grid, words, wordCheck, realRows, freeGrid, freeWords);

	return 0;
}

// ############################# FUNCTIONS #############################

char** create2DArrayForPuzzle(int maxRows, int maxColumns)
{
	char** grid = (char**)malloc(sizeof(*grid) * maxRows);
	for (int i = 0; i < maxRows; i++)
	{
		grid[i] = (char*)malloc(sizeof(**grid) * maxColumns);
	}
	return grid;
}

char** createArrayOfStringsForListOfWords(int numCities, int cityLength)
{
	char** words = (char**)malloc(sizeof(*words) * numCities);
	for (int i = 0; i < numCities; i++)
	{
		words[i] = (char*)malloc(sizeof(char*) * cityLength);
	}
	return words;
}


void emptyLineCheckF(int realRows, int emptyLineCheck, int freeGrid, int freeWords, char** grid, char* words[])
{
	if (realRows == 0 || emptyLineCheck != 1)
	{
		invalidInputExitFunction(freeGrid, freeWords, grid, words);
	}
}

int** create2DArrayPuzzleBitmask(int realRows, int realColumns)
{
	int** isMatched = (int**)malloc(sizeof(*isMatched) * realRows);
	for (int i = 0; i < realRows; i++)
	{
		isMatched[i] = (int*)malloc(sizeof(**isMatched) * realColumns);
	}
	return isMatched;
}

int* createWordCheckBitmask(int realNumberCities)
{
	int* wordCheck = (int*)malloc(sizeof(int*) * realNumberCities);
	return wordCheck;
}

void solvePuzzle(char** grid, char** words, int** isMatched, int* wordCheck, int realNumberCities, int realRows, int realColumns)
{
	char c;
	for (int k = 0; k < realNumberCities; k++)
	{
		for (int i = 0; i < realRows; i++)
		{
			for (int j = 0; j < realColumns; j++)
			{
				if (grid[i][j] == words[k][0])
				{
					if (strlen(words[k]) == 1)
					{
						if (words[k][1] == '\0')
						{
							wordCheck[k] = 1;
							isMatched[i][j] = 1;
						}
						break;
					}
					else {

					if (realColumns >= (int)(j + strlen(words[k])) && grid[i][j+1] != '.')
					{
						for (int l = 0; words[k][l] == grid[i][j+l]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i][j+n] = 1;
								}
								break;
							}
						}
					}
					if (realRows >= (int)(i + strlen(words[k])) && realColumns >= (int)(j + strlen(words[k])) && grid[i+1][j+1] != '.')
					{
						for (int l = 0; words[k][l] == grid[i+l][j+l]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i+n][j+n] = 1;
								}
								break;
							}
						}
					}
					if (realRows >= (int)(i + strlen(words[k])) && grid[i+1][j] != '.')
					{
						for (int l = 0; words[k][l] == grid[i+l][j]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i+n][j] = 1;
								}
								break;
							}
						}
					}
					if (realRows >= (int)(i + strlen(words[k])) && j+1 >= (int)(strlen(words[k])) && grid[i+1][j-1] != '.')
					{
						for (int l = 0;  words[k][l] == grid[i+l][j-l]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i+n][j-n] = 1;
								}
								break;
							}
						}
					}
					if (j+1 >= (int)(strlen(words[k])) && grid[i][j-1] != '.')
					{
						for (int l = 0; words[k][l] == grid[i][j-l]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i][j-n] = 1;
								}
								break;
							}
						}
					}
					if (i+1 >= (int)(strlen(words[k])) && j+1 >= (int)(strlen(words[k])) && grid[i-1][j-1] != '.')
					{
						for (int l = 0; words[k][l] == grid[i-l][j-l]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i-n][j-n] = 1;
								}
								break;
							}
						}
					}
					if (i+1 >= (int)(strlen(words[k])) && grid[i-1][j] != '.')
					{
						for (int l = 0; words[k][l] == grid[i-l][j]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i-n][j] = 1;
								}
								break;
							}
						}
					}
					if (realColumns >= (int)(j + strlen(words[k])) && i+1 >= (int)(strlen(words[k])) && grid[i-1][j+1] != '.')
					{
						for (int l = 0; words[k][l] == grid[i-l][j+l]; l++)
						{
							if (words[k][l+1] == '\0')
							{
								wordCheck[k] = 1;
								for (int n = 0; n < (int)(strlen(words[k])); n++)
								{
									isMatched[i-n][j+n] = 1;
								}
								break;
							}
						}
					}
					}
				}
			}
		}
	}
}

void detectionNotFoundWord(int* wordCheck, int realNumberCities, char* words[], int** isMatched, char** grid, int realRows, int freeGrid, int freeWords)
{
	for (int i = 0; i < realNumberCities; i++)
	{
		if (wordCheck[i] != 1)
		{
			printf("Word '%s' not found.\n", words[i]);
			exitWithoutError(isMatched, grid, words, wordCheck, realRows, freeGrid, freeWords);		
		}
	}
}

void struckOut(int** isMatched, char** grid, int realRows, int realColumns)
{
	int counter = 0;
	printf("Result:\n");
	for (int i = 0; i < realRows; i++)
	{
		for (int j = 0; j < realColumns; j++)
		{
			if (isMatched[i][j] != 1 && grid[i][j] != '.')
			{
				printf("%c", grid[i][j]);
				counter++;
				if (counter == 60)
				{
					printf("\n");
					counter = 0;
				}
			}
		}
	}
	if (counter != 0)
	{
		printf("\n");
	}
}

void exitWithoutError(int** isMatched, char** grid, char* words[], int* wordCheck, int realRows, int freeGrid, int freeWords)
{
	for(int i = 0; i < realRows; i++)
	{
		free(isMatched[i]);
	}
	free(isMatched);
	free(wordCheck);
	for(int i = 0; i < freeGrid; i++)
	{
		free(grid[i]);
	}
	free(grid);
	for(int i = 0; i < freeWords; i++)
	{
		free(words[i]);
	}
	free(words);
	exit(0);
}

void invalidInputExitFunction(int freeGrid, int freeWords, char** grid, char* words[])
{
	printf("Invalid input.\n");
	for (int i = 0; i < freeGrid; i++)
	{
		free(grid[i]);
	}
	free(grid);
	for (int i = 0; i < freeWords; i++)
	{
		free(words[i]);
	}
	free(words);
	exit(1);
}

char** reallocationwords(char*** arr, int newsize)
{
	*arr = (char**)realloc(*arr, sizeof(**arr) * newsize);
	for (int i = newsize-100; i < newsize; i++)
	{
		*arr[i] = (char*)malloc(sizeof(char*) * 50);
	}
	return *arr;
}

char** reallocationwordslength(char*** arr, int newsize)
{
	for (int i = newsize-100; i < newsize; i++)
	{
		*arr[i] = (char*)realloc(*arr, sizeof(char*) * newsize);
	}
	return *arr;
}

char** reallocationgrid(char*** arr, int newsize)
{
	*arr = (char**)realloc(*arr, sizeof(**arr) * newsize);
	for (int i = newsize-100; i < newsize; i++)
	{
		*arr[i] = (char*)malloc(sizeof(***arr) * newsize);
	}
	return *arr;
}

char** reallocationgridcolumns(char*** arr, int newsize)
{
	for (int i = 0; i < newsize; i++)
	{
		*arr[i] = (char*)realloc(*arr[i], sizeof(***arr)*newsize);
	}
	return *arr;
}









