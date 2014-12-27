#include <deque>
#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <random>

#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)

class Memento
{
public:
	void Write(unsigned char* data, int size)
	{
		m_image_snapshot.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			m_image_snapshot.emplace_back(data[i]);
		}
	}
	std::unique_ptr<unsigned char> Read() const
	{
		unsigned char* buf = new unsigned char[m_image_snapshot.size()];
		std::copy(m_image_snapshot.begin(), m_image_snapshot.end(), buf);
		return std::unique_ptr<unsigned char>(buf);
	}
private:
	std::vector<unsigned char> m_image_snapshot;
};
class BitmapImage
{
public:
	BitmapImage() :m_image_data(nullptr)
	{
		std::default_random_engine g_generator;

		std::uniform_int_distribution<int> distribution(2, 3);
		m_Width = distribution(g_generator);
		m_Height = distribution(g_generator);
		m_size = m_Width * m_Height * 4;
		m_image_data = new unsigned char[m_size];

		std::uniform_int_distribution<int> RGBAVALUE(0, 255);
		for (size_t i = 0; i < m_size; i++)
		{
			m_image_data[i] = RGBAVALUE(g_generator);
		}
	}
	~BitmapImage()
	{
		delete[]m_image_data;
	}
	void Dump() const
	{
		for (size_t i = 0; i < m_size; i++)
		{
			std::cout << (int)m_image_data[i] << ", ";
		}
		std::cout << std::endl;
	}
	unsigned char *m_image_data;
	unsigned int m_size;
private:
	int m_Width;
	int m_Height;


};

class ICommand
{
public:
	virtual Memento* Execute() = 0;
	virtual void Undo(Memento*) = 0;
	virtual ~ICommand()
	{
		
	}
};

class CommandMgr
{
	std::deque<ICommand*> m_CommandQueue;
public:
	void ExecuteCommand(ICommand* command);
	void UndoCommand();
	~CommandMgr();
	
	
private:
	
	std::stack<Memento*> m_MementoStack;
};
class NegativeCommand : public ICommand
{
public:
	NegativeCommand(BitmapImage* bitmap);
	virtual Memento* Execute();
	virtual void Undo(Memento *);
private:
	BitmapImage * m_BitmapImage;
};

class GrayValueCommand : public ICommand
{
public:
	GrayValueCommand(BitmapImage* bitmap);
	virtual Memento* Execute();
	virtual void Undo(Memento*);
private:
	BitmapImage * m_BitmapImage;
};

