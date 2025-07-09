struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{    float4 texColor = gTexture.Sample(gSampler, input.uv);
    return texColor * input.color;
}
