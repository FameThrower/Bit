#include<stdio.h>
#include<stdlib.h>

#define FOUR_DIR	0x08
#define THREE_DIR	0x04
#define TWO_DIR		0x02
#define ONE_DIR		0x01
#define NO_DIR		0x00

#define NORTH		0         	  //		0
#define SOUTH		2		  //	3		1
#define EAST		1		  //		2				 
#define WEST		3

#define startr 		3//start row
#define startc 		2//start column

#define WALL		6//largest dist to wall
#define SQUARE		12 //distance from one square to the next in inches

struct ryans{
	int noWallN;
	int noWallS;
	int noWallE;
	int noWallW;//north south east and west orientation.
	int prev;
	char opening;
};

int facing = NORTH; //robot starts the maze facing NORTH
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

//keep track of direction robot is facing
//sends command to PSOC
//faces the correct direction, then moves forward 12 inches
//returns distance moved
//
//		0
//	3		1
//		2
int move(int direction){
	if(direction != facing){
		//direction is 180 degrees from where the robot is facing
		if(((facing-direction) % 4) == 2){
			turnAround();
		}
		//direction is 90 degrees to the right of the robot
		else if((facing - direction) < 0 || (facing == 3 && direction == 0)){
			turnCW();
		//direction is 90 degrees to the left of the robot
		}else{
			turnCCW();
		}
	}
	moveForward();//advance to a new square, which is 12 inches ahead
	
	//should return 
	return 0;
}

//turn 90 degrees right, then update facing variable 
void turnCW(){	
	
	
	facing = (facing+1) % 4;
}

//turn 90 degrees right, then update facing variable 
void turnCCW(){
	
	
	facing = (facing-1) % 4;
}

//turn 180 degrees, then update facing variable
void turnAround(){
	
	
	facing = (facing+2) % 4;
}

void moveForward(){ //move 12 inches forward
	
	
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
