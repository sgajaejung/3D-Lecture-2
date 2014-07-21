
// -------------------------------------------------------------
// ��������
// -------------------------------------------------------------
float4x4 mWVP;		// ���ÿ��� �������������� ��ǥ��ȯ


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
};

struct VS_OUTPUT2
{
    float4 Pos	 : POSITION;
    float2 Tex : TEXCOORD1;
};


// -------------------------------------------------------------
// 1�н�:�������̴�
// -------------------------------------------------------------
VS_OUTPUT VS_pass0(
      float4 Pos    : POSITION,          // ������
	  float4 Diffuse : COLOR0			// ���� �÷�
)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
    
    // ��ǥ��ȯ
	float4 pos = mul( Pos, mWVP );
	
    // ��ġ��ǥ
    Out.Pos = pos;
	Out.Diffuse = Diffuse;
    
    return Out;
}


// -------------------------------------------------------------
// 1�н�:�ȼ����̴�
// -------------------------------------------------------------
float4 PS_pass0(VS_OUTPUT In) : COLOR
{
    float4 Out;

	Out = float4(0, 1, 0, 1);

    return Out;
}


// -------------------------------------------------------------
// 1�н�:�������̴�
// -------------------------------------------------------------
VS_OUTPUT2 VS_pass1(
      float4 Pos : POSITION,          // ������
	  float2 Tex : TEXCOORD0			// �ؽ��� ��ǥ
)
{
    VS_OUTPUT2 Out = (VS_OUTPUT2)0;        // ��µ�����
    
    // ��ǥ��ȯ
    float4 pos = mul( Pos, mWVP );
	
    // ��ġ��ǥ
    Out.Pos = pos;
    Out.Tex = Tex;
    
    return Out;
}


// -------------------------------------------------------------
// 1�н�:�ȼ����̴�
// -------------------------------------------------------------
float4 PS_pass1(VS_OUTPUT2 In) : COLOR
{
    float4 Out;

	Out = tex2D(Samp, In.Tex);

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
        VertexShader = compile vs_1_1 VS_pass0();
        PixelShader  = compile ps_2_0 PS_pass0();
    }

    pass P1
    {
        // ���̴�
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
    }
}


