#pragma once

namespace Mazel
{
	class MAZEL_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}