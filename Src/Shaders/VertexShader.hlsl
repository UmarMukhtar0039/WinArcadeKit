struct VS_OUT
{
    float3 color : Color;
    float4 pos : SV_POSITION;
};

cbuffer TransformBuffer
{
    matrix transform; // 4x4 transformation matrix (float4x4)
};

VS_OUT main(float2 pos : Position, float3 color : Color)
{
    VS_OUT output;
    output.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    output.color = color;
    return output;
}