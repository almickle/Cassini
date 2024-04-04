cbuffer PointLight
{
    float3 position;
    float3 color;
    float3 ambient;
    float3 modelColor;
};

float4 main( float3 worldPos : Position, float3 n : Normal, float2 tc : TextureCoordinate ) : SV_Target
{
    float intensity = 1.0f;
    
    float3 normal = normalize( n );
    float lightDir = normalize( position - worldPos );

    float diffuseStrength = max( 0.0f, dot( lightDir, normal ) );
    float3 diffuse = diffuseStrength * color;
    
    float3 viewDir = normalize( float3( 0.0f, 0.0f, -1.0f ) );
    float3 reflectDir = normalize( reflect( -lightDir, normal ) );
    
    float specularStrength = pow( max( 0.0f, dot( viewDir, reflectDir ) ), 32.0f );
    float3 specular = specularStrength * color;
    
    //float3 lighting = ambient * 0.5f + diffuse * intensity + specular * intensity;
    
    //float3 lighting = ambient * 0.5f + specular * intensity;
    float3 lighting = diffuse * intensity;
    
    return float4( modelColor * lighting, 1.0f );
}