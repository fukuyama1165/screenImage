#include "DrawingObj.h"

const float PI = 3.141592653589;

//D3D12_DESCRIPTOR_HEAP_DESC DrawingObj::srvHeapDesc = {};



DrawingObj::DrawingObj(const int windowWidth,const int windowHeight)
{
	Win_width = windowWidth;
	Win_height = windowHeight;

	//���_�f�[�^(�l�_���̍��W)
					//  x     y    z      u    v
	vertices[0] = { {-0.4f,-0.7f,0.0f},{0.0f,1.0f} };//����
	vertices[1] = { {-0.4f,+0.7f,0.0f},{0.0f,0.0f} };//����
	vertices[2] = { {+0.4f,-0.7f,0.0f},{1.0f,1.0f} };//�E��
	vertices[3] = { {+0.4f,+0.7f,0.0f},{1.0f,0.0f} };//�E��

	//�C���f�b�N�X�f�[�^
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;
	
	
	
	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	//directXInit_=DirectXInit::GetInstance();
}

DrawingObj::DrawingObj(const int windowWidth, const int windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1, XMFLOAT2 vertexUv2, XMFLOAT2 vertexUv3, XMFLOAT2 vertexUv4)
{
	Win_width = windowWidth;
	Win_height = windowHeight;

	//���_�f�[�^(�l�_���̍��W)
					//  x     y    z      u    v
	vertices[0] = { vertexPos1,vertexUv1 };//����
	vertices[1] = { vertexPos2,vertexUv2 };//����
	vertices[2] = { vertexPos3,vertexUv3 };//�E��
	vertices[3] = { vertexPos4,vertexUv4 };//�E��

	//�C���f�b�N�X�f�[�^
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;



	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));
}


DrawingObj::~DrawingObj()
{
}

void DrawingObj::basicInit(ID3D12Device* dev)
{
	

	vertexBuffGeneration(dev);

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	indicesBuffGeneration(dev);

	imageDataGeneration();

	textureBuffGeneraion(dev);

	SRVGeneraion(dev);

}

void DrawingObj::colorChangeInit(ID3D12Device* dev)
{
	vertexBuffGeneration(dev);

	vertexShaderGeneration2();

	pixelShaderGeneration2();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	indicesBuffGeneration(dev);

	imageDataGeneration();

	textureBuffGeneraion(dev);

	SRVGeneraion(dev);

}

void DrawingObj::screenImageInit(ID3D12Device* dev)
{
	vertexBuffGeneration(dev);

	vertexShaderGeneration();

	pixelShaderGeneration();

	vertexLayout();

	graphicPipelineGeneration();

	descriptorRangeGeneration();

	rootParamGeneration();

	textureSamplerGeneration();

	rootsignatureGeneration(dev);

	constantBuffGeneration(dev);

	indicesBuffGeneration(dev);

	//screenImageDataGeneration();

	textureBuffGeneraion(dev);

	SRVGeneraion(dev);

}

void DrawingObj::screenImageTextureBuffGeneraion(ID3D12Device* dev)
{
	/*screenImageDataGeneration();

	textureBuffGeneraion(dev);*/
}

void DrawingObj::vertexBuffGeneration(ID3D12Device* dev)
{
#pragma region ���_�o�b�t�@�̊m��

	//���_�o�b�t�@�̊m�ە�
	
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

	
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;//���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

#pragma endregion

#pragma region ���_�o�b�t�@�̐���

	//���_�o�b�t�@�̐���
	
	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

#pragma endregion

#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);

	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//���W���R�s�[
	}


	//�Ȃ��������
	vertBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region ���_�o�b�t�@�r���[�̐���

	//���_�o�b�t�@�r���[�̍쐬(GPU�ւ̓������)��

	//���_�o�b�t�@�r���[�̍쐬
	
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

}

