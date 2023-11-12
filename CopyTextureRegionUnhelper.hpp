/*
CopyTextureRegionUnhelper

Simple (un-)helper class for Direct3D 12 that accompanies an article on my blog:

https://asawicki.info/news_1772_secrets_of_direct3d_12_do_rtv_and_dsv_descriptors_make_any_sense

Author:  Adam Sawicki - http://asawicki.info
Version: 1.0.0, 2023-11-12
License: Public Domain
*/
#pragma once

#include <d3d12.h>

UINT GetCopyingDescriptorHandleIncrementSize();

struct D3D12_COPYING_DESCRIPTOR_HEAP_DESC
{
    UINT NumDescriptors;
};

class ICopyingDescriptorHeap : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) { *ppvObject = NULL; return E_FAIL; }
    virtual ULONG STDMETHODCALLTYPE AddRef() { return ++m_RefCount; }
    virtual ULONG STDMETHODCALLTYPE Release();

    D3D12_COPYING_DESCRIPTOR_HEAP_DESC GetDesc() { D3D12_COPYING_DESCRIPTOR_HEAP_DESC Desc = { m_NumDescriptors }; return Desc; }

    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() { return D3D12_CPU_DESCRIPTOR_HANDLE{ (SIZE_T)m_Locations }; }

private:
    friend HRESULT CreateCopyingDescriptorHeap(
        const D3D12_COPYING_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc,
        ICopyingDescriptorHeap** ppHeap);

    ULONG m_RefCount = 1;
    UINT m_NumDescriptors = 0;
    D3D12_TEXTURE_COPY_LOCATION* m_Locations = NULL;

    ICopyingDescriptorHeap(UINT NumDescriptors);
    ~ICopyingDescriptorHeap();
};

HRESULT CreateCopyingDescriptorHeap(
    const D3D12_COPYING_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc,
    ICopyingDescriptorHeap** ppHeap);

struct COPYING_FOOTPRINT_VIEW_DESC
{
    UINT64 Offset;
    D3D12_SUBRESOURCE_FOOTPRINT Footprint;
};
void CreateCopyingFootprintView(
    ID3D12Resource* pResource,
    const COPYING_FOOTPRINT_VIEW_DESC* pDesc,
    D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

struct COPYING_SUBRESOURCE_VIEW_DESC
{
    UINT SubresourceIndex;
};
void CreateCopyingSubresourceView(
    ID3D12Resource* pResource,
    const COPYING_SUBRESOURCE_VIEW_DESC* pDesc,
    D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

void CopyTextureRegion(
    ID3D12GraphicsCommandList* pCommandList,
    D3D12_CPU_DESCRIPTOR_HANDLE Dst, UINT DstX, UINT DstY, UINT DstZ,
    D3D12_CPU_DESCRIPTOR_HANDLE Src, const D3D12_BOX* pSrcBox);
