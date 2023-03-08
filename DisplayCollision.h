//--------------------------------------------------------------------------------------
// File: DisplayCollision.h
//
// コリジョン表示クラス（デバッグ用）
//
// Date: 2023.3.8
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <vector>

namespace Imase
{

	class DisplayCollision
	{
	private:

		// 球の情報
		struct Sphere
		{
			DirectX::SimpleMath::Vector3 center;	// 中心
			float radius;							// 半径
			DirectX::SimpleMath::Quaternion rotate;	// 回転
		};

		// ボックスの情報
		struct Box
		{
			DirectX::SimpleMath::Vector3 center;	// 中心
			DirectX::SimpleMath::Vector3 extents;	// 各面の中心からの距離.
		};

		// 表示可能なコリジョンの最大数
		static const uint32_t DISPLAY_COLLISION_MAX = 100;

		// 球のコリジョン情報
		std::vector<Sphere> m_spheres;

		// ボックスのコリジョン情報
		std::vector<Box> m_boxes;

		// プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

		// エフェクト（ライン用）
		std::unique_ptr<DirectX::BasicEffect> m_lineEffect;

		// 入力レイアウト（ライン用）
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_lineInputLayout;

		// 球のモデル
		std::unique_ptr<DirectX::GeometricPrimitive> m_modelSphere;

		// ボックスのモデル
		std::unique_ptr<DirectX::GeometricPrimitive> m_modelBox;

		// エフェクト（モデル用）
		std::unique_ptr<DirectX::NormalMapEffect> m_modelEffect;

		// 入力レイアウト（モデル用）
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_modelInputLayout;

		// インスタンス用頂点バッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_instancedVB;

	private:

		// 球のラインを描画する関数
		void DrawSphereLine(
			DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch,
			const Sphere& sphere
		);

	public:

		// コンストラクタ
		DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context);

		// 登録されたコリジョンの描画関数
		void DrawCollision(
			ID3D11DeviceContext* context,
			DirectX::CommonStates* states,
			const DirectX::SimpleMath::Matrix& view,
			const DirectX::SimpleMath::Matrix& proj,
			DirectX::SimpleMath::Color modelColor = DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,0.5f)
		);
		
		// 球のコリジョンを登録する関数
		void AddSphereCollision(
			DirectX::BoundingSphere shpere,
			DirectX::SimpleMath::Quaternion rotate = DirectX::SimpleMath::Quaternion::Identity
		)
		{
			Sphere data = { shpere.Center, shpere.Radius, rotate };
			m_spheres.push_back(data);
		}

		// ボックスのコリジョンを登録する関数
		void AddBoxCollision(DirectX::BoundingBox box)
		{
			Box data = { box.Center, box.Extents };
			m_boxes.push_back(data);
		}
	};

}


