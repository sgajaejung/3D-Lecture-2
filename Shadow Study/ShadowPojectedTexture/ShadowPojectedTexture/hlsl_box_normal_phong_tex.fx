
// -------------------------------------------------------------
// 전역변수
// -------------------------------------------------------------
float4x4 mWorld;		// 월드 행렬
float4x4 mVP;		// 로컬에서 투영공간으로의 좌표변환
float4x4 mWIT;
float4x4 mWVPT;
float3 vLightDir;
float3 vEyePos;


// 광원 밝기.
float4 I_a = {0.2f, 0.2f, 0.2f, 0.0f}; // ambient
float4 I_d = {0.8f, 0.8f, 0.8f, 0.0f}; // diffuse
float4 I_s = {1.f, 1.f, 1.f, 0.0f}; // diffuse

// 반사율
float4 K_a = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient 
float4 K_d = {1.0f, 1.0f, 1.0f, 1.0f}; // diffuse


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

    AddressU = Wrap;
    AddressV = Wrap;
};


// ------------------------------------------------------------
// 그림자맵
// ------------------------------------------------------------
texture ShadowMap;
sampler ShadowMapSamp = sampler_state
{
    Texture = <ShadowMap>;
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
	float2 Tex : TEXCOORD0;
	float3 Eye : TEXCOORD1;
	float3 N : TEXCOORD2;
};


// -------------------------------------------------------------
// 정점셰이더에서 픽셀셰이더로 넘기는 데이터
// -------------------------------------------------------------
struct VS_SHADOW_OUTPUT
{
	float4 Pos : POSITION;
	float4 Diffuse : COLOR0;
};

// -------------------------------------------------------------
// 정점셰이더에서 픽셀셰이더로 넘기는 데이터
// 모델 + 그림자.
// -------------------------------------------------------------
struct VS_OUTPUT2
{
    float4 Pos	 : POSITION;
	float4 Diffuse : COLOR0;
	float2 Tex : TEXCOORD0;
	float4 TexShadow : TEXCOORD1;
	float3 Eye : TEXCOORD2;
	float3 N : TEXCOORD3;
};





// -------------------------------------------------------------
// 1패스:정점셰이더
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
    
    // 좌표변환
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// 정점 색
	float3 L = -vLightDir;
	float3 N = normalize(mul(Normal, (float3x3)mWIT)); // 월드 좌표계에서의 법선.

	Out.Diffuse = I_a * K_a
						 + I_d * K_d * max(0, dot(N,L));

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;

	Out.Tex = Tex;

    return Out;
}


// -------------------------------------------------------------
// 1패스:픽셀셰이더
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
    float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	Out = In.Diffuse + I_s * pow( max(0, dot(N,H)), 32);
	//Out *= tex2D(Samp, In.Tex);

    return Out;
}


// -------------------------------------------------------------
// 2패스:정점셰이더, 그림자 맵 생성.
// -------------------------------------------------------------
VS_SHADOW_OUTPUT VS_pass1(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_SHADOW_OUTPUT Out = (VS_SHADOW_OUTPUT)0;  // 출력데이터
    
    // 좌표변환
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	Out.Diffuse = float4(1,1,1,1);

    return Out;
}



// -------------------------------------------------------------
// 3패스:정점셰이더, 포그 출력, 그림자 출력.
// -------------------------------------------------------------
VS_OUTPUT2 VS_pass2(
      float4 Pos : POSITION,          // 모델정점
	  float3 Normal : NORMAL,		// 법선벡터
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT2 Out = (VS_OUTPUT2)0;        // 출력데이터
    
    // 좌표변환
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// 법선 벡터 계산.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // 월드 좌표계에서의 법선.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
	Out.TexShadow = mul( Pos, mWVPT );
    
    return Out;
}

// -------------------------------------------------------------
// 3패스:픽셀셰이더, 포그 출력.
// -------------------------------------------------------------
float4 PS_pass2(VS_OUTPUT2 In) : COLOR
{
	float4 Out;

	float3 L = -vLightDir.xyz;
	float3 H = normalize(L + normalize(In.Eye));
	float3 N = normalize(In.N);

	float4 Color = 	I_a * K_a
				+ I_d * K_d * max(0, dot(N,L));
				+ I_s * pow( max(0, dot(N,H)), 16);

	float4 decale = tex2D(Samp, In.Tex);
	Out = Color * decale;

	float4 shadow = tex2Dproj( ShadowMapSamp, In.TexShadow );
	Out = Out * saturate(Color - (0.8f*shadow));

	//float distance = length(In.Eye);
	//float l = saturate((distance-vFog.x) / (vFog.y - vFog.x));
	//Out = lerp(Out, fogColor, l);

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

    pass P1
    {
        // 그림자 맵 셰이더
        VertexShader = compile vs_3_0 VS_pass1();
    }

    pass P2
    {
        // 그림자 맵 셰이더
        VertexShader = compile vs_3_0 VS_pass2();
		PixelShader  = compile ps_3_0 PS_pass2();
    }
}
