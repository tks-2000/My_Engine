#include "stdafx.h"
#include "Test.h"

namespace test {
	
	Test::Test()
	{
		m_hero = NewGO<render::model::SkinModelRender>(0);
		m_shield = NewGO<render::model::SkinModelRender>(0);
		m_sword = NewGO<render::model::SkinModelRender>(0);

		m_heroAnim[enAnim1].Load("Assets/animData/hero/Hero_Idle.tka");
		m_heroAnim[enAnim1].SetLoopFlag(true);

		m_hero->InitDeferrd(
			"Assets/modelData/character/HeroPBR.tkm",
			false,
			"Assets/modelData/character/HeroPBR.tks",
			m_heroAnim,
			enAnimNum
		);

		//m_hero->SetScale({ 2.0f,2.0f,2.0f });
		m_shield->InitDeferrd("Assets/modelData/character/Hero_Shield.tkm");
		m_sword->InitDeferrd("Assets/modelData/character/Hero_Sword.tkm");
		//m_sword->Init("Assets/modelData/sphere/sphere.tkm");
		//m_sword->SetScale({ 0.1f,0.1f,0.1f });

		m_stage = NewGO <render::model::SkinModelRender>(0);
		m_stage->InitDeferrd("Assets/modelData/bg/testStage3.tkm");

	}

	Test::~Test()
	{

	}

	void Test::Update()
	{
		

		m_hero->SetPosition(m_pos);
		m_hero->SetRotation(m_qRot);

		m_angle += g_pad[0]->GetRStickXF()*0.05;

		m_qRot.SetRotationY(m_angle);

		m_hero->Execution();


		Skeleton* heroSkeleton = m_hero->GetSkeleton();

		int boneId = heroSkeleton->FindBoneID(L"hand_l");

		Bone* hand = heroSkeleton->GetBone(boneId);

		Matrix boneMatrix = hand->GetWorldMatrix();

		m_sword->SetWorldMatrix(boneMatrix);

		boneId = heroSkeleton->FindBoneID(L"hand_r");

		hand = heroSkeleton->GetBone(boneId);

		boneMatrix = hand->GetWorldMatrix();

		m_shield->SetWorldMatrix(boneMatrix);

		m_shield->Execution();
		m_sword->Execution();

		m_stage->Execution();

	}

}