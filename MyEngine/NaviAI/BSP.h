#pragma once
class BSP
{
private:
	/// @brief 平面
	struct StPlane {
		Vector3 normal = g_vec3Zero;
		float distance;
	};
public:
	/// @brief BSPツリーの要素の種類
	enum EnEntityType {
		enEntityType_Node,
		enEntityType_Leaf,
		enEntityType_LeafList
	};

	/// @brief BSPツリーの要素
	struct StEntity {
		int type = enEntityType_Node;
	};

	using StEntityPtr = std::shared_ptr<StEntity>;

	/// @brief ノード
	struct StNode : public StEntity
	{
		StPlane plane;
		StEntityPtr rightEntity;
		StEntityPtr leftEntity;
		Vector3 centerPos;
		std::vector<StEntityPtr> leafArray;
	};

	/// @brief リーフ
	struct StLeaf : public StEntity
	{
		Vector3 position;
		void* extraData;
	};

	/// @brief リーフのリスト
	struct StLeafList : public StEntity {
		std::vector<StEntityPtr> leafList;
	};

private:
	StEntityPtr m_rootNode = nullptr;
	std::vector<StEntityPtr> m_leafArray;
public:
	/// @brief リーフを追加
	/// @param position 
	/// @param extraData 
	void AddLeaf(const Vector3& position, void* extraData)
	{
		auto newEntity = std::make_shared<StLeaf>();
		m_leafArray.emplace_back(newEntity);
		StLeaf* leaf = static_cast<StLeaf*>(newEntity.get());
		leaf->position = position;
		leaf->extraData = extraData;
		leaf->type = enEntityType_Leaf;

	}

	/// @brief BVHを構築
	void Build();

	/// @brief BSPツリーを探索してリーフに到達すると、引数で指定されたコールバック関数が呼ばれます。
	/// @param pos 座標
	/// @param onEndWalk 探索が終了した時に呼ばれる処理
	void WalkTree(const Vector3& pos, std::function<void(StLeaf* leaf)> onEndWalk) const;
private:

	/// @brief 平面でリーフノードを分割していく
	void SplitLeafArray(
		std::vector<StEntityPtr>& leftLeafArray,
		std::vector<StEntityPtr>& rightLeafArray,
		const StPlane& plane,
		const std::vector<StEntityPtr>& leafArray
	);

	void CalcSplitPlaneFromCovarianceMatrix(
		StPlane& plane,
		float covarianceMatrix[3][3],
		const Vector3& centerPos,
		const std::vector<StEntityPtr>& leafArray
	);

	/// @brief リーフノードの配列から共分散行列を計算する
	/// @param covarianceMatrix 共分散行列の計算先
	/// @param leafArray リーフノードの配列
	/// @param centerPos リーフノードの中心座標
	void CalcCovarianceMatrixFromLeafNodeList(
		float covarianceMatrix[3][3],
		const std::vector<StEntityPtr>& leafArray,
		const Vector3& centerPos
	);

	/// @brief リーフのリストから中心座標を計算する
	/// @param leafArray 
	/// @return 
	Vector3 CalcCenterPositionFromLeafList(
		const std::vector<StEntityPtr>& leafArray
	);

	/// @brief 新しいBSPツリーの要素を作成する
	/// @param leafArray 
	/// @return 
	StEntityPtr CreateBSPTreeEntity(const std::vector<StEntityPtr>& leafArray);

	/// @brief BSPツリーのLeafList要素を作成する
	/// @param leafArray 
	/// @return 
	StEntityPtr CreateBSPTreeEntity_LeafList(const std::vector<StEntityPtr>& leafArray);

	void WalkTree(StEntityPtr entity, const Vector3& pos, std::function<void(StLeaf* leaf)> onEndWalk) const;
};

