#include <C8051F020.h>
#include <stdlib.h>
#include <lcd.h>
#include <pongheader.h>



sbit BTN1 = P2^2;			//BTN1 FOR START GAME
sbit BTN2 = P2^3;
unsigned char LED18;
sbit LED9 = P2^4;
sbit LED10 = P2^5;


void main(void)
{

//----------------------------------
//Init
//-----------------------------------
	WDTCN = 0xde;// disable watch dog
	WDTCN = 0xad;
	XBR2 = 0x40; // enable port output
	XBR0 = 4;
	OSCXCN = 0x67; // TURN ON EXTERNAL CRYSTAL
	TMOD = 0x22; // wait 1ms using t1 mode2
	TH1 = -167; // 2MHZ CLOCK, 167 COUNTS - 1MS
	TR1 = 1;
	while (TF1 == 0) {} //wait 1ms
	while (!(OSCXCN & 0x80) ) {} //wait till oscillator stable
	OSCICN = 8; // switch over to 22.1184mhz
	SCON0 = 0x50; // 8-BIT, VARIABLE BAUD, RECEIVE ENABLE
	TH1 = -6;  // 9600 BAUD
	REF0CN = 0x07; // ref0 voltage
	ADC0CF = 0xf4; // GAIN
	ADC0CN = 0x80; //ENALBE AD
	IE = 0xA2;
	IP = 0x20;
	DAC0CN = 0x9C;
	T2CON = 0x00;
	TH1 = 0; //set timer 1 for a two bit timer
	TH0 = 0;
	TR0 = 1;
	TMR3CN = 4; // turn on timer 3 for ball speed


//----------------------	
//initializing game
//----------------------			
		init_lcd();
		blank_screen();
		init_text();
		get_switches();
		init_game();
		scores(score1, score2);

//----------------------	
//Playing game
//----------------------
	while(BTN2 == 1) {


		//SHOW SCORE LOOP. IE AFTER A POINT IS SCORED
		while(show_score == 0) {
				scores(score1, score2);
				onemillidelay(2000);//DELAY TO SHOW THE SCORE OF THE GAME
				get_switches();
				draw_ball(ball_x, ball_y);
				show_score = 1;
			}
			//PLAYING GAME LOOP
			while(show_score == 1 && BTN2 == 1)
			{

					//CHECKS TO SEE IF SOMEONE HAS WON
					if(score1 < 11 && score2 < 11) {

					blank_screen();				
					init_game();			
					get_switches(); 
					move_comps();
					draw_ball(ball_x, ball_y);
						while ((TMR3CN & 0x80) ==0)
						{
						}
						TMR3CN &= 0x7F;
						speed();
						refresh_screen();						
						
					}									 
					else if(score1 == 11 || score2 == 11)
					{
						winner();
						return;
					}//end else if

				
	
			}//end while
	
	}//end for

}//end main




//==============================================================================
//TIMER_0
//Variables: count, random[], serve[], rise, run, ball_x, ball_y
//Function Calls: none
//picks a random number from serve[] & random[] for random serve and random angle
//==============================================================================

void timer_0(void) interrupt 1
{
	if(count < 3)//parses through 4 possible
	{	
		rise = random[count];
		ball_x = serve[count];
		count++;			
	}
	else if(count == 3)
	{
		rise = random[count];
		count = 0;			
	}


	if(ball_x ==110) { //if random picks 110 make ball move left
		run = -1;
	}
	else{
		run = 1;//if randm picks 8 make ball move right
	}
	ball_y = 30;//serve from 30 y coordinate

}


//==============================================================================
//TIMER2
//Variables: phase, sine[], duration, TR2
//Function Calls: none
//this function is the sound interrupt
//==============================================================================
void timer2(void) interrupt 5
{

		TF2 = 0;
		DAC0H = sine[phase];
		if ( phase < sizeof(sine)-1 )	// if mid-cycle
		{				// complete it
			phase++;
		}
		else if ( duration > 0 )	// if more cycles left to go
		{				// start a new cycle
			phase = 0;
			duration--;
		}
		if ( duration == 0)
		{
			TR2 = 0;
		}
	
}