void DrawingObj::vertexShaderGeneration()
{
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"vs_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region ���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::vertexShaderGeneration2()
{
#pragma region ���_�V�F�[�_�t�@�C���̓ǂݍ��݂ƃR���p�C��

	//���_�V�F�[�_�t�@�C���̓ǂݍ��ݕ�
	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"vertexMoveVS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"vs_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&vsBlob,
		&errorBlob
	);

#pragma endregion 

#pragma region ���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//���_�V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::pixelShaderGeneration()
{
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"ps_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::pixelShaderGeneration2()
{
#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"colorChangePS.hlsl",//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main",//�G���g���[�|�C���g��
		"ps_5_0",//�V�F�[�_���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�N�p�ݒ�
		0,
		&psBlob,
		&errorBlob
	);

#pragma endregion

#pragma region �s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ

	//�s�N�Z���V�F�[�_�̓ǂݍ��ݎ��̃G���[��\������ꏊ
	if (FAILED(result))
	{
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

#pragma endregion
}

void DrawingObj::vertexLayout()
{
#pragma region ���_���C�A�E�g�̐ݒ�

	//���_���C�A�E�g
	//�O���t�B�b�N�p�C�v���C���Œ��_����̃f�[�^�ɉ����������邩���߂�
	//����͍Œ����3D���W�����������Ă���炵��
	inputLayout[0] =
	{
		"POSITION",//�Z�}���e�B�b�N��
		0,//�����Z�}���e�B�b�N������������Ƃ��̃C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,//�v�f���ƃr�b�g����Ԃ�(XYZ�̂R��float�^�Ȃ̂�R32G32B32_FLOAT)
		0,//���̓X���b�g�C���f�b�N�X
		D3D12_APPEND_ALIGNED_ELEMENT,//�f�[�^�̃I�t�Z�b�g�l(D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�)
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//���̓f�[�^����(�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA)
		0,//��x�ɕ`�悷��C���X�^���X��
	};
		//���W�ȊO�Ɂ@�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͂���ɑ�����
	inputLayout[1] =
	{//UV���W
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

#pragma endregion
}

void DrawingObj::graphicPipelineGeneration()
{
#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�

	//�O���t�B�b�N�X�p�C�v���C����

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region �u�����h�X�e�[�g�̋��ʐݒ�(�A���t�@�l)

	blenddesc.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion


#pragma region ���Z����

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region ���Z����

	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��

#pragma endregion �R�����g�A�E�g��

#pragma region �F���]

	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�

#pragma endregion �R�����g�A�E�g��

#pragma region ����������

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion



	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`��|���S������h��Ԃ�(�O�p�`)

#pragma region �O���t�B�b�N�X�p�C�v���C���Q�̐ݒ�

//�O���t�B�b�N�X�p�C�v���C����
	

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline2.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline2.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline2.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline2.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
	gpipeline2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	gpipeline2.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//���C���[�t���[���`��
	gpipeline2.RasterizerState.DepthClipEnable = true;//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g�̐ݒ�
	//gpipeline2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = gpipeline2.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

#pragma region ���ʐݒ�(�A���t�@�l)

	blenddesc2.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��

#pragma endregion

#pragma region ����������

	blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l

#pragma endregion

	//���_���C�A�E�g�̐ݒ�
	gpipeline2.InputLayout.pInputElementDescs = inputLayout;
	gpipeline2.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline2.NumRenderTargets = 1;//�`��Ώۂ͂P��
	gpipeline2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline2.SampleDesc.Count = 1;//�P�s�N�Z���ɂ��P��T���v�����O

#pragma endregion �ʏ�`�惏�C���[�t���[���`��(�O�p�`)

}

void DrawingObj::descriptorRangeGeneration()
{
#pragma region �f�X�N���v�^�����W�̐ݒ�
	descriptorRange.NumDescriptors = 1;//��x�̕`��Ɏg���e�N�X�`�����P���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion
}

void DrawingObj::rootParamGeneration()
{
#pragma region ���[�g�p�����[�^

	//���[�g�p�����[�^�̐ݒ�
	
	//�F
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
	rootParam[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�ʒu
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[1].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�
	rootParam[1].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�摜�f�[�^�p
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//���
	rootParam[2].DescriptorTable.pDescriptorRanges = &descriptorRange;//�f�X�N���v�^�����W
	rootParam[2].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����

	rootParam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParam[3].Descriptor.ShaderRegister = 2;//�萔�o�b�t�@�ԍ�
	rootParam[3].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

#pragma endregion �萔�o�b�t�@�𑝂₵���瑝�₷�Ƃ��낪����
}

void DrawingObj::textureSamplerGeneration()
{
#pragma region �e�N�X�`���T���v���[�̐ݒ�

	//�e�N�X�`���T���v���[�̐ݒ�
	sampleDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�(�^�C�����O)
	sampleDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�(�^�C�����O)
	sampleDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�(�^�C�����O)
	sampleDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	sampleDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ă��j�A���
	sampleDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	sampleDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	sampleDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampleDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂ݎg�p�\

#pragma endregion
}

void DrawingObj::rootsignatureGeneration(ID3D12Device* dev)
{
#pragma region ���[�g�V�O�l�`���ݒ�

	//���[�g�V�O�l�`���̐���
	

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParam);//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &sampleDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	rootSigBlob->Release();

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature;

	//�p�C�v���C��2�ɂ����[�g�V�O�l�`�����Z�b�g
	gpipeline2.pRootSignature = rootsignature;

#pragma endregion �萔�o�b�t�@�𑝂₵���烋�[�g�p�����[�^�����������p�����[�^��������������

#pragma region �p�C�v���C���X�e�[�g�̐���

	//�p�C�v���C���X�e�[�g�̐���
	
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	//�p�C�v���C���X�e�[�g�Q�̐���
	
	result = dev->CreateGraphicsPipelineState(&gpipeline2, IID_PPV_ARGS(&pipelinestate2));

#pragma endregion

}

void DrawingObj::constantBuffGeneration(ID3D12Device* dev)
{
#pragma region �萔�o�b�t�@

	//�萔�o�b�t�@�̐����p�̐ݒ�
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p


	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial));
	
	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataMaterial2));
	
	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial2)
	);
	assert(SUCCEEDED(result));

	cbResourceDesc = constBuffResourceGeneration(sizeof(ConstBufferDataTransform));

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));


	//�萔�o�b�t�@�̃}�b�s���O
	
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//�}�b�s���O
	assert(SUCCEEDED(result));

	//�}�b�s���O�������GPU��VRSM��CPU�ƘA������
	//Unmap������ƂȂ��肪��������邪�萔�o�b�t�@�͏��������邱�Ƃ������̂�
	//���Ȃ��Ă����v

	//�l���������ނƎ����I�ɓ]�������



	constMapMaterial->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	
	result = constBuffMaterial2->Map(0, nullptr, (void**)&constMapMaterial2);//�}�b�s���O
	assert(SUCCEEDED(result));

	constMapMaterial2->posM = XMFLOAT4(0,0,0,1);

	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//�}�b�s���O
	assert(SUCCEEDED(result));

	constMapTransform->mat = XMMatrixIdentity();

	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / Win_width;
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / Win_height;
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;

