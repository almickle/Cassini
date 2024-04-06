struct particleData
{
    float3 x;
    float3 v;
};

StructuredBuffer<particleData> particle_input : register(t0);
RWStructuredBuffer<particleData> particle_output : register(u0);

[numthreads(10, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint index = DTid.x;
    
    particleData newData =
    {
        { particle_input[index].x * 2 },
        { particle_input[index].v * 2 }
    };
    
    particle_output[index] = newData;
}