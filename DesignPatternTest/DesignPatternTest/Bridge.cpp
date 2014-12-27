/*
Bridge Pattern

*/


#include <iostream>
using namespace std;




class IRenderer
{
public:
	virtual void PartileEffect() = 0;
};
class OpenGLRenderer : public IRenderer
{
public:
	virtual void PartileEffect()
	{
		cout << "�ϥ�OpenGL��@ Particle�ĪG" << endl;
	}
};

class DirectXRenderer : public IRenderer
{
public:
	virtual void PartileEffect()
	{
		cout << "�ϥ�DX��@ Particle�ĪG" << endl;
	}
};
class Renderer
{
public:
	Renderer()
	{
		mRenderer = new OpenGLRenderer();
	}
	void PartileEffect()
	{
		mRenderer->PartileEffect();
	}
	~Renderer()
	{
		delete mRenderer;
	}
private:
	IRenderer *mRenderer;
};

class Game
{
public:
	Game()
	{
		mRenderSystem = new Renderer();
	}
	~Game()
	{
		delete mRenderSystem;
	}
	void ShowFireWorks()
	{
		mRenderSystem->PartileEffect();
	}
	Renderer* mRenderSystem;
};
void BridgeTest()
{
	Game game;
	game.ShowFireWorks();
}