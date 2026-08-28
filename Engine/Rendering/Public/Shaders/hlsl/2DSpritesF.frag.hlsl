struct PSInput
{
    float4 Color : COLOR0;
};

float4 main( PSInput input ) : SV_Target0
{
    return input.Color;
}

