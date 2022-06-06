#include "stdafx.h"
#include "Path.h"

namespace nsAI {
	void Path::Build()
	{
		m_sectionArray.resize(m_pointArray.size() - 1);

		for (int pointNo = 0; pointNo < m_pointArray.size() - 1; pointNo++) {
			auto& section = m_sectionArray.at(pointNo);
			section.startPos = m_pointArray.at(pointNo);
			section.endPos = m_pointArray.at(pointNo + 1);
			section.direction = section.endPos - section.startPos;
			section.length = section.direction.Length();
			section.direction.Normalize();
		}
	}

	Vector3 Path::Move(
		Vector3 pos,
		float moveSpeed,
		bool& isEnd
	)
	{
		if (m_sectionArray.empty()
			|| m_sectionNo >= m_sectionArray.size()
			)
		{
			return pos;
		}
		StSection& currentSection = m_sectionArray.at(m_sectionNo);

		Vector3 toEnd = currentSection.endPos - pos;
		toEnd.Normalize();
		pos += toEnd * moveSpeed;

		Vector3 toEnd2 = currentSection.endPos - pos;
		toEnd2.Normalize();

		Vector3 toEnd3 = currentSection.endPos - pos;

		if (toEnd3.Length() < 50.0f) {
			pos = currentSection.endPos;
			if (m_sectionNo == m_sectionArray.size() - 1) {
				isEnd = true;
			}
			else {
				m_sectionNo++;
			}
		}
		return pos;
	}
}