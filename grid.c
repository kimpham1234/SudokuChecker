
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){

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

	int sum = 0;
	int row = 0;
	int col = 3;
	for(int i = row; i < row + 3; i++){
			for(int j = col; j < col + 3; j++){
				sum += sudoku[i][j];
			}
	}

	printf("Sum %d", sum);

}
