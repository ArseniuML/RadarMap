#pragma once
namespace RLib {
	namespace Collection {

class Renderbuffer
{
public:
	Renderbuffer(GLenum internalFormat):
	mTarget(GL_RENDERBUFFER),
	mInternalFormat(internalFormat)
	{
		glGenRenderbuffers(1, &mId);
	}

	~Renderbuffer() 
	{
		glDeleteRenderbuffers(1, &mId);
	}

	inline void Bind()
	{
		glBindRenderbuffer(mTarget, mId);
	}

	inline void Unbind()
	{
		glBindRenderbuffer(mTarget, 0);
	}

	void Resize(int width, int height)
	{
		if (width != mWidth || height != mHeight)
		{
			mWidth = width;
			mHeight = height;

			Bind();
			glRenderbufferStorage(mTarget, mInternalFormat, mWidth, mHeight);
			Unbind();
		}
	}

	int Width() {return mWidth;}
	int Height() {return mHeight;}

	GLenum Target() {return mTarget;}
	GLuint Id() {return mId;}

protected:
	GLenum mTarget; 
	GLenum mInternalFormat;

	GLuint mId;
	GLint mWidth, mHeight;
};

class RenderbufferDepth32F: public Renderbuffer {public: RenderbufferDepth32F():Renderbuffer(GL_DEPTH_COMPONENT32F_NV){}};

	}
}
