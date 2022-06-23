#include "DirectXInit.h"
#include <stdio.h>
#include <Wincodec.h>  

//DirectXInit* DirectXInit::instance = nullptr;

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define HRCHECK(__expr) {result=(__expr);if(FAILED(result)){wprintf(L"FAILURE 0x%08X (%i)\n\tline: %u file: '%s'\n\texpr: '" WIDEN(#__expr) L"'\n",result, result, __LINE__,__WFILE__);goto cleanup;}}
#define RELEASE(__p) {if(__p!=nullptr){__p->Release();__p=nullptr;}}


HRESULT SavePixelsToFile32bppPBGRA(UINT width, UINT height, UINT stride, LPBYTE pixels, LPWSTR filePath, const GUID& format)
{
	if (!filePath || !pixels)
		return E_INVALIDARG;

	HRESULT result = S_OK;
	IWICImagingFactory* factory = nullptr;
	IWICBitmapEncoder* encoder = nullptr;
	IWICBitmapFrameEncode* frame = nullptr;
	IWICStream* stream = nullptr;
	GUID pf = GUID_WICPixelFormat32bppPBGRA;
	BOOL coInit = CoInitialize(nullptr);

	HRCHECK(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory)));
	HRCHECK(factory->CreateStream(&stream));
	HRCHECK(stream->InitializeFromFilename(filePath, GENERIC_WRITE));
	HRCHECK(factory->CreateEncoder(format, nullptr, &encoder));
	HRCHECK(encoder->Initialize(stream, WICBitmapEncoderNoCache));
	HRCHECK(encoder->CreateNewFrame(&frame, nullptr)); // we don't use options here
	HRCHECK(frame->Initialize(nullptr)); // we dont' use any options here
	HRCHECK(frame->SetSize(width, height));
	HRCHECK(frame->SetPixelFormat(&pf));
	HRCHECK(frame->WritePixels(height, stride, stride * height, pixels));
	HRCHECK(frame->Commit());
	HRCHECK(encoder->Commit());

cleanup:
	RELEASE(stream);
	RELEASE(frame);
	RELEASE(encoder);
	RELEASE(factory);
	if (coInit) CoUninitialize();
	return result;
}

DirectXInit::~DirectXInit()
{
}

DirectXInit* DirectXInit::GetInstance()
{
	static DirectXInit instance;
	return &instance;
}


void DirectXInit::Init(WNDCLASSEX w, HWND hwnd)
{

#pragma region デバックレイヤーの有効化



#ifdef _DEBUG

	//デバックレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // DEBUG

#pragma endregion

	


	//グラフィックアダプター
	GraphicAdapterGeneration();

	//コマンドリスト
	CommandListGeneration();

	//コマンドキュー
	CommandQueueGeneration();

	//スワップチェーン
	SwapChainGeneration(hwnd);

	//レンダーターゲットビュー
	RTVGeneration();

	//フェンス
	FenceGeneration();

	//DirectInput(できたら専用クラスを作る)
	DirectInputGeneration(w, hwnd);
}

void DirectXInit::GraphicAdapterGeneration()
{
#pragma region グラフィックアダプター設定

	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	//アダプターの列挙用
	std::vector<IDXGIAdapter1*> adapters;

	//ここに特定の名前を持つアダプターオブジェクトが入る
	IDXGIAdapter1* tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);//アダプターの情報を取得

		//ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description;//アダプター名

		//Intel UHD Graphice(オンボードグラフィック)を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//採用
			break;
		}

	}

	//採用したグラフィックデバイスを操作するためのオブジェクトを生成
	//※普通は一つしか作らないのでクラス化等する際に複数生成しないように

	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)//ちょくちょくresultの中身がS_OKであることを確認すること
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion これのクラス化で複数生成に注意
}

void DirectXInit::CommandListGeneration()
{
#pragma region コマンドリスト辺

	//コマンドリストを生成するにはコマンドアロケータが必要
	//なので先にコマンドアロケータを生成

	//コマンドアロケータを生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator)
	);

	//コマンドリストを生成
	result = dev->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator,
		nullptr,
		IID_PPV_ARGS(&cmdList)
	);

#pragma endregion
}

