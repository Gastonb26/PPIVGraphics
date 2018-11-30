// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>

#include "myPshader.csh"
#include "myVshader.csh"

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;
	
	struct myVertex
	{
		float xyzw[4];
		float rgba[4];
	};

	ID3D11Buffer * vBuffer; 
	ID3D11InputLayout* vLayout;
	ID3D11VertexShader* vShader; //hlsl
	ID3D11PixelShader* pShader; //hlsl



	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)

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

			myVertex tri[] 
			{ //xyzq, rgba
				{{0,0.5f,0,1},{1,1,1,1} },
				{{0.5f, -0.5f, 0, 1}, {1,1,1,1} },
				{{-0.5, -0.5f, 0, 1}, {1,1,1,1} }
			};

			//load it to the card
			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData; 
			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));

			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
			bDesc.ByteWidth = sizeof(myVertex) * 3;
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
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			myDevice->CreateInputLayout(ieDesc, 2, myVshader, sizeof(myVshader), &vLayout); 

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
			

			myContext->IASetInputLayout(vLayout);

			UINT strides[] = { sizeof(myVertex) };
			UINT offsets[] = { 0 }; 
			ID3D11Buffer *tempVB[] = { vBuffer };

			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!
			myContext->VSSetShader(vShader, 0, 0);
			myContext->PSSetShader(pShader, 0, 0); 

			myContext->Draw(3, 0); 

			// try to make triagle 3d 
				//1. make into a pyrimid(more verts) 
				
				//make a world view & projection matrix

				// upload those matrices to the video card
					//create and update a constant buffer (move variables from c++ to shaders)

				//apply matrix math in vertex Shader



			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(1, 0); // set first argument to 1 to enable vertical refresh sync with display **/GasReminder originally set to 0, setting to 1 will prevent tears, but reduce performance and speed. 

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}