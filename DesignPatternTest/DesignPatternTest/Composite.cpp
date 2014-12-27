/*
²Õ¦X¼Ò¦¡, 

*/


#include "Composite.h"
#include <iostream>
using namespace std;

void File::Print(const std::string &prefix) const
{
	cout << "File Name = " << prefix + "\\" + this->GetName() << " , Size = " << this->GetSize() << endl;
}

 Entry* File::Add(Entry *item)
{
	 return this;
}


 inline int Directory::GetSize() const
 {
	 int size = 0;
	 std::vector<Entry*>::const_iterator citr = m_EntryList.begin();

	 while (citr != m_EntryList.end())
	 {
		 size += (*citr)->GetSize();
		 citr++;
	 }
	 return size;
 }
 void Directory::Print(const std::string &prefix) const
{
	 cout << "Dir Name = " << prefix + "\\" + this->GetName() << " , Size = " << this->GetSize() << endl;
	std::vector<Entry*>::const_iterator citr = m_EntryList.begin();

	while (citr != m_EntryList.end())
	{
		(*citr)->Print(prefix + "\\" + this->GetName());
		citr++;
	}
}

Entry* Directory::Add(Entry *item)
{
	this->m_EntryList.push_back(item);
	return this;
}

void CompositeTest()
{
	Directory *windows = new Directory("windows");
	Directory *system32 = new Directory("system32");
	Directory *font = new Directory("font");
	windows->Add(system32);
	windows->Add(font);
	File *file1 = new File("1.txt", 1024);
	system32->Add(file1);
	File *file2 = new File("2.txt", 4096);
	system32->Add(file2);
	File *font1 = new File("font1.ttf", 256);
	font->Add(font1);

	windows->Print("");


	delete windows;
	system("PAUSE");
}