void DirectXInit::CommandQueueGeneration()
{
#pragma region コマンドキュー辺

	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};//構造体の中身を{}で０でクリアしている(microsoftの標準が0のため)

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion
}

void DirectXInit::SwapChainGeneration(HWND hwnd)
{
#pragma region スワップチェーン辺

	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapchainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapchainDesc.BufferCount = 2;//バッファ数を2つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue,
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain
	);

#pragma endregion
}

void DirectXInit::RTVGeneration()
{
#pragma region レンダーターゲットビュー辺

	//レンダーターゲットビューはデスクリプタヒープから生成されるので
	//デスクリプタヒープを先に生成する

	//各種設定をしてデスクリプタヒープを生成
	
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;//裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//レンダーターゲットビューの生成部分


	//裏表の2つ分について
	for (int i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		//レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i],
			nullptr,
			handle
		);

	}

#pragma endregion バックバッファを描画キャンバスとして扱う為のオブジェクト
}


void DirectXInit::DrawStart()
{
#pragma region キーボード情報の取得

	//キーボード情報の取得開始
	keyboard->Acquire();

	//前フレームのキーボード入力を保存
	for (int i = 0; i < 256; i++)
	{
		oldKey[i] = key[i];
	}

	//全キーの入力情報を取得する
	keyboard->GetDeviceState(sizeof(key), key);

#pragma endregion



	//リソースバリア辺

	//バックバッファの番号を取得(２つなので0番か1番)
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// 1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex];//バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//表示から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
	cmdList->ResourceBarrier(1, &barrierDesc);

	// 2.描画先指定
	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

	

	// 3.画面クリア

	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}


void DirectXInit::DrawEnd()
{
	

	// 5.リソースバリアを戻す(書き込める状態だと描画できないので描画できるようにする)
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;//表示に
	cmdList->ResourceBarrier(1, &barrierDesc);

	//命令のクローズ(ここで描画の命令はそろったので実行にうつす)
	cmdList->Close();
	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList };
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	//バックバッファを画像データとして取得(ScratchImage型)
	result = CaptureTexture(cmdQueue, backBuffers[swapchain->GetCurrentBackBufferIndex()], false, imagescrreen);

	//画像を保存(危険なのでコメントアウト)実行するならここを元に戻すととられます
	//result = SaveToWICFile(*imagescrreen.GetImage(0, 0, 0), WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), L"NEW_IMAGE3.PNG");


	//画面に表示するバッファをフリップ(裏表の入れ替え)
	swapchain->Present(1, 0);

	//コマンド完了待ち(これを書かないとすぐ処理に移行してしまうため)
	//コマンドの実行完了を待つ
	cmdQueue->Signal(fence, ++fenceVel);
	if (fence->GetCompletedValue() != fenceVel)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVel, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator, nullptr);
}

void DirectXInit::clearColorChange(float R, float G, float B, float A)
{
	clearColor[0] = R;
	clearColor[1] = G;
	clearColor[2] = B;
	clearColor[3] = A;
}

bool DirectXInit::PushKey(BYTE CheckKey)
{
	return key[CheckKey];
}

bool DirectXInit::TriggerKey(BYTE CheckKey)
{
	return key[CheckKey] and oldKey[CheckKey] == 0;
}

//get関数群

void DirectXInit::FenceGeneration()
{
#pragma region フェンス辺
	//フェンスを生成
	result = dev->CreateFence(fenceVel, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion CPUとGPUで同期をとるためのDirectXの仕組み
}

void DirectXInit::DirectInputGeneration(WNDCLASSEX w, HWND hwnd)
{
#pragma region DirectInoutオブジェクトの生成

	//DirectInputの初期化
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		w.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion 全体の初期化であり入力デバイスが増えてもこのオブジェクトは増やさなくてもいい

#pragma region キーボードデバイスの生成

	//キーボードデバイスの生成
	
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region ゲームパッドの入力

	//DIDEVICEINSTANCE* gamePadDID = {};
	//result = directInput->EnumDevices(
	//	DI8DEVTYPE_JOYSTICK,//JOYSTICKだと認識しないことがあるらしい(確認できるなら確認すること)
	//	DeviceFindCallBack,
	//	&gamePadDID,
	//	DIEDFL_ATTACHEDONLY
	//);
	//assert(SUCCEEDED(result));

	//IDirectInputDevice8* gamePadDirct = nullptr;
	//result = directInput->CreateDevice(gamePadDID->guidInstance, &gamePadDirct, NULL);
	//assert(SUCCEEDED(result));


#pragma endregion(中止)

#pragma region 入力データ形式のセット

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region 排他制御レベルのセット

	result = keyboard->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND/*画面が手前にある場合のみ入力を受け付ける*/ |
		DISCL_NONEXCLUSIVE/*デバイスをこのアプリだけで占有しない*/ |
		DISCL_NOWINKEY/*windowキーを無効にする*/);
	assert(SUCCEEDED(result));

#pragma endregion
}

