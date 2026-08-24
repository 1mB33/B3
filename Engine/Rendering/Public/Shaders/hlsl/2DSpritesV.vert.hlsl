#define MAX_SPRITES 1024

struct VSInput
{
    [[vk::location(0)]] float2 Position : POSITION0;
    uint Id : SV_InstanceID;
};

struct SpriteInstance 
{
    float3 Position;
    uint   _PADDING0;
    float3 Color;
    uint   _PADDING1;
    float3 Scale;
    uint   _PADDING2;
};

StructuredBuffer<SpriteInstance> g_Instances : register( t1 );

struct VSOutput
{
    float4 Position     : SV_Position;
    float4 Color        : COLOR0;
};

struct PushConstants 
{
    float2 Dims;
    float2 _PADDING0;
};

#if defined( VULKAN )

[[vk::push_constant]]
PushConstants pc;

#else

cbuffer PushConstantsBuffer : register( b1 )
{
    PushConstants pc;
};

#endif

VSOutput main( VSInput input )
{
    float aspectRatio = pc.Dims.x / pc.Dims.y;
    printf("My float is %f", aspectRatio);

    float2 normalizedPos = (g_Instances[input.Id].Position.xy * 2) - 1;
    normalizedPos.y = -normalizedPos.y;

    VSOutput output;
    output.Position = float4( normalizedPos.x + (input.Position.x * g_Instances[input.Id].Scale.x),
                              normalizedPos.y + (input.Position.y * g_Instances[input.Id].Scale.y * aspectRatio),
                              g_Instances[input.Id].Position.z,
                              1.0 );

    output.Color = float4(g_Instances[input.Id].Color.xyz, 1.);

    return output;
}

