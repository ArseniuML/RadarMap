#pragma once

#include <RLib/Collection/Texture.h>
#include <RLib/Collection/Renderbuffer.h>

namespace RLib {
	namespace Processing {

class TextureSource
{
public:
	TextureSource(Collection::Texture& tex, GLenum block = GL_TEXTURE0):
	mTex(tex),
	mBlock(block)
	{
		glActiveTexture(mBlock);
		mTex.Bind();
	}

	~TextureSource()
	{
		glActiveTexture(mBlock);
		mTex.Unbind();
	}

private:
	Collection::Texture& mTex;
	GLenum mBlock;
};

class FramebufferTextureAttachment
{
public:
	FramebufferTextureAttachment(Collection::Texture& tex, GLenum attachment = GL_COLOR_ATTACHMENT0):
	mTex(tex),
	mTarget(GL_DRAW_FRAMEBUFFER),
	mAttachment(attachment)
	{
		glViewport(0, 0, mTex.Width(), mTex.Height());
		glFramebufferTexture2D(mTarget, mAttachment, mTex.Target(), mTex.Id(), 0);
	}

	~FramebufferTextureAttachment()
	{
		glFramebufferTexture2D(mTarget, mAttachment, mTex.Target(), 0, 0);
	}

private:
	Collection::Texture& mTex;
	GLenum mTarget;
	GLenum mAttachment;
};

class FramebufferRenderbufferAttachment
{
public:
	FramebufferRenderbufferAttachment(Collection::Renderbuffer& renderbuffer, GLenum attachment = GL_COLOR_ATTACHMENT0):
	mRenderbuffer(renderbuffer),
	mTarget(GL_DRAW_FRAMEBUFFER),
	mAttachment(attachment)
	{
		glViewport(0, 0, mRenderbuffer.Width(), mRenderbuffer.Height());
		glFramebufferRenderbuffer(mTarget, mAttachment, mRenderbuffer.Target(), mRenderbuffer.Id());
	}

	~FramebufferRenderbufferAttachment()
	{
		glFramebufferRenderbuffer(mTarget, mAttachment, mRenderbuffer.Target(), 0);
	}

private:
	Collection::Renderbuffer& mRenderbuffer;
	GLenum mTarget;
	GLenum mAttachment;
};

	}
}