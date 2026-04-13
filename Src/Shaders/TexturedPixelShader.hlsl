Texture2D spriteTexture : register(t0);
SamplerState spriteSampler : register(s0);

float4 main(float2 uv : TexCoord, float4 color : Color) : SV_Target
{
    return spriteTexture.Sample(spriteSampler, uv) * color;
}
