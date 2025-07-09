#include "pch.h"
#include "D12RootSignature.h"

namespace Engine
{
	D12RootSignature::~D12RootSignature()
	{
		Release();
	}

	void D12RootSignature::Initialize(ID3D12Device* pDevice)
	{
                D3D12_ROOT_PARAMETER rootParameter[2];

                // Constant buffer at b0
                rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                rootParameter[0].Descriptor.ShaderRegister = 0;
                rootParameter[0].Descriptor.RegisterSpace = 0;
                rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

                // Descriptor table for texture at t0
                D3D12_DESCRIPTOR_RANGE range = {};
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                range.NumDescriptors = 1;
                range.BaseShaderRegister = 0;
                range.RegisterSpace = 0;
                range.OffsetInDescriptorsFromTableStart = 0;

                rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                rootParameter[1].DescriptorTable.NumDescriptorRanges = 1;
                rootParameter[1].DescriptorTable.pDescriptorRanges = &range;
                rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rsDesc = {};
		rsDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
                D3D12_STATIC_SAMPLER_DESC sampler = {};
                sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
                sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
                sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
                sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
                sampler.ShaderRegister = 0;
                sampler.RegisterSpace = 0;
                sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

                rsDesc.Desc_1_0.pParameters = rootParameter;
                rsDesc.Desc_1_0.NumParameters = 2;
                rsDesc.Desc_1_0.NumStaticSamplers = 1;
                rsDesc.Desc_1_0.pStaticSamplers = &sampler;
		rsDesc.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ID3DBlob* pSerializedRS = nullptr;
		ID3DBlob* pErrorBlob = nullptr;

		KOSMO_EVAL_HR(D3D12SerializeVersionedRootSignature(&rsDesc, &pSerializedRS, &pErrorBlob), "Error serializing the root signature");
		if(pErrorBlob)
		{
			PRINT_N("RootSignature serialisation error: " << (const char*)pErrorBlob->GetBufferPointer())
		}
		KOSMO_EVAL_HR(pDevice->CreateRootSignature(0,pSerializedRS->GetBufferPointer(), pSerializedRS->GetBufferSize(), IID_PPV_ARGS(GetAddressOf())), "Error creating the root signature")
	}

	void D12RootSignature::Release()
	{
		if(Get())
		{
			Reset();
		}
	}
}
