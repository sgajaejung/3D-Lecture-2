
// -------------------------------------------------------------
// 전역변수
// -------------------------------------------------------------
float4x4 mWVP;		// 로컬에서 투영공간으로의 좌표변환
float4x4 mWIT;
float3 vLightDir; //(0,-1,0)



// 광원 밝기.
float4 I_a = {0.3f, 0.3f, 0.3f, 0.0f}; // ambient
float4 I_d = {0.7f, 0.7f, 0.7f, 0.0f}; // diffuse

// 반사율
float4 K_a = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient 
float4 K_d = {1.0f, 1.0f, 1.0f, 1.0f}; // diffuse


// -------------------------------------------------------------
// 정점셰이더에서 픽셀셰이더로 넘기는 데이터
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
};


// -------------------------------------------------------------
// 1패스:정점셰이더
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL		// 법선벡터
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
    // 좌표변환
	Out.Pos = mul( Pos, mWVP );

	// 정점 색
	float3 L = -vLightDir;
	float3 N = normalize( mul( float4(Normal,0), mWIT) ); // 월드 좌표계에서의 법선.
	
	Out.Diffuse = I_a * K_a + 
					I_d * K_d * max(0, dot(N,L));

    return Out;
}


// -------------------------------------------------------------
// 1패스:픽셀셰이더
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
    float4 Out;

	Out = In.Diffuse;

    return Out;
}


// -------------------------------------------------------------
// 테크닉
// -------------------------------------------------------------
technique TShader
{
    pass P0
    {
        // 셰이더
        VertexShader = compile vs_3_0 VS_pass0();
        PixelShader  = compile ps_3_0 PS_pass0();
    }

}
