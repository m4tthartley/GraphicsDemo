#include <d3d11.h>
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx11.h"
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx10.h"
#include <d3dcompiler.h>

struct dx_state
{
	HWND hwnd;

	IDXGISwapChain *swapChain;
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	ID3D11RenderTargetView *backBuffer;

	ID3D11Buffer *vertexBuffer;
};

struct color
{
	float R;
	float G;
	float B;
	float A;
};

struct v3
{
	float x;
	float y;
	float z;
};

struct v4
{
	float x;
	float y;
	float z;
	float w;
};

union m4
{
	float data[16];
	struct
	{
		v4 vec0;
		v4 vec1;
		v4 vec2;
		v4 vec3;
	};
};

struct r_vertex
{
	v3 position;
	v3 normal;
	v4 color;
};

struct matrix_constant_buffer
{
	m4 MatProjection;
	m4 rotationMatrix;
};

ID3D11Buffer *ContantBuffer;

void DX11Init (dx_state *dx)
{
	// Create device
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = dx->hwnd;
	SwapChainDesc.SampleDesc.Count = 4;
	SwapChainDesc.Windowed = TRUE;

	//IDXGISwapChain SwapChain;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &SwapChainDesc, &dx->swapChain, &dx->device,
		NULL, &dx->context);

	// Render target
	ID3D11Texture2D *BackBufferTexture;
	dx->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferTexture);

	dx->device->CreateRenderTargetView(BackBufferTexture, NULL, &dx->backBuffer);
	BackBufferTexture->Release();

	dx->context->OMSetRenderTargets(1, &dx->backBuffer, NULL);

	// Viewport
	D3D11_VIEWPORT Viewport;
	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = 1280;
	Viewport.Height = 720;

	dx->context->RSSetViewports(1, &Viewport);

	D3DXMATRIX OrthoMatrix;
	D3DXMatrixOrthoLH(&OrthoMatrix, 1280, 720, -20, 20);


	//D3DXMatrixOrthoOffCenterRH(&OrthoMatrix, 0, 1280, 0, 720, -10, 10);

	// todo: compile shader from string
	//		 https://msdn.microsoft.com/en-us/library/windows/desktop/dd607324(v=vs.85).aspx

	// Temp
	ID3D10Blob *VertexShaderBlob;
	ID3D10Blob *PixelShaderBlob;
	HRESULT VertexHr = S_OK;
	HRESULT PixelHr = S_OK;
	ID3DBlob *VertexErrorBlob;
	ID3DBlob *PixelErrorBlob;
	VertexHr = D3DX11CompileFromFile("../code/shader.hlsl", 0, 0, "VertexasdShader",
															 "vs_4_0",
															 D3DCOMPILE_DEBUG,
															 0, 0,
															 &VertexShaderBlob,
															 &VertexErrorBlob, 0);
	PixelHr = D3DX11CompileFromFile("../code/shader.hlsl", 0, 0, "PixelasdShader",
															"ps_4_0",
															D3DCOMPILE_DEBUG,
															0, 0,
															&PixelShaderBlob,
															&PixelErrorBlob, 0);

	if (FAILED(VertexHr))
	{
		if (VertexErrorBlob != NULL)
		{
			OutputDebugString((char*)VertexErrorBlob->GetBufferPointer());
		}
	}

	ID3D11VertexShader *VertexShader;
	ID3D11PixelShader *PixelShader;
	dx->device->CreateVertexShader(VertexShaderBlob->GetBufferPointer(),
		VertexShaderBlob->GetBufferSize(),
		NULL,
		&VertexShader);
	dx->device->CreatePixelShader(PixelShaderBlob->GetBufferPointer(),
		PixelShaderBlob->GetBufferSize(),
		NULL,
		&PixelShader);

	dx->context->VSSetShader(VertexShader, 0, 0);
	dx->context->PSSetShader(PixelShader, 0, 0);

	v4 white = {1.0f, 1.0f, 1.0f, 1.0f};
	v4 yellow = {1.0f, 1.0f, 0.2f, 1.0f};
	v4 red = {1.0f, 0.2f, 0.2f, 1.0f};
	v4 blue = {0.2f, 0.2f, 1.0f, 1.0f};
	v4 purple = {1.0f, 0.2f, 1.0f, 1.0f};
	v4 green = {0.2f, 1.0f, 0.2f, 1.0f};
	v4 black = {0.0f, 0.0f, 0.0f, 1.0f};
	v4 terq = {0.2f, 1.0f, 1.0f, 1.0f};
	r_vertex Vertices[]=
	{
		// front
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, white },
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, yellow },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, red },
		
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, white },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, red },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, purple },

		// back
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, green },
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, terq },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, blue },
		
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, green },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, blue },
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, black },

		// right
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, yellow },
		{ { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, green },
		{ { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, black },
		
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, yellow },
		{ { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, black },
		{ { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, red },

		// left
		{ {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, terq },
		{ {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, white },
		{ {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, purple },
		
		{ {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, terq },
		{ {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, purple },
		{ {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, blue },

		// top
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, terq },
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, green },
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, yellow },
		
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, terq },
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, yellow },
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, white },

		// bottom
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, black },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, blue },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, purple },
		
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, black },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, purple },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, red },
	};

	ID3D11Buffer *VertexBuffer;
	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.Usage = D3D11_USAGE_DYNAMIC; // Might want to change this
	BufferDesc.ByteWidth = sizeof(Vertices); //sizeof(r_vertex) * 4;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dx->device->CreateBuffer(&BufferDesc, NULL, &VertexBuffer);

	D3D11_MAPPED_SUBRESOURCE MappedBuffer;
	dx->context->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &MappedBuffer);
	memcpy(MappedBuffer.pData, Vertices, sizeof(Vertices));
	dx->context->Unmap(VertexBuffer, NULL);

	ID3D11InputLayout *Layout;
	D3D11_INPUT_ELEMENT_DESC InputDesc[]=
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(v3), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24/*sizeof(v3)+sizeof(v3)*/, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dx->device->CreateInputLayout(InputDesc, 3, VertexShaderBlob->GetBufferPointer(),
		VertexShaderBlob->GetBufferSize(), &Layout);
	dx->context->IASetInputLayout(Layout);

	dx->vertexBuffer = VertexBuffer;

	// Contant buffer
	D3D11_BUFFER_DESC ContantBufferDesc;
	ZeroMemory(&ContantBufferDesc, sizeof(ContantBufferDesc));
	ContantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ContantBufferDesc.ByteWidth = sizeof(matrix_constant_buffer);
	ContantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dx->device->CreateBuffer(&ContantBufferDesc, NULL, &ContantBuffer);

	dx->context->VSSetConstantBuffers(0, 1, &ContantBuffer);

	// matrix_contant_buffer Matrix;
	// Matrix.MatProjection

	dx->context->UpdateSubresource(ContantBuffer, 0, 0, &OrthoMatrix, 0, 0);
}

