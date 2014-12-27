#pragma once
class IAction
{
public:
	virtual void Attack() = 0;
	virtual void Defend() = 0;

};
class IPlayerState :public IAction
{
public:
	

};

class Player : public IAction
{


};

class Kyo : Player
{
	/*IPlayerState * state = Ãz®ð;
	Weapon * currenWeapon = ¤j³DîK;
	void Attack()
	{
		state->Attack();
	}*/

};

class Iori : Player
{


};