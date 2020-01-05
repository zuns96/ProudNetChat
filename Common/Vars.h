extern Proud::Guid g_Version;
extern int g_ServerPort;

class MyClass
{
public:
	int a;
	float b;
	double c;
};

namespace Proud
{ 
	void AppendTextOut(Proud::String &a, const MyClass &b);

	Proud::CMessage& operator >> (Proud::CMessage &a, const MyClass &b);

	Proud::CMessage& operator << (Proud::CMessage &a, const MyClass &b);
}