#pragma once

#include "LinkedList.h"
template<typename Type, class Container = LinkedList<Type>>
class Stack_LinkedList
{
public:
	Stack_LinkedList():m_size(0)
	{

	}
	Type Pop()
	{
		if (m_size != 0)
		{
			m_size--;
			auto tail = m_linkedlist.RemoveTail();
			auto data = tail->m_data;
			delete tail;
			return data;

		}
		else
		{
			throw nullptr;
		}
	}
	void Push(Type data)
	{
		m_linkedlist.Insert(new Node<Type>(data));
		m_size++;
	}
	int GetSize()
	{
		return m_size;
	}
	~Stack_LinkedList()
	{
		m_linkedlist.DeleteAll();
	}
private:
	Container m_linkedlist;
	int m_size;
};
