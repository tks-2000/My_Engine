#pragma once

namespace test {
	class Test : public IGameObject
	{
	public:
		Test();
		~Test();

		void Update();
	private:


		render::model::SkinModelRender* m_hero = nullptr;
		render::model::SkinModelRender* m_shield = nullptr;
		render::model::SkinModelRender* m_sword = nullptr;
		render::model::SkinModelRender* m_stage = nullptr;

		Vector3 m_pos = g_vec3Zero;

		Quaternion m_qRot = Quaternion::Identity;

		float m_angle = 0.0f;

		enum EnTestAnim {
			enAnim1,
			enAnimNum
		};

		AnimationClip m_heroAnim[enAnimNum];
	};
}
