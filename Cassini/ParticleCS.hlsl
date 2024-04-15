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

static float atomicRadius = 50.0f; // pm
static float bondLength = 100.0f; // pm

static float kq2 = 138954.2435; // amu*pm3/ps2
static float sK = 20000.0f;

float3 boundaryCollisionCheck( float3 positionFinal, float3 velocityFinal )
{
    float3 reflected = velocityFinal;
    if (positionFinal.x < minBoundary.x)
    {
        reflected = reflect( reflected, float3( 1.0f, 0.0f, 0.0f ) );
    }
    if (positionFinal.x > maxBoundary.x)
    {
        reflected = reflect( reflected, float3( -1.0f, 0.0f, 0.0f ) );
    }
    if (positionFinal.y < minBoundary.y)
    {
        reflected = reflect( reflected, float3( 0.0f, 1.0f, 0.0f ) );
    }
    if (positionFinal.y > maxBoundary.y)
    {
        reflected = reflect( reflected, float3( 0.0f, -1.0f, 0.0f ) );
    }
    if (positionFinal.z < minBoundary.z)
    {
        reflected = reflect( reflected, float3( 0.0f, 0.0f, 1.0f ) );
    }
    if (positionFinal.z > maxBoundary.z)
    {
        reflected = reflect( reflected, float3( 0.0f, 0.0f, -1.0f ) );
    }
    
    return reflected;
}

float3 particleCollisionCheck( float3 positionA, float3 positionB, float radiusA, float radiusB, float3 velocityInitial )
{
    if (distance( positionA, positionB ) <= radiusA + radiusB)
    {
        float3 collisionNormal = normalize( positionA - positionB );
        return reflect( velocityInitial, collisionNormal );
    }
    else
    {
        return velocityInitial;
    }
}

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint index = DTid.x;
    
    float3 netForce = { 0.0f, 0.0f, 0.0f };

    float3 fieldPoint = particle_input[index].s;
    float3 fieldIntensity = { 0.0f, 0.0f, 0.0f };
    float3 collisionVelocity = particle_input[index].v;
    for (unsigned int i = 0; i < size; i++)
    {
        if (i != index)
        {
            float3 ptclPos = particle_input[i].s;
            float dist = max( intrinsicData[index].radius + intrinsicData[i].radius, distance( fieldPoint, ptclPos ) );
            float3 dir = normalize( fieldPoint - ptclPos );
            float3 intensity = ( kq2 * intrinsicData[i].charge / pow( dist, 2 ) ) * dir;
            fieldIntensity += intensity;

            //collisionVelocity = particleCollisionCheck( fieldPoint, ptclPos, intrinsicData[index].radius, intrinsicData[i].radius, collisionVelocity );
        }
    }
    float3 bondForce = { 0.0f, 0.0f, 0.0f };
    ParticleData targetPtcl = particle_input[index];
    for (unsigned int j = 0; j < boundCount; j++)
    {
        if (bondingPairs[j].ptcla == index)
        {
            ParticleData bondedPtcl = particle_input[bondingPairs[j].ptclb];
            uint bondedIndex = bondingPairs[j].ptclb;
            float dist = max( intrinsicData[index].radius + intrinsicData[bondedIndex].radius, distance( targetPtcl.s, bondedPtcl.s ) );
            float3 dir = normalize( targetPtcl.s - bondedPtcl.s );
            float dx = ( dist - bondingPairs[j].equillibriumLength ) / 2.0f;
            bondForce += dir * (-sK * dx);
        }
    }
    float3 electricForce = fieldIntensity * intrinsicData[index].charge;
    netForce += electricForce;
    netForce += bondForce;
    float3 acceleration = netForce / ( intrinsicData[index].mass );
    float3 velocityFinal = collisionVelocity + acceleration * dt;
    float3 positionFinal = particle_input[index].s + ( velocityFinal + collisionVelocity ) * dt / 2.0f;
    
    //velocityFinal = boundaryCollisionCheck(positionFinal, velocityFinal);
    
    particle_output[index].s = positionFinal;
    particle_output[index].v = velocityFinal;
}