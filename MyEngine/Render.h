#pragma once

namespace render {

	struct RenderInitData
	{
		const char* shaderFilePath = nullptr;
	};

	class Render
	{
	public:
		Render();
		~Render();

		virtual void Init(const RenderInitData& initData);

		virtual void Execution(RenderContext& rc);
	};

}