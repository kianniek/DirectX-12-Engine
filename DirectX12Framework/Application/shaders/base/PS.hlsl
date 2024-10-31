struct VS_INPUT //From VS_OUTPUT
{
	
    float4 position : SV_POSITION;
    float3 color : COLOR;
	
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.color;
}