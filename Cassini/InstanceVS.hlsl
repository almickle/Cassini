struct VertexInput
{
    float3 pos : POSITION;
    float3 n : NORMAL;
    float2 tc : TextureCoordinate;
};

struct InstanceInput
{
    matrix modelMatrix;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float2 tc : TextureCoordinate;
    uint id : SV_InstanceID;
    float4 pos : SV_Position;
};

struct VSInput
{
    float3 pos : Position;
    float3 n : Normal;
    float2 tc : TextureCoordinate;
};

cbuffer GlobalBuffer : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
};

StructuredBuffer<InstanceInput> instanceBuffer : register(t0);

VSOut main( VSInput vertex, in uint instanceID : SV_InstanceID)
{
    VSOut vso;
    vso.worldPos = mul( float4( vertex.pos, 1.0f ), instanceBuffer[instanceID].modelMatrix );
    vso.worldPos = mul( float4( vso.worldPos, 1.0f ), viewMatrix );
    vso.normal = mul( vertex.n, (float3x3) instanceBuffer[instanceID].modelMatrix );
    vso.normal = mul( vso.normal, (float3x3) viewMatrix );
    vso.pos = mul( float4( vertex.pos, 1.0f ), instanceBuffer[instanceID].modelMatrix );
    vso.pos = mul( vso.pos, viewMatrix );
    vso.pos = mul( vso.pos, projectionMatrix );
    vso.tc = vertex.tc;
    vso.id = instanceID;
    return vso;
}