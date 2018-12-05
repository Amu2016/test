#pragma once
#include <google/protobuf/message.h>

struct MsgInfo
{
	void* msg;
	int32_t lenth;
};

class base_message {
public:
	base_message() {};
	virtual void reading(const void* data, int size) = 0;
	//virtual MsgInfo writing() {};
	virtual int32_t getId() = 0;

	virtual void callmsg()  = 0;
};

class base_reqmessage {
public:
	base_reqmessage() {};
	virtual void reading(const void* data, int size) = 0;
	virtual MsgInfo writing() = 0;
	virtual int32_t getId() = 0;
};