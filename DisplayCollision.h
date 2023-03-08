//--------------------------------------------------------------------------------------
// File: DisplayCollision.h
//
// �R���W�����\���N���X�i�f�o�b�O�p�j
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

		// ���̏��
		struct Sphere
		{
			DirectX::SimpleMath::Vector3 center;	// ���S
			float radius;							// ���a
			DirectX::SimpleMath::Quaternion rotate;	// ��]
		};

		// �{�b�N�X�̏��
		struct Box
		{
			DirectX::SimpleMath::Vector3 center;	// ���S
			DirectX::SimpleMath::Vector3 extents;	// �e�ʂ̒��S����̋���.
		};

		// �\���\�ȃR���W�����̍ő吔
		static const uint32_t DISPLAY_COLLISION_MAX = 100;

		// ���̃R���W�������
		std::vector<Sphere> m_spheres;

		// �{�b�N�X�̃R���W�������
		std::vector<Box> m_boxes;

		// �v���~�e�B�u�o�b�`
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

		// �G�t�F�N�g�i���C���p�j
		std::unique_ptr<DirectX::BasicEffect> m_lineEffect;

		// ���̓��C�A�E�g�i���C���p�j
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_lineInputLayout;

		// ���̃��f��
		std::unique_ptr<DirectX::GeometricPrimitive> m_modelSphere;

		// �{�b�N�X�̃��f��
		std::unique_ptr<DirectX::GeometricPrimitive> m_modelBox;

		// �G�t�F�N�g�i���f���p�j
		std::unique_ptr<DirectX::NormalMapEffect> m_modelEffect;

		// ���̓��C�A�E�g�i���f���p�j
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_modelInputLayout;

		// �C���X�^���X�p���_�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_instancedVB;

	private:

		// ���̃��C����`�悷��֐�
		void DrawSphereLine(
			DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch,
			const Sphere& sphere
		);

	public:

		// �R���X�g���N�^
		DisplayCollision(ID3D11Device* device, ID3D11DeviceContext* context);

		// �o�^���ꂽ�R���W�����̕`��֐�
		void DrawCollision(
			ID3D11DeviceContext* context,
			DirectX::CommonStates* states,
			const DirectX::SimpleMath::Matrix& view,
			const DirectX::SimpleMath::Matrix& proj,
			DirectX::SimpleMath::Color modelColor = DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,0.5f)
		);
		
		// ���̃R���W������o�^����֐�
		void AddSphereCollision(
			DirectX::BoundingSphere shpere,
			DirectX::SimpleMath::Quaternion rotate = DirectX::SimpleMath::Quaternion::Identity
		)
		{
			Sphere data = { shpere.Center, shpere.Radius, rotate };
			m_spheres.push_back(data);
		}

		// �{�b�N�X�̃R���W������o�^����֐�
		void AddBoxCollision(DirectX::BoundingBox box)
		{
			Box data = { box.Center, box.Extents };
			m_boxes.push_back(data);
		}
	};

}


