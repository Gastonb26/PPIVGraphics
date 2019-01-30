// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "DDSTextureLoader.h"

#include "PS_shader.csh"
#include "VS_shader.csh"

#include "PS_texture.csh"
#include "PS_point.csh"

#include "PS_sky.csh"
#include "VS_sky.csh"

#include "FitMetaknight00.h"
#include "holyhand.h"
#include "Rabbit.h"
#include "Grail.h"
#include "holyGrail.h"
#include "Knight.h"

#include "cloud.h"
#include "sephiroth.h"
#include "aerith.h"


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
		float pointIntensity; // padding too 
		XMFLOAT3 spotLight;
		float spotIntensity; 
		XMFLOAT4 spotCol; 
		XMFLOAT3 spotDirection;

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

	//Might need these for later (Resizing Window, aspect Ratio adjustment and more);
	float aspectR;
	unsigned int height = 0;
	unsigned int width = 0;
	//Render Scenes
	int scene = 1; 
	bool cubesOn = false; 

	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	GW::SYSTEM::GWindow *attPoint = nullptr;	// attach point? Might need this for adjusting aspect ratio! 

	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)

	// SHADERS
	ID3D11VertexShader *myVertexShader = nullptr; 
	ID3D11VertexShader *mySkyVshader = nullptr; 

	ID3D11PixelShader *myPixelShader = nullptr; 
	ID3D11PixelShader *myTextureShader = nullptr; 
	ID3D11PixelShader *mySkyPshader = nullptr; 
	ID3D11PixelShader *myPointShader = nullptr; 
	//ID3D11PixelShader *myEmissiveShader = nullptr; 

	// BUFFERS
	ID3D11Buffer *constantBuffer = nullptr; 
				 
	ID3D11Buffer *vertexBuffer = nullptr; // cube
	ID3D11Buffer *indexBuffer = nullptr; // cube
				 
	ID3D11Buffer *skyVertexBuffer = nullptr; 
	ID3D11Buffer *skyIndexBuffer = nullptr; 
				 
	ID3D11Buffer *knightVB = nullptr; 
	ID3D11Buffer *knightIB = nullptr; 

	ID3D11Buffer *rabbitVB = nullptr; 
	ID3D11Buffer *rabbitIB = nullptr; 

	ID3D11Buffer *holyhandVB = nullptr; 
	ID3D11Buffer *holyhandIB = nullptr; 

	ID3D11Buffer *grailVB = nullptr; 
	ID3D11Buffer *grailIB = nullptr; 

	ID3D11Buffer *metaVB = nullptr;
	ID3D11Buffer *metaIB = nullptr;

	ID3D11Buffer *cloudVB = nullptr; 
	ID3D11Buffer *cloudIB = nullptr;

	ID3D11Buffer *sephirothVB = nullptr; 
	ID3D11Buffer *sephirothIB = nullptr; 
	
	ID3D11Buffer *aerithVB = nullptr;
	ID3D11Buffer *aerithIB = nullptr;

	// MATRICES
	XMMATRIX worldMat; 
	XMMATRIX viewMat;
	XMMATRIX projectionMat;

	XMMATRIX scale;
	XMMATRIX trans;
	XMMATRIX rot;
	
	XMMATRIX skyMat = XMMatrixTranslation(0.0f, 10.0f, 5.0f);
	XMMATRIX spotMat = XMMatrixTranslation(10.0f, 1.0f, 30.0f);
	XMMATRIX pointMat = XMMatrixTranslation(0.0f, 0.0f, -5.0f); 
	XMMATRIX grailMat = XMMatrixIdentity(); 

	XMMATRIX metaMat = XMMatrixTranslation(0.0f, 4.0f, 0.0f); 
	
	// RENDER TO TEXTURE CUBE
	XMMATRIX renderCube = XMMatrixTranslation(0.0f, 0.0f, 25.0f);
	XMMATRIX renderCubeIn = XMMatrixTranslation(0.0f, 0.0f, 30.0f);
		
	// POSTIONS
	XMFLOAT3 pointPos = XMFLOAT3(0.0f, 0.0f, -5.0f);
	XMFLOAT3 spotPos = XMFLOAT3(10.0f, 1.0f, 30.0f);
	XMFLOAT3 spotDir = XMFLOAT3(0.0f, -2.0f, -10.0f);

	// TEXTURES
	ID3D11ShaderResourceView *textureResource = nullptr; //simple fake looking metal (used for meta knight
	ID3D11ShaderResourceView *knightTex = nullptr; 
	ID3D11ShaderResourceView *rabbitTex = nullptr; 
	ID3D11ShaderResourceView *goldTex = nullptr; 

	ID3D11ShaderResourceView *skyResource = nullptr; 
	ID3D11ShaderResourceView *skyCastleResource = nullptr; 
	//ID3D11ShaderResourceView* emissiveTex = nullptr; 
	
	// TARFET RENDER TEXTURES
	ID3D11ShaderResourceView *rttTexture = nullptr;
	ID3D11Texture2D *rttMap = nullptr;
	ID3D11Texture2D *depthBuffer = nullptr; 
	
	// MISC.
	ID3D11InputLayout *inputLayout = nullptr; 
	ID3D11SamplerState *samplerState = nullptr; 
	ID3D11DepthStencilView *depthStencil = nullptr; 
	D3D_DRIVER_TYPE DDT = D3D_DRIVER_TYPE_NULL; 

	// RENDER TO TEXTURE VARIABLES
	ID3D11RenderTargetView *renderTargetViewMap = nullptr;
	ID3D11RasterizerState *cwCullMode = nullptr;

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
	// Contols
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
			
			//attach point might be needed for resizing window
			attPoint = attatchPoint; 
			attPoint->GetClientHeight(height); 
			attPoint->GetClientWidth(width); 
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
			///////////////////////////////////SECTION INIT 2: Models ////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////
			
			//Buffer & Subresource descriptors for models 
			D3D11_BUFFER_DESC bDesc = {};
			D3D11_SUBRESOURCE_DATA subData;
			
			////////////////////////////////////// BASIC CUBE //////////////////////////////////////
			myVertex cube[]
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

			subData.pSysMem = cube;

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

			///////////////////////////////// SKYBOX //////////////////////////////////////
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

			//////////////////////////////// KNIGHT //////////////////////////////////////

			myVertex* tempKnight = new myVertex[1211];
			for (int i = 0; i < 1211; i++)
			{							 
				tempKnight[i].Position.x = knight_data[i].pos[0];
				tempKnight[i].Position.y = knight_data[i].pos[1];
				tempKnight[i].Position.z = knight_data[i].pos[2];
					
				tempKnight[i].Normal.x = knight_data[i].nrm[0];
				tempKnight[i].Normal.y = knight_data[i].nrm[1];
				tempKnight[i].Normal.z = knight_data[i].nrm[2];
					
				tempKnight[i].Tex.x = knight_data[i].uvw[0];
				tempKnight[i].Tex.y = knight_data[i].uvw[1];
			}

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 1211;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = tempKnight;

			myDevice->CreateBuffer(&bDesc, &subData, &knightVB);

			WORD* tempKnightI = new WORD[2922];
			for (int i = 0; i < 2922; i++)
			{
				tempKnightI[i] = knight_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 2922;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = tempKnightI;

			myDevice->CreateBuffer(&bDesc, &subData, &knightIB);

			//////////////////////////////// RABBIT //////////////////////////////////////

			myVertex* tempRabbitV = new myVertex[1172];
			for (int i = 0; i < 1172; i++)
			{
				tempRabbitV[i].Position.x = Rabbit_data[i].pos[0];
				tempRabbitV[i].Position.y = Rabbit_data[i].pos[1];
				tempRabbitV[i].Position.z = Rabbit_data[i].pos[2];
				
				tempRabbitV[i].Normal.x = Rabbit_data[i].nrm[0];
				tempRabbitV[i].Normal.y = Rabbit_data[i].nrm[1];
				tempRabbitV[i].Normal.z = Rabbit_data[i].nrm[2];
				
				tempRabbitV[i].Tex.x = Rabbit_data[i].uvw[0];
				tempRabbitV[i].Tex.y = Rabbit_data[i].uvw[1];
			}

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 1172;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = tempRabbitV;

			myDevice->CreateBuffer(&bDesc, &subData, &rabbitVB);

			WORD* tempRabbitI = new WORD[3420];
			for (int i = 0; i < 3420; i++)
			{
				tempRabbitI[i] = Rabbit_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 3420;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = tempRabbitI;

			myDevice->CreateBuffer(&bDesc, &subData, &rabbitIB);

			//////////////////////////////// HOLY HAND //////////////////////////////////////

			myVertex* tempholyhandV = new myVertex[1255];
			for (int i = 0; i < 1255; i++)
			{
				tempholyhandV[i].Position.x = holyhand_data[i].pos[0];
				tempholyhandV[i].Position.y = holyhand_data[i].pos[1];
				tempholyhandV[i].Position.z = holyhand_data[i].pos[2];
				
				tempholyhandV[i].Normal.x = holyhand_data[i].nrm[0];
				tempholyhandV[i].Normal.y = holyhand_data[i].nrm[1];
				tempholyhandV[i].Normal.z = holyhand_data[i].nrm[2];
				
				tempholyhandV[i].Tex.x = holyhand_data[i].uvw[0];
				tempholyhandV[i].Tex.y = holyhand_data[i].uvw[1];
			}

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 1255;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = tempholyhandV;

			myDevice->CreateBuffer(&bDesc, &subData, &holyhandVB);

			WORD* tempholyhandI = new WORD[4032];
			for (int i = 0; i < 4032; i++)
			{
				tempholyhandI[i] = holyhand_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 4032;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = tempholyhandI;

			myDevice->CreateBuffer(&bDesc, &subData, &holyhandIB);

			//////////////////////////////// THE GRAIL //////////////////////////////////////

			myVertex* tempgrailV = new myVertex[3476];
			for (int i = 0; i < 3476; i++)
			{
				tempgrailV[i].Position.x = grail_data[i].pos[0];
				tempgrailV[i].Position.y = grail_data[i].pos[1];
				tempgrailV[i].Position.z = grail_data[i].pos[2];
				
				tempgrailV[i].Normal.x = grail_data[i].nrm[0];
				tempgrailV[i].Normal.y = grail_data[i].nrm[1];
				tempgrailV[i].Normal.z = grail_data[i].nrm[2];
				
				tempgrailV[i].Tex.x = grail_data[i].uvw[0];
				tempgrailV[i].Tex.y = grail_data[i].uvw[1];
			}

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 3476;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = tempgrailV;

			myDevice->CreateBuffer(&bDesc, &subData, &grailVB);

			WORD* tempgrailI = new WORD[11382];
			for (int i = 0; i < 11382; i++)
			{
				tempgrailI[i] = grail_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 11382;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = tempgrailI;

			myDevice->CreateBuffer(&bDesc, &subData, &grailIB);
			
			
			//////////////////////////////// META KNIGHT //////////////////////////////////////
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

			myDevice->CreateBuffer(&bDesc, &subData, &metaVB);

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

			myDevice->CreateBuffer(&bDesc, &subData, &metaIB);

			//////////////////////////////// Cloud //////////////////////////////////////
			myVertex* tempcloudV = new myVertex[7649];
			for (int i = 0; i < 7649; i++)
			{							  
				tempcloudV[i].Position.x = cloud_data[i].pos[0];
				tempcloudV[i].Position.y = cloud_data[i].pos[1];
				tempcloudV[i].Position.z = cloud_data[i].pos[2];
										
				tempcloudV[i].Normal.x = cloud_data[i].nrm[0];
				tempcloudV[i].Normal.y = cloud_data[i].nrm[1];
				tempcloudV[i].Normal.z = cloud_data[i].nrm[2];

				tempcloudV[i].Tex.x = cloud_data[i].uvw[0];
				tempcloudV[i].Tex.y = cloud_data[i].uvw[1];
			}

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 7649;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = tempcloudV;

			myDevice->CreateBuffer(&bDesc, &subData, &cloudVB);

			WORD* tempcloudI = new WORD[8553];
			for (int i = 0; i < 8553; i++)
			{
				tempcloudI[i] = cloud_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 8553;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = tempcloudI;

			myDevice->CreateBuffer(&bDesc, &subData, &cloudIB);

			//////////////////////////////// Sephiroth  /////////////////////////////////

			myVertex* sephirothV = new myVertex[6224];
			for (int i = 0; i < 6224; i++)
			{
				sephirothV[i].Position.x = sephiroth_data[i].pos[0];
				sephirothV[i].Position.y = sephiroth_data[i].pos[1];
				sephirothV[i].Position.z = sephiroth_data[i].pos[2];
				
				sephirothV[i].Normal.x = sephiroth_data[i].nrm[0];
				sephirothV[i].Normal.y = sephiroth_data[i].nrm[1];
				sephirothV[i].Normal.z = sephiroth_data[i].nrm[2];
										
				sephirothV[i].Tex.x = sephiroth_data[i].uvw[0];
				sephirothV[i].Tex.y = sephiroth_data[i].uvw[1];
			}						

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 6224;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = sephirothV;

			myDevice->CreateBuffer(&bDesc, &subData, &sephirothVB);

			WORD* sephirothI = new WORD[7128];
			for (int i = 0; i < 7128; i++)
			{
				sephirothI[i] = sephiroth_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 7128;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = sephirothI;

			myDevice->CreateBuffer(&bDesc, &subData, &sephirothIB);

			//////////////////////////////// aerith /////////////////////////////////

			myVertex* aV = new myVertex[6665];
			for (int i = 0; i < 6665; i++)
			{					  
				aV[i].Position.x = aerith_data[i].pos[0];
				aV[i].Position.y = aerith_data[i].pos[1];
				aV[i].Position.z = aerith_data[i].pos[2];
				 
				aV[i].Normal.x = aerith_data[i].nrm[0];
				aV[i].Normal.y = aerith_data[i].nrm[1];
				aV[i].Normal.z = aerith_data[i].nrm[2];
				
				aV[i].Tex.x = aerith_data[i].uvw[0];
				aV[i].Tex.y = aerith_data[i].uvw[1];
			}

			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(myVertex) * 6665;
			bDesc.CPUAccessFlags = 0;

			subData.pSysMem = aV;

			myDevice->CreateBuffer(&bDesc, &subData, &aerithVB);

			WORD* aI = new WORD[7569];
			for (int i = 0; i < 7569; i++)
			{
				aI[i] = aerith_indicies[i];
			}
			bDesc.Usage = D3D11_USAGE_DEFAULT;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bDesc.ByteWidth = sizeof(WORD) * 7569;
			bDesc.CPUAccessFlags = 0;
			subData.pSysMem = aI;

			myDevice->CreateBuffer(&bDesc, &subData, &aerithIB);



			//////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////SECTION INIT 3: Buffers, Shaders, Textures, Camera /////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////

			/////////////////// Create Constant Buffer(s) // you can use bDesc instead? 
			D3D11_BUFFER_DESC constantBufferDesc = {};
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
			constantBufferDesc.CPUAccessFlags = 0;

			myDevice->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);


			/////////////////// Write, compile & load our shaders
			myDevice->CreatePixelShader(&PS_shader, sizeof(PS_shader), nullptr, &myPixelShader);
			myDevice->CreatePixelShader(&PS_texture, sizeof(PS_texture), nullptr, &myTextureShader);
			myDevice->CreateVertexShader(&VS_shader, sizeof(VS_shader), nullptr, &myVertexShader);
			myDevice->CreatePixelShader(&PS_sky, sizeof(PS_sky), nullptr, &mySkyPshader); 
			myDevice->CreateVertexShader(&VS_sky, sizeof(VS_sky), nullptr, &mySkyVshader);
			myDevice->CreatePixelShader(&PS_point, sizeof(PS_point), nullptr, &myPointShader); 

			/////////////////// Create Sampler, & Load Textures
			D3D11_SAMPLER_DESC sDesc = {}; 

			CreateDDSTextureFromFile(myDevice, L"blackarmor.dds", nullptr, &knightTex);
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;

			CreateDDSTextureFromFile(myDevice, L"fur.dds", nullptr, &rabbitTex);
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;

			CreateDDSTextureFromFile(myDevice, L"gold.dds", nullptr, &goldTex);
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;

			CreateDDSTextureFromFile(myDevice, L"metaTexture.dds", nullptr, &textureResource);
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;
			
			CreateDDSTextureFromFile(myDevice, L"ff7.dds", nullptr, &skyResource); 
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;

			CreateDDSTextureFromFile(myDevice, L"SkyboxCastle2.dds", nullptr, &skyCastleResource);
			sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sDesc.MinLOD = 0;
			sDesc.MaxLOD = D3D11_FLOAT32_MAX;

			myDevice->CreateSamplerState(&sDesc, &samplerState);

			/////////////////////////////// Create Texture Desc, Render Target View & Shader Resource View (for render to texture)
			
			
			D3D11_TEXTURE2D_DESC tDesc;
			ZeroMemory(&tDesc, sizeof(tDesc));
			tDesc.Width = width;
			tDesc.Height = height;
			tDesc.MipLevels = 1;
			tDesc.ArraySize = 1;
			tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			tDesc.SampleDesc.Count = 1;
			tDesc.Usage = D3D11_USAGE_DEFAULT;
			tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			tDesc.CPUAccessFlags = 0;
			tDesc.MiscFlags = 0;

			myDevice->CreateTexture2D(&tDesc, NULL, &rttMap); 
			
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			rtvDesc.Format = tDesc.Format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
		
			myDevice->CreateRenderTargetView(rttMap, &rtvDesc, &renderTargetViewMap);
			
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc; 		
			srvDesc.Format = tDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			
			myDevice->CreateShaderResourceView(rttMap, &srvDesc, &rttTexture); 
			
			D3D11_RASTERIZER_DESC rasterDesc;
			ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.CullMode = D3D11_CULL_BACK;		
			rasterDesc.FrontCounterClockwise = false;
			
			//myDevice->CreateRasterizerState(&rasterDesc, &cwCullMode); 			
			myDevice->CreateTexture2D(&tDesc, NULL, &depthBuffer);  

			//////////////////// Init Camera
			worldMat = XMMatrixIdentity();
			//init View Matrix
			camera.eye = XMVectorSet(0.0f, -2.5f, -30.0f, 0.0f);
			camera.at = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
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
	
	//SHADERS
	myVertexShader->Release(); 
	mySkyVshader->Release(); 
	myPixelShader->Release(); 
	mySkyPshader->Release(); 
	myTextureShader->Release(); 
	myPointShader->Release(); 

	//BUFFERS
	constantBuffer->Release();
	depthBuffer->Release(); 

	vertexBuffer->Release();
	indexBuffer->Release();
	skyVertexBuffer->Release(); 
	skyIndexBuffer->Release(); 
	metaVB->Release(); 
	metaIB->Release(); 
	knightVB->Release(); 
	knightIB->Release(); 
	rabbitVB->Release(); 
	rabbitIB->Release(); 
	holyhandVB->Release(); 
	holyhandIB->Release(); 
	grailVB->Release(); 
	grailIB->Release(); 
	cloudVB->Release(); 
	cloudIB->Release(); 
	sephirothVB->Release(); 
	sephirothIB->Release(); 
	aerithIB->Release(); 
	aerithVB->Release(); 

	//TEXTURES 
	textureResource->Release(); //fake metal tex 
	knightTex->Release(); 
	rabbitTex->Release(); 
	goldTex->Release(); 

	skyResource->Release(); 
	skyCastleResource->Release(); 
	
	rttTexture->Release(); // render to target
	rttMap->Release();  // render to target

	//MISC
	samplerState->Release();
	depthStencil->Release(); 
	renderTargetViewMap->Release(); 
	cwCullMode->Release(); 
	

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
			

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			myDevice->CreateDepthStencilView(depthBuffer, &dsvDesc, &depthStencil);

			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!

			myContext->IASetInputLayout(inputLayout);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			myContext->PSSetSamplers(0, 1, &samplerState);

			UINT stride = sizeof(myVertex);
			UINT offset = 0;
												//0			  //1		  //2		   //3			//4		  //5			//6				//7			//8				//9
			ID3D11Buffer *tempBuffer[] = { {vertexBuffer}, {knightVB}, {rabbitVB}, {holyhandVB}, {grailVB}, {metaVB}, {skyVertexBuffer}, {cloudVB}, {sephirothVB}, {aerithVB} };

			//Camera Movement & Controls
			MoveCamera(); 


			//TODO: look into using a better timer ( xtime ) 
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


			//////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////SECTION RENDER: Models/////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////
			
			//HINT: use rot , trans, scale for model transformations (create unique matrix for special purposes)

			ConstantBuffer constBuffer;
			
			constBuffer.pointLight = pointPos;
			constBuffer.spotLight = spotPos;
			constBuffer.spotDirection = spotDir;

			if (scene == 1) // RENDER MONTY PYTHON THEME
			{


				constBuffer.pointIntensity = 5.0f;
				constBuffer.spotIntensity = 1.0f;
				constBuffer.vOutputCol = XMFLOAT4(.95f, .90f, .250f, 1.0f); 
				constBuffer.spotCol = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				/////////////////////////////////////KNIGHT
				scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
				trans = XMMatrixTranslation(2.0f, -1.0f, 0.0f);
				rot = XMMatrixRotationY(4.0f);

				constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * rot * trans * scale );
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[1], &stride, &offset);
				myContext->IASetIndexBuffer(knightIB, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShaderResources(0, 1, &knightTex);

				myContext->PSSetShader(myPointShader , 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(2922, 0, 0);

				/////////////////////////////////////////RABBIT
				scale = XMMatrixScaling(.1f, .1f, .1f);
				trans = XMMatrixTranslation(-10.0f, -5.0f, 3.0f);
				rot = XMMatrixRotationY(3.0f);

				constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * rot * trans);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[2], &stride, &offset);
				myContext->IASetIndexBuffer(rabbitIB, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShaderResources(0, 1, &rabbitTex);

				myContext->PSSetShader(myTextureShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(3420, 0, 0);


				///////////////////////////////////////////HOLY GRAIL

				scale = XMMatrixScaling(25.0f, 25.0f, 25.0f);
				rot = XMMatrixRotationX(0.25f);
				grailMat = XMMatrixRotationY(t);

				constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * rot * grailMat);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[4], &stride, &offset);
				myContext->IASetIndexBuffer(grailIB, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShaderResources(0, 1, &goldTex);

				myContext->PSSetShader(myPointShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(824091, 0, 0);
				
				///////////////////////// Floor
			
				scale = XMMatrixScaling(25.0f, 0.1f, 25.0f);
				trans = XMMatrixTranslation(0.0f, -5.0f, 0.0f);

				constBuffer.mWorld = XMMatrixTranspose(worldMat * scale * trans);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);
				constBuffer.spotIntensity = 30.0f; 
				constBuffer.pointIntensity = 3.0f; 
				constBuffer.vOutputCol = XMFLOAT4(.95f, .90f, .250f, 1.0f);
				constBuffer.spotCol = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				
				myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
				myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShaderResources(0, 1, &textureResource);
				
				myContext->PSSetShader(myPointShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(36, 0, 0);

			}
			else // RENDER CUSTOM THEME (Final Fantasy VII) 
			{

				// set color and intensity for models 
				
				constBuffer.pointIntensity = 5.0f;
				constBuffer.spotIntensity = 20.0f;
				constBuffer.vOutputCol = XMFLOAT4(.35f, .92f, .49f, 1.0f); // 90, 232, 125
				constBuffer.spotCol = XMFLOAT4(.90f, .27f, .26f, 1.0f); // 232, 70, 67)

				/////////////////////// CLOUD 
				scale = XMMatrixScaling(.05f, .05f, .05f);
				rot = XMMatrixRotationX(XMConvertToRadians(180.0f));
				trans = XMMatrixTranslation(-15.0f, -5.0f, 0.0f);

				constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * XMMatrixRotationY(XMConvertToRadians(90.0f)) * rot * trans);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[7], &stride, &offset);
				myContext->IASetIndexBuffer(cloudIB, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShaderResources(0, 1, &goldTex);

				myContext->PSSetShader(myPointShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(8553, 0, 0);


				////////////////////////////// Sephiroth
				scale = XMMatrixScaling(.05f, .05f, .05f);
				rot = XMMatrixRotationX(XMConvertToRadians(180.0f));
				trans = XMMatrixTranslation(15.0f, -5.0f, 0.0f); 

				constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * XMMatrixRotationY(XMConvertToRadians(-90.0f)) * rot * trans );
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[8], &stride, &offset);
				myContext->IASetIndexBuffer(sephirothIB, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShaderResources(0, 1, &goldTex);

				myContext->PSSetShader(myPointShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(7128, 0, 0);

				/////////////////////////// Floor

				scale = XMMatrixScaling(25.0f, 0.1f, 25.0f);
				trans = XMMatrixTranslation(0.0f, -5.0f, 0.0f);

				constBuffer.mWorld = XMMatrixTranspose(worldMat * scale * trans);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);
				constBuffer.pointIntensity = 15.0f;
				constBuffer.spotIntensity = 40.0f;
				//constBuffer.vOutputCol = XMFLOAT4(.35f, .92f, .49f, 1.0f); // 90, 232, 125
				//constBuffer.spotCol = XMFLOAT4(.90f, .27f, .26f, 1.0f); // 232, 70, 67)

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
				myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShaderResources(0, 1, &textureResource);

				myContext->PSSetShader(myPointShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(36, 0, 0);

			}


			////////////////////////BOTH SCENES WILL USE CUBES TO INDICATE LIGHT LOCATIONS AND SKYBOXES
			if (cubesOn)
			{
				////////////////////////// Spot Light Cube
				scale = XMMatrixScaling(.5f, .5f, .5f);

				constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * spotMat);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);
				myContext->PSSetShaderResources(0, 1, &textureResource);
				myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
				myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
				myContext->PSSetShader(myPointShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);
				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(36, 0, 0);

				////////////////////////// Point Light Cube
				scale = XMMatrixScaling(.5f, .5f, .5f);
				constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * pointMat);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);
				myContext->PSSetShaderResources(0, 1, &textureResource);
				myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
				myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
				myContext->PSSetShader(myPointShader, 0, 0);
				myContext->VSSetShader(myVertexShader, 0, 0);
				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(36, 0, 0);
			}
			
			////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////// SECTION: SKYBOX /////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////


			switch (scene)
			{
			case 1: 
				myContext->PSSetShaderResources(0, 1, &skyCastleResource);
				trans = XMMatrixTranslation(0.0f, 20.0f, 0.0f);
				scale = XMMatrixScaling(105.0f, 105.0f, 105.0f);
				break;
			case 2: 
				myContext->PSSetShaderResources(0, 1, &skyResource);
				trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
				scale = XMMatrixScaling(105.0f, 105.0f, 105.0f);
				break; 
			default:
				break;
			}

			constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * skyMat * trans);
			constBuffer.mView = XMMatrixTranspose(viewMat);
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);
			//constBuffer.vOutputCol = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
			myContext->IASetVertexBuffers(0, 1, &tempBuffer[6], &stride, &offset);
			myContext->IASetIndexBuffer(skyIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			myContext->PSSetShader(mySkyPshader, 0, 0);			
			myContext->VSSetShader(mySkyVshader, 0, 0);
			myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->PSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
			myContext->DrawIndexed(36, 0, 0);

			////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////// RENDER TEXTURE TO CUBE ///////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////// SET UP AMBIENT LIGHTS FOR RENDER TO TEXTURE 

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
			constBuffer.vLightDir[0] = vLightDirs[0];
			constBuffer.vLightDir[1] = vLightDirs[1];
			constBuffer.vLightCol[0] = vLightColors[0];
			constBuffer.vLightCol[1] = vLightColors[1];
			constBuffer.vOutputCol = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
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

			}
			////////////////////////////////////////////////////////////////////////////////

			myContext->OMSetRenderTargets(1, &renderTargetViewMap, myDepthStencilView);
			if (scene == 1)
			{
				myContext->ClearRenderTargetView(renderTargetViewMap, DirectX::Colors::BlanchedAlmond);
			}
			else
			{
				myContext->ClearRenderTargetView(renderTargetViewMap, DirectX::Colors::MediumSeaGreen);
			}

			////////////////////////What will be textured onto the surface goes below 

			if (scene == 1)
			{

				///////////////////////////////////////////HOLY HAND GRENADE

				scale = XMMatrixScaling(4.0f, 4.0f,4.0f);

				constBuffer.mWorld = XMMatrixTranspose(XMMatrixRotationX(XMConvertToRadians(180.0f)) * worldMat * XMMatrixRotationY(t) * scale * renderCubeIn);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);

				constBuffer.vOutputCol = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				myContext->PSSetShaderResources(0, 1, &textureResource);

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[3], &stride, &offset);
				myContext->IASetIndexBuffer(holyhandIB, DXGI_FORMAT_R16_UINT, 0);

				//myContext->VSSetShader(myVertexShader, 0, 0);
				myContext->PSSetShader(myPixelShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->PSSetSamplers(0, 1, &samplerState);
				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(4032, 0, 0);

				//scale = XMMatrixScaling(1 / 5.0f, 1 / 5.0f, 1 / 5.0f);
				//rot = XMMatrixRotationZ(60.0f);
				//constBuffer.mWorld = XMMatrixTranspose(worldMat * XMMatrixRotationY(t) * scale * renderCubeIn * rot);
				//constBuffer.mView = XMMatrixTranspose(viewMat);
				//constBuffer.mProjection = XMMatrixTranspose(projectionMat);

				//constBuffer.vOutputCol = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				//myContext->PSSetShaderResources(0, 1, &textureResource);

				//myContext->IASetVertexBuffers(0, 1, &tempBuffer[5], &stride, &offset);
				//myContext->IASetIndexBuffer(metaIB, DXGI_FORMAT_R16_UINT, 0);

				////myContext->VSSetShader(myVertexShader, 0, 0);
				//myContext->PSSetShader(myPixelShader, 0, 0);

				//myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				//myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				//myContext->PSSetSamplers(0, 1, &samplerState);
				//myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				//myContext->DrawIndexed(22242, 0, 0);
			}
			else
			{
				//scale = XMMatrixScaling(.05f, .05f, .05f);
				//rot = XMMatrixRotationX(XMConvertToRadians(180.0f));
				//trans = XMMatrixTranslation(-15.0f, -5.0f, 0.0f);
				//constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity() * scale * XMMatrixRotationY(XMConvertToRadians(90.0f)) * rot * trans);

				scale = XMMatrixScaling(.05f, .05f, .05f);
				trans = XMMatrixTranslation(0.0, 10.0, 40.0f); 
				///rot = XMMatrixRotationX(XMConvertToRadians(180.0f));


				constBuffer.mWorld = XMMatrixTranspose(worldMat * XMMatrixRotationY(t) * scale * trans);
				constBuffer.mView = XMMatrixTranspose(viewMat);
				constBuffer.mProjection = XMMatrixTranspose(projectionMat);
				
				constBuffer.vOutputCol = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

				myContext->PSSetShaderResources(0, 1, &textureResource);

				myContext->IASetVertexBuffers(0, 1, &tempBuffer[9], &stride, &offset);
				myContext->IASetIndexBuffer(aerithIB, DXGI_FORMAT_R16_UINT, 0);

				myContext->PSSetShader(myPixelShader, 0, 0);

				myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
				myContext->PSSetConstantBuffers(0, 1, &constantBuffer);

				myContext->PSSetSamplers(0, 1, &samplerState);
				myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);
				myContext->DrawIndexed(7569, 0, 0);
			}
	
			///////////////////////////////////////////////Surface that will have textures renders ONTO
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);
			scale = XMMatrixScaling(5.0f, 5.0, 0.0f);
			constBuffer.mWorld = XMMatrixTranspose(XMMatrixIdentity()* scale * renderCube);
			constBuffer.mView = XMMatrixTranspose(viewMat);
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);
		
			myContext->PSSetShaderResources(0, 1, &rttTexture);
			myContext->IASetVertexBuffers(0, 1, &tempBuffer[0], &stride, &offset);
			myContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
			myContext->PSSetShader(myTextureShader, 0, 0);
			myContext->UpdateSubresource(constantBuffer, 0, NULL, &constBuffer, 0, 0);
			myContext->PSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->VSSetConstantBuffers(0, 1, &constantBuffer);
			myContext->PSSetSamplers(0, 1, &samplerState);
			myContext->DrawIndexed(36, 0, 0);

	

			//////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////SECTION: SwapChain & more ///////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////// UPDATE CAMERA //////////////////////////////////// 
			constBuffer.mView = XMMatrixTranspose(viewMat); 
			constBuffer.mProjection = XMMatrixTranspose(projectionMat);
			myContext->UpdateSubresource(constantBuffer, 0, nullptr, &constBuffer, 0, 0);

			/////////////////////////////// UPDATE SWAP CHAIN////////////////////////////////

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(1, 0); // set first argument to 1 to enable vertical refresh sync with display
			
			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();

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
		camera.posZ -= camera.speed *.5f;
	}
	else if (GetAsyncKeyState('S'))
	{
		camera.posZ += camera.speed*.5f;
	}
	if (GetAsyncKeyState('D'))
	{
		camera.posX -= camera.speed *.5f;
	}
	else if (GetAsyncKeyState('A'))
	{
		camera.posX += camera.speed*.5f;
	}

	//Roll, Pitch, Yaw
	if (GetAsyncKeyState('Q'))
	{
		camera.roll -= camera.speed*.05f;
	}
	else if (GetAsyncKeyState('E'))
	{
		camera.roll += camera.speed*.05f;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		camera.pitch += camera.speed*.05f;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		camera.pitch -= camera.speed*.05f;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		camera.yaw += camera.speed*.05f;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		camera.yaw -= camera.speed*.05f;
	}

	//////////  Controls for Lights ( 1: Point, 2: Spot, etc.)
	if (GetAsyncKeyState('8'))
	{
		camera.lightControl = 1; 
	}
	if (GetAsyncKeyState('9'))
	{
		camera.lightControl = 2; 
	}

	if (GetAsyncKeyState('I'))
	{
		switch (camera.lightControl)
		{
		case 1: 
			pointPos.z += 0.5f * camera.speed;
			pointMat *= XMMatrixTranslation(0.0f, 0.0f, 0.5f * camera.speed); 
			break; 
		case 2: 
			spotPos.z += 0.5f  * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, 0.0f, 0.5f * camera.speed);
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
			pointPos.z -= 0.5f* camera.speed;
			pointMat *= XMMatrixTranslation(0.0f, 0.0f, -0.5f *camera.speed);
			break;
		case 2:
			spotPos.z -= 0.5f * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, 0.0f, -0.5f *camera.speed);
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
			pointPos.x -= 0.5f* camera.speed;
			pointMat *= XMMatrixTranslation(-0.5f * camera.speed, 0.0f, 0.0f);
			break;
		case 2:
			spotPos.x -= 0.5f * camera.speed;
			spotMat *= XMMatrixTranslation(-0.5f * camera.speed, 0.0f, 0.0f);
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
			pointPos.x += 0.5f * camera.speed;
			pointMat *= XMMatrixTranslation(0.5f * camera.speed, 0.0f, 0.0f);
			break;
		case 2:
			spotPos.x += 0.5f  * camera.speed;
			spotMat *= XMMatrixTranslation(0.5f * camera.speed, 0.0f, 0.0f);
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
			pointPos.y -= 0.5f * camera.speed;
			pointMat *= XMMatrixTranslation(0.0f, -0.5f * camera.speed, 0.0f);
			break;
		case 2:
			spotPos.y -= 0.5f  * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, -0.5f * camera.speed, 0.0f);
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
			pointPos.y += 0.5f * camera.speed;
			pointMat *= XMMatrixTranslation(0.0f, 0.5f * camera.speed, 0.0f);
			break;
		case 2:
			spotPos.y += 0.5f  * camera.speed;
			spotMat *= XMMatrixTranslation(0.0f, 0.5f * camera.speed, 0.0f);
			break;
		default:
			break;
		}
	}

	//ROTATES SpotLight
	if (GetAsyncKeyState('M'))
	{
		spotDir.x += 0.5 * camera.speed; 
	}
	else if (GetAsyncKeyState('N'))
	{
		spotDir.x -= 0.5 * camera.speed;
	}
								
	//TOGGLES LIGHTS LOCATION CUBES 
	if (GetAsyncKeyState('5')) 
	{
		cubesOn = false; 
	}
	if (GetAsyncKeyState('6'))
	{
		cubesOn = true;
	}


	// Near/Far Plane and Zoom adjustments
	if (GetAsyncKeyState('Z'))
	{
		camera.nearPlane -= 0.2f; 
		if (camera.nearPlane < 0.01f)
		{
			camera.nearPlane = 0.01f;
		}
	}
	else if (GetAsyncKeyState('X'))
	{
		camera.nearPlane += 1.0f;
	}
	if (GetAsyncKeyState('C'))
	{
		camera.farPlane -=  1.0f;
	}
	else if (GetAsyncKeyState('V'))
	{
		camera.farPlane += 1.0f; 
	}

	if (GetAsyncKeyState('T'))
	{
		camera.camZoom -= 0.01f; 
		{
			camera.camZoom = -0.80f; 
		}
	}
	else if (GetAsyncKeyState('Y'))
	{
		camera.camZoom += 0.01f;
	}

	if (GetAsyncKeyState('1'))
	{
		scene = 1; //Monty Python Theme
		pointPos = XMFLOAT3(0.0f, 0.0f, -5.0f);
		pointMat = XMMatrixTranslation(0.0f, 0.0f, -5.0f);
		spotPos = XMFLOAT3(10.0f, 1.0f, 30.0f);
		spotMat = XMMatrixTranslation(10.0f, 1.0f, 30.0f);
		spotDir = XMFLOAT3(0.0f, -2.0f, -10.0f);
	}
	if (GetAsyncKeyState('2'))
	{
		scene = 2; //Custom Theme
		pointPos = XMFLOAT3(-11.0f, 3.0f, 0.0f);
		pointMat = XMMatrixTranslation(-11.0f, 3.0f, 0.0f);
		spotPos = XMFLOAT3(-14.0f, 3.0f, 25.0f);
		spotMat = XMMatrixTranslation(-14.0f, 3.0f , 25.0f);
		spotDir = XMFLOAT3(12.0f, -2.0f, -10.0f); 

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

	//Reset Values
	camera.posX = 0; 
	camera.posZ = 0;
	camera.yaw = 0; 
	camera.roll = 0; 
	camera.yaw = 0; 
	camera.pitch = 0; 
	camera.speed = 0.5f; 


}

