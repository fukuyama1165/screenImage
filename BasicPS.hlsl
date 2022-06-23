#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);//０番スロットの設定されたテクスチャ
SamplerState smp :register(s0);//０番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{
	return float4(tex.Sample(smp,input.uv)) * color;
}