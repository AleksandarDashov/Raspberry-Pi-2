#include "GPIO_Controller.h"
#include <iostream>
#include <string>
int main()
{
    //just something to test, LED flashes when button is clicked
    string gpio22value;
    GPIO_Controller gpio4(4, "out", 0);
    GPIO_Controller gpio22;
    gpio22.GPIO_Export(22);
    gpio22.GPIO_Direction("in");
    while(1)
    {
 
 
        if(gpio22.getGPIO_Value(gpio22value) == 0)
        {
            gpio4.GPIO_Value(1);
 
        }
        else
        {
            gpio4.GPIO_Value(0);
        }
 
    }
 
    cout << "Hello world" << endl;
 
    return 0;
}
