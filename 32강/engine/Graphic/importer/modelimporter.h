#pragma once

namespace graphic 
{
	struct sRawMesh;

	namespace importer 
	{

		bool ReadRawMeshFile( const string &fileName, 
			OUT sRawMeshGroup &raw, OUT sRawAniGroup &rawAni );

	}
}
