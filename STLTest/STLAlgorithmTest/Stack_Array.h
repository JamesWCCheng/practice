#pragma once 
template<typename Type, int ARRAYSIZE>

class Stack_Array
{
public:
	Stack_Array() :
		m_size(0),
		m_max_stack_current_size(0),
		m_array(new Type[ARRAYSIZE]),
		m_max_value_array(new Type[ARRAYSIZE])
	{

	}
	bool IsEmpty()
	{
		return m_size == 0;
	}
	Type Pop()
	{
		if (m_size != 0)
		{
			m_size--;
			Type temp = m_array[m_size];
			PopMaxValue(temp);
			return temp;
		}
		else
		{
			throw nullptr;
		}
	}
	void Push(Type &&data)
	{
		if (m_size == ARRAYSIZE)
		{
			return; //exceed
		}
		m_array[m_size] = data;
		m_size++;
		PushToMaxValueStack(data);
	}
	int GetSize()
	{
		return m_size;
	}
	int GetMax()
	{
		if (m_max_stack_current_size != 0)
		{
			return m_max_value_array[m_max_stack_current_size - 1];
		}
		else
		{
			return INT_MIN;
		}
	}
	~Stack_Array()
	{
		delete[] m_array;
		delete[] m_max_value_array;
	}
private:
	void PushToMaxValueStack(Type max)
	{
		//no need to put in the max value.
		if (m_max_value_array[m_max_stack_current_size - 1] >= max)
		{
			return;
		}
		if (m_max_stack_current_size == ARRAYSIZE)
		{
			return; //exceed
		}
		m_max_value_array[m_max_stack_current_size] = max;
		m_max_stack_current_size++;
	}
	void PopMaxValue(Type test_value)
	{

		if (m_max_stack_current_size != 0)
		{ 
			if (m_max_value_array[m_max_stack_current_size - 1] == test_value) //Need to pop
			{
				m_max_stack_current_size--;
			}
		}
		else
		{
			throw nullptr;
		}
	}
	Type* m_array;
	Type* m_max_value_array;
	int m_max_stack_current_size;
	int m_size;
};