#pragma endregion
}

D3D12_RESOURCE_DESC DrawingObj::constBuffResourceGeneration(int size)
{
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (size + 0xff) & ~0xff;//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	return cbResourceDesc;
}

void DrawingObj::indicesBuffGeneration(ID3D12Device* dev)
{
#pragma region �C���f�b�N�X�o�b�t�@

	//���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	result = dev->CreateCommittedResource(
		&heapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//�C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];//�C���f�b�N�X���R�s�[
	}

	//�}�b�s���O����
	indexBuff->Unmap(0, nullptr);

#pragma endregion ���_��ʂ鏇�Ԃ����߂�ꏊ

#pragma region �C���f�b�N�X�o�b�t�@�r���[

	//�C���f�b�N�X�o�b�t�@�r���[�̐���
	
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

}

void DrawingObj::imageDataGeneration()
{
#pragma region �摜�C���[�W�f�[�^�̍쐬

	//���삵���C���[�W�f�[�^
	////�������s�N�Z����
	//const size_t textureWidth = 256;
	////�c�����s�N�Z����
	//const size_t textureHeight = 256;
	////�z��̗v�f��
	//const size_t imageDataCount = textureWidth * textureHeight;
	////�摜�C���[�W�f�[�^�z��
	//XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];//�K����ŉ������

	//for (size_t i = 0; i < imageDataCount; i++)
	//{
	//	imageData[i].x = 0.0f;//R
	//	imageData[i].y = 1.0f;//G
	//	imageData[i].z = 0.0f;//B
	//	imageData[i].w = 1.0f;//A

	//}

	//�摜�ǂݍ��݂��ĉ摜�C���[�W�f�[�^�𐶐�

	/*directXInit_ = DirectXInit::GetInstance();

	directXInit_->Direct3D9TakeScreenshots({ 0,0,Win_width,Win_height });
	
	result = LoadFromWICMemory(directXInit_->GetShots(), Win_width*Win_height, WIC_FLAGS_NONE, &metadata, scratchImg);*/




	//WIC�e�N�X�`���̃��[�h
	/*result = LoadFromWICFile(
		L"Resources/basketballman2.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);*/

	directXInit_ = DirectXInit::GetInstance();

	result = CaptureTexture(directXInit_->GetCmdQueue(), directXInit_->GetBackBuffers()[directXInit_->GetSwapchain()->GetCurrentBackBufferIndex()], false, scratchImg);


	//result = SaveToWICFile(*scratchImg.GetImage(0,0,0), WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_JPEG), L"NEW_IMAGE.PNG");

	ScratchImage mipChain{};
	//�~�b�v�}�b�v�̐���
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

#pragma endregion
}

