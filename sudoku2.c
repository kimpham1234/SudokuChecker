
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

struct Parameters{
	int row;
	int column;
	int byRow;
	int byGrid;
	int worker;
};

const int NUM = 27;

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

int result[27];

void* checkSudokuRow(void* param){
	int sum = 0;
	struct Parameters *grid = (struct Parameters*) param;

	for(int i = 0; i < 9; i++){
		sum += sudoku[grid->row][i];
	}

	if(sum!=45){
		printf("duplicates in row %d!\n", grid->worker);
	}else{
		printf("Sum %d\n", sum);
	}

	pthread_exit(0);
}

void* checkSudokuCol(void* param){
	int sum = 0;
	struct Parameters *grid = (struct Parameters*) param;

	for(int i = 0; i < 9; i++){
		sum += sudoku[i][grid->column];
	}

	if(sum!=45){
		printf("duplicates in col %d!\n", (grid->worker)-9);
	}else{
		printf("Sum %d\n", sum);
	}

	pthread_exit(0);
}

void* checkSudokuGrid(void* param){
	int sum = 0;
	struct Parameters *grid = (struct Parameters*) param;

	for(int i = grid->row; i < grid->row + 3; i++){
		for(int j = grid->column; j < grid->column + 3; j++){
			sum += sudoku[i][j];
		}
	}

	if(sum!=45){
		printf("duplicates in grid %d!\n", grid->worker);
	}else{
		printf("sum %d\n", sum);
	}

	pthread_exit(0);
}

int main(int argc, char** argv){
	struct Parameters params[9];
	pthread_t tid[9];

	//row
	for(int i = 0; i < 9; i++){
		params[i].worker = i;
		params[i].row = i;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid[i], &attr, checkSudokuRow, &params[i]);
	}

	for(int i = 0; i < 9; i++)
		pthread_join(tid[i], NULL);
	printf("done checking row");

	for(int i = 0; i < 9; i++){
		params[i].worker = 9 + i;
		params[i].column = i;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid[i], &attr, checkSudokuCol, &params[i]);
	}

	for(int i = 0; i < 9; i++)
		pthread_join(tid[i],NULL);
	printf("done checking col\n");

	int id = 0;

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			params[id].row = i*3;
			params[id].column = j*3;
			params[id].worker = id;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tid[id], &attr, checkSudokuGrid, &params[id]);
			id++;
		}
	}

	for(int i = 0; i < 9; i++)
		pthread_join(tid[i], NULL);
	printf("done checking grid");
}
/*
 */

