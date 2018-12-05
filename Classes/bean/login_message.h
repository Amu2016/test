#pragma once
#include"login.pb.h"
#include"stdafx.h"
#include"base/base_message.h"

class loginSuccess_ResMessage : public base_message {
public:
	loginSuccess_ResMessage(){}

	virtual void reading(const void* data, int size)
	{
		loginSuccess.ParseFromArray(data, size);
		msgId = loginSuccess.msgid();

		name = loginSuccess.name();
		id = loginSuccess.id();
		email = loginSuccess.email();
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
	Login::LoginSuccess loginSuccess;
	int32_t msgId;

	string name;
	int32_t id;
	string email;
};

class login_ReqMessage : public base_reqmessage {
public:
	login_ReqMessage(string name, int32_t password)
		:buffer(malloc(BUFFER_SIZE - 4)),
		msgId(210001)
	{
		loginInfo.set_msgid(210001);

		loginInfo.set_name(name);
		loginInfo.set_password(password);
	}

	virtual void reading(const void* data, int size)
	{
		//personMsg = msg;
	}

	virtual MsgInfo writing()
	{
		int size = loginInfo.ByteSize();
		memset(buffer, 0, size);
		loginInfo.SerializeToArray(buffer, size);

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
	Login::Login loginInfo;
	int32_t msgId;

	void* buffer;

	string name;
	int32_t password;
};