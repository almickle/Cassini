
float4 main( float3 worldPos : Position, float3 n : Normal, float2 tc : TextureCoordinate ) : SV_Target
{
    float scale = 100.0f;
    float thickness = 0.1f;
    float3 defaultColor = { 0.9f, 0.9f, 0.9f };
    float majorInterval = 10.0f;
    float minorInterval = 1.0f;
    
    float4 major = float4( defaultColor, 0.9f );
    float4 minor = float4( defaultColor, 0.7f );
    float4 clearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    
    float4 x = { 1.0f, 0.1f, 0.1f, 0.9f };
    float4 y = { 0.1f, 1.0f, 0.1f, 0.9f };
    
    float4 value;
    if (( tc.x <= 0.5f + 0.0005 && tc.x >= 0.5f - 0.0005 ))
        value = x;
    else if (( tc.y <= 0.5f + 0.0005 && tc.y >= 0.5f - 0.0005 ))
        value = y;
    else if (( tc.x * scale ) % 10.0f <= thickness || ( tc.y * scale ) % 10.0f <= thickness)
        value = major;
    else if (( tc.x * scale ) % 1.0f <= thickness / 2 || ( tc.y * scale ) % 1.0f <= thickness / 2)
        value = minor;
    else
        value = clearColor;
    
    return value;
}