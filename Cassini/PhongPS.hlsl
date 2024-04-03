cbuffer PointLight
{
    float3 position;
    float3 color;
    float3 ambient;
    float intensity;
    float att;
    float3 modelColor;
};


float4 main( float3 worldPos : Position, float3 n : Normal, float2 tc : TextureCoordinate ) : SV_Target
{   
    float3 normal = normalize( n );
    float diffuseStrength = max( 0.0f, dot( position, normal ) );
    float3 diffuse = diffuseStrength * color;
    
    float3 viewDir = normalize( float3( 0.0f, 0.0f, -1.0f ) );
    float3 reflectDir = normalize( reflect( -position, normal ) );
    float specularStrength = max( 0.0f, dot( viewDir, reflectDir ) );
          specularStrength = pow( specularStrength, 32.0 );
    float3 specular = specularStrength * color;

    float3 lighting = ambient * 0.5f + diffuse + specular;
    
    return float4( modelColor * lighting * intensity, 1.0f );
}