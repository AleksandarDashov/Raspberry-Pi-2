#include <iostream>

using namespace std;
//relativly fast swap function using XOR
void swap(int& a, int& b)
{
	//if a = 8, b = 4
	a = a ^ b;// 1000 = 1000 XOR 0100 = 1100
	b = a ^ b;// 0100 = 1100 XOR 0100 = 1000
	a = a ^ b;// 1100 = 1100 XOR 1000 = 0100
}

bool isEven(int x)
{
	//if x = 8
	if((x & 1) == 0 ) //1000 AND 0001 = 0000
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool checkBit(int number, int bit)
{
	//if number = 8, bit = 3
	if(number & (1<<bit))//1000 AND 1000
	{
		return true;
	}
	else
	{
		return false;	
	} 
	
}
int setBit(int number, int bit)
{
	//if number = 8, bit 0
	int new_number = number | (1<<bit); //1000 OR 0001 = 1001  
	return new_number;
}

int unsetBit(int number, int bit)
{
	//if number = 8, bit = 3
	int new_number = number & ~(1<<bit); // 1000 AND  0111 = 0000
	return new_number;
}

int main()
{

	int x = 5;
	int y = 10;
	int number = 8;
	swap(x, y);
	cout << "x: " << x << " y: " << y <<  endl;
	cout << isEven(y) << endl;
	cout << checkBit(number,3) << endl;//checking if the 3rd bit is on, 1000 = 8
	cout << setBit(number, 0) << endl;;//setting the 0th bit to 1, adding 1, 1001 = 9
	
	cout << unsetBit(number, 0) << endl; //unset the 0th bit to 0, so number is back to 8, 1000 = 8;
	cout <<unsetBit(number, 3) << endl;//unset the 3rd bit to 0, so number is 0, 0000 = 0;
	
	
	return 0;
}
