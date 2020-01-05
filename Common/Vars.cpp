#include "pch.h"

#include "ProudNetClient.h"
#include "Vars.h";

// 클라이언트와 서버간 주고 받는 프로토콜 버전(GUID) 및 서버 포트 정의
Proud::PNGUID gProtocolVersionSrc = { 0xeedf4dc9, 0xd6c4, 0x42cf, { 0xb1, 0x11, 0xe3, 0x7e, 0x6e, 0x46, 0x3e, 0xdb } };

Proud::Guid gProtocolVersion = Proud::Guid(gProtocolVersionSrc);
int gServerPort = 33334;
