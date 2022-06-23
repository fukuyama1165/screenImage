cbuffer ConstBufferDataMaterial:register(b0)
{
	float4 color;//�F(RGBA)
}

cbuffer ConstBufferDataMaterial2 : register(b1)//�萔�o�b�t�@�̔ԍ���b�̌��ɂ���
{
	float4 posm;//�ʒu(XYZ)
}

cbuffer ConstBufferDataTransform : register(b2)
{
	matrix mat;//3D�ϊ��s��
}

//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos:SV_POSITION;
	//uv�l
	float2 uv:TEXCOORD;
};