struct ParticleData
{
    float3 s;
    float3 v;
};

struct IntrinsicData
{
    float mass;
    float charge;
    float radius;
};

cbuffer Constants : register(b0)
{
    float3 minBoundary;
    float3 maxBoundary;
    unsigned int size;
    float dt;
};

//StructuredBuffer<IntrinsicData> bondIndices : register(t2);
StructuredBuffer<IntrinsicData> intrinsicData : register(t0);
StructuredBuffer<ParticleData> particle_input : register(t1);
RWStructuredBuffer<ParticleData> particle_output : register(u0);

static float k = 8.9876e+9; // N*m^2 / C^2
static float reg = 1.0f;

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

[numthreads(100, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint index = DTid.x;
    
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
            float3 intensity = ( k * intrinsicData[i].charge / pow( dist, 2 ) ) * dir;
            fieldIntensity += intensity;

            collisionVelocity = particleCollisionCheck( fieldPoint, ptclPos, intrinsicData[index].radius, intrinsicData[i].radius, collisionVelocity );
        }
    }

    float3 electricForce = fieldIntensity * intrinsicData[index].charge;
    float3 acceleration = electricForce / intrinsicData[index].mass;
    float3 velocityFinal = collisionVelocity + acceleration * dt;
    float3 positionFinal = particle_input[index].s + ( velocityFinal + collisionVelocity ) * dt / 2;
    
    velocityFinal = boundaryCollisionCheck(positionFinal, velocityFinal);
    
    particle_output[index].s = positionFinal;
    particle_output[index].v = velocityFinal;
}