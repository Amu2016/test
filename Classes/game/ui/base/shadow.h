#pragma once

#include"stdafx.h"
#include "2d/CCNode.h"
#include "base/ccTypes.h"
#include "renderer/CCCustomCommand.h"
#include "math/CCMath.h"

class Shadow : public Node {
public:
	Shadow() 
		: _vao(0)
		, _vbo(0)
		, _bufferCapacity(0)
		, _bufferCount(0)
		, _buffer(nullptr)
		, _dirty(false)

	{}
	static Shadow* create();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

	virtual void onDraw(const Mat4 &transform, uint32_t flags);


	void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Color4F &color);


	void clear();

	virtual ~Shadow();
	virtual bool init() override;
protected:
	void ensureCapacity(int count);

	GLuint      _vao;
	GLuint      _vbo;

	int         _bufferCapacity;
	GLsizei     _bufferCount;
	V2F_C4B_T2F *_buffer;

	CustomCommand _customCommand;


	bool        _dirty;

private:


};