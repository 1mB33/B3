#define MAX_SPRITES 1024

struct VSInput
{
    [[vk::location(0)]] float2 Position : POSITION0;
};

struct VSOutput
{
    float4 Position     : SV_Position;
    float4 Color        : COLOR0;
};

VSOutput main( VSInput input )
{
    VSOutput output;
    output.Position = float4( input.Position, 0.0, 1.0 );;
    output.Color    = float4(1., 1., 1., 1.);

    return output;
}

