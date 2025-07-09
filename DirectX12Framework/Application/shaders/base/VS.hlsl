struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct PassData
{
    float4x4 viewproj;
    float4x4 world;
};

ConstantBuffer<PassData> gPassData : register(b0);


VS_OUTPUT main(VS_INPUT input)
{

    VS_OUTPUT output;
    float4 worldPos = mul(gPassData.world, float4(input.position,1.0f));
    output.position = mul(gPassData.viewproj, worldPos);
    output.color = input.color;
    output.uv = input.uv;
    
    return output;}