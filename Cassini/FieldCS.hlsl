
struct particle
{
    float3 pos;
    float charge;
};

cbuffer buffer
{
    particle ptcls[6];
    uint size;
};

RWTexture3D<float4> field;

static float k = 8.9876e+9; // N*m^2 / C^2
static float max = 2e+9;

[numthreads(10, 10, 10)]
void main( uint3 coord : SV_DispatchThreadID )
{
    float3 fieldIntensity = { 0.0f, 0.0f, 0.0f };
    for (int i = 0; i < size; i++)
    {
        float dist = distance(coord, ptcls[i].pos);
        float3 dir = normalize( coord - ptcls[i].pos );
        float3 intensity = ( k * ptcls[i].charge / pow( dist, 2 ) ) * dir;
        fieldIntensity += intensity;
    }
    
    field[coord] = float4( fieldIntensity, length( fieldIntensity ) );
}