void DrawingObj::screenImageDataGeneration()
{
	////�摜�ǂݍ��݂��ĉ摜�C���[�W�f�[�^�𐶐�

	//directXInit_ = DirectXInit::GetInstance();

	//directXInit_->Direct3D9TakeScreenshots({ 0,0,Win_width,Win_height });

	//result = LoadFromWICMemory(directXInit_->GetShots(), Win_width * Win_height, WIC_FLAGS_NONE, &metadata, scratchImg);

	////SaveToWICMemory(directXInit_->GetShots())

	//ScratchImage mipChain{};
	////�~�b�v�}�b�v�̐���
	//result = GenerateMipMaps(
	//	scratchImg.GetImages(),
	//	scratchImg.GetImageCount(),
	//	scratchImg.GetMetadata(),
	//	TEX_FILTER_DEFAULT,
	//	0,
	//	mipChain
	//);

	//if (SUCCEEDED(result))
	//{
	//	scratchImg = std::move(mipChain);
	//	metadata = scratchImg.GetMetadata();
	//}

	////�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	//metadata.format = MakeSRGB(metadata.format);
}


void DrawingObj::textureBuffGeneraion(ID3D12Device* dev)
{
#pragma region �e�N�X�`���o�b�t�@�ݒ�

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//��
	textureResourceDesc.Height = (UINT)metadata.height;//����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

#pragma endregion

#pragma region �e�N�X�`���o�b�t�@�̐���

	//�e�N�X�`���o�b�t�@�̐���
	
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	//�~�b�v�}�b�v�Œu��������
	//�S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		//�~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,//�S�̈�փR�s�[
			img->pixels,//���f�[�^�A�h���X
			(UINT)img->rowPitch,//1���C���A�h���X
			(UINT)img->slicePitch//1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	//result = texBuff->WriteToSubresource(
	//	0,
	//	nullptr,//�S�̈�փR�s�[
	//	imageData,//���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * textureWidth,//1���C���T�C�Y
	//	sizeof(XMFLOAT4) * imageDataCount//�S�T�C�Y
	//);

	//delete[] imageData;

#pragma endregion �����ŉ摜�C���[�W�f�[�^��delete���Ă���(�R�����g�A�E�g��)
}

