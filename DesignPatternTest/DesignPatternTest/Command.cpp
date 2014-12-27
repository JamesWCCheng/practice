#pragma warning(disable : 4996)
#include "Command.h"
#include <iostream>
#include <memory>
void CommandMgr::ExecuteCommand(ICommand* command)
{
	m_CommandQueue.push_back(command);
	if (m_CommandQueue.size() > 4)
	{
		delete m_CommandQueue.front();
		m_CommandQueue.pop_front();
		std::cout << "Exceed the undo command steps, pop one" << std::endl;
	}
	m_MementoStack.push(command->Execute());
}
void CommandMgr::UndoCommand()
{
	if (m_CommandQueue.empty() != true)
	{
		auto command = m_CommandQueue.back();
		m_CommandQueue.pop_back();
		auto memento = m_MementoStack.top();
		command->Undo(memento);
		m_MementoStack.pop();
		delete memento;
		delete command;
	}
	else
	{
		std::cout << "Cannot Undo, Exceed the undo steps" << std::endl;
	}
}

CommandMgr::~CommandMgr()
{
	for (auto command : m_CommandQueue)
	{
		delete command;
	}
	while (true)
	{
		if (m_MementoStack.empty() != true)
		{
			delete m_MementoStack.top();
			m_MementoStack.pop();
		}
		else
			break;
	}
}

NegativeCommand::NegativeCommand(BitmapImage * bitmap)
{
	m_BitmapImage = bitmap;
}
Memento* NegativeCommand::Execute()
{
	Memento *snapshot = new Memento();
	snapshot->Write(this->m_BitmapImage->m_image_data, this->m_BitmapImage->m_size);

	std::cout << "Do Negative Transform" << std::endl;
	for (size_t i = 0; i < m_BitmapImage->m_size; i += 4)
	{
		m_BitmapImage->m_image_data[i] = 255 - m_BitmapImage->m_image_data[i];
		m_BitmapImage->m_image_data[i + 1] = 255 - m_BitmapImage->m_image_data[i + 1];
		m_BitmapImage->m_image_data[i + 2] = 255 - m_BitmapImage->m_image_data[i + 2];
		m_BitmapImage->m_image_data[i + 3] = 255 - m_BitmapImage->m_image_data[i + 3];
	}
	
	return snapshot;
}
void NegativeCommand::Undo(Memento* memento)
{
	std::cout << "UnDo Negative Transform" << std::endl;
	auto data_from_memento = memento->Read();
	delete [] m_BitmapImage->m_image_data;
	m_BitmapImage->m_image_data = data_from_memento.release();
}

GrayValueCommand::GrayValueCommand(BitmapImage * bitmap)
{
	m_BitmapImage = bitmap;
}
Memento* GrayValueCommand::Execute()
{
	Memento *snapshot = new Memento();
	snapshot->Write(this->m_BitmapImage->m_image_data, this->m_BitmapImage->m_size);

	std::cout << "Do Gray Value Transform" << std::endl;
	for (size_t i = 0; i < m_BitmapImage->m_size; i+=4)
	{
		unsigned int average = (m_BitmapImage->m_image_data[i] +
			m_BitmapImage->m_image_data[i + 1] +
			m_BitmapImage->m_image_data[i + 2] +
			m_BitmapImage->m_image_data[i + 3]) / 4;
		
		m_BitmapImage->m_image_data[i] =
			m_BitmapImage->m_image_data[i + 1] =
			m_BitmapImage->m_image_data[i + 2] =
			m_BitmapImage->m_image_data[i + 3] = static_cast<unsigned char>(average);

	}
	return snapshot;
}
void GrayValueCommand::Undo(Memento* memento)
{
	std::cout << "UnDo Gray Value Transform" << std::endl;
	auto data_from_memento = memento->Read();
	delete[] m_BitmapImage->m_image_data;
	m_BitmapImage->m_image_data = data_from_memento.release();
}

void CommandTest()
{
	BitmapImage * bitmap_image = new BitmapImage();

	CommandMgr * command_mgr = new CommandMgr();
	bitmap_image->Dump(); //original
	command_mgr->ExecuteCommand(new NegativeCommand(bitmap_image));
	bitmap_image->Dump(); //1
	command_mgr->ExecuteCommand(new GrayValueCommand(bitmap_image));
	bitmap_image->Dump(); //2
	command_mgr->ExecuteCommand(new NegativeCommand(bitmap_image));
	bitmap_image->Dump(); //3


	command_mgr->UndoCommand();
	bitmap_image->Dump(); //2
	command_mgr->UndoCommand();
	bitmap_image->Dump(); //1
	command_mgr->UndoCommand(); 
	bitmap_image->Dump(); //original
	command_mgr->UndoCommand();
	command_mgr->UndoCommand();
	delete command_mgr;
	delete bitmap_image;
}