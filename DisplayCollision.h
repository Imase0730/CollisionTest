//--------------------------------------------------------------------------------------
// File: DisplayCollision.h
//
// コリジョン表示クラス（デバッグ用）
//
// Usage: AddBoundingSphereとAddBoundingBoxで登録してください。
//        DrawCollision関数で表示します。登録された情報は描画後クリアされます。
//        モデル情報の衝突判定用のコリジョン情報の表示などに使用してください。
//        ボックスは回転には対応していません。（AABBの衝突判定用）
//
// Date: 2023.3.8
// Author: Hideyasu Imase
//
//--------------------------------------------------------------------------------------
#pragma once

#include <vector>
#include "SimpleMath.h"
#include "CommonStates.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "DirectXHelpers.h"

namespace Imase
{

	class DisplayCollision
	{
	private:

		// 表示可能なコリジョンの最大数
		static const uint32_t DISPLAY_COLLISION_MAX = 100;

		// 球の情報
		struct Sphere
		{
			DirectX::SimpleMath::Vector3 center;	// 中心
			float radius;							// 半径

			constexpr Sphere(const DirectX::SimpleMath::Vector3& center, float radius) noexcept
				: center(center), radius(radius) {}
		};

		// ボックスの情報
		struct Box
		{
			DirectX::SimpleMath::Vector3 center;	// 中心
			DirectX::SimpleMath::Vector3 extents;	// 各面の中心からの距離.

			constexpr Box(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& extents) noexcept
				: center(center), extents(extents) {}
		};

		// 球のコリジョン情報
		std::vector<Sphere> m_spheres;

		// ボックスのコリジョン情報
		std::vector<Box> m_boxes;

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

	public:

		// コンストラクタ
		DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context);

		// 登録されたコリジョンの描画関数
		void DrawCollision(
			ID3D11DeviceContext* context,
			DirectX::CommonStates* states,
			const DirectX::SimpleMath::Matrix& view,
			const DirectX::SimpleMath::Matrix& proj,
			DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,0.5f)
		);
		
		// 球のコリジョンを登録する関数
		void AddBoundingSphere(DirectX::BoundingSphere shpere, DirectX::SimpleMath::Vector3 pos)
		{
			DirectX::XMFLOAT3 center = shpere.Center + pos;
			m_spheres.push_back(Sphere(center, shpere.Radius));
		}

		// ボックスのコリジョンを登録する関数
		void AddBoundingBox(DirectX::BoundingBox box, DirectX::SimpleMath::Vector3 pos)
		{
			DirectX::XMFLOAT3 center = box.Center + pos;
			m_boxes.push_back(Box(center, box.Extents));
		}

	};

}


