
global Outfile -- 저장될 file
global g_FilePath

global g_varSetMeshes = #()

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
	for i=1 to obj.numverts do
	(
		v = getnormal obj i
		append varSetNormal v
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



fn CollectVtxNormTex obj = 
(
	varSetVtxNormTex = #()

	print obj.name
	print (classof obj)
	print "vertex count: " 
	print obj.numverts
	print "texture vertex count: " 
	print obj.numtverts
	print "vertex face count: "
	print obj.faces.count

	if (obj.numTVerts > obj.numVerts) then
	(
		print "obj.numTVerts > obj.numVerts"
		
		for i=1 to obj.numTVerts do
		--for i=1 to obj.numVerts do
		(
			local vnt = VtxNormTex #()
			vnt.tex = getTvert obj i
			append varSetVtxNormTex vnt
		)

		for i=1 to obj.faces.count do
		(
			local tvfaceidx = getTVFace obj i
			local faceidx = getFace obj i

			varSetVtxNormTex[ faceidx[ 1]].vtx = copy (getvert obj faceidx[ 1])
			varSetVtxNormTex[ faceidx[ 2]].vtx = copy (getvert obj faceidx[ 2])
			varSetVtxNormTex[ faceidx[ 3]].vtx = copy (getvert obj faceidx[ 3])

			
			local normal = normalize (getFaceNormal obj i)
			varSetVtxNormTex[ faceidx[ 1]].nrm = normal
			varSetVtxNormTex[ faceidx[ 2]].nrm = normal
			varSetVtxNormTex[ faceidx[ 3]].nrm = normal
		)
	)
	else
	(
		for i=1 to obj.numverts do
		(
			local vnt = VtxNormTex #()
			vnt.vtx = getvert obj i
			vnt.nrm = getnormal obj i
			vnt.tex = getTVert obj i
			append varSetVtxNormTex vnt
		)
	)

	varSetVtxNormTex
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

	format "NORMAL = %\n" varSetNormal.count to:Outfile
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

fn WriteVtxNormTex obj = 
(
	varSetVertice = CollectVtxNormTex obj
	
	format "VERTEX = %\n" varSetVertice.count to:Outfile
	for i=1 to varSetVertice.count do
	(
		vtx = (ConvertToD3D_POS varSetVertice[ i].vtx)
		format "% % %\n" vtx.x vtx.y vtx.z to:Outfile
	)

	format "NORMAL = %\n" varSetVertice.count to:Outfile
	for i=1 to varSetVertice.count do
	(
		nrm = (ConvertToD3D_POS varSetVertice[ i].nrm)
		format "% % %\n" nrm.x nrm.y nrm.z to:Outfile
	)
	
	format "TEX = %\n" varSetVertice.count to:Outfile
	for i=1 to varSetVertice.count do
	(
		uv = (ConvertToD3D_UV varSetVertice[ i].tex)
		format "% %\n" uv.x  uv.y to:Outfile
	)	

	format "INDEX = %\n"  obj.faces.count to:Outfile
	for i=1 to obj.faces.count do
	(
		local tvfaceidx = getTVFace obj i
		--local tvfaceidx = getFace obj i
		index = (ConvertToD3D_INDEX tvfaceidx)
		format "% % %\n" (index.x as integer) (index.y as integer) (index.z as integer) to:Outfile
	)
)


fn ExporterStart = 
(
	CollectObject()

	Outfile = createfile g_FilePath

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
		editFilePath.text = "data.txt"
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

