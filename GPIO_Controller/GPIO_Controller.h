#ifndef GPIO_CONTROLLER_H
#define GPIO_CONTROLLER_H
#include <string>
using namespace std;
 
class GPIO_Controller
{
  public:
        GPIO_Controller();
        GPIO_Controller(int exp_number, string dir_IO, int val_numb);//constructor for setting all values at creation of instance
        int GPIO_Unexport();//will unexport the gpio pin of the instance that calls it
        int GPIO_Export(int number);//export/set, the gpio number
        int GPIO_Direction(string dir);//will set the gpio direction, input or output
        int GPIO_Value(int number);//will set the value ot 1 or 0,LED on or off
        int getGPIO_Value(string& val);//will return the GPIO value, we will mostly use this for input pins
    private:
        int gpio_number;//it will store the number of the gpio pin we are working with
        string direction; // will store the direction, input or output
};
#endif
