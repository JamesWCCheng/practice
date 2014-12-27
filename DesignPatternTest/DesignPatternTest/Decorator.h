/*
�opattern�����H�n���N�O �������F��o���i�H�ۤ��|�[���S��

�Ҧp�o�Ҥl    �ǻ�����, ���r��  �i�H�|�[  ���� �C��

�ҥH �ǻ�����, ���r��  ����  �����C�� �P���A, �i�H���~��base �Z��

�M��

�ǻ������O�ӭ׹��Z����

�ǻ������غc�l(�Y�Z��)

�ǻ��� �̭��N�n�O���ҭ׹����Z��

�o�˰��]�I�sGetName, ���F�i�J�ǻ�����GetName�~ �٥����[�W �׹��Z����GetName 
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
		std::cout << "�P���Z������" << std::endl;
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
		Name = "�ǻ�����";
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
		std::cout << "�P���ǻ������Z������" << std::endl;
		delete weapon;
	}
};
class Toxic : public Decorator
{
public:
	Weapon *weapon;
	Toxic(Weapon *d)
	{
		Name = "���r��";
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
		std::cout << "�P�����r���Z������" << std::endl;
		delete weapon;
	}
};
class Kano : public Weapon
{
public:
	Kano()
	{
		Name = "�[�A��";
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
		std::cout << "�P���[�A���Z������" << std::endl;
	}
};

class Sword : public Weapon
{
public:
	Sword()
	{
		Name = "�C";
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
		std::cout << "�P���C�Z������" << std::endl;
	}
};