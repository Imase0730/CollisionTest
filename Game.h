//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "DisplayCollision.h"
#include "DebugCamera.h"
#include "DebugFont.h"
#include "TaskManager.h"
#include "Player.h"
#include "DebugDraw.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

private:

    // 共通ステート
    std::unique_ptr<DirectX::CommonStates> m_states;

    // エフェクトファクトリー
    std::unique_ptr<DirectX::EffectFactory> m_EffectFactory;

    // 床のモデル
    std::unique_ptr<DirectX::Model> m_floorModel;

    // カメラ（デバッグ用）
    std::unique_ptr<Imase::DebugCamera> m_debugCamera;

    // テスト用モデル
    std::unique_ptr<DirectX::Model> m_monkeyModel;

    // テスト用モデルの表示位置
    DirectX::SimpleMath::Vector3 m_monkeyPos;

    std::unique_ptr<DirectX::DGSLEffectFactory> m_DGSLEffectFactory;
    std::unique_ptr<DirectX::Model> m_shieldModel;


    // コリジョンを可視化するためのオブジェクト（デバッグ用）
    std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

    // デバッグ用フォント出力オブジェクト
    std::unique_ptr<Imase::DebugFont> m_font;

    // デバッグ用フォント出力オブジェクト（3D版）
    std::unique_ptr<Imase::DebugFont3D> m_font3D;

    // タスクマネージャー
    std::unique_ptr<Imase::TaskManager> m_taskManager;

    // プレイヤータスク
    Player* m_playerTask;

    // 床のモデル
    std::unique_ptr<DirectX::Model> m_model;

    // プリミティブバッチ
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;


};
