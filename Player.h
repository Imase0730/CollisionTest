#pragma once
#include "TaskManager.h"
#include "DeviceResources.h"

class Player : public Imase::Task
{
private:

	// �f�o�C�X���\�[�X�ւ̃|�C���^
	DX::DeviceResources* m_deviceResources;

	// ���ʃX�e�[�g�ւ̃|�C���^
	DirectX::CommonStates* m_states;

	// ���f��
	DirectX::Model* m_model;

	// �r���[�s��
	const DirectX::SimpleMath::Matrix* m_view;

	// �ˉe�s��
	const DirectX::SimpleMath::Matrix* m_proj;

	// �ʒu
	DirectX::SimpleMath::Vector3 m_pos;

	// �^�C�}�[
	int m_timer;

	// �^�X�N���E�����߂̃t���O
	bool m_kill;

public:

	// �R���X�g���N�^
	Player(
		DX::DeviceResources* deviceResources,
		DirectX::CommonStates* states,
		DirectX::Model* model);

	// �f�X�g���N�^
	~Player();

	// �X�V�֐�
	bool Update(float elapsedTime) override;

	// �`��֐�
	void Render() override;

	// �r���[�s���ݒ肷��֐�
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view);

	// �ˉe�s���ݒ肷��֐�
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& proj);

	// �^�X�N���E���֐�
	void Kill() { m_kill = true; }
};

