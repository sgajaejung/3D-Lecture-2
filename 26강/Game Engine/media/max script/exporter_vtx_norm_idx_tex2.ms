
global Outfile -- 저장될 file
global g_FilePath

global g_varSetMeshes = #()
global g_varSetMaterials = #()


struct VtxNormTex
(
	vtx,
	nrm,
	tex
)


fn ConvertToD3D_POS max_pos =
(
	local d3d_pos = [0,0,0]
	d3d_pos.x = max_pos.x
	d3d_pos.y = max_pos.z
	d3d_pos.z = max_pos.y
	
	d3d_pos
)


fn ConvertToD3D_INDEX max_index =
(
	local d3d_index = [0,0,0]
	d3d_index.x = max_index.x - 1
	d3d_index.y = max_index.z - 1
	d3d_index.z = max_index.y - 1

	d3d_index
)

-- MAX의 텍스쳐 좌표를 D3D의 uv좌표로 바꿔준다.
fn ConvertToD3D_UV max_pos =
(
	if max_pos == undefined then return undefined
	local d3d_uv = [0,0]

	d3d_uv.x = max_pos.x
	d3d_uv.y = 1.0 - max_pos.y

	d3d_uv
)



fn CollectObject = 
(
	g_varSetMeshes = #()

	for i=1 to objects.count do
	(
		append g_varSetMeshes objects[i]
	)
)


fn CollectVertex obj = 
(
	varSetVertex= #()
	for i=1 to obj.numverts do
	(
		v = getvert obj i
		append varSetVertex v
	)
	varSetVertex
)


fn CollectNormals obj = 
(
	varSetNormal = #()
	for i=1 to obj.numfaces do
	(
		local normal = normalize (getFaceNormal obj i)
		append varSetNormal normal
	)
	varSetNormal
)


fn CollectIndex obj = 
(
	varSetIndex = #()
	for i=1 to obj.numfaces do
	(
		v = copy (getface obj i)
		append varSetIndex v
	)
	varSetIndex
)


fn WriteVertex obj = 
(
	varSetVertice = CollectVertex obj

	format "VERTEX = %\n" varSetVertice.count to:Outfile
	for i=1 to varSetVertice.count do
	(
		vtx = (ConvertToD3D_POS varSetVertice[ i]) 
		format "% % %\n" vtx.x vtx.y vtx.z to:Outfile
	)
)


fn WriteNormal obj = 
(
	varSetNormal = CollectNormals obj

	format "FACE-NORMAL = %\n" varSetNormal.count to:Outfile
	for i=1 to varSetNormal.count do
	(
		norm = (ConvertToD3D_POS varSetNormal[ i])
		format "% % %\n" norm.x norm.y norm.z to:Outfile
	)
)


fn WriteIndex obj = 
(
	varSetIndex = CollectIndex obj

	format "INDEX = %\n" varSetIndex.count to:Outfile
	for i=1 to varSetIndex.count do
	(
		index = (ConvertToD3D_INDEX varSetIndex[ i])
		format "% % %\n" (index.x as integer) (index.y as integer) (index.z as integer) to:Outfile
	)
)


fn WriteTVertTVFace obj = 
(
	format "TEXTURE-VERTEX = %\n" obj.numTVerts to:Outfile
	for i=1 to obj.numTVerts do
	(
		local tex = getTvert obj i
		local uv = (ConvertToD3D_UV tex)
		format "% %\n" uv.x  uv.y to:Outfile
	)
		
	format "TEXTURE-FACE = %\n" obj.faces.count to:Outfile	
	for i=1 to obj.faces.count do
	(
		local tvfaceidx = getTVFace obj i
		local index = (ConvertToD3D_INDEX tvfaceidx)
		format "% % %\n" (index.x as integer) (index.y as integer) (index.z as integer) to:Outfile
	)	
)


fn WriteVtxNormTex obj = 
(
	format "MATERIAL_ID % \n" ((GetMtrlId obj)-1) to:Outfile

	WriteVertex obj
	WriteIndex obj
	WriteNormal obj
	WriteTVertTVFace obj
)


