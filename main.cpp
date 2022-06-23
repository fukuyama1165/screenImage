#include<Windows.h>

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

//#pragma comment�Ƃ́A�I�u�W�F�N�g�t�@�C���ɁA
//�����J�Ń����N���郉�C�u�����̖��O���L�q�������
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//�L�[�{�[�h��R���g���[���[�Ȃǂ̓��͂���w�b�_�ƃ��C�u�����̃����N
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

const float PI = 3.141592653589;

#include <DirectXTex.h>

#include "DrawingObj.h"

#include "DirectXInit.h"

#pragma region �E�B���h�E�v���V�[�W��

static HDC hdc;
static int width, height;
static HDC hMemDC;

LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	/*
	PAINTSTRUCT ps;
	static BITMAPINFO bmpInfo;
	static LPDWORD lpPixel;
	static HBITMAP hBitmap;
	

	HWND desktop;
	RECT rc;*/
	
	switch (msg)
	{
	//case WM_CREATE:
	//	//�X�N���[���̏��𓾂�
	//	desktop = GetDesktopWindow();
	//	GetWindowRect(desktop, &rc);
	//	width = 1920;
	//	height = 1020;

	//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	//	//DIB�̏���ݒ肷��
	//	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//	bmpInfo.bmiHeader.biWidth = width;
	//	bmpInfo.bmiHeader.biHeight = height;
	//	bmpInfo.bmiHeader.biPlanes = 1;
	//	bmpInfo.bmiHeader.biBitCount = 32;
	//	bmpInfo.bmiHeader.biCompression = BI_RGB;



	//	//DIBSection�쐬
	//	hdc = GetDC(hwnd);
	//	hBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
	//	hMemDC = CreateCompatibleDC(hdc);
	//	SelectObject(hMemDC, hBitmap);
	//	ReleaseDC(hwnd, hdc);

	//	//�X�N���[����DIBSection�ɃR�s�[
	//	hdc = GetDC(desktop);
	//	BitBlt(hMemDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	//	ReleaseDC(desktop, hdc);
	//	return 0;
	//case WM_PAINT:
	//	hdc = BeginPaint(hwnd, &ps);
	//	//�\��ʂ֓]��


	//	//BitBlt(hdc, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
	//	StretchBlt(hdc, 0.01, 0.01, width * 0.8, height * 0.8, hMemDC, 0, 0, width, height, SRCCOPY);
	//	EndPaint(hwnd, &ps);
	//	return 0;
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		//PostQuitMessage(0);//OS�ɑ΂��āA�I��������Ƃ�`����
		////����lpPixel���������ׂ��炸
		//DeleteDC(hMemDC);
		//DeleteObject(hBitmap);	//BMP���폜�������AlpPixel�������I�ɉ�������
		PostQuitMessage(0);
		return 0;
	}



	return DefWindowProc(hwnd, msg, wparam, lparam);//�W���̏���������
}

#pragma endregion


//�f�o�C�X�������̎��s�����
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}


