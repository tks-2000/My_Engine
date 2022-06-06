#pragma once

class TknFile;

namespace nsAI {
	class Cell
	{
	public:

		/// @brief 頂点座標を設定
		/// @param vertNo 
		/// @param pos 
		void SetVertexPosition(int vertNo, const Vector3& pos)
		{
			m_vertexPosition[vertNo] = pos;
		}

		/// @brief 法線を設定
		/// @param normal 設定する法線
		void SetNormal(const Vector3& normal)
		{
			m_normal = normal;
		}

		/// @brief 隣接セル情報を設定
		/// @param linkNo 隣接番号 (0～2)
		/// @param linkCell 隣接セル
		void SetLinkCell(int linkNo, Cell* linkCell) {
			m_linkCell[linkNo] = linkCell;
				
		}

		/// @brief セルの中心座標を設定
		/// @param centerPos 設定する中心座標
		void SetCenterPosition(const Vector3& centerPos)
		{
			m_centerPos = centerPos;
		}
		
		/// @brief セルの中心座標を入手
		/// @return セルの中心座標
		const Vector3& GetCenterPosition()const
		{
			return m_centerPos;
		}

		/// @brief セル番号を設定
		/// @param no 設定する番号
		void SetCellNo(int no)
		{
			m_no = no;
		}

		/// @brief セル番号を入手
		/// @return セル番号
		int GetCellNo() const
		{
			return m_no;
		}

		/// @brief 隣接セルを取得
		/// @param linkCellNo 隣接セル番号
		/// @return 
		Cell* GetLinkCell(int linkCellNo)const
		{
			return m_linkCell[linkCellNo];
		}

		/// @brief セルの頂点座標を入手
		/// @param vertNo 頂点番号
		/// @return 頂点座標
		const Vector3& GetVertexPosition(int vertNo) const
		{
			return m_vertexPosition[vertNo];
		}

	private:
		Vector3 m_vertexPosition[3];
		Vector3 m_normal;
		Vector3 m_centerPos;
		Cell* m_linkCell[3] = { nullptr };
		int m_no = 0;
	};

}