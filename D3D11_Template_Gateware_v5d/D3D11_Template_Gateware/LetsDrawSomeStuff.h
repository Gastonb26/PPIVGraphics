// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include <DirectXMath.h>

#include "DDSTextureLoader.h"

#include "PS_shader.csh"
#include "VS_shader.csh"
#include "PS_texture.csh"
#include "PS_point.csh"
//#include "PS_emissive"

#include "PS_sky.csh"
#include "VS_sky.csh"

#include "FitMetaknight00.h"

using namespace DirectX;

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	struct myVertex
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct ConstantBuffer
	{
		XMMATRIX mWorld;
		XMMATRIX mView;
		XMMATRIX mProjection;
		XMFLOAT4 vLightDir[2];
		XMFLOAT4 vLightCol[2];
		XMFLOAT4 vOutputCol;
		XMFLOAT3 pointLight;
		float pad;
		XMFLOAT3 spotLight;
	};

	struct CamInfo
	{
		XMVECTOR eye;
		XMVECTOR at;
		XMVECTOR up;
		float posX, posY, posZ;
		float yaw, pitch, roll;
		float speed = 0.5f;
		float nearPlane = 0.02f;
		float farPlane = 200.0f;
		float camZoom = 1.0f;
		int lightControl = 1;

	} camera;

	//Might need these for later
	float aspectR;
	unsigned int height = 0;
	unsigned int width = 0;

	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	GW::SYSTEM::GWindow *resize = nullptr;	// attach point? Might need this for adjusting aspect ratio! 

	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)

	// SHADERS
	ID3D11VertexShader* myVertexShader = nullptr; 
	ID3D11VertexShader* mySkyVshader = nullptr; 

	ID3D11PixelShader* myPixelShader = nullptr; 
	ID3D11PixelShader* myTextureShader = nullptr; 
	ID3D11PixelShader* mySkyPshader = nullptr; 
	ID3D11PixelShader* myPointShader = nullptr; 
	ID3D11PixelShader* myEmissiveShader = nullptr; 

	ID3D11InputLayout* inputLayout = nullptr; 
	ID3D11SamplerState* samplerState = nullptr; 
	D3D_DRIVER_TYPE DDT = D3D_DRIVER_TYPE_NULL; 

	// MATRICES
	XMMATRIX worldMat; 
	XMMATRIX viewMat;
	XMMATRIX projectionMat;
	//SkyBox
	XMMATRIX skyMat = XMMatrixTranslation(0.0f, 0.0f, 5.0f);
	//Models (for lights and objects)
	XMMATRIX spotMat = XMMatrixTranslation(5.0f, 0.0f, 0.0f);
	XMMATRIX metaMat = XMMatrixTranslation(0.0f, 4.0f, 0.0f); 

	// Light Positions
	XMFLOAT3 pointPos = XMFLOAT3(0.0f, 0.0f, -5.0f);
	XMFLOAT3 spotPos = XMFLOAT3(5.0f, 0.0f, 0.0f);

	// BUFFERS
	ID3D11Buffer* constantBuffer = nullptr; 
	
	ID3D11Buffer* vertexBuffer = nullptr; // cube
	ID3D11Buffer* indexBuffer = nullptr; // cube

	ID3D11Buffer* skyVertexBuffer = nullptr; 
	ID3D11Buffer* skyIndexBuffer = nullptr; 

	ID3D11Buffer* metaVertexBuffer = nullptr;
	ID3D11Buffer* metaIndexBuffer = nullptr;

	//TEXTURES
	ID3D11ShaderResourceView* textureResource = nullptr; 
	ID3D11ShaderResourceView* skyResource = nullptr; 
	//ID3D11ShaderResourceView* emissiveTex = nullptr; 