/// <summary>
/// �񎟌��̃A�t�B���ϊ�������֐�
/// </summary>
/// <param name="box">���_�f�[�^(X,Y)</param>
/// <param name="box">�}�`�̒��̌��_(X,Y)</param>
/// <param name="moveX">x�����̈ړ���</param>
/// <param name="moveY">y�����̈ړ���</param>
/// <param name="rotate">��]�p�x(�x���@)</param>
/// <param name="scaleX">x�����̊g�嗦</param>
/// <param name="scaleY">y�����̊g�嗦</param>
XMFLOAT3 Afin(XMFLOAT3 box, XMFLOAT3 box2, float moveX, float moveY, float rotate, float scaleX, float scaleY);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region �E�B���h�E�̏���������
	const int window_width = 1280;
	const int window_height = 720;


	WNDCLASSEX w = {};//�E�B���h�E�N���X�̐ݒ�

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)windowProc;//�E�B���h�E�v���V�[�W��
	w.lpszClassName = L"DirectXGame";//�E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);//�E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);

	//�E�B���h�E�̃T�C�Y�̍\����{x���W,y���W,����,�c��}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�����ŃT�C�Y�␳

	//�E�B���h�E�̍\���v�f�H
	HWND hwnd = CreateWindow(w.lpszClassName,//�N���X��
		L"DirectXGame",//�^�C�g���o�[�̖��O
		WS_OVERLAPPEDWINDOW,//�W���I�ȃE�C���h�E�X�^�C��
		CW_USEDEFAULT,//x���W(OS�ɔC����)
		CW_USEDEFAULT,//y���W(OS�ɔC����)
		wrc.right - wrc.left,//�E�B���h�E����
		wrc.bottom - wrc.top,//�E�B���h�E�c��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		w.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�I�v�V����

	//�E�B���h�E�\������
	ShowWindow(hwnd, SW_SHOW);

	//����������Ȃ��ƃE�B���h�E����u�ŏ����邽�ߋL�q
	MSG msg{};

#pragma endregion
	
	DirectXInit* directXinit;

	directXinit = DirectXInit::GetInstance();

	directXinit->Init(w, hwnd);

	DrawingObj charactorObj(window_width, window_height, { 0.0f,window_height,0.0f }, { 0.0f,0.0f,0.0f }, { window_width,window_height,0.0f }, { window_width,0.0f,0.0f });


	charactorObj.basicInit(directXinit->Getdev());

	DrawingObj testObj(window_width, window_height, { 0.0f,100.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,0.0f }, { 100.0f,0.0f,0.0f });

	testObj.colorChangeInit(directXinit->Getdev());

	/*DrawingObj screenObj(window_width,window_height);

	screenObj.screenImageInit(directXinit->Getdev());*/



	//�p�C�v���C���X�e�[�g�؂�ւ��p�t���O
	bool PipeLineRuleFlag = true;

	//�l�p�`�ɕύX����Ƃ��̃t���O
	bool ChangeSquareFlag = true;

	//�S�L�[�̓��͏����擾����ׂ̕ϐ�
	/*BYTE key[256] = {};
	BYTE oldKey[256] = {};*/

	
	float Red = 1.0f;
	float Green = 1.0f;
	float Blue = 1.0f;

	float X1 = 0.0f;
	float X2 = 0.0f;
	float Y1 = 0.0f;
	float Y2 = 0.0f;
	float rotate = 0;
	float scaleX = 1;
	float scaleY = 1;

	


	//�Q�[�����[�v
	while (true)
	{
		//���b�Z�[�W������H
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg);//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (msg.message == WM_QUIT)
		{
			break;
		}

#pragma region DirectX���t���[������

		//DirectX���t���[�������@��������
		
#pragma region �L�[�{�[�h���̎擾

		////�L�[�{�[�h���̎擾�J�n
		//directXinit.GetKeyBoard()->Acquire();

		////�O�t���[���̃L�[�{�[�h���͂�ۑ�
		//for (int i = 0; i < 256; i++)
		//{
		//	oldKey[i] = key[i];
		//}

		////�S�L�[�̓��͏����擾����
		//directXinit.GetKeyBoard()->GetDeviceState(sizeof(key), key);

#pragma endregion

		directXinit->DrawStart();

#pragma region �`��R�}���h

		// 4.�`��R�}���h��������
		
		//�r���[�|�[�g�ݒ�
		//�������̃r���[�|�[�g��p��(���ɂ����̂Ō��炵��)
		D3D12_VIEWPORT viewport{};

		viewport.Width = window_width;//����
		viewport.Height = window_height;//�c��
		viewport.TopLeftX = 0;//����X
		viewport.TopLeftY = 0;//����Y
		viewport.MinDepth = 0.1f;//�ŏ��[�x(0�ł悢)
		viewport.MaxDepth = 1.0f;//�ő�[�x(�P�ł悢)

		//�R�}���h���X�g�ɒǉ�
		directXinit->GetcmdList()->RSSetViewports(1, &viewport);
		

		//�V�U�[��`�ݒ�
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//�؂蔲�����W��
		scissorrect.right = scissorrect.left + window_width;//�؂蔲�����W�E
		scissorrect.top = 0;//�؂蔲�����W��
		scissorrect.bottom = scissorrect.top + window_height;//�؂蔲�����W��

		directXinit->GetcmdList()->RSSetScissorRects(1, &scissorrect);

		//�v���~�e�B�u�`��(�O�p�`���X�g)
		directXinit->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		testObj.Draw(directXinit->GetcmdList(), PipeLineRuleFlag, ChangeSquareFlag);
		
		if (directXinit->GetScreenImage()->GetImage(0,0,0) != nullptr)
		{
			charactorObj.SRVmake(directXinit->Getdev(), directXinit->GetScreenImage());
		}

		charactorObj.Draw(directXinit->GetcmdList(), PipeLineRuleFlag, ChangeSquareFlag);
		
		// 4.�`��R�}���h�����܂�

#pragma endregion

//		directXinit->Direct3D9TakeScreenshots({0,0,window_width,window_height});

		//screenObj.screenImageTextureBuffGeneraion(directXinit->Getdev());

		directXinit->DrawEnd();

		//DirectX���t���[�������@�����܂�

		//break;

#pragma endregion

#pragma region �X�V����

		X1 = 0.0f;
		Y1 = 0.0f;
		rotate = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;

		if (directXinit->PushKey(DIK_D))
		{
			OutputDebugStringA("Hit D\n");
		}

		if (directXinit->TriggerKey(DIK_2))
		{
			PipeLineRuleFlag = !PipeLineRuleFlag;
			OutputDebugStringA("Hit W\n");
		}

		if (directXinit->PushKey(DIK_W))
		{
			X2 += 0.001f;
		}

		if (directXinit->PushKey(DIK_S) )
		{
			X2 -= 0.001f;
		}

		if (directXinit->PushKey(DIK_SPACE))
		{
			directXinit->clearColorChange(0.9f, 0.2f, 0.5f, 0.0f);
		}
		else
		{
			directXinit->clearColorChange(0.1f, 0.25f, 0.5f, 0.0f);
		}

		if (directXinit->TriggerKey(DIK_1))
		{
			ChangeSquareFlag = !ChangeSquareFlag;
		}


		//�F�ύX
		/*if (Red > 0 and Blue <= 0)
		{
			Red -= 0.05f;
			Green += 0.05f;
		}
		if (Green > 0 and Red <= 0)
		{
			Green -= 0.05f;
			Blue += 0.05f;
		}
		if (Blue > 0 and Green <= 0)
		{
			Blue -= 0.05f;
			Red += 0.05f;
		}*/

#pragma endregion

#pragma region �`�揈��

		charactorObj.constBuffPosMUpdata(X1, X2, 0);

#pragma endregion

		StretchBlt(hdc, 0.01, 0.01, width * 0.8, height * 0.8, hMemDC, 0, 0, width, height, SRCCOPY);

	}

	//�E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);


	return 0;

	
}

