#include<Windows.h>

//D3Dコンパイラ
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include<vector>
#include<string>

#include<d3d12.h>
#include<dxgi1_6.h>

//数学ライブラリ(最適化されすぎて使いにくいらしいのであとで自作しろって)
#include<DirectXMath.h>
using namespace DirectX;

//#pragma commentとは、オブジェクトファイルに、
//リンカでリンクするライブラリの名前を記述するもの
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

const float PI = 3.141592653589;

#include <DirectXTex.h>

#include "DrawingObj.h"

#include "DirectXInit.h"

#pragma region ウィンドウプロシージャ

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
	//	//スクリーンの情報を得る
	//	desktop = GetDesktopWindow();
	//	GetWindowRect(desktop, &rc);
	//	width = 1920;
	//	height = 1020;

	//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	//	//DIBの情報を設定する
	//	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//	bmpInfo.bmiHeader.biWidth = width;
	//	bmpInfo.bmiHeader.biHeight = height;
	//	bmpInfo.bmiHeader.biPlanes = 1;
	//	bmpInfo.bmiHeader.biBitCount = 32;
	//	bmpInfo.bmiHeader.biCompression = BI_RGB;



	//	//DIBSection作成
	//	hdc = GetDC(hwnd);
	//	hBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
	//	hMemDC = CreateCompatibleDC(hdc);
	//	SelectObject(hMemDC, hBitmap);
	//	ReleaseDC(hwnd, hdc);

	//	//スクリーンをDIBSectionにコピー
	//	hdc = GetDC(desktop);
	//	BitBlt(hMemDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	//	ReleaseDC(desktop, hdc);
	//	return 0;
	//case WM_PAINT:
	//	hdc = BeginPaint(hwnd, &ps);
	//	//表画面へ転送


	//	//BitBlt(hdc, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);
	//	StretchBlt(hdc, 0.01, 0.01, width * 0.8, height * 0.8, hMemDC, 0, 0, width, height, SRCCOPY);
	//	EndPaint(hwnd, &ps);
	//	return 0;
	case WM_DESTROY://ウィンドウが破棄された
		//PostQuitMessage(0);//OSに対して、終わったことを伝える
		////自らlpPixelを解放するべからず
		//DeleteDC(hMemDC);
		//DeleteObject(hBitmap);	//BMPを削除した時、lpPixelも自動的に解放される
		PostQuitMessage(0);
		return 0;
	}



	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理をする
}

#pragma endregion


//デバイス発見時の実行される
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}


/// <summary>
/// 二次元のアフィン変換をする関数
/// </summary>
/// <param name="box">頂点データ(X,Y)</param>
/// <param name="box">図形の中の原点(X,Y)</param>
/// <param name="moveX">x方向の移動量</param>
/// <param name="moveY">y方向の移動量</param>
/// <param name="rotate">回転角度(度数法)</param>
/// <param name="scaleX">x方向の拡大率</param>
/// <param name="scaleY">y方向の拡大率</param>
XMFLOAT3 Afin(XMFLOAT3 box, XMFLOAT3 box2, float moveX, float moveY, float rotate, float scaleX, float scaleY);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region ウィンドウの初期化部分
	const int window_width = 1280;
	const int window_height = 720;


	WNDCLASSEX w = {};//ウィンドウクラスの設定

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)windowProc;//ウィンドウプロシージャ
	w.lpszClassName = L"DirectXGame";//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);

	//ウィンドウのサイズの構造体{x座標,y座標,横幅,縦幅}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正

	//ウィンドウの構成要素？
	HWND hwnd = CreateWindow(w.lpszClassName,//クラス名
		L"DirectXGame",//タイトルバーの名前
		WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
		CW_USEDEFAULT,//x座標(OSに任せる)
		CW_USEDEFAULT,//y座標(OSに任せる)
		wrc.right - wrc.left,//ウィンドウ横幅
		wrc.bottom - wrc.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//ウィンドウ表示部分
	ShowWindow(hwnd, SW_SHOW);

	//これを書かないとウィンドウが一瞬で消えるため記述
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



	//パイプラインステート切り替え用フラグ
	bool PipeLineRuleFlag = true;

	//四角形に変更するときのフラグ
	bool ChangeSquareFlag = true;

	//全キーの入力情報を取得する為の変数
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

	


	//ゲームループ
	while (true)
	{
		//メッセージがある？
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//キー入力メッセージの処理
			DispatchMessage(&msg);//プロシージャにメッセージを送る
		}

		//×ボタンで終了メッセージが来たらゲームループを抜ける
		if (msg.message == WM_QUIT)
		{
			break;
		}

#pragma region DirectX毎フレーム処理

		//DirectX毎フレーム処理　ここから
		
#pragma region キーボード情報の取得

		////キーボード情報の取得開始
		//directXinit.GetKeyBoard()->Acquire();

		////前フレームのキーボード入力を保存
		//for (int i = 0; i < 256; i++)
		//{
		//	oldKey[i] = key[i];
		//}

		////全キーの入力情報を取得する
		//directXinit.GetKeyBoard()->GetDeviceState(sizeof(key), key);

#pragma endregion

		directXinit->DrawStart();

#pragma region 描画コマンド

		// 4.描画コマンドここから
		
		//ビューポート設定
		//分割分のビューポートを用意(見にくいので減らした)
		D3D12_VIEWPORT viewport{};

		viewport.Width = window_width;//横幅
		viewport.Height = window_height;//縦幅
		viewport.TopLeftX = 0;//左上X
		viewport.TopLeftY = 0;//左上Y
		viewport.MinDepth = 0.1f;//最少深度(0でよい)
		viewport.MaxDepth = 1.0f;//最大深度(１でよい)

		//コマンドリストに追加
		directXinit->GetcmdList()->RSSetViewports(1, &viewport);
		

		//シザー矩形設定
		D3D12_RECT scissorrect{};

		scissorrect.left = 0;//切り抜き座標左
		scissorrect.right = scissorrect.left + window_width;//切り抜き座標右
		scissorrect.top = 0;//切り抜き座標上
		scissorrect.bottom = scissorrect.top + window_height;//切り抜き座標下

		directXinit->GetcmdList()->RSSetScissorRects(1, &scissorrect);

		//プリミティブ形状(三角形リスト)
		directXinit->GetcmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		testObj.Draw(directXinit->GetcmdList(), PipeLineRuleFlag, ChangeSquareFlag);

		charactorObj.Draw(directXinit->GetcmdList(), PipeLineRuleFlag, ChangeSquareFlag);
		
		// 4.描画コマンドここまで

#pragma endregion

//		directXinit->Direct3D9TakeScreenshots({0,0,window_width,window_height});

		//screenObj.screenImageTextureBuffGeneraion(directXinit->Getdev());

		directXinit->DrawEnd();

		//DirectX毎フレーム処理　ここまで

		//break;

#pragma endregion

#pragma region 更新処理

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


		//色変更
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

#pragma region 描画処理

		charactorObj.constBuffPosMUpdata(X1, X2, 0);

#pragma endregion

		StretchBlt(hdc, 0.01, 0.01, width * 0.8, height * 0.8, hMemDC, 0, 0, width, height, SRCCOPY);

	}

	//ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);


	return 0;

	
}

