struct VS_OUT
{
    float3 color : Color;
    float4 pos : SV_POSITION;
};

VS_OUT main(float2 pos : Position, float3 color : Color)
{
    VS_OUT output;
    output.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    output.color = color;
    return output;
}