fn GetMtrlId obj =
(
	if classOf obj.material == UndefinedClass then
	(
		return 0
	)
	
	if classOf obj.material == Standardmaterial then
	(
		for i=1 to g_varSetMaterials.count do
		(
			if obj.material == g_varSetMaterials[i] then
			(
				return (i as integer)
			)
		)
	)
	
	return 0
)



fn CollectMaterial = 
(
	temp = standard()
	Append g_varSetMaterials temp

	for i=1 to objects.count do
	(
		if objects[i].material == undefined do continue

		if classof objects[i].material == standardmaterial then
		(
			Append g_varSetMaterials objects[i].material
		)

		if classof objects[i].material == multimaterial then
		(
			for j=1 to objects[i].material.count do
			(
				Append g_varSetMaterials objects[i].material[j]
			)
		)
	)
)




fn WriteMaterial = 
(
	format "MATERIALGROUP = %\n" g_varSetMaterials.count to:Outfile

	for i=1 to g_varSetMaterials.count do
	(
		format "MATERIAL %\n" (i-1) to:Outfile

		format "DIFFUSE % % % 1\n"  (g_varSetMaterials[i].Diffuse.r/255) (g_varSetMaterials[i].Diffuse.g/255) (g_varSetMaterials[i].Diffuse.b/255) to:Outfile
		format "AMBIENT % % % 1\n"  (g_varSetMaterials[i].Ambient.r/255) (g_varSetMaterials[i].Ambient.g/255) (g_varSetMaterials[i].Ambient.b/255) to:Outfile
		format "SPECULAR % % % 1\n" (g_varSetMaterials[i].Specular.r/255) (g_varSetMaterials[i].Specular.g/255) (g_varSetMaterials[i].Specular.b/255) to:Outfile
		format "EMISSIVE % % % 0\n" (g_varSetMaterials[i].Diffuse.r/255) (g_varSetMaterials[i].Diffuse.g/255) (g_varSetMaterials[i].Diffuse.b/255) to:Outfile
		format "POWER %\n"  g_varSetMaterials[i].SpecularLevel to:Outfile

		-- Diffuse Map
		if g_varSetMaterials[i].DiffuseMap != undefined then
		(
			format "TEXTURE %\n" g_varSetMaterials[i].DiffuseMap.filename to:Outfile
		)
		else
		(
			format "TEXTURE  \n"  to:Outfile
		)
	)
)


fn ExporterStart = 
(
	CollectObject()
	CollectMaterial()

	Outfile = createfile g_FilePath

	WriteMaterial()

	for i=1 to g_varSetMeshes.count do
	(
		print g_varSetMeshes[i] 
		print g_varSetMeshes[i].name 
		
		-- vertex, index값은 EditMesh모드일때만 얻어올수 있어서 ConvertToMesh 함수를
		-- 사용했다. 문제는 EditMesh모드일때 VertexWeight값을 얻을 수 없어서,
		-- Vertex, index값을 얻은 후 undo명령어로 이전상태로 바꾼후 얻어온다.
		undo on
		(
			ConvertToMesh g_varSetMeshes[ i]
			WriteVtxNormTex g_varSetMeshes[ i]
			--WriteVertex g_varSetMeshes[ i]
			--WriteNormal g_varSetMeshes[ i]
			--WriteIndex g_varSetMeshes[ i]
		)
		max undo
	)

	close Outfile

)


rollout ui "냠냠" width:387 height:266
(
	button btnRun "DX Exporter 실행" pos:[101,108] width:177 height:30

	-- Bmm, Ani 파일명 입력창
	label lbl1 "파일명:" pos:[30,13] width:70 height:17
	edittext editFilePath "" pos:[100,10] width:250 height:17 enabled:true

	-- UI 창이 열릴때 
	on ui open do
	(
		editFilePath.text = "data.dat"
	)

	-- DirectX Export
	on btnRun pressed do
	(
		g_FilePath = editFilePath.text
		ExporterStart()
	)

)

MainUI = newRolloutFloater "Simple Exporter" 400 300
addrollout ui MainUI

