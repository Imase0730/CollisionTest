#include "pch.h"
#include "Player.h"

using namespace DirectX;

Player::Player(
    DX::DeviceResources* deviceResources,
    DirectX::CommonStates* states,
    DirectX::Model* model)
    : m_deviceResources{}, m_states{}, m_pos{}, m_timer{}, m_view{}, m_proj{}, m_model{}
    , m_kill{false}
{
    m_deviceResources = deviceResources;
    m_states = states;
    m_model = model;

    // モデルの位置をランダムで決める
    m_pos.x = static_cast<float>(rand() % 100 - 50) / 10.0f;
    m_pos.z = static_cast<float>(rand() % 100 - 50) / 10.0f;

    SetName("Player");
}

Player::~Player()
{
}

bool Player::Update(float elapsedTime)
{
    //static float angle = 0.0f;

    //angle += elapsedTime;

    //m_pos.x = sinf(angle) * 2.0f;

    m_timer++;


    if (m_timer == 200)
    {
        // 新しいタスクを生成する
        Player* player = GetTaskManager()->AddTask<Player>(m_deviceResources, m_states, m_model);
        player->ChangeParent(GetTaskManager()->GetRootTask());
        player->SetViewMatrix(*m_view);
        player->SetProjectionMatrix(*m_proj);
        //this->UpdateTasks([](Imase::Task* task)
        //    {
        //        std::string name = task->GetName();
        //        std::wostringstream oss;
        //        oss << "Name:" << name.c_str() << std::endl;
        //        OutputDebugString(oss.str().c_str());
        //    }
        //);
    }
    if (m_timer > 300) return false;

    return !m_kill;
}

void Player::Render()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
 
    SimpleMath::Matrix world;

    // ワールド行列を作成
    world = SimpleMath::Matrix::CreateTranslation(m_pos);

    // モデルを描画する
    m_model->Draw(context, *m_states, world, *m_view, *m_proj);
}

void Player::SetViewMatrix(const DirectX::SimpleMath::Matrix& view)
{
    m_view = &view;
}

void Player::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& proj)
{
    m_proj = &proj;
}