//==============================================================================
//ONEMILLIDELAY
//Variables: i, j
//Function Calls: none
//delay of i time, from the book. ie(int time = 2000 -> 2 second delay)
//==============================================================================
void onemillidelay(unsigned int time) 
{
unsigned int i;	
unsigned int j;	
	for(i=0; i<time; i++){ //looping to create a delay
		for(j=0; j<1843; j++){
		}
	}
}


//==============================================================================
//PUT_CHAR
//Variables:
//Function Calls: none
//inserts characters onto the lcd screen using lcd.h and lcd_driver.asm
//==============================================================================
unsigned char put_char (unsigned char row, unsigned char col, char ch) {
		int loc = row * 128 + col; 
		int font = (ch - ' ') * 5; //*5 because each letter has 5 hex values
		char i;
		for (i=0; i<5; i++) //print 5 hex values
		{
			screen[loc +i] = font5x8[font+i];
		}
		return (0);
}



//==============================================================================
//INIT_GAME
//Variable: i, soccer
//Function Calls: soccermode(),
//draws the bounds for the game and decides if it is in soccer mode or not (switch 5)
//==============================================================================
void init_game () {

		unsigned int i;
		//TOP BOUNDARY
		for(i=0; i<127; i++) {
			screen[i] = 0x03; 
		}
		//BOTTOM BOUNDARY
		for(i=0; i<127; i++) {
			screen[896+i] = 0xC0; 
		}

		//-----------------------
		//SOCCER MODE
		//-----------------------
		if (soccer == 0) {
			soccermode();


		}
		else {
			//NET
			screen[63] = 0xCF;
			screen[64] = 0xCF;
			for(i=191; i<960; i++) {
				screen[i+1] = 0xCC;
				screen[i] = 0xCC; 
				i= i+127;
			}

		}
}



//==============================================================================
//INIT_TEXT
//Variable:
//Funcation Calls: put_char(), blank_screen(), refresh_screen(), get_switches()
//Prints HELLO PRESS START TO PLAY! and waits for user to press start
//==============================================================================
void init_text(void) {

		put_char(1,0, 'H'); put_char(1,6, 'E'); put_char(1,12,'L');	put_char(1,18,'L'); //displays HELLO
		put_char(1,24,'O'); put_char(1,32,' ');

		put_char(1,38,'P'); put_char(1,44,'R'); put_char(1,50,'E'); put_char(1,56,'S');	//displays PRESS
		put_char(1,62,'S'); put_char(1,68,' '); 
	
		put_char(1,74,'S'); put_char(1,80,'T'); put_char(1,84,'A'); put_char(1,90,'R');	//displays START TO
		put_char(1,96,'T'); put_char(1,102,' '); put_char(1,108,'T'); put_char(1,114,'O'); 

		put_char(3,52, 'P'); put_char(3,58, 'L'); put_char(3,64, 'A'); put_char(3,70, 'Y'); //PLAY!
		put_char(3,76,'!');	put_char(6,30,' ');	put_char(6,36,' '); put_char(6,42,' ');

		refresh_screen();
		while(BTN1) {		//waits until BTN1 is pressed to continue

		}
		blank_screen();//reset screen


		TR0 = 0;
		get_switches();	
}



