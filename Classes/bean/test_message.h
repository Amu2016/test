#pragma once
#include"test.pb.h"
#include"stdafx.h"
#include"base/base_message.h"

class test_ResMessage : public base_message {
public:
	test_ResMessage() 
		:personMsg(new Test::ResPersonInfo())
	{}

	virtual void reading(const void* data, int size)
	{
		personMsg->ParseFromArray(data, size);
		msgId = personMsg->msgid();

		name = personMsg->name();
		id = personMsg->id();
		email = personMsg->email();
	}

	//virtual MsgInfo writing()
	//{
	//	msgId = personMsg->msgid();
	//	MsgInfo msg;
	//	return msg;
	//}

	virtual int32_t getId()
	{
		return msgId;
	}

	virtual void callmsg()
	{
		log("========callmsg=======");
	}

private:
	Test::ResPersonInfo* personMsg;
	int32_t msgId;

	string name;
	int32_t id;
	string email;
};

class test_ReqMessage : public base_reqmessage {
public:
	test_ReqMessage()
		:buffer(malloc(BUFFER_SIZE - 4)),
		msgId(200001)
	{
		personMsg.set_msgid(200001);
	}

	virtual void reading(const void* data, int size)
	{
		//personMsg = msg;
	}

	virtual MsgInfo writing()
	{
		int size = personMsg.ByteSize();
		memset(buffer, 0, size);
		personMsg.SerializeToArray(buffer, size);

		MsgInfo msgInfo;
		msgInfo.msg = buffer;
		msgInfo.lenth = size;

		return msgInfo;
	}

	virtual int32_t getId()
	{
		return msgId;
	}

private:
	Test::ReqPersonInfo personMsg;
	int32_t msgId;

	void* buffer;

	string name;
	int32_t id;
	string email;
};