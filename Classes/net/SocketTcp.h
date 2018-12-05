#pragma once
#include"cocos2d.h"
#include"network/SocketIO.h"
#include"CocosNet/CCNetDelegate.h"
#include <functional>
USING_NS_CC;
using namespace cocos2d::network;


//继承SocketIO::SIODelegate和实现四个虚函数
class SocketTcp : public CCNetDelegate, public Node
{
protected:
	typedef std::function<void()> ConnectCallBack;
public:
	//static SocketTcp* getInstance() {
	//	static SocketTcp* socketTcp = new SocketTcp();
	//	return socketTcp;
	//}

	SocketTcp()
		:m_scheduler(Director::getInstance()->getScheduler())
	{
		Init();
	}

	//CCNetDelegate* getSocket() {
	//	return client;
	//}

	//void setSocket(CCNetDelegate* clit) {
	//	client = clit;
	//}

	void Connect(const char* ip, unsigned short port, ConnectCallBack fun);

protected:

	virtual bool Init();

	// will ccbuffer calling when a package is coming
	virtual void onMessageReceived(CCBuffer& oBuffer);

	// will char  calling when a package is coming
	virtual void onMessage(const char* oBuffer, int msgLen);

	// when connected will calling
	virtual void onConnected();

	// when connect time out will calling
	virtual void onConnectTimeout();

	// on disconnected will call
	virtual void onDisconnected();

	// on exception
	virtual void onExceptionCaught(CCSocketStatus eStatus);

	void updateOnce(float dt);

private:
	CCScheduler * m_scheduler;

	ConnectCallBack callback;

	//CCNetDelegate* client;
};