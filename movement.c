#include<stdio.h>
#include<stdlib.h>

#define FOUR_DIR	0x08
#define THREE_DIR	0x04
#define TWO_DIR		0x02
#define ONE_DIR		0x01
#define NO_DIR		0x00 //hello
#define NORTH		1
#define SOUTH		2
#define EAST		3
#define WEST		4//direction/...^^^
#define startr 		3//start row
#define startc 		2//start column
#define WALL		6//largest dist to wall

struct ryans{
	int noWallN;
	int noWallS;
	int noWallE;
	int noWallW;//north south east and west orientation.
	int prev;
	char opening;
	
};

struct ryans ryan[7][7];

void init(){
	int i,x;
	for(i = 0; i<7; i++){
		for(x=0; x<7; x++){
			ryan[i][x].noWallN = 0;
			ryan[i][x].noWallS = 0;
			ryan[i][x].noWallE = 0;
			ryan[i][x].noWallW = 0;
			ryan[i][x].prev = 0;
			ryan[i][x].opening = FOUR_DIR;
		}
	}
}

void discover(int dir, int row, int col){
	ryan[row][col].opening >> 1;
	if(dir){
		ryan[row][col].prev = dir;
	}
	isWall(row,col);
	if(ryan[row][col].noWallE && 
	ryan[row][col].opening && 
	!(dir & WEST)) {
	        ryan[row][col].opening >> 1;
		move(EAST);
		discover(WEST, ++row,col);
	}
	if(ryan[row][col].noWallN && 
	ryan[row][col].opening && 
	!(dir & SOUTH)) {
		ryan[row][col].opening >> 1;
		move(NORTH);
		discover(SOUTH, row,++col);
	}
	if(ryan[row][col].noWallW && 
	ryan[row][col].opening && 
	!(dir & EAST)) {
		ryan[row][col].opening >> 1;
		move(WEST);
		discover(EAST, --row,col);	
	}
	if(ryan[row][col].noWallS && 
	ryan[row][col].opening && 
	!(dir & NORTH)) {
		ryan[row][col].opening >> 1;
		move(SOUTH);
		discover(NORTH, row,--col);
	}
	if(!ryan[row][col].opening) MOVE(dir); //reverse
	
}

int move(){
return 0;
}

void isWall(int row, int col){
	int i;
	i = getDistN();
	if(i > WALL) {
		ryan[row][col].noWallN = 1;
	}else{
		ryan[row][col].opening >> 1;
	}
	i = getDistS();
        if(i > WALL) {
		ryan[row][col].noWallS = 1;
	}else{
                ryan[row][col].opening >> 1;
        }
	i = getDistE();
        if(i > WALL) {
		ryan[row][col].noWallE = 1;
	}else{
                ryan[row][col].opening >> 1;
        }
	i = getDistW();
	if(i > WALL) {
		ryan[row][col].noWallW = 1;
	}else{
                ryan[row][col].opening >> 1;
        }
}

int main(){
init();
discover(0, 3,4);



return 0;
}
