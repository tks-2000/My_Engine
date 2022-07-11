#pragma once
#include "RenderResource.h"

namespace render {

	class RenderingEngine;

	class ForewardRender {
	public:
		ForewardRender();
		~ForewardRender();

		void Init(const RenderInitData& initData);

		void Execution(RenderContext& rc);
	};

}