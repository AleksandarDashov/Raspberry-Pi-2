#include "GPIO_Controller.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
 
GPIO_Controller::GPIO_Controller()
{
    gpio_number = 0;
    direction = "none";
}
GPIO_Controller::GPIO_Controller(int exp_number, string dir_IO, int val_numb)
{
    this->GPIO_Export(exp_number);
    this->gpio_number = exp_number;
 
    this->GPIO_Direction(dir_IO);
    this->direction = dir_IO;
 
    this->GPIO_Value(val_numb);
 
}
 
 
//GPIO Export will activate a GPIO pin number
int GPIO_Controller::GPIO_Export(int number)
{
    gpio_number = number;
    ofstream gpio_export_location ("/sys/class/gpio/export"); //the location of sys gpio pins
    if(gpio_export_location.is_open())//if there is no error while opening it
    {
        gpio_export_location << number;
        gpio_export_location.close();
 
    }
    else
    {
        cout << "Error while opening file" << endl;
        return -1;
    }
    return 1;
 
}
 
//GPIO Unexport will deactivate a GPIO pin number
int GPIO_Controller::GPIO_Unexport()
{
    ofstream gpio_unexport_location ("/sys/class/gpio/unexport"); //the location of sys gpio pins
    if(gpio_unexport_location.is_open())//if there is no error while opening it
    {
        gpio_unexport_location << this->gpio_number;
        gpio_unexport_location.close();
        return this->gpio_number;
 
    }
    else
    {
        cout << "Error while opening file" << endl;
        return -1;
    }
    return 1;
 
}
 
 
int GPIO_Controller::GPIO_Direction(string dir)
{
    direction = dir;
    //converting int to string using string stream
    stringstream ss;
    ss << this->gpio_number;
    string numb_string = ss.str();
 
    string gpio_direction = "/sys/class/gpio/gpio" + numb_string + "/direction";
 
 
 
    ofstream gpio_IO(gpio_direction.c_str());
    if(gpio_IO.is_open())
    {
        gpio_IO << dir;
        gpio_IO.close();
    }
    else
    {
        cout << "Error while opening file" << endl;
        return -1;
    }
    return 1;
 
}
 
 
int GPIO_Controller::GPIO_Value(int number)
{
    stringstream ss;
    ss << this->gpio_number;
    string numb_string = ss.str();
 
 
    string gpio_direction = "/sys/class/gpio/gpio" + numb_string + "/value";
    ofstream gpio_value(gpio_direction.c_str());
    if(gpio_value.is_open())
    {
        gpio_value << number;
        gpio_value.close();
 
    }
    else
    {
        cout << "Error while opening file" << endl;
        return -1;
    }
    return 1;
}
 
//reading the input value if it's 1 the button is not pushed if 0 the button is pushed
int GPIO_Controller::getGPIO_Value(string& val)
{
    stringstream ss;
    ss << this->gpio_number;
    string numb_string = ss.str();
 
    string gpio_direction = "/sys/class/gpio/gpio" + numb_string + "/value";
    ifstream get_gpio_value(gpio_direction.c_str());
    if(get_gpio_value < 0)
    {
        cout << "Error while reading value file of GPIO number: " << this->gpio_number << endl;
        return -1;
    }
    else
    {
        get_gpio_value >> val;
        get_gpio_value.close();
    }
    if(val == "0")
    {
        return 0;
    }
    else
    {
        return 1;
    }
 
}
