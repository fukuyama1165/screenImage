#pragma once
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

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

//キーボードやコントローラーなどの入力するヘッダとライブラリのリンク
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
	//初期化をここで適当にエラーをはかないと思う値を入れている基本的に書き換えても問題ないはず(設定が破綻してなければ)
	DrawingObj(const int windowWidth, const int windowHeight);

	//座標を指定できるが引数大杉
	DrawingObj(const int windowWidth, const int windowHeight,XMFLOAT3 vertexPos1, XMFLOAT3 vertexPos2, XMFLOAT3 vertexPos3, XMFLOAT3 vertexPos4, XMFLOAT2 vertexUv1 = { 0.0f,1.0f }, XMFLOAT2 vertexUv2 = { 0.0f,0.0f }, XMFLOAT2 vertexUv3 = { 1.0f,1.0f }, XMFLOAT2 vertexUv4 = { 1.0f,0.0f });

	~DrawingObj();

	//描画初期化処理関数
	void basicInit(ID3D12Device* dev);//basicPS読み込み
	void colorChangeInit(ID3D12Device* dev);//colorChangePS読み込み

	void screenImageInit(ID3D12Device* dev);
	void screenImageTextureBuffGeneraion(ID3D12Device* dev);

	//頂点データ構造体
	struct Vertex
	{
		XMFLOAT3 pos;//xyz座標
		XMFLOAT2 uv;//uv座標
	};

	//頂点バッファ生成
	void vertexBuffGeneration(ID3D12Device* dev);

	//頂点シェーダの読み込みとコンパイル
	void vertexShaderGeneration();//basicVS読み込み
	void vertexShaderGeneration2();//vertexMoveVS読み込み

	//ピクセルシェーダの読み込みとコンパイル
	void pixelShaderGeneration();//basicPS読み込み
	void pixelShaderGeneration2();//colorChangePS読み込み

	//頂点レイアウトの設定
	void vertexLayout();

	//グラフィックスパイプラインの設定
	void graphicPipelineGeneration();

	//デスクリプタレンジの設定
	void descriptorRangeGeneration();

	//ルートパラメータの設定(定数バッファとシェーダについて)
	void rootParamGeneration();
	
	//テクスチャサンプラーの設定
	void textureSamplerGeneration();

	//ルートシグネチャ
	void rootsignatureGeneration(ID3D12Device* dev);

	//定数バッファ
	void constantBuffGeneration(ID3D12Device* dev);

	//インデックスデータ関連(インデックスビューもここ)
	void indicesBuffGeneration(ID3D12Device* devconst);

	//画像イメージデータ
	void imageDataGeneration();

	//スクリーン画像イメージデータ
	void screenImageDataGeneration();

	//テクスチャバッファ
	void textureBuffGeneraion(ID3D12Device* dev);

	//シェーダリソースビュー
	void SRVGeneraion(ID3D12Device* dev);

	//GPUに頂点データを転送する関数
	void vertexMap();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="PipeLineRuleFlag">描画方法を変更するかどうか(現在はワイヤーフレームか塗りつぶし)trueは塗りつぶし</param>
	/// <param name="ChangeSquareFlag">三角形で描画するか四角形に描画する(trueは四角形)</param>
	void Draw(ID3D12GraphicsCommandList* cmdList,bool PipeLineRuleFlag, bool ChangeSquareFlag);


	//アフィン変換そのものの関数(2D)
	XMFLOAT3 Afin(XMFLOAT3 box, float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//Objに対してアフィン変換をかける関数
	void ObjAfin(float moveX, float moveY, float rotate, float scaleX, float scaleY);

	//定数バッファを変更する関数
	void constBuffColorUpdata(float Red,float Green,float Blue);
	void constBuffPosMUpdata(float X,float Y,float Z);

	

private:

	DirectXInit* directXInit_ = nullptr;

	//画面サイズ
	int Win_width;
	int Win_height;

	//頂点データ(増やしたいならここも増やしておく)
	Vertex vertices[4];

	//頂点データサイズ
	UINT sizeVB;

	HRESULT result;

	//頂点バッファ用変数
	D3D12_HEAP_PROPERTIES heapprop{};//ヒープ設定
	D3D12_RESOURCE_DESC resDesc{};//リソース設定
	ID3D12Resource* vertBuff = nullptr;
	Vertex* vertMap = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//頂点シェーダオブジェクト
	ID3DBlob* vsBlob = nullptr;

	//ピクセルシェーダオブジェクト
	ID3DBlob* psBlob = nullptr;

	//エラーオブジェクト
	ID3DBlob* errorBlob = nullptr;

	//頂点レイアウト(要素を増やすなら配列数を増やす)
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];

	//グラフィックスパイプラインの各ステージの設定をする構造体を用意
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline2{};

	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//ルートパラメータ(定数バッファの数が増えたら配列の要素数を増やして設定をしている関数の中身にも追加すること)
	D3D12_ROOT_PARAMETER rootParam[4] = {};

	//テクスチャサンプラー
	D3D12_STATIC_SAMPLER_DESC sampleDesc{};

	//ルートシグネチャ
	ID3D12RootSignature* rootsignature;

	//パイプラインステート
	ID3D12PipelineState* pipelinestate = nullptr;
	ID3D12PipelineState* pipelinestate2 = nullptr;

	//定数バッファ用のリソース設定関数
	D3D12_RESOURCE_DESC constBuffResourceGeneration(int size);

	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial
	{
		XMFLOAT4 color;//色(RGBA)
	};

	struct ConstBufferDataMaterial2
	{
		XMFLOAT4 posM;//位置移動に使う(XYZ);
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform
	{
		XMMATRIX mat;//3D変換行列
	};

	//定数バッファそのもの
	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12Resource* constBuffMaterial2 = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;

	//マッピングするときのポインタ
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	ConstBufferDataMaterial2* constMapMaterial2 = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;

	//インデックスデータ
	unsigned short indices[6];
	//インデックスデータ全体のサイズ
	UINT sizeIB;
	//インデックスビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	//画像データ等
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	ID3D12Resource* texBuff = nullptr;

	ID3D12DescriptorHeap* srvHeap = nullptr;

	
	
	ScratchImage imagescrreen;

};
