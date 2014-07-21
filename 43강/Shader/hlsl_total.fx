
// -------------------------------------------------------------
// 전역변수
// -------------------------------------------------------------
float4x4 mWVP;		// 로컬에서 투영공간으로의 좌표변환


// ------------------------------------------------------------
// 텍스처
// ------------------------------------------------------------
texture Tex;
sampler Samp = sampler_state
{
    Texture = <Tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};



// -------------------------------------------------------------
// 정점셰이더에서 픽셀셰이더로 넘기는 데이터
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
};

struct VS_OUTPUT2
{
    float4 Pos	 : POSITION;
    float2 Tex : TEXCOORD1;
};


// -------------------------------------------------------------
// 1패스:정점셰이더
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos    : POSITION,          // 모델정점
	  float4 Diffuse : COLOR0			// 정점 컬러
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
    // 좌표변환
	float4 pos = mul( Pos, mWVP );
	
    // 위치좌표
    Out.Pos = pos;
	Out.Diffuse = Diffuse;
    
    return Out;
}


// -------------------------------------------------------------
// 1패스:픽셀셰이더
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
    float4 Out;

	Out = float4(0, 1, 0, 1);

    return Out;
}


// -------------------------------------------------------------
// 1패스:정점셰이더
// -------------------------------------------------------------
VS_OUTPUT2 VS_pass1(
      float4 Pos : POSITION,          // 모델정점
	  float2 Tex : TEXCOORD0			// 텍스쳐 좌표
)
{
    VS_OUTPUT2 Out = (VS_OUTPUT2)0;        // 출력데이터
    
    // 좌표변환
    float4 pos = mul( Pos, mWVP );
	
    // 위치좌표
    Out.Pos = pos;
    Out.Tex = Tex;
    
    return Out;
}


// -------------------------------------------------------------
// 1패스:픽셀셰이더
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT2 In) : COLOR
{
    float4 Out;

	Out = tex2D(Samp, In.Tex);

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
        VertexShader = compile vs_1_1 VS_pass0();
        PixelShader  = compile ps_2_0 PS_pass0();
    }

    pass P1
    {
        // 셰이더
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
    }
}


