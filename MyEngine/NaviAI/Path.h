#pragma once

namespace nsAI {
	class NaviMesh;

	/// @brief Œo˜H‚ðˆÚ“®‚·‚éƒNƒ‰ƒX
	class Path
	{
	public:

		Vector3 Move(
			Vector3 pos,
			float moveSpeed,
			bool& isEnd
		);

		void Clear()
		{
			m_sectionNo = 0;
			m_pointArray.clear();
			m_sectionArray.clear();
		}

		void AddPoint(const Vector3& point)
		{
			m_pointArray.push_back(point);
		}

		void Build();

	private:
		struct StSection
		{
			Vector3 startPos;
			Vector3 endPos;
			Vector3 direction;
			float length;

		};
		std::vector<Vector3> m_pointArray;
		std::vector<StSection> m_sectionArray;
		int m_sectionNo = 0;
	};

}

