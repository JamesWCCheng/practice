/*
�u�t�Ҧ�, ��n���ͪ��F��̪��b�@�Ӥu�t�@�@�W�ߥX���~��base�u�t

���ͪ��F�� �]���@�@�W�ߥX�� �~��base �F��

�M�ᰵ�@��maker �a�J �ݭn����factory

�a�i�Ӥ���maker�u�ݭn�L���I�sfactory������create �N�i�H���ͪ���, �Y�٦��@�Ǧ@�P���n�]�w��function�]�i�H�b�o�I�s��return

���H�b �� �u�t�@�@�W�ߥX�Ӫ��Q�k�a????????????

*/


#include <iostream>
#include <string>
#include "./Decorator.h"
using namespace std;

//�Ҧ����ت�base class����
class Creature
{
 public:
	Creature(const string &name) :Name(name)
	{
		
	}
	virtual const string& GetName() const = 0;
	virtual Weapon* GetWeapon() = 0;
	virtual ~Creature()
	{}
protected:
	string Name;


};

class NightElf : public Creature
{
	Weapon * weapon;
public:
	NightElf() : Creature("�]���F")
	{
		weapon = new Kano();
		weapon = new Legendary(weapon);
	}
	virtual const string& GetName() const
	{
		return Name;
	}

	virtual Weapon* GetWeapon()
	{
		return weapon;
	}
	~NightElf()
	{
		delete weapon;
	}
};

class Undead : public Creature
{
	Weapon * weapon;
public:
	Undead() :Creature("������")
	{
		weapon = new Sword();
		weapon = new Toxic(weapon);
	}
	
	virtual const string& GetName() const
	{
		return Name;
	}
	virtual Weapon* GetWeapon()
	{
		return weapon;
	}
	~Undead()
	{
		delete weapon;
	}
};

class CreatureFactory
{
public:
	virtual Creature* MakeCreature() = 0;
	
	
	virtual ~CreatureFactory()
	{
		
	}
private:
	virtual void DestroyCreature(Creature* creature)
	{
		delete creature;
	}
	friend class CreatureMaker;
};

class UndeadFactory : public CreatureFactory
{
	virtual Creature* MakeCreature()
	{
		return new Undead();
	}
	
};
class NightElfFactory : public CreatureFactory
{
	virtual Creature* MakeCreature()
	{
		return new NightElf();
	}
	

};

class CreatureMaker
{
	CreatureFactory* m_Facotry;
public:
	CreatureMaker(CreatureFactory *factory)
	{
		m_Facotry = factory;
	}

	Creature* MakeCreature()
	{
		auto creature = m_Facotry->MakeCreature();
		return creature;
	}
	void DestroyCreature(Creature* creature)
	{
		m_Facotry->DestroyCreature(creature);
	}
};
void FactoryTest()
{
	CreatureFactory *factory = new NightElfFactory();
	CreatureMaker *NEmaker = new CreatureMaker(factory);
	Creature* ne = NEmaker->MakeCreature();

	cout<<"���ͤ@��~"<<ne->GetName()<<endl;
	cout << "�����Z����" << ne->GetWeapon()->GetName()<< endl;
	
	NEmaker->DestroyCreature(ne);

	delete NEmaker;
	delete factory;


	CreatureFactory *factory2 = new UndeadFactory();
	CreatureMaker *UDmaker = new CreatureMaker(factory2);
	Creature* ud = UDmaker->MakeCreature();

	cout << "���ͤ@��~" << ud->GetName() << endl;
	cout << "�����Z����" << ud->GetWeapon()->GetName() << endl;
	
	UDmaker->DestroyCreature(ud);
	delete UDmaker;
	delete factory2;
	system("PAUSE");
}