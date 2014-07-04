
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


// ��带 �ڽ����� �߰��Ѵ�.
bool cNode::InsertChild(cNode *node)
{
	RETV(!node, false);
	RETV(FindNode(node->GetId()), false); // Already Exist
	
	node->m_parent = this;
	m_children.push_back(node);
	return true;
}


// �ڽ� ��� �߿� id �� ���� ��尡 �ִٸ� �����Ѵ�.
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


// id ��带 �����Ѵ�. �޸𸮱��� ���ŵȴ�.
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


// ��� ��带 �����Ѵ�.
void cNode::Clear()
{
	BOOST_FOREACH (auto node, m_children)
	{
		node->Clear();
		delete node;
	}

	m_children.clear();
}