void DX11Draw (dx_state *dx)
{
	matrix_constant_buffer constantBufferData = {};

	static float xRotation = 0.0f;
	static float yRotation = 0.0f;
	static float zRotation = 0.0f;

	D3DXMATRIX yRotateMatrix;
	D3DXMatrixRotationY(&yRotateMatrix, xRotation);
	D3DXMATRIX xRotateMatrix;
	D3DXMatrixRotationX(&xRotateMatrix, yRotation);
	D3DXMATRIX zRotateMatrix;
	D3DXMatrixRotationZ(&zRotateMatrix, zRotation);

	D3DXMATRIX translateMatrix;
	D3DXMatrixTranslation(&translateMatrix, 0.0f, 0.0f, 2.0f);
	
	D3DXMATRIX modelMatrix = xRotateMatrix * yRotateMatrix * zRotateMatrix;

	// constantBufferData.rotationMatrix = *((m4*)&modelMatrix);
	memcpy(&constantBufferData.rotationMatrix, &modelMatrix, sizeof(m4));

	modelMatrix = modelMatrix * translateMatrix;

	xRotation -= 0.00005f;
	yRotation += 0.00005f;
	zRotation += 0.00005f;
	// yRotation = -(3.14f / 8.0f);

	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(70), (float)1280/(float)720, 0.1f, 100.0f);
	D3DXMATRIX viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &D3DXVECTOR3(0, 0, 0),
									&D3DXVECTOR3(0, 0, 1.0f),
									&D3DXVECTOR3(0, 1.0f, 0));
	D3DXMATRIX matrix = modelMatrix * viewMatrix * projectionMatrix;

	constantBufferData.MatProjection = *((m4*)&matrix);

	dx->context->UpdateSubresource(ContantBuffer, 0, 0, &constantBufferData, 0, 0);

	float ClearColor[4] = {0.4f, 0.6f, 0.9f, 1.0f};
	dx->context->ClearRenderTargetView(dx->backBuffer, ClearColor);

	UINT Stride = sizeof(r_vertex);
	UINT Offset = 0;
	dx->context->IASetVertexBuffers(0, 1, &dx->vertexBuffer, &Stride, &Offset);
	dx->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dx->context->Draw(36, 0);

	dx->swapChain->Present(0, 0);
}