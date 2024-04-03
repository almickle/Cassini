cbuffer VSConstantData
{
    matrix modelMatrix;
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

VSOut main( float3 pos : Position, float3 n : Normal, float2 tc : TextureCoordinate )
{
    VSOut vso = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };
    vso.worldPos = mul( float4( pos, 1.0f ), modelMatrix );
    vso.worldPos = mul( vso.worldPos, viewMatrix );
    vso.normal = mul( n, modelMatrix );
    vso.normal = mul( vso.normal, viewMatrix );
    vso.pos = mul( float4( pos, 1.0f ), modelMatrix );
    vso.pos = mul( vso.pos, viewMatrix );
    vso.pos = mul( vso.pos, projectionMatrix );
    vso.tc = tc;
    return vso;
}