public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();

	void MoveCamera(); 
};

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	if (attatchPoint) // valid window?
	{
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);
			

			//USE T
			resize = attatchPoint; 
			resize->GetClientHeight(height); 
			resize->GetClientWidth(width); 
			aspectR = (float)width / (float)height;


			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)
			D3D11_INPUT_ELEMENT_DESC vLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			UINT numberOfElements = ARRAYSIZE(vLayout); 
			myDevice->CreateInputLayout(vLayout, numberOfElements, VS_shader, sizeof(VS_shader), &inputLayout);


			//////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////SECTION : MODELS //////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////
			
			////////////////////////// CUBE FOR CAMERAS //////////////////////////////////////////////
			myVertex tri[]
			{ //xyzq, rgba, uv 
				{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

				{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },

			};

			D3D11_BUFFER_DESC bDesc = {};
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 24;
			bDesc.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA subData;
			subData.pSysMem = tri;

			myDevice->CreateBuffer(&bDesc, &subData, &vertexBuffer);

			//Create Index Buffer
			WORD indices[] =
			{
				3,1,0,
				2,1,3,

				6,4,5,
				7,4,6,

				11,9,8,
				10,9,11,

				14,12,13,
				15,12,14,

				19,17,16,
				18,17,19,

				22,20,21,
				23,20,22
			};

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 36;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = indices;

			myDevice->CreateBuffer(&bDesc, &subData, &indexBuffer);


			///////////////////////////// SKYBOX //////////////////////////////////////////

			myVertex SKYBOX[]
			{ //xyzq, rgba, uv 
				{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
				{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

				{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
				{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

				{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
				{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },

			};

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 24;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = SKYBOX;

			myDevice->CreateBuffer(&bDesc, &subData, &skyVertexBuffer);

			//Create Index Buffer
			WORD SKY_indices[] =
			{
				3,0,1,
				2,3,1,

				6,5,4,
				7,6,4,

				11,8,9,
				10,11,9,

				14,13,12,
				15,14,12,

				19,16,17,
				18,19,17,

				22,21,20,
				23,22,20
			};

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 36;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = SKY_indices;

			myDevice->CreateBuffer(&bDesc, &subData, &skyIndexBuffer);

								
			///////////////////////// META //////////////////////////////

			myVertex* tempMeta = new myVertex[5979]; 
			for (int i = 0; i < 5979; i++)
			{
				tempMeta[i].Position.x = FitMetaknight00_data[i].pos[0];
				tempMeta[i].Position.y = FitMetaknight00_data[i].pos[1];
				tempMeta[i].Position.z = FitMetaknight00_data[i].pos[2];

				tempMeta[i].Normal.x = FitMetaknight00_data[i].nrm[0];
				tempMeta[i].Normal.y = FitMetaknight00_data[i].nrm[1];
				tempMeta[i].Normal.z = FitMetaknight00_data[i].nrm[2];

				tempMeta[i].Tex.x = FitMetaknight00_data[i].uvw[0];
				tempMeta[i].Tex.y = FitMetaknight00_data[i].uvw[1];
			}

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 5979;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = tempMeta;

			myDevice->CreateBuffer(&bDesc, &subData, &metaVertexBuffer);


			WORD* tempMetaIndices = new WORD[22242]; 
			for (int i = 0; i < 22242; i++)
			{
				tempMetaIndices[i] = FitMetaknight00_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 22242;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = tempMetaIndices;

			myDevice->CreateBuffer(&bDesc, &subData, &metaIndexBuffer);
			////////////////////// SECTION 3: MODELS END ////////////////////////////////////////

			////////////////////// SECTION 4: work in progress( constant buffers, camera, shaders, textures////////////////////////////

			// Create Constant Buffer
			D3D11_BUFFER_DESC constantBufferDesc = {};
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
			constantBufferDesc.CPUAccessFlags = 0;

			myDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);


			//write and compile & load our shaders
			myDevice->CreatePixelShader(&PS_shader, sizeof(PS_shader), nullptr, &myPixelShader);
			myDevice->CreatePixelShader(&PS_texture, sizeof(PS_texture), nullptr, &myTextureShader);
			myDevice->CreateVertexShader(&VS_shader, sizeof(VS_shader), nullptr, &myVertexShader);
			myDevice->CreatePixelShader(&PS_sky, sizeof(PS_sky), nullptr, &mySkyPshader); 
			myDevice->CreateVertexShader(&VS_sky, sizeof(VS_sky), nullptr, &mySkyVshader);
			myDevice->CreatePixelShader(&PS_point, sizeof(PS_point), nullptr, &myPointShader); 

			D3D11_SAMPLER_DESC sDesc = {}; 

			CreateDDSTextureFromFile(myDevice, L"metaTexture.dds", nullptr, &textureResource);
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;
			
			CreateDDSTextureFromFile(myDevice, L"SkyboxOcean.dds", nullptr, &skyResource); 
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;

			myDevice->CreateSamplerState(&sDesc, &samplerState);


			//INIT CAMERA
			worldMat = XMMatrixIdentity();

			//init View Matrix
			camera.eye = XMVectorSet(0.0f, 5.0f, -30.0f, 0.0f);
			camera.at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			camera.up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewMat = XMMatrixLookAtRH(camera.eye, camera.at, camera.up);

			//init Projection Matrix
			
			projectionMat = XMMatrixPerspectiveFovRH(XMConvertToRadians(65)/camera.camZoom, mySurface->GetAspectRatio(aspectR), camera.nearPlane, camera.farPlane);
		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	// Release DX Objects aquired from the surface
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();
	// SHADERS
	myVertexShader->Release(); 
	mySkyVshader->Release(); 
	myPixelShader->Release(); 
	mySkyPshader->Release(); 
	myTextureShader->Release(); 
	myPointShader->Release(); 

	//BUFFERS
	constantBuffer->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	skyVertexBuffer->Release(); 
	skyIndexBuffer->Release(); 
	metaVertexBuffer->Release(); 
	metaIndexBuffer->Release(); 

	//TEXTURES 
	textureResource->Release(); 
	samplerState->Release();



	// TODO: "Release()" more stuff here!

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		// this could be changed during resolution edits, get it every frame
		ID3D11RenderTargetView *myRenderTargetView = nullptr;
		ID3D11DepthStencilView *myDepthStencilView = nullptr;
		if (G_SUCCESS(mySurface->GetRenderTarget((void**)&myRenderTargetView)))
		{
			// Grab the Z Buffer if one was requested
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
				myDepthStencilView->Release();
			}

			/*if (mySwapChain)
			//{
			//	resize->GetClientHeight(height);
			//	resize->GetClientWidth(width);
			//	aspectR = (float)width / (float)height;
			//	resize->ResizeWindow(height, width);
			//	resize.
				//myContext->OMSetRenderTargets(0, 0, 0);
				//myRenderTargetView->Release();
				//mySwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

				//ID3D11Texture2D* pBuffer;
				//mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

				//myDevice->CreateRenderTargetView(pBuffer, NULL, &myRenderTargetView);

				//pBuffer->Release();

				//myContext->OMSetRenderTargets(1, &myRenderTargetView, NULL);

				//D3D11_VIEWPORT re;
				//resize->GetClientWidth(width);
				//resize->GetClientHeight(height);
				//re.Width = (float)width;
				//re.Height = (float)height;
				//re.MinDepth = 0.0f;
				//re.MaxDepth = 1.0f;
				//re.TopLeftX = 0;
				//re.TopLeftY = 0;
				//myContext->RSSetViewports(1, &re);

				//resize->ResizeWindow
			//}*/
			
			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			// Clear the screen to dark green
			const float d_green[] = { 0, 0.5f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);
			
			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!

			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			myContext->IASetInputLayout(inputLayout);

			myContext->PSSetSamplers(0, 1, &samplerState);
			//myContext->PSSetShaderResources(0, 1, &textureResource);

			UINT stride = sizeof(myVertex);
			UINT offset = 0;

			ID3D11Buffer *tempBuffer[] = { {vertexBuffer }, {metaVertexBuffer} , {skyVertexBuffer} };


			////////////////////////////////////LIGHTS////////////////////////////////////
			// you originally declared T here (look into using xTIME instead) 
			static float t = +(float)XM_PI * 0.0125f;
			if (DDT == D3D_DRIVER_TYPE_REFERENCE)
			{
				t += (float)XM_PI * 0.0125f;
			}
			else
			{
				static ULONGLONG timeStart = 0;
				ULONGLONG timeCur = GetTickCount64();
				if (timeStart == 0)
					timeStart = timeCur;
				t = (timeCur - timeStart) / 1000.0f;
			}

			//Camera Movement
			MoveCamera(); 

			// Rotate cube around the origin
			worldMat = XMMatrixRotationY(t);


			XMFLOAT4 vLightDirs[2] =
			{
				XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
				XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
			};
			XMFLOAT4 vLightColors[2] =
			{
				XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
				XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f)
			};
			XMMATRIX mRotate = XMMatrixRotationY(-2.0f * t);
			XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
			vLightDir = XMVector3Transform(vLightDir, mRotate);
			XMStoreFloat4(&vLightDirs[1], vLightDir);


			ConstantBuffer constBuffer;

			//COPY PASE THIS BLOCK OF CODE FOR DRAWIN, JUST REMEMBER TO CHANGE VERTEX AND PIXEL SHADERS.
			constBuffer.mWorld = XMMatrixTranspose(worldMat);
			constBuffer.mView = XMMatrixTranspose(viewMat);
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);
			constBuffer.vLightDir[0] = vLightDirs[0];
			constBuffer.vLightDir[1] = vLightDirs[1];
			constBuffer.vLightCol[0] = vLightColors[0];
			constBuffer.vLightCol[1] = vLightColors[1];
			constBuffer.vOutputCol = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
			myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
			myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			//Vertex shader
			myContext->PSSetShader(myPixelShader, 0, 0);
			//Pixel Shader					
			myContext->VSSetShader(myVertexShader, 0, 0);
			myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
			myContext->DrawIndexed(36, 0, 0);
			//STOP THE COPY PASTE.

			///// UPDATE LIGHTS
			for (int m = 0; m < 2; m++)
			{
				
				XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&vLightDirs[m]));
				XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
				mLight = mLightScale * mLight;
				
				// Update the world variable to reflect the current light
				constBuffer.mWorld = XMMatrixTranspose(mLight);
				constBuffer.vOutputCol = vLightColors[m];
				myContext->PSSetShader(myPixelShader, NULL, 0);
				myContext->UpdateSubresource(constantBuffer, 0, NULL, &constBuffer, 0, 0);
				myContext->DrawIndexed(36, 0, 0);
			}

			//////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////SECTION 2: MODELS//////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////
			XMMATRIX scale;
			XMMATRIX trans; 
		
			constBuffer.pointLight = pointPos; 
			constBuffer.spotLight = spotPos; 
			constBuffer.vOutputCol = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			
			/////////////////MODEL : FLoor
			myContext->PSSetShaderResources(0, 1, &textureResource);

			scale = XMMatrixScaling(30.0f, 0.05f, 30.0f);
			trans = XMMatrixTranslation(0.0f, -5.0f, 0.0f); 
			
			
			constBuffer.mWorld = XMMatrixTranspose(worldMat * scale * trans);
			constBuffer.mView = XMMatrixTranspose(viewMat);
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);

			myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
			myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			//Vertex shader 
			myContext->PSSetShader(myPointShader, 0, 0);
			//Pixel Shader					
			myContext->VSSetShader(myVertexShader, 0, 0);
			myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
			myContext->DrawIndexed(36, 0, 0);
			//STOP THE COPY PASTE.

			////////////////////////////MODEL : SPOT CAM BOX

			scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
			constBuffer.vOutputCol = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

			constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * spotMat);
			constBuffer.mView = XMMatrixTranspose(viewMat);
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);

			myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
			myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

			//Vertex shader
			myContext->PSSetShaderResources(0, 1, &textureResource);
			myContext->PSSetShader(myTextureShader, 0, 0);
			//Pixel Shader					
			myContext->VSSetShader(myVertexShader, 0, 0);
			myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
			myContext->DrawIndexed(36, 0, 0);

			//////////////////////////////MODEL : META
			
			myContext->PSSetShaderResources(0, 1, &textureResource);

			scale = XMMatrixScaling(1/5.0f, 1 / 5.0f, 1 / 5.0f); 
			constBuffer.mWorld = XMMatrixTranspose(worldMat * scale * metaMat);
			constBuffer.mView = XMMatrixTranspose(viewMat);
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);
			myContext->PSSetShader(myTextureShader, 0, 0);
			//constBuffer.vOutputCol = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			myContext->IASetVertexBuffers(0, 1, &tempBuffer[1], &stride, &offset);
			myContext->IASetIndexBuffer(metaIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

			////Pixel shader
			//myContext->PSSetShader(myPixelShader, 0, 0);
			////Vertex Shader					
			myContext->VSSetShader(myVertexShader, 0, 0);
			myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
			myContext->DrawIndexed(22242, 0, 0);


			//////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////SECTION 3: SKYBOX /////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////

			//Object data
			myContext->PSSetShaderResources(0, 1, &skyResource);
			
			scale = XMMatrixScaling(100.0f, 100.0f, 100.0f); 
			constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * skyMat);
			constBuffer.mView = XMMatrixTranspose(viewMat);
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);
			//Set Vertex and Index buffers
			myContext->IASetVertexBuffers(0, 1, &tempBuffer[2], &stride, &offset);
			myContext->IASetIndexBuffer(skyIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			//Pixel shader
			myContext->PSSetShader(mySkyPshader, 0, 0);
			//Vertex Shader					
			myContext->VSSetShader(mySkyVshader, 0, 0);
			//Constant Buffers for V and P shaders
			myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->PSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
			myContext->DrawIndexed(36, 0, 0);

			//////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////SECTION 4: SwapChain & more ///////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////


	
			////////////////////////////// UPDATE CAMERA //////////////////////////////////// 
			constBuffer.mView = XMMatrixTranspose(viewMat); 
			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
			////////////////////////////// END UPDATE ///////////////////////////////////////
		

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display
			
			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();

			//mySwapChain->ResizeBuffers(0,0,0, XGI_FORMAT_UNKNOWN, 0)
		}
	}
}

