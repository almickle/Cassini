
cbuffer buffer
{
    float a;
    float b;
    float c;
};

RWTexture3D<float4> field;

[numthreads(8, 8, 8)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    field[DTid] = float4( a, b, c, 1.0f );
}
