#pragma once

#include <vector>

class DisplayCollision
{
public:

	// �����
	struct CollisionSphere
	{
		DirectX::SimpleMath::Vector3 center;	// ���S
		float radius;							// ���a
		DirectX::SimpleMath::Quaternion rotate;	// ��]
	};

private:

	// 
	std::vector<CollisionSphere> m_spheres;

	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// ����`�悷��֐�
	void DrawSphere(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch, const CollisionSphere& sphere);

	std::unique_ptr<DirectX::DGSLEffectFactory> m_effectFactory;
	std::unique_ptr<DirectX::Model> m_model;

public:

	// �R���X�g���N�^
	DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context);

	// �R���W������`�悷��֐�
	void DrawCollision(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj);
		
	void AddColiision(DirectX::SimpleMath::Vector3 center, float radius, DirectX::SimpleMath::Quaternion rotate);

};

