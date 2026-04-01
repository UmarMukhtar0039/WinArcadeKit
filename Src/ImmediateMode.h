#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace wak
{
    class ImmediateMode
    {
    public:
         ImmediateMode(const ImmediateMode&) = delete;
         ImmediateMode& operator=(const ImmediateMode&) = delete;
		~ImmediateMode() = default;


         static ImmediateMode* CreateImmediateMode(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext);
		 static void DestroyImmediateMode(ImmediateMode* device);


    private:
        ImmediateMode(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dDeviceContext);

        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
    };
}