void LetsDrawSomeStuff::MoveCamera()
{

	//Increase Movement Speed 
	if (GetAsyncKeyState(VK_SHIFT))
	{
		camera.speed = 1.5f; 
	}

	//MOVE Forward, Back, Left, Right
	if (GetAsyncKeyState('W'))
	{
		camera.posZ -= camera.speed *.05f;
	}
	else if (GetAsyncKeyState('S'))
	{
		camera.posZ += camera.speed*.05f;
	}
	if (GetAsyncKeyState('D'))
	{
		camera.posX -= camera.speed *.05f;
	}
	else if (GetAsyncKeyState('A'))
	{
		camera.posX += camera.speed*.05f;
	}

	//Roll, Pitch, Yaw
	if (GetAsyncKeyState('Q'))
	{
		camera.roll -= camera.speed*.005f;
	}
	else if (GetAsyncKeyState('E'))
	{
		camera.roll += camera.speed*.005f;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		camera.pitch += camera.speed*.005f;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		camera.pitch -= camera.speed*.005f;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		camera.yaw += camera.speed*.005f;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		camera.yaw -= camera.speed*.005f;
	}

	//////////  Controls for Lights ( 1: Point, 2: Spot, etc.)
	if (GetAsyncKeyState('1'))
	{
		camera.lightControl = 1; 
	}
	if (GetAsyncKeyState('2'))
	{
		camera.lightControl = 2; 
	}

	if (GetAsyncKeyState('I'))
	{
		switch (camera.lightControl)
		{
		case 1: 
			pointPos.z -= 0.05f * camera.speed;
			break; 
		case 2: 
			spotPos.z -= 0.05f  * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, 0.0f, -0.05f * camera.speed);
			break; 
		default:
			break;
		}	
	}
	else if (GetAsyncKeyState('K'))
	{
		switch (camera.lightControl)
		{
		case 1:
			pointPos.z += 0.05f* camera.speed;
			break;
		case 2:
			spotPos.z += 0.05f * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, 0.0f, 0.05f *camera.speed);
			break;
		default:
			break;
		}
	}
	if (GetAsyncKeyState('J'))
	{
		switch (camera.lightControl)
		{
		case 1:
			pointPos.x += 0.05f* camera.speed;
			break;
		case 2:
			spotPos.x += 0.05f * camera.speed;
			spotMat *= XMMatrixTranslation(0.05f * camera.speed, 0.0f, 0.0f);
			break;
		default:
			break;
		}
	}
	else if (GetAsyncKeyState('L'))
	{
		switch (camera.lightControl)
		{
		case 1:
			pointPos.x -= 0.05f * camera.speed;
			break;
		case 2:
			spotPos.x -= 0.05f  * camera.speed;
			spotMat *= XMMatrixTranslation(-0.05f * camera.speed, 0.0f, 0.0f);
			break;
		default:
			break;
		}
	}
	if (GetAsyncKeyState('U'))
	{
		switch (camera.lightControl)
		{
		case 1:
			pointPos.y += 0.05f * camera.speed;
			break;
		case 2:
			spotPos.y += 0.05f  * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, 0.05f * camera.speed, 0.0f);
			break;
		default:
			break;
		}
	}
	else if (GetAsyncKeyState('O'))
	{
		switch (camera.lightControl)
		{
		case 1:
			pointPos.y -= 0.05f * camera.speed;
			break;
		case 2:
			spotPos.y -= 0.05f  * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, -0.05f * camera.speed, 0.0f);
			break;
		default:
			break;
		}
	}

	// Near/Far Plane and Zoom adjustments
	if (GetAsyncKeyState('Z'))
	{
		camera.nearPlane -= 0.1f; 
		if (camera.nearPlane < 0.01f)
		{
			camera.nearPlane = 0.01f;
		}
	}
	else if (GetAsyncKeyState('X'))
	{
		camera.nearPlane += 0.1f;
	}
	if (GetAsyncKeyState('N'))
	{
		camera.farPlane -=  0.1f;
	}
	else if (GetAsyncKeyState('M'))
	{
		camera.farPlane += 0.1f; 
	}

	if (GetAsyncKeyState('T'))
	{
		camera.camZoom -= 0.001f; 
	}
	else if (GetAsyncKeyState('Y'))
	{
		camera.camZoom += 0.001f;
	}

	// Update Camera
	viewMat = XMMatrixInverse(nullptr, viewMat);
	viewMat = XMMatrixTranslationFromVector(XMVECTOR{ 0,0, camera.posZ})*viewMat;
	viewMat = XMMatrixTranslationFromVector(XMVECTOR{ camera.posX,0,0 })*viewMat;

	viewMat = viewMat * XMMatrixTranslationFromVector(XMVECTOR{ 0,camera.posY,0 });
	viewMat = XMMatrixRotationX(camera.pitch)* viewMat;
	viewMat = XMMatrixRotationZ(camera.roll) * viewMat; 

	XMVECTOR tmp= viewMat.r[3];
	viewMat.r[3] = XMVECTOR{ 0,0,0 };
	viewMat = viewMat * XMMatrixRotationY(camera.yaw);
	viewMat.r[3] = tmp;

	camera.eye = viewMat.r[3];
	camera.at = viewMat.r[2];
	camera.up = viewMat.r[1];

	viewMat = XMMatrixInverse(0, viewMat);

	//Adjust for infinite skybox
	skyMat = XMMatrixTranslation(XMVectorGetX(camera.eye), XMVectorGetY(camera.eye), XMVectorGetZ(camera.eye));

	projectionMat = XMMatrixPerspectiveFovRH(XMConvertToRadians(65)/camera.camZoom, mySurface->GetAspectRatio(aspectR), camera.nearPlane, camera.farPlane);

	//aspectR = 0; 
	camera.posX = 0; 
	camera.posZ = 0;
	camera.yaw = 0; 
	camera.roll = 0; 
	camera.yaw = 0; 
	camera.pitch = 0; 
	camera.speed = 0.5f; 
}