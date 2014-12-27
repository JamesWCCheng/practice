#pragma once
#include <string>
#include <iostream>
#include <vector>
class Entry
{
public:
	virtual ~Entry()
	{
	
	}
	Entry(const std::string & name)
		:m_Name(name), m_Size(0)
	{
		
	}

	inline const std::string& GetName() const
	{
		return m_Name;
	}

	inline std::string& GetName()
	{
		return m_Name;
	}
	
	inline virtual int GetSize() const
	{
		return m_Size;
	}
	virtual void Print(const std::string &prefix) const = 0;
	
	virtual Entry* Add(Entry *item) = 0;
protected:
	std::string m_Name;
	int m_Size;
};

class File : public Entry
{
public:
	File(const std::string & name, int size)
		: Entry(name)
	{
		m_Size = size;
	}
	~File()
	{
		std::cout << "¸Ñºc" + GetName() << std::endl;
	}
	virtual void Print(const std::string &prefix) const;

	virtual Entry* Add(Entry *item);

	inline virtual int GetSize() const
	{
		return m_Size;
	}
private:
	

};

class Directory : public Entry
{
public:
	Directory(const std::string & name)
		: Entry(name)
	{
	
	}

	~Directory()
	{
		std::cout << "¸Ñºc" + GetName() << std::endl;
		std::vector<Entry*>::const_iterator citr = m_EntryList.begin();

		while (citr != m_EntryList.end())
		{
			delete *citr;
			citr++;
		}
	}

	virtual void Print(const std::string &prefix) const;

	virtual Entry* Add(Entry *item);

	inline virtual int GetSize() const;
private:
	std::vector<Entry*> m_EntryList;


};
