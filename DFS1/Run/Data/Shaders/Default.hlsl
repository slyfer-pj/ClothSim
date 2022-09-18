struct vs_input_t
{
	float3 localPosition : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct v2p_t
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer CameraConstants : register(b2)
{
	float1 orthoMinX;
	float1 orthoMinY;
	float1 orthoMinZ;
	float1 orthoMinPadding;
	float1 orthoMaxX;
	float1 orthoMaxY;
	float1 orthoMaxZ;
	float1 orthoMaxPadding;
}

Texture2D diffuseTexture : register(t0);
SamplerState diffuseSampler : register(s0);

float RangeMap(float1 inputValue, float1 inputStart, float1 inputEnd, float1 outputStart, float1 outputEnd)
{
	float1 outputValue = outputStart + (((inputValue - inputStart) / (inputEnd - inputStart)) * (outputEnd - outputStart));
	return outputValue;
}

v2p_t VertexMain(vs_input_t input)
{
	float1 ndcPosX = RangeMap(input.localPosition.x, orthoMinX, orthoMaxX, -1, 1);
	float1 ndcPosY = RangeMap(input.localPosition.y, orthoMinY, orthoMaxY, -1, 1);
	float1 ndcPosZ = RangeMap(input.localPosition.z, orthoMinZ, orthoMaxZ, 0, 1);
	float4 ndcPos = float4(ndcPosX, ndcPosY, ndcPosZ, 1);
	v2p_t v2p;
	v2p.position = ndcPos;
	v2p.color = input.color;
	v2p.uv = input.uv;
	return v2p;
}

float4 PixelMain(v2p_t input) : SV_Target0
{
	//return float4(input.color);
	return float4(diffuseTexture.Sample(diffuseSampler, input.uv) * input.color);
}
