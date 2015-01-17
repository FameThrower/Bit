#include<stdio.h>
#include<stdlib.h>

#define FOUR_DIR	0x08
#define THREE_DIR	0x04
#define TWO_DIR		0x02
#define ONE_DIR		0x01
#define NO_DIR		0x00

#define NORTH		0       	  //		0
#define SOUTH		2		  //	3		1
#define EAST		1		  //		2				 
#define WEST		3

#define startr 		3//start row
#define startc 		2//start column

#define WALL		6//largest dist to wall in inches
#define SQUARE		12 //distance from one square to the next in inches

struct ryans{
	int noWall[4]; //north = 0, east = 1, south = 2, west = 3
	int prev;
	char opening;
};

int facing = NORTH; //robot starts the maze facing NORTH
struct ryans ryan[7][7];

void init(){
	int i,x;
	for(i = 0; i<7; i++){
		for(x=0; x<7; x++){
			ryan[i][x].noWall[NORTH] = 0;
			ryan[i][x].noWall[SOUTH] = 0;
			ryan[i][x].noWall[EAST] = 0;
			ryan[i][x].noWall[WEST] = 0;
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
	if(ryan[row][col].noWall[EAST] && 
	ryan[row][col].opening && 
	!(dir & WEST)) {
	        ryan[row][col].opening >> 1;
		move(EAST);
		discover(WEST, ++row,col);
	}
	if(ryan[row][col].noWall[NORTH] && 
	ryan[row][col].opening && 
	!(dir & SOUTH)) {
		ryan[row][col].opening >> 1;
		move(NORTH);
		discover(SOUTH, row,++col);
	}
	if(ryan[row][col].noWall[WEST] && 
	ryan[row][col].opening && 
	!(dir & EAST)) {
		ryan[row][col].opening >> 1;
		move(WEST);
		discover(EAST, --row,col);	
	}
	if(ryan[row][col].noWall[SOUTH] && 
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
void turnCW(void){	
	
	
	facing = (facing+1) % 4;
}

//turn 90 degrees right, then update facing variable 
void turnCCW(void){
	
	
	facing = (facing-1) % 4;
}

//turn 180 degrees, then update facing variable
void turnAround(void){
	
	
	facing = (facing+2) % 4;
}

void moveForward(void){ //move 12 inches forward
	
	
}

//determines if a wall exists to the front, left, and right of the robot
void isWall(int row, int col){
	int i;
	
	i = getDistFront(facing);
	if(i > WALL) {
		ryan[row][col].noWall[facing] = 1;
	}else{
		ryan[row][col].opening >> 1;
	}
	
	i = getDistLeft((facing-1) % 4);
    if(i > WALL) {
		ryan[row][col].noWall[(facing-1) % 4] = 1;
	}else{
        ryan[row][col].opening >> 1;
    }
	
	i = getDistRight((facing+1) % 4);
    if(i > WALL) {
		ryan[row][col].noWall[(facing+1) % 4] = 1;
	}else{
        ryan[row][col].opening >> 1;
    }
}

//uses sensor on the front of the robot and returns distance to nearest wall
int getDistFront(int direction){
	return 0;
}
//uses sensor on the robot's left side and returns distance to nearest wall
int getDistLeft(int direction){
	return 0;
}
//uses sensor on the robot's right side and returns distance to nearest wall
int getDistRight(int direction){
	return 0;
}

int main(){
init();
discover(0, 3,4);



return 0;
}
