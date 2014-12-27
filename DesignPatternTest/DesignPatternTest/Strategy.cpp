/*
�����Ҧ�, 

*/

#include <iostream>
#include <string>
using namespace std;

class ISpeedUpBehavior
{
public:
	virtual void SpeedUp() = 0;
};
class Formula1SpeedUpBehavior : public ISpeedUpBehavior
{
public:
	virtual void SpeedUp()
	{
		cout << "���}���i�� ���������l" << endl;
	}
};
class NormalSpeedUpBehavior : public ISpeedUpBehavior
{
public:
	virtual void SpeedUp()
	{
		cout << "�o����j�O�@�I" << endl;
	}
};

class SuperCarSpeedUpBehavior : public ISpeedUpBehavior
{
public:
	virtual void SpeedUp()
	{
		cout << "���b�Q�g�[�t" << endl;
	}
};
class RacingCar
{
public:
	RacingCar(ISpeedUpBehavior* speedupbehavior)
		:m_Speedupbehavior(speedupbehavior)
	{
		
	}
	virtual void SpeedUp() = 0;
protected:
	ISpeedUpBehavior* m_Speedupbehavior;
};
class VolksWagen : public RacingCar
{
public:
	VolksWagen(ISpeedUpBehavior* speedupbehavior) :RacingCar(speedupbehavior)
	{
		
	}
	virtual void SpeedUp()
	{
		m_Speedupbehavior->SpeedUp();
	}
	
};

class BatManCar : public RacingCar
{
public:
	BatManCar(ISpeedUpBehavior* speedupbehavior) :RacingCar(speedupbehavior)
	{
		
	}
	virtual void SpeedUp()
	{
		m_Speedupbehavior->SpeedUp();
	}
	
};

class AsuradaCar : public RacingCar
{
public:
	AsuradaCar(ISpeedUpBehavior* speedupbehavior) :RacingCar(speedupbehavior)
	{
		
	}
	virtual void SpeedUp()
	{
		m_Speedupbehavior->SpeedUp();
	}
};
void StrategyTest()
{
	ISpeedUpBehavior *speedup = new NormalSpeedUpBehavior();
	RacingCar *volkswagen = new VolksWagen(speedup);
	volkswagen->SpeedUp();
	delete volkswagen;
	delete speedup;

	speedup = new SuperCarSpeedUpBehavior();
	RacingCar *batmancar = new BatManCar(speedup);
	batmancar->SpeedUp();
	delete batmancar;
	delete speedup;
}
