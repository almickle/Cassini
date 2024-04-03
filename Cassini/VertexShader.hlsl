cbuffer VSConstantData
{
   matrix worldMatrix;
   matrix viewMatrix;
   matrix projectionMatrix;
};

float4 main( float3 pos : POSITION ) : SV_POSITION
{
    float4 transformed = mul( float4( pos, 1.0f ), worldMatrix );
    transformed = mul( transformed, viewMatrix );
    transformed = mul( transformed, projectionMatrix );
    return transformed;
}