//==============================================================================
//GET_SWITCHES
//Variables: switches, paddle1, paddle1top, paddle1height, paddleheight,
//paddle2, paddle2top, paddle2height
//gets the input of the switches and passes them to the paddle function
//==============================================================================
void get_switches(){

			switches = ~P3;							//inverts P3 so all 0
			switches &= 0x03;						//ANDING paddle1 with 0x03 to mask
			paddle1 = switches;						//all switches except 1 and 2
			draw_paddle (0, paddle1top, paddle1); 	
			paddle1height = height;
		
			switches = ~P3;							//inverts p3 so all 0
			switches &= 0x0c;						//ANDING paddle2 with 0x0C to mask
			paddle2 = switches >> 2;				//all switches excpet 3 and 4. Shifts 2 to right to get number	
			draw_paddle (126, paddle2top, paddle2); // or else it would be 00001100 and we would get wrong values
			paddle2height = height;						


			soccer = ~P3;							//inverts p3 so all 0
			soccer &= 0x10; 						//ANDING soccer with 0x10 to mask
			soccer = soccer >> 4;					//all switches expect 5. Shifting 4 to right to get number

}



//==============================================================================
//SOUND
//Variables: sound, RCAP2H, RCAP2L, TR2, duration
//Function Calls: none
//produces a noise based on where the ball hits. We pass a number into this 
//function and it will produce a noise depending on what number is passed in.
//==============================================================================
void sound(int sound){

	if(sound == 1) {				//sound one is played at 600Hz
		RCAP2H = (-307)>>8; //high
		RCAP2L = -307; 		//low
		TR2 = 1; 
		duration = 60;
	}
	
	if(sound == 2) {				//sound two is played at 800Hz
		RCAP2H = (-230)>>8; //high
		RCAP2L = -230; 		//low
		TR2 = 1; 
		duration = 80;
	}

	if(sound ==3) {					//sound three is played at 400Hz
		RCAP2H = (-460)>>8; //high
		RCAP2L = -460; 		//low
		TR2 = 1; 
		duration = 40;
	}
}



