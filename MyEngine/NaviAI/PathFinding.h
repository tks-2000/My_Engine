#pragma once

namespace nsAI{
	class Path;
	class Cell;
	class NaviMesh;

	/// @brief パス検索処理を行うクラス
	class PathFinding
	{
	public:
		void Execute(
			Path& path,
			const NaviMesh& naviMesh,
			const Vector3& startPos,
			const Vector3& endPos,
			float agentRadius = 50.0f,
			float agentHeight = 200.0f
		);
	private:
		struct CellWork
		{
			void Init(const Cell* cell);

			const Cell* cell;
			CellWork* parentCell;
			float costFromStartCell;
			Vector3 pathPoint;
			float cost;
			bool isOpend;
			bool isClosed;
			bool isSmooth;
		};
	private:
		void CalcCost(
			float& totalCost,
			float& costFromStartCell,
			const CellWork* nextCell,
			const CellWork* prevCell,
			const Cell* endCell
		);

		void Smoothing(
			std::list<CellWork*>& cellList,
			float agentRadius,
			float agentHeight
		);

		bool IsIntercetRayToCell(Vector3 rayStartPos, Vector3 rayEndPos, CellWork* currentCellWork) const;
	private:
		std::vector<CellWork> m_cellWork;
	};

}