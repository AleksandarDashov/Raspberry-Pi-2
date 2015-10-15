#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
 
/*
 *   A simple game of reflexes
 *   There are 3 LEDs red, green and yellow, there are 2 buttons representing 2 players.
 *   When the red LED blinks the fastest of the 2 players wins.
 *   Wiring link below:
 *   https://www.dropbox.com/s/ruy0fgbd167bo8e/20151015_203254.jpg?dl=0
 */
 
 
int main(void)
{
    wiringPiSetup();
 
    int greenButton = 3;//#22, player1 button
    int yellowButton = 4;//#23, player2 button
 
    int greenLED = 0;//#17, player1 LED
    int yellowLED = 1;//#18, player2 LED
 
    int redLED = 5;//#24, winnerLED
 
    //setting appropriate GPIO pins to input or output
    pinMode(greenLED, OUTPUT);
    pinMode(greenButton, INPUT);
 
    pinMode(yellowLED, OUTPUT);
    pinMode(yellowButton, INPUT);
 
    pinMode(redLED, OUTPUT);
 
    int pressGreen = 0;//player 1 state
    int pressYellow = 0;//player 2 state
 
    while(1)
    {
        pressGreen = 0;
        pressYellow = 0;
        srand(time(NULL));
        int randomDelay = rand() % 10000 + 1000;//random number between 10secs and 1 sec
        printf("%d\n", randomDelay);
        delay(randomDelay);
        digitalWrite(redLED, 1);
        delay(50);
        digitalWrite(redLED, 0);
        
        while(1)
        {
            if(!digitalRead(greenButton))
            {
                pressGreen = 1;
            }
            else if(!digitalRead(yellowButton))
            {
                pressYellow = 1;
            }
            if(pressGreen == 1)
            {
 
                digitalWrite(greenLED, 1);
                delay(1000);
                digitalWrite(greenLED, 0);
                printf("Green wins\n");
                break;
            }
            else if(pressYellow == 1)
            {
                digitalWrite(yellowLED, 1);
                delay(1000);
                digitalWrite(yellowLED, 0);
                printf("Yellow wins\n");
                break;
            }
        }
 
    }
 
 
    return 0;
}
