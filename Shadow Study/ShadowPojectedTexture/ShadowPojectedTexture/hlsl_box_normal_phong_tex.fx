
// -------------------------------------------------------------
// ��������
// -------------------------------------------------------------
float4x4 mWorld;		// ���� ���
float4x4 mVP;		// ���ÿ��� �������������� ��ǥ��ȯ
float4x4 mWIT;
float4x4 mWVPT;
float3 vLightDir;
float3 vEyePos;


// ���� ���.
float4 I_a = {0.2f, 0.2f, 0.2f, 0.0f}; // ambient
float4 I_d = {0.8f, 0.8f, 0.8f, 0.0f}; // diffuse
float4 I_s = {1.f, 1.f, 1.f, 0.0f}; // diffuse

// �ݻ���
float4 K_a = {1.0f, 1.0f, 1.0f, 1.0f}; // ambient 
float4 K_d = {1.0f, 1.0f, 1.0f, 1.0f}; // diffuse


// ------------------------------------------------------------
// �ؽ�ó
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
// �׸��ڸ�
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
// �������̴����� �ȼ����̴��� �ѱ�� ������
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
// �������̴����� �ȼ����̴��� �ѱ�� ������
// -------------------------------------------------------------
struct VS_SHADOW_OUTPUT
{
	float4 Pos : POSITION;
	float4 Diffuse : COLOR0;
};

// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// �� + �׸���.
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
// 1�н�:�������̴�
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// ���� ��
	float3 L = -vLightDir;
	float3 N = normalize(mul(Normal, (float3x3)mWIT)); // ���� ��ǥ�迡���� ����.

	Out.Diffuse = I_a * K_a
						 + I_d * K_d * max(0, dot(N,L));

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;

	Out.Tex = Tex;

    return Out;
}


// -------------------------------------------------------------
// 1�н�:�ȼ����̴�
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
// 2�н�:�������̴�, �׸��� �� ����.
// -------------------------------------------------------------
VS_SHADOW_OUTPUT VS_pass1(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_SHADOW_OUTPUT Out = (VS_SHADOW_OUTPUT)0;  // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	Out.Diffuse = float4(1,1,1,1);

    return Out;
}



// -------------------------------------------------------------
// 3�н�:�������̴�, ���� ���, �׸��� ���.
// -------------------------------------------------------------
VS_OUTPUT2 VS_pass2(
      float4 Pos : POSITION,          // ������
	  float3 Normal : NORMAL,		// ��������
	  float2 Tex : TEXCOORD0
)
{
    VS_OUTPUT2 Out = (VS_OUTPUT2)0;        // ��µ�����
    
    // ��ǥ��ȯ
	float4x4 mWVP = mul(mWorld, mVP);
	Out.Pos = mul( Pos, mWVP );

	// ���� ���� ���.
	float3 N = normalize( mul(Normal, (float3x3)mWIT) ); // ���� ��ǥ�迡���� ����.

	Out.N = N;
	Out.Eye = vEyePos - Pos.xyz;
	Out.Tex = Tex;
	Out.TexShadow = mul( Pos, mWVPT );
    
    return Out;
}

// -------------------------------------------------------------
// 3�н�:�ȼ����̴�, ���� ���.
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
// ��ũ��
// -------------------------------------------------------------
technique TShader
{
    pass P0
    {
        // ���̴�
        VertexShader = compile vs_3_0 VS_pass0();
        PixelShader  = compile ps_3_0 PS_pass0();
    }

    pass P1
    {
        // �׸��� �� ���̴�
        VertexShader = compile vs_3_0 VS_pass1();
    }

    pass P2
    {
        // �׸��� �� ���̴�
        VertexShader = compile vs_3_0 VS_pass2();
		PixelShader  = compile ps_3_0 PS_pass2();
    }
}
