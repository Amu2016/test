#include"MsgMgr.h"

#include"MessagePool.h"

void MsgMgr::connect(const char* ip, unsigned short port, ConnectCallBack fun) {

	Socket->Connect(ip, port, fun);

	callback = fun;
}

void MsgMgr::onConnected()
{
	if (callback)
		callback();
}

void MsgMgr::OnMessage(const char * msg, int msgLen)
{
	//消息长度
	auto m_len = ((byte)msg[0] << 24) | ((byte)msg[1] << 16) | ((byte)msg[2] << 8) | ((byte)msg[3]);
	//消息id
	auto msgId = ((byte)msg[4] << 24) | ((byte)msg[5] << 16) | ((byte)msg[6] << 8) | ((byte)msg[7]);
	//消息名字长度
	auto nameLenth = ((byte)msg[8] << 24) | ((byte)msg[9] << 16) | ((byte)msg[10] << 8) | ((byte)msg[11]);

	//消息名字
	memset(recv_msgName, 0, nameLenth);
	strncpy(recv_msgName, msg + 12, (12 + nameLenth));
	recv_msgName[nameLenth] = 0x00;

	//消息主体
	memset(recv_msgBuffer, 0, msgLen - 12 - nameLenth);
	strncpy(recv_msgBuffer, msg + 12 + nameLenth, msgLen - 12 - nameLenth);

	//从消息池中取出消息对象  读取收到的消息并执行
	auto meg_Obj = MessagePool::getInstance()->GetMsg(msgId);
	meg_Obj->reading(recv_msgBuffer, msgLen - 12 - nameLenth);
	meg_Obj->callmsg();
}

void MsgMgr::Send(base_reqmessage* msg)
{
	auto msgInfo = msg->writing();

	auto size = msgInfo.lenth;
	buffer = msgInfo.msg;

	const char* msgName = "ResPersonInfo";
	int32_t name_len = strlen(msgName);
	int32_t msgId = msg->getId();

	// msgId(int32_t)字节数  name_len(int32_t)字节数  name(char[])字节数
	int32_t msg_len = 4 + 4 + name_len + size;

	c_msg[0] = (char)(0xff & (msg_len >> 24));
	c_msg[1] = (char)(0xff & (msg_len >> 16));
	c_msg[2] = (char)(0xff & (msg_len >> 8));
	c_msg[3] = (char)(0xff & (msg_len));

	c_msg[4] = (char)(0xff & (msgId >> 24));
	c_msg[5] = (char)(0xff & (msgId >> 16));
	c_msg[6] = (char)(0xff & (msgId >> 8));
	c_msg[7] = (char)(0xff & (msgId));

	c_msg[8] = (char)(0xff & (name_len >> 24));
	c_msg[9] = (char)(0xff & (name_len >> 16));
	c_msg[10] = (char)(0xff & (name_len >> 8));
	c_msg[11] = (char)(0xff & (name_len));

	sprintf(c_msg + 12, msgName);
	sprintf(c_msg + 12 + name_len, (char*)buffer);
	int32_t len = 12 + name_len + size;

	Socket->send(c_msg, len);
	delete buffer;
}
