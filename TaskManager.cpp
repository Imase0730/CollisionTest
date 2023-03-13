//--------------------------------------------------------------------------------------
// File: TaskManager.cpp
//
// �^�X�N���Ǘ�����N���X
//
// Date: 2015.3.8
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "TaskManager.h"

// �X�V�֐�
void TaskManager::Update(float elapsedTime)
{
	// ���[�g�^�X�N����q���^�X�N�Ɍ������čċN�I�ɍX�V���������s����
	ChildTaskUpdate(&m_rootTask, elapsedTime);
}

// �`��֐�
void TaskManager::Render()
{
	// �`�揇���Ǘ��e�[�u���ɏ]���ă^�X�N�̕`��֐����Ăяo��
	std::multiset<Task*>::iterator p = m_ot.begin();
	while (p != m_ot.end())
	{
		(*p)->Render();
		p++;
	}
	// �`�揇���Ǘ��e�[�u���N���A
	m_ot.clear();
}

// �^�X�N�̍폜�i�q���^�X�N���S�Ď��ʁj
void TaskManager::DeleteTask(Task* task)
{
	// �q���^�X�N���ċA�ō폜���Ă���
	while (task->GetTaskConnectInfo().GetChildList().empty() != true)
	{
		DeleteTask(task->GetTaskConnectInfo().GetChildList().back());
		task->GetTaskConnectInfo().GetChildList().pop_back();
	}
}

// �q���^�X�N�̍X�V�֐����ċN�Ăяo���Ŏ��s����֐�
void TaskManager::ChildTaskUpdate(Task* task, float elapsedTime)
{
	std::list<Task*>::iterator p = task->GetTaskConnectInfo().GetChildList().begin();
	while (p != task->GetTaskConnectInfo().GetChildList().end())
	{
		// Update�֐��̖߂�l��false�̏ꍇ�̓^�X�N���폜����
		if (!(*p)->Update(elapsedTime))
		{
			// �^�X�N�̍폜
			DeleteTask(*p);
			// �q���^�X�N���X�g����폜
			p = task->GetTaskConnectInfo().GetChildList().erase(p);
			continue;
		}
		// �`�揇���Ǘ��e�[�u���ɓo�^
		m_ot.insert(*p);
		// �q���̃^�X�N�̍X�V���������s����
		ChildTaskUpdate(*p, elapsedTime);
		p++;
	}
}
