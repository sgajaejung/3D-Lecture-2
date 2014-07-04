
#include "stdafx.h"
#include "node.h"

using namespace graphic;


cNode::cNode( const int id) : 
	m_id(id)
,	m_parent(NULL)
{

}

cNode::~cNode()
{
	Clear();
}


// 노드를 자식으로 추가한다.
bool cNode::InsertChild(cNode *node)
{
	RETV(!node, false);
	RETV(FindNode(node->GetId()), false); // Already Exist
	
	node->m_parent = this;
	m_children.push_back(node);
	return true;
}


// 자식 노드 중에 id 와 같은 노드가 있다면 리턴한다.
cNode* cNode::FindNode(const int id)
{
	if (GetId() == id)
		return this;

	BOOST_FOREACH (auto node, m_children)
	{
		if (cNode *ret = node->FindNode(id))
			return ret;
	}

	return NULL;
}


// id 노드를 제거한다. 메모리까지 제거된다.
bool cNode::RemoveNode(const int id)
{
	BOOST_FOREACH (auto node, m_children)
	{
		if (node->GetId() == id)
		{
			delete node;
			common::removevector(m_children, node);
			return true;
		}
	}

	BOOST_FOREACH (auto node, m_children)
	{
		if (node->RemoveNode(id))
			return true;
	}

	return false;
}


// 모든 노드를 제거한다.
void cNode::Clear()
{
	BOOST_FOREACH (auto node, m_children)
	{
		node->Clear();
		delete node;
	}

	m_children.clear();
}
