[[vk::binding(2, 0)]]
Texture2D uTexture : register(t2, space0);

[[vk::binding(3, 0)]]
SamplerState uSampler : register(s3, space0);

struct PSInput
{
    float4 Position : SV_Position;
    float2 UV       : TEXCOORD0;
    float4 Color    : COLOR0;
};

float4 main( PSInput input ) : SV_Target
{
    float4 texColor = uTexture.Sample( uSampler, input.UV );
    return texColor * input.Color;
}
