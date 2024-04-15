struct ParticleData
{
    float3 s; // pm
    float3 v; // pm / ps
};
struct IntrinsicData
{
    float mass;
    float charge; // e
    float radius; // pm
};
struct BondData
{
    float bondEnergy;
    float equillibriumLength; // pm
    uint ptcla;
    uint ptclb;
};

cbuffer StaticBuffer : register(b0)
{
    float3 minBoundary;
    float3 maxBoundary;
    unsigned int size;
    unsigned int boundCount;
};

cbuffer DynamicBuffer : register(b1)
{
    float dt; // picoseconds
};

StructuredBuffer<IntrinsicData> intrinsicData : register(t0);
StructuredBuffer<ParticleData> particle_input : register(t1);
StructuredBuffer<BondData> bondingPairs : register(t2);

RWStructuredBuffer<ParticleData> particle_output : register(u0);

static float kq2 = 138954.2435; // amu*pm3/ps2
static float sK = 1.0f;

float3 computeElectricForce( uint index, float3 position )
{
    float3 fieldIntensity = { 0.0f, 0.0f, 0.0f };
    for (unsigned int i = 0; i < size; i++)
    {
        if (i != index)
        {
            float dist = max( intrinsicData[index].radius + intrinsicData[i].radius, distance( position, particle_input[i].s ) );
            float3 dir = normalize( position - particle_input[i].s );
            float3 intensity = ( kq2 * intrinsicData[i].charge / pow( dist, 2 ) ) * dir;
            fieldIntensity += intensity;
        }
    }
    return float3( fieldIntensity * intrinsicData[index].charge );
}
float3 computeBondForce( uint index, float3 position )
{
    float3 bondForce = { 0.0f, 0.0f, 0.0f };
    for (unsigned int j = 0; j < boundCount; j++)
    {
        if (bondingPairs[j].ptcla == index)
        {
            ParticleData bondedPtcl = particle_input[bondingPairs[j].ptclb];
            uint bondedIndex = bondingPairs[j].ptclb;
            float dist = max( intrinsicData[index].radius + intrinsicData[bondedIndex].radius, distance( position, bondedPtcl.s ) );
            float3 dir = normalize( position - bondedPtcl.s );
            float dx = ( dist - bondingPairs[j].equillibriumLength ) / 2.0f;
            bondForce += dir * ( -sK * dx );
        }
    }
    return bondForce;
}

float3 computeAcceleration(uint index, float3 position)
{
    float3 netForce = { 0.0f, 0.0f, 0.0f };
    float3 electricForce = computeElectricForce( index, position );
    float3 bondForce = computeBondForce( index, position );
    netForce += electricForce;
    netForce += bondForce;
    float3 acceleration = netForce / ( intrinsicData[index].mass );
    return acceleration;
}

[numthreads( 1024, 1, 1 )]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint index = DTid.x;
    
    // Initial state
    float3 initialPosition = particle_input[index].s;
    float3 initialVelocity = particle_input[index].v;
    
    float3 k1 = initialVelocity;
    float3 k2 = initialVelocity + computeAcceleration( index, initialPosition + k1 * dt / 2.0f ) * dt / 2.0f;
    float3 k3 = initialVelocity + computeAcceleration( index, initialPosition + k2 * dt / 2.0f ) * dt / 2.0f;
    float3 k4 = initialVelocity + computeAcceleration( index, initialPosition + k3 * dt ) * dt;
    
    float3 velocityFinal = ( k1 + k2 * 2.0f + k3 * 2.0f + k4 ) / 6.0f;
    float3 positionFinal = initialPosition + velocityFinal * dt;
    
    particle_output[index].s = positionFinal;
    particle_output[index].v = velocityFinal;
}