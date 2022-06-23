#pragma once
//D3D�R���p�C��
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//���w���C�u����(�œK�����ꂷ���Ďg���ɂ����炵���̂ł��ƂŎ��삵�����)
#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//�L�[�{�[�h��R���g���[���[�Ȃǂ̓��͂���w�b�_�ƃ��C�u�����̃����N
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")

#pragma comment(lib,"dxguid.lib")

//const float PI = 3.141592653589;

#include <DirectXTex.h>

#include "DirectXInit.h"

class DrawingObj
{
public:
	//�������������œK���ɃG���[���͂��Ȃ��Ǝv���l�����Ă����{�I�ɏ��������Ă����Ȃ��͂�(�ݒ肪�j�]���ĂȂ����)
	DrawingObj(const int windowWidth, const int windowHeight);

	//���W���w��ł��邪�����吙
	DrawingObj(const int windowWidth, const int windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1 = { 0.0f,1.0f }, XMFLOAT2 vertexUv2 = { 0.0f,0.0f }, XMFLOAT2 vertexUv3 = { 1.0f,1.0f }, XMFLOAT2 vertexUv4 = { 1.0f,0.0f });

	~DrawingObj();

	//�`�揉���������֐�
	void basicInit(ID3D12Device* dev);//basicPS�ǂݍ���
	void colorChangeInit(ID3D12Device* dev);//colorChangePS�ǂݍ���

	void screenImageInit(ID3D12Device* dev);
	void screenImageTextureBuffGeneraion(ID3D12Device* dev);

	//���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos;//xyz���W
		XMFLOAT2 uv;//uv���W
	};

	//���_�o�b�t�@����
	void vertexBuffGeneration(ID3D12Device* dev);

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void vertexShaderGeneration();//basicVS�ǂݍ���
	void vertexShaderGeneration2();//vertexMoveVS�ǂݍ���

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	void pixelShaderGeneration();//basicPS�ǂݍ���
	void pixelShaderGeneration2();//colorChangePS�ǂݍ���

	//���_���C�A�E�g�̐ݒ�
	void vertexLayout();

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	void graphicPipelineGeneration();

	//�f�X�N���v�^�����W�̐ݒ�
	void descriptorRangeGeneration();

	//���[�g�p�����[�^�̐ݒ�(�萔�o�b�t�@�ƃV�F�[�_�ɂ���)
	void rootParamGeneration();
	
	//�e�N�X�`���T���v���[�̐ݒ�
	void textureSamplerGeneration();

	//���[�g�V�O�l�`��
	void rootsignatureGeneration(ID3D12Device* dev);

	//�萔�o�b�t�@
	void constantBuffGeneration(ID3D12Device* dev);

	//�C���f�b�N�X�f�[�^�֘A(�C���f�b�N�X�r���[������)
	void indicesBuffGeneration(ID3D12Device* devconst);

	//�摜�C���[�W�f�[�^
	void imageDataGeneration();

	//�X�N���[���摜�C���[�W�f�[�^
	void screenImageDataGeneration();

	//�e�N�X�`���o�b�t�@
	void textureBuffGeneraion(ID3D12Device* dev);

	//�V�F�[�_���\�[�X�r���[
	void SRVGeneraion(ID3D12Device* dev);

	//GPU�ɒ��_�f�[�^��]������֐�
	void vertexMap();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="PipeLineRuleFlag">�`����@��ύX���邩�ǂ���(���݂̓��C���[�t���[�����h��Ԃ�)true�͓h��Ԃ�</param>
	/// <param name="ChangeSquareFlag">�O�p�`�ŕ`�悷�邩�l�p�`�ɕ`�悷��(true�͎l�p�`)</param>
	void Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag);


	//�A�t�B���ϊ����̂��̂̊֐�(2D)
	XMFLOAT3 Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//Obj�ɑ΂��ăA�t�B���ϊ���������֐�
	void ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//�萔�o�b�t�@��ύX����֐�
	void constBuffColorUpdata(float Red,float Green,float Blue);
	void constBuffPosMUpdata(float X,float Y,float Z);

	//
	void SRVmake(ID3D12Device* dev, ScratchImage* image);

private:

	DirectXInit* directXInit_ = nullptr;

	//��ʃT�C�Y
	int Win_width;
	int Win_height;

	//���_�f�[�^(���₵�����Ȃ炱�������₵�Ă���)
	Vertex vertices[4];

	//���_�f�[�^�T�C�Y
	UINT sizeVB;

	HRESULT result;

	//���_�o�b�t�@�p�ϐ�
	D3D12_HEAP_PROPERTIES heapprop{};//�q�[�v�ݒ�
	D3D12_RESOURCE_DESC resDesc{};//���\�[�X�ݒ�
	ID3D12Resource* vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* vsBlob = nullptr;

	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr;

	//�G���[�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr;

	//���_���C�A�E�g(�v�f�𑝂₷�Ȃ�z�񐔂𑝂₷)
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];

	//�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p��
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};

	//�f�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//���[�g�p�����[�^(�萔�o�b�t�@�̐�����������z��̗v�f���𑝂₵�Đݒ�����Ă���֐��̒��g�ɂ��ǉ����邱��)
	D3D12_ROOT_PARAMETER rootParam[4] = {};

	//�e�N�X�`���T���v���[
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootsignature;

	//�p�C�v���C���X�e�[�g
	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12PipelineState* pipelinestate2 = nullptr;

	//�萔�o�b�t�@�p�̃��\�[�X�ݒ�֐�
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);

	//�萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;//�F(RGBA)
	};

	struct ConstBufferDataMaterial2
	{
		XMFLOAT4 posM;//�ʒu�ړ��Ɏg��(XYZ);
	};

	//�萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;//3D�ϊ��s��
	};

	//�萔�o�b�t�@���̂���
	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12Resource* constBuffMaterial2 = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;

	//�}�b�s���O����Ƃ��̃|�C���^
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferDataMaterial2* constMapMaterial2 = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;

	//�C���f�b�N�X�f�[�^
	unsigned short indices[6];
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;
	//�C���f�b�N�X�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//�摜�f�[�^��
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	ID3D12Resource* texBuff = nullptr;

	ID3D12DescriptorHeap* srvHeap = nullptr;

	
	
	ScratchImage imagescrreen;

};