void DrawingObj::SRVGeneraion(ID3D12Device* dev)
{
#pragma region �V�F�[�_�[���\�[�X�r���[�ׂ̈̃f�X�N���v�^�q�[�v����

	

	//�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	
	//SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	//�f�X�N���v�^�q�[�v�̐ݒ�
	 D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc={};

	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));
	

	//�f�X�N���v�^�n���h��(�q�[�v���̑��삷��ꏊ�w��Ɏg��)
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region �V�F�[�_���\�[�X�r���[

	//�V�F�[�_���\�[�X�r���[
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

#pragma endregion
}

void DrawingObj::vertexMap()
{

	vertBuff->Map(0, nullptr, (void**)&vertMap);
	//�S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];//���W���R�s�[
	}

	//�Ȃ��������
	vertBuff->Unmap(0, nullptr);
}

void DrawingObj::Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag)
{

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�
	//������p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`�����Z�b�g����
	//���߂����[���ŕ`������肢����Ƃ���
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;

	if (PipeLineRuleFlag)
	{
		cmdList->SetPipelineState(pipelinestate);
	}
	else
	{
		cmdList->SetPipelineState(pipelinestate2);
	}

	cmdList->SetGraphicsRootSignature(rootsignature);

	//���_�o�b�t�@�r���[�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);



	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffMaterial2->GetGPUVirtualAddress());


	//SRV�q�[�v�̐ݒ�R�}���h
	cmdList->SetDescriptorHeaps(1, &srvHeap);
	//SRV�q�[�v�̐擪�n���h�����擾(SRV���w���Ă���͂�)
	srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^�Q�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(2, srvGpuHandle);

	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(��ԍŏ��̈�����"���[�g�p�����[�^"�̗v�f�ԍ��ł���)
	cmdList->SetGraphicsRootConstantBufferView(3, constBuffTransform->GetGPUVirtualAddress());

	cmdList->IASetIndexBuffer(&ibView);

	//�`��R�}���h
	if (ChangeSquareFlag)
	{
		//�l�p�`�ɕ`��
		cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	}
	else
	{
		cmdList->DrawInstanced(3, 1, 0, 0);
	}
}

