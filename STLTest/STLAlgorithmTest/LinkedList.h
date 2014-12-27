#pragma once
#include <iostream>
#include <ostream>
template<typename Type>
class Node
{
public:
	Node(Type data) :m_next(nullptr), m_data(std::move(data))
	{

	}
	Node* m_next;
	Type m_data;

};

template<typename Type>
std::ostream& operator << (std::ostream & out, Node<Type> * node)
{
	out << node->m_data;
	return out;
}
template<typename Type>
class LinkedList
{
public:
	LinkedList() :m_head(nullptr)
	{
		
	}
	void Insert(Node<Type>* node)
	{
		if (m_head == nullptr)
		{
			m_head = node;
			m_tail = m_head;
		}
		else
		{
			m_tail->m_next = node;
			m_tail = node;
		}
	}

	void RemoveSpecifyNode(Node<Type>* node)
	{
		if (node->m_next == nullptr)
		{
			RemoveTail();
			return;
		}
		//swap with next
		Type temp = node->m_data;
		node->m_data = node->m_next->m_data;
		node->m_next->m_data = temp;

		auto need2delete = node->m_next;
		node->m_next = node->m_next->m_next;
		//delete next
		delete need2delete;
	}
	Node<Type>* RemoveTail()
	{
		if (m_head == m_tail)
		{
			auto temp = m_head;
			m_head = m_tail = nullptr;
			return temp;
		}
		auto current = m_head;
		while (current->m_next != m_tail)
		{
			current = current->m_next;
		}
		auto last = m_tail;
		m_tail = current;
		current->m_next = nullptr;
		return last;
	}
	void Reverse()
	{
		if (m_head == m_tail)
		{
			return;
		}
		m_tail = m_head;
		m_head = ReverseImpIterative();
	}

	void PrintList()
	{
		if (m_head == m_tail)
		{
			if (m_tail != nullptr)
			{
				cout << m_tail << endl;
			}
			
			return;
		}
		auto current = m_head;
		while (current != nullptr)
		{
			cout << current <<"\t";
			current = current->m_next;
		}
		cout << endl;
	}
	void DeleteAll()
	{
		if (m_head == m_tail)
		{
			return;
		}
		auto current = m_head;
		while (current != nullptr)
		{
			auto temp = current;
			current = current->m_next;
			delete temp;
		}
		m_tail = m_head = nullptr;
	}
	~LinkedList()
	{
		DeleteAll();
	}
private:
	Node<Type>* ReverseImpIterative()
	{
		Node<Type>* prev_node = nullptr;
		Node<Type>* this_node = m_head;
		Node<Type>* next_node = m_head->m_next;

		while (next_node != nullptr)
		{
			this_node->m_next = prev_node;
			prev_node = this_node;
			this_node = next_node;
			next_node = next_node->m_next;
		}
		this_node->m_next = prev_node;
		return this_node;
	}

	Node<Type>* m_head;
	Node<Type>* m_tail;
};