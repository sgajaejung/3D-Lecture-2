#pragma once

namespace graphic 
{
	struct sRawMesh;

	namespace importer 
	{

		bool ReadRawMeshFile( const string &fileName, OUT sRawMeshGroup &raw );

		bool ReadRawAnimationFile( const string &fileName, OUT sRawAniGroup &rawAni );

		RESOURCE_TYPE::TYPE GetFileKind( const string &fileName );

	}
}

