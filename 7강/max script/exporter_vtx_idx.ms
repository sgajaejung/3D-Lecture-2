
global Outfile -- 저장될 file
global g_FilePath

global g_varSetMeshes = #()


fn ExporterStart = 
(
	Outfile = createfile g_FilePath

	for i=1 to g_varSetMeshes.count do
	(
		print g_varSetMeshes[i].name 
		
		-- vertex, index값은 EditMesh모드일때만 얻어올수 있어서 ConvertToMesh 함수를
		-- 사용했다. 문제는 EditMesh모드일때 VertexWeight값을 얻어올수 없다는것이다.
		-- 그래서 Vertex, index값을 얻은 후 undo명령어로 이전상태로 바꾼후 얻어온다.
		undo on
		(
			ConvertToMesh g_varSetMeshes[ i]

		)
		max undo
	)

	close Outfile

)



rollout ui "Exporter" width:387 height:266
(
	button btnRun "DX Exporter 실행" pos:[101,108] width:177 height:30

	-- 파일명 입력창
	label lbl1 "파일명:" pos:[30,13] width:70 height:17
	edittext editFilePath "" pos:[100,10] width:250 height:17 enabled:true

	-- UI 창이 열릴때 
	on ui open do
	(
		editFilePath.text = "model.dat"
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
