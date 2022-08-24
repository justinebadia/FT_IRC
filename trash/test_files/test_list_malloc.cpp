#include <iostream>
#include <list>
#include <string>

using std::string;
using std::list;
using std::cout;
using std::cerr;
using std::endl;

class Test
{
	public:

		string*	str;
		Test()
		{
			str = new string("Me");
		}
		Test(const Test& rhs)
		{
			*this = rhs;
		}
		Test& operator=(const Test& rhs)
		{
			str = new string(*rhs.str);
			return *this;
		}
		~Test()
		{
			delete str;
		}
};

int main( void )
{
	Test		one;
	list<Test>	ori;
	list<Test*>	ptr;
	list<Test*>	ptr2;

	ori.push_front(one);
	ptr.push_front(&ori.front());
	ptr2 = ptr;

	cout << "original list: " << ori.front().str << endl;
	cout << "ptr list: " << ptr.front()->str << endl;
	cout << "ptr list: " << ptr2.front()->str << endl;

	ptr.clear();

	cout << "original list: " << ori.front().str << endl;
	//cout << "ptr list: " << ptr.front()->str << endl;
	cout << "ptr2 list: " << ptr2.front()->str << endl;

	return 0;
}