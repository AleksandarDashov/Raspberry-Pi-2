#include <wiringPi.h>
#include <stdio.h>
 
/*
 *   Simple program for blinking LEDs with buttons.
 *   There are 3 LEDs red, green and yellow, there are 3 buttons also, one for every LED.
 *   When a button is pressed the corresponding LED lights.
 *   Push all 3 buttons to terminate the program.
 *
 *   Here is the wiring for this program:
 *   https://www.dropbox.com/s/sdpwc0ewuob7og6/buttonsLED.jpg?dl=0
 *   
 *   I used 220Ω (ohms) resistors for the LEDs and 10 000Ω resistors for the buttons.
 */
 
 
int main(void)
{
 
    int redLED = 0;//#17 pin
    int redButton = 3;// #22 pin
 
    int greenLED = 1;//#18 pin
    int greenButton = 2;//#27 pin
 
    int yellowLED = 4; //#23
    int yellowButton = 5;//#24
 
 
    wiringPiSetup();
 
    pinMode(redLED, OUTPUT);//setting GPIO pin #17 to output
    pinMode(redButton, INPUT);//setting GPIO pin #22 to input
 
    pinMode(greenLED, OUTPUT);//setting GPIO pin #18 to output
    pinMode(greenButton, INPUT);//setting GPIO pin #27 to input
 
    pinMode(yellowLED, OUTPUT);//setting GPIO #23 to output
    pinMode(yellowButton, INPUT);//setting GPIO #24 to input
 
    int pressRed = 0;//identifier that the red button is pressed
    int pressGreen = 0;//-//- the green button is pressed
    int pressYellow = 0;
 
    printf("Press all buttons to exit\n");
    while(1)
    {
        pressRed = 0;
        pressGreen = 0;
        pressYellow = 0;
        if(!digitalRead(redButton))
        {
 
            pressRed = 1;
 
        }
        else if(!digitalRead(greenButton))
        {
 
            pressGreen = 1;
        }
 
        else if(!digitalRead(yellowButton))
        {
            pressYellow = 1;
        }
 
        if(pressRed == 1)
        {
            printf("Red\n");
            digitalWrite(redLED, 1);
            delay(100);
            digitalWrite(redLED, 0);
 
        }
        else if(pressGreen == 1)
        {
            printf("Green\n");
            digitalWrite(greenLED, 1);
            delay(100);
            digitalWrite(greenLED, 0);
 
        }
        else if(pressYellow == 1)
        {
            printf("Yellow\n");
            digitalWrite(yellowLED, 1);
            delay(100);
            digitalWrite(yellowLED, 0);
        }
 
        if(!digitalRead(redButton) && !digitalRead(greenButton) && !digitalRead(yellowButton))
        {
            printf("Terminating!\n");
            break;
        }
    }
    return 0;
}
