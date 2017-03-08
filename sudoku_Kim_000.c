
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

//struct for passing parameters
struct Parameters{
	int row;
	int column;
	int worker;
};

//sudoku grid
int sudoku[9][9] = {
	{6,5,3,1,2,8,7,9,4},
	{1,7,4,3,5,9,6,8,2},
	{9,2,8,4,6,7,5,3,1},
	{2,8,6,5,1,4,3,7,9},
	{3,9,1,7,8,2,4,5,6},
	{5,4,7,6,9,3,2,1,8},
	{8,6,5,2,3,1,9,4,7},
	{4,1,2,9,7,5,8,6,3},
	{7,3,9,8,4,6,1,2,5}};

//array to store result
int row[9], col[9], square[9];

//thread to check a single row
void* checkSudokuRow(void* param){
	int sum = 0;
	struct Parameters *grid = (struct Parameters*) param;

	for(int i = 0; i < 9; i++){
		sum += sudoku[grid->row][i];
	}

	if(sum!=45)
		row[grid->worker] = 1;
	else row[grid->worker] = 0;

	pthread_exit(0);
}

//thread to check a single column
void* checkSudokuCol(void* param){
	int sum = 0;
	struct Parameters *grid = (struct Parameters*) param;

	for(int i = 0; i < 9; i++){
		sum += sudoku[i][grid->column];
	}

	if(sum!=45)
		col[grid->worker] = 1;
	else col[grid->worker] = 0;

	pthread_exit(0);
}

//thread to check a single grid
void* checkSudokuGrid(void* param){
	int sum = 0;
	struct Parameters *grid = (struct Parameters*) param;

	for(int i = grid->row; i < grid->row + 3; i++){
		for(int j = grid->column; j < grid->column + 3; j++){
			sum += sudoku[i][j];
		}
	}

	if(sum!=45)
		square[grid->worker] = 1;
	else square[grid->worker] = 0;

	pthread_exit(0);
}

//print a sudoku
void printSudoku(){
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			printf("%d ", sudoku[i][j]);
		}
		printf("\n");
	}
}


int main(int argc, char** argv){

	printf("CS149 Sudoku from Kim Pham\n");

	printSudoku();

	//structs to pass param for rows, cols, grids
	struct Parameters rowParams[9];
	struct Parameters colParams[9];
	struct Parameters gridParams[9];

	//tid to track threads that check row, col, grid
	pthread_t rowId[9];
	pthread_t colId[9];
	pthread_t gridId[9];

	//loop to start threads
	for(int i = 0; i < 9; i++){
		//set up thread to check a row
		rowParams[i].worker = i; //thread #
		rowParams[i].row = i;

		//start thread
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&rowId[i], &attr, checkSudokuRow, &rowParams[i]);

		//set up thread to check a col
		colParams[i].worker = i; //thread #
		colParams[i].column = i;

		//start thread
		pthread_attr_t attr2;
		pthread_attr_init(&attr2);
		pthread_create(&colId[i], &attr2, checkSudokuCol, &colParams[i]);
	}

	//set up thread to check a grid
	int id = 0;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){

			//set up grid coordinate
			gridParams[id].row = i*3;
			gridParams[id].column = j*3;

			//thread #
			gridParams[id].worker = id;

			//start thread
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&gridId[id], &attr, checkSudokuGrid, &gridParams[id]);
			id++;
		}
	}

	//join threads
	for(int i = 0; i < 9; i++){
		pthread_join(rowId[i], NULL);
		pthread_join(colId[i], NULL);
		pthread_join(gridId[i], NULL);
	}

	//check results
	for(int i = 0; i < 9; i++){
		if(row[i]==1||col[i]==1||square[i]==1){
			printf("Invalid sudoku\n");
			return 0;
		}
	}
	printf("Valid sudoku\n");
}
