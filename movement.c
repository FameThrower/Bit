//TODO: Green LED light at the beginning square, Red LED light at the end square
//	Find easy way to switch from different size mazes (5x5 and 6x6) start from an outside square, so
//	cant just make general code to fit all mazes
//	sensors, PSoC communication......

#include<stdio.h>
#include<stdlib.h>

#define NORTH		0       	  //		0
#define SOUTH		2		  //	3		1
#define EAST		1		  //		2				 
#define WEST		3

//starting and ending squares (changed from 7 and 1 to 6 and 0 instead to make array stuff easier)
#define startRow	6              //this is numbered like the maze will be, with a 1 in the upper left
#define startCol	6  	       //and 49 in the lower right. I decided to count from 0 though, its just
#define endRow		0  	       //easier
#define endCol		0  

#define WALL		6  //largest dist to wall in inches
#define SQUARE		12 //distance from one square to the next in inches

struct ryans{
	int noWall[4]; //north = 0, east = 1, south = 2, west = 3

	char newSquare; //first time entering new square, so use sensors to determine wall placement
	                //also used in critical path determination
};

//used for determining critical path. squares are added as the robot finds them, then
//increments path_size. if the robot backtracks to a previously traversed square
// (newSquare == 0) before the critical path is found (path_found == 0), 
//then path_size is decremented, which removes the square from the critical path
int critPathArr[50]; 
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
			ryan[i][x].newSquare = 1;
		}
	}
	for(i = 0; i < 50; i++){
		critPathArr[i] = -1;	
	}
	path_size = -1; // starts at -1 for ease of incrementing in discover() (really starts at 0)
	path_found = 0;
	
}

//used for PART 1 of the maze - searching
void discover(int dir, int row, int col){

	if(row==endRow && col==endCol){ //end square reached, so stop searching for critical path
		//LIGHT A RED LED HERE!
		path_found = 1;
		critPathArr[path_size + 1] = -1; //ensures that the robot stops at the correct number of path moves
						 //since its possible to have a positive number after this in the
						 //case of a long dead end path
	}
	if(!path_found)path_size++; //go to next 
	
	if(ryan[row][col].newSquare == 1){ //robot has not been to this square before, so check for walls
		isWall(row,col);
		ryan[row][col].newSquare = 0;
	}
	
	//moving EAST
	if(ryan[row][col].noWall[EAST] &&  
	(dir != WEST)) { //if you just went WEST (came from the EAST), don't go east til last(backtracking direction)
	        if(!path_found)critPathArr[path_size] = EAST;

		move(EAST);
		discover(EAST,row,col+1);
	}
	
	//moving NORTH
	if(ryan[row][col].noWall[NORTH] && 
	(dir != SOUTH)) {//if you just went SOUTH (came from the NORTH), don't go east (backtracking direction)
		if(!path_found)critPathArr[path_size] = NORTH;
		
		move(NORTH);
		discover(NORTH, row-1,col);
	}
	
	//moving WEST
	if(ryan[row][col].noWall[WEST] && 
	(dir != EAST)) {
		if(!path_found)critPathArr[path_size] = WEST;
		
		move(WEST);
		discover(WEST, row,col-1);	
	}
	
	//moving SOUTH
	if(ryan[row][col].noWall[SOUTH] && 
	(dir != NORTH)) {
		if(!path_found)critPathArr[path_size] = SOUTH;
		
		move(SOUTH);
		discover(SOUTH, row+1,col);
	}
	
	//backtracking if there are no more squares to explore from the current square, 
	//and moves up the recursion stack
	if(!path_found)path_size--; //this square is not part of the critical path since you backtrack from it
				    //before the critical path is found
	move((dir + 2) % 4);
	
}

//used for PART 2 of the maze - critical path
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
	//LIGHT A GREEN LED HERE!
	
	//wait here until a button on the robot is pushed
	
	discover(0, startRow,startCol);

	//wait here until the button is pushed again
	
	takeCriticalPath();


	return 0;
}
