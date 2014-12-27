/*
工廠模式, 把要產生的東西依附在一個工廠一一獨立出來繼承base工廠

產生的東西 也都一一獨立出來 繼承base 東西

然後做一個maker 帶入 看要哪個factory

帶進來之後maker只需要無腦呼叫factory對應的create 就可以產生物件, 若還有一些共同都要設定的function也可以在這呼叫後return

精隨在 把 工廠一一獨立出來的想法吧????????????

*/


#include <iostream>
#include <string>
#include "./Decorator.h"
using namespace std;

//所有物種的base class介面
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
	NightElf() : Creature("夜精靈")
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
	Undead() :Creature("不死族")
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

	cout<<"產生一隻~"<<ne->GetName()<<endl;
	cout << "持有武器為" << ne->GetWeapon()->GetName()<< endl;
	
	NEmaker->DestroyCreature(ne);

	delete NEmaker;
	delete factory;


	CreatureFactory *factory2 = new UndeadFactory();
	CreatureMaker *UDmaker = new CreatureMaker(factory2);
	Creature* ud = UDmaker->MakeCreature();

	cout << "產生一隻~" << ud->GetName() << endl;
	cout << "持有武器為" << ud->GetWeapon()->GetName() << endl;
	
	UDmaker->DestroyCreature(ud);
	delete UDmaker;
	delete factory2;
	system("PAUSE");
}