//==============================================================================
//MOVE_COMPS
//Variables: bally, ballx, left_hit1, left_hit2, left_hit3, left_hit4, right_hit1
//right_hit2, right_hit3, right_hit4, paddle1height, paddle1top, paddle2height
//paddle2top
//gets input from the pots and moves the ball
//==============================================================================
void move_comps(void) {

	//BALL COORDINATES
	char bally, ballx;
	//LEFT 4 PADDLE HIT SPOTS
	char left_hit1 = (paddle1height/4)+paddle1top;
	char left_hit2 = (paddle1height/4*2)+paddle1top;
	char left_hit3 = (paddle1height/4*3)+paddle1top;
	char left_hit4 = paddle1height+paddle1top;
	//RIGHT 4 PADDLE HIT SPOTS
	char right_hit1 = (paddle2height/4)+paddle2top;
	char right_hit2 = (paddle2height/4*2)+paddle2top;
	char right_hit3 = (paddle2height/4*3)+paddle2top;
	char right_hit4 = paddle2height+paddle2top;

	//-------------------
	//GET POT INPUT
	//-------------------
	AMX0SL = 0x00;  //Select ain0
	onemillidelay(4);
	ADC0CF = 0x40;
	AD0INT = 0; 	//clear flag
	AD0BUSY = 1; 			
	while(AD0INT == 0){}	
	paddle1top = ADC0H*4; //so we get value out of 64	
								
	AMX0SL = 0x01; //select ain1	
	onemillidelay(4);
	ADC0CF = 0x40;
	AD0INT = 0;    //clear flag
	AD0BUSY = 1; 
	while(AD0INT == 0) 
	{}
	paddle2top = ADC0H*4;//so we get value out of 64	
	

		//---------------------
		//MOVE THE BALL
		//---------------------
		ball_x += run;		
		ball_y += rise;	
		
		ballx = ball_x+2; //so we know where the edges of the ball are
		bally = ball_y+2; //ball_x and ball_y are the center of the ball

		//----------------------------------	  
		//HITTING TOP OR BOTTOM
		//----------------------------------
		if (bally < 2 || bally > 61) {
		rise = -rise; 
		sound(1); //INVERT TO MOVE OPPOSITE
		}  
		else{ 
		rise = rise;
		}	
						
	//---------------------------------		  
	//LEFT PADDLE
	//---------------------------------
	if (ballx < 3){ 

		//-----FULL PADDLE-----
		if (paddle1 == 3){
		run = -run; //INVERT DIRECTION AUTOMATICALLY BECAUSE NO MATTER WHERE IT HITS IT WILL BOUNCE BACK
		sound(1); 
		} 
		//-----TOP QUAD-----									
		else if (bally >= paddle1top && bally <= left_hit1) {
		rise = -2; //STEEP ANGLE UP
		run = 1; 
		sound(2);
		}
		//-----THIRD QUAD-----
		else if (bally >= left_hit1 && bally <= left_hit2){
		rise = -1; //SHALLOW ANGLE UP
		run = 2; 
		sound(2);
		}
		//-----SECOND QUAD-----
		else if (bally >= left_hit2 && bally <= left_hit3){
		rise = 1; //SHALLOW ANGLE DOWN
		run = 2; 
		sound(2);
		}
		//-----FIRST QUAD-----
		else if (bally >= left_hit3 && bally <= left_hit4){
		rise = 2; //STEEP ANGLE DOWN
		run = 1; 
		sound(2);
		}
		//-----MISS-----
		else {
		score2++; 			//add 1 to score
		show_score = 0; 	//score loop is 0 so it shows score
		ball_x = 110; 		//set serve location
		ball_y = 30;		//set serve location
		run = -1; 
		rise = 1; 
		//---LED BLINKING---
			P1 = 0x00;
			LED9 = 0;
			LED10 = 0;
			sound(3);
			onemillidelay(500);
			P1 = 0xFF;
			LED9 = 1;
			LED10 = 1;
		}//end else
	refresh_screen();
	}//end if 

	//---------------------------------		  
	//RIGHT PADDLE
	//---------------------------------
	if (ballx > 123)
	{	
		//-----FULL PADDLE-----
		if (paddle2 == 3){
		run = -run; //INVERT DIRECTION AUTOMATICALLY BECAUSE NO MATTER WHERE IT HITS IT WILL BOUNCE BACK
		sound(1);
		}
		//-----TOP QUAD-----
		else if (bally >= paddle2top && bally <= right_hit1){
		rise = -2; //STEEP ANGLE UP
		run = -1; 
		sound(2);
		}
		//-----THIRD QUAD-----
		else if (bally >= right_hit1 && bally <= right_hit2) {
		rise = -1; //SHALLOW ANGLE UP
		run = -2; 
		sound(2);
		}
		//-----SECOND QUAD-----
		else if (bally >= right_hit2 && bally <= right_hit3){
		rise = 1; //SHALLOW ANGLE DOWN
		run = -2; 
		sound(2);
		}
		//-----FULL PADDLE-----
		else if (bally >= right_hit3 && bally <= right_hit4){
		rise = 2; 
		run = -1;
		sound(2);
		} 
		//-----MISS-----
		else {
		score1++; 			//add 1 to score
		show_score = 0; 	//set score loop to 0 to show score
		ball_x = 8; 		//set serve location
		ball_y = 30;		//set serve location
		run = 1; 
		rise = 1; 
		//---LED BLINKING---
			P1 = 0x00;
			LED9 = 0;
			LED10 = 0;
			sound(3);
			onemillidelay(500);
			P1 = 0xFF;
			LED9 = 1;
			LED10 = 1;
		}
	}

	
}


