/*
�˹��Ҧ�, 

*/


#include <iostream>
#include "Decorator.h"
using namespace std;


void DecoratorTest()
{
	Weapon * ptr = new Sword();
	ptr = new Toxic(ptr);
	ptr = new Legendary(ptr);
	Weapon *ptr2 = new Kano();
	ptr2 = new Legendary(ptr2);
	cout << ptr2->GetName() << ", �����O��: " << ptr2->AttackPower() << endl;
	cout << ptr->GetName() << ", �����O��: " << ptr->AttackPower() << endl;
	delete ptr;
	delete ptr2;
	system("PAUSE");
}


