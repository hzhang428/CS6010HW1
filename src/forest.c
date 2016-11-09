/*
 * forest.c
 *
 *  Created on: Aug 26, 2016
 *      Author: haozhang
 *		GTID: 902790220
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define GRID 100

double strikeRate = 0.02;
double growthRate = 0.01;

/*
 * random number generation method
 */
double roll() {
	return (double)rand() / (double)RAND_MAX;
}

/*
 * decide if a tree will grow when the cell is 0,
 * otherwise record the tree number if the cell is 1 to calculate the P
 */
void grow(int field[GRID][GRID], double growthRate, int *treeNumber) {
	int i,j;
	for (i = 0; i < GRID; i++) {
		for (j = 0; j < GRID; j++) {
			if (field[i][j] == 0) {
				double dice = roll();
				if (dice < growthRate) {
					field[i][j] = 1;
				}
			} else {
				*treeNumber += 1;
			}
		}
	}
}

/*
 * recursive call to burn the neighbors
 * when hit the boundary return,
 * when the neighbor cell is 0 (no tree) return,
 * otherwise burn the neighbors then return
 */
void burn(int field[GRID][GRID], int row, int col) {
	if (row == GRID || col == GRID || row < 0 || col < 0) {
		return;
	} else if (field[row][col] == 0) {
		return;
	} else if (field[row][col] == 1) {
		field[row][col] = 0;
		burn(field, row - 1, col);
		burn(field, row, col + 1);
		burn(field, row + 1, col);
		burn(field, row, col - 1);
	}
	return;
}

/*
 * if the cell if 1 decide whether the strike is happening,
 * otherwise, if the cell is 0 skip.
 */
void strike(int field[GRID][GRID], double strikeRate) {
	int i,j;
	for (i = 0; i < GRID; i++) {
		for (j = 0; j < GRID; j++) {
			if (field[i][j] == 1) {
				double dice = roll();
				if (dice < strikeRate) {
					burn(field, i, j);
				}
			}
		}
	}
}

/*
 * printing out the field for debugging purpose
 */
void printField(int field[GRID][GRID]) {
	int i, j;
	for (i = 0; i < GRID; i++) {
		for (j = 0; j < GRID; j++) {
			if (j == GRID - 1) {
				printf("%d\n", field[i][j]);
			} else {
				printf("%d ", field[i][j]);
			}
		}
	}
}

/*
 * process the simulation with growth rate looping through 0.01 to 0.99
 */
int main() {

	printf("Growth Rate \tAverage Tree Number\n");

	for (; growthRate < 1; growthRate += 0.01) {
		int treeNumber = 0;
		int timeStep = 0;
		int field[GRID][GRID];
		double avgTreeNumber = 0.0;
		int i, j;

		//initialize the array with all 0
		for (i = 0; i < GRID; i++) {
			for (j = 0; j < GRID; j++) {
				field[i][j] = 0;
			}
		}

		//repeat grow and strike steps until reaches steady state
		while (timeStep <= 400) {
			grow(field, growthRate, &treeNumber);
			strike(field, strikeRate);
			timeStep += 1;
		}

		//print the p after each growth rate iteration
		avgTreeNumber = (double)treeNumber / (double)(timeStep - 1);
		printf("%lf \t%lf\n", growthRate, avgTreeNumber);
	}
}
