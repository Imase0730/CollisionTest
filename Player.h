#pragma once
#include "TaskManager.h"
#include "DeviceResources.h"

class Player : public Imase::Task
{
private:

	// デバイスリソースへのポインタ
	DX::DeviceResources* m_deviceResources;

	// 共通ステートへのポインタ
	DirectX::CommonStates* m_states;

	// モデル
	DirectX::Model* m_model;

	// ビュー行列
	const DirectX::SimpleMath::Matrix* m_view;

	// 射影行列
	const DirectX::SimpleMath::Matrix* m_proj;

	// 位置
	DirectX::SimpleMath::Vector3 m_pos;

	// タイマー
	int m_timer;

	// タスクを殺すためのフラグ
	bool m_kill;

public:

	// コンストラクタ
	Player(
		DX::DeviceResources* deviceResources,
		DirectX::CommonStates* states,
		DirectX::Model* model);

	// デストラクタ
	~Player();

	// 更新関数
	bool Update(float elapsedTime) override;

	// 描画関数
	void Render() override;

	// ビュー行列を設定する関数
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view);

	// 射影行列を設定する関数
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& proj);

	// タスクを殺す関数
	void Kill() { m_kill = true; }
};

