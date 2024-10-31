struct VS_INPUT
{
    float3 position : POSITION;
    float3 color : COLOR;
    
};


struct VS_OUTPUT
{
	
    float4 position : SV_POSITION;
    float3 color : COLOR;
	
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //float3 worldPos = input.position;
    //worldPos = mul(gObjectData.transform, float4(worldPos, 1.0f)).xyz;

    //output.position = mul(gPassData.viewproj, float4(worldPos, 1.0f));
    //output.normal = mul((float3x3) gObjectData.transform, input.normal);
    
    return output;
}