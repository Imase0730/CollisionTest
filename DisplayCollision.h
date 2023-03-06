#pragma once

#include <vector>

class DisplayCollision
{
public:

	// 球情報
	struct CollisionSphere
	{
		DirectX::SimpleMath::Vector3 center;	// 中心
		float radius;							// 半径
		DirectX::SimpleMath::Quaternion rotate;	// 回転
	};

private:

	// 
	std::vector<CollisionSphere> m_spheres;

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 球を描画する関数
	void DrawSphere(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch, const CollisionSphere& sphere);

	std::unique_ptr<DirectX::DGSLEffectFactory> m_effectFactory;
	std::unique_ptr<DirectX::Model> m_model;

public:

	// コンストラクタ
	DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context);

	// コリジョンを描画する関数
	void DrawCollision(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj);
		
	void AddColiision(DirectX::SimpleMath::Vector3 center, float radius, DirectX::SimpleMath::Quaternion rotate);

};

