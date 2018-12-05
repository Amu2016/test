#include"shadow.h"

Shadow* Shadow::create()
{
	Shadow* obj = new Shadow();
	if (obj && obj->init()) {
		obj->autorelease();
	}
	else
		CC_SAFE_DELETE(obj);
	return obj;
}

Shadow::~Shadow()
{
	free(_buffer);
	_buffer = nullptr;

	glDeleteBuffers(1, &_vbo);
	_vbo = 0;

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(0);
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
}

bool Shadow::init()
{
	//_blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;

	//setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));

	//create my own program
	auto program = new GLProgram;
	program->initWithFilenames("shader/shadow.vert", "shader/shadow.frag");
	//auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(program);
	//setGLProgramState(glprogramstate);
	program->link();
	//set uniform locations
	program->updateUniforms();

	setGLProgram(program);

	ensureCapacity(512);

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		glGenVertexArrays(1, &_vao);
		GL::bindVAO(_vao);
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
		// vertex
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		//// texcood
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));

		GL::bindVAO(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	else
	{
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	CHECK_GL_ERROR_DEBUG();

	_dirty = true;


	return true;
}


void Shadow::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	if (_bufferCount)
	{
		_customCommand.init(_globalZOrder, transform, flags);
		_customCommand.func = CC_CALLBACK_0(Shadow::onDraw, this, transform, flags);
		renderer->addCommand(&_customCommand);
	}
}

void Shadow::onDraw(const Mat4 &transform, uint32_t flags)
{
	auto glProgram = getGLProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins(transform);

	//GL::blendFunc(_blendFunc.src, _blendFunc.dst);

	if (_dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);

		_dirty = false;
	}
	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(_vao);
	}
	else
	{
		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// vertex
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
		// color
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
		// texcood
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
	}

	glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (Configuration::getInstance()->supportsShareableVAO())
	{
		GL::bindVAO(0);
	}

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
	CHECK_GL_ERROR_DEBUG();
}



void Shadow::ensureCapacity(int count)
{
	CCASSERT(count >= 0, "capacity must be >= 0");

	if (_bufferCount + count > _bufferCapacity)
	{
		_bufferCapacity += MAX(_bufferCapacity, count);
		_buffer = (V2F_C4B_T2F*)realloc(_buffer, _bufferCapacity * sizeof(V2F_C4B_T2F));
	}
}

void Shadow::drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Color4F &color)
{
	unsigned int vertex_count = 3;
	ensureCapacity(vertex_count);

	Color4B col = Color4B(color);
	V2F_C4B_T2F a = { Vec2(p1.x, p1.y), col, Tex2F(0.0,  0.0) };
	V2F_C4B_T2F b = { Vec2(p2.x, p2.y), col, Tex2F(0.0,  0.0) };
	V2F_C4B_T2F c = { Vec2(p3.x, p3.y), col, Tex2F(0.0,  0.0) };

	V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
	V2F_C4B_T2F_Triangle triangle = { a, b, c };
	triangles[0] = triangle;

	_bufferCount += vertex_count;
	_dirty = true;
}

void Shadow::clear()
{
	_bufferCount = 0;
	_dirty = true;
}