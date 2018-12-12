// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"
#include "Gateware Redistribution R5d\Interface\G_Math\GMatrix.h"
#include "Gateware Redistribution R5d\Interface\G_System\GInput.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include <DirectXMath.h>

#include "myPshader.csh"
#include "myVshader.csh"
#include "XTime.h"

using namespace DirectX;
using namespace GW::MATH;
using namespace GW::SYSTEM;

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;
	GInput *inputManager; 
	
	struct myVertex
	{	
		float xyzw[4];
		float Normal[3]; 
		float rgba[4];
	};

	struct ConstantBuffer
	{
		GMATRIXF mWorld;
		GMATRIXF mView; 
		GMATRIXF mProjection;
		GVECTORF vLightDir;  
		GVECTORF vLightCol; 
		GVECTORF vOutputCol; 
	};


	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	//Buffers
	ID3D11Buffer * vBuffer;
	ID3D11Buffer * iBuffer; 
	ID3D11Buffer * cBuffer; 
	//Shaders
	ID3D11VertexShader* vShader; //hlsl
	ID3D11PixelShader* pShader; //hlsl	
	//Layout
	ID3D11InputLayout* vLayout;
	//confusing matrix function pointer thing. 
	GMatrix *matrixFunctions;
	//Matrices and Vectors
	GMATRIXF myWorldMatrix;
	GMATRIXF myViewMatrix;
	GMATRIXF myProjectionMatrix;

	//GInput *inputMan; Learn how to use this. 
	float camX, camZ;
	XTime timer; 

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

			CreateGMatrix(&matrixFunctions);
			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)
			

			//Sets Up GInput Manager
			HWND wind; 
			attatchPoint->GetWindowHandle(sizeof(wind), (void**)&wind);
			CreateGInput(wind, sizeof(wind), &inputManager);

			timer.Restart(); 

			myVertex tri[]
			{ //xyzq, rgba
				{ { -0.5f,0.5,-0.5,1 } , { 1,1,1,1 } },
				{ { 0.5f,0.5,-0.5,1 } ,  { 1,1,1,1 } },
				{ { 0.5f,0.5,0.5,1 } ,   { 1,1,1,1 } },
				{ { -0.5f,0.5,0.5,1 } ,  { 1,1,1,1 } },
				{ { -0.5f,-0.5,-0.5,1 }, { 1,1,1,1 } },
				{ { 0.5f,-0.5,-0.5,1 } , { 1,1,1,1 } },
				{ { 0.5f,-0.5,0.5,1 } ,  { 1,1,1,1 } },
				{ { -0.5f,-0.5,0.5,1 } , { 1,1,1,1 } },

			};

			//load it to the card
			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData; 
			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));

			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
			bDesc.ByteWidth = sizeof(myVertex) * 8;
			bDesc.CPUAccessFlags = 0; 
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0; 
			bDesc.Usage = D3D11_USAGE_DEFAULT;

			subData.pSysMem = tri; 
			
			myDevice->CreateBuffer(&bDesc, &subData, &vBuffer); 
			
			//write and compile & load our shaders

			myDevice->CreateVertexShader(myVshader, sizeof(myVshader), nullptr, &vShader);
			myDevice->CreatePixelShader(myPshader, sizeof(myPshader), nullptr, &pShader); 
			
			//describe vertex to d3d11
			D3D11_INPUT_ELEMENT_DESC ieDesc[] = 
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			// THIS LINE GOES INTO THE CREATEINPUT layout to simplify hardcording
			//UINT numberOfElements = ARRAYSIZE(ieDesc); 

			// Create the input layout 
			myDevice->CreateInputLayout(ieDesc, 2, myVshader, sizeof(myVshader), &vLayout); 

			//Create Index Buffer
			WORD indices[] =
			{
			    3,1,0,
			    2,1,3,

			    0,5,4,
			    1,5,0,

			    3,4,7,
			    0,4,3,

			    1,6,5,
			    2,6,1,

			    2,7,6,
			    3,7,2,

			    6,4,5,
			    7,4,6,
			};

			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof( WORD) * 36;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			subData.pSysMem = indices;

			myDevice->CreateBuffer(&bDesc, &subData, &iBuffer);

			// Create Constant Buffer
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDesc.ByteWidth = sizeof(ConstantBuffer);
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			bDesc.Usage = D3D11_USAGE_DEFAULT;

			myDevice->CreateBuffer(&bDesc, nullptr, &cBuffer);


			//init World Matrix
			matrixFunctions->IdentityF(myWorldMatrix);

			//init View Matrix
			GVECTORF eye = { 0.0f, 1.0f, -5.0f, 0.0f };
			GVECTORF at = { 0.0f, 1.0f, 0.0f, 0.0f };
			GVECTORF up = { 0.0f, 1.0f, 0.0f, 0.0f };
			matrixFunctions->LookAtLHF(eye, at, up, myViewMatrix);
			
			//init Projection Matrix
			float aspectR;
			mySurface->GetAspectRatio(aspectR);
			matrixFunctions->ProjectionLHF(1.5708f, aspectR, 0.01f, 100.0f, myProjectionMatrix);
	
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

	// TODO: "Release()" more stuff here!

	vBuffer->Release(); 
	iBuffer->Release(); 
	cBuffer->Release(); 
	vShader->Release(); 
	pShader->Release(); 
	vLayout->Release(); 

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
			
			/////////////////LIGHTING//////////////////////////////
			GVECTORF lightDirection = { 0.0f, 0.0f, -1.0f, 1.0f }; 

			GVECTORF lightColor = { 0.0f, 0.0f, 1.0f, 1.0f }; 
			///////////////////////////////////////////////////////
			GVECTORF FowardBack = { 0.0f, 0.0f, 1.0f, 0.0f };
			GVECTORF LeftRight = { 1.0f, 0.0f, 0.0f, 0.0f };
			//Get Input To update camera
			if (GetAsyncKeyState('W'))
			{
				camZ += .01f;

			}
			else if (GetAsyncKeyState('S'))
			{
				camZ -= .01f;
			}
			if (GetAsyncKeyState('D'))
			{
				camX += .01f;
			}
			else if (GetAsyncKeyState('A'))
			{
				camZ -= .01f;
			}	
			//Update Camera
			GVECTORF move = { camX, 0, camZ, 1 };
			GMATRIXF trans;
			matrixFunctions->IdentityF(trans); 
			matrixFunctions->TranslatelocalF(trans, move, trans);
			GMATRIXF tempView = myViewMatrix;			
			GMATRIXF complete;
			matrixFunctions->MultiplyMatrixF(trans,tempView,complete);
			//matrixFunctions->InverseF(complete, myViewMatrix);
			myViewMatrix = complete; 
			float aspectR;
			mySurface->GetAspectRatio(aspectR);
			matrixFunctions->ProjectionLHF(1.5708f, aspectR, 0.01f, 100.0f, myProjectionMatrix);
			camX = 0; 
			camZ = 0; 
			//////////////////////////////////////////////////////////

			//SetUp the Pipeline

			myContext->IASetInputLayout(vLayout);

			//set Vertex Buffer
			UINT strides[] = { sizeof(myVertex) };
			UINT offsets[] = { 0 }; 
			ID3D11Buffer *tempVB[] = { vBuffer };
			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//set Index Buffer
			myContext->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R16_UINT, 0);

			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!
			//Vertex shader
			myContext->VSSetShader(vShader, 0, 0);
			//Pixel Shader					
			myContext->PSSetShader(pShader, 0, 0); 

			//Updates variables and constant buff
			ConstantBuffer constBuff;

			matrixFunctions->TransposeF(myWorldMatrix, constBuff.mWorld);
			matrixFunctions->TransposeF(myViewMatrix, constBuff.mView);
			matrixFunctions->TransposeF(myProjectionMatrix, constBuff.mProjection);

			constBuff.vLightDir = lightDirection; 



			myContext->UpdateSubresource(cBuffer, 0, nullptr, &constBuff, 0, 0);
			myContext->VSSetConstantBuffers(0, 1, &cBuffer);
			
			
			myContext->DrawIndexed(36, 0, 0); 
			//myContext->Draw(8, 0); 

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(1, 0); // set first argument to 1 to enable vertical refresh sync with display **/GasReminder originally set to 0, setting to 1 will prevent tears, but reduce performance and speed. 

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}