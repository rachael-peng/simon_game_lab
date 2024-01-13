/*Author: 			Rachael Peng
 *Date:				  Dec. 1, 2021
 *Purpose:			Code Simon Game on circuit board according to specified rules.
*/



//symbolic constants

#define ON HIGH
#define OFF LOW

#define MAXSEQUENCELENGTH 5

	//led light names and corresponding pin numbers
int const LOSE = 13;
int const WIN = 8;
int const RED = 12;
int const YELLOW = 11;
int const BLUE = 10;
int const GREEN = 9;

	//button names and corresponding pin numbers
int const REDBUTTON = 7;
int const YELLOWBUTTON = 6;
int const BLUEBUTTON = 5;
int const GREENBUTTON = 4;



//function prototypes

	//function to generate a random number and store the value generated
int generateRandom(void) {
  int randomVal;
  
  randomSeed(analogRead(A0));
  randomVal = random(0, 4);
  
  return randomVal;
}

	//function to turn a specified LED light on for 0.5 seconds then 
	//	turn off for 0.25 seconds
void specificledOn(int number){
  if(number == 0) {
    digitalWrite(RED, ON);
    delay(500);
    digitalWrite(RED, OFF);
    delay(250);
  }
  else if(number == 1) {
    digitalWrite(YELLOW, ON);
    delay(500);
    digitalWrite(YELLOW, OFF);
    delay(250);
  }
  else if(number == 2) {
    digitalWrite(BLUE, ON);
    delay(500);
    digitalWrite(BLUE, OFF);
    delay(250);
  }
  else {
    digitalWrite(GREEN, ON); 
    delay(500);
    digitalWrite(GREEN, OFF);
    delay(250);
  }
  return;
}

	//function to check which button is pushed
int checkButtons(void) {
  int buttonPressed = -1;
  
  if(digitalRead(REDBUTTON) == ON) {
    buttonPressed = 0;
  }
  if(digitalRead(YELLOWBUTTON) == ON) {
    buttonPressed = 1;
  }
  if(digitalRead(BLUEBUTTON) == ON) {
    buttonPressed = 2;
  }
  if(digitalRead(GREENBUTTON) == ON) {
    buttonPressed = 3;
  }
  
  return buttonPressed;
}

	//function to turn ON all LED lights
void allOn(void) {
  digitalWrite(RED, ON);
  digitalWrite(YELLOW, ON);
  digitalWrite(BLUE, ON);
  digitalWrite(GREEN, ON);
  digitalWrite(LOSE, ON);
  digitalWrite(WIN, ON);
}

	//function to turn OFF all LED lights
void allOff(void) {
  digitalWrite(RED, OFF);
  digitalWrite(YELLOW, OFF);
  digitalWrite(BLUE, OFF);
  digitalWrite(GREEN, OFF);
  digitalWrite(LOSE, OFF);
  digitalWrite(WIN, OFF);
}

	//delay for 0.25 seconds so light timing doesn't overlap, then
	//  flash "lose" LED 3 times, each flash lasting 0.5 seconds with
	//	0.5 seconds interval in between 
void lose(void) {
  int count = 0;
  delay(250);
  
  while(count < 3) {
    digitalWrite(LOSE, ON);
    delay(500);
    digitalWrite(LOSE, OFF);
    delay(500);
    count++;
  }
  
  return;
}  

	//delay for 0.25 seconds so light timing doesn't overlap, then 
	//  flash "win" LED 3 times, each flash lasting 0.5 seconds with
	//	0.5 seconds interval in between 
void win(void) {
  int count = 0;
  delay(250);
  
  while(count < 3) {
    digitalWrite(WIN, ON);
    delay(500);
    digitalWrite(WIN, OFF);
    delay(500);
    count++;
  }
  
  return;
}  
 


	//initialization
void setup() 
{  
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(LOSE, OUTPUT);
  pinMode(WIN, OUTPUT);
  
  pinMode(REDBUTTON, INPUT);
  pinMode(GREENBUTTON, INPUT);
  pinMode(BLUEBUTTON, INPUT);
  pinMode(YELLOWBUTTON, INPUT);
}



	//running the game
void loop()
{
  //variable declarations
  	//arrays to store sequences
  int sequence[MAXSEQUENCELENGTH];
  int buttonsPressed[MAXSEQUENCELENGTH];
  int ledinSequence;
  int buttonOn = -1;
  boolean gameOver = false;
  boolean waitingforbuttons = false;
  int index = -1;
  int buttonIndex;
  int compareIndex;
  
  	//wait 0.5 seconds before turning on all leds to allow leeway/pause between
  	//	old and new games, then turn on all leds for 1 second to indicate 
  	//	new game is starting, then turn off all leds for 1 second to allow 
 	//	leeway/pause before sequences starts to show
  delay(500);
  allOn();
  delay(1000);
  allOff();
  delay(1000);
  
  	//reset all array values before each game
  sequence[5] = -1;
  buttonsPressed[5] = -1;
  
  //loop runs while user correctly presses right buttons in sequences
  //	up to a max of 5 lights in a sequence
  //use index variable to track how many led lights stored in sequence array at 
  //	the time
  while(gameOver == false && index < (MAXSEQUENCELENGTH - 1)) {
    
    //use ledinSequence variable to index led lights ALREADY in array
    ledinSequence = -1;
    
    //first the leds light up according to light-up sequence already generated,
    //	already tracked in the array (if any)
    while(ledinSequence < index) {
      ledinSequence++;
      //allow for 0.5 seconds delay so led lights timing don't overlap one another 
      delay(500);
      specificledOn(sequence[ledinSequence]);
    }
    
    //a new random led light lights up and is added to the sequence array 
    //	tracking led light-up sequence
    index++;
    sequence[index] = generateRandom();
    //allow for 0.5 seconds delay so led lights timing don't overlap one another
    delay(500);
    specificledOn(sequence[index]);
    
 
    
    //waiting for user to press buttons after light sequence shown
    waitingforbuttons = true;
    buttonIndex = -1;
  
    while(waitingforbuttons == true) {
      //keep on checking the buttons until one is pressed 
      buttonOn = checkButtons();
      
      //if a button is pressed execute the following
      if(buttonOn > -1) {
        //store the sequence of buttons pressed in an array
        buttonIndex++;
        buttonsPressed[buttonIndex] = buttonOn;
        
        //flash the button that is pressed
        specificledOn(buttonOn);
        
        //reset the value of button pressed
        buttonOn = -1;
      }
      
      //keep waiting for user to press buttons until they have pressed as many 
      //	buttons as led lights in sequence 
      if(buttonIndex == index) {
           waitingforbuttons = false;       
      }
   
    }
    
    
    
    //check if the user pressed the correct buttons corresponding 
    //	to the correct led light sequence
    compareIndex = 0;
    
    while(compareIndex <= index) {
      
      //each value in sequence array is compared to buttons pressed array.
      //	if all led light-up sequence matches all buttons pressed by the user, 
      //	the game continues
      if(sequence[compareIndex] == buttonsPressed[compareIndex]) {
        compareIndex++;
      }
      
      //if any buttons pressed do not match the led light-up sequence, 
      //	the game ends
      else {
        gameOver = true;
        compareIndex = (index + 1);
      }
      
    }
    
  }
  
  //if the user correctly pressed all the buttons for all 5 led lights
  //	in sequence, the user wins the game! the win light flashes 3 times
  if(gameOver == false) {
    win();
  }
  
  //if not the user loses the game and the lose light flashes 3 times
  else {
    lose();
  } 
  
  //end of game
}
    