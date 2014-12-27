/*
這pattern的精隨好像就是 類似的東西卻有可以相互疊加的特性

例如這例子    傳說中的, 有毒的  可以疊加  砲阿 劍阿

所以 傳說中的, 有毒的  其實跟  砲阿劍阿 同型態, 可以都繼承base 武器

然後

傳說中的是來修飾武器的

傳說中的建構子(吃武器)

傳說中 裡面就要記錄所修飾的武器

這樣假設呼叫GetName, 除了進入傳說中的GetName外 還必須加上 修飾武器的GetName 
*/

#include <string>
#include <iostream>
class Weapon
{
protected:
	std::string Name;
public:
	Weapon()
	{

	}

	virtual int AttackPower() = 0;

	virtual std::string GetName() = 0;


	virtual ~Weapon()
	{
		std::cout << "銷毀武器完畢" << std::endl;
	}
};

class Decorator : public Weapon
{

};
class Legendary : public Decorator
{
public:
	Weapon *weapon;
	Legendary(Weapon *d)
	{
		Name = "傳說中的";
		weapon = d;
	}
	virtual int AttackPower()
	{
		return (weapon->AttackPower() + 100);
	}

	virtual std::string GetName()
	{
		return Name + weapon->GetName();
	}
	~Legendary()
	{
		std::cout << "銷毀傳說中的武器完畢" << std::endl;
		delete weapon;
	}
};
class Toxic : public Decorator
{
public:
	Weapon *weapon;
	Toxic(Weapon *d)
	{
		Name = "有毒的";
		weapon = d;
	}
	virtual int AttackPower()
	{
		return (weapon->AttackPower() + 110);
	}

	virtual std::string GetName()
	{
		return Name + weapon->GetName();
	}
	~Toxic()
	{
		std::cout << "銷毀有毒的武器完畢" << std::endl;
		delete weapon;
	}
};
class Kano : public Weapon
{
public:
	Kano()
	{
		Name = "加農砲";
	}
	virtual int AttackPower()
	{
		return 1000;
	}
	virtual std::string GetName()
	{
		return Name;
	}
	~Kano()
	{
		std::cout << "銷毀加農砲武器完畢" << std::endl;
	}
};

class Sword : public Weapon
{
public:
	Sword()
	{
		Name = "劍";
	}
	virtual int AttackPower()
	{
		return 10;
	}
	virtual std::string GetName()
	{
		return Name;
	}
	~Sword()
	{
		std::cout << "銷毀劍武器完畢" << std::endl;
	}
};