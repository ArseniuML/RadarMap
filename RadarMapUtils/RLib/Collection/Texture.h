#pragma once
namespace RLib {
	namespace Processing{class FramebufferTextureAttachment;}
	namespace Collection {

class Texture
{
public:
	Texture(GLint internalFormat, GLenum format, GLenum textureType):
	mTarget(GL_TEXTURE_RECTANGLE_ARB),
	mInternalFormat(internalFormat),
	mFormat(format),
	mTextureType(textureType),
	mWidth(0),
	mHeight(0)
	{
		glGenTextures(1, &mId);
		Filter(GL_NEAREST, GL_NEAREST);
		Wrap(GL_CLAMP, GL_CLAMP);
	}

	~Texture() 
	{
		glDeleteTextures(1, &mId);
	}

	inline void Bind()
	{
		glBindTexture(mTarget, mId);
	}

	inline void Unbind()
	{
		glBindTexture(mTarget, 0);
	}

	void Filter(GLint minFilter, GLint magFilter)
	{
		Bind();
		glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, magFilter);
		Unbind();
	}

	void Wrap(GLint s, GLint t)
	{
		Bind();
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, s);
		glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, t);
		Unbind();
	}

	void Resize(int width, int height)
	{
		if (width != mWidth || height != mHeight)
		{
			mWidth = width;
			mHeight = height;

			Bind();
			glTexImage2D(mTarget, 0, mInternalFormat, mWidth, mHeight, 0, mFormat, mTextureType, NULL);
			Unbind();
		}
	}

	void ToArray(void* pArray)
	{
		Bind();
		glGetTexImage(mTarget, 0, mFormat, mTextureType, pArray);
		Unbind();
	}

	void ToArray(void* pArray, GLenum format)
	{
		Bind();
		glGetTexImage(mTarget, 0, format, mTextureType, pArray);
		Unbind();
	}

	void FromArray(void* pArray)
	{
		Bind();
		glTexSubImage2D(mTarget, 0, 0, 0, mWidth, mHeight, mFormat, mTextureType, pArray);
		Unbind();
	}

	void FromArray(void* pArray, GLenum format)
	{
		Bind();
		glTexSubImage2D(mTarget, 0, 0, 0, mWidth, mHeight, format, mTextureType, pArray);
		Unbind();
	}

	inline void FromScreen()
	{
		Bind();
		glCopyTexImage2D(mTarget, 0, mInternalFormat, 0, 0, mWidth, mHeight, 0);
		Unbind();
	}
	

	int Width() {return mWidth;}
	int Height() {return mHeight;}
	
	GLenum Target() {return mTarget;}
	GLuint Id() {return mId;}

protected:
	GLenum mTarget; 
	GLint mInternalFormat;
	GLenum mFormat;
	GLenum mTextureType;

	GLuint mId;
	GLint mWidth, mHeight;
};

class TextureFloatR: public Texture {public: TextureFloatR():Texture(GL_LUMINANCE32F_ARB, GL_LUMINANCE, GL_FLOAT){}};
class TextureFloatRG: public Texture {public: TextureFloatRG():Texture(GL_FLOAT_RG32_NV, GL_RG, GL_FLOAT){}};
class TextureFloatRGB: public Texture {public: TextureFloatRGB():Texture(GL_RGB32F_ARB, GL_RGB, GL_FLOAT){}};
class TextureFloatRGBA: public Texture {public: TextureFloatRGBA():Texture(GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT){}};

class TextureHalfR: public Texture {public: TextureHalfR():Texture(GL_LUMINANCE16F_ARB, GL_LUMINANCE, GL_HALF_FLOAT_ARB){}};
class TextureHalfRGB: public Texture {public: TextureHalfRGB():Texture(GL_RGB16F_ARB, GL_RGB, GL_HALF_FLOAT_ARB){}};
class TextureUIntR: public Texture {public: TextureUIntR():Texture(GL_LUMINANCE32UI_EXT, GL_LUMINANCE, GL_UNSIGNED_INT){}};

class TextureRGB: public Texture {public: TextureRGB():Texture(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE){}};

	}
}
