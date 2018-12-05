#pragma once
#include"stdafx.h"

class ShaderMgr {
public:
	static ShaderMgr* getInstance()
	{
		static ShaderMgr* shaderMgr = new ShaderMgr();
		return shaderMgr;
	}

	GLProgram* getGlProgram(char* url) {

	}

private:
	ShaderMgr()
	{
	}
};