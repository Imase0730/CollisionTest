//--------------------------------------------------------------------------------------
// File: TaskManager.cpp
//
// タスクを管理するクラス
//
// Date: 2015.3.8
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "TaskManager.h"

// 更新関数
void TaskManager::Update(float elapsedTime)
{
	// ルートタスクから子供タスクに向かって再起的に更新処理を実行する
	ChildTaskUpdate(&m_rootTask, elapsedTime);
}

// 描画関数
void TaskManager::Render()
{
	// 描画順序管理テーブルに従ってタスクの描画関数を呼び出す
	std::multiset<Task*>::iterator p = m_ot.begin();
	while (p != m_ot.end())
	{
		(*p)->Render();
		p++;
	}
	// 描画順序管理テーブルクリア
	m_ot.clear();
}

// タスクの削除（子供タスクも全て死ぬ）
void TaskManager::DeleteTask(Task* task)
{
	// 子供タスクを再帰で削除していく
	while (task->GetTaskConnectInfo().GetChildList().empty() != true)
	{
		DeleteTask(task->GetTaskConnectInfo().GetChildList().back());
		task->GetTaskConnectInfo().GetChildList().pop_back();
	}
}

// 子供タスクの更新関数を再起呼び出しで実行する関数
void TaskManager::ChildTaskUpdate(Task* task, float elapsedTime)
{
	std::list<Task*>::iterator p = task->GetTaskConnectInfo().GetChildList().begin();
	while (p != task->GetTaskConnectInfo().GetChildList().end())
	{
		// Update関数の戻り値がfalseの場合はタスクを削除する
		if (!(*p)->Update(elapsedTime))
		{
			// タスクの削除
			DeleteTask(*p);
			// 子供タスクリストから削除
			p = task->GetTaskConnectInfo().GetChildList().erase(p);
			continue;
		}
		// 描画順序管理テーブルに登録
		m_ot.insert(*p);
		// 子供のタスクの更新処理を実行する
		ChildTaskUpdate(*p, elapsedTime);
		p++;
	}
}
