//TODO: Green LED light at the beginning square, Red LED light at the end square
//	Change maze_size variable using switch/button
//	just added errorCorrection() function for after turns maybe? its not finished so ya
//	sensors, PSoC communication......
// Need to switch over to cm

#include<stdio.h>
#include<stdlib.h>

#include "ROBOTlib.h" //this brings in the functions fwd(distance,speed), bwd(distance,speed), 
		     //right(), and left(), cw(degrees, velocity),ccw(float,velocity),turnLED(1=On, 0=Off), 
		     //test_drive(), test_sonar()
#include "srf02.h" //brings in get_srf02_range_f(), brings in get_srf02_range_r(), brings in get_srf02_range_l()
#define NORTH		0       	  //		0
#define SOUTH		2		  //	3		1
#define EAST		1		  //		2				 
#define WEST		3

#define ROBOT_SPEED     8.0 //in inches per second. used in moveForward() to move to the next square
#define TURN_SPEED      4.0 //in inches per second. this is only used for turnAround(), because the default
			    //for 90 degree turns right and left in the PSoC is 4 inches per second
#define WALL		6  //largest dist to wall in inches, need to change to cm 
#define SQUARE		12.0 //distance from one square to the next in inches
#define SQUARE_CM	30.48
 
//prototypin'
void init();
void discover(int dir, int row, int col);
void takeCriticalPath(void);

int move(int direction);
void turnCW(void);
void turnCCW(void);
void turnAround(void);
void moveForward(void);
void errorCorrection(void);

void isWall(int row, int col);
int getDistFront(void);
int getDistLeft(void);
int getDistRight(void);


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

//we need to change this with a switch/button
int maze_size = 5; //5, 6, or 7 for length and width of the maze. This number determines 
		   //the starting and ending squares

//this is numbered like the maze will be, with a 1 in the upper left
//and 49 in the lower right. Its like in Windows people!
//I decided to count from 0 though, its just easier
struct ryans ryan[7][7];
int endRow; //these change depending on the maze_size
int endCol; 
int startRow = 6; //always starts in the same row
int startCol;


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
	
	
	if(maze_size == 5){ //starts in square 48(outside the maze), ends in square 9.
		startCol = 5;
		endRow = 1;
		endCol = 1;
	}else if(maze_size == 6){ //starts in square 48(outside the maze), ends in square 1
		startCol = 5;
		endRow = 0;
		endCol = 0;
	}else{ //size = 7, so starts in square 49 and ends in square 1
		startCol = 6;
		endRow = 0;
		endCol = 0;
	}
}

//used for PART 1 of the maze - searching
void discover(int dir, int row, int col){
	/***probably need a routine here to correct the robot's ENGEL to be parallel with the walls***/


	if(row==endRow && col==endCol){ //end square reached, so stop searching for critical path
		//LIGHT A RED LED HERE!
		
		path_found = 1;
		critPathArr[path_size + 1] = -1; //ensures that the robot stops at the correct number of path moves
						 //since its possible to have a positive number after this in the
						 //case of a long dead end path
	}
	if(!path_found)path_size++; //go to next critical path square
	
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
	(dir != SOUTH)) {//if you just went SOUTH (came from the NORTH), don't go north (backtracking direction)
		if(!path_found)critPathArr[path_size] = NORTH;
		
		move(NORTH);
		discover(NORTH, row-1,col);
	}
	
	//moving WEST
	if(ryan[row][col].noWall[WEST] && 
	(dir != EAST)) {//if you just went EAST(came from the WEST), don't go west(backtracking direction)
		if(!path_found)critPathArr[path_size] = WEST;
		
		move(WEST);
		discover(WEST, row,col-1);	
	}
	
	//moving SOUTH
	if(ryan[row][col].noWall[SOUTH] && 
	(dir != NORTH)) {//if you just went NORTH (came from the SOUTH), don't go south (backtracking direction)
		if(!path_found)critPathArr[path_size] = SOUTH;
		
		move(SOUTH);
		discover(SOUTH, row+1,col);
	}
	
	//backtracking if there are no more squares to explore from the current square, 
	//and moves up the recursion stack
	if(!path_found)path_size--; //this square is not part of the critical path since you backtrack from it
				    //before the critical path is found
	if(row!=startRow || col!=startCol){ //prevents backtracking from the initial square, which would make the 
					   // robot go south into a wall
		move((dir + 2) % 4);
	}
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
	right(); //this is from ROBOTlib.c
	facing = (facing+1) % 4;
}

//turn 90 degrees left, then update facing variable 
void turnCCW(void){
	left(); //this is from ROBOTlib.c
	facing = (facing-1) % 4;
}

//turn 180 degrees, then update facing variable
void turnAround(void){
	cw(180.0,TURN_SPEED); //from ROBOTlib.c
	facing = (facing+2) % 4;
}

void moveForward(void){ //move 12 inches forward
	fwd(SQUARE, ROBOT_SPEED);
}

void errorCorrection(void){
	int leftWall;
	int rightWall;
	rightWall = getDistRight();
	leftWall = getDistLeft();
	
	if(rightWall < leftWall){ //rotate a bit left
		
	}
	else if(leftWall < rightWall){ //rotate a bit right
		
	}
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

//uses sensor on the front of the robot and returns distance to nearest wall (in inches??)
//direction = facing
int getDistFront(void){
	return get_srf02_range_f(); //this function from srf02.h
}
//uses sensor on the robot's left side and returns distance to nearest wall
//direction = (facing - 1) % 4
int getDistLeft(void){
       	return	get_srf02_range_l(); //this function from srf02.h
}
//uses sensor on the robot's right side and returns distance to nearest wall
//direction = (facing + 1) % 4
int getDistRight(void){
	return get_srf02_range_r; //this function from srf02.h
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
