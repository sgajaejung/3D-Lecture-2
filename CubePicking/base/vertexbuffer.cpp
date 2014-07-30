
#include "base.h"
#include "vertexbuffer.h"

using namespace graphic;


cVertexBuffer::cVertexBuffer() : 
	m_pVtxBuff(NULL)
{

}

cVertexBuffer::~cVertexBuffer()
{
	Clear();
}


bool cVertexBuffer::Create(int vertexCount, int sizeofVertex, DWORD fvf)
{
	if (FAILED(GetDevice()->CreateVertexBuffer( vertexCount*sizeofVertex,
		D3DUSAGE_WRITEONLY, 
		fvf,
		D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return false;
	}

	m_fvf = fvf;
	m_vertexCount = vertexCount;
	m_sizeOfVertex = sizeofVertex;
	return true;
}


void* cVertexBuffer::Lock()
{
	if (!m_pVtxBuff)
		return NULL;

	void *vertices = NULL;
	if (FAILED(m_pVtxBuff->Lock( 0, 0, (void**)&vertices, 0)))
		return NULL;

	return vertices;
}


void cVertexBuffer::Unlock()
{
	if (!m_pVtxBuff)
		return;

	m_pVtxBuff->Unlock();
}


void cVertexBuffer::Bind() const
{
	GetDevice()->SetStreamSource( 0, m_pVtxBuff, 0, m_sizeOfVertex );
	GetDevice()->SetFVF( m_fvf );
}


void cVertexBuffer::Clear()
{
	SAFE_RELEASE(m_pVtxBuff);	
}
