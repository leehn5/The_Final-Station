#include "stdafx.h"
#include "NPCManager.h"

void NPCManager::Init()
{
}

void NPCManager::Release()
{
}

void NPCManager::Update()
{
	for (auto n : _mNPC)
		n.second->SetHunger(n.second->GetHungerSpeed() * TIMEMANAGER->getElapsedTime());
}

void NPCManager::Render()
{
}

void NPCManager::SetNPC(NPC_Type type, Vector2 pos)
{
	switch (type)
	{
		case NPC_Type::Alex:
		{
			NPC* npc = Object::CreateObject<NPCAlex>();
			npc->GetTrans()->SetPos(pos);
			npc->Init();
			npc->GetSprite()->SetPosition(pos + Vector2::down * 33);

			//_vNPC.push_back(make_pair(npc->GetName(), npc));
			_mNPC.insert(make_pair(npc->GetName(), npc));
			break;
		}

		case NPC_Type::Javis:
		{
			NPC* npc = Object::CreateObject<NPCJavis>();
			npc->GetTrans()->SetPos(pos);
			npc->Init();
			npc->GetSprite()->SetPosition(pos + Vector2::down * 33);

			//_vNPC.push_back(make_pair(npc->GetName(), npc));
			_mNPC.insert(make_pair(npc->GetName(), npc));
			break;
		}
	}
}

void NPCManager::DamageNPC_HP()
{
	for (auto n : _mNPC)
		n.second->DamageHP(n.second->GetHpDamageSpeed() * TIMEMANAGER->getElapsedTime() + n.second->GetAcceleration());
}

NPC* NPCManager::FindNPC(string name)
{
	map<string, NPC*>::iterator it = _mNPC.find(name);

	if(it != _mNPC.end()) return it->second;

	//for (int i = 0; i < _vNPC.size(); ++i)
	//{
	//	if (_vNPC[i].first == name) return _vNPC[i].second;
	//}
	return nullptr;
}
