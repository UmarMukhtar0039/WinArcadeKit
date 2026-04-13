struct VS_OUT
{
    float2 uv : TexCoord;
    float4 color : Color;
    float4 pos : SV_POSITION;
};

cbuffer TransformBuffer
{
    matrix transform;
};

VS_OUT main(float2 pos : Position, float2 uv : TexCoord, float4 color : Color)
{
    VS_OUT output;
    output.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    output.uv = uv;
    output.color = color;
    return output;
}
