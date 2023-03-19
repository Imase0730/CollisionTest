//--------------------------------------------------------------------------------------
// File: TaskManager.h
//
// タスクを管理するクラス
//
// Date: 2023.3.20
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <list>
#include <set>

namespace Imase
{
	class TaskManager;
	class Task;

	//////////////////////////////////////////////////////////////////////////
	//																		//
	//	タスク連結情報管理クラス											//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	class TaskConnectInfo
	{
	private:
		// タスク管理オブジェクトへのポインタ
		TaskManager* m_taskManager;

		// 親タスクへのポインタ
		Task* m_parent;

		// 子供タスクのリスト
		std::list<Task*> m_child;

	public:
		TaskConnectInfo() : m_taskManager{}, m_parent{} {}
		TaskManager* GetTaskManager() const { return m_taskManager; }
		Task* GetParent() const { return m_parent; }
		std::list<Task*>* GetChildList() { return &m_child; }
		void SetTaskManager(TaskManager* taskManager) { m_taskManager = taskManager; }
		void SetParent(Task* task) { m_parent = task; }
		void AddChild(Task* task) { m_child.push_back(task); }
		bool DeleteChildList(Task* task);
	};

	//////////////////////////////////////////////////////////////////////////
	//																		//
	//	タスクの基底クラス													//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	class Task
	{
	private:
		// 描画順序管理用(小さいほど手前に描画される）
		int m_ot;

		// タスク連結情報
		TaskConnectInfo m_connect;

	public:

		TaskConnectInfo& GetTaskConnectInfo() { return m_connect; }

	public:
		Task() : m_ot(0) {}
		virtual ~Task() {}

		//////////////////////////////////////////
		//										//
		//	オーバーライドして使う関数			//
		//										//
		//////////////////////////////////////////
		virtual bool Update(float elapsedTime)
		{
			UNREFERENCED_PARAMETER(elapsedTime);
			return true;
		}
		virtual void Render() {}

	public:
		// タスクマネージャーの取得関数
		TaskManager* GetTaskManager() const { return m_connect.GetTaskManager(); }

		// 親タスクの取得関数
		Task* GetParent() const { return m_connect.GetParent(); }

		// 描画順設定関数（otの値が0が一番手前）
		void SetOt(int ot) { this->m_ot = ot; }

		// 描画順の取得関数
		int GetOt() { return m_ot; }

		// 指定したタスクの子供にする変更する関数
		void ChangeParent(Task* parent);

	};

	//////////////////////////////////////////////////////////////////////////
	//																		//
	//	タスク管理クラス													//
	//																		//
	//////////////////////////////////////////////////////////////////////////
	class TaskManager
	{

	private:
#pragma region
		// 描画順序管理テーブルへの登録用比較関数（otの値が大きい物が先頭へ）
		struct greater_ot : std::binary_function<Task*, Task*, bool>
		{
			bool operator()(Task* a, Task* b) const
			{
				return a->GetOt() > b->GetOt();
			}
		};

		// 描画順序管理テーブル
		std::multiset<Task*, greater_ot> m_ot;

		// ルートタスク
		Task* m_rootTask;

		// 実行中のタスク
		Task* m_currentTask;

		// タスクの削除関数
		void DeleteTask(Task* task);

		// 子供タスクの更新関数を実行する関数
		void ChildTaskUpdate(Task* task, float elapsedTime);
#pragma endregion

	public:
		// コンストラクタ
		TaskManager() : m_rootTask{}, m_currentTask {}
		{
			m_currentTask = m_rootTask = new Task();
		}

		// デストラクタ
		virtual ~TaskManager()
		{
			DeleteTask(m_rootTask);
		}

		// 更新関数
		virtual void Update(float elapsedTime);

		// 描画関数
		virtual void Render();

		// タスクの生成関数
		template <class T, class... Args>
		T* AddTask(Args&&... args);

		// ルートタスクを取得する関数
		Task* GetRootTask() { return m_rootTask; }

	};

#pragma region
	// タスクの生成関数（実行中のタスクの子供になる）
	template <class T, class... Args>
	T* TaskManager::AddTask(Args&&... args)
	{
		// タスクを生成する
		T* task = new T(std::forward<Args>(args)...);

		// タスク管理オブジェクトを設定
		task->GetTaskConnectInfo().SetTaskManager(this);

		// 親を設定
		task->GetTaskConnectInfo().SetParent(m_currentTask);

		// 親の子供リストに追加
		m_currentTask->GetTaskConnectInfo().AddChild(task);

		return task;
	}
#pragma endregion

}