ID3D12Device* DirectXInit::Getdev()
{
	return dev;
}

IDirectInputDevice8* DirectXInit::GetKeyBoard()
{
	return keyboard;
}

ID3D12GraphicsCommandList* DirectXInit::GetcmdList()
{
	return cmdList;
}

HRESULT DirectXInit::Direct3D9TakeScreenshots(RECT winSize)
{

	//IDirect3D12* d3d = nullptr;
	//ID3D12Device* device = nullptr;

	////IDirect3DSurface9* surface = nullptr;
	//DXGI_SWAP_CHAIN_DESC1 parameters = { 0 };
	//RECT mode = winSize;
	////D3DLOCKED_RECT rc;
	//UINT pitch;
	//SYSTEMTIME st;
	//LPBYTE* shots = nullptr;

	//// init D3D and get screen size
	//d3d = Direct3DCreate9(D3D_SDK_VERSION);
	//HRCHECK(d3d->GetAdapterDisplayMode(adapter, &mode));



	// create device & capture surface
   /* HRCHECK(d3d->CreateDevice(adapter, D3DDEVTYPE_HAL, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device));
	HRCHECK(device->CreateOffscreenPlainSurface(mode.Width, mode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, nullptr));*/

	// compute the required buffer size
	//HRCHECK(surface->LockRect(&rc, NULL, 0));
	//pitch = mode.right;
	//HRCHECK(surface->UnlockRect());

	// allocate screenshots buffers
	//shots = new LPBYTE;
	//shots = new LPBYTE[pitch * (mode.bottom - mode.top)]();

	//GetSystemTime(&st); // measure the time we spend doing <count> captures
	//wprintf(L"%i:%i:%i.%i\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); 
		// get the data
	//result = swapchain->GetBuffer(1, IID_PPV_ARGS(&backBuffers[swapchain->GetCurrentBackBufferIndex()]));

	

	//result= CaptureTexture(cmdQueue, backBuffers[swapchain->GetCurrentBackBufferIndex()], false, imagescrreen);



	// copy it into our buffers
	//HRCHECK(surface->LockRect(&rc, NULL, 0));
	//CopyMemory(shots, backBuffers[swapchain->GetCurrentBackBufferIndex()], pitch * (mode.bottom - mode.top));
	//HRCHECK(surface->UnlockRect());
/*GetSystemTime(&st);
wprintf(L"%i:%i:%i.%i\n", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);*/

// save all screenshots
/* for (UINT i = 0; i < count; i++)

 {


 }*/
	/*WCHAR file[100];
	wsprintf(file, L"cap.png");
	HRCHECK(SavePixelsToFile32bppPBGRA((mode.right - mode.left), (mode.bottom - mode.top), pitch, *shots, file, GUID_ContainerFormatPng));*/

//cleanup:
//	if (shots != nullptr)
//	{
//		delete shots[0];
//		delete[] shots;
//	}
//	// RELEASE(surface);
//	RELEASE(device);
//	//RELEASE(d3d);
	return result;
}


ScratchImage* DirectXInit::GetScreenImage()
{
	return &imagescrreen;
}

ID3D12CommandQueue* DirectXInit::GetCmdQueue()
{
	return cmdQueue;
}

IDXGISwapChain4* DirectXInit::GetSwapchain()
{
	return swapchain;
}

std::vector<ID3D12Resource*> DirectXInit::GetBackBuffers()
{
	return backBuffers;
}



