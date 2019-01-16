// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"


// Include DirectX11 for interface access
#include <d3d11.h>
#include <DirectXMath.h>
#include "PS_shader.csh"
#include "VS_shader.csh"

using namespace DirectX;

struct myVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	//XMFLOAT2 Tex;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightCol[2];
	XMFLOAT4 vOutputCol;
};


float aspectR;

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* constantBuffer = nullptr;

	ID3D11VertexShader* myVertexShader = nullptr; 
	ID3D11PixelShader* myPixelShader = nullptr; 

	ID3D11InputLayout* inputLayout = nullptr; 
	D3D_DRIVER_TYPE DDT = D3D_DRIVER_TYPE_NULL; 

	XMMATRIX worldMat; 
	XMMATRIX viewMat;
	XMMATRIX projectionMat;


public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
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

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)
			D3D11_INPUT_ELEMENT_DESC vLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			UINT numberOfElements = ARRAYSIZE(vLayout); 
			myDevice->CreateInputLayout(vLayout, numberOfElements, VS_shader, sizeof(VS_shader), &inputLayout);

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


			////USE THIS FOR SKYBOX INDEX
			//WORD indices[] =
			//{
			//	3,1,0,
			//	2,1,3,

			//	6,4,5,
			//	7,4,6,

			//	11,9,8,
			//	10,9,11,

			//	14,12,13,
			//	15,12,14,

			//	19,17,16,
			//	18,17,19,

			//	22,20,21,
			//	23,20,22
			//};

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 36;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = indices;

			myDevice->CreateBuffer(&bDesc, &subData, &indexBuffer);

			// Create Constant Buffer
			D3D11_BUFFER_DESC constantBufferDesc = {};
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
			constantBufferDesc.CPUAccessFlags = 0;


			myDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);

			//write and compile & load our shaders
			myDevice->CreatePixelShader(&PS_shader, sizeof(PS_shader), nullptr, &myPixelShader);

			myDevice->CreateVertexShader(&VS_shader, sizeof(VS_shader), nullptr, &myVertexShader);

			worldMat = XMMatrixIdentity();

			//init View Matrix
			XMVECTOR eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
			XMVECTOR at = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewMat = XMMatrixLookAtLH(eye, at, up);

			//init Projection Matrix

			float aspectR;
			
			projectionMat = XMMatrixPerspectiveFovLH(XMConvertToRadians(65), mySurface->GetAspectRatio(aspectR), 0.01f, 100.0f);

			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	vertexBuffer->Release();
	indexBuffer->Release();
	constantBuffer->Release();
	myVertexShader->Release(); 
	myPixelShader->Release(); 

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

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			// Clear the screen to dark green
			const float d_green[] = { 0, 0.5f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);
			
			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!

			myContext->IASetInputLayout(inputLayout);

			UINT stride = sizeof(myVertex);
			UINT offset = 0;

			ID3D11Buffer *tempBuffer[] = { {vertexBuffer } };


			///////////////LIGHTS////////////////////
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


			//COPY PASE THIS BLOCK OF CODE FOR DRAWIN, JUST REMEMBER TO CHANGE VERTEX AND PIXEL SHADERS.
			ConstantBuffer constBuffer;
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

			//Updates LIGHTS
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


			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}