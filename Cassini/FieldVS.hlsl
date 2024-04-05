cbuffer VSConstantData
{
    matrix viewMatrix;
    matrix projectionMatrix;
    float charge;
    float mass;
    float3 velocity;
    float dt;
};

SamplerState sState;
Texture3D field;

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float2 tc : TextureCoordinate;
    float4 pos : SV_Position;
};

VSOut main( float3 x1 : Position, float3 n : Normal, float2 tc : TextureCoordinate )
{

    float4 intensity = field.SampleLevel( sState, normalize( x1 ), 0 );
    float3 force = intensity.xyz * charge;
    float3 acceleration = force / mass;
    float3 x2 = x1 + velocity * dt + 0.5f * acceleration * dt * dt;
    float3 dx = x2 - x1;
    
    VSOut vso = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
    
    vso.worldPos = mul( float4( x2, 1.0f ), viewMatrix );
    
    vso.normal = n + dx;
    vso.normal = mul( vso.normal, (float3x3) viewMatrix );
    
    vso.pos = mul( float4( dx, 1.0f ), viewMatrix );
    vso.pos = mul( vso.pos, projectionMatrix );
    
    vso.tc = tc;
    
    return vso;
}