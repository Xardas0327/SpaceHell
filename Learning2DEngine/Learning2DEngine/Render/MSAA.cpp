#include "MSAA.h"

#include "RenderManager.h"
#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
	namespace Render
	{
		MSAA::MSAA()
			: fbo(0), rbo(0), sampleNumber(0), resolution(0,0)
		{

		}

		void MSAA::Init(unsigned int sampleNumber, Resolution resolution)
		{
			this->sampleNumber = sampleNumber;
			this->resolution = resolution;
			glGenFramebuffers(1, &fbo);
			glGenRenderbuffers(1, &rbo);

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleNumber, GL_RGB, resolution.GetWidth(), resolution.GetHeight());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				L2DE_LOG_ERROR("MSAA: Failed to initialize framebuffer.");
			}
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void MSAA::Destroy()
		{
			glDeleteRenderbuffers(1, &rbo);
			glDeleteFramebuffers(1, &fbo);
			sampleNumber = 0;
		}

		void MSAA::StartRender(bool useClear)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			if (useClear)
				RenderManager::GetInstance().ClearWindow();
		}

		void MSAA::EndRender(unsigned int resultFbo, Resolution resultResolution)
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resultFbo);
			glBlitFramebuffer(0, 0, resolution.GetWidth(), resolution.GetHeight(), 
				0, 0, resultResolution.GetWidth(), resultResolution.GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}