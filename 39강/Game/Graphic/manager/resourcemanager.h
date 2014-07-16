#pragma  once


namespace graphic
{
	struct sRawMesh;
	struct sRawAni;
	struct sRawBone;


	class cResourceManager : public common::cSingleton<cResourceManager>
	{
	public:
		cResourceManager();
		virtual ~cResourceManager();
		
		sRawMeshGroup* LoadModel( const string &fileName );
		sRawAniGroup* LoadAnimation( const string &fileName );
		cTexture* LoadTexture( const string &fileName, const bool isSizePow2=true );

		sRawMeshGroup* FindModel( const string &fileName );
		sRawAniGroup* FindAnimation( const string &fileName );
		cTexture* FindTexture( const string &fileName );
		void Clear();

		RESOURCE_TYPE::TYPE GetFileKind( const string &fileName );


	private:
		map<string, sRawMeshGroup*> m_meshes; // key = fileName
		map<string, sRawAniGroup*> m_anies;	// key = fileName
		map<string, cTexture*> m_textures; // key = fileName
	};

}
