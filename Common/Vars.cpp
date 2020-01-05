#include "pch.h"

#include "ProudNetClient.h"
#include "Vars.h";

// 클라이언트와 서버간 주고 받는 프로토콜 버전(GUID) 및 서버 포트 정의
Proud::PNGUID guid = { 0xeedf4dc9, 0xd6c4, 0x42cf, { 0xb1, 0x11, 0xe3, 0x7e, 0x6e, 0x46, 0x3e, 0xdb } };

Proud::Guid g_Version = Proud::Guid(guid);
int g_ServerPort = 33334;

namespace Proud
{
	void AppendTextOut(String &a, const MyClass &b)
	{
		String f;
		f.Format(_PNT("MyClass.a : %d, MyClass.b: %.f, MyClass : %lf\n"), b.a, b.b, b.c);
		a += f;
	}

	CMessage& operator >> (CMessage &a, const MyClass &b)
	{
		int _a = b.a;
		float _b = b.b;
		double _c = b.c;

		a >> _a;
		a >> _b;
		a >> _c;
		return a;
	}

	CMessage& operator << (CMessage &a, const MyClass &b)
	{
		a << b.a;
		a << b.b;
		a << b.c;
		return a;
	}
}