//==============================================================================
//DISPLAY SCORES
//Variables: font, font2, soccer
//Funcation Calls: soccermode, put_char
//This function displays the scores. It multiplies the score by 5 so we get the 
//correct values from the screen[] table. It also checks if soccermode is enabled
//and will display P1 and P2 with score if not, USA and ITALY and score if enabled
//==============================================================================
void scores (char score1, char score2) 
{

		int font = (score1)*5;
		int font2;
		char i;
			if(score1 >=10){				//this is for the first digit in the score '0'1
				font2 = 5;					//we dont have double digit numbers in the font5x8
				font = (((font/5)-10)*5);	//so we make a new variable, font2 and make it display 1 if over 10
			}								//then we make the font number reset 
			else {
				font2 = 0;
			}


				for (i=0; i<5; i++)
				{
					screen[304+i] = score5x8[font+i];
					screen[298+i] = score5x8[font2+i];
						//FOR SOCCER MODE
						if(soccer==1) {			
							put_char(1,42,'P');			//Displays p1 if not soccer mode
							put_char(1,48,'1');
						}
						else if(soccer==0){
							put_char(1,42, 'U');		//Displays USA if sovvermode is enabled
							put_char(1,48, 'S');
							put_char(1,54, 'A');
						}
				}
		font = (score2)*5;
			if(score2 >=10){					//this is for the first digit in the score '0'1
				font2 = 5;						//we dont have double digit numbers in the font5x8
				font = (((font/5)-10)*5);		//so we make a new variable, font2 and make it display 1 if over 10
			}									//then we make the font number reset 
			else {
				font2 = 0;
			}
				for (i=0; i<5; i++)
				{
					screen[336+i] = score5x8[font+i];
					screen[330+i] = score5x8[font2+i];
						//FOR SOCCER MODE
						if(soccer==1) {				//display P2 if not soccermode
							put_char(1,74,'P');
							put_char(1,80,'2');
						}
						else if(soccer==0){
							put_char(1,70, 'I');	//display ITALY if soccermode is enabled
							put_char(1,76, 'T');
							put_char(1,82, 'A');
							put_char(1,88, 'L');
							put_char(1,94, 'Y');
						}

				}//end for
		refresh_screen();		
}




//==============================================================================
//WINNER
//Variables: none
//Function Calls: blank_screen, init_game, scores, put_char, refresh_screen
//onemillidelay
//This is the winning function. It is called when a player reaches the score of
//11. It will display WINNER then blink the LEDS.
//==============================================================================
void winner(void) {
			int i;
			blank_screen();
			init_game();
			scores(score1, score2);	//Display the score to see what end result was
			put_char(4,48, 'W'); put_char(4,54, 'I'); put_char(4,60,'N');	put_char(4,66,'N'); //displays WINNER
			put_char(4,72,'E'); put_char(4,78,'R'); put_char(4,84, '!');
			refresh_screen();
			

			for(i=0;i<5;i++) {
			//---LED BLINKING---
				P1 = 0x00;
				LED9 = 0;
				LED10 = 0;
				sound(3);
				onemillidelay(500);
				P1 = 0xFF;
				LED9 = 1;
				LED10 = 1;

			}
			onemillidelay(2000);
}



//==============================================================================
//SOCCERMODE
//Variables: i
//Function Calls:
//Draws a soccer field on the lcd screen.
//==============================================================================
void soccermode(void) {
	int i; 

	for(i=63; i<960; i++) {
		screen[i+1] = 0xFF; 
		i= i+127;
	}

	//LEFT GOAL
	for(i=256; i<276; i++) {
		screen[i] = 0x1;
	}
	for(i=768; i<788; i++) {
		screen[i] = 0x1;
	}
	for(i=275; i<768; i++) {
		screen[i] = 0xFF;
		i = i+127;
	}

	//RIGHT GOAL
	for(i=363; i<383; i++) {
		screen[i] = 0x1;
	}
	for(i=875; i<895; i++) {
		screen[i] = 0x1;
	}
	for(i=363; i<875; i++) {
		screen[i] = 0xFF;
		i = i+127;
	}

	//CENTER 
	for(i=449;i<459;i++){
	screen[i] = 0x01;
	screen[i+256] = 0x01;
	}
	for(i=438;i<448;i++){
	screen[i] = 0x01;
	screen[i+256] = 0x01;
	}
	for(i=438;i<689;i++){
	screen[i] = 0xFF;
	screen[i+20] = 0xFF;
	i=i+127;
	}

}