XMFLOAT3 DrawingObj::Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
	float ansBuff[3] = {};
	float ansBuff2[3] = {};
	float ansBuff3[3] = {};
	float ansBuff4[3] = {};
	float ans[3] = {};

	float moveA[3][3] = {
		{1.0f,0.0f, moveX},
		{0.0f,1.0f, moveY},
		{0.0f,0.0f, 1.0f}
	};
	float rotateA[3][3] = {
		{cos(rotate * (PI / 180)),-sin(rotate * (PI / 180)), 0.0f},
		{sin(rotate * (PI / 180)),cos(rotate * (PI / 180)), 0.0f},
		{0.0f,0.0f, 1.0f}
	};

	float scaleA[3][3] = {
		{scaleX,0.0f, 0.0f},
		{0.0f,scaleY, 0.0f},
		{0.0f,0.0f, 1.0f}
	};

	ans[0] = box.x;
	ans[1] = box.y;
	ans[2] = 1.0f;

	ansBuff[0] = moveA[0][0] * vertices[0].pos.x + moveA[0][1] * vertices[0].pos.y + -ans[0] * 1.0f;
	ansBuff[1] = moveA[1][0] * vertices[0].pos.x + moveA[1][1] * vertices[0].pos.y + -ans[1] * 1.0f;
	ansBuff[2] = moveA[2][0] * vertices[0].pos.x + moveA[2][1] * vertices[0].pos.y + ans[2] * 1.0f;


	ansBuff2[0] = rotateA[0][0] * ansBuff[0] + rotateA[0][1] * ansBuff[1] + rotateA[0][2] * ansBuff[2];
	ansBuff2[1] = rotateA[1][0] * ansBuff[0] + rotateA[1][1] * ansBuff[1] + rotateA[1][2] * ansBuff[2];
	ansBuff2[2] = rotateA[2][0] * ansBuff[0] + rotateA[2][1] * ansBuff[1] + rotateA[2][2] * ansBuff[2];

	ansBuff3[0] = scaleA[0][0] * ansBuff2[0] + scaleA[0][1] * ansBuff2[1] + scaleA[0][2] * ansBuff2[2];
	ansBuff3[1] = scaleA[1][0] * ansBuff2[0] + scaleA[1][1] * ansBuff2[1] + scaleA[1][2] * ansBuff2[2];
	ansBuff3[2] = scaleA[2][0] * ansBuff2[0] + scaleA[2][1] * ansBuff2[1] + scaleA[2][2] * ansBuff2[2];

	ansBuff4[0] = moveA[0][0] * ansBuff3[0] + moveA[0][1] * ansBuff3[1] + ans[0] * ansBuff3[2];
	ansBuff4[1] = moveA[1][0] * ansBuff3[0] + moveA[1][1] * ansBuff3[1] + ans[1] * ansBuff3[2];
	ansBuff4[2] = moveA[2][0] * ansBuff3[0] + moveA[2][1] * ansBuff3[1] + ans[2] * ansBuff3[2];

	box.x = moveA[0][0] * ansBuff4[0] + moveA[0][1] * ansBuff4[1] + moveA[0][2] * ansBuff4[2];
	box.y = moveA[1][0] * ansBuff4[0] + moveA[1][1] * ansBuff4[1] + moveA[1][2] * ansBuff4[2];

	return box;

}

//Obj�ɑ΂��ăA�t�B���ϊ���������֐�
void DrawingObj::ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY)
{
#pragma region �A�t�B���ϊ�

	//���_�͍Ō�ɕϊ�����(�����W���ς���Ă��܂�����)
	vertices[1].pos = Afin(vertices[1].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[2].pos = Afin(vertices[2].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[3].pos = Afin(vertices[3].pos, moveX, moveY, rotate, scaleX, scaleY);
	vertices[0].pos = Afin(vertices[0].pos, moveX, moveY, rotate, scaleX, scaleY);

#pragma endregion

#pragma region �A�t�B���ϊ���̒��_�f�[�^���V�F�[�_�ɓ]��

	vertexMap();

#pragma endregion
}

//�萔�o�b�t�@��ύX����֐�
void DrawingObj::constBuffColorUpdata(float Red, float Green, float Blue)
{
	constMapMaterial->color = XMFLOAT4(Red, Green, Blue, 1.0f);
}

void DrawingObj::constBuffPosMUpdata(float X, float Y, float Z)
{
	constMapMaterial2->posM = XMFLOAT4(X, Y, Z, 0.0f);
}

void DrawingObj::SRVmake(ID3D12Device* dev, ScratchImage* image)
{
	/*result = LoadFromWICFile(
		L"Resources/basketballman2.png",
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);*/


	ScratchImage mipChain{};
	//�~�b�v�}�b�v�̐���
	result = GenerateMipMaps(
		image->GetImages(),
		image->GetImageCount(),
		image->GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	if (SUCCEEDED(result))
	{
		*image = std::move(mipChain);
		metadata = image->GetMetadata();

		textureBuffGeneraion(dev);
	}

	

	//�f�X�N���v�^�n���h��(�q�[�v���̑��삷��ꏊ�w��Ɏg��)
	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region �V�F�[�_���\�[�X�r���[

	//�V�F�[�_���\�[�X�r���[
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = resDesc.Format;//RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//�n���h���̎w���ʒu�ɃV�F�[�_���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

#pragma endregion
	
}