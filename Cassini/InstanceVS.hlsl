cbuffer VSConstantData
{
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VSOut
{
    float3 worldPos : Position;
    float3 normal : Normal;
    float2 tc : TextureCoordinate;
    float4 pos : SV_Position;
};

struct VertexInput
{
    float3 pos : POSITION;
    float3 n : NORMAL;
    float2 tc : TextureCoordinate;
};

struct InstanceInput
{
    matrix modelMatrix : INSTANCE;
};

VSOut main( VertexInput vertex, InstanceInput instance )
{
    VSOut vso = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
    vso.worldPos = mul( float4( vertex.pos, 1.0f ), instance.modelMatrix );
    vso.worldPos = mul( float4( vso.worldPos, 1.0f ), viewMatrix );
    vso.normal = mul( vertex.n, (float3x3) instance.modelMatrix );
    vso.normal = mul( vso.normal, (float3x3) viewMatrix );
    vso.pos = mul( float4( vertex.pos, 1.0f ), instance.modelMatrix );
    vso.pos = mul( vso.pos, viewMatrix );
    vso.pos = mul( vso.pos, projectionMatrix );
    vso.tc = vertex.tc;
    return vso;
}