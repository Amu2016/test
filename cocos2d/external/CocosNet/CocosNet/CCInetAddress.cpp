/****************************************************************************
Copyright (c) 2014 Lijunlin - Jason lee

Created by Lijunlin - Jason lee on 2014

jason.lee.c@foxmail.com
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CCInetAddress.h"

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	#pragma comment(lib, "Ws2_32.lib")
#endif

NS_CC_BEGIN

CCInetAddress::CCInetAddress()
{
//#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
//	sin_len = sizeof(struct sockaddr_in);
//	sin_family = AF_INET;
//	sin_addr.s_addr = INADDR_ANY;
//	sin_port = 0;
//	memset(sin_zero, 0, 8);
//#endif
//
//#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
//	sin_family = AF_INET;
//	sin_addr.s_addr = INADDR_ANY;
//	sin_port = 0;
//	memset(sin_zero, 0, 8);
//#endif
//
//#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
//	sin_family = AF_INET;
//	sin_addr.s_addr = INADDR_ANY;
//	sin_port = 0;
//	memset(sin_zero, 0, 8);
//#endif
}

CCInetAddress::CCInetAddress(const char* ip, unsigned short port)
{
	char iparr[128];
	memset(iparr, 0, sizeof(iparr));
	strcpy(iparr, ip);

	struct sockaddr_in addr_4;
	struct sockaddr_in6 addr_6;
	struct addrinfo *result;
	if (int error = getaddrinfo(iparr, NULL, NULL, &result))
	{
		CCLOGERROR("net_listen error for : %s", gai_strerror(error));
		return;
	}
	const struct sockaddr *sa = result->ai_addr;
	family = sa->sa_family;
	switch (family)
	{
		case AF_INET:
			if (inet_ntop(family, &(((struct sockaddr_in*) sa)->sin_addr), iparr, 128) < 0)
			{
				CCLOGERROR("inet_ntop ipv4 error : %s", ip);
				break;
			}
			addr_4.sin_family = family;
			addr_4.sin_port = htons(port);
			addr_4.sin_addr.s_addr = inet_addr(iparr);
			addr_len = sizeof(addr_4);
			sockadd4 = addr_4;
			break;
		case AF_INET6:
			if (inet_ntop(family, &(((struct sockaddr_in6*) sa)->sin6_addr), iparr, 128) < 0)
			{
				CCLOGERROR("inet_ntop ipv6 error : %s", ip);
				break;
			}
			memset(&addr_6, 0, sizeof(addr_6));
			addr_6.sin6_family = family;
			addr_6.sin6_port = htons(port);
			if (inet_pton(family, iparr, &addr_6.sin6_addr) < 0)
			{
				CCLOGERROR("inet_pton ipv6 error : %s", ip);
				break;
			}

			addr_len = sizeof(addr_6);
			sockadd6 = addr_6;
			break;
		default:
			CCLOGERROR(" Unknown AF Value.");
			break;
	}
}

CCInetAddress::CCInetAddress(const struct sockaddr * addr)
{
	//memcpy(&this->sin_family, addr, sizeof(struct sockaddr));
}

CCInetAddress::~CCInetAddress(void)
{
	
}

CCInetAddress::operator struct sockaddr*()
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	if (family == AF_INET6)
		return (struct sockaddr *)(&(&sockadd6)->sin6_family);
	return (struct sockaddr *)(&(&sockadd4)->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	if(family == AF_INET6)
		return (struct sockaddr *)(&(&sockadd6)->sin6_family);
	return (struct sockaddr *)(&(&sockadd4)->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	if (family == AF_INET6)
		return (struct sockaddr *)(&(&sockadd6)->sin6_len);
	return (struct sockaddr *)(&(&sockadd4)->sin_len);
#endif
}

CCInetAddress::operator const struct sockaddr*() const
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	if (family == AF_INET6)
		return (const struct sockaddr *)(&(&sockadd6)->sin6_family);
	return (const struct sockaddr *)(&(&sockadd4)->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	if (family == AF_INET6)
		return (const struct sockaddr *)(&(&sockadd6)->sin6_family);
	return (const struct sockaddr *)(&(&sockadd4)->sin_family);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	if (family == AF_INET6)
		return (const struct sockaddr *)(&(&sockadd6)->sin6_len);
	return (const struct sockaddr *)(&(&sockadd4)->sin_len);
#endif
}

const char* CCInetAddress::getHostAddress() const
{
	static char addr[64];

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	sprintf_s(addr, 64, "%s:%u", getIp(), getPort());
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	snprintf(addr, 64, "%s:%u", getIp(), getPort());
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	snprintf(addr, 64, "%s:%u", getIp(), getPort());
#endif
	return addr;
}

const char* CCInetAddress::getIp() const
{
	void* sin_addr;

	if (family == AF_INET6)
		sin_addr = &((struct sockaddr_in6*)&sockadd6)->sin6_addr;
	else
		sin_addr = &((struct sockaddr_in*)&sockadd4)->sin_addr;
	char iparr[128];
	memset(iparr, 0, sizeof(iparr));
	inet_ntop(family, sin_addr, iparr, 128);
	char* str = (char *)malloc(128);
	strcpy(str, iparr);
	return str;
}

unsigned short CCInetAddress::getPort() const
{
	unsigned short sin_port;
	if (family == AF_INET6)
		sin_port = ((struct sockaddr_in6*)&sockadd6)->sin6_port;
	else
		sin_port = ((struct sockaddr_in*)&sockadd4)->sin_port;
	return ntohs(sin_port);
}

void CCInetAddress::setIp(const char* ip)
{
	/*sin_addr.s_addr = inet_addr(ip);*/
}

void CCInetAddress::setIp(unsigned int ip)
{
	/*sin_addr.s_addr = ip;*/
}

void CCInetAddress::setPort(unsigned short port)
{
	/*sin_port = htons(port);*/
}

void CCInetAddress::setHost(const char* name)
{
	//hostent* h = gethostbyname(name);
	//if( h != NULL )
	//{
	//	sin_addr.s_addr = *((u_long *)h->h_addr_list[0]);
	//}
}

int CCInetAddress::getLength() 
{
	return addr_len;
}

NS_CC_END