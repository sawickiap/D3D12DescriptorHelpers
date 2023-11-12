/*
CopyTextureRegionUnhelper

Simple (un-)helper class for Direct3D 12 that accompanies an article on my blog:

https://asawicki.info/news_1772_secrets_of_direct3d_12_do_rtv_and_dsv_descriptors_make_any_sense

Author:  Adam Sawicki - http://asawicki.info
Version: 1.0.0, 2023-11-12
License: Public Domain
*/
#include "CopyTextureRegionUnhelper.hpp"

ULONG STDMETHODCALLTYPE ICopyingDescriptorHeap::Release()
{
    const ULONG Result = --m_RefCount;
    if(Result == 0)
        delete this;
    return Result;
}

ICopyingDescriptorHeap::ICopyingDescriptorHeap(UINT NumDescriptors) :
    m_NumDescriptors(NumDescriptors),
    m_Locations(new D3D12_TEXTURE_COPY_LOCATION[NumDescriptors])
{
}

ICopyingDescriptorHeap::~ICopyingDescriptorHeap()
{
    delete[] m_Locations;
}

UINT GetCopyingDescriptorHandleIncrementSize()
{
    return (UINT)sizeof(D3D12_TEXTURE_COPY_LOCATION);
}

HRESULT CreateCopyingDescriptorHeap(const D3D12_COPYING_DESCRIPTOR_HEAP_DESC* pDescriptorHeapDesc, ICopyingDescriptorHeap** ppHeap)
{
    *ppHeap = new ICopyingDescriptorHeap(pDescriptorHeapDesc->NumDescriptors);
    return S_OK;
}

void CreateCopyingFootprintView(
    ID3D12Resource* pResource,
    const COPYING_FOOTPRINT_VIEW_DESC* pDesc,
    D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
{
    D3D12_TEXTURE_COPY_LOCATION* pLocation = (D3D12_TEXTURE_COPY_LOCATION*)(void*)DestDescriptor.ptr;
    pLocation->pResource = pResource;
    pLocation->Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    pLocation->PlacedFootprint.Offset = pDesc->Offset;
    pLocation->PlacedFootprint.Footprint = pDesc->Footprint;
}

void CreateCopyingSubresourceView(
    ID3D12Resource* pResource,
    const COPYING_SUBRESOURCE_VIEW_DESC* pDesc,
    D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
{
    D3D12_TEXTURE_COPY_LOCATION* pLocation = (D3D12_TEXTURE_COPY_LOCATION*)(void*)DestDescriptor.ptr;
    pLocation->pResource = pResource;
    pLocation->Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    pLocation->SubresourceIndex = pDesc->SubresourceIndex;
}

void CopyTextureRegion(
    ID3D12GraphicsCommandList* pCommandList,
    D3D12_CPU_DESCRIPTOR_HANDLE Dst, UINT DstX, UINT DstY, UINT DstZ,
    D3D12_CPU_DESCRIPTOR_HANDLE Src, const D3D12_BOX* pSrcBox)
{
    const D3D12_TEXTURE_COPY_LOCATION* pDst = (const D3D12_TEXTURE_COPY_LOCATION*)(void*)Dst.ptr;
    const D3D12_TEXTURE_COPY_LOCATION* pSrc = (const D3D12_TEXTURE_COPY_LOCATION*)(void*)Src.ptr;
    pCommandList->CopyTextureRegion(pDst, DstX, DstY, DstZ, pSrc, pSrcBox);
}
