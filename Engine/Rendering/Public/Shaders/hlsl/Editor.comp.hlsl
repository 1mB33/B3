struct PushConstants
{
    float3 CameraPos;
    uint   _Padding0;
    int3   GridSize;
    uint   _Padding1;
    float3 CameraLookDir;
    uint   _Padding2;
    float3 CameraRight;
    uint   _Padding3;
    float3 CameraUp;
    uint   _Padding4;
    float  fFov;
    uint   uDebugMode;
    uint   _Padding6;
    uint   _Padding7;
};


RWTexture2D<float4> g_OutputImage : register( u0 );

#if defined( VULKAN )

[[vk::push_constant]]
PushConstants pc;

#else

cbuffer PushConstantsBuffer : register( b1 )
{
    PushConstants pc;
};

#endif

[ RootSignature( "DescriptorTable( UAV( u0 ) ) " ) ][ numthreads( 32, 8, 1 ) ] void
main( uint3 dispatchThreadId : SV_DispatchThreadID )
{
    return;
}