//==============================================================================
//SPEED
//Variables: ballspeed, TMR3RLH
//Function Calls: 
//gets input from the pots and moves the ball. Uses timer 2.
//==============================================================================
void speed() {

		
		ballspeed = ~P3;
		ballspeed &= 0xc0;			//ANDs ballspeed and 0xc0 so we can see switches 7 and 8
		ballspeed = ballspeed >> 6; //shift so we get accurate numbers
		

		if(ballspeed==-01) {
		//sound(1);
			TMR3RLH = (-100000)>>8; //100 pixels per second
			TMR3RLL = -100000;
		}
		else if(ballspeed==-02) {
		//sound(2);
			TMR3RLH = (-50000)>>8;	//slow speed
			TMR3RLL = -50000;
		}
		else if(ballspeed==00) {
		//sound(3);
			TMR3RLH = (-12288)>>8;	//150 pixels per second
			TMR3RLL = -12288;
		}
		else if(ballspeed==01) {
		//sound(1);
			TMR3RLH = (-9216)>>8;	//200 pixels per second
			TMR3RLL = -9216;
		}
	
}

//==============================================================================
//DRAW_PADDLE
//Variables: mask, tmp, side, height, paddle, bottom, top
//Function Calls: none
//Draws the paddles. Finds if the paddle is 8, 12, 16, or 64 pixels. Then makes 
//sure paddle stays in screen limit. Then ORs screen with mask.
//==============================================================================
void draw_paddle (int side, int tmp, char paddle) {

		long mask;
		if(paddle == 0) {
			if(tmp>54) {
				 tmp = group[5];
			}							//paddle = 0 makes the paddle 8 pixels
			mask = maskit[1];  			 
			height = group[0];	
		}
		else if(paddle == group[8]) {
			if(tmp>group[6]){
				tmp = group[6];	
			}							//paddle = 1 makes the paddle 12 pixels
			mask = maskit[2];			
			height = group[1];	
		}
		else if(paddle == group[9]) {	
			if(tmp > group[7]) {
				tmp= group[7];
			}							//paddle = 2 makes the paddle 16 pixels
			mask = maskit[3]; 			 
			height = group[2];	
		}
		else if(paddle == group[10]) {
			while(side<1024)				//paddle = 3 makes the paddle 64 pixels
			{							
			   	screen[side] = maskit[1];		//we need to make the paddle 64 so this loop displays it
			    screen[side+1] = maskit[1];
				side = side+128;
			}				
							//top is 2 pixels because the wall is above it
				bottom = group[3];	//
				height = group[3];
				top = group[3];	
			}
		
	if (tmp<group[4]) {
		tmp =group[4];
	}								
	mask = mask <<(tmp%8);			
	side += (tmp/8)*128;	
	top = tmp;
	bottom = tmp+height-1;					//apply the mask on the screen
	screen[side] |= mask;					//modulus 8 to select correct page
	screen[side+1] |= mask;					
	screen[side+128] |= mask >> group[0];		
	screen[side+129] |= mask >> group[0];	
	screen[side+256] |= mask >> group[2];	
	screen[side+257] |= mask >> group[2];
}


//==============================================================================
//draw_ball
//Variables: mask, i, j
//Function Calls: 
//Masks the ball and draws it on the screen
//==============================================================================
void draw_ball (int side, int tmp)
{
	long mask = maskit[0] << (tmp%8); 		//make a 5 pixel tall mask 					  
	int j = (tmp/group[0])*128+side; 		//divide by 8 then add side to get in right place vertically					
	int i;

	for (i = 0; i<5; ++i) //this loop to make it 5 pixels wise
	{ 
		screen[j] |= mask; 
		screen[j+128] |= mask >>group[0];
		j++; 
	}
}



