
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

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

struct Parameters{
	int row;
	int column;
	int byRow;
	int byGrid;
	int worker;
};

void* checkSudoku(void* param){
	int slot[10] = {0,0,0,0,0,0,0,0,0};
	struct Parameters *grid = (struct Parameters*) param;
	if(grid->byGrid){
		for(int i = grid->row; i < grid->row + 3; i++){
			for(int j = grid->column; j < grid->column + 3; j++){
				if(slot[sudoku[i][j]]!=0){//duplicates
					result[grid->worker] = 1;
					pthread_exit(0);
				}
			}
		}
	}
	else{
		if(grid->byRow){
			for(int i = 0; i < 9; i++){
				if(sudoku[grid->byRow][i]!=0){
					result[grid->worker] = 1;
					pthread_exit(0);
				}
			}
		}else{
			for(int i = 0; i < 9; i++){
				if(sudoku[i][grid->column]!=0){
					result[grid->worker] = 1;
					pthread_exit(0);
				}
			}
		}
	}

	result[grid->worker] = 0;
	pthread_exit(0);
}

int main(){
	printf("CS149 Sudoku from Kim Pham");

	struct Parameters param[NUM];
	pthread_t tid[NUM];
	//launch row thread
	for(int i = 0; i < 18; i ++){
		if(i < 9){ //by row thread
			param[i].row = i;
			param[i].column = 0;
			param[i].byRow = 1;
			param[i].byGrid = 0;
			param[i].worker = i;
		}
		else{ //by column thread
			param[i].row = 0;
			param[i].column = i-9;
			param[i].byRow = 0;
			param[i].byGrid = 0;
			param[i].worker = i;
		}
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid[i], &attr, checkSudoku, &param[i]);
	}

	int id = 18;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			param[i].row = i*3;
			param[i].column = j*3;
			param[i].byRow = 0;
			param[i].byGrid = 1;
			param[i].worker = id;

			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tid[id], &attr, checkSudoku, &param[id]);
			id++;
		}
	}

	//waiting for thread
	for(int i = 0; i < NUM; i++){
		pthread_join(tid[i], NULL);
	}

	//annoucing result
	for(int i = 0; i < NUM; i++){
		if(result[i]==1){
			printf("Duplicates found by thread %d", i);
		}else{
			printf("no duplicates found by thread %d", i);
		}
	}



}
