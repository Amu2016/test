#include"SocketTcp.h"

#include"bean/test.pb.h"
#include"game/ui/utils/tipMgr.h"
#include"MsgMgr.h"


bool SocketTcp::Init()
{
	////1.connect方法创建实例
	//client = SocketIO::connect("tcp://192.168.51.171:8888", *this);
	//client->setTag("SocketTcp");
	////4.初始化的时候设置一个监听器：使用on监听事件和获取接收到的数据
	//client->on("loginresult", [=](SIOClient *client, const std::string &data) {//使用C++匿名函数实现
	//	log("login result is :%s", data.c_str());

	//	client->send("Login in...");
	//});

	return true;
}

void SocketTcp::Connect(const char * ip, unsigned short port, ConnectCallBack fun)
{
	connectAsync(ip, port, m_scheduler);
	callback = fun;
}

// will calling when a package is coming
void  SocketTcp::onMessageReceived(CCBuffer& oBuffer) 
{
	CCLOG("onMessageReceived %s:", oBuffer);
};

void SocketTcp::onMessage(const char* buffer, int msgLen) {
	//CCLOG("onMessage  :");
	//Test::Person personMsg;
	//personMsg.ParseFromArray((void*)buffer, 1000);
	////buf_msg[ret_val] = 0x00;
	//auto name = personMsg.name();
	//auto id = personMsg.id();
	//auto email = personMsg.email();
	//CCLOG("name  : %s", personMsg.name());
	//CCLOG("id  : %d", personMsg.id());
	//CCLOG("email  : %s", personMsg.email());

	//TipMgr::getInstance()->ShowNotic(name);

	MsgMgr::getInstance()->OnMessage(buffer, msgLen);
}

// when connected will calling
void  SocketTcp::onConnected() 
{
	log("onConnect");
	//const char* sendData;
	//sendData = "你好，TCP服务端，我是客户端\n";
	//SocketTcp::getInstance()->send(sendData, strlen(sendData));

	//Test::Person personMsg;
	//personMsg.set_name("客户端一号");
	//personMsg.set_id(7);
	//personMsg.set_email("461427494@qq.com");

	//int size = personMsg.ByteSize();
	//void *buffer = malloc(size);
	//personMsg.SerializeToArray(buffer, size);

	//personMsg.ParseFromArray((void*)buffer, 1000);
	////buf_msg[ret_val] = 0x00;
	//auto name = personMsg.name();
	//auto id = personMsg.id();
	//auto email = personMsg.email();
	//auto a = strlen((char *)buffer);

	//SocketTcp::getInstance()->send((char *)buffer, strlen((char *)buffer));

	//if (callback)
	//	callback();
	MsgMgr::getInstance()->onConnected();
}

// when connect time out will calling
void  SocketTcp::onConnectTimeout() 
{
	log("onConnectTimeout");

}

// on disconnected will call
void  SocketTcp::onDisconnected() 
{
	log("onDisconnected");
}

// on exception
void  SocketTcp::onExceptionCaught(CCSocketStatus eStatus) 
{
	log("onExceptionCaught");
}

void SocketTcp::updateOnce(float dt) {
	log("updateOnce");
}

//void SocketTcp::onConnect(SIOClient* client) {
//	log("onConnect");
//	log("%s connect", client->getTag());
//}
//
//void SocketTcp::onMessage(SIOClient* client, const std::string& data) {
//	log("onMessage");
//	log("%s received content is:%s", client->getTag(), data.c_str());
//}
//
//void SocketTcp::onError(SIOClient* client, const std::string& data) {
//	log("onError");
//	log("%s error is:%s", client->getTag(), data.c_str());
//}
//
//void SocketTcp::onClose(SIOClient * client) {
//	log("onClose");
//	log("%s is closed", client->getTag());
//}