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
	/*IPlayerState * state = �z��;
	Weapon * currenWeapon = �j�D�K;
	void Attack()
	{
		state->Attack();
	}*/

};

class Iori : Player
{


};