#include<stdio.h>
#include<stdlib.h>

#define NORTH		0       	  //		0
#define SOUTH		2		  //	3		1
#define EAST		1		  //		2				 
#define WEST		3

#define startRow	7  //start row
#define startCol	7  //start column
#define endRow		1  //end row
#define endCol		1  //end col

#define WALL		6  //largest dist to wall in inches
#define SQUARE		12 //distance from one square to the next in inches

struct ryans{
	int noWall[4]; //north = 0, east = 1, south = 2, west = 3

	char newSquare; //first time entering new square, so use sensors to determine wall placement
};

int critPathArr[50]; //used for determining critical path
int path_size;
char path_found; //boolean used to stop looking for critical path once the endRow,endCol is reached


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
			ryan[i][x].opening = FOUR_DIR;
			ryan[i][x].newSquare = 1;
		}
	}
	for(i = 0; i < 50; i++){
		critPathArr[i] = -1;	
	}
	path_size = 0;
}

//used for PART 1 of the maze - searching
void discover(int dir, int row, int col){

	if(row==endRow && col==endCol){ //end square reached, so stop searching for critical path
		path_found == 1;	
	}
	if(!path_found)path_size++; //go to next 
	
	if(ryan[row][col].newSquare == 1){ //robot has not been to this square before, so check for walls
		isWall(row,col);
		ryan[row][col].newSquare == 0;
	}
	
	//moving EAST
	if(ryan[row][col].noWall[EAST] &&  
	!(dir & WEST)) { //if you just went WEST (came from the EAST), don't go east til last(backtracking direction)
	        if(!path_found)critPathArr[path_size] = EAST;
	        
	        ryan[row][col].opening >> 1;
		move(EAST);
		discover(EAST, ++row,col);
	}
	
	//moving NORTH
	if(ryan[row][col].noWall[NORTH] && 
	!(dir & SOUTH)) {//if you just went SOUTH (came from the NORTH), don't go east (backtracking direction)
		if(!path_found)critPathArr[path_size] = NORTH;
		
		move(NORTH);
		discover(NORTH, row,++col);
	}
	
	//moving WEST
	if(ryan[row][col].noWall[WEST] && 
	!(dir & EAST)) {
		if(!path_found)critPathArr[path_size] = WEST;
		
		move(WEST);
		discover(WEST, --row,col);	
	}
	
	//moving SOUTH
	if(ryan[row][col].noWall[SOUTH] && 
	!(dir & NORTH)) {
		if(!path_found)critPathArr[path_size] = SOUTH;
		
		ryan[row][col].opening >> 1;
		move(SOUTH);
		discover(SOUTH, row,--col);
	}
	
	//backtracking if there are no more squares to explore from the current square, 
	//and moves up the recursion stack
	if(!path_found)path_size--; //this square is not part of the critical path since it
	move((dir + 2) % 4);
	
}
void takeCriticalPath(void){
	int i = 0;
	while(critPathArr[i] >= 0){ // a -1 means the path is over
		move(critPathArr[i]);
		i++;
	}
}

//keep track of direction robot is facing
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
	
	//should return distance moved
	return 0;
}

//turn 90 degrees right, then update facing variable 
void turnCW(void){	
	
	
	facing = (facing+1) % 4;
}

//turn 90 degrees left, then update facing variable 
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
	
	i = getDistFront();
	if(i > WALL) {
		ryan[row][col].noWall[facing] = 1;
	}
	
	i = getDistLeft();
    	if(i > WALL) {
		ryan[row][col].noWall[(facing-1) % 4] = 1;
	}
	
	i = getDistRight();
    	if(i > WALL) {
		ryan[row][col].noWall[(facing+1) % 4] = 1;
	}
    	//when a new square is entered, there is never a wall behind (except at the beginning)
        if(row!=startRow && col!=startCol)ryan[row][col].noWall[(facing+2) % 4] = 1;
}

//uses sensor on the front of the robot and returns distance to nearest wall
//direction = facing
int getDistFront(void){
	return 0;
}
//uses sensor on the robot's left side and returns distance to nearest wall
//direction = (facing - 1) % 4
int getDistLeft(void){
	return 0;
}
//uses sensor on the robot's right side and returns distance to nearest wall
//direction = (facing + 1) % 4
int getDistRight(void){
	return 0;
}

int main(){
	init();
	
	//wait here until a button on the robot is pushed
	
	discover(2, startRow,startCol);

	//wait here until the button is pushed again
	
	takeCriticalPath();


	return 0;
}
