#include"MessagePool.h"

#include"bean/test_message.h"

MessagePool::MessagePool() {
	Add(100001, new test_ResMessage());
}