//**************************************************************
//River Schenck and Chad Gardner
//Creation Date: 4/8/18
//Description-
//This a the pong game. There are 2 paddles that can change length.
//Switches 1&2 control paddle1, and 3&4 control paddle2.
//Switch 5 controls soccer mode and switches 7&8 control ball speed.
//The left button is the start button and right button is reset button.
//The game will play until a player reaches the score of 11
		//-----------------
		//Revision History
		//-----------------
		//4/08/18 - starting the code
		//4/14/18 - got the pots to work, and the bounds and net is in
		//4/15/18 - paddles and paddle sizes are working
		//4/16/18 - fixing paddle sizes, started ball function
		//4/20/18 - fixing errors
		//4/21/18 - display score and winner function added
		//4/22/18 - sounds are working and adding soccer mode
		//4/23/18 - better readability and reducing redundant code


	

//-------------------------------------------------------------
//variables
//-------------------------------------------------------------	 
unsigned char score1=0, score2=0;		//START SCORE AT 0
char paddle1, paddle2;
char run = 1;				//SET  BALL ANGLE AT 1/1			   
char rise = 1;	   	   				   
char show_score = 0;		//SET LOOP FOR SHOWING SCORE TO 0
char bottom, top, height;
int paddle1height, paddle1top=20, paddle1bottom;
int paddle2height, paddle2top=20, paddle2bottom;
int ball_x, ball_y, ball_left, ball_right, ball_top, ball_bottom;
int soccer;
char switches, ballspeed = 0;
char pause =0;




//-------------------------------------------------------------
//functions
//-------------------------------------------------------------	
void move_comps(void);
void init_game (void);
void onemillidelay(unsigned int); 
unsigned char put_char(unsigned char, unsigned char, char);
void get_switches(void);
void init_text(void); 
void speed();
void sound(int);
void winner(void);
void soccermode(void);
void scores (char, char);
void draw_paddle (int, int, char);
void draw_ball (int, int);

//-------------------------------------------------------------
//SERVES RANDOM
//-------------------------------------------------------------	
char random[] = {-2, -1, 1, 2};
char serve[] = {110, 8, 110, 8};
char maskit[] = {0x1fl, 0xffl, 0xfffl, 0xffffl,  0xffffl};
char group[] = {8, 12, 16, 60, 2, 54 ,50, 46, 1, 2 ,3};
char count=1;

//-------------------------------------------------------------
//SOUND
//-------------------------------------------------------------
code unsigned char sine[] = { 176, 217, 244, 254, 244, 217, 176, 128, 80, 39, 12, 2, 12, 39, 80, 128 };
unsigned char phase = sizeof(sine)-1;
unsigned int duration;






