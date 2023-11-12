# D3D12DescriptorHelpers

Simple helper classes for Direct3D 12 that accompany an article on my blog:

**[Secrets of Direct3D 12: Do RTV and DSV descriptors make any sense?](https://asawicki.info/news_1772_secrets_of_direct3d_12_do_rtv_and_dsv_descriptors_make_any_sense)**

- [RenderTargetHelper.hpp](RenderTargetHelper.hpp) + [RenderTargetHelper.cpp](RenderTargetHelper.cpp) - helper class `RenderTargetHelper` that hides the complexity of creating and using RTV, DSV descriptors and offers convenient functions: `OMSetRenderTargets`, `ClearRenderTargetView`, `ClearDepthStencilView` that take pointer to a `ID3D12Resource` directly.

- [CopyTextureRegionUnhelper.hpp](CopyTextureRegionUnhelper.hpp) + [CopyTextureRegionUnhelper.cpp](CopyTextureRegionUnhelper.cpp) - a nonsensical library that introduces a concept of "copying descriptor" that you could use with `CopyTextureRegion` function instead of passing `ID3D12Resource` pointer directly. It adds additional, unnecessary complexity, which Microsoft doesn't require for this copy operation, while for RTV and DSV, unfortunately, they do.
