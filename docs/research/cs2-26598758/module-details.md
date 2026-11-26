# Per-module evidence

Names and RVAs below come from PE tables and strings. Roles are discussed in overview.md.
Interface candidates are strings only. RTTI names identify types, not named function bodies.

## amd_ags_x64.dll

Path: `game/bin/win64/amd_ags_x64.dll`  
SHA-256: `2449e8339e0f031bc4f954398f5917b8eb5a2d20c32d4688a083d5dd9f637ab8`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x6d6c`.  
Exports: 38; RTTI types: 3; x64 unwind entries: 534.

Normal import modules: `KERNEL32.dll`, `USER32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `agsCheckDriverVersion` | `0x1e70` |  |
| `agsDeInitialize` | `0x6400` |  |
| `agsDriverExtensionsDX11_BeginUAVOverlap` | `0x26b0` |  |
| `agsDriverExtensionsDX11_CreateBuffer` | `0x1000` |  |
| `agsDriverExtensionsDX11_CreateDevice` | `0x2730` |  |
| `agsDriverExtensionsDX11_CreateFromDevice` | `0x2af0` |  |
| `agsDriverExtensionsDX11_CreateTexture1D` | `0x1090` |  |
| `agsDriverExtensionsDX11_CreateTexture2D` | `0x1120` |  |
| `agsDriverExtensionsDX11_CreateTexture3D` | `0x11b0` |  |
| `agsDriverExtensionsDX11_Destroy` | `0x2b80` |  |
| `agsDriverExtensionsDX11_DestroyDevice` | `0x2ba0` |  |
| `agsDriverExtensionsDX11_EndUAVOverlap` | `0x2c90` |  |
| `agsDriverExtensionsDX11_GetMaxClipRects` | `0x2d10` |  |
| `agsDriverExtensionsDX11_IASetPrimitiveTopology` | `0x2d60` |  |
| `agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect` | `0x2dc0` |  |
| `agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect` | `0x2e90` |  |
| `agsDriverExtensionsDX11_MultiDrawInstancedIndirect` | `0x2f90` |  |
| `agsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect` | `0x3060` |  |
| `agsDriverExtensionsDX11_NotifyResourceBeginAllAccess` | `0x1240` |  |
| `agsDriverExtensionsDX11_NotifyResourceEndAllAccess` | `0x1280` |  |
| `agsDriverExtensionsDX11_NotifyResourceEndWrites` | `0x12c0` |  |
| `agsDriverExtensionsDX11_NumPendingAsyncCompileJobs` | `0x3160` |  |
| `agsDriverExtensionsDX11_SetClipRects` | `0x31b0` |  |
| `agsDriverExtensionsDX11_SetDepthBounds` | `0x3340` |  |
| `agsDriverExtensionsDX11_SetDiskShaderCacheEnabled` | `0x33e0` |  |
| `agsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount` | `0x3430` |  |
| `agsDriverExtensionsDX11_SetViewBroadcastMasks` | `0x3470` |  |
| `agsDriverExtensionsDX11_WriteBreadcrumb` | `0x34c0` |  |
| `agsDriverExtensionsDX12_CreateDevice` | `0x3c50` |  |
| `agsDriverExtensionsDX12_CreateFromDevice` | `0x3e60` |  |

RTTI sample: `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

## animationsystem.dll

Path: `game/bin/win64/animationsystem.dll`  
SHA-256: `766e37db084fe5e867251120ee5493b2c3e6dc8d28e92e4ac6f9b8d15076552b`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x5ef088`.  
Exports: 6; RTTI types: 2044; x64 unwind entries: 31627.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\animationsystem\_vpc_animationsystem\unity\win64\_msvc_\Retail\animationsystem.pdb`; GUID `e3e73187-5759-4301-853d-d24c34b7731b`; age 12; lookup key `E3E7318757594301853DD24C34B7731BC`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x8bca0` |  |
| `CreateInterface` | `0x5e1090` |  |
| `ExtractModuleMetadata` | `0x8c180` |  |
| `GetResourceManifestCount` | `0x5e88f0` |  |
| `GetResourceManifests` | `0x5e8900` |  |
| `InstallSchemaBindings` | `0x5e84e0` |  |

RTTI sample: `.?AU?$ArrayWrapper@V?$CUtlVector@UCWeightPreview@@HV?$CUtlVectorMemory_Growable@UCWeightPreview@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@V?$CSmartPtr@VCAnimComponentInstance@@VCRefCountAccessor@@@@HV?$CUtlVectorMemory_Growable@V?$CSmartPtr@VCAnimComponentInstance@@VCRefCountAccessor@@@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@V?$CSmartPtr@VCAnimMotorInstance@@VCRefCountAccessor@@@@HV?$CUtlVectorMemory_Growable@V?$CSmartPtr@VCAnimMotorInstance@@VCRefCountAccessor@@@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@V?$CSmartPtr@VCAnimParameterInstance@@VCRefCountAccessor@@@@HV?$CUtlVectorMemory_Growable@V?$CSmartPtr@VCAnimParameterInstance@@VCRefCountAccessor@@@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@VCGlobalSymbol@@HV?$CUtlVectorMemory_Growable@VCGlobalSymbol@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$CFactoryMakerT@VCAudioAnimTag@@@?$CTypeFactory@XVCAnimTagBase@@$$V@Reflection@@`, `.?AU?$CFactoryMakerT@VCBlend2DAnimNodeInstance@@@CAnimNodeInstanceFactory@@`, `.?AU?$CFactoryMakerT@VCBodyGroupAnimTag@@@?$CTypeFactory@XVCAnimTagBase@@$$V@Reflection@@`, `.?AU?$CFactoryMakerT@VCBoolAnimParameter@@@?$CTypeFactory@XVCAnimParameterBase@@$$V@Reflection@@`, `.?AU?$CFactoryMakerT@VCChoreoAnimNodeInstance@@@CAnimNodeInstanceFactory@@`, `.?AU?$CFactoryMakerT@VCClothSettingsAnimTag@@@?$CTypeFactory@XVCAnimTagBase@@$$V@Reflection@@`, `.?AU?$CFactoryMakerT@VCDirectPlaybackAnimNodeInstance@@@CAnimNodeInstanceFactory@@`, `.?AU?$CFactoryMakerT@VCEnumAnimParameter@@@?$CTypeFactory@XVCAnimParameterBase@@$$V@Reflection@@`, `.?AU?$CFactoryMakerT@VCFloatAnimParameter@@@?$CTypeFactory@XVCAnimParameterBase@@$$V@Reflection@@`, `.?AU?$CFactoryMakerT@VCFootFallAnimTag@@@?$CTypeFactory@XVCAnimTagBase@@$$V@Reflection@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## assetpreview.dll

Path: `game/bin/win64/assetpreview.dll`  
SHA-256: `99311efc545502e9e5226800f6eb9364dcfd33ac445e7b92e1ebc356faaed018`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x7430a4`.  
Exports: 7; RTTI types: 6295; x64 unwind entries: 42629.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\assetpreview\_vpc_assetpreview\default\win64\_msvc_\Retail\assetpreview.pdb`; GUID `c5145636-de4e-4e64-b106-d35c5373ce4b`; age 12; lookup key `C5145636DE4E4E64B106D35C5373CE4BC`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `winsqlite3.dll`, `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`, `WS2_32.dll`, `Secur32.dll`, `CRYPT32.dll`, `bcrypt.dll`, `IPHLPAPI.DLL`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x33160` |  |
| `CreateInterface` | `0x68bf00` |  |
| `ExtractModuleMetadata` | `0x336e0` |  |
| `GetResourceManifestCount` | `0x693070` |  |
| `GetResourceManifests` | `0x693080` |  |
| `InstallSchemaBindings` | `0x694b40` |  |
| `icudt74_dat` | `0xcb0a60` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUCMorphSetData@@`, `.?AUCPulseCell_BooleanSwitchState_DebugInfo_t@@`, `.?AUCPulseCell_Inflow_Wait_DebugInfo_t@@`, `.?AUCPulseCell_IntervalTimer_DebugInfo_t@@`, `.?AUCPulseCell_Timeline_DebugInfo_t@@`, `.?AUCScaleBiasFilter@CModelDocClothProxyMeshFile@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## ati_compress_wrapper.dll

Path: `game/bin/win64/ati_compress_wrapper.dll`  
SHA-256: `59b8e6afa8bd163213b63bbc8b7af18e495ddebee801ebda39ef62fd559901c3`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x408f0`.  
Exports: 2; RTTI types: 63; x64 unwind entries: 1969.

PDB reference: `d:\clients\source2_main\src\tools\ati_compress_wrapper\x64\Release\ati_compress_wrapper.pdb`; GUID `bbf8b1e5-b046-40f8-9e44-0fd316267742`; age 1; lookup key `BBF8B1E5B04640F89E440FD3162677421`.

Normal import modules: `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `ATI_TC_CalculateBufferSize_Wrapper` | `0x1030` |  |
| `ATI_TC_ConvertTexture_Wrapper` | `0x1000` |  |

RTTI sample: `.?AUctype_base@std@@`, `.?AV?$ctype@D@std@@`, `.?AV?$num_get@DV?$istreambuf_iterator@DU?$char_traits@D@std@@@std@@@std@@`, `.?AV?$num_put@DV?$ostreambuf_iterator@DU?$char_traits@D@std@@@std@@@std@@`, `.?AV?$numpunct@D@std@@`, `.?AVCCodec@ATI_Compress@@`, `.?AVCCodecBuffer@@`, `.?AVCCodecBuffer_Block@@`, `.?AVCCodecBuffer_R16@@`, `.?AVCCodecBuffer_R16F@@`, `.?AVCCodecBuffer_R32@@`, `.?AVCCodecBuffer_R32F@@`, `.?AVCCodecBuffer_R8@@`, `.?AVCCodecBuffer_RG16@@`, `.?AVCCodecBuffer_RG16F@@`.

## bugreporter_filequeue.dll

Path: `game/bin/win64/bugreporter_filequeue.dll`  
SHA-256: `03cddfc7db62863a767bce90c62e80c23a4c53cc6280e00018a2c7b0f94a81ce`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xa478`.  
Exports: 3; RTTI types: 6; x64 unwind entries: 490.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\bugreporter_filequeue\_vpc_bugreporter_filequeue\default\win64\_msvc_\Retail\bugreporter_filequeue.pdb`; GUID `618b2e46-465a-4e5b-b4e0-7286f5ccc0ce`; age 7; lookup key `618B2E46465A4E5BB4E07286F5CCC0CE7`.

Normal import modules: `WSOCK32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x6750` |  |
| `CreateInterface` | `0x6d60` |  |
| `ExtractModuleMetadata` | `0x6b90` |  |

RTTI sample: `.?AVCBugReporter@@`, `.?AVIBaseInterface@@`, `.?AVIBugReporter@@`, `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `BugReporter005`, `VFileSystem017`.

## cairo.dll

Path: `game/bin/win64/cairo.dll`  
SHA-256: `44d613d7c51049241755fb95707c52c86565e50e7153ed601a6df6c5e5e8e9ae`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xddf84`.  
Exports: 325; RTTI types: 3; x64 unwind entries: 4273.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\thirdparty\cairo\_vpc_cairo\default\win64\_msvc_\Retail\cairo.pdb`; GUID `63fb0b7b-1307-40e4-aa9c-482465e52ad1`; age 5; lookup key `63FB0B7B130740E4AA9C482465E52AD15`.

Normal import modules: `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `cairo_append_path` | `0x74a20` |  |
| `cairo_arc` | `0x72990` |  |
| `cairo_arc_negative` | `0x72a90` |  |
| `cairo_clip` | `0x73550` |  |
| `cairo_clip_extents` | `0x73670` |  |
| `cairo_clip_preserve` | `0x735b0` |  |
| `cairo_close_path` | `0x72d30` |  |
| `cairo_copy_clip_rectangle_list` | `0x73760` |  |
| `cairo_copy_page` | `0x73290` |  |
| `cairo_copy_path` | `0x74980` |  |
| `cairo_copy_path_flat` | `0x749d0` |  |
| `cairo_create` | `0x71be0` |  |
| `cairo_curve_to` | `0x72910` |  |
| `cairo_debug_reset_static_data` | `0x10640` |  |
| `cairo_destroy` | `0x71c30` |  |
| `cairo_device_acquire` | `0x14e00` |  |
| `cairo_device_destroy` | `0x14cc0` |  |
| `cairo_device_finish` | `0x14c60` |  |
| `cairo_device_flush` | `0x14c20` |  |
| `cairo_device_get_reference_count` | `0x14f10` |  |
| `cairo_device_get_type` | `0x14de0` |  |
| `cairo_device_get_user_data` | `0x14f20` |  |
| `cairo_device_observer_elapsed` | `0x5c880` |  |
| `cairo_device_observer_fill_elapsed` | `0x5caa0` |  |
| `cairo_device_observer_glyphs_elapsed` | `0x5cc00` |  |
| `cairo_device_observer_mask_elapsed` | `0x5c9f0` |  |
| `cairo_device_observer_paint_elapsed` | `0x5c950` |  |
| `cairo_device_observer_print` | `0x5c770` |  |
| `cairo_device_observer_stroke_elapsed` | `0x5cb50` |  |
| `cairo_device_reference` | `0x14bd0` |  |

RTTI sample: `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

## cs2.exe

Path: `game/bin/win64/cs2.exe`  
SHA-256: `3020e5ee5d625bebb971e7a853acf9bc3c70f77ade540ffc2a04ee2669280ba2`  
Architecture: PE32+ x86-64; preferred base `0x140000000`; entry RVA `0xead0`.  
Exports: 7; RTTI types: 7; x64 unwind entries: 714.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\source2launcher\_vpc_source2launcher_csgo\default\win64\_msvc_\Retail\cs2.pdb`; GUID `91f6ea07-cfe7-4557-aa46-926707d81a22`; age 5; lookup key `91F6EA07CFE74557AA46926707D81A225`.

Normal import modules: `USER32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `AmdPowerXpressRequestHighPerformance` | `0xbcde8` |  |
| `BSecureAllowed` | `0x4cf0` |  |
| `CountFilesCompletedTrustCheck` | `0x4cd0` |  |
| `CountFilesNeedTrustCheck` | `0x4cc0` |  |
| `CountItemsToReport` | `0x4ce0` |  |
| `GetTotalFilesLoaded` | `0x4cb0` |  |
| `NvOptimusEnablement` | `0xbcde4` |  |

RTTI sample: `.?AVbad_alloc@std@@`, `.?AVbad_array_new_length@std@@`, `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVlength_error@std@@`, `.?AVlogic_error@std@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `ttthhhddd049`.

## d3dcompiler_47.dll

Path: `game/bin/win64/d3dcompiler_47.dll`  
SHA-256: `4432bbd1a390874f3f0a503d45cc48d346abc3a8c0213c289f4b615bf0ee84f3`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x25e6b0`.  
Exports: 29; RTTI types: 0; x64 unwind entries: 10663.

PDB reference: `D3DCompiler_47.pdb`; GUID `1465bef7-74cf-8f52-6caa-3f238f2325fe`; age 1; lookup key `1465BEF774CF8F526CAA3F238F2325FE1`.

Normal import modules: `msvcrt.dll`, `KERNEL32.dll`, `ADVAPI32.dll`, `RPCRT4.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `D3DAssemble` | `0xeb4e0` |  |
| `DebugSetMute` | `0x620b0` |  |
| `D3DCompile` | `0xeb1e0` |  |
| `D3DCompile2` | `0xeb260` |  |
| `D3DCompileFromFile` | `0xeb360` |  |
| `D3DCompressShaders` | `0xee310` |  |
| `D3DCreateBlob` | `0x7390` |  |
| `D3DCreateFunctionLinkingGraph` | `0x7540` |  |
| `D3DCreateLinker` | `0x73b0` |  |
| `D3DDecompressShaders` | `0xee680` |  |
| `D3DDisassemble` | `0x6f80` |  |
| `D3DDisassemble10Effect` | `0x169a0` |  |
| `D3DDisassemble11Trace` | `0x7050` |  |
| `D3DDisassembleRegion` | `0x6fc0` |  |
| `D3DGetBlobPart` | `0xecf60` |  |
| `D3DGetDebugInfo` | `0xed580` |  |
| `D3DGetInputAndOutputSignatureBlob` | `0xed610` |  |
| `D3DGetInputSignatureBlob` | `0xed5b0` |  |
| `D3DGetOutputSignatureBlob` | `0xed5e0` |  |
| `D3DGetTraceInstructionOffsets` | `0x7010` |  |
| `D3DLoadModule` | `0x7440` |  |
| `D3DPreprocess` | `0xec6f0` |  |
| `D3DReadFileToBlob` | `0xe9460` |  |
| `D3DReflect` | `0x7090` |  |
| `D3DReflectLibrary` | `0x7200` |  |
| `D3DReturnFailure1` | `0x17820` |  |
| `D3DSetBlobPart` | `0xecfa0` |  |
| `D3DStripShader` | `0xed640` |  |
| `D3DWriteBlobToFile` | `0xe9680` |  |

## dbghelp.dll

Path: `game/bin/win64/dbghelp.dll`  
SHA-256: `13f0c74845318b52b76e6000564b1a99c37de48422b44ac74d034fa222c65a23`  
Architecture: PE32+ x86-64; preferred base `0x3000000`; entry RVA `0xb0b50`.  
Exports: 203; RTTI types: 0; x64 unwind entries: 5823.

PDB reference: `dbghelp.pdb`; GUID `a8298dbe-560e-4d32-a8f5-8b073c8f9261`; age 1; lookup key `A8298DBE560E4D32A8F58B073C8F92611`.

Normal import modules: `msvcrt.dll`, `KERNEL32.dll`.

Delay import modules: ADVAPI32.dll, RPCRT4.dll.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `SymGetOmapBlockBase` | `0x4c610` |  |
| `DbgHelpCreateUserDump` | `0x63970` |  |
| `DbgHelpCreateUserDumpW` | `0x63a90` |  |
| `EnumDirTree` | `0x523c0` |  |
| `EnumDirTreeW` | `0x52500` |  |
| `EnumerateLoadedModules` | `0x4b540` |  |
| `EnumerateLoadedModules64` | `0x4b540` |  |
| `EnumerateLoadedModulesEx` | `0x4b680` |  |
| `EnumerateLoadedModulesExW` | `0x4b720` |  |
| `EnumerateLoadedModulesW64` | `0x4b5e0` |  |
| `ExtensionApiVersion` | `0x33dd0` |  |
| `FindDebugInfoFile` | `0x50910` |  |
| `FindDebugInfoFileEx` | `0x51ae0` |  |
| `FindDebugInfoFileExW` | `0x51a70` |  |
| `FindExecutableImage` | `0x4f650` |  |
| `FindExecutableImageEx` | `0x50530` |  |
| `FindExecutableImageExW` | `0x506b0` |  |
| `FindFileInPath` | `0x4f4b0` |  |
| `FindFileInSearchPath` | `0x4f530` |  |
| `GetTimestampForLoadedLibrary` | `0x3d230` |  |
| `ImageDirectoryEntryToData` | `0x3cc50` |  |
| `ImageDirectoryEntryToDataEx` | `0x3ca90` |  |
| `ImageNtHeader` | `0x3c610` |  |
| `ImageRvaToSection` | `0x3cca0` |  |
| `ImageRvaToVa` | `0x3cd50` |  |
| `ImagehlpApiVersion` | `0x52970` |  |
| `ImagehlpApiVersionEx` | `0x52980` |  |
| `MakeSureDirectoryPathExists` | `0x526f0` |  |
| `MiniDumpReadDumpStream` | `0x775a0` |  |
| `MiniDumpWriteDump` | `0x771a0` |  |

## embree3.dll

Path: `game/bin/win64/embree3.dll`  
SHA-256: `903b3c680ffbf2965d7988f0a42d902c5e198cab15c435ddea79852573be3620`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x13e6550`.  
Exports: 117; RTTI types: 1121; x64 unwind entries: 14592.

Normal import modules: `KERNEL32.dll`, `ADVAPI32.dll`, `MSVCP140.dll`, `VCRUNTIME140.dll`, `VCRUNTIME140_1.dll`, `api-ms-win-crt-runtime-l1-1-0.dll`, `api-ms-win-crt-heap-l1-1-0.dll`, `api-ms-win-crt-stdio-l1-1-0.dll`, `api-ms-win-crt-math-l1-1-0.dll`, `api-ms-win-crt-convert-l1-1-0.dll`, `api-ms-win-crt-string-l1-1-0.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `?add@ThreadPool@TaskScheduler@embree@@QEAAXAEBV?$Ref@UTaskScheduler@embree@@@3@@Z` | `0x13e4940` |  |
| `?addScheduler@TaskScheduler@embree@@CAXAEBV?$Ref@UTaskScheduler@embree@@@2@@Z` | `0x13e4a00` |  |
| `?allocThreadIndex@TaskScheduler@embree@@QEAA_JXZ` | `0x13e4a10` |  |
| `?execute_local@TaskQueue@TaskScheduler@embree@@QEAA_NAEAUThread@23@PEAUTask@23@@Z` | `0x13e4c10` |  |
| `?instance@TaskScheduler@embree@@CAPEAU12@XZ` | `0x13e4d30` |  |
| `?remove@ThreadPool@TaskScheduler@embree@@QEAAXAEBV?$Ref@UTaskScheduler@embree@@@3@@Z` | `0x13e4ef0` |  |
| `?removeScheduler@TaskScheduler@embree@@CAXAEBV?$Ref@UTaskScheduler@embree@@@2@@Z` | `0x13e4f90` |  |
| `?run@Task@TaskScheduler@embree@@QEAAXAEAUThread@23@@Z` | `0x13e4fa0` |  |
| `?startThreads@TaskScheduler@embree@@CAXXZ` | `0x13e5260` |  |
| `?startThreads@ThreadPool@TaskScheduler@embree@@QEAAXXZ` | `0x13e5280` |  |
| `?swapThread@TaskScheduler@embree@@CAPEAUThread@12@PEAU312@@Z` | `0x13e53c0` |  |
| `?thread@TaskScheduler@embree@@CAPEAUThread@12@XZ` | `0x13e53f0` |  |
| `?threadCount@TaskScheduler@embree@@SA_KXZ` | `0x13e5410` |  |
| `?threadID@TaskScheduler@embree@@SA_KXZ` | `0x13e5420` |  |
| `?threadIndex@TaskScheduler@embree@@SA_KXZ` | `0x13e5420` |  |
| `?wait@TaskScheduler@embree@@SA_NXZ` | `0x13e57e0` |  |
| `rtcAttachGeometry` | `0x11ec0` |  |
| `rtcAttachGeometryByID` | `0x12040` |  |
| `rtcBuildBVH` | `0x38580` |  |
| `rtcCollide` | `0x12230` |  |
| `rtcCommitGeometry` | `0x12260` |  |
| `rtcCommitScene` | `0x122e0` |  |
| `rtcDetachGeometry` | `0x12360` |  |
| `rtcDisableGeometry` | `0x12440` |  |
| `rtcEnableGeometry` | `0x124c0` |  |
| `rtcFilterIntersection` | `0x12540` |  |
| `rtcFilterOcclusion` | `0x12580` |  |
| `rtcGetBufferData` | `0x125c0` |  |
| `rtcGetDeviceError` | `0x12640` |  |
| `rtcGetDeviceProperty` | `0x12670` |  |

RTTI sample: `.?AU?$BVHNBuilderFastSpatialSAH@$03UQuadMesh@embree@@U?$QuadMv@$03@2@UQuadSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UQuadMesh@embree@@U?$QuadMv@$03@2@UQuadSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleM@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleM@$03@2@UTriangleSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMi@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMi@$03@2@UTriangleSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMv@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMv@$03@2@UTriangleSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$07UQuadMesh@embree@@U?$QuadMv@$03@2@UQuadSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$07UTriangleMesh@embree@@U?$TriangleM@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$07UTriangleMesh@embree@@U?$TriangleMv@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UInstance@embree@@UInstancePrimitive@2@@avx@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UInstance@embree@@UInstancePrimitive@2@@sse2@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UQuadMesh@embree@@U?$QuadMi@$03@2@@avx@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UQuadMesh@embree@@U?$QuadMi@$03@2@@sse2@embree@@`.

## engine2.dll

Path: `game/bin/win64/engine2.dll`  
SHA-256: `dcb5fe13f60f0db9faa365c7b48c3ca107ce56783748bf9ed9ca41fd892af63c`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x4ae26c`.  
Exports: 43; RTTI types: 1308; x64 unwind entries: 22043.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\engine2\_vpc_engine2\default\win64\_msvc_\Retail\engine2.pdb`; GUID `fcc665e0-6e57-43ea-872e-19951a118605`; age 24; lookup key `FCC665E06E5743EA872E19951A11860518`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `GDI32.dll`, `WS2_32.dll`, `steam_api64.dll`, `steamnetworkingsockets.dll`, `tier0.dll`, `KERNEL32.dll`, `WINMM.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x246270` |  |
| `CreateInterface` | `0x407190` |  |
| `ExtractModuleMetadata` | `0x246710` |  |
| `GetResourceManifestCount` | `0x41ee80` |  |
| `GetResourceManifests` | `0x41ee90` |  |
| `InstallSchemaBindings` | `0x4205a0` |  |
| `Source2Main` | `0x20dbe0` |  |
| `opus_decode` | `0x39a780` |  |
| `opus_decode_float` | `0x39a8d0` |  |
| `opus_decoder_create` | `0x39b960` |  |
| `opus_decoder_ctl` | `0x39ba50` |  |
| `opus_decoder_destroy` | `0x37b8b0` |  |
| `opus_decoder_get_nb_samples` | `0x39bc80` |  |
| `opus_decoder_get_size` | `0x39bd10` |  |
| `opus_decoder_init` | `0x39bd60` |  |
| `opus_encode` | `0x378180` |  |
| `opus_encode_float` | `0x378370` |  |
| `opus_encoder_create` | `0x37ae30` |  |
| `opus_encoder_ctl` | `0x37af50` |  |
| `opus_encoder_destroy` | `0x37b8b0` |  |
| `opus_encoder_get_size` | `0x37b8c0` |  |
| `opus_encoder_init` | `0x37b910` |  |
| `opus_get_version_string` | `0x38d360` |  |
| `opus_multistream_packet_pad` | `0x386700` |  |
| `opus_multistream_packet_unpad` | `0x386800` |  |
| `opus_packet_get_bandwidth` | `0x39beb0` |  |
| `opus_packet_get_nb_channels` | `0x39bf00` |  |
| `opus_packet_get_nb_frames` | `0x39bf10` |  |
| `opus_packet_get_nb_samples` | `0x39bf50` |  |
| `opus_packet_get_samples_per_frame` | `0x399910` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUCCallbackInternal_Steam_OnUGCDownload@CGameResourceManifest@@`, `.?AUCPulseCell_Inflow_Wait_DebugInfo_t@@`, `.?AUISocketCreatorListener@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AUNetPacketCustomAlloc_std_string@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `BugBugService001`, `BugService001`.

## filesystem_stdio.dll

Path: `game/bin/win64/filesystem_stdio.dll`  
SHA-256: `651f16b94ef6e982f6d82a9c0481a6b21b48657aa1c339079730a010156c6454`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x12cc74`.  
Exports: 6; RTTI types: 63; x64 unwind entries: 5660.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\filesystem\_vpc_filesystem_stdio\default\win64\_msvc_\Retail\filesystem_stdio.pdb`; GUID `5dbb4b21-d705-4c71-8ef3-25381d0ece6d`; age 10; lookup key `5DBB4B21D7054C718EF325381D0ECE6DA`.

Normal import modules: `steam_api64.dll`, `tier0.dll`, `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`, `WS2_32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x6c560` |  |
| `CreateInterface` | `0x11c2e0` |  |
| `ExtractModuleMetadata` | `0x6c9d0` |  |
| `GetResourceManifestCount` | `0x123190` |  |
| `GetResourceManifests` | `0x1231a0` |  |
| `InstallSchemaBindings` | `0x1233f0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIAsyncFileSystem2@@@@`, `.?AV?$CBaseAppSystem@VIFileSystem@@@@`, `.?AV?$CTier0AppSystem@VIAsyncFileSystem2@@$0A@@@`, `.?AV?$CTier0AppSystem@VIFileSystem@@$0A@@@`, `.?AV?$CTier1AppSystem@VIAsyncFileSystem2@@$0A@@@`, `.?AV?$CTier1AppSystem@VIFileSystem@@$0A@@@`, `.?AV?$CTier2AppSystem@VIAsyncFileSystem2@@$0A@@@`, `.?AV?$CTier2AppSystem@VIFileSystem@@$0A@@@`, `.?AV?$_Ref_count_obj2@VCBarrierRequest@CAsyncFileSystem2@@@std@@`, `.?AV?$_Ref_count_obj2@VCReadRequestHandle@CAsyncFileSystem2@@@std@@`, `.?AV?$_Ref_count_obj2@VCWriteRequestHandle@CAsyncFileSystem2@@@std@@`, `.?AV?$enable_shared_from_this@VCReadRequestHandle@CAsyncFileSystem2@@@std@@`, `.?AV?$enable_shared_from_this@VCWriteRequestHandle@CAsyncFileSystem2@@@std@@`, `.?AVCAsyncFileSystem2@@`, `.?AVCBarrierRequest@CAsyncFileSystem2@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## gfsdk_aftermath_lib.x64.dll

Path: `game/bin/win64/gfsdk_aftermath_lib.x64.dll`  
SHA-256: `d3fa76f1928d3914a8a2a0f9e55a8077c1253e8f9d64cba9e1d3760cfd5fa25e`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x21a500`.  
Exports: 43; RTTI types: 491; x64 unwind entries: 9787.

PDB reference: `E:\temp\Built\Int\rel-pub\windows-desktop-win7-x64\Shared\GCD\Modules\NsightAftermath\GFSDK_Aftermath_Lib.x64.pdb`; GUID `1eb32198-f6fa-4e76-a6ab-befc4a918aa9`; age 1; lookup key `1EB32198F6FA4E76A6ABBEFC4A918AA91`.

Normal import modules: `VERSION.dll`, `KERNEL32.dll`, `USER32.dll`, `ole32.dll`, `OLEAUT32.dll`, `ADVAPI32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `GFSDK_Aftermath_DX11_CreateContextHandle` | `0x76190` |  |
| `GFSDK_Aftermath_DX11_Initialize` | `0x761a0` |  |
| `GFSDK_Aftermath_DX12_CreateContextHandle` | `0x761b0` |  |
| `GFSDK_Aftermath_DX12_Initialize` | `0x761c0` |  |
| `GFSDK_Aftermath_DX12_RegisterResource` | `0x761e0` |  |
| `GFSDK_Aftermath_DX12_UnregisterResource` | `0x761f0` |  |
| `GFSDK_Aftermath_DisableGpuCrashDumps` | `0x76200` |  |
| `GFSDK_Aftermath_EnableGpuCrashDumps` | `0x76210` |  |
| `GFSDK_Aftermath_GetContextError` | `0x76220` |  |
| `GFSDK_Aftermath_GetCrashDumpStatus` | `0x76230` |  |
| `GFSDK_Aftermath_GetData` | `0x76240` |  |
| `GFSDK_Aftermath_GetDeviceStatus` | `0x76250` |  |
| `GFSDK_Aftermath_GetPageFaultInformation` | `0x76260` |  |
| `GFSDK_Aftermath_GetShaderDebugInfoIdentifier` | `0x76270` |  |
| `GFSDK_Aftermath_GetShaderDebugName` | `0x76280` |  |
| `GFSDK_Aftermath_GetShaderDebugNameSpirv` | `0x76290` |  |
| `GFSDK_Aftermath_GetShaderHash` | `0x762a0` |  |
| `GFSDK_Aftermath_GetShaderHashForShaderInfo` | `0x762b0` |  |
| `GFSDK_Aftermath_GetShaderHashSpirv` | `0x762c0` |  |
| `GFSDK_Aftermath_GpuCrashDump_CreateDecoder` | `0x762d0` |  |
| `GFSDK_Aftermath_GpuCrashDump_DestroyDecoder` | `0x762e0` |  |
| `GFSDK_Aftermath_GpuCrashDump_GenerateJSON` | `0x762f0` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetActiveShadersInfo` | `0x76300` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetActiveShadersInfoCount` | `0x76310` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetBaseInfo` | `0x76320` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetDescription` | `0x76330` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetDescriptionSize` | `0x76340` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetDeviceInfo` | `0x76350` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetEventMarkersInfo` | `0x76360` |  |
| `GFSDK_Aftermath_GpuCrashDump_GetEventMarkersInfoCount` | `0x76370` |  |

RTTI sample: `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0x74c0a773@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0x7816ffbb@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0x8389e6e7@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0x89cda846@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0x944aaa40@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0xcc9ac081@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0xd620d486@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$DesktopDumpHelperHAL@$1?WarpErrors@?A0xfcbe250a@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUWarpErrorDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$MmuHAL@$1?MmuFaultTypes@?A0x74c0a773@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B$1?MmuAccessTypes@2345@3V?$unordered_map@IUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuEngineIds@2345@3V?$unordered_map@IUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuGpcClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuHubClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$MmuHAL@$1?MmuFaultTypes@?A0x7816ffbb@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B$1?MmuAccessTypes@2345@3V?$unordered_map@IUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuEngineIds@2345@3V?$unordered_map@IUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuGpcClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuHubClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$MmuHAL@$1?MmuFaultTypes@?A0x8389e6e7@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B$1?MmuAccessTypes@2345@3V?$unordered_map@IUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuEngineIds@2345@3V?$unordered_map@IUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuGpcClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuHubClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$MmuHAL@$1?MmuFaultTypes@?A0x89cda846@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B$1?MmuAccessTypes@2345@3V?$unordered_map@IUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuEngineIds@2345@3V?$unordered_map@IUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuGpcClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuHubClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$MmuHAL@$1?MmuFaultTypes@?A0x944aaa40@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B$1?MmuAccessTypes@2345@3V?$unordered_map@IUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuEngineIds@2345@3V?$unordered_map@IUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuGpcClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuHubClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$MmuHAL@$1?MmuFaultTypes@?A0xcc9ac081@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B$1?MmuAccessTypes@2345@3V?$unordered_map@IUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuEngineIds@2345@3V?$unordered_map@IUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuGpcClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuHubClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B@DesktopDumpHelper@GCD@NV@@`, `.?AU?$MmuHAL@$1?MmuFaultTypes@?A0xd620d486@DesktopDumpHelper@GCD@NV@@3V?$unordered_map@IUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuFaultTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@std@@B$1?MmuAccessTypes@2345@3V?$unordered_map@IUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuAccessTypeDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuEngineIds@2345@3V?$unordered_map@IUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuEngineIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuGpcClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B$1?MmuHubClientIds@2345@3V?$unordered_map@IUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@U?$hash@I@std@@U?$equal_to@I@6@V?$allocator@U?$pair@$$CBIUMmuClientIdDetails@DesktopDumpHelper@GCD@NV@@@std@@@6@@7@B@DesktopDumpHelper@GCD@NV@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `DXGI_FORMAT_P010`, `DXGI_FORMAT_P016`, `UnormInt101010`.

## helpsystem.dll

Path: `game/bin/win64/helpsystem.dll`  
SHA-256: `b10ce433baa4efb9d01ee31d64e5f28da2e93e20391cae08844321aa48704389`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x57ce8`.  
Exports: 6; RTTI types: 191; x64 unwind entries: 2031.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\tools\helpsystem\_vpc_helpsystem\default\win64\_msvc_\Retail\helpsystem.pdb`; GUID `4934e312-0797-42c3-b6cb-fd02069c97c5`; age 12; lookup key `4934E312079742C3B6CBFD02069C97C5C`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `KERNEL32.dll`, `USER32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xe240` |  |
| `CreateInterface` | `0x42540` |  |
| `ExtractModuleMetadata` | `0xe690` |  |
| `GetResourceManifestCount` | `0x57190` |  |
| `GetResourceManifests` | `0x571a0` |  |
| `InstallSchemaBindings` | `0x573f0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIHelpSystem@@@@`, `.?AV?$CBindTargetDescriptor@VCGlobalHotkeyCommands@@@@`, `.?AV?$CBindTargetDescriptor@VCQHelpEditorWidget@@@@`, `.?AV?$CDynamicWidgetFactory@VCQDraggableWidgetList@@@@`, `.?AV?$CDynamicWidgetFactory@VCQFilteredTreeWidget@@@@`, `.?AV?$CDynamicWidgetFactory@VCQTextEditSuppressShortcuts@@@@`, `.?AV?$CDynamicWidgetFactory@VCQTreeWidgetWithHover@@@@`, `.?AV?$CDynamicWidgetFactory@VQComboBox@@@@`, `.?AV?$CDynamicWidgetFactory@VQFrame@@@@`, `.?AV?$CDynamicWidgetFactory@VQGroupBox@@@@`, `.?AV?$CDynamicWidgetFactory@VQLabel@@@@`, `.?AV?$CDynamicWidgetFactory@VQLineEdit@@@@`, `.?AV?$CDynamicWidgetFactory@VQListWidget@@@@`, `.?AV?$CDynamicWidgetFactory@VQMenu@@@@`, `.?AV?$CDynamicWidgetFactory@VQProgressBar@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## imemanager.dll

Path: `game/bin/win64/imemanager.dll`  
SHA-256: `9b20d7cfcd2975bc3886527140c4f31779cea3d66a40325bf8a7b99b78629829`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x16378`.  
Exports: 6; RTTI types: 32; x64 unwind entries: 783.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\imemanager\_vpc_imemanager\default\win64\_msvc_\Retail\imemanager.pdb`; GUID `079b98ca-f95d-40d9-aa15-bca291b641e0`; age 9; lookup key `079B98CAF95D40D9AA15BCA291B641E09`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `IMM32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xcf90` |  |
| `CreateInterface` | `0xdf60` |  |
| `ExtractModuleMetadata` | `0xd3e0` |  |
| `GetResourceManifestCount` | `0x158b0` |  |
| `GetResourceManifests` | `0x158c0` |  |
| `InstallSchemaBindings` | `0x15b10` |  |

RTTI sample: `.?AUITfCompartmentEventSink@@`, `.?AUITfInputProcessorProfileActivationSink@@`, `.?AUITfThreadMgrEventSink@@`, `.?AUITfUIElementSink@@`, `.?AUIUnknown@@`, `.?AV?$CBaseAppSystem@VIIMEManager@@@@`, `.?AV?$CConCommandMemberAccessor@VCIMEManager@@@@`, `.?AV?$CConCommandMemberAccessor@VCIMENamesManager@@@@`, `.?AV?$CTier0AppSystem@VIIMEManager@@$0A@@@`, `.?AV?$CTier1AppSystem@VIIMEManager@@$0A@@@`, `.?AV?$CTier2AppSystem@VIIMEManager@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCIMEManager@@`, `.?AVCIMENamesManager@@`, `.?AVCIMENamesManagerVista@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## inputsystem.dll

Path: `game/bin/win64/inputsystem.dll`  
SHA-256: `6cc263653ac949c1a007b28a867d02276fa58aac6341f7d8f0318ce66a6f63b3`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x207a8`.  
Exports: 6; RTTI types: 20; x64 unwind entries: 849.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\inputsystem\_vpc_inputsystem\default\win64\_msvc_\Retail\inputsystem.pdb`; GUID `33e4a072-7bed-4796-bf3b-3b8e13fc5a81`; age 10; lookup key `33E4A0727BED4796BF3B3B8E13FC5A81A`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `IMM32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x16270` |  |
| `CreateInterface` | `0x16f00` |  |
| `ExtractModuleMetadata` | `0x166c0` |  |
| `GetResourceManifestCount` | `0x1fce0` |  |
| `GetResourceManifests` | `0x1fcf0` |  |
| `InstallSchemaBindings` | `0x1ff40` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIInputStackSystem@@@@`, `.?AV?$CBaseAppSystem@VIInputSystem@@@@`, `.?AV?$CTier0AppSystem@VIInputStackSystem@@$0A@@@`, `.?AV?$CTier0AppSystem@VIInputSystem@@$0A@@@`, `.?AV?$CTier1AppSystem@VIInputStackSystem@@$0A@@@`, `.?AV?$CTier1AppSystem@VIInputSystem@@$0A@@@`, `.?AV?$CTier2AppSystem@VIInputStackSystem@@$0A@@@`, `.?AV?$CTier2AppSystem@VIInputSystem@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCInputStackSystem@@`, `.?AVCInputSystem@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCSchemaInstallCallback@@`, `.?AVIAppSystem@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## libavcodec-58.dll

Path: `game/bin/win64/libavcodec-58.dll`  
SHA-256: `0984e89dce98008fcdfe82ba398ecbda8356ca9374fe40a784c5d25056558870`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x4feafc`.  
Exports: 217; RTTI types: 10; x64 unwind entries: 11398.

PDB reference: `C:\buildslave\libav_win32\build\dev\libavcodec\libavcodec-58.pdb`; GUID `591b4ad9-db26-440a-abfd-93c6c8c39b34`; age 2; lookup key `591B4AD9DB26440AABFD93C6C8C39B342`.

Normal import modules: `libavutil-56.dll`, `ole32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `av_ac3_parse_header` | `0x29fd40` |  |
| `av_adts_header_parse` | `0x2a0050` |  |
| `av_bitstream_filter_close` | `0x2a5c40` |  |
| `av_bitstream_filter_filter` | `0x2a5c70` |  |
| `av_bitstream_filter_init` | `0x2a5f10` |  |
| `av_bitstream_filter_next` | `0x2a5fa0` |  |
| `av_bsf_alloc` | `0x2a6780` |  |
| `av_bsf_flush` | `0x2a6930` |  |
| `av_bsf_free` | `0x2a6970` |  |
| `av_bsf_get_by_name` | `0x2a5ff0` |  |
| `av_bsf_get_class` | `0x2a6a10` |  |
| `av_bsf_get_null_filter` | `0x2a6a20` |  |
| `av_bsf_init` | `0x2a6a30` |  |
| `av_bsf_iterate` | `0x2a6050` |  |
| `av_bsf_list_alloc` | `0x2a6ba0` |  |
| `av_bsf_list_append` | `0x2a6bc0` |  |
| `av_bsf_list_append2` | `0x2a6bd0` |  |
| `av_bsf_list_finalize` | `0x2a6be0` |  |
| `av_bsf_list_free` | `0x2a6c60` |  |
| `av_bsf_list_parse_str` | `0x2a6d60` |  |
| `av_bsf_next` | `0x2a6070` |  |
| `av_bsf_receive_packet` | `0x2a6f30` |  |
| `av_bsf_send_packet` | `0x2a6f40` |  |
| `av_codec_ffversion` | `0x5adf60` |  |
| `av_codec_get_chroma_intra_matrix` | `0x4af120` |  |
| `av_codec_get_codec_descriptor` | `0x4af130` |  |
| `av_codec_get_codec_properties` | `0x4af140` |  |
| `av_codec_get_lowres` | `0x4af150` |  |
| `av_codec_get_max_lowres` | `0x4af160` |  |
| `av_codec_get_pkt_timebase` | `0x4af170` |  |

RTTI sample: `.?AVDNameNode@@`, `.?AVDNameStatusNode@@`, `.?AVbad_alloc@std@@`, `.?AVbad_exception@std@@`, `.?AVcharNode@@`, `.?AVexception@std@@`, `.?AVpDNameNode@@`, `.?AVpairNode@@`, `.?AVpcharNode@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`, `jpeg2000`, `smpte2084`, `smpte2085`.

## libavformat-58.dll

Path: `game/bin/win64/libavformat-58.dll`  
SHA-256: `9a7e0abf5a09e983d7f46999b6b413a31f0b70dfb8cca1072ec6b88f790c0d0d`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x5394`.  
Exports: 171; RTTI types: 10; x64 unwind entries: 4319.

PDB reference: `C:\buildslave\libav_win32\build\dev\libavformat\libavformat-58.pdb`; GUID `92fef839-7415-440f-b443-a8d94fdc3d13`; age 1; lookup key `92FEF8397415440FB443A8D94FDC3D131`.

Normal import modules: `libavcodec-58.dll`, `libavutil-56.dll`, `WS2_32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `av_add_index_entry` | `0x4985` |  |
| `av_append_packet` | `0x3008` |  |
| `av_apply_bitstream_filters` | `0x5119` |  |
| `av_codec_get_id` | `0x2c9d` |  |
| `av_codec_get_tag` | `0x1cee` |  |
| `av_codec_get_tag2` | `0x1aff` |  |
| `av_demuxer_iterate` | `0x19ec` |  |
| `av_demuxer_open` | `0x4fa2` |  |
| `av_dump_format` | `0x2419` |  |
| `av_filename_number_test` | `0x1d93` |  |
| `av_find_best_stream` | `0x13de` |  |
| `av_find_default_stream_index` | `0x455c` |  |
| `av_find_input_format` | `0x1d52` |  |
| `av_find_program_from_stream` | `0x2d9c` |  |
| `av_fmt_ctx_get_duration_estimation_method` | `0x237e` |  |
| `av_fopen_utf8` | `0x40cf` |  |
| `av_format_ffversion` | `0x180120` |  |
| `av_format_get_audio_codec` | `0x4e17` |  |
| `av_format_get_control_message_cb` | `0x381e` |  |
| `av_format_get_data_codec` | `0x476e` |  |
| `av_format_get_metadata_header_padding` | `0x268a` |  |
| `av_format_get_opaque` | `0x4d27` |  |
| `av_format_get_open_cb` | `0x2572` |  |
| `av_format_get_probe_score` | `0x1311` |  |
| `av_format_get_subtitle_codec` | `0x12f8` |  |
| `av_format_get_video_codec` | `0x254f` |  |
| `av_format_inject_global_side_data` | `0x1221` |  |
| `av_format_set_audio_codec` | `0x510a` |  |
| `av_format_set_control_message_cb` | `0x521d` |  |
| `av_format_set_data_codec` | `0x4755` |  |

RTTI sample: `.?AVDNameNode@@`, `.?AVDNameStatusNode@@`, `.?AVbad_alloc@std@@`, `.?AVbad_exception@std@@`, `.?AVcharNode@@`, `.?AVexception@std@@`, `.?AVpDNameNode@@`, `.?AVpairNode@@`, `.?AVpcharNode@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`, `mjpeg_2000`.

## libavresample-4.dll

Path: `game/bin/win64/libavresample-4.dll`  
SHA-256: `b2a72526916f00748fc8957d8186d1756c8eb2d0628102135d0244bed2773c45`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3e90`.  
Exports: 21; RTTI types: 10; x64 unwind entries: 1886.

PDB reference: `C:\buildslave\libav_win32\build\dev\libavresample\libavresample-4.pdb`; GUID `4d91deda-0862-4b28-bcc3-65f2f3dfad5d`; age 1; lookup key `4D91DEDA08624B28BCC365F2F3DFAD5D1`.

Normal import modules: `libavutil-56.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `avresample_alloc_context` | `0x3e2c` |  |
| `avresample_available` | `0x2e69` |  |
| `avresample_build_matrix` | `0x17c1` |  |
| `avresample_close` | `0x1b9f` |  |
| `avresample_config` | `0x3e0e` |  |
| `avresample_configuration` | `0x2004` |  |
| `avresample_convert` | `0x33a0` |  |
| `avresample_convert_frame` | `0x3189` |  |
| `avresample_free` | `0x2f04` |  |
| `avresample_get_class` | `0x2e9b` |  |
| `avresample_get_delay` | `0x31f2` |  |
| `avresample_get_matrix` | `0x2b3f` |  |
| `avresample_get_out_samples` | `0x1294` |  |
| `avresample_is_open` | `0x2e78` |  |
| `avresample_license` | `0x37bf` |  |
| `avresample_open` | `0x389b` |  |
| `avresample_read` | `0x202c` |  |
| `avresample_set_channel_mapping` | `0x1ce4` |  |
| `avresample_set_compensation` | `0x3639` |  |
| `avresample_set_matrix` | `0x27c0` |  |
| `avresample_version` | `0x20b8` |  |

RTTI sample: `.?AVDNameNode@@`, `.?AVDNameStatusNode@@`, `.?AVbad_alloc@std@@`, `.?AVbad_exception@std@@`, `.?AVcharNode@@`, `.?AVexception@std@@`, `.?AVpDNameNode@@`, `.?AVpairNode@@`, `.?AVpcharNode@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## libavutil-56.dll

Path: `game/bin/win64/libavutil-56.dll`  
SHA-256: `114622d5f56bc8c27d2dc01059e4ea975069643f5d0f24da585f62854191bb1b`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x566e`.  
Exports: 547; RTTI types: 10; x64 unwind entries: 3439.

PDB reference: `C:\buildslave\libav_win32\build\dev\libavutil\libavutil-56.pdb`; GUID `fd2b31c6-cb71-4773-81b8-b7c1fbf67f78`; age 1; lookup key `FD2B31C6CB71477381B8B7C1FBF67F781`.

Normal import modules: `USER32.dll`, `bcrypt.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `av_add_i` | `0x310c` |  |
| `av_add_q` | `0x2919` |  |
| `av_add_stable` | `0x4836` |  |
| `av_adler32_update` | `0x1852` |  |
| `av_aes_alloc` | `0x1253` |  |
| `av_aes_crypt` | `0x3be8` |  |
| `av_aes_ctr_alloc` | `0x2ad1` |  |
| `av_aes_ctr_crypt` | `0x41c9` |  |
| `av_aes_ctr_free` | `0x25c7` |  |
| `av_aes_ctr_get_iv` | `0x5362` |  |
| `av_aes_ctr_increment_iv` | `0x5399` |  |
| `av_aes_ctr_init` | `0x32d8` |  |
| `av_aes_ctr_set_full_iv` | `0x362a` |  |
| `av_aes_ctr_set_iv` | `0x5272` |  |
| `av_aes_ctr_set_random_iv` | `0x3053` |  |
| `av_aes_init` | `0x4b47` |  |
| `av_aes_size` | `0x123c20` |  |
| `av_append_path_component` | `0x3837` |  |
| `av_asprintf` | `0x3c42` |  |
| `av_assert0_fpu` | `0x32c9` |  |
| `av_audio_fifo_alloc` | `0x1f2d` |  |
| `av_audio_fifo_drain` | `0x4de5` |  |
| `av_audio_fifo_free` | `0x2685` |  |
| `av_audio_fifo_peek` | `0x21b7` |  |
| `av_audio_fifo_peek_at` | `0x4633` |  |
| `av_audio_fifo_read` | `0x3062` |  |
| `av_audio_fifo_realloc` | `0x2392` |  |
| `av_audio_fifo_reset` | `0x4a75` |  |
| `av_audio_fifo_size` | `0x12f8` |  |
| `av_audio_fifo_space` | `0x30a8` |  |

RTTI sample: `.?AVDNameNode@@`, `.?AVDNameStatusNode@@`, `.?AVbad_alloc@std@@`, `.?AVbad_exception@std@@`, `.?AVcharNode@@`, `.?AVexception@std@@`, `.?AVpDNameNode@@`, `.?AVpairNode@@`, `.?AVpcharNode@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`, `smpte2084`, `smpte2085`.

## libfbxsdk_2020_3_1.dll

Path: `game/bin/win64/libfbxsdk_2020_3_1.dll`  
SHA-256: `474abba2d946573bbd32f39baf39a295c6fd6cd6a4a58825651a7bd2cd8fed3f`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x5de210`.  
Exports: 11378; RTTI types: 392; x64 unwind entries: 27394.

PDB reference: `D:\Stage\pipeline\master\build\vc142_x64_dll\RelWithDebInfo\libfbxsdk.pdb`; GUID `abc53aed-7cac-4bb5-9c60-9bfa069837d5`; age 2; lookup key `ABC53AED7CAC4BB59C609BFA069837D52`.

Normal import modules: `KERNEL32.dll`, `ADVAPI32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??$EvaluateValue@C@FbxProperty@fbxsdk@@QEAACAEBVFbxTime@1@_N@Z` | `0x608a0` |  |
| `??$EvaluateValue@E@FbxProperty@fbxsdk@@QEAAEAEBVFbxTime@1@_N@Z` | `0x60940` |  |
| `??$EvaluateValue@F@FbxProperty@fbxsdk@@QEAAFAEBVFbxTime@1@_N@Z` | `0x609e0` |  |
| `??$EvaluateValue@G@FbxProperty@fbxsdk@@QEAAGAEBVFbxTime@1@_N@Z` | `0x60a80` |  |
| `??$EvaluateValue@H@FbxProperty@fbxsdk@@QEAAHAEBVFbxTime@1@_N@Z` | `0x60b20` |  |
| `??$EvaluateValue@I@FbxProperty@fbxsdk@@QEAAIAEBVFbxTime@1@_N@Z` | `0x60bc0` |  |
| `??$EvaluateValue@M@FbxProperty@fbxsdk@@QEAAMAEBVFbxTime@1@_N@Z` | `0x60c60` |  |
| `??$EvaluateValue@N@FbxProperty@fbxsdk@@QEAANAEBVFbxTime@1@_N@Z` | `0x60d00` |  |
| `??$EvaluateValue@V?$FbxVectorTemplate2@N@fbxsdk@@@FbxProperty@fbxsdk@@QEAA?AV?$FbxVectorTemplate2@N@1@AEBVFbxTime@1@_N@Z` | `0x60da0` |  |
| `??$EvaluateValue@V?$FbxVectorTemplate3@N@fbxsdk@@@FbxProperty@fbxsdk@@QEAA?AV?$FbxVectorTemplate3@N@1@AEBVFbxTime@1@_N@Z` | `0x60e30` |  |
| `??$EvaluateValue@V?$FbxVectorTemplate4@N@fbxsdk@@@FbxProperty@fbxsdk@@QEAA?AV?$FbxVectorTemplate4@N@1@AEBVFbxTime@1@_N@Z` | `0x60ed0` |  |
| `??$EvaluateValue@V?$FbxVectorTemplate4@V?$FbxVectorTemplate4@N@fbxsdk@@@fbxsdk@@@FbxProperty@fbxsdk@@QEAA?AV?$FbxVectorTemplate4@V?$FbxVectorTemplate4@N@fbxsdk@@@1@AEBVFbxTime@1@_N@Z` | `0x60f70` |  |
| `??$EvaluateValue@VFbxHalfFloat@fbxsdk@@@FbxProperty@fbxsdk@@QEAA?AVFbxHalfFloat@1@AEBVFbxTime@1@_N@Z` | `0x61020` |  |
| `??$EvaluateValue@_J@FbxProperty@fbxsdk@@QEAA_JAEBVFbxTime@1@_N@Z` | `0x610d0` |  |
| `??$EvaluateValue@_K@FbxProperty@fbxsdk@@QEAA_KAEBVFbxTime@1@_N@Z` | `0x61170` |  |
| `??$EvaluateValue@_N@FbxProperty@fbxsdk@@QEAA_NAEBVFbxTime@1@_N@Z` | `0x61210` |  |
| `??0?$FbxArray@C$0BA@@fbxsdk@@QEAA@AEBV01@@Z` | `0x756b0` |  |
| `??0?$FbxArray@C$0BA@@fbxsdk@@QEAA@H@Z` | `0x75790` |  |
| `??0?$FbxArray@C$0BA@@fbxsdk@@QEAA@XZ` | `0x75800` |  |
| `??0?$FbxArray@E$0BA@@fbxsdk@@QEAA@AEBV01@@Z` | `0x75810` |  |
| `??0?$FbxArray@E$0BA@@fbxsdk@@QEAA@H@Z` | `0x758f0` |  |
| `??0?$FbxArray@E$0BA@@fbxsdk@@QEAA@XZ` | `0x75960` |  |
| `??0?$FbxArray@F$0BA@@fbxsdk@@QEAA@AEBV01@@Z` | `0x75970` |  |
| `??0?$FbxArray@F$0BA@@fbxsdk@@QEAA@H@Z` | `0x75a50` |  |
| `??0?$FbxArray@F$0BA@@fbxsdk@@QEAA@XZ` | `0x75ad0` |  |
| `??0?$FbxArray@G$0BA@@fbxsdk@@QEAA@AEBV01@@Z` | `0x75ae0` |  |
| `??0?$FbxArray@G$0BA@@fbxsdk@@QEAA@H@Z` | `0x75bc0` |  |
| `??0?$FbxArray@G$0BA@@fbxsdk@@QEAA@XZ` | `0x75c40` |  |
| `??0?$FbxArray@H$0BA@@fbxsdk@@QEAA@AEBV01@@Z` | `0x21a70` |  |
| `??0?$FbxArray@H$0BA@@fbxsdk@@QEAA@H@Z` | `0x75c50` |  |

RTTI sample: `.?AU?$TArrayDeleter@C@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@E@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@F@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@G@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@H@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@I@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@M@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@N@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@V?$Vec3@M@fbxsdk_Imath_2_5@@@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@V?$basic_string@_WU?$char_traits@_W@std@@V?$allocator@_W@2@@std@@@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@V__alembic_openexr_half_@@@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@Vbool_t@fbxsdk_v12@Util@Alembic@@@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@_J@fbxsdk_v12@AbcCoreAbstract@Alembic@@`, `.?AU?$TArrayDeleter@_K@fbxsdk_v12@AbcCoreAbstract@Alembic@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `FBX201000`, `FBX202000`, `Frames1000`, `SystemFunction036`.

## libfontconfig-1.dll

Path: `game/bin/win64/libfontconfig-1.dll`  
SHA-256: `209507f621bf5cd09422a4d9cf2a027af0f7aa7cf7c9dc82fd98ef4b86b4bc46`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3b5c4`.  
Exports: 196; RTTI types: 0; x64 unwind entries: 1759.

PDB reference: `c:\buildslave\pango_win32\build\fontconfig-2.11.1\x64\Release\libfontconfig-1.pdb`; GUID `6fe4b77f-7529-4b94-bb71-75e1bf260f53`; age 1; lookup key `6FE4B77F75294B94BB7175E1BF260F531`.

Normal import modules: `libfreetype-6.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `FcAtomicCreate` | `0x16a30` |  |
| `FcAtomicDeleteNew` | `0x16b70` |  |
| `FcAtomicDestroy` | `0x30930` |  |
| `FcAtomicLock` | `0x16b80` |  |
| `FcAtomicNewFile` | `0x16c30` |  |
| `FcAtomicOrigFile` | `0x16c40` |  |
| `FcAtomicReplaceOrig` | `0x16c50` |  |
| `FcAtomicUnlock` | `0x16c80` |  |
| `FcBlanksAdd` | `0x16c90` |  |
| `FcBlanksCreate` | `0x295e0` |  |
| `FcBlanksDestroy` | `0x16d40` |  |
| `FcBlanksIsMember` | `0x16d70` |  |
| `FcCacheCopySet` | `0x16da0` |  |
| `FcCacheCreateTagFile` | `0x16e50` |  |
| `FcCacheDir` | `0x17030` |  |
| `FcCacheNumFont` | `0x17430` |  |
| `FcCacheNumSubdir` | `0x17440` |  |
| `FcCacheSubdir` | `0x17600` |  |
| `FcCharSetAddChar` | `0x1c9d0` |  |
| `FcCharSetCopy` | `0x1ca20` |  |
| `FcCharSetCount` | `0x1ca60` |  |
| `FcCharSetCoverage` | `0x1cb70` |  |
| `FcCharSetCreate` | `0x1cc20` |  |
| `FcCharSetDestroy` | `0x1cc50` |  |
| `FcCharSetEqual` | `0x1ccf0` |  |
| `FcCharSetFirstPage` | `0x1d030` |  |
| `FcCharSetHasChar` | `0x1d560` |  |
| `FcCharSetIntersect` | `0x1d740` |  |
| `FcCharSetIntersectCount` | `0x1d760` |  |
| `FcCharSetIsSubset` | `0x1da60` |  |

## libfreetype-6.dll

Path: `game/bin/win64/libfreetype-6.dll`  
SHA-256: `bdfb01f8ecf2f5af6014bd4eb4ea733a24f1de6ec6167bc504b8eb088d6b5822`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x7ba38`.  
Exports: 278; RTTI types: 0; x64 unwind entries: 2880.

PDB reference: `c:\buildslave\pango_win32\build\freetype-2.5.3\builds\windows\vc2010\x64\Release\libfreetype-6.pdb`; GUID `7ddae394-ebfd-408c-9a1c-a06b9c270cb7`; age 1; lookup key `7DDAE394EBFD408C9A1CA06B9C270CB71`.

Normal import modules: `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `FTC_CMapCache_Lookup` | `0x142e` |  |
| `FTC_CMapCache_New` | `0x1474` |  |
| `FTC_ImageCache_Lookup` | `0x12da` |  |
| `FTC_ImageCache_LookupScaler` | `0x137a` |  |
| `FTC_ImageCache_New` | `0x13bb` |  |
| `FTC_Manager_Done` | `0x14ce` |  |
| `FTC_Manager_LookupFace` | `0x13fc` |  |
| `FTC_Manager_LookupSize` | `0x145b` |  |
| `FTC_Manager_New` | `0x133e` |  |
| `FTC_Manager_RemoveFaceID` | `0x1046` |  |
| `FTC_Manager_Reset` | `0x1230` |  |
| `FTC_Node_Unref` | `0x12cb` |  |
| `FTC_SBitCache_Lookup` | `0x150f` |  |
| `FTC_SBitCache_LookupScaler` | `0x11c2` |  |
| `FTC_SBitCache_New` | `0x10f5` |  |
| `FT_Activate_Size` | `0x136b` |  |
| `FT_Add_Default_Modules` | `0x12d0` |  |
| `FT_Add_Module` | `0x1113` |  |
| `FT_Angle_Diff` | `0x128f` |  |
| `FT_Atan2` | `0x12e4` |  |
| `FT_Attach_File` | `0x13b1` |  |
| `FT_Attach_Stream` | `0x149c` |  |
| `FT_Bitmap_Convert` | `0x11fe` |  |
| `FT_Bitmap_Copy` | `0x1195` |  |
| `FT_Bitmap_Done` | `0x1177` |  |
| `FT_Bitmap_Embolden` | `0x1550` |  |
| `FT_Bitmap_New` | `0x1019` |  |
| `FT_CMap_Done` | `0x121c` |  |
| `FT_CMap_New` | `0x1311` |  |
| `FT_CeilFix` | `0x1217` |  |

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## libglib-2.0-0.dll

Path: `game/bin/win64/libglib-2.0-0.dll`  
SHA-256: `a23990999e3f5ae299e104b01d70fcf0a035064058dbcc18e6f3ba2ce0431013`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xbecf8`.  
Exports: 1609; RTTI types: 0; x64 unwind entries: 4638.

PDB reference: `c:\buildslave\pango_win32\build\glib-2.42.0\build\win32\vs12\Release\x64\bin\libglib-2.0-0.pdb`; GUID `e15e6669-dc1c-4581-94aa-b37f0dc42392`; age 1; lookup key `E15E6669DC1C458194AAB37F0DC423921`.

Normal import modules: `WS2_32.dll`, `WINMM.dll`, `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`, `SHELL32.dll`, `ole32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `__glib_assert_msg` | `0x16be90` |  |
| `g_access` | `0x5b200` |  |
| `g_allocator_free` | `0x11e0` |  |
| `g_allocator_new` | `0x11d0` |  |
| `g_array_append_vals` | `0x49f0` |  |
| `g_array_free` | `0x48f0` |  |
| `g_array_get_element_size` | `0x48a0` |  |
| `g_array_insert_vals` | `0x4b70` |  |
| `g_array_new` | `0x4620` |  |
| `g_array_prepend_vals` | `0x4aa0` |  |
| `g_array_ref` | `0x47b0` |  |
| `g_array_remove_index` | `0x4d10` |  |
| `g_array_remove_index_fast` | `0x4df0` |  |
| `g_array_remove_range` | `0x4ec0` |  |
| `g_array_set_clear_func` | `0x4760` |  |
| `g_array_set_size` | `0x4c50` |  |
| `g_array_sized_new` | `0x4670` |  |
| `g_array_sort` | `0x5020` |  |
| `g_array_sort_with_data` | `0x5080` |  |
| `g_array_unref` | `0x4800` |  |
| `g_ascii_digit_value` | `0x5ed00` |  |
| `g_ascii_dtostr` | `0x5e0f0` |  |
| `g_ascii_formatd` | `0x5e100` |  |
| `g_ascii_strcasecmp` | `0x5ed60` |  |
| `g_ascii_strdown` | `0x5e9c0` |  |
| `g_ascii_strncasecmp` | `0x5ee20` |  |
| `g_ascii_strtod` | `0x5dc80` |  |
| `g_ascii_strtoll` | `0x5e620` |  |
| `g_ascii_strtoull` | `0x5e600` |  |
| `g_ascii_strup` | `0x5ea60` |  |

Interface-like string sample (unconfirmed; shared constants and false positives included): `CP12000`, `CP12001`, `CP65001`, `GB18030`, `IBM01047`, `IBM1026`, `SystemFunction036`.

## libgmodule-2.0-0.dll

Path: `game/bin/win64/libgmodule-2.0-0.dll`  
SHA-256: `351d05acc97d19ea348a69a844b3d959a02ed6189357b8d8d65eb70b7f68e537`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x2414`.  
Exports: 8; RTTI types: 0; x64 unwind entries: 208.

PDB reference: `c:\buildslave\pango_win32\build\glib-2.42.0\build\win32\vs12\Release\x64\bin\libgmodule-2.0-0.pdb`; GUID `5e1426ff-713b-4729-977d-e7e83f7ead25`; age 1; lookup key `5E1426FF713B4729977DE7E83F7EAD251`.

Normal import modules: `KERNEL32.dll`, `libglib-2.0-0.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `g_module_build_path` | `0x1f90` |  |
| `g_module_close` | `0x1bf0` |  |
| `g_module_error` | `0x1d90` |  |
| `g_module_make_resident` | `0x1d60` |  |
| `g_module_name_utf8` | `0x1f40` |  |
| `g_module_open_utf8` | `0x16b0` |  |
| `g_module_supported` | `0x1310` |  |
| `g_module_symbol` | `0x1da0` |  |

## libgobject-2.0-0.dll

Path: `game/bin/win64/libgobject-2.0-0.dll`  
SHA-256: `166ce8496289ebe41cd624e24ba99a5856402141d1f095359314d853581ab22a`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x27ca0`.  
Exports: 428; RTTI types: 0; x64 unwind entries: 1274.

PDB reference: `c:\buildslave\pango_win32\build\glib-2.42.0\build\win32\vs12\Release\x64\bin\libgobject-2.0-0.pdb`; GUID `ad775095-48cb-4d96-86ff-30f4f78bd697`; age 1; lookup key `AD77509548CB4D9686FF30F4F78BD6971`.

Normal import modules: `libglib-2.0-0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `g_array_get_type` | `0x34b0` |  |
| `g_binding_flags_get_type` | `0x2250` |  |
| `g_binding_get_flags` | `0x22a0` |  |
| `g_binding_get_source` | `0x2460` |  |
| `g_binding_get_source_property` | `0x2510` |  |
| `g_binding_get_target` | `0x25c0` |  |
| `g_binding_get_target_property` | `0x2670` |  |
| `g_binding_get_type` | `0x2720` |  |
| `g_binding_unbind` | `0x28c0` |  |
| `g_boxed_copy` | `0x3510` |  |
| `g_boxed_free` | `0x3680` |  |
| `g_boxed_type_register_static` | `0x3790` |  |
| `g_byte_array_get_type` | `0x3880` |  |
| `g_bytes_get_type` | `0x38e0` |  |
| `g_cclosure_marshal_BOOLEAN__BOXED_BOXED` | `0x7790` |  |
| `g_cclosure_marshal_BOOLEAN__BOXED_BOXEDv` | `0x7850` |  |
| `g_cclosure_marshal_BOOLEAN__FLAGS` | `0x7970` |  |
| `g_cclosure_marshal_BOOLEAN__FLAGSv` | `0x7a30` |  |
| `g_cclosure_marshal_STRING__OBJECT_POINTER` | `0x7aa0` |  |
| `g_cclosure_marshal_STRING__OBJECT_POINTERv` | `0x7b60` |  |
| `g_cclosure_marshal_VOID__BOOLEAN` | `0x7c20` |  |
| `g_cclosure_marshal_VOID__BOOLEANv` | `0x8240` |  |
| `g_cclosure_marshal_VOID__BOXED` | `0x7cb0` |  |
| `g_cclosure_marshal_VOID__BOXEDv` | `0x7d40` |  |
| `g_cclosure_marshal_VOID__CHAR` | `0x7de0` |  |
| `g_cclosure_marshal_VOID__CHARv` | `0x8700` |  |
| `g_cclosure_marshal_VOID__DOUBLE` | `0x7e70` |  |
| `g_cclosure_marshal_VOID__DOUBLEv` | `0x7f00` |  |
| `g_cclosure_marshal_VOID__ENUM` | `0x7f30` |  |
| `g_cclosure_marshal_VOID__ENUMv` | `0x8240` |  |

## libgthread-2.0-0.dll

Path: `game/bin/win64/libgthread-2.0-0.dll`  
SHA-256: `6dea26270e891756ca78422c289cf77c6780194e541598ff91c3b73c124c249c`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1210`.  
Exports: 2; RTTI types: 0; x64 unwind entries: 186.

PDB reference: `c:\buildslave\pango_win32\build\glib-2.42.0\build\win32\vs12\Release\x64\bin\libgthread-2.0-0.pdb`; GUID `a94402a4-038e-4266-881f-f90496fadbdf`; age 1; lookup key `A94402A4038E4266881FF90496FADBDF1`.

Normal import modules: `libglib-2.0-0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `g_thread_init` | `0x1000` |  |
| `g_thread_init_with_errorcheck_mutexes` | `0x1020` |  |

## libmpg123-0.dll

Path: `game/bin/win64/libmpg123-0.dll`  
SHA-256: `24e71004d29377b42a5145acdac4195da073a3a0b8e96d9a37b286fb09b2cba7`  
Architecture: PE32+ x86-64; preferred base `0x63c80000`; entry RVA `0x1330`.  
Exports: 136; RTTI types: 0; x64 unwind entries: 436.

Normal import modules: `KERNEL32.dll`, `msvcrt.dll`, `SHLWAPI.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `mpg123_add_string` | `0x1b130` |  |
| `mpg123_add_substring` | `0x1b220` |  |
| `mpg123_chomp_string` | `0x1b7f0` |  |
| `mpg123_clip` | `0x18e70` |  |
| `mpg123_close` | `0x1a750` |  |
| `mpg123_copy_string` | `0x1b0a0` |  |
| `mpg123_current_decoder` | `0x10da0` |  |
| `mpg123_decode` | `0x187a0` |  |
| `mpg123_decode_frame` | `0x3ece0` |  |
| `mpg123_decode_frame_32` | `0x3faa0` |  |
| `mpg123_decode_frame_64` | `0x17890` |  |
| `mpg123_decoder` | `0x15ea0` |  |
| `mpg123_decoders` | `0x10dd0` |  |
| `mpg123_delete` | `0x1a7b0` |  |
| `mpg123_delete_pars` | `0x49d0` |  |
| `mpg123_enc_from_id3` | `0x1a550` |  |
| `mpg123_encodings` | `0x7ac0` |  |
| `mpg123_encsize` | `0x7ae0` |  |
| `mpg123_eq` | `0x16720` |  |
| `mpg123_errcode` | `0x1a870` |  |
| `mpg123_exit` | `0x15d70` |  |
| `mpg123_feature` | `0x3ead0` |  |
| `mpg123_feed` | `0x18730` |  |
| `mpg123_feedseek` | `0x3eec0` |  |
| `mpg123_feedseek_32` | `0x3fb10` |  |
| `mpg123_feedseek_64` | `0x19330` |  |
| `mpg123_fmt` | `0x8e50` |  |
| `mpg123_fmt_all` | `0x8b60` |  |
| `mpg123_fmt_none` | `0x87f0` |  |
| `mpg123_fmt_support` | `0x9630` |  |

COFF symbol names: `__imp_abort`, `__lib64_libkernel32_a_iname`, `__imp__stricmp`, `__imp__lock`, `_head_lib64_libshlwapi_a`, `__imp_RtlVirtualUnwind`, `__imp_calloc`, `__imp_LoadLibraryW`, `__imp__strnicmp`, `__imp_DeleteCriticalSection`, `__imp_GetFileAttributesW`, `__imp_LocalAlloc`, `__imp__amsg_exit`, `__imp__errno`, `__imp_FindClose`, `__imp_strncpy`, `__imp__open`, `__imp_GetCurrentThreadId`, `__imp_read`, `__imp_MultiByteToWideChar`, `__imp_GetLastError`, `__imp_fdopen`, `__imp_free`, `__imp__wopen`, `__imp_RtlLookupFunctionEntry`, `__imp_LeaveCriticalSection`, `__imp_wcsncmp`, `__imp_PathCombineW`, `__imp__wfopen`, `__imp_GetTickCount`, `__imp_GetSystemTimeAsFileTime`, `__imp_PathIsUNCW`, `__imp_write`, `__imp_memset`, `__IAT_end__`, `__imp_memcpy`, `__imp_FindNextFileW`, `__imp_FindFirstFileW`, `__imp_memmove`, `__imp_RtlAddFunctionTable`, `__imp_strerror`, `__imp_fwrite`, `_head_lib64_libkernel32_a`, `__imp_EnterCriticalSection`, `__imp_GetCurrentProcessId`, `__imp_TerminateProcess`, `__imp_GetProcAddress`, `__lib64_libmsvcrt_os_a_iname`, `__imp_sscanf`, `__imp_signal`, `__imp_GetModuleHandleA`, `__imp_QueryPerformanceCounter`, `__imp_WideCharToMultiByte`, `__imp_strlen`, `__imp_atof`, `__imp_malloc`, `__imp_InitializeCriticalSection`, `__imp_realloc`, `__imp_vfprintf`, `__IAT_start__`, `__imp_UnhandledExceptionFilter`, `__imp_SetUnhandledExceptionFilter`, `__imp__wgetenv`, `__lib64_libshlwapi_a_iname`, `__imp_Sleep`, `__imp__close`, `__imp___setusermatherr`, `__imp_TlsGetValue`, `__imp_PathIsRelativeW`, `__imp_fprintf`, `__imp_FreeLibrary`, `__imp_VirtualProtect`, `__imp_VirtualQuery`, `__imp__initterm`, `__imp_fclose`, `__imp___iob_func`, `__imp_strncmp`, `_head_lib64_libmsvcrt_os_a`, `__imp_close`, `__imp_wcslen`, `__imp__unlock`, `__imp__lseeki64`, `__imp_RtlCaptureContext`, `__imp_fopen`, `__imp_LocalFree`, `__imp_GetCurrentProcess`.

## libpango-1.0-0.dll

Path: `game/bin/win64/libpango-1.0-0.dll`  
SHA-256: `0c2971d0cf26cb70021d03c7a1fca8555f3c4b68a8b9e5d5e28508c7d53a7448`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x273a8`.  
Exports: 405; RTTI types: 0; x64 unwind entries: 1208.

PDB reference: `c:\buildslave\pango_win32\build\pango-1.36.8\build\win32\vs12\Release\x64\bin\libpango-1.0-0.pdb`; GUID `a8277586-b680-4765-b0c6-fa4c4a2af2a6`; age 1; lookup key `A8277586B6804765B0C6FA4C4A2AF2A61`.

Normal import modules: `libgmodule-2.0-0.dll`, `libgobject-2.0-0.dll`, `libglib-2.0-0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `pango_alignment_get_type` | `0x24bb0` |  |
| `pango_attr_background_new` | `0x9570` |  |
| `pango_attr_fallback_new` | `0x9fa0` |  |
| `pango_attr_family_new` | `0x9260` |  |
| `pango_attr_font_desc_new` | `0x9bd0` |  |
| `pango_attr_foreground_new` | `0x94e0` |  |
| `pango_attr_gravity_hint_new` | `0xa370` |  |
| `pango_attr_gravity_new` | `0xa2d0` |  |
| `pango_attr_iterator_copy` | `0xaf60` |  |
| `pango_attr_iterator_destroy` | `0xafd0` |  |
| `pango_attr_iterator_get` | `0xb020` |  |
| `pango_attr_iterator_get_attrs` | `0xb4b0` |  |
| `pango_attr_iterator_get_font` | `0xb080` |  |
| `pango_attr_iterator_next` | `0xae20` |  |
| `pango_attr_iterator_range` | `0xadd0` |  |
| `pango_attr_language_new` | `0x9360` |  |
| `pango_attr_letter_spacing_new` | `0xa020` |  |
| `pango_attr_list_change` | `0xa8c0` |  |
| `pango_attr_list_copy` | `0xa510` |  |
| `pango_attr_list_filter` | `0xb3b0` |  |
| `pango_attr_list_get_iterator` | `0xad50` |  |
| `pango_attr_list_get_type` | `0xa3f0` |  |
| `pango_attr_list_insert` | `0xa5f0` |  |
| `pango_attr_list_insert_before` | `0xa760` |  |
| `pango_attr_list_new` | `0xa450` |  |
| `pango_attr_list_ref` | `0x63c0` |  |
| `pango_attr_list_splice` | `0xab70` |  |
| `pango_attr_list_unref` | `0xa480` |  |
| `pango_attr_rise_new` | `0x9ea0` |  |
| `pango_attr_scale_new` | `0x9f20` |  |

## libpangoft2-1.0-0.dll

Path: `game/bin/win64/libpangoft2-1.0-0.dll`  
SHA-256: `4d7f3151cbcdffccad1b68c14a463dfeb19f78893bc74b1e104a04eb987082ff`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x2f940`.  
Exports: 85; RTTI types: 0; x64 unwind entries: 1339.

PDB reference: `c:\buildslave\pango_win32\build\pango-1.36.8\build\win32\vs12\Release_FC\x64\bin\libpangoft2-1.0-0.pdb`; GUID `20bd1243-b55f-441a-97cf-2724dfb19fba`; age 1; lookup key `20BD1243B55F441A97CF2724DFB19FBA1`.

Normal import modules: `libfontconfig-1.dll`, `libfreetype-6.dll`, `USP10.dll`, `RPCRT4.dll`, `libgobject-2.0-0.dll`, `libglib-2.0-0.dll`, `KERNEL32.dll`, `USER32.dll`, `GDI32.dll`, `libpango-1.0-0.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `pango_fc_decoder_get_charset` | `0x6db0` |  |
| `pango_fc_decoder_get_glyph` | `0x6e80` |  |
| `pango_fc_decoder_get_type` | `0x6d30` |  |
| `pango_fc_font_create_base_metrics_for_context` | `0x2600` |  |
| `pango_fc_font_description_from_pattern` | `0x5d70` |  |
| `pango_fc_font_get_glyph` | `0x2d70` |  |
| `pango_fc_font_get_raw_extents` | `0x30f0` |  |
| `pango_fc_font_get_type` | `0x1ab0` |  |
| `pango_fc_font_get_unknown_glyph` | `0x2db0` |  |
| `pango_fc_font_has_char` | `0x2c50` |  |
| `pango_fc_font_kern_glyphs` | `0x2dd0` |  |
| `pango_fc_font_key_get_context_key` | `0x3890` |  |
| `pango_fc_font_key_get_matrix` | `0x3880` |  |
| `pango_fc_font_key_get_pattern` | `0x3740` |  |
| `pango_fc_font_lock_face` | `0x2ab0` |  |
| `pango_fc_font_map_add_decoder_find_func` | `0x4390` |  |
| `pango_fc_font_map_cache_clear` | `0x5620` |  |
| `pango_fc_font_map_create_context` | `0x5980` |  |
| `pango_fc_font_map_find_decoder` | `0x44a0` |  |
| `pango_fc_font_map_get_type` | `0x4050` |  |
| `pango_fc_font_map_shutdown` | `0x5b00` |  |
| `pango_fc_font_unlock_face` | `0x2b80` |  |
| `pango_fc_fontset_key_get_absolute_size` | `0x3770` |  |
| `pango_fc_fontset_key_get_context_key` | `0x3790` |  |
| `pango_fc_fontset_key_get_description` | `0x3750` |  |
| `pango_fc_fontset_key_get_language` | `0x3740` |  |
| `pango_fc_fontset_key_get_matrix` | `0x3760` |  |
| `pango_fc_fontset_key_get_resolution` | `0x3780` |  |
| `pango_ft2_font_get_coverage` | `0x7990` |  |
| `pango_ft2_font_get_face` | `0x72c0` |  |

## libswscale-5.dll

Path: `game/bin/win64/libswscale-5.dll`  
SHA-256: `0ef8ffe68f41957b36a3f0390e8fb13e400b41cc7e0f63c2bb43d18ab8c5419c`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3f85`.  
Exports: 34; RTTI types: 10; x64 unwind entries: 3195.

PDB reference: `C:\buildslave\libav_win32\build\dev\libswscale\libswscale-5.pdb`; GUID `118db531-158e-4ce6-b5a9-88451c3a458f`; age 1; lookup key `118DB531158E4CE6B5A988451C3A458F1`.

Normal import modules: `libavutil-56.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `sws_addVec` | `0x33c8` |  |
| `sws_allocVec` | `0x272f` |  |
| `sws_alloc_context` | `0x1c3f` |  |
| `sws_alloc_set_opts` | `0x17f8` |  |
| `sws_cloneVec` | `0x3db4` |  |
| `sws_convVec` | `0x16cc` |  |
| `sws_convertPalette8ToPacked24` | `0x3ad0` |  |
| `sws_convertPalette8ToPacked32` | `0x2cd4` |  |
| `sws_freeContext` | `0x141a` |  |
| `sws_freeFilter` | `0x3e7c` |  |
| `sws_freeVec` | `0x3a12` |  |
| `sws_getCachedContext` | `0x1a3c` |  |
| `sws_getCoefficients` | `0x1618` |  |
| `sws_getColorspaceDetails` | `0x290f` |  |
| `sws_getConstVec` | `0x37d3` |  |
| `sws_getContext` | `0x3193` |  |
| `sws_getDefaultFilter` | `0x2d65` |  |
| `sws_getGaussianVec` | `0x31b1` |  |
| `sws_getIdentityVec` | `0x34c2` |  |
| `sws_get_class` | `0x3c4c` |  |
| `sws_init_context` | `0x1492` |  |
| `sws_isSupportedEndiannessConversion` | `0x2338` |  |
| `sws_isSupportedInput` | `0x27d9` |  |
| `sws_isSupportedOutput` | `0x19f1` |  |
| `sws_normalizeVec` | `0x2c57` |  |
| `sws_printVec2` | `0x22ed` |  |
| `sws_scale` | `0x26d5` |  |
| `sws_scaleVec` | `0x1e24` |  |
| `sws_setColorspaceDetails` | `0x15f0` |  |
| `sws_shiftVec` | `0x1622` |  |

RTTI sample: `.?AVDNameNode@@`, `.?AVDNameStatusNode@@`, `.?AVbad_alloc@std@@`, `.?AVbad_exception@std@@`, `.?AVcharNode@@`, `.?AVexception@std@@`, `.?AVpDNameNode@@`, `.?AVpairNode@@`, `.?AVpcharNode@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## localize.dll

Path: `game/bin/win64/localize.dll`  
SHA-256: `d1fd9c6660afbd2c07d002fed251bca5e38bde110ed306e7e7e81a3bf37d7827`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x31c2c`.  
Exports: 8; RTTI types: 37; x64 unwind entries: 1167.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\localize\_vpc_localize\default\win64\_msvc_\Retail\localize.pdb`; GUID `9f373683-a487-4fe2-8322-1f30ad76de6f`; age 9; lookup key `9F373683A4874FE283221F30AD76DE6F9`.

Normal import modules: `steam_api64.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x285a0` |  |
| `CreateInterface` | `0x30900` |  |
| `ExtractModuleMetadata` | `0x289f0` |  |
| `GetResourceManifestCount` | `0x311f0` |  |
| `GetResourceManifests` | `0x31200` |  |
| `InstallSchemaBindings` | `0x31450` |  |
| `Localize_Create` | `0x267b0` |  |
| `Localize_EconProvider_Create` | `0x26a60` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VILocalize@@@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted1@VILocalizeCompiledString@@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AV?$CTier0AppSystem@VILocalize@@$0A@@@`, `.?AV?$CTier1AppSystem@VILocalize@@$0A@@@`, `.?AV?$CTier2AppSystem@VILocalize@@$0A@@@`, `.?AVCAssertErrorListener@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCLocVariableSet@@`, `.?AVCLocVariableSetBase@@`, `.?AVCLocalizationEngineParseNode@@`, `.?AVCLocalizationEngineParseNode_GenderSelector@@`, `.?AVCLocalizationEngineParseNode_PluralSelector@@`, `.?AVCLocalizationEngineParseNode_SelectorBase@@`, `.?AVCLocalizationEngineParseNode_StringSelector@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## materialsystem2.dll

Path: `game/bin/win64/materialsystem2.dll`  
SHA-256: `c256e7a296c68ba742dee6126bc59d967ac4b2060ce20ef63a7a09f9af9ca78e`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xdc558`.  
Exports: 6; RTTI types: 102; x64 unwind entries: 4122.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\materialsystem2\_vpc_materialsystem2\default\win64\_msvc_\Retail\materialsystem2.pdb`; GUID `ad3daa8c-93e5-4ba3-a847-a80a57add6d1`; age 10; lookup key `AD3DAA8C93E54BA3A847A80A57ADD6D1A`.

Normal import modules: `GDI32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x46e20` |  |
| `CreateInterface` | `0x99200` |  |
| `ExtractModuleMetadata` | `0x47270` |  |
| `GetResourceManifestCount` | `0x99980` |  |
| `GetResourceManifests` | `0x99990` |  |
| `InstallSchemaBindings` | `0x99cc0` |  |

RTTI sample: `.?AUAsyncProcessResult@CBaseIntrospectedToKV3TypeManager@@`, `.?AUAsyncStaticComboResult_t@CMaterial2@@`, `.?AV?$CBaseAppSystem@VIFontManager@@@@`, `.?AV?$CBaseAppSystem@VIMaterialSystem2@@@@`, `.?AV?$CBaseAppSystem@VIMaterialSystem2Utils@@@@`, `.?AV?$CBaseAppSystem@VIPostProcessingSystem@@@@`, `.?AV?$CBaseAppSystem@VITextLayout@@@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1??VfxDynamicShaderCompile@@YAXAEAV?$CUtlVector@UVfxDynamicShaderCompileTask_t@@HV?$CUtlVectorMemory_Growable@UVfxDynamicShaderCompileTask_t@@H$0A@@@@@PEAVIVfx@@PEAVIRuntimeShaderCreateCallbacks@@P6AXH_NH@Z@Z@@CParallelProcessLauncher@@`, `.?AV?$CTier0AppSystem@VIFontManager@@$0A@@@`, `.?AV?$CTier0AppSystem@VIMaterialSystem2@@$0A@@@`, `.?AV?$CTier0AppSystem@VIMaterialSystem2Utils@@$0A@@@`, `.?AV?$CTier0AppSystem@VIPostProcessingSystem@@$0A@@@`, `.?AV?$CTier0AppSystem@VITextLayout@@$0A@@@`, `.?AV?$CTier1AppSystem@VIFontManager@@$0A@@@`, `.?AV?$CTier1AppSystem@VIMaterialSystem2@@$0A@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## meshsystem.dll

Path: `game/bin/win64/meshsystem.dll`  
SHA-256: `a2ee7346f4b9a7ca75df18f68b8bfbfaed6a90b8dd0152f2318d9d22b144c04b`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x106dc0`.  
Exports: 6; RTTI types: 127; x64 unwind entries: 5583.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\meshsystem\_vpc_meshsystem\default\win64\_msvc_\Retail\meshsystem.pdb`; GUID `6bf2d774-f314-42ac-bbfd-1052b0eb319b`; age 10; lookup key `6BF2D774F31442ACBBFD1052B0EB319BA`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x300d0` |  |
| `CreateInterface` | `0x101e80` |  |
| `ExtractModuleMetadata` | `0x30520` |  |
| `GetResourceManifestCount` | `0x103a70` |  |
| `GetResourceManifests` | `0x103a80` |  |
| `InstallSchemaBindings` | `0x103cd0` |  |

RTTI sample: `.?AUAsyncProcessDataResult@CRenderMeshManager@@`, `.?AUAsyncProcessResult@CBaseIntrospectedToKV3TypeManager@@`, `.?AUCMorphSetData@@`, `.?AV?$CBaseAppSystem@VIMeshSystem@@@@`, `.?AV?$CBaseAppSystem@VIMeshUtils@@@@`, `.?AV?$CMutableRefCountBase@VCRefMT@@@@`, `.?AV?$CMutableRefCounted@V?$CMutableRefCountBase@VCRefMT@@@@@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted1@VIMeshRayTrace@@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AV?$CRefCounted@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AV?$CTier0AppSystem@VIMeshSystem@@$0A@@@`, `.?AV?$CTier0AppSystem@VIMeshUtils@@$0A@@@`, `.?AV?$CTier1AppSystem@VIMeshSystem@@$0A@@@`, `.?AV?$CTier1AppSystem@VIMeshUtils@@$0A@@@`, `.?AV?$CTier2AppSystem@VIMeshSystem@@$0A@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## navsystem.dll

Path: `game/bin/win64/navsystem.dll`  
SHA-256: `16b5f5b88b45a011fe44b7d25bdad6b23c964bf00c654bb08743510363351e77`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xd6508`.  
Exports: 6; RTTI types: 167; x64 unwind entries: 3594.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\navsystem\_vpc_navsystem\default\win64\_msvc_\Retail\navsystem.pdb`; GUID `52a241ec-aafe-4d2f-8e8b-489a06ae57a6`; age 12; lookup key `52A241ECAAFE4D2F8E8B489A06AE57A6C`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x9920` |  |
| `CreateInterface` | `0xc0b10` |  |
| `ExtractModuleMetadata` | `0x9d70` |  |
| `GetResourceManifestCount` | `0xd5550` |  |
| `GetResourceManifests` | `0xd5560` |  |
| `InstallSchemaBindings` | `0xd57b0` |  |

RTTI sample: `.?AU?$default_delete@VCNavGenPolyProcessorStageBase@@@std@@`, `.?AUVDataInitInfo_t@@`, `.?AV?$CBaseAppSystem@VINavSystem@@@@`, `.?AV?$CBaseTypesafeVDataFileManager@VCNavHullPresetVData@@@@`, `.?AV?$CBaseTypesafeVDataFileManager@VCNavHullVData@@@@`, `.?AV?$CErrorContextValue@PEBVKeyValues3@@@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?M@???$ParallelFor@$00V<lambda_1>@?1??CalculateAllPathsBetweenFlowMapNodes@CNavFlowMapGenerator@@AEAAXXZ@@@YAXHHPEBD$$QEAV1?1??CalculateAllPathsBetweenFlowMapNodes@CNavFlowMapGenerator@@AEAAXXZ@HHW4JobPriority_t@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted1@VIRefCounted@@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AV?$CTier0AppSystem@VINavSystem@@$0A@@@`, `.?AV?$CTier1AppSystem@VINavSystem@@$0A@@@`, `.?AV?$CTier2AppSystem@VINavSystem@@$0A@@@`, `.?AV?$CTier3AppSystem@VINavSystem@@$0A@@@`, `.?AV?$CVDataFileManager@VCNavHullPresetVData@@@@`, `.?AV?$CVDataFileManager@VCNavHullVData@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## networksystem.dll

Path: `game/bin/win64/networksystem.dll`  
SHA-256: `1a04b6549b050472336baba4111d36c6892230e2d432c7a9dfa5e1a6e78df75e`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1d7450`.  
Exports: 6; RTTI types: 355; x64 unwind entries: 8462.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\networksystem\_vpc_networksystem\default\win64\_msvc_\Retail\networksystem.pdb`; GUID `7593b982-a75f-4dd0-bb30-feb3373b655a`; age 14; lookup key `7593B982A75F4DD0BB30FEB3373B655AE`.

Normal import modules: `ADVAPI32.dll`, `WS2_32.dll`, `steam_api64.dll`, `steamnetworkingsockets.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x117bf0` |  |
| `CreateInterface` | `0x1c8030` |  |
| `ExtractModuleMetadata` | `0x1180d0` |  |
| `GetResourceManifestCount` | `0x1d4d50` |  |
| `GetResourceManifests` | `0x1d4d60` |  |
| `InstallSchemaBindings` | `0x1d4940` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AU?$NetFlow_t@UNetFrameIn_t@CNetChan@@@CNetChan@@`, `.?AU?$NetFlow_t@UNetFrameOut_t@CNetChan@@@CNetChan@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AUNetFlowBase_t@CNetChan@@`, `.?AUNetFlowIn_t@CNetChan@@`, `.?AUNetFlowOut_t@CNetChan@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$CBaseAppSystem@VINetworkSystem@@@@`, `.?AV?$CConCommandMemberAccessor@VCFlattenedSerializers@@@@`, `.?AV?$CConCommandMemberAccessor@VCNetworkMessages@@@@`, `.?AV?$CNetMessagePB@$02VCNETMsg_SplitScreenUser@@$0L@$00$0A@@@`, `.?AV?$CNetMessagePB@$0?0VNetMessageConnectionClosed@@$0L@$00$0A@@@`, `.?AV?$CNetMessagePB@$0?0VNetMessageConnectionCrashed@@$0L@$00$0A@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## p4lib.dll

Path: `game/bin/win64/p4lib.dll`  
SHA-256: `656411736048b94928ec7813109203f3f1fd5ce307aa2e4a53396c95d9cf05eb`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x6288`.  
Exports: 3; RTTI types: 9; x64 unwind entries: 431.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\p4lib\_vpc_p4lib\default\win64\_msvc_\Retail\p4lib.pdb`; GUID `a3a983c6-75bb-474b-8886-4c0d0286cc69`; age 7; lookup key `A3A983C675BB474B88864C0D0286CC697`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x2300` |  |
| `CreateInterface` | `0x2910` |  |
| `ExtractModuleMetadata` | `0x2740` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIP4@@@@`, `.?AVCP4@@`, `.?AVCP4Dummy@@`, `.?AVCP4Mimic@@`, `.?AVIAppSystem@@`, `.?AVIP4@@`, `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## panorama.dll

Path: `game/bin/win64/panorama.dll`  
SHA-256: `8b10f2b1f9150ecfaf9ffa21c07cc01abaef23ea89cb74de627e87da7d75693c`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3fee00`.  
Exports: 7; RTTI types: 517; x64 unwind entries: 17349.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\panorama\_vpc_panorama\default\win64\_msvc_\Retail\panorama.pdb`; GUID `987d1814-8efd-4787-8456-e05e445a7cfa`; age 16; lookup key `987D18148EFD47878456E05E445A7CFA10`.

Normal import modules: `USER32.dll`, `WINMM.dll`, `SDL3.dll`, `steam_api64.dll`, `tier0.dll`, `v8.dll`, `video64.dll`, `KERNEL32.dll`, `ADVAPI32.dll`, `WS2_32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x9f040` |  |
| `CreateInterface` | `0x3dbd80` |  |
| `CreatePanoramaUIEngineInternal` | `0xa21f0` |  |
| `ExtractModuleMetadata` | `0x9f540` |  |
| `GetResourceManifestCount` | `0x3f1690` |  |
| `GetResourceManifests` | `0x3f16a0` |  |
| `InstallSchemaBindings` | `0x3f1280` |  |

RTTI sample: `.?AU?$CContentASTNode@W4ELayoutNodeType@@@panorama@@`, `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUCircleElement_t@@`, `.?AUClipPathElement_t@@`, `.?AUContainerElement_t@@`, `.?AUDefsElement_t@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## panorama_text_pango.dll

Path: `game/bin/win64/panorama_text_pango.dll`  
SHA-256: `2b276d889bf7fd9c128e2c64f8439c2baa7be8709ecf1cff05d95997540143d4`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1b1c00`.  
Exports: 7; RTTI types: 144; x64 unwind entries: 7755.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\panorama\_vpc_panorama_text_pango\default\win64\_msvc_\Retail\panorama_text_pango.pdb`; GUID `b50f7bf6-67ab-4e3e-b026-f7428a25ba7c`; age 12; lookup key `B50F7BF667AB4E3EB026F7428A25BA7CC`.

Normal import modules: `libfontconfig-1.dll`, `libglib-2.0-0.dll`, `libgobject-2.0-0.dll`, `libfreetype-6.dll`, `libpango-1.0-0.dll`, `libpangoft2-1.0-0.dll`, `tier0.dll`, `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`, `WS2_32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x4ae70` |  |
| `CreateInterface` | `0x1af1a0` |  |
| `ExtractModuleMetadata` | `0x4b370` |  |
| `GetResourceManifestCount` | `0x1af8b0` |  |
| `GetResourceManifests` | `0x1af8c0` |  |
| `InstallSchemaBindings` | `0x1afb10` |  |
| `UI_Font_New_Face_Override` | `0x52a30` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$CBaseAppSystem@VIUITextServices@panorama@@@@`, `.?AV?$CTier0AppSystem@VIUITextServices@panorama@@$0A@@@`, `.?AV?$CTier1AppSystem@VIUITextServices@panorama@@$0A@@@`, `.?AV?$CTier2AppSystem@VIUITextServices@panorama@@$0A@@@`, `.?AV?$CTier3AppSystem@VIUITextServices@panorama@@$0A@@@`, `.?AV?$RepeatedField@H@protobuf@google@@`, `.?AV?$RepeatedField@I@protobuf@google@@`, `.?AV?$RepeatedField@M@protobuf@google@@`, `.?AV?$RepeatedField@N@protobuf@google@@`, `.?AV?$RepeatedField@_J@protobuf@google@@`, `.?AV?$RepeatedField@_K@protobuf@google@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## panoramauiclient.dll

Path: `game/bin/win64/panoramauiclient.dll`  
SHA-256: `76ba195bda135acc1404108b6914d06c5526aeaa31ee06c30ad49bfb9b1468d5`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1bc4e4`.  
Exports: 6; RTTI types: 446; x64 unwind entries: 10233.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\panoramauiclient\_vpc_panoramauiclient\default\win64\_msvc_\Retail\panoramauiclient.pdb`; GUID `5c0e861f-a1b5-4892-8d8e-98392b10166e`; age 16; lookup key `5C0E861FA1B548928D8E98392B10166E10`.

Normal import modules: `steam_api64.dll`, `tier0.dll`, `v8.dll`, `video64.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x16df0` |  |
| `CreateInterface` | `0x1ab2a0` |  |
| `ExtractModuleMetadata` | `0x172d0` |  |
| `GetResourceManifestCount` | `0x1b5600` |  |
| `GetResourceManifests` | `0x1b5610` |  |
| `InstallSchemaBindings` | `0x1b5860` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AUGamePadData_t@panorama@@`, `.?AUHtmlFormHasFocus_t@panorama@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AULineColumn_t@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$CBaseAppSystem@VIPanoramaUIClient@@@@`, `.?AV?$CBaseSymbol@$00@panorama@@`, `.?AV?$CCallResult@VCHTML@panorama@@UHTML_BrowserReady_t@@@@`, `.?AV?$CCallback@VCHTML@panorama@@UHTML_CanGoBackAndForward_t@@$0A@@@`, `.?AV?$CCallback@VCHTML@panorama@@UHTML_ChangedTitle_t@@$0A@@@`, `.?AV?$CCallback@VCHTML@panorama@@UHTML_CloseBrowser_t@@$0A@@@`, `.?AV?$CCallback@VCHTML@panorama@@UHTML_FileOpenDialog_t@@$0A@@@`, `.?AV?$CCallback@VCHTML@panorama@@UHTML_FinishedRequest_t@@$0A@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## particles.dll

Path: `game/bin/win64/particles.dll`  
SHA-256: `e2cc4e44f244f7e90901ff8160f874b89646d3ac7090f10695c859ccda815e91`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x42a994`.  
Exports: 6; RTTI types: 999; x64 unwind entries: 14323.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\particles\_vpc_particles\default\win64\_msvc_\Retail\particles.pdb`; GUID `90954fa1-7997-482e-b035-01e27865ab8a`; age 12; lookup key `90954FA17997482EB03501E27865AB8AC`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xd0360` |  |
| `CreateInterface` | `0x425380` |  |
| `ExtractModuleMetadata` | `0xd07b0` |  |
| `GetResourceManifestCount` | `0x4297b0` |  |
| `GetResourceManifests` | `0x4297c0` |  |
| `InstallSchemaBindings` | `0x429af0` |  |

RTTI sample: `.?AUCPulseCell_BooleanSwitchState_DebugInfo_t@@`, `.?AUCPulseCell_Inflow_Wait_DebugInfo_t@@`, `.?AUCPulseCell_IntervalTimer_DebugInfo_t@@`, `.?AUCPulseCell_Timeline_DebugInfo_t@@`, `.?AV<lambda_1>@?$PulseBindingResourceHandleTraits_t@V?$CWeakHandle@UInfoForResourceTypeCModel@@@@@@`, `.?AV<lambda_1>@?$PulseBindingSchemaEnumTraits_t@W4PulseCursorCancelPriority_t@@@@`, `.?AV<lambda_1>@?$PulseBindingSchemaEnumTraits_t@W4PulseDurationStringFormat_t@@@@`, `.?AV<lambda_2>@?$PulseBindingResourceHandleTraits_t@V?$CWeakHandle@UInfoForResourceTypeCModel@@@@@@`, `.?AV<lambda_2>@?$PulseBindingSchemaEnumTraits_t@W4PulseCursorCancelPriority_t@@@@`, `.?AV<lambda_2>@?$PulseBindingSchemaEnumTraits_t@W4PulseDurationStringFormat_t@@@@`, `.?AV<lambda_3>@?$PulseBindingResourceHandleTraits_t@V?$CWeakHandle@UInfoForResourceTypeCModel@@@@@@`, `.?AV<lambda_3>@?$PulseBindingSchemaEnumTraits_t@W4PulseCursorWakePriority_t@@@@`, `.?AV<lambda_4>@?$PulseBindingResourceHandleTraits_t@V?$CWeakHandle@UInfoForResourceTypeCModel@@@@@@`, `.?AV<lambda_4>@?$PulseBindingSchemaEnumTraits_t@W4PulseCursorWakePriority_t@@@@`, `.?AV?$CBaseAppSystem@VIParticleSystemMgr@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## phonon.dll

Path: `game/bin/win64/phonon.dll`  
SHA-256: `a97c891d49619e6d6dc3cbad8d64db1648db8dc6efcfe1d01378e90c954161ea`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x2674b14`.  
Exports: 418; RTTI types: 1602; x64 unwind entries: 24751.

PDB reference: `C:\buildworker\phonon_main_win64\build\core\build\windows-vs2022-x64\src\core\Release\phonon.pdb`; GUID `bc3d9049-52c4-4d40-88eb-039b8fa1fbd9`; age 3; lookup key `BC3D904952C44D4088EB039B8FA1FBD93`.

Normal import modules: `KERNEL32.dll`, `ole32.dll`, `ADVAPI32.dll`, `CFGMGR32.dll`.

Delay import modules: OpenCL.dll, nvcuda.dll, VCOMP140.DLL.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `TANCreateContext` | `0x262e250` |  |
| `TANCreateConvolution` | `0x263b800` |  |
| `TANCreateFFT` | `0x2641980` |  |
| `TANCreateMath` | `0x264b5d0` |  |
| `TANGetCacheFolder` | `0x262e570` |  |
| `TANSetCacheFolder` | `0x262e5b0` |  |
| `createQueue` | `0x26291b0` |  |
| `getDeviceAndContext` | `0x26291f0` |  |
| `iplAirAbsorptionCalculate` | `0xaa76e0` |  |
| `iplAmbisonicsBinauralEffectApply` | `0xaa7120` |  |
| `iplAmbisonicsBinauralEffectCreate` | `0xaa7390` |  |
| `iplAmbisonicsBinauralEffectGetTail` | `0xaa7070` |  |
| `iplAmbisonicsBinauralEffectGetTailSize` | `0xaa7300` |  |
| `iplAmbisonicsBinauralEffectRelease` | `0xaa6f10` |  |
| `iplAmbisonicsBinauralEffectReset` | `0xaa7180` |  |
| `iplAmbisonicsBinauralEffectRetain` | `0xaa6f00` |  |
| `iplAmbisonicsDecodeEffectApply` | `0xaa7120` |  |
| `iplAmbisonicsDecodeEffectCreate` | `0xaa73d0` |  |
| `iplAmbisonicsDecodeEffectGetTail` | `0xaa7070` |  |
| `iplAmbisonicsDecodeEffectGetTailSize` | `0xaa7300` |  |
| `iplAmbisonicsDecodeEffectRelease` | `0xaa6f10` |  |
| `iplAmbisonicsDecodeEffectReset` | `0xaa7180` |  |
| `iplAmbisonicsDecodeEffectRetain` | `0xaa6f00` |  |
| `iplAmbisonicsEncodeEffectApply` | `0xaa7120` |  |
| `iplAmbisonicsEncodeEffectCreate` | `0xaa7350` |  |
| `iplAmbisonicsEncodeEffectGetTail` | `0xaa7070` |  |
| `iplAmbisonicsEncodeEffectGetTailSize` | `0xaa7300` |  |
| `iplAmbisonicsEncodeEffectRelease` | `0xaa6f10` |  |
| `iplAmbisonicsEncodeEffectReset` | `0xaa7180` |  |
| `iplAmbisonicsEncodeEffectRetain` | `0xaa6f00` |  |

RTTI sample: `.?AU?$BVHNBuilderFastSpatialSAH@$03UQuadMesh@embree@@U?$QuadMv@$03@2@UQuadSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UQuadMesh@embree@@U?$QuadMv@$03@2@UQuadSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleM@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleM@$03@2@UTriangleSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMi@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMi@$03@2@UTriangleSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMv@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$03UTriangleMesh@embree@@U?$TriangleMv@$03@2@UTriangleSplitterFactory@sse2@2@@sse2@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$07UQuadMesh@embree@@U?$QuadMv@$03@2@UQuadSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$07UTriangleMesh@embree@@U?$TriangleM@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderFastSpatialSAH@$07UTriangleMesh@embree@@U?$TriangleMv@$03@2@UTriangleSplitterFactory@avx@2@@avx@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UInstance@embree@@UInstancePrimitive@2@@avx@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UInstance@embree@@UInstancePrimitive@2@@sse2@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UInstanceArray@embree@@UInstanceArrayPrimitive@2@@avx@embree@@`, `.?AU?$BVHNBuilderMBlurSAH@$03UInstanceArray@embree@@UInstanceArrayPrimitive@2@@sse2@embree@@`.

## phonon4.dll

Path: `game/bin/win64/phonon4.dll`  
SHA-256: `a844ddfea7141d42f7cfeb37d9595b19575de4e3c2d880985cea926840b6b3a9`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x2de10`.  
Exports: 616; RTTI types: 69; x64 unwind entries: 1877.

PDB reference: `C:\Valve\ThirdPartyCode\lgpl\qt\qt_valve_win64\lib\phonon4.pdb`; GUID `929df8d3-255d-4ee4-a369-45cfbfe9d0e3`; age 1; lookup key `929DF8D3255D4EE4A36945CFBFE9D0E31`.

Normal import modules: `QtGui4.dll`, `QtCore4.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0AbstractAudioOutput@Phonon@@IEAA@AEAVAbstractAudioOutputPrivate@1@PEAVQObject@@@Z` | `0x1280` |  |
| `??0AbstractMediaStream@Phonon@@IEAA@AEAVAbstractMediaStreamPrivate@1@PEAVQObject@@@Z` | `0x1e90` |  |
| `??0AbstractMediaStream@Phonon@@IEAA@PEAVQObject@@@Z` | `0x1e00` |  |
| `??0AbstractMediaStreamPrivate@Phonon@@IEAA@XZ` | `0x18a0` |  |
| `??0AbstractMediaStreamPrivate@Phonon@@QEAA@AEBV01@@Z` | `0x1ba0` |  |
| `??0AbstractVideoOutput@Phonon@@IEAA@AEAVAbstractVideoOutputPrivate@1@@Z` | `0x2190` |  |
| `??0AbstractVideoOutput@Phonon@@QEAA@AEBV01@@Z` | `0x20c0` |  |
| `??0AudioDataOutput@Phonon@@QEAA@PEAVQObject@@@Z` | `0x2460` |  |
| `??0AudioOutput@Phonon@@QEAA@PEAVQObject@@@Z` | `0x4ba0` |  |
| `??0AudioOutput@Phonon@@QEAA@W4Category@1@PEAVQObject@@@Z` | `0x4b00` |  |
| `??0Effect@Phonon@@IEAA@AEAVEffectPrivate@1@PEAVQObject@@@Z` | `0x8490` |  |
| `??0Effect@Phonon@@QEAA@AEBV?$ObjectDescription@$00@1@PEAVQObject@@@Z` | `0x9090` |  |
| `??0EffectParameter@Phonon@@QEAA@AEBV01@@Z` | `0x9ee0` |  |
| `??0EffectParameter@Phonon@@QEAA@HAEBVQString@@V?$QFlags@W4Hint@EffectParameter@Phonon@@@@AEBVQVariant@@22AEBV?$QList@VQVariant@@@@0@Z` | `0x9d90` |  |
| `??0EffectParameter@Phonon@@QEAA@XZ` | `0x9d40` |  |
| `??0EffectWidget@Phonon@@QEAA@PEAVEffect@1@PEAVQWidget@@@Z` | `0xbbe0` |  |
| `??0GlobalConfig@Phonon@@QEAA@AEBV01@@Z` | `0x3690` |  |
| `??0GlobalConfig@Phonon@@QEAA@XZ` | `0xf9c0` |  |
| `??0MediaController@Phonon@@QEAA@PEAVMediaObject@1@@Z` | `0x163d0` |  |
| `??0MediaNode@Phonon@@IEAA@AEAVMediaNodePrivate@1@@Z` | `0x184d0` |  |
| `??0MediaNode@Phonon@@QEAA@AEBV01@@Z` | `0x1060` |  |
| `??0MediaNodePrivate@Phonon@@IEAA@W4CastId@01@@Z` | `0x18ee0` |  |
| `??0MediaObject@Phonon@@QEAA@PEAVQObject@@@Z` | `0x1b770` |  |
| `??0MediaSource@Phonon@@IEAA@AEAVMediaSourcePrivate@1@@Z` | `0x1ce10` |  |
| `??0MediaSource@Phonon@@QEAA@AEBV01@@Z` | `0x1d050` |  |
| `??0MediaSource@Phonon@@QEAA@AEBVQString@@@Z` | `0x1d2a0` |  |
| `??0MediaSource@Phonon@@QEAA@AEBVQUrl@@@Z` | `0x1ce80` |  |
| `??0MediaSource@Phonon@@QEAA@PEAVAbstractMediaStream@1@@Z` | `0x1d540` |  |
| `??0MediaSource@Phonon@@QEAA@PEAVQIODevice@@@Z` | `0x1d5d0` |  |
| `??0MediaSource@Phonon@@QEAA@W4DiscType@1@AEBVQString@@@Z` | `0x1cf10` |  |

RTTI sample: `.?AVAbstractAudioOutput@Phonon@@`, `.?AVAbstractAudioOutputPrivate@Phonon@@`, `.?AVAbstractMediaStream@Phonon@@`, `.?AVAbstractMediaStreamPrivate@Phonon@@`, `.?AVAbstractVideoOutput@Phonon@@`, `.?AVAbstractVideoOutputPrivate@Phonon@@`, `.?AVAudioDataOutput@Phonon@@`, `.?AVAudioDataOutputPrivate@Phonon@@`, `.?AVAudioOutput@Phonon@@`, `.?AVAudioOutputPrivate@Phonon@@`, `.?AVBackendCapabilitiesPrivate@Phonon@@`, `.?AVEffect@Phonon@@`, `.?AVEffectPrivate@Phonon@@`, `.?AVEffectWidget@Phonon@@`, `.?AVFactoryPrivate@Phonon@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## physicsbuilder.dll

Path: `game/bin/win64/physicsbuilder.dll`  
SHA-256: `5d7af04f64350427cb7f91a999630685fc63e8b0d645b44293f21ad2a4e6b89f`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x673914`.  
Exports: 6; RTTI types: 5622; x64 unwind entries: 33042.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\physicsbuilder\_vpc_physicsbuilder\default\win64\_msvc_\Retail\physicsbuilder.pdb`; GUID `117615e5-be86-4d86-8a7c-83caf50ae1a1`; age 12; lookup key `117615E5BE864D868A7C83CAF50AE1A1C`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`, `USER32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x1b7b0` |  |
| `CreateInterface` | `0x628000` |  |
| `ExtractModuleMetadata` | `0x1bd10` |  |
| `GetResourceManifestCount` | `0x645a50` |  |
| `GetResourceManifests` | `0x645a60` |  |
| `InstallSchemaBindings` | `0x6462a0` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUCMorphSetData@@`, `.?AUCScaleBiasFilter@CModelDocClothProxyMeshFile@@`, `.?AU_Anon@?$_Associated_state@H@std@@`, `.?AU_Anon@?$_Associated_state@I@std@@`, `.?AU_Crt_new_delete@std@@`, `.?AU_TaskProcHandle@details@Concurrency@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## propertyeditor.dll

Path: `game/bin/win64/propertyeditor.dll`  
SHA-256: `3082327f5d1fc3db5b2da686318d60a36b8350de99e514539b54e22cd2f01942`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xcf550`.  
Exports: 6; RTTI types: 356; x64 unwind entries: 4453.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\propertyeditor\_vpc_propertyeditor\default\win64\_msvc_\Retail\propertyeditor.pdb`; GUID `03d7c320-3789-4ea4-aa7c-d9914171c7d4`; age 12; lookup key `03D7C32037894EA4AA7CD9914171C7D4C`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `KERNEL32.dll`, `USER32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x42b90` |  |
| `CreateInterface` | `0xc6850` |  |
| `ExtractModuleMetadata` | `0x42fe0` |  |
| `GetResourceManifestCount` | `0xccb90` |  |
| `GetResourceManifests` | `0xccba0` |  |
| `InstallSchemaBindings` | `0xccdf0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIPropertyEditorSystem@@@@`, `.?AV?$CBindTargetDescriptor@VCGlobalHotkeyCommands@@@@`, `.?AV?$CBindTargetDescriptor@VCQCurveEditorWidget@@@@`, `.?AV?$CBindTargetDescriptor@VCQPropertyEditor@@@@`, `.?AV?$CDynamicWidgetFactory@VCQDraggableWidgetList@@@@`, `.?AV?$CDynamicWidgetFactory@VCQFilteredTreeWidget@@@@`, `.?AV?$CDynamicWidgetFactory@VCQTreeWidgetWithHover@@@@`, `.?AV?$CDynamicWidgetFactory@VQComboBox@@@@`, `.?AV?$CDynamicWidgetFactory@VQFrame@@@@`, `.?AV?$CDynamicWidgetFactory@VQGroupBox@@@@`, `.?AV?$CDynamicWidgetFactory@VQLabel@@@@`, `.?AV?$CDynamicWidgetFactory@VQLineEdit@@@@`, `.?AV?$CDynamicWidgetFactory@VQListWidget@@@@`, `.?AV?$CDynamicWidgetFactory@VQMenu@@@@`, `.?AV?$CDynamicWidgetFactory@VQProgressBar@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## pulse_system.dll

Path: `game/bin/win64/pulse_system.dll`  
SHA-256: `b29db0bf30474883f943f75e4cfd6d82221d37197cf59e17094b1506cc11a08e`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x101f60`.  
Exports: 6; RTTI types: 208; x64 unwind entries: 6354.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\pulse_system\_vpc_pulse_system\default\win64\_msvc_\Retail\pulse_system.pdb`; GUID `b1f6b1c3-b816-4822-972f-2782a67af165`; age 11; lookup key `B1F6B1C3B8164822972F2782A67AF165B`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x21610` |  |
| `CreateInterface` | `0xfdef0` |  |
| `ExtractModuleMetadata` | `0x21a60` |  |
| `GetResourceManifestCount` | `0x1014b0` |  |
| `GetResourceManifests` | `0x1014c0` |  |
| `InstallSchemaBindings` | `0x1010a0` |  |

RTTI sample: `.?AUCPulseCell_BooleanSwitchState_DebugInfo_t@@`, `.?AUCPulseCell_Inflow_Wait_DebugInfo_t@@`, `.?AUCPulseCell_IntervalTimer_DebugInfo_t@@`, `.?AUCPulseCell_Timeline_DebugInfo_t@@`, `.?AUFakeEntityDerivedA_t@@`, `.?AUFakeEntityDerivedB_t@@`, `.?AUFakeEntity_t@@`, `.?AUPulseSystemEventBase_t@@`, `.?AUPulseSystemEventCursorResult_t@@`, `.?AUPulseSystemEventObservableVariableListenerNotify_t@@`, `.?AUPulseSystemEventObservableVariableSourceChanged_t@@`, `.?AUPulseSystemEventYieldResume_t@@`, `.?AUTestComponent_t@@`, `.?AV<lambda_10>@?$PulseBindingSchemaEnumTraits_t@W4PulseTestEnumFlags_t@@@@`, `.?AV<lambda_11>@?$PulseBindingSchemaEnumTraits_t@W4PulseTestEnumFlagsAlt_t@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## qgif.dll

Path: `game/bin/win64/qt5_plugins/imageformats/qgif.dll`  
SHA-256: `df50647b02abf30cda040460e9be6d903cc7a3d07fd79c229ce136997beafd96`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x460c`.  
Exports: 2; RTTI types: 6; x64 unwind entries: 344.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\plugins\imageformats\qgif.pdb`; GUID `1571c47b-98bd-4522-946a-62e6b3b469a5`; age 1; lookup key `1571C47B98BD4522946A62E6B3B469A51`.

Normal import modules: `Qt5Gui.dll`, `Qt5Core.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `qt_plugin_instance` | `0x3ab0` |  |
| `qt_plugin_query_metadata` | `0x3bb0` |  |

RTTI sample: `.?AVQGifHandler@@`, `.?AVQGifPlugin@@`, `.?AVQImageIOHandler@@`, `.?AVQImageIOPlugin@@`, `.?AVQObject@@`, `.?AVtype_info@@`.

## qico.dll

Path: `game/bin/win64/qt5_plugins/imageformats/qico.dll`  
SHA-256: `ddafd14ac6215b2ba8af3cbb0a251dffc4d76b7c5d0f419d614991fe9d16a093`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3928`.  
Exports: 2; RTTI types: 8; x64 unwind entries: 344.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\plugins\imageformats\qico.pdb`; GUID `055594d8-33cb-4500-8f45-b73ca69a22ce`; age 1; lookup key `055594D833CB45008F45B73CA69A22CE1`.

Normal import modules: `Qt5Gui.dll`, `Qt5Core.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `qt_plugin_instance` | `0x2d90` |  |
| `qt_plugin_query_metadata` | `0x2ea0` |  |

RTTI sample: `.?AVQICOPlugin@@`, `.?AVQImage@@`, `.?AVQImageIOHandler@@`, `.?AVQImageIOPlugin@@`, `.?AVQObject@@`, `.?AVQPaintDevice@@`, `.?AVQtIcoHandler@@`, `.?AVtype_info@@`.

## qjpeg.dll

Path: `game/bin/win64/qt5_plugins/imageformats/qjpeg.dll`  
SHA-256: `4d0201f4dd243bd1fd29417b57901eb03b50bb3bc2603f3a6641d12b4eb3fd81`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x36a48`.  
Exports: 2; RTTI types: 6; x64 unwind entries: 1015.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\plugins\imageformats\qjpeg.pdb`; GUID `26ed5868-d2e9-4e45-ae1b-83ccf4747513`; age 1; lookup key `26ED5868D2E94E45AE1B83CCF47475131`.

Normal import modules: `Qt5Gui.dll`, `Qt5Core.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `qt_plugin_instance` | `0x35a80` |  |
| `qt_plugin_query_metadata` | `0x35b90` |  |

RTTI sample: `.?AVQImageIOHandler@@`, `.?AVQImageIOPlugin@@`, `.?AVQJpegHandler@@`, `.?AVQJpegPlugin@@`, `.?AVQObject@@`, `.?AVtype_info@@`.

## qtga.dll

Path: `game/bin/win64/qt5_plugins/imageformats/qtga.dll`  
SHA-256: `fce0a0c801f8be7799c12947d42bc0d975760f85934e9a3bfdf08e9f021ff6ab`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x27f8`.  
Exports: 2; RTTI types: 10; x64 unwind entries: 313.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtimageformats\plugins\imageformats\qtga.pdb`; GUID `83a8813d-007e-431b-9262-a52e7fe593a4`; age 1; lookup key `83A8813D007E431B9262A52E7FE593A41`.

Normal import modules: `Qt5Gui.dll`, `Qt5Core.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `qt_plugin_instance` | `0x1c70` |  |
| `qt_plugin_query_metadata` | `0x1d80` |  |

RTTI sample: `.?AUTga16Reader@@`, `.?AUTga24Reader@@`, `.?AUTga32Reader@@`, `.?AUTgaReader@@`, `.?AVQImageIOHandler@@`, `.?AVQImageIOPlugin@@`, `.?AVQObject@@`, `.?AVQTgaHandler@@`, `.?AVQTgaPlugin@@`, `.?AVtype_info@@`.

## qtiff.dll

Path: `game/bin/win64/qt5_plugins/imageformats/qtiff.dll`  
SHA-256: `a13b60ffad79e982148e757e622afe7176b0790a74a5982083c542bb6176bac6`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x32da4`.  
Exports: 2; RTTI types: 6; x64 unwind entries: 1135.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtimageformats\plugins\imageformats\qtiff.pdb`; GUID `959d8f22-3f90-4cdb-9049-21608c7189db`; age 1; lookup key `959D8F223F904CDB904921608C7189DB1`.

Normal import modules: `Qt5Gui.dll`, `Qt5Core.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `qt_plugin_instance` | `0x1260` |  |
| `qt_plugin_query_metadata` | `0x1370` |  |

RTTI sample: `.?AVQImageIOHandler@@`, `.?AVQImageIOPlugin@@`, `.?AVQObject@@`, `.?AVQTiffHandler@@`, `.?AVQTiffPlugin@@`, `.?AVtype_info@@`.

## qwindows.dll

Path: `game/bin/win64/qt5_plugins/platforms/qwindows.dll`  
SHA-256: `e9b4e3c270d7b64eb06871e8d5022d4ea768d93bdd205faab070c6fae695e550`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xeedd4`.  
Exports: 2; RTTI types: 183; x64 unwind entries: 5072.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\plugins\platforms\qwindows.pdb`; GUID `88e86fac-3524-40ba-bf1d-3c383fd4793a`; age 1; lookup key `88E86FAC352440BABF1D3C383FD4793A1`.

Normal import modules: `dwmapi.dll`, `IMM32.dll`, `OLEAUT32.dll`, `WTSAPI32.dll`, `ole32.dll`, `USER32.dll`, `GDI32.dll`, `Qt5Gui.dll`, `Qt5Core.dll`, `SHELL32.dll`, `WINMM.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `qt_plugin_instance` | `0x60630` |  |
| `qt_plugin_query_metadata` | `0x60740` |  |

RTTI sample: `.?AUID2D1SimplifiedGeometrySink@@`, `.?AUIDWriteFontFileLoader@@`, `.?AUIDWriteFontFileStream@@`, `.?AUIDataObject@@`, `.?AUIDropSource@@`, `.?AUIDropTarget@@`, `.?AUIEnumFORMATETC@@`, `.?AUIExpandCollapseProvider@@`, `.?AUIFileDialogEvents@@`, `.?AUIGridItemProvider@@`, `.?AUIGridProvider@@`, `.?AUIInvokeProvider@@`, `.?AUIRangeValueProvider@@`, `.?AUIRawElementProviderFragment@@`, `.?AUIRawElementProviderFragmentRoot@@`.

## qwindowsvistastyle.dll

Path: `game/bin/win64/qt5_plugins/styles/qwindowsvistastyle.dll`  
SHA-256: `be29d91c9eac2bb826a011357d13593f216a68b55d8914effc480b61ea4348a5`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1615c`.  
Exports: 2; RTTI types: 25; x64 unwind entries: 515.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\plugins\styles\qwindowsvistastyle.pdb`; GUID `f33ad160-2074-4014-9591-e2ead7511bc4`; age 1; lookup key `F33AD160207440149591E2EAD7511BC41`.

Normal import modules: `Qt5Widgets.dll`, `Qt5Gui.dll`, `Qt5Core.dll`, `UxTheme.dll`, `USER32.dll`, `GDI32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `qt_plugin_instance` | `0x11b0` |  |
| `qt_plugin_query_metadata` | `0x12c0` |  |

RTTI sample: `.?AVQAbstractAnimation@@`, `.?AVQBlendStyleAnimation@@`, `.?AVQCommonStyle@@`, `.?AVQCommonStylePrivate@@`, `.?AVQObject@@`, `.?AVQObjectData@@`, `.?AVQObjectPrivate@@`, `.?AVQProgressStyleAnimation@@`, `.?AVQStyle@@`, `.?AVQStyleAnimation@@`, `.?AVQStylePlugin@@`, `.?AVQStylePrivate@@`, `.?AVQWindowsStyle@@`, `.?AVQWindowsStylePrivate@@`, `.?AVQWindowsVistaAnimation@@`.

## Qt5Concurrent.dll

Path: `game/bin/win64/Qt5Concurrent.dll`  
SHA-256: `d4a01961fff02cc38ab906d3bffaeb49db893edc624f840e06d07985086db29f`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3114`.  
Exports: 34; RTTI types: 6; x64 unwind entries: 386.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\lib\Qt5Concurrent.pdb`; GUID `9cfca0c2-b547-44e5-bb52-6aabc1ef361a`; age 1; lookup key `9CFCA0C2B54744E5BB526AABC1EF361A1`.

Normal import modules: `Qt5Core.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0BlockSizeManager@QtConcurrent@@QEAA@H@Z` | `0x1f60` |  |
| `??0BlockSizeManagerV2@QtConcurrent@@QEAA@H@Z` | `0x2020` |  |
| `??0ThreadEngineBase@QtConcurrent@@QEAA@XZ` | `0x1050` |  |
| `??1BlockSizeManager@QtConcurrent@@QEAA@XZ` | `0x2110` |  |
| `??1ThreadEngineBase@QtConcurrent@@UEAA@XZ` | `0x10d0` |  |
| `??_7ThreadEngineBase@QtConcurrent@@6B@` | `0x113b8` |  |
| `?acquireBarrierSemaphore@ThreadEngineBase@QtConcurrent@@QEAAXXZ` | `0x11c0` |  |
| `?blockSize@BlockSizeManager@QtConcurrent@@QEAAHXZ` | `0x2300` |  |
| `?blockSize@BlockSizeManagerV2@QtConcurrent@@QEAAHXZ` | `0x2310` |  |
| `?blockSizeMaxed@BlockSizeManager@QtConcurrent@@AEAA_NXZ` | `0x2320` |  |
| `?blockSizeMaxed@BlockSizeManagerV2@QtConcurrent@@AEAA_NXZ` | `0x2330` |  |
| `?finish@ThreadEngineBase@QtConcurrent@@MEAAXXZ` | `0x11f0` |  |
| `?handleException@ThreadEngineBase@QtConcurrent@@AEAAXAEBVQException@@@Z` | `0x1200` |  |
| `?isCanceled@ThreadEngineBase@QtConcurrent@@QEAA_NXZ` | `0x1220` |  |
| `?isProgressReportingEnabled@ThreadEngineBase@QtConcurrent@@QEAA_NXZ` | `0x1240` |  |
| `?run@ThreadEngineBase@QtConcurrent@@EEAAXXZ` | `0x12b0` |  |
| `?setProgressRange@ThreadEngineBase@QtConcurrent@@QEAAXHH@Z` | `0x1350` |  |
| `?setProgressValue@ThreadEngineBase@QtConcurrent@@QEAAXH@Z` | `0x1370` |  |
| `?shouldStartThread@ThreadEngineBase@QtConcurrent@@MEAA_NXZ` | `0x1390` |  |
| `?shouldThrottleThread@ThreadEngineBase@QtConcurrent@@MEAA_NXZ` | `0x13c0` |  |
| `?start@ThreadEngineBase@QtConcurrent@@MEAAXXZ` | `0x11f0` |  |
| `?startBlocking@ThreadEngineBase@QtConcurrent@@QEAAXXZ` | `0x13e0` |  |
| `?startSingleThreaded@ThreadEngineBase@QtConcurrent@@QEAAXXZ` | `0x14f0` |  |
| `?startThread@ThreadEngineBase@QtConcurrent@@QEAAXXZ` | `0x1520` |  |
| `?startThreadInternal@ThreadEngineBase@QtConcurrent@@AEAA_NXZ` | `0x1530` |  |
| `?startThreads@ThreadEngineBase@QtConcurrent@@AEAAXXZ` | `0x15b0` |  |
| `?threadExit@ThreadEngineBase@QtConcurrent@@AEAAXXZ` | `0x1660` |  |
| `?threadFunction@ThreadEngineBase@QtConcurrent@@MEAA?AW4ThreadFunctionResult@2@XZ` | `0x16e0` |  |
| `?threadThrottleExit@ThreadEngineBase@QtConcurrent@@AEAA_NXZ` | `0x16f0` |  |
| `?timeAfterUser@BlockSizeManager@QtConcurrent@@QEAAXXZ` | `0x2920` |  |

RTTI sample: `.?AVQException@@`, `.?AVQRunnable@@`, `.?AVThreadEngineBase@QtConcurrent@@`, `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

## Qt5Core.dll

Path: `game/bin/win64/Qt5Core.dll`  
SHA-256: `b72018655360463896edbd86b120be6dfa7235ae8a0aaa728165cb496573acb9`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x2db8fc`.  
Exports: 8368; RTTI types: 262; x64 unwind entries: 12323.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\lib\Qt5Core.pdb`; GUID `529a2eb7-ade5-49a1-ba8f-ad1915695fe7`; age 1; lookup key `529A2EB7ADE549A1BA8FAD1915695FE71`.

Normal import modules: `MPR.dll`, `USERENV.dll`, `VERSION.dll`, `NETAPI32.dll`, `WS2_32.dll`, `ADVAPI32.dll`, `KERNEL32.dll`, `ole32.dll`, `SHELL32.dll`, `USER32.dll`, `WINMM.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??$qbswap@$01@@YAPEAXPEBX_JPEAX@Z` | `0x17cb0` |  |
| `??$qbswap@$03@@YAPEAXPEBX_JPEAX@Z` | `0x17d30` |  |
| `??$qbswap@$07@@YAPEAXPEBX_JPEAX@Z` | `0x17d80` |  |
| `??0?$QList@VQItemSelectionRange@@@@QEAA@$$QEAV0@@Z` | `0x19a0d0` |  |
| `??0?$QList@VQItemSelectionRange@@@@QEAA@AEBV0@@Z` | `0x19a0f0` |  |
| `??0?$QList@VQItemSelectionRange@@@@QEAA@V?$initializer_list@VQItemSelectionRange@@@std@@@Z` | `0x19a180` |  |
| `??0?$QList@VQItemSelectionRange@@@@QEAA@XZ` | `0x19a1a0` |  |
| `??0?$QVector@VQPoint@@@@QEAA@$$QEAV0@@Z` | `0x12f0` |  |
| `??0?$QVector@VQPoint@@@@QEAA@AEBV0@@Z` | `0x642d0` |  |
| `??0?$QVector@VQPoint@@@@QEAA@H@Z` | `0x643c0` |  |
| `??0?$QVector@VQPoint@@@@QEAA@HAEBVQPoint@@@Z` | `0x64460` |  |
| `??0?$QVector@VQPoint@@@@QEAA@U?$QArrayDataPointerRef@VQPoint@@@@@Z` | `0x1330` |  |
| `??0?$QVector@VQPoint@@@@QEAA@V?$initializer_list@VQPoint@@@std@@@Z` | `0x64510` |  |
| `??0?$QVector@VQPoint@@@@QEAA@XZ` | `0x1340` |  |
| `??0?$QVector@VQPointF@@@@QEAA@$$QEAV0@@Z` | `0x12f0` |  |
| `??0?$QVector@VQPointF@@@@QEAA@AEBV0@@Z` | `0x645b0` |  |
| `??0?$QVector@VQPointF@@@@QEAA@H@Z` | `0x646b0` |  |
| `??0?$QVector@VQPointF@@@@QEAA@HAEBVQPointF@@@Z` | `0x64750` |  |
| `??0?$QVector@VQPointF@@@@QEAA@U?$QArrayDataPointerRef@VQPointF@@@@@Z` | `0x1330` |  |
| `??0?$QVector@VQPointF@@@@QEAA@V?$initializer_list@VQPointF@@@std@@@Z` | `0x64800` |  |
| `??0?$QVector@VQPointF@@@@QEAA@XZ` | `0x1340` |  |
| `??0?$QVector@VQXmlStreamAttribute@@@@QEAA@$$QEAV0@@Z` | `0x12f0` |  |
| `??0?$QVector@VQXmlStreamAttribute@@@@QEAA@AEBV0@@Z` | `0x24cb50` |  |
| `??0?$QVector@VQXmlStreamAttribute@@@@QEAA@H@Z` | `0x24cc30` |  |
| `??0?$QVector@VQXmlStreamAttribute@@@@QEAA@HAEBVQXmlStreamAttribute@@@Z` | `0x24cd00` |  |
| `??0?$QVector@VQXmlStreamAttribute@@@@QEAA@U?$QArrayDataPointerRef@VQXmlStreamAttribute@@@@@Z` | `0x1330` |  |
| `??0?$QVector@VQXmlStreamAttribute@@@@QEAA@V?$initializer_list@VQXmlStreamAttribute@@@std@@@Z` | `0x24ce00` |  |
| `??0?$QVector@VQXmlStreamAttribute@@@@QEAA@XZ` | `0x1340` |  |
| `??0Connection@QMetaObject@@AEAA@PEAX@Z` | `0x1330` |  |
| `??0Connection@QMetaObject@@QEAA@$$QEAV01@@Z` | `0xe4a0` |  |

RTTI sample: `.?AUBackEase@@`, `.?AUBezierEase@@`, `.?AUBounceEase@@`, `.?AUElasticEase@@`, `.?AUIFileOperationProgressSink@@`, `.?AUIUnknown@@`, `.?AUQAbstractDynamicMetaObject@@`, `.?AUQDynamicMetaObjectData@@`, `.?AUQFactoryInterface@@`, `.?AUQMetaObject@@`, `.?AUTCBEase@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$ctype@D@std@@`, `.?AVDefaultStateTransition@?A0x084053f4@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `DataSizeBase1000`, `GB18030`, `SystemFunction036`.

## Qt5Gui.dll

Path: `game/bin/win64/Qt5Gui.dll`  
SHA-256: `2ea37f7655223c5cdf1f28616c601ca58bf8b215e371d6f612cfa1aa34517fbf`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3c8ea8`.  
Exports: 25056; RTTI types: 469; x64 unwind entries: 20383.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\lib\Qt5Gui.pdb`; GUID `74525e54-01ec-48f4-9c67-e7a5a07b8000`; age 1; lookup key `74525E5401EC48F49C67E7A5A07B80001`.

Normal import modules: `d3d11.dll`, `dxgi.dll`, `Qt5Core.dll`, `GDI32.dll`, `ole32.dll`, `USER32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??$handleApplicationStateChanged@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXW4ApplicationState@Qt@@_N@Z` | `0x27690` |  |
| `??$handleApplicationStateChanged@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXW4ApplicationState@Qt@@_N@Z` | `0x276e0` |  |
| `??$handleApplicationStateChanged@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXW4ApplicationState@Qt@@_N@Z` | `0x27740` |  |
| `??$handleApplicationTermination@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NXZ` | `0x27790` |  |
| `??$handleApplicationTermination@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NXZ` | `0x277c0` |  |
| `??$handleApplicationTermination@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NXZ` | `0x27810` |  |
| `??$handleCloseEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@@Z` | `0x27840` |  |
| `??$handleCloseEvent@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@@Z` | `0x278a0` |  |
| `??$handleCloseEvent@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@@Z` | `0x27920` |  |
| `??$handleEnterEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQPointF@@1@Z` | `0x27980` |  |
| `??$handleEnterEvent@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQPointF@@1@Z` | `0x27a80` |  |
| `??$handleEnterEvent@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQPointF@@1@Z` | `0x27b90` |  |
| `??$handleExposeEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQRegion@@@Z` | `0x27c90` |  |
| `??$handleExposeEvent@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQRegion@@@Z` | `0x27d60` |  |
| `??$handleExposeEvent@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQRegion@@@Z` | `0x27e40` |  |
| `??$handleGeometryChange@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQRect@@@Z` | `0x27f10` |  |
| `??$handleGeometryChange@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQRect@@@Z` | `0x28030` |  |
| `??$handleGeometryChange@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@AEBVQRect@@@Z` | `0x28170` |  |
| `??$handleKeyEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@KW4Type@QEvent@@HV?$QFlags@W4KeyboardModifier@Qt@@@@AEBVQString@@_NG@Z` | `0x28290` |  |
| `??$handleKeyEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@W4Type@QEvent@@HV?$QFlags@W4KeyboardModifier@Qt@@@@AEBVQString@@_NG@Z` | `0x28360` |  |
| `??$handleKeyEvent@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@KW4Type@QEvent@@HV?$QFlags@W4KeyboardModifier@Qt@@@@AEBVQString@@_NG@Z` | `0x283e0` |  |
| `??$handleKeyEvent@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@W4Type@QEvent@@HV?$QFlags@W4KeyboardModifier@Qt@@@@AEBVQString@@_NG@Z` | `0x284d0` |  |
| `??$handleKeyEvent@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@KW4Type@QEvent@@HV?$QFlags@W4KeyboardModifier@Qt@@@@AEBVQString@@_NG@Z` | `0x28550` |  |
| `??$handleKeyEvent@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@W4Type@QEvent@@HV?$QFlags@W4KeyboardModifier@Qt@@@@AEBVQString@@_NG@Z` | `0x28620` |  |
| `??$handleLeaveEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@@Z` | `0x286a0` |  |
| `??$handleLeaveEvent@UDefaultDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@@Z` | `0x28700` |  |
| `??$handleLeaveEvent@USynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SAXPEAVQWindow@@@Z` | `0x28780` |  |
| `??$handleMouseEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@AEBVQPointF@@1V?$QFlags@W4MouseButton@Qt@@@@V?$QFlags@W4KeyboardModifier@Qt@@@@W4MouseEventSource@Qt@@@Z` | `0x287e0` |  |
| `??$handleMouseEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@AEBVQPointF@@1V?$QFlags@W4MouseButton@Qt@@@@W4MouseButton@Qt@@W4Type@QEvent@@V?$QFlags@W4KeyboardModifier@Qt@@@@W4MouseEventSource@5@@Z` | `0x28860` |  |
| `??$handleMouseEvent@UAsynchronousDelivery@QWindowSystemInterface@@@QWindowSystemInterface@@SA_NPEAVQWindow@@KAEBVQPointF@@1V?$QFlags@W4MouseButton@Qt@@@@V?$QFlags@W4KeyboardModifier@Qt@@@@W4MouseEventSource@Qt@@@Z` | `0x288f0` |  |

RTTI sample: `.?AUPixmapEntry@@`, `.?AUQD3D11Buffer@@`, `.?AUQD3D11CommandBuffer@@`, `.?AUQD3D11ComputePipeline@@`, `.?AUQD3D11GraphicsPipeline@@`, `.?AUQD3D11ReferenceRenderTarget@@`, `.?AUQD3D11RenderBuffer@@`, `.?AUQD3D11RenderPassDescriptor@@`, `.?AUQD3D11Sampler@@`, `.?AUQD3D11ShaderResourceBindings@@`, `.?AUQD3D11SwapChain@@`, `.?AUQD3D11Texture@@`, `.?AUQD3D11TextureRenderTarget@@`, `.?AUQGles2Buffer@@`, `.?AUQGles2CommandBuffer@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SF010000`, `SF020000`, `SF030000`, `SF040000`, `SF050000`, `SF060000`, `SF070000`, `SF080000`, `SF090000`, `SF100000`, `SF110000`, `SF190000`.

## Qt5Widgets.dll

Path: `game/bin/win64/Qt5Widgets.dll`  
SHA-256: `e619823dad8db291f9da78eb81938da3f74a08d2318d660a8f2f9757b862dd0b`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x336ddc`.  
Exports: 9053; RTTI types: 655; x64 unwind entries: 17944.

PDB reference: `D:\dev\thirdparty\lgpl\qt_source2\valve\qtbase\lib\Qt5Widgets.pdb`; GUID `5281e698-685b-49d5-bf78-6cb4e1e6e54a`; age 1; lookup key `5281E698685B49D5BF786CB4E1E6E54A1`.

Normal import modules: `Qt5Gui.dll`, `Qt5Core.dll`, `UxTheme.dll`, `dwmapi.dll`, `SHELL32.dll`, `GDI32.dll`, `ole32.dll`, `USER32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0QAbstractButton@@IEAA@AEAVQAbstractButtonPrivate@@PEAVQWidget@@@Z` | `0xde380` |  |
| `??0QAbstractButton@@QEAA@PEAVQWidget@@@Z` | `0xde3c0` |  |
| `??0QAbstractGraphicsShapeItem@@IEAA@AEAVQAbstractGraphicsShapeItemPrivate@@PEAVQGraphicsItem@@@Z` | `0x2ab9b0` |  |
| `??0QAbstractGraphicsShapeItem@@QEAA@PEAVQGraphicsItem@@@Z` | `0x2ab9f0` |  |
| `??0QAbstractItemDelegate@@IEAA@AEAVQObjectPrivate@@PEAVQObject@@@Z` | `0x246200` |  |
| `??0QAbstractItemDelegate@@QEAA@PEAVQObject@@@Z` | `0x246230` |  |
| `??0QAbstractItemView@@IEAA@AEAVQAbstractItemViewPrivate@@PEAVQWidget@@@Z` | `0x229910` |  |
| `??0QAbstractItemView@@QEAA@PEAVQWidget@@@Z` | `0x229950` |  |
| `??0QAbstractScrollArea@@IEAA@AEAVQAbstractScrollAreaPrivate@@PEAVQWidget@@@Z` | `0xd9610` |  |
| `??0QAbstractScrollArea@@QEAA@PEAVQWidget@@@Z` | `0xd9650` |  |
| `??0QAbstractScrollAreaPrivate@@QEAA@XZ` | `0xd96b0` |  |
| `??0QAbstractSlider@@IEAA@AEAVQAbstractSliderPrivate@@PEAVQWidget@@@Z` | `0xe0e70` |  |
| `??0QAbstractSlider@@QEAA@PEAVQWidget@@@Z` | `0xe0ea0` |  |
| `??0QAbstractSpinBox@@IEAA@AEAVQAbstractSpinBoxPrivate@@PEAVQWidget@@@Z` | `0x1793c0` |  |
| `??0QAbstractSpinBox@@QEAA@PEAVQWidget@@@Z` | `0x179400` |  |
| `??0QAccessibleWidget@@QEAA@PEAVQWidget@@W4Role@QAccessible@@AEBVQString@@@Z` | `0x213880` |  |
| `??0QAction@@IEAA@AEAVQActionPrivate@@PEAVQObject@@@Z` | `0x2150` |  |
| `??0QAction@@QEAA@AEBVQIcon@@AEBVQString@@PEAVQObject@@@Z` | `0x2220` |  |
| `??0QAction@@QEAA@AEBVQString@@PEAVQObject@@@Z` | `0x22b0` |  |
| `??0QAction@@QEAA@PEAVQObject@@@Z` | `0x2320` |  |
| `??0QActionGroup@@QEAA@PEAVQObject@@@Z` | `0xc090` |  |
| `??0QActionPrivate@@QEAA@XZ` | `0x2370` |  |
| `??0QApplication@@QEAA@AEAHPEAPEADH@Z` | `0xd250` |  |
| `??0QApplicationPrivate@@QEAA@AEAHPEAPEADH@Z` | `0xd2d0` |  |
| `??0QBlendStyleAnimation@@QEAA@W4Type@0@PEAVQObject@@@Z` | `0xbe1c0` |  |
| `??0QBoxLayout@@QEAA@W4Direction@0@PEAVQWidget@@@Z` | `0x213b0` |  |
| `??0QButtonGroup@@QEAA@PEAVQObject@@@Z` | `0xe2470` |  |
| `??0QCalendarWidget@@QEAA@PEAVQWidget@@@Z` | `0xe4410` |  |
| `??0QCheckBox@@QEAA@AEBVQString@@PEAVQWidget@@@Z` | `0xec6c0` |  |
| `??0QCheckBox@@QEAA@PEAVQWidget@@@Z` | `0xec710` |  |

RTTI sample: `.?AUAnchorData@QtGraphicsAnchorLayout@@`, `.?AUAnchorVertex@QtGraphicsAnchorLayout@@`, `.?AUAnchorVertexPair@QtGraphicsAnchorLayout@@`, `.?AUAnimatedOperation@QTreeViewPrivate@@`, `.?AUParallelAnchorData@QtGraphicsAnchorLayout@@`, `.?AUQEffects@@`, `.?AUQSimplexVariable@@`, `.?AUSequentialAnchorData@QtGraphicsAnchorLayout@@`, `.?AUTabBarAnimation@Tab@QTabBarPrivate@@`, `.?AV<lambda_c6b2a79855a1ec73a15b68bdd05839ca>@@`, `.?AV?$ControlElement@VControlLabel@QMdi@@@QMdi@@`, `.?AV?$ControlElement@VControllerWidget@QMdi@@@QMdi@@`, `.?AV?$QMainWindowLayoutSeparatorHelper@VQDockWidgetGroupLayout@@@@`, `.?AV?$QMainWindowLayoutSeparatorHelper@VQMainWindowLayout@@@@`, `.?AV?$_Func_base@VQPoint@@AEBVQSize@@@std@@`.

## qtadvanceddocking-qt5.dll

Path: `game/bin/win64/qtadvanceddocking-qt5.dll`  
SHA-256: `2cf5fd2ad49534111f18ff23732375f59017199f73c82d21d2bd022d797af9d1`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1078`.  
Exports: 739; RTTI types: 51; x64 unwind entries: 1646.

PDB reference: `D:\dev\thirdpartycode\lgpl\qt_ads\valve\x64\bin\RelWithDebInfo\qtadvanceddocking-qt5.pdb`; GUID `fa2454ad-62f8-44d0-a756-512fbd18a38c`; age 1; lookup key `FA2454AD62F844D0A756512FBD18A38C1`.

Normal import modules: `Qt5Widgets.dll`, `Qt5Gui.dll`, `Qt5Core.dll`, `KERNEL32.dll`, `USER32.dll`, `MSVCP140.dll`, `VCRUNTIME140_1.dll`, `VCRUNTIME140.dll`, `api-ms-win-crt-heap-l1-1-0.dll`, `api-ms-win-crt-runtime-l1-1-0.dll`, `api-ms-win-crt-string-l1-1-0.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0CAutoHideDockContainer@ads@@QEAA@PEAVCDockWidget@1@W4SideBarLocation@1@PEAVCDockContainerWidget@1@@Z` | `0x1bd6` |  |
| `??0CAutoHideSideBar@ads@@QEAA@PEAVCDockContainerWidget@1@W4SideBarLocation@1@@Z` | `0x1dca` |  |
| `??0CAutoHideTab@ads@@QEAA@PEAVQWidget@@@Z` | `0x11a9` |  |
| `??0CDockAreaTabBar@ads@@QEAA@PEAVCDockAreaWidget@1@@Z` | `0x28e2` |  |
| `??0CDockAreaTitleBar@ads@@QEAA@PEAVCDockAreaWidget@1@@Z` | `0x1e01` |  |
| `??0CDockAreaWidget@ads@@QEAA@PEAVCDockManager@1@PEAVCDockContainerWidget@1@@Z` | `0x29c8` |  |
| `??0CDockComponentsFactory@ads@@QEAA@AEBV01@@Z` | `0x1b36` |  |
| `??0CDockComponentsFactory@ads@@QEAA@XZ` | `0x1604` |  |
| `??0CDockContainerWidget@ads@@QEAA@PEAVCDockManager@1@PEAVQWidget@@@Z` | `0x1654` |  |
| `??0CDockFocusController@ads@@QEAA@PEAVCDockManager@1@@Z` | `0x20a4` |  |
| `??0CDockManager@ads@@QEAA@PEAVQWidget@@@Z` | `0x25a9` |  |
| `??0CDockOverlay@ads@@QEAA@PEAVQWidget@@W4eMode@01@@Z` | `0x1c67` |  |
| `??0CDockSplitter@ads@@QEAA@PEAVQWidget@@@Z` | `0x18e8` |  |
| `??0CDockSplitter@ads@@QEAA@W4Orientation@Qt@@PEAVQWidget@@@Z` | `0x1c08` |  |
| `??0CDockWidget@ads@@QEAA@AEBVQString@@PEAVQWidget@@@Z` | `0x1e15` |  |
| `??0CDockWidget@ads@@QEAA@PEAVCDockManager@1@AEBVQString@@PEAVQWidget@@@Z` | `0x2941` |  |
| `??0CDockWidgetTab@ads@@QEAA@PEAVCDockWidget@1@PEAVQWidget@@@Z` | `0x27ac` |  |
| `??0CElidingLabel@ads@@QEAA@AEBVQString@@PEAVQWidget@@V?$QFlags@W4WindowType@Qt@@@@@Z` | `0x1bd1` |  |
| `??0CElidingLabel@ads@@QEAA@PEAVQWidget@@V?$QFlags@W4WindowType@Qt@@@@@Z` | `0x146a` |  |
| `??0CFloatingDockContainer@ads@@QEAA@PEAVCDockAreaWidget@1@@Z` | `0x2be4` |  |
| `??0CFloatingDockContainer@ads@@QEAA@PEAVCDockManager@1@@Z` | `0x1802` |  |
| `??0CFloatingDockContainer@ads@@QEAA@PEAVCDockWidget@1@@Z` | `0x2581` |  |
| `??0CIconProvider@ads@@QEAA@AEBV01@@Z` | `0x2b35` |  |
| `??0CIconProvider@ads@@QEAA@XZ` | `0x1f78` |  |
| `??0CPushButton@ads@@IEAA@AEAVQPushButtonPrivate@@@Z` | `0x1ae6` |  |
| `??0CPushButton@ads@@IEAA@AEAVQPushButtonPrivate@@PEAVQWidget@@@Z` | `0x2379` |  |
| `??0CPushButton@ads@@QEAA@AEBVQIcon@@AEBVQString@@@Z` | `0x2185` |  |
| `??0CPushButton@ads@@QEAA@AEBVQIcon@@AEBVQString@@PEAVQWidget@@@Z` | `0x1de8` |  |
| `??0CPushButton@ads@@QEAA@AEBVQString@@@Z` | `0x1b59` |  |
| `??0CPushButton@ads@@QEAA@AEBVQString@@PEAVQWidget@@@Z` | `0x2af9` |  |

RTTI sample: `.?AVCAutoHideDockContainer@ads@@`, `.?AVCAutoHideSideBar@ads@@`, `.?AVCAutoHideTab@ads@@`, `.?AVCDockAreaTabBar@ads@@`, `.?AVCDockAreaTitleBar@ads@@`, `.?AVCDockAreaWidget@ads@@`, `.?AVCDockComponentsFactory@ads@@`, `.?AVCDockContainerWidget@ads@@`, `.?AVCDockFocusController@ads@@`, `.?AVCDockManager@ads@@`, `.?AVCDockOverlay@ads@@`, `.?AVCDockOverlayCross@ads@@`, `.?AVCDockSplitter@ads@@`, `.?AVCDockWidget@ads@@`, `.?AVCDockWidgetTab@ads@@`.

## rendersystemdx11.dll

Path: `game/bin/win64/rendersystemdx11.dll`  
SHA-256: `c43386d01b87955fa8b4a7be3510777c511a98b92343130fb1e1b3bbd62fcec2`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1ad128`.  
Exports: 49; RTTI types: 271; x64 unwind entries: 7329.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\rendersystem\_vpc_rendersystemdx11\default\win64\_msvc_\Retail\rendersystemdx11.pdb`; GUID `83a83230-fc31-4d49-aaf8-2fa5fd7496f1`; age 10; lookup key `83A83230FC314D49AAF82FA5FD7496F1A`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `amd_ags_x64.dll`, `d3d11.dll`, `d3d9.dll`, `D3DCOMPILER_47.dll`, `dxgi.dll`, `tier0.dll`, `KERNEL32.dll`, `SHLWAPI.dll`, `SETUPAPI.dll`, `CFGMGR32.dll`, `ole32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x82740` |  |
| `CreateInterface` | `0x15f950` |  |
| `ExtractModuleMetadata` | `0x82ca0` |  |
| `GetResourceManifestCount` | `0x167640` |  |
| `GetResourceManifests` | `0x167650` |  |
| `InstallSchemaBindings` | `0x167230` |  |
| `NVSDK_NGX_D3D11_AllocateParameters` | `0x8c050` |  |
| `NVSDK_NGX_D3D11_CreateFeature` | `0x8c0b0` |  |
| `NVSDK_NGX_D3D11_DestroyParameters` | `0x8c130` |  |
| `NVSDK_NGX_D3D11_EvaluateFeature` | `0x8c190` |  |
| `NVSDK_NGX_D3D11_EvaluateFeature_C` | `0x8c200` |  |
| `NVSDK_NGX_D3D11_GetCapabilityParameters` | `0x8c240` |  |
| `NVSDK_NGX_D3D11_GetFeatureRequirements` | `0x8c2a0` |  |
| `NVSDK_NGX_D3D11_GetParameters` | `0x8c380` |  |
| `NVSDK_NGX_D3D11_GetScratchBufferSize` | `0x8c3d0` |  |
| `NVSDK_NGX_D3D11_ReleaseFeature` | `0x8c550` |  |
| `NVSDK_NGX_D3D11_Shutdown` | `0x8c5a0` |  |
| `NVSDK_NGX_D3D11_Shutdown1` | `0x8c5b0` |  |
| `NVSDK_NGX_Parameter_GetD` | `0x8b0c0` |  |
| `NVSDK_NGX_Parameter_GetD3d11Resource` | `0x8b120` |  |
| `NVSDK_NGX_Parameter_GetD3d12Resource` | `0x8b180` |  |
| `NVSDK_NGX_Parameter_GetF` | `0x8b1e0` |  |
| `NVSDK_NGX_Parameter_GetI` | `0x8b240` |  |
| `NVSDK_NGX_Parameter_GetUI` | `0x8b2a0` |  |
| `NVSDK_NGX_Parameter_GetULL` | `0x8b300` |  |
| `NVSDK_NGX_Parameter_GetVoidPointer` | `0x8b360` |  |
| `NVSDK_NGX_Parameter_SetD` | `0x8b3c0` |  |
| `NVSDK_NGX_Parameter_SetD3d11Resource` | `0x8b410` |  |
| `NVSDK_NGX_Parameter_SetD3d12Resource` | `0x8b470` |  |
| `NVSDK_NGX_Parameter_SetF` | `0x8b4d0` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUID3DInclude@@`, `.?AU_Anon@?$_Associated_state@H@std@@`, `.?AU_Anon@?$_Associated_state@I@std@@`, `.?AU_Crt_new_delete@std@@`, `.?AU_TaskProcHandle@details@Concurrency@@`, `.?AU_Task_impl_base@details@Concurrency@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## rendersystemempty.dll

Path: `game/bin/win64/rendersystemempty.dll`  
SHA-256: `bbe03046212f67ed61b5fe8ef1d9af6f6b62d977878efebfe64ba649e8374983`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x10e2f8`.  
Exports: 6; RTTI types: 188; x64 unwind entries: 4465.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\rendersystem\_vpc_rendersystemempty\default\win64\_msvc_\Retail\rendersystemempty.pdb`; GUID `5e1c53fb-552e-43b3-94be-6d2993de64fe`; age 10; lookup key `5E1C53FB552E43B394BE6D2993DE64FEA`.

Normal import modules: `ADVAPI32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x2b870` |  |
| `CreateInterface` | `0xf91d0` |  |
| `ExtractModuleMetadata` | `0x2bdd0` |  |
| `GetResourceManifestCount` | `0x100b30` |  |
| `GetResourceManifests` | `0x100b40` |  |
| `InstallSchemaBindings` | `0x100720` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AU_Anon@?$_Associated_state@H@std@@`, `.?AU_Anon@?$_Associated_state@I@std@@`, `.?AU_Crt_new_delete@std@@`, `.?AU_TaskProcHandle@details@Concurrency@@`, `.?AU_Task_impl_base@details@Concurrency@@`, `.?AUctype_base@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## rendersystemvulkan.dll

Path: `game/bin/win64/rendersystemvulkan.dll`  
SHA-256: `39ffdb87b4983c1726e1a387c888928181371bbadcf096a981aba9a25f8faafe`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x26fc9c`.  
Exports: 58; RTTI types: 335; x64 unwind entries: 10455.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\rendersystem\_vpc_rendersystemvulkan\default\win64\_msvc_\Retail\rendersystemvulkan.pdb`; GUID `83159f95-0536-45bc-aef1-62825688c93d`; age 11; lookup key `83159F95053645BCAEF162825688C93DB`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `GFSDK_Aftermath_Lib.x64.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x12e490` |  |
| `CreateInterface` | `0x240f20` |  |
| `ExtractModuleMetadata` | `0x12ea10` |  |
| `GetResourceManifestCount` | `0x242b20` |  |
| `GetResourceManifests` | `0x242b30` |  |
| `InstallSchemaBindings` | `0x242710` |  |
| `NVSDK_NGX_Parameter_GetD` | `0x1388b0` |  |
| `NVSDK_NGX_Parameter_GetD3d11Resource` | `0x138910` |  |
| `NVSDK_NGX_Parameter_GetD3d12Resource` | `0x138970` |  |
| `NVSDK_NGX_Parameter_GetF` | `0x1389d0` |  |
| `NVSDK_NGX_Parameter_GetI` | `0x138a30` |  |
| `NVSDK_NGX_Parameter_GetUI` | `0x138a90` |  |
| `NVSDK_NGX_Parameter_GetULL` | `0x138af0` |  |
| `NVSDK_NGX_Parameter_GetVoidPointer` | `0x138b50` |  |
| `NVSDK_NGX_Parameter_SetD` | `0x138bb0` |  |
| `NVSDK_NGX_Parameter_SetD3d11Resource` | `0x138c00` |  |
| `NVSDK_NGX_Parameter_SetD3d12Resource` | `0x138c60` |  |
| `NVSDK_NGX_Parameter_SetF` | `0x138cc0` |  |
| `NVSDK_NGX_Parameter_SetI` | `0x138d10` |  |
| `NVSDK_NGX_Parameter_SetUI` | `0x138d70` |  |
| `NVSDK_NGX_Parameter_SetULL` | `0x138dd0` |  |
| `NVSDK_NGX_Parameter_SetVoidPointer` | `0x138e30` |  |
| `NVSDK_NGX_UpdateFeature` | `0x13da60` |  |
| `NVSDK_NGX_VULKAN_AllocateParameters` | `0x139c10` |  |
| `NVSDK_NGX_VULKAN_CreateFeature` | `0x139c70` |  |
| `NVSDK_NGX_VULKAN_CreateFeature1` | `0x139cf0` |  |
| `NVSDK_NGX_VULKAN_DestroyParameters` | `0x139d80` |  |
| `NVSDK_NGX_VULKAN_EvaluateFeature` | `0x139de0` |  |
| `NVSDK_NGX_VULKAN_EvaluateFeature_C` | `0x139e50` |  |
| `NVSDK_NGX_VULKAN_GetCapabilityParameters` | `0x139e90` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUConcurrentDatabase@Fossilize@@`, `.?AUPerThreadDataBase_t@@`, `.?AUPerThreadDataVulkan_t@@`, `.?AUStreamArchive@Fossilize@@`, `.?AU_Anon@?$_Associated_state@I@std@@`, `.?AU_Crt_new_delete@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## resourcecompiler.dll

Path: `game/bin/win64/resourcecompiler.dll`  
SHA-256: `7091085d260786ca6963b78b9139fac086355ba98b18aac1754f4cc12c69f6b2`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x201347c`.  
Exports: 6; RTTI types: 16673; x64 unwind entries: 147808.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\resourcecompiler\_vpc_resourcecompiler\default\win64\_msvc_\Retail\resourcecompiler.pdb`; GUID `d6bf55da-c502-404a-b119-7f8e5d86dac2`; age 19; lookup key `D6BF55DAC502404AB1197F8E5D86DAC213`.

Normal import modules: `libfbxsdk_2020_3_1.dll`, `embree3.dll`, `WS2_32.dll`, `steam_api64.dll`, `tier0.dll`, `v8.dll`, `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x1d0de0` |  |
| `CreateInterface` | `0x1ec7970` |  |
| `ExtractModuleMetadata` | `0x1d1360` |  |
| `GetResourceManifestCount` | `0x1ed80e0` |  |
| `GetResourceManifests` | `0x1ed80f0` |  |
| `InstallSchemaBindings` | `0x1ed9430` |  |

RTTI sample: `.?AU?$AccessorT@VCActionComponent@@VCAnimGraphDoc_Action@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCAnimGraphDoc_ComponentManager@@VCAnimGraphDoc_Component@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCAnimGraphDoc_ConditionContainer@@VCAnimGraphDoc_Condition@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCAnimGraphDoc_MotionParameterManager@@VCAnimGraphDoc_MotionParameter@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCAnimGraphDoc_ParameterManager@@VIAnimParameter@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCAnimGraphDoc_SingleFrameNode@@VCAnimGraphDoc_Action@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCAnimGraphDoc_State@@UCStateAction@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCAnimGraphDoc_TagManager@@VIAnimTag@@@ArrayOperator@Attribute@@`, `.?AU?$AccessorT@VCMovementComponent@@VCAnimGraphDoc_Motor@@@ArrayOperator@Attribute@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@MHV?$CUtlVectorMemory_Growable@MH$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@UCAnimGraphDoc_ParamSpanSample@@HV?$CUtlVectorMemory_Growable@UCAnimGraphDoc_ParamSpanSample@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@UCBlendNodeChild@@HV?$CUtlVectorMemory_Growable@UCBlendNodeChild@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@UCRigidBodyWeight@@HV?$CUtlVectorMemory_Growable@UCRigidBodyWeight@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@UCStateAction@@HV?$CUtlVectorMemory_Growable@UCStateAction@@H$0A@@@@@@Detail@Reflection@@`, `.?AU?$ArrayWrapper@V?$CUtlVector@UCWeightPreview@@HV?$CUtlVectorMemory_Growable@UCWeightPreview@@H$0A@@@@@@Detail@Reflection@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## resourcesystem.dll

Path: `game/bin/win64/resourcesystem.dll`  
SHA-256: `075112bd12ccf50da3e3add370acad7f92ec2e67dca0efb31292ac50169b8f21`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x484d0`.  
Exports: 6; RTTI types: 79; x64 unwind entries: 1864.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\resourcesystem\_vpc_resourcesystem\default\win64\_msvc_\Retail\resourcesystem.pdb`; GUID `2c667171-21aa-4bda-bdd0-d074e991382f`; age 10; lookup key `2C66717121AA4BDABDD0D074E991382FA`.

Normal import modules: `ADVAPI32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x29ee0` |  |
| `CreateInterface` | `0x46f40` |  |
| `ExtractModuleMetadata` | `0x2a330` |  |
| `GetResourceManifestCount` | `0x47c10` |  |
| `GetResourceManifests` | `0x47c20` |  |
| `InstallSchemaBindings` | `0x47800` |  |

RTTI sample: `.?AU?$require_make_shared@VCAsyncResourceDataRequest@@@@`, `.?AV?$CBaseAppSystem@VIResourceSystem@@@@`, `.?AV?$CConCommandMemberAccessor@VCResourceSystemProfiler@@@@`, `.?AV?$CDefaultKV3TransferResourceTypeManager@$0HEHDGFHEHG@@@`, `.?AV?$CKV3ResourceTypeManager@$0GBHEGBGEHG@@@`, `.?AV?$CKV3ResourceTypeManager@$0HEHDGFHEHG@@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted1@VIRefCounted@@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AV?$CTier0AppSystem@VIResourceSystem@@$0A@@@`, `.?AV?$CTier1AppSystem@VIResourceSystem@@$0A@@@`, `.?AV?$CTier2AppSystem@VIResourceSystem@@$0A@@@`, `.?AV?$_Func_base@XPEAVIAsyncReadRequest2@@W4AsyncRequestStatus_t@@@std@@`, `.?AV?$_Func_impl_no_alloc@V<lambda_1>@?1??InitForFileRead@CAsyncResourceDataRequest@@AEAAXPEBUResourceBindingBase_t@@W4ResourceManifestLoadPriority_t@@_NPEBD@Z@XPEAVIAsyncReadRequest2@@W4AsyncRequestStatus_t@@@std@@`, `.?AV?$_Ref_count_obj2@VCAsyncResourceDataRequest@@@std@@`, `.?AV?$enable_shared_from_this@VCAsyncResourceDataRequest@@@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## scenefilecache.dll

Path: `game/bin/win64/scenefilecache.dll`  
SHA-256: `979b640bfc86620cc8d593df4f2c9b1460a9eae1b5456fbe488ac06470a7eb49`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xa358c`.  
Exports: 6; RTTI types: 400; x64 unwind entries: 3692.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\scenefilecache\_vpc_scenefilecache\default\win64\_msvc_\Retail\scenefilecache.pdb`; GUID `17224292-5113-4857-9ac0-2de9104b024e`; age 11; lookup key `17224292511348579AC02DE9104B024EB`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xc9f0` |  |
| `CreateInterface` | `0x9ca40` |  |
| `ExtractModuleMetadata` | `0xce40` |  |
| `GetResourceManifestCount` | `0xa0af0` |  |
| `GetResourceManifests` | `0xa0b00` |  |
| `InstallSchemaBindings` | `0xa0610` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIAppSystem@@@@`, `.?AV?$CBaseAppSystem@VIResponseRulesFileCache@@@@`, `.?AV?$CBaseAppSystem@VISceneFileCache@@@@`, `.?AV?$CBaseConcreteToolAttr@HVCChoreoEvent_ANIM_CLIP@@@@`, `.?AV?$CBaseConcreteToolAttr@HVCChoreoEvent_BaseLocalEvent@@@@`, `.?AV?$CBaseConcreteToolAttr@HVCChoreoEvent_LOOP@@@@`, `.?AV?$CBaseConcreteToolAttr@MVCChoreoEvent_ANIM_CLIP@@@@`, `.?AV?$CBaseConcreteToolAttr@MVCChoreoEvent_BaseLocalEvent@@@@`, `.?AV?$CBaseConcreteToolAttr@MVCChoreoEvent_CommonBase@@@@`, `.?AV?$CBaseConcreteToolAttr@MVCChoreoEvent_LOOKAT@@@@`, `.?AV?$CBaseConcreteToolAttr@MVCChoreoEvent_LOOP@@@@`, `.?AV?$CBaseConcreteToolAttr@MVCChoreoEvent_REQUIREMENTS@@@@`, `.?AV?$CBaseConcreteToolAttr@MVCChoreoEvent_SPEAK@@@@`, `.?AV?$CBaseConcreteToolAttr@V?$Vec3D@M@@VCChoreoEvent_LOCATOR@@@@`, `.?AV?$CBaseConcreteToolAttr@VCUtlString@@VCChoreoActor@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## scenesystem.dll

Path: `game/bin/win64/scenesystem.dll`  
SHA-256: `03525e62f9ccdc00a9e53c64c50cbc0601c26e33d052466bee02d64a21c9d5c1`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x4cc16c`.  
Exports: 6; RTTI types: 447; x64 unwind entries: 20232.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\scenesystem\_vpc_scenesystem\default\win64\_msvc_\Retail\scenesystem.pdb`; GUID `71276e81-4aee-468c-8589-0ae3f1030951`; age 13; lookup key `71276E814AEE468C85890AE3F1030951D`.

Normal import modules: `USER32.dll`, `tier0.dll`, `KERNEL32.dll`, `SHELL32.dll`, `IMM32.dll`, `GDI32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x161cd0` |  |
| `CreateInterface` | `0x4c7960` |  |
| `ExtractModuleMetadata` | `0x162190` |  |
| `GetResourceManifestCount` | `0x4c81d0` |  |
| `GetResourceManifests` | `0x4c81e0` |  |
| `InstallSchemaBindings` | `0x4c86d0` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$CBaseAppSystem@VIAppSystem@@@@`, `.?AV?$CBaseAppSystem@VISceneSystem@@@@`, `.?AV?$CBaseAppSystem@VISceneUtils@@@@`, `.?AV?$CBaseNonPooledSceneObjectDesc@VC6FaceSkyboxObject@@@@`, `.?AV?$CConCommandMemberAccessor@VCSceneSystem@@@@`, `.?AV?$CFixedSizeCircularBuffer@VCGPUProfHistoryFrame@@$0IA@H@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1??Thread_CullView@CSceneSystem@@AEAAXPEAVCSceneView@@PEAVCViewDrawList@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?BO@??GpuCullPrimitivesInView@CAggregateSceneObjectDesc@@UEAAXW4EGpuCullPhase@@PEAVIRenderContext@@PEBVISceneView@@PEBVISceneLayer@@PEAUCMeshDrawPrimitive_t@@IPEAUSceneSystemPerFrameStats_t@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?M@???$ParallelFor@$00V<lambda_1>@?1??OnBeginRenderingFrame@CAggregateSceneObjectDesc@@UEAAXXZ@@@YAXHHPEBD$$QEAV1?1??OnBeginRenderingFrame@CAggregateSceneObjectDesc@@UEAAXXZ@HHW4JobPriority_t@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelProcessorAbstract@V?$CParallelProcessor@PEAVCSceneObject@@V?$CFuncJobItemProcessor@PEAVCSceneObject@@@@$00@@@@`, `.?AV?$CParallelProcessorAbstract@V?$CParallelProcessor@UGenerateTileWorkUnit_t@@V?$CMemberFuncJobItemProcessor@UGenerateTileWorkUnit_t@@VCBuildSparseShadowTree@@V2@@@$00@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## schemasystem.dll

Path: `game/bin/win64/schemasystem.dll`  
SHA-256: `f88667df86e23531813e28a0fbd3b8cd66bb9f25e4a689f810c03dd328243479`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x39b3c`.  
Exports: 6; RTTI types: 43; x64 unwind entries: 1397.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\schemasystem\_vpc_schemasystem\default\win64\_msvc_\Retail\schemasystem.pdb`; GUID `349418a5-597d-45cd-bdc1-2453ab05fefb`; age 9; lookup key `349418A5597D45CDBDC12453AB05FEFB9`.

Normal import modules: `USER32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x267a0` |  |
| `CreateInterface` | `0x37560` |  |
| `ExtractModuleMetadata` | `0x26bf0` |  |
| `GetResourceManifestCount` | `0x391f0` |  |
| `GetResourceManifests` | `0x39200` |  |
| `InstallSchemaBindings` | `0x38de0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VISchemaSystem@@@@`, `.?AV?$CConCommandMemberAccessor@VCSchemaSystem@@@@`, `.?AV?$CTier0AppSystem@VISchemaSystem@@$0A@@@`, `.?AV?$CTier1AppSystem@VISchemaSystem@@$0A@@@`, `.?AV?$CTier2AppSystem@VISchemaSystem@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCExampleSchemaVData_PolymorphicBase@@`, `.?AVCExampleSchemaVData_PolymorphicDerivedA@@`, `.?AVCExampleSchemaVData_PolymorphicDerivedB@@`, `.?AVCKV3TransferContextBase@@`, `.?AVCKV3TransferSaveContext@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCSchemaInstallCallback@@`, `.?AVCSchemaRegistration@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## SDL3.dll

Path: `game/bin/win64/SDL3.dll`  
SHA-256: `e7c0c22aaadc2836242ba001aa488773eceb4609005a7f02464206ca59a86481`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1bdd9c`.  
Exports: 1303; RTTI types: 3; x64 unwind entries: 9429.

PDB reference: `C:\buildworker\sdl3_win32\build\dev\VisualC\x64\Release\SDL3.pdb`; GUID `c2affd70-622b-4884-bebd-af26bd409cb0`; age 1; lookup key `C2AFFD70622B4884BEBDAF26BD409CB01`.

Normal import modules: `SETUPAPI.dll`, `WINMM.dll`, `IMM32.dll`, `VERSION.dll`, `KERNEL32.dll`, `USER32.dll`, `GDI32.dll`, `ADVAPI32.dll`, `SHELL32.dll`, `ole32.dll`, `OLEAUT32.dll`, `HID.DLL`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `JNI_OnLoad` | `0x670b0` |  |
| `SDL_AcquireCameraFrame` | `0x670c0` |  |
| `SDL_AcquireGPUCommandBuffer` | `0x670d0` |  |
| `SDL_AcquireGPUSwapchainTexture` | `0x670e0` |  |
| `SDL_AddAtomicInt` | `0x670f0` |  |
| `SDL_AddAtomicU32` | `0x67100` |  |
| `SDL_AddEventWatch` | `0x67110` |  |
| `SDL_AddGamepadMapping` | `0x67120` |  |
| `SDL_AddGamepadMappingsFromFile` | `0x67130` |  |
| `SDL_AddGamepadMappingsFromIO` | `0x67140` |  |
| `SDL_AddHintCallback` | `0x67150` |  |
| `SDL_AddSurfaceAlternateImage` | `0x67160` |  |
| `SDL_AddTimer` | `0x67170` |  |
| `SDL_AddTimerNS` | `0x67180` |  |
| `SDL_AddVulkanRenderSemaphores` | `0x67190` |  |
| `SDL_AsyncIOFromFile` | `0x671a0` |  |
| `SDL_AttachVirtualJoystick` | `0x671b0` |  |
| `SDL_AudioDevicePaused` | `0x671c0` |  |
| `SDL_AudioStreamDevicePaused` | `0x671d0` |  |
| `SDL_BeginGPUComputePass` | `0x671e0` |  |
| `SDL_BeginGPUCopyPass` | `0x671f0` |  |
| `SDL_BeginGPURenderPass` | `0x67200` |  |
| `SDL_BindAudioStream` | `0x67210` |  |
| `SDL_BindAudioStreams` | `0x67220` |  |
| `SDL_BindGPUComputePipeline` | `0x67230` |  |
| `SDL_BindGPUComputeSamplers` | `0x67240` |  |
| `SDL_BindGPUComputeStorageBuffers` | `0x67250` |  |
| `SDL_BindGPUComputeStorageTextures` | `0x67260` |  |
| `SDL_BindGPUFragmentSamplers` | `0x67270` |  |
| `SDL_BindGPUFragmentStorageBuffers` | `0x67280` |  |

RTTI sample: `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SDL_PIXELFORMAT_ABGR2101010`, `SDL_PIXELFORMAT_ARGB2101010`, `SDL_PIXELFORMAT_P010`, `SDL_PIXELFORMAT_XBGR2101010`, `SDL_PIXELFORMAT_XRGB2101010`, `SteamAPI_SteamRemoteStorage_v016`.

## soundsystem.dll

Path: `game/bin/win64/soundsystem.dll`  
SHA-256: `d5eb6daf6aa555d4085f74f4ffef639e745d7b366eb367d2a34687dfe9f49cd5`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x3bbda0`.  
Exports: 42; RTTI types: 989; x64 unwind entries: 15565.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\soundsystem\_vpc_soundsystem\default\win64\_msvc_\Retail\soundsystem.pdb`; GUID `741bd531-f488-46d6-955d-cea06c94a77a`; age 12; lookup key `741BD531F48846D6955DCEA06C94A77AC`.

Normal import modules: `WINMM.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xaa420` |  |
| `CreateInterface` | `0x3b81b0` |  |
| `ExtractModuleMetadata` | `0xaa870` |  |
| `GetResourceManifestCount` | `0x3b9390` |  |
| `GetResourceManifests` | `0x3b93a0` |  |
| `InstallSchemaBindings` | `0x3b9bf0` |  |
| `opus_decode` | `0x210180` |  |
| `opus_decode_float` | `0x2102d0` |  |
| `opus_decoder_create` | `0x211360` |  |
| `opus_decoder_ctl` | `0x211450` |  |
| `opus_decoder_destroy` | `0x211680` |  |
| `opus_decoder_get_nb_samples` | `0x211690` |  |
| `opus_decoder_get_size` | `0x211720` |  |
| `opus_decoder_init` | `0x211770` |  |
| `opus_encode` | `0x213880` |  |
| `opus_encode_float` | `0x213a70` |  |
| `opus_encoder_create` | `0x216530` |  |
| `opus_encoder_ctl` | `0x216650` |  |
| `opus_encoder_destroy` | `0x211680` |  |
| `opus_encoder_get_size` | `0x216fb0` |  |
| `opus_encoder_init` | `0x217000` |  |
| `opus_get_version_string` | `0x22e6c0` |  |
| `opus_multistream_packet_pad` | `0x227a60` |  |
| `opus_multistream_packet_unpad` | `0x227b60` |  |
| `opus_packet_get_bandwidth` | `0x2118c0` |  |
| `opus_packet_get_nb_channels` | `0x211910` |  |
| `opus_packet_get_nb_frames` | `0x211920` |  |
| `opus_packet_get_nb_samples` | `0x211960` |  |
| `opus_packet_get_samples_per_frame` | `0x21bf00` |  |
| `opus_packet_pad` | `0x227d90` |  |

RTTI sample: `.?AUBufferedImpulseResponse_t@@`, `.?AUCVoiceContainerAmpedDecayingSineWave@@`, `.?AUCVoiceContainerAnalysisBase@@`, `.?AUCVoiceContainerDecayingSineWave@@`, `.?AUCVoiceContainerEnvelopeAnalyzer@@`, `.?AUCVoiceContainerGranulator@@`, `.?AUCVoiceContainerRandomSampler@@`, `.?AUCVoiceContainerRealtimeFMSineWave@@`, `.?AUCVoiceContainerShapedNoise@@`, `.?AUCVoiceContainerStaticAdditiveSynth@@`, `.?AUCVoiceContainerTapePlayer@@`, `.?AUIMMNotificationClient@@`, `.?AUIUnknown@@`, `.?AUSndBeatEventKeyedMidiNotes_t@@`, `.?AUSndBeatEventKeyedSndEvts_t@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## steam_api64.dll

Path: `game/bin/win64/steam_api64.dll`  
SHA-256: `2a785780af426dfc5c809087d100db8d4ff41a1e1a2956ad5473689e77e3c863`  
Architecture: PE32+ x86-64; preferred base `0x13b400000`; entry RVA `0xa110`.  
Exports: 1046; RTTI types: 15; x64 unwind entries: 808.

PDB reference: `C:\buildworker\steam_rel_client_win64\build\src\steam_api\win64\Release\steam_api64.pdb`; GUID `33efa307-2138-4112-aef5-8d71a5443f9f`; age 1; lookup key `33EFA30721384112AEF58D71A5443F9F1`.

Normal import modules: `KERNEL32.dll`, `ADVAPI32.dll`, `SHELL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `GetHSteamPipe` | `0x7a80` |  |
| `GetHSteamUser` | `0x7a90` |  |
| `SteamAPI_GetHSteamPipe` | `0x7a80` |  |
| `SteamAPI_GetHSteamUser` | `0x7a90` |  |
| `SteamAPI_GetSteamInstallPath` | `0x7aa0` |  |
| `SteamAPI_ISteamApps_BGetDLCDataByIndex` | `0x1790` |  |
| `SteamAPI_ISteamApps_BIsAppInstalled` | `0x17a0` |  |
| `SteamAPI_ISteamApps_BIsCybercafe` | `0x17b0` |  |
| `SteamAPI_ISteamApps_BIsDlcInstalled` | `0x17c0` |  |
| `SteamAPI_ISteamApps_BIsLowViolence` | `0x17d0` |  |
| `SteamAPI_ISteamApps_BIsSubscribed` | `0x17e0` |  |
| `SteamAPI_ISteamApps_BIsSubscribedApp` | `0x17f0` |  |
| `SteamAPI_ISteamApps_BIsSubscribedFromFamilySharing` | `0x1800` |  |
| `SteamAPI_ISteamApps_BIsSubscribedFromFreeWeekend` | `0x1810` |  |
| `SteamAPI_ISteamApps_BIsTimedTrial` | `0x1820` |  |
| `SteamAPI_ISteamApps_BIsVACBanned` | `0x1830` |  |
| `SteamAPI_ISteamApps_GetAppBuildId` | `0x1840` |  |
| `SteamAPI_ISteamApps_GetAppInstallDir` | `0x1850` |  |
| `SteamAPI_ISteamApps_GetAppOwner` | `0x1860` |  |
| `SteamAPI_ISteamApps_GetAvailableGameLanguages` | `0x1880` |  |
| `SteamAPI_ISteamApps_GetBetaInfo` | `0x1890` |  |
| `SteamAPI_ISteamApps_GetCurrentBetaName` | `0x18a0` |  |
| `SteamAPI_ISteamApps_GetCurrentGameLanguage` | `0x18b0` |  |
| `SteamAPI_ISteamApps_GetDLCCount` | `0x18c0` |  |
| `SteamAPI_ISteamApps_GetDlcDownloadProgress` | `0x18d0` |  |
| `SteamAPI_ISteamApps_GetEarliestPurchaseUnixTime` | `0x18e0` |  |
| `SteamAPI_ISteamApps_GetFileDetails` | `0x18f0` |  |
| `SteamAPI_ISteamApps_GetInstalledDepots` | `0x1900` |  |
| `SteamAPI_ISteamApps_GetLaunchCommandLine` | `0x1910` |  |
| `SteamAPI_ISteamApps_GetLaunchQueryParam` | `0x1920` |  |

RTTI sample: `.?AV?$CCallback@VCCallbackMgr@@USteamAPICallCompleted_t@@$00@@`, `.?AV?$CCallback@VCCallbackMgr@@USteamAPICallCompleted_t@@$0A@@@`, `.?AV?$CCallbackImpl@$0BA@@@`, `.?AV?$CCallbackManual@VCCallbackMgr@@USteamAPICallCompleted_t@@$00@@`, `.?AV?$CCallbackManual@VCCallbackMgr@@USteamAPICallCompleted_t@@$0A@@@`, `.?AVCAppManifestParser@@`, `.?AVCCallbackBase@@`, `.?AVCKeyValuesParser@@`, `.?AVbad_alloc@std@@`, `.?AVbad_array_new_length@std@@`, `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVlength_error@std@@`, `.?AVlogic_error@std@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `STEAMAPPS_INTERFACE_VERSION008`, `STEAMHTMLSURFACE_INTERFACE_VERSION_005`, `STEAMHTTP_INTERFACE_VERSION003`, `STEAMINVENTORY_INTERFACE_V003`, `STEAMMUSIC_INTERFACE_VERSION001`, `STEAMPARENTALSETTINGS_INTERFACE_VERSION001`, `STEAMREMOTEPLAY_INTERFACE_VERSION003`, `STEAMREMOTESTORAGE_INTERFACE_VERSION016`, `STEAMSCREENSHOTS_INTERFACE_VERSION003`, `STEAMTIMELINE_INTERFACE_V004`, `STEAMUGC_INTERFACE_VERSION021`, `STEAMUSERSTATS_INTERFACE_VERSION013`.

## steamaudio.dll

Path: `game/bin/win64/steamaudio.dll`  
SHA-256: `93f3d7310192a07f57241859bc8d4b4b2a57cb6f6bdde6c56f62df9589a5c662`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1f0400`.  
Exports: 6; RTTI types: 413; x64 unwind entries: 8917.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\soundsystem\steamaudio\_vpc_steamaudio\default\win64\_msvc_\Retail\steamaudio.pdb`; GUID `8e0989af-63b2-4f83-a264-b7523c85501d`; age 11; lookup key `8E0989AF63B24F83A264B7523C85501DB`.

Normal import modules: `phonon.dll`, `tier0.dll`, `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`, `WS2_32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x91b20` |  |
| `CreateInterface` | `0x1e3a50` |  |
| `ExtractModuleMetadata` | `0x91f90` |  |
| `GetResourceManifestCount` | `0x1e5d80` |  |
| `GetResourceManifests` | `0x1e5d90` |  |
| `InstallSchemaBindings` | `0x1e5970` |  |

RTTI sample: `.?AUBufferedImpulseResponse_t@@`, `.?AV?$CBaseAppSystem@VISteamAudio@@@@`, `.?AV?$CBaseConcreteToolAttr@$$BY02UVMixDynamicsBand_t@@UVMixDynamics3BandDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@$$BY07UVMixEQFilterDesc_t@@UVMixEQ8Desc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@HUVMixPitchShiftDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@HUVMixShaperDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@HUVMixVocoderDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@IVCVMixDataOffset@@@@`, `.?AV?$CBaseConcreteToolAttr@MUVMixAutoFilterDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@MUVMixBoxverbDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@MUVMixConvolutionDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@MUVMixDelayDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@MUVMixDiffusorDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@MUVMixDualCompressorDesc_t@@@@`, `.?AV?$CBaseConcreteToolAttr@MUVMixDynamics3BandDesc_t@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## steamnetworkingsockets.dll

Path: `game/bin/win64/steamnetworkingsockets.dll`  
SHA-256: `4e3b672036cf3a158ba9d27fb41281a2c7945b1ac9b3c20a6271f60e3593fe46`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x2cdae4`.  
Exports: 32; RTTI types: 291; x64 unwind entries: 11564.

PDB reference: `C:\buildworker\sdr_public_win64\build\obj\steamnetworkingsockets_Release_x642022\steamnetworkingsockets.pdb`; GUID `30a05eb5-ab59-4ec6-b325-a29d3065c525`; age 101; lookup key `30A05EB5AB594EC6B325A29D3065C52565`.

Normal import modules: `KERNEL32.dll`, `WS2_32.dll`, `WINMM.dll`, `IPHLPAPI.DLL`, `bcrypt.dll`, `USER32.dll`, `ADVAPI32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `SteamDatagramClient_Init` | `0x7f920` |  |
| `SteamDatagramClient_InitSteam_InternalV1` | `0x88c70` |  |
| `SteamDatagramClient_Kill` | `0x7f990` |  |
| `SteamDatagramClient_SetCredentialsDurableCacheCallbacks` | `0x14cee0` |  |
| `SteamDatagramServer_Init` | `0x7f9d0` |  |
| `SteamDatagramServer_InitSteam_InternalV1` | `0x88d70` |  |
| `SteamDatagramServer_Kill` | `0x7fa40` |  |
| `SteamDatagram_GetNetworkConfigURL` | `0x14cef0` |  |
| `SteamDatagram_Internal_SteamAPIKludge` | `0x88e70` |  |
| `SteamDatagram_SetAppID` | `0x7fa80` |  |
| `SteamDatagram_SetNetworkConfig` | `0x14cfe0` |  |
| `SteamDatagram_SetUniverse` | `0x7fb10` |  |
| `SteamGameServerNetworkingMessages_LibV2` | `0x7fbb0` |  |
| `SteamGameServerNetworkingSockets_LibV12` | `0x7fbf0` |  |
| `SteamNetworkingIPAddr_GetFakeIPType` | `0xbb2b0` |  |
| `SteamNetworkingIPAddr_ParseString` | `0xbb310` |  |
| `SteamNetworkingIPAddr_ToString` | `0xbb430` |  |
| `SteamNetworkingIdentity_ParseString` | `0xbb540` |  |
| `SteamNetworkingIdentity_ToString` | `0xbb910` |  |
| `SteamNetworkingMessages_LibV2` | `0x7fc00` |  |
| `SteamNetworkingSockets_DefaultPreFormatDebugOutputHandler` | `0xd14d0` |  |
| `SteamNetworkingSockets_LibV12` | `0x7fc40` |  |
| `SteamNetworkingSockets_Poll` | `0xd12f0` |  |
| `SteamNetworkingSockets_SetEnvironmentVariable` | `0x7fc50` |  |
| `SteamNetworkingSockets_SetLockAcquiredCallback` | `0xaee20` |  |
| `SteamNetworkingSockets_SetLockHeldCallback` | `0xaee30` |  |
| `SteamNetworkingSockets_SetLockWaitWarningThreshold` | `0xaee40` |  |
| `SteamNetworkingSockets_SetManualPollMode` | `0xd1390` |  |
| `SteamNetworkingSockets_SetPreFormatDebugOutputHandler` | `0xd1cd0` |  |
| `SteamNetworkingSockets_SetServiceThreadInitCallback` | `0xd1480` |  |

RTTI sample: `.?AU?$LinkStatsTracker@ULinkStatsTrackerClientRelaySession@SteamNetworkingSocketsLib@@@SteamNetworkingSocketsLib@@`, `.?AU?$LinkStatsTracker@ULinkStatsTrackerEndToEnd@SteamNetworkingSocketsLib@@@SteamNetworkingSocketsLib@@`, `.?AU?$LinkStatsTracker@ULinkStatsTrackerRelayServerSession@SteamNetworkingSocketsLib@@@SteamNetworkingSocketsLib@@`, `.?AUCLaggedPacket@SteamNetworkingSocketsLib@@`, `.?AUCProcessP2PFailureFromSteamBackend@SteamNetworkingSocketsLib@@`, `.?AUCProcessP2PSignalFromSteamBackend@SteamNetworkingSocketsLib@@`, `.?AUFakeUDPSession@SteamNetworkingSocketsLib@@`, `.?AUISymmetricDecryptContext@@`, `.?AUISymmetricEncryptContext@@`, `.?AULinkStatsTrackerBase@SteamNetworkingSocketsLib@@`, `.?AULinkStatsTrackerClientRelaySession@SteamNetworkingSocketsLib@@`, `.?AULinkStatsTrackerEndToEnd@SteamNetworkingSocketsLib@@`, `.?AULinkStatsTrackerRelayBase@SteamNetworkingSocketsLib@@`, `.?AULinkStatsTrackerRelayServerSession@SteamNetworkingSocketsLib@@`, `.?AUPostConnectionStateMsgTask@SteamNetworkingSocketsLib@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `STEAMHTTP_INTERFACE_VERSION003`, `SteamClient020`, `SteamGameServer015`, `SteamNetworkingSocketsSerialized003`, `SteamNetworkingSocketsSerialized005`, `SteamUser023`, `SteamUtils010`, `SystemFunction036`, `ffdhe2048`, `ffdhe3072`, `ffdhe4096`, `gost2001`.

## convarhelper_subtool.dll

Path: `game/bin/win64/subtools/convarhelper_subtool.dll`  
SHA-256: `c2cae6703f24952197fa531721828921cbbf3b2cb55a4ab32c363aff8e8cdada`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x34f58`.  
Exports: 6; RTTI types: 101; x64 unwind entries: 1754.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\vconsole2\subtools\convarhelper\_vpc_convarhelper_subtool\default\win64\_msvc_\Retail\convarhelper_subtool.pdb`; GUID `3e00e0ad-1ca8-410f-b791-430923f0137b`; age 12; lookup key `3E00E0AD1CA8410FB791430923F0137BC`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x1a3e0` |  |
| `CreateInterface` | `0x2c370` |  |
| `ExtractModuleMetadata` | `0x1a830` |  |
| `GetResourceManifestCount` | `0x2d1c0` |  |
| `GetResourceManifests` | `0x2d1d0` |  |
| `InstallSchemaBindings` | `0x2d420` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIExternalSubTool@@@@`, `.?AV?$CTier0AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier1AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier2AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier3AppSystem@VIExternalSubTool@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCConvarAttr@@`, `.?AVCConvarHelperTool@@`, `.?AVCEditButtonDialog@@`, `.?AVCEditLabelDialog@@`, `.?AVCGenericExprPart@@`, `.?AVCGenericExpr_Binary@@`, `.?AVCGenericExpr_BoolLiteral@@`, `.?AVCGenericExpr_FloatLiteral@@`, `.?AVCGenericExpr_FunctionCall@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## dashboard_subtool.dll

Path: `game/bin/win64/subtools/dashboard_subtool.dll`  
SHA-256: `1e4201f4d6e7006fd5731e55e7c3a6cb894889b320fb9a303f3ddd33ff662273`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x11a7c`.  
Exports: 6; RTTI types: 31; x64 unwind entries: 689.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\vconsole2\subtools\dashboard\_vpc_dashboard_subtool\default\win64\_msvc_\Retail\dashboard_subtool.pdb`; GUID `058e3040-e9fa-4371-baf6-70f3e2807aa6`; age 12; lookup key `058E3040E9FA4371BAF670F3E2807AA6C`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x6970` |  |
| `CreateInterface` | `0x78e0` |  |
| `ExtractModuleMetadata` | `0x6dc0` |  |
| `GetResourceManifestCount` | `0x11040` |  |
| `GetResourceManifests` | `0x11050` |  |
| `InstallSchemaBindings` | `0x112a0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIExternalSubTool@@@@`, `.?AV?$CTier0AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier1AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier2AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier3AppSystem@VIExternalSubTool@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCDashboardTool@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCQDashboardHistoryGraphWidget@@`, `.?AVCQDashboardWidget@@`, `.?AVCSchemaInstallCallback@@`, `.?AVIAppSystem@@`, `.?AVIExternalSubTool@@`, `.?AVIExternalVConsoleSubTool@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## netgraph_subtool.dll

Path: `game/bin/win64/subtools/netgraph_subtool.dll`  
SHA-256: `1553d7c043afeabfbe6650036886cfea9921eeac3cd7ffc6274a52b174a0e47d`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x24ca8`.  
Exports: 6; RTTI types: 57; x64 unwind entries: 979.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\vconsole2\subtools\netgraph\_vpc_netgraph_subtool\default\win64\_msvc_\Retail\netgraph_subtool.pdb`; GUID `1eb78e85-2d3a-4f27-801c-c411bb01bc9a`; age 9; lookup key `1EB78E852D3A4F27801CC411BB01BC9A9`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x18d40` |  |
| `CreateInterface` | `0x1a320` |  |
| `ExtractModuleMetadata` | `0x19190` |  |
| `GetResourceManifestCount` | `0x241e0` |  |
| `GetResourceManifests` | `0x241f0` |  |
| `InstallSchemaBindings` | `0x24440` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIExternalSubTool@@@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AV?$CTier0AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier1AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier2AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier3AppSystem@VIExternalSubTool@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCKV3TransferContextBase@@`, `.?AVCKV3TransferLoadContext@@`, `.?AVCKV3TransferSaveContext@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCNetGraphTool@@`, `.?AVCPostConnectCallback@@`, `.?AVCQGraphLegendModel@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## soundviewer_subtool.dll

Path: `game/bin/win64/subtools/soundviewer_subtool.dll`  
SHA-256: `39aa2678f76d82aa5ce5e80831e478a20a63a414ea76c133d95d46e348cd8fce`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x28078`.  
Exports: 6; RTTI types: 75; x64 unwind entries: 1093.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\vconsole2\subtools\soundviewer\_vpc_soundviewer_subtool\default\win64\_msvc_\Retail\soundviewer_subtool.pdb`; GUID `0d523195-0511-474b-bfe7-75cd2daf403c`; age 12; lookup key `0D5231950511474BBFE775CD2DAF403CC`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x1bba0` |  |
| `CreateInterface` | `0x1fdd0` |  |
| `ExtractModuleMetadata` | `0x1bff0` |  |
| `GetResourceManifestCount` | `0x275b0` |  |
| `GetResourceManifests` | `0x275c0` |  |
| `InstallSchemaBindings` | `0x27810` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIExternalSubTool@@@@`, `.?AV?$CTier0AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier1AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier2AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier3AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$IQFilterProvider@$$QEBVQString@@@@`, `.?AV?$IQFilterProvider@W4soundcache_eventtype_t@@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCMixDisplayWidget@@`, `.?AVCMixGrpsDisplayWidget@@`, `.?AVCMixGrpsLevelDisplay@@`, `.?AVCMixGrpsLevelLayout@@`, `.?AVCMixGrpsViewer@@`, `.?AVCMixInputs@@`, `.?AVCMixLevelDisplay@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## vprof_subtool.dll

Path: `game/bin/win64/subtools/vprof_subtool.dll`  
SHA-256: `9da39ffd7ac0349fdf66ecfdc88ba1bea79624e5538a9cbaf0794ac5f53a2392`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1b5c8`.  
Exports: 6; RTTI types: 67; x64 unwind entries: 953.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\vconsole2\subtools\vprof\_vpc_vprof_subtool\default\win64\_msvc_\Retail\vprof_subtool.pdb`; GUID `bc89b4dd-e3a8-4f44-bf15-531f55ea170c`; age 12; lookup key `BC89B4DDE3A84F44BF15531F55EA170CC`.

Normal import modules: `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xe7f0` |  |
| `CreateInterface` | `0x11d20` |  |
| `ExtractModuleMetadata` | `0xec40` |  |
| `GetResourceManifestCount` | `0x1ab00` |  |
| `GetResourceManifests` | `0x1ab10` |  |
| `InstallSchemaBindings` | `0x1ad60` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIExternalSubTool@@@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AV?$CTier0AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier1AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier2AppSystem@VIExternalSubTool@@$0A@@@`, `.?AV?$CTier3AppSystem@VIExternalSubTool@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCQBudgetBarsWidget@@`, `.?AVCQBudgetHistoryBarsWidget@@`, `.?AVCQBudgetHistoryWidget@@`, `.?AVCQBudgetNamesWidget@@`, `.?AVCQBudgetSamplesWidget@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## symsrv.dll

Path: `game/bin/win64/symsrv.dll`  
SHA-256: `c67be7d3f54d44ac264a18e33909482f1f8ca7b7fbaaf5659ef71ed9f8092c34`  
Architecture: PE32+ x86-64; preferred base `0x1d00000`; entry RVA `0x19864`.  
Exports: 30; RTTI types: 0; x64 unwind entries: 374.

PDB reference: `symsrv.pdb`; GUID `9bf46329-5024-4327-adfb-8a4b20f71bcb`; age 1; lookup key `9BF4632950244327ADFB8A4B20F71BCB1`.

Normal import modules: `msvcrt.dll`, `KERNEL32.dll`, `ADVAPI32.dll`.

Delay import modules: LZ32.dll, Cabinet.dll, USER32.dll, WININET.dll, WINHTTP.dll, GDI32.dll.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `EulaDlgProc` | `0x16990` |  |
| `RunDllEntry` | `0x9340` |  |
| `SymbolServerDeltaName` | `0x8650` |  |
| `SymbolServerDeltaNameW` | `0x8530` |  |
| `SymbolServerGetSupplement` | `0x89a0` |  |
| `SymbolServerGetSupplementW` | `0x8770` |  |
| `SymbolServerGetVersion` | `0x84c0` |  |
| `SymbolServerIsStore` | `0x92b0` |  |
| `SymbolServerIsStoreW` | `0x9190` |  |
| `SymbolServerPing` | `0x8460` |  |
| `SymbolServerPingW` | `0x83a0` |  |
| `SymbolServerSetOptionsW` | `0x7e40` |  |
| `SymbolServerStoreFile` | `0x9090` |  |
| `SymbolServerStoreFileW` | `0x8e50` |  |
| `SymbolServerStoreSupplement` | `0x8c20` |  |
| `SymbolServerStoreSupplementW` | `0x8a90` |  |
| `SymbolServerW` | `0x79a0` |  |
| `SymbolServer` | `0x7a90` |  |
| `SymbolServerByIndex` | `0x7d50` |  |
| `SymbolServerByIndexW` | `0x7b80` |  |
| `SymbolServerClose` | `0x7670` |  |
| `SymbolServerGetIndexString` | `0x8db0` |  |
| `SymbolServerGetIndexStringW` | `0x8d30` |  |
| `SymbolServerGetOptions` | `0x8380` |  |
| `SymbolServerSetOptions` | `0x82e0` |  |
| `httpCloseHandle` | `0x13c00` |  |
| `httpOpenFileHandle` | `0x13aa0` |  |
| `httpOpenFileHandleW` | `0x13760` |  |
| `httpQueryDataAvailable` | `0x13b30` |  |
| `httpReadFile` | `0x13b90` |  |

## tier0.dll

Path: `game/bin/win64/tier0.dll`  
SHA-256: `0bcd160dfa72d9f97fbf85095d59baad18fca61f20d840e83a5bd9f7635d5c32`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x2a269c`.  
Exports: 2625; RTTI types: 151; x64 unwind entries: 10808.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\tier0\_vpc_tier0\default\win64\_msvc_\Retail\tier0.pdb`; GUID `b66b06d2-c2a1-4fba-97f6-408cae2b0dd1`; age 9; lookup key `B66B06D2C2A14FBA97F6408CAE2B0DD19`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `GDI32.dll`, `WS2_32.dll`, `RPCRT4.dll`, `api-ms-win-core-synch-l1-2-0.dll`, `WINMM.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0?$CUniformRandomStreamImpl@VCAtomicMutex@@@@QEAA@H@Z` | `0x15ed40` |  |
| `??0?$CUniformRandomStreamImpl@VCAtomicMutex@@@@QEAA@XZ` | `0x15ed70` |  |
| `??0?$CUniformRandomStreamImpl@VCThreadNullMutex@@@@QEAA@H@Z` | `0x15e8e0` |  |
| `??0?$CUniformRandomStreamImpl@VCThreadNullMutex@@@@QEAA@XZ` | `0x15e900` |  |
| `??0?$CUniformRandomStreamImpl@VCThreadNullVerificationMutex@@@@QEAA@AEBV0@@Z` | `0x15d770` |  |
| `??0?$CUniformRandomStreamImpl@VCThreadNullVerificationMutex@@@@QEAA@H@Z` | `0x15d910` |  |
| `??0?$CUniformRandomStreamImpl@VCThreadNullVerificationMutex@@@@QEAA@XZ` | `0x15d940` |  |
| `??0CBlake3HashContext@@QEAA@XZ` | `0x56e90` |  |
| `??0CBufferStringFormatOutputStream@@QEAA@$$QEAV0@@Z` | `0x17d70` |  |
| `??0CBufferStringFormatOutputStream@@QEAA@AEAVCBufferString@@@Z` | `0x17d00` |  |
| `??0CBufferStringFormatOutputStream@@QEAA@AEBV0@@Z` | `0x17d70` |  |
| `??0CBufferStringFormatOutputStream@@QEAA@XZ` | `0x17d40` |  |
| `??0CCallStackCapture@@QEAA@P6AHIPEAPEAXHH@ZII@Z` | `0x164170` |  |
| `??0CCallStackMarker@@QEAA@_N@Z` | `0x164280` |  |
| `??0CCallStackMarker_ThisLine@@QEAA@_N@Z` | `0x164290` |  |
| `??0CCharBufferFormatOutputStream@@QEAA@$$QEAV0@@Z` | `0x17f20` |  |
| `??0CCharBufferFormatOutputStream@@QEAA@AEBV0@@Z` | `0x17f20` |  |
| `??0CCharBufferFormatOutputStream@@QEAA@PEADH@Z` | `0x17ee0` |  |
| `??0CCircularBuffer@@QEAA@I@Z` | `0x5a3b0` |  |
| `??0CCircularBuffer@@QEAA@XZ` | `0x5a390` |  |
| `??0CCommandBuffer@@QEAA@XZ` | `0x5aa00` |  |
| `??0CCountedStringPool_CI@@QEAA@XZ` | `0x16a2a0` |  |
| `??0CDbgHelpFunctionsLoader@@QEAA@XZ` | `0x1923c0` |  |
| `??0CDirIterator@@QEAA@PEBD0I@Z` | `0x92160` |  |
| `??0CDirIterator@@QEAA@PEBDI@Z` | `0x921c0` |  |
| `??0CDirIterator@@QEAA@XZ` | `0x92140` |  |
| `??0CDirTreeWatchManager@@QEAA@I@Z` | `0x96e40` |  |
| `??0CDirWatcher@@QEAA@XZ` | `0x93600` |  |
| `??0CEnvironmentBlock@@QEAA@XZ` | `0x15a150` |  |
| `??0CEventDispatcher_Base@@IEAA@XZ` | `0x85a20` |  |

RTTI sample: `.?AUConcurrentQueueProducerTypelessBase@details@moodycamel@@`, `.?AUEnumerateAbsPathsInDirRecursiveVisitor_t@@`, `.?AUImplicitProducer@?$ConcurrentQueue@PEAVCThreadedJob@@UConcurrentQueueDefaultTraits@moodycamel@@@moodycamel@@`, `.?AUProducerBase@?$ConcurrentQueue@PEAVCThreadedJob@@UConcurrentQueueDefaultTraits@moodycamel@@@moodycamel@@`, `.?AU_Crt_new_delete@std@@`, `.?AV?$CBaseAppSystem@VICvar@@@@`, `.?AV?$CBaseAppSystem@VIProcessUtils@@@@`, `.?AV?$CBaseAppSystem@VITestScriptMgr@@@@`, `.?AV?$CBaseAppSystem@VIUtlStringTokenSystem@@@@`, `.?AV?$CConCommandMemberAccessor@VCCvar@@@@`, `.?AV?$CMemPassAllocT@U?$LLA_AddAlignedSelectors@ULowLevelAllocationScheme_mimalloc@@@@@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?9??ParallelForSharedCode@@YAXHHPEBDAEBV?$function@$$A6AXHH@Z@std@@HHW4JobPriority_t@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelProcessorAbstract@V?$CParallelProcessor@HV?$CMemberFuncJobItemProcessor@HVCCallQueue@@V1@@@$00@@@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted1@VIRefCounted@@V?$CRefCountServiceBase@VCRefMT@@@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## toolframework2.dll

Path: `game/bin/win64/toolframework2.dll`  
SHA-256: `0248760abc9e216779b36b7739c5ebe3478a0faef26ed69f50cf6cfd98fff899`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x5e5624`.  
Exports: 6; RTTI types: 5375; x64 unwind entries: 28070.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\toolframework2\_vpc_toolframework2\default\win64\_msvc_\Retail\toolframework2.pdb`; GUID `1c29325a-2b81-42a5-87de-d654165e3ff7`; age 12; lookup key `1C29325A2B8142A587DED654165E3FF7C`.

Normal import modules: `ADVAPI32.dll`, `libfbxsdk_2020_3_1.dll`, `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `WS2_32.dll`, `tier0.dll`, `KERNEL32.dll`, `USER32.dll`, `SHLWAPI.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x4f550` |  |
| `CreateInterface` | `0x58f5b0` |  |
| `ExtractModuleMetadata` | `0x4fab0` |  |
| `GetResourceManifestCount` | `0x597f60` |  |
| `GetResourceManifests` | `0x597f70` |  |
| `InstallSchemaBindings` | `0x5981c0` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUCMorphSetData@@`, `.?AUCPulseCell_Inflow_Wait_DebugInfo_t@@`, `.?AUCScaleBiasFilter@CModelDocClothProxyMeshFile@@`, `.?AU_Anon@?$_Associated_state@H@std@@`, `.?AU_Anon@?$_Associated_state@I@std@@`, `.?AU_Crt_new_delete@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## v8.dll

Path: `game/bin/win64/v8.dll`  
SHA-256: `f85c79b1fe07c75befe22acd91c5d34a39ff83d9284fd9243a72c364a406822d`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x6680`.  
Exports: 27959; RTTI types: 3712; x64 unwind entries: 95039.

PDB reference: `E:\p4_temp\thirdpartycode\nonredist\v8\v8-11.9.169.6\v8\out.gn\x64.release\v8.dll.pdb`; GUID `86c11ca3-27dd-4618-b937-ccc3a54436ad`; age 2; lookup key `86C11CA327DD4618B937CCC3A54436AD2`.

Normal import modules: `ADVAPI32.dll`, `KERNEL32.dll`, `v8_libbase.dll`, `v8_icui18n.dll`, `v8_icuuc.dll`, `v8_zlib.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??$?6$0BA@@compiler@internal@v8@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV34@AEBV?$SimdImmediateParameter@$0BA@X@012@@Z` | `0x121220` |  |
| `??$?6$0CA@@compiler@internal@v8@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AEAV34@AEBV?$SimdImmediateParameter@$0CA@X@012@@Z` | `0x5ab00` |  |
| `??$?8$0BA@@compiler@internal@v8@@YA_NAEBV?$SimdImmediateParameter@$0BA@X@012@0@Z` | `0xa19a0` |  |
| `??$?8$0CA@@compiler@internal@v8@@YA_NAEBV?$SimdImmediateParameter@$0CA@X@012@0@Z` | `0x7d210` |  |
| `??$Add@VIsolate@internal@v8@@$0A@@?$Dictionary@VGlobalDictionary@internal@v8@@VGlobalDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VGlobalDictionary@internal@v8@@@12@PEAVIsolate@12@V312@V?$Handle@VName@internal@v8@@@12@V?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0xe46d0` |  |
| `??$Add@VIsolate@internal@v8@@$0A@@?$Dictionary@VNameDictionary@internal@v8@@VNameDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VNameDictionary@internal@v8@@@12@PEAVIsolate@12@V312@V?$Handle@VName@internal@v8@@@12@V?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0x102a50` |  |
| `??$Add@VIsolate@internal@v8@@$0A@@?$Dictionary@VNumberDictionary@internal@v8@@VNumberDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VNumberDictionary@internal@v8@@@12@PEAVIsolate@12@V312@IV?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0x18f40` |  |
| `??$Add@VIsolate@internal@v8@@$0A@@?$Dictionary@VSimpleNumberDictionary@internal@v8@@VSimpleNumberDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VSimpleNumberDictionary@internal@v8@@@12@PEAVIsolate@12@V312@IV?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0x129540` |  |
| `??$Add@VLocalIsolate@internal@v8@@$00@?$Dictionary@VGlobalDictionary@internal@v8@@VGlobalDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VGlobalDictionary@internal@v8@@@12@PEAVLocalIsolate@12@V312@V?$Handle@VName@internal@v8@@@12@V?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0xcf620` |  |
| `??$Add@VLocalIsolate@internal@v8@@$00@?$Dictionary@VNameDictionary@internal@v8@@VNameDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VNameDictionary@internal@v8@@@12@PEAVLocalIsolate@12@V312@V?$Handle@VName@internal@v8@@@12@V?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0xe53f0` |  |
| `??$Add@VLocalIsolate@internal@v8@@$00@?$Dictionary@VNumberDictionary@internal@v8@@VNumberDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VNumberDictionary@internal@v8@@@12@PEAVLocalIsolate@12@V312@IV?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0x62290` |  |
| `??$Add@VLocalIsolate@internal@v8@@$00@?$Dictionary@VSimpleNumberDictionary@internal@v8@@VSimpleNumberDictionaryShape@23@@internal@v8@@SA?AV?$Handle@VSimpleNumberDictionary@internal@v8@@@12@PEAVLocalIsolate@12@V312@IV?$Handle@VObject@internal@v8@@@12@VPropertyDetails@12@PEAVInternalIndex@12@@Z` | `0xa0670` |  |
| `??$AddExternalResourceAndHash@VExternalOneByteStringResource@String@v8@@@StringForwardingTable@internal@v8@@QEAAHV?$Tagged@VString@internal@v8@@@12@PEAVExternalOneByteStringResource@String@2@I@Z` | `0x10ec20` |  |
| `??$AddExternalResourceAndHash@VExternalStringResource@String@v8@@@StringForwardingTable@internal@v8@@QEAAHV?$Tagged@VString@internal@v8@@@12@PEAVExternalStringResource@String@2@I@Z` | `0x9fb90` |  |
| `??$Allocate@VIsolate@internal@v8@@@AstConsString@internal@v8@@AEBA?AV?$Handle@VString@internal@v8@@@12@PEAVIsolate@12@@Z` | `0x3d840` |  |
| `??$Allocate@VIsolate@internal@v8@@@DescriptorArray@internal@v8@@SA?AV?$Handle@VDescriptorArray@internal@v8@@@12@PEAVIsolate@12@HHW4AllocationType@12@@Z` | `0xc2840` |  |
| `??$Allocate@VIsolate@internal@v8@@@OrderedHashMap@internal@v8@@SA?AV?$MaybeHandle@VOrderedHashMap@internal@v8@@@12@PEAVIsolate@12@HW4AllocationType@12@@Z` | `0xb5be0` |  |
| `??$Allocate@VIsolate@internal@v8@@@OrderedHashSet@internal@v8@@SA?AV?$MaybeHandle@VOrderedHashSet@internal@v8@@@12@PEAVIsolate@12@HW4AllocationType@12@@Z` | `0x12bdb0` |  |
| `??$Allocate@VLocalIsolate@internal@v8@@@AstConsString@internal@v8@@AEBA?AV?$Handle@VString@internal@v8@@@12@PEAVLocalIsolate@12@@Z` | `0x107750` |  |
| `??$AllocateFixedArray@UIntPtrT@internal@v8@@@CodeStubAssembler@internal@v8@@QEAA?AV?$TNode@VFixedArrayBase@internal@v8@@@12@W4ElementsKind@12@V?$TNode@UIntPtrT@internal@v8@@@12@V?$Flags@W4AllocationFlag@CodeStubAssembler@internal@v8@@HH@base@2@V?$Optional@V?$TNode@VMap@internal@v8@@@internal@v8@@@72@@Z` | `0xe0cd0` |  |
| `??$AllocateFlat@VIsolate@internal@v8@@@AstConsString@internal@v8@@QEBA?AV?$Handle@VString@internal@v8@@@12@PEAVIsolate@12@@Z` | `0x5abc0` |  |
| `??$AllocateFlat@VLocalIsolate@internal@v8@@@AstConsString@internal@v8@@QEBA?AV?$Handle@VString@internal@v8@@@12@PEAVLocalIsolate@12@@Z` | `0xf3940` |  |
| `??$AllocateScopeInfos@VIsolate@internal@v8@@@DeclarationScope@internal@v8@@SAXPEAVParseInfo@12@PEAVIsolate@12@@Z` | `0x118d80` |  |
| `??$AllocateScopeInfos@VLocalIsolate@internal@v8@@@DeclarationScope@internal@v8@@SAXPEAVParseInfo@12@PEAVLocalIsolate@12@@Z` | `0x62ae0` |  |
| `??$AllocateScopeInfosRecursively@VIsolate@internal@v8@@@Scope@internal@v8@@AEAAXPEAVIsolate@12@V?$MaybeHandle@VScopeInfo@internal@v8@@@12@@Z` | `0x2e060` |  |
| `??$AllocateScopeInfosRecursively@VLocalIsolate@internal@v8@@@Scope@internal@v8@@AEAAXPEAVLocalIsolate@12@V?$MaybeHandle@VScopeInfo@internal@v8@@@12@@Z` | `0x7f7b0` |  |
| `??$At@VIsolate@internal@v8@@@ConstantArrayBuilder@interpreter@internal@v8@@QEBA?AV?$MaybeHandle@VObject@internal@v8@@@23@_KPEAVIsolate@23@@Z` | `0x10c710` |  |
| `??$At@VLocalIsolate@internal@v8@@@ConstantArrayBuilder@interpreter@internal@v8@@QEBA?AV?$MaybeHandle@VObject@internal@v8@@@23@_KPEAVLocalIsolate@23@@Z` | `0xd3990` |  |
| `??$BigIntLiteral@VIsolate@internal@v8@@@internal@v8@@YA?AV?$MaybeHandle@VBigInt@internal@v8@@@01@PEAVIsolate@01@PEBD@Z` | `0x191b0` |  |
| `??$BigIntLiteral@VLocalIsolate@internal@v8@@@internal@v8@@YA?AV?$MaybeHandle@VBigInt@internal@v8@@@01@PEAVLocalIsolate@01@PEBD@Z` | `0x2310` |  |

RTTI sample: `.?AU?$BeginRegionOperator@$00@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BeginRegionOperator@$0A@@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$00$00@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$00$01@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$00$0A@@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$01$00@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$01$01@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$01$0A@@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$0A@$00@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$0A@$01@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$BranchOperator@$0A@$0A@@CommonOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$ChangeFloat64ToTaggedOperator@$00@SimplifiedOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$ChangeFloat64ToTaggedOperator@$0A@@SimplifiedOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$CheckFloat64HoleNaNOperator@$00@SimplifiedOperatorGlobalCache@compiler@internal@v8@@`, `.?AU?$CheckFloat64HoleNaNOperator@$0A@@SimplifiedOperatorGlobalCache@compiler@internal@v8@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`, `wasm_16x8_splat_0x0001`.

## v8_icui18n.dll

Path: `game/bin/win64/v8_icui18n.dll`  
SHA-256: `098cbdaa7a7c7266beedd747d7a455d5917b665887934e8bc8e04ac6d5bff488`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1cc0`.  
Exports: 6474; RTTI types: 428; x64 unwind entries: 7570.

PDB reference: `E:\p4_temp\thirdpartycode\nonredist\v8\v8-11.9.169.6\v8\out.gn\x64.release\v8_icui18n.dll.pdb`; GUID `489cb263-15ca-4245-8fd2-19db5d1e7c4f`; age 1; lookup key `489CB26315CA42458FD219DB5D1E7C4F1`.

Normal import modules: `KERNEL32.dll`, `v8_icuuc.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0?$CompactUnicodeString@$03@impl@numparse@icu_73@@QEAA@$$QEAV0123@@Z` | `0x3d00` |  |
| `??0?$CompactUnicodeString@$03@impl@numparse@icu_73@@QEAA@AEBVUnicodeString@3@AEAW4UErrorCode@@@Z` | `0x1ede0` |  |
| `??0?$CompactUnicodeString@$03@impl@numparse@icu_73@@QEAA@XZ` | `0x8330` |  |
| `??0?$EnumSet@W4UDateFormatBooleanAttribute@@$0A@$03@icu_73@@QEAA@AEBV01@@Z` | `0x1e520` |  |
| `??0?$EnumSet@W4UDateFormatBooleanAttribute@@$0A@$03@icu_73@@QEAA@XZ` | `0xa810` |  |
| `??0?$LocalMemory@H@icu_73@@QEAA@$$QEAV01@@Z` | `0x5b80` |  |
| `??0?$LocalMemory@H@icu_73@@QEAA@PEAH@Z` | `0x25f60` |  |
| `??0?$LocalPointer@VAdoptingModifierStore@impl@number@icu_73@@@icu_73@@QEAA@$$QEAV01@@Z` | `0xb030` |  |
| `??0?$LocalPointer@VAdoptingModifierStore@impl@number@icu_73@@@icu_73@@QEAA@$$QEAV?$unique_ptr@VAdoptingModifierStore@impl@number@icu_73@@U?$default_delete@VAdoptingModifierStore@impl@number@icu_73@@@std@@@std@@@Z` | `0x253d0` |  |
| `??0?$LocalPointer@VAdoptingModifierStore@impl@number@icu_73@@@icu_73@@QEAA@PEAVAdoptingModifierStore@impl@number@1@@Z` | `0xd630` |  |
| `??0?$LocalPointer@VAdoptingModifierStore@impl@number@icu_73@@@icu_73@@QEAA@PEAVAdoptingModifierStore@impl@number@1@AEAW4UErrorCode@@@Z` | `0x29000` |  |
| `??0?$LocalPointer@VComplexUnitsConverter@units@icu_73@@@icu_73@@QEAA@$$QEAV01@@Z` | `0x8260` |  |
| `??0?$LocalPointer@VComplexUnitsConverter@units@icu_73@@@icu_73@@QEAA@$$QEAV?$unique_ptr@VComplexUnitsConverter@units@icu_73@@U?$default_delete@VComplexUnitsConverter@units@icu_73@@@std@@@std@@@Z` | `0x21b80` |  |
| `??0?$LocalPointer@VComplexUnitsConverter@units@icu_73@@@icu_73@@QEAA@PEAVComplexUnitsConverter@units@1@@Z` | `0x1a780` |  |
| `??0?$LocalPointer@VComplexUnitsConverter@units@icu_73@@@icu_73@@QEAA@PEAVComplexUnitsConverter@units@1@AEAW4UErrorCode@@@Z` | `0x1c5f0` |  |
| `??0?$LocalPointer@VCurrencyPluralInfo@icu_73@@@icu_73@@QEAA@$$QEAV01@@Z` | `0x20030` |  |
| `??0?$LocalPointer@VCurrencyPluralInfo@icu_73@@@icu_73@@QEAA@$$QEAV?$unique_ptr@VCurrencyPluralInfo@icu_73@@U?$default_delete@VCurrencyPluralInfo@icu_73@@@std@@@std@@@Z` | `0x13500` |  |
| `??0?$LocalPointer@VCurrencyPluralInfo@icu_73@@@icu_73@@QEAA@PEAVCurrencyPluralInfo@1@@Z` | `0xf210` |  |
| `??0?$LocalPointer@VCurrencyPluralInfo@icu_73@@@icu_73@@QEAA@PEAVCurrencyPluralInfo@1@AEAW4UErrorCode@@@Z` | `0x17cb0` |  |
| `??0?$LocalPointer@VMeasureUnitImpl@icu_73@@@icu_73@@QEAA@$$QEAV01@@Z` | `0x1c860` |  |
| `??0?$LocalPointer@VMeasureUnitImpl@icu_73@@@icu_73@@QEAA@$$QEAV?$unique_ptr@VMeasureUnitImpl@icu_73@@U?$default_delete@VMeasureUnitImpl@icu_73@@@std@@@std@@@Z` | `0x5570` |  |
| `??0?$LocalPointer@VMeasureUnitImpl@icu_73@@@icu_73@@QEAA@PEAVMeasureUnitImpl@1@@Z` | `0x20640` |  |
| `??0?$LocalPointer@VMeasureUnitImpl@icu_73@@@icu_73@@QEAA@PEAVMeasureUnitImpl@1@AEAW4UErrorCode@@@Z` | `0x225c0` |  |
| `??0?$LocalPointerBase@H@icu_73@@QEAA@PEAH@Z` | `0x2ac20` |  |
| `??0?$LocalPointerBase@VAdoptingModifierStore@impl@number@icu_73@@@icu_73@@QEAA@PEAVAdoptingModifierStore@impl@number@1@@Z` | `0x101a0` |  |
| `??0?$LocalPointerBase@VComplexUnitsConverter@units@icu_73@@@icu_73@@QEAA@PEAVComplexUnitsConverter@units@1@@Z` | `0x156c0` |  |
| `??0?$LocalPointerBase@VCurrencyPluralInfo@icu_73@@@icu_73@@QEAA@PEAVCurrencyPluralInfo@1@@Z` | `0x37f0` |  |
| `??0?$LocalPointerBase@VMeasureUnitImpl@icu_73@@@icu_73@@QEAA@PEAVMeasureUnitImpl@1@@Z` | `0x2bd00` |  |
| `??0?$MaybeStackArray@PEAUConverterPreference@units@icu_73@@$07@icu_73@@QEAA@$$QEAV01@@Z` | `0x2d210` |  |
| `??0?$MaybeStackArray@PEAUConverterPreference@units@icu_73@@$07@icu_73@@QEAA@HW4UErrorCode@@@Z` | `0xac70` |  |

RTTI sample: `.?AUAllowedHourFormatsSink@?A0xf0171a54@icu_73@@`, `.?AUAppendItemFormatsSink@DateTimePatternGenerator@icu_73@@`, `.?AUAppendItemNamesSink@DateTimePatternGenerator@icu_73@@`, `.?AUAvailableFormatsSink@DateTimePatternGenerator@icu_73@@`, `.?AUCalendarDataSink@?A0x8a1acdd4@icu_73@@`, `.?AUCollationCacheEntry@icu_73@@`, `.?AUCollationSettings@icu_73@@`, `.?AUCollationTailoring@icu_73@@`, `.?AUCurrencySpacingSink@?A0xf29ac9a7@icu_73@@`, `.?AUDateIntervalSink@DateIntervalInfo@icu_73@@`, `.?AUDayPeriodRulesCountSink@icu_73@@`, `.?AUDayPeriodRulesDataSink@icu_73@@`, `.?AUDecFmtSymDataSink@?A0xf29ac9a7@icu_73@@`, `.?AUKeywordsSink@?A0x5a286af2@@`, `.?AUListPatternsSink@ListFormatter@icu_73@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `GB18030`, `SystemFunction036`.

## v8_icuuc.dll

Path: `game/bin/win64/v8_icuuc.dll`  
SHA-256: `44ce115a5cf9d141ce11c92c6114bc0bcef92eedfb7737b0792d83213bee456f`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1850`.  
Exports: 3160; RTTI types: 160; x64 unwind entries: 4696.

PDB reference: `E:\p4_temp\thirdpartycode\nonredist\v8\v8-11.9.169.6\v8\out.gn\x64.release\v8_icuuc.dll.pdb`; GUID `2438f0c7-28a0-453f-b389-39568cc9c912`; age 1; lookup key `2438F0C728A0453FB38939568CC9C9121`.

Normal import modules: `ADVAPI32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0?$MaybeStackArray@D$0CI@@icu_73@@QEAA@$$QEAV01@@Z` | `0xe1c0` |  |
| `??0?$MaybeStackArray@D$0CI@@icu_73@@QEAA@HW4UErrorCode@@@Z` | `0x11930` |  |
| `??0?$MaybeStackArray@D$0CI@@icu_73@@QEAA@XZ` | `0x9320` |  |
| `??0?$_Atomic_storage@H$03@std@@QEAA@H@Z` | `0x18290` |  |
| `??0?$_Atomic_storage@H$03@std@@QEAA@XZ` | `0x7180` |  |
| `??0?$_Atomic_storage@PEAVmutex@std@@$07@std@@QEAA@QEAVmutex@1@@Z` | `0x41f0` |  |
| `??0?$_Atomic_storage@PEAVmutex@std@@$07@std@@QEAA@XZ` | `0x1a7d0` |  |
| `??0?$atomic@H@std@@QEAA@H@Z` | `0x8520` |  |
| `??0?$atomic@H@std@@QEAA@XZ` | `0x1aba0` |  |
| `??0?$atomic@PEAVmutex@std@@@std@@QEAA@QEAVmutex@1@@Z` | `0x1a860` |  |
| `??0?$atomic@PEAVmutex@std@@@std@@QEAA@XZ` | `0x6ed0` |  |
| `??0Appendable@icu_73@@QEAA@AEBV01@@Z` | `0x5f90` |  |
| `??0Appendable@icu_73@@QEAA@XZ` | `0xd5a0` |  |
| `??0BreakIterator@icu_73@@IEAA@AEBV01@@Z` | `0xb040` |  |
| `??0BreakIterator@icu_73@@IEAA@AEBVLocale@1@0@Z` | `0xf350` |  |
| `??0BreakIterator@icu_73@@IEAA@XZ` | `0x18c30` |  |
| `??0Builder@LocaleMatcher@icu_73@@QEAA@$$QEAV012@@Z` | `0x6990` |  |
| `??0Builder@LocaleMatcher@icu_73@@QEAA@XZ` | `0x172d0` |  |
| `??0ByteSink@icu_73@@QEAA@XZ` | `0xf950` |  |
| `??0BytesDictionaryMatcher@icu_73@@QEAA@AEBV01@@Z` | `0xb5e0` |  |
| `??0BytesDictionaryMatcher@icu_73@@QEAA@PEBDHPEAUUDataMemory@@@Z` | `0x1600` |  |
| `??0BytesTrie@icu_73@@AEAA@PEAXPEBX@Z` | `0x6f00` |  |
| `??0BytesTrie@icu_73@@QEAA@AEBV01@@Z` | `0x13500` |  |
| `??0BytesTrie@icu_73@@QEAA@PEBX@Z` | `0x6f70` |  |
| `??0BytesTrieBuilder@icu_73@@QEAA@AEAW4UErrorCode@@@Z` | `0xff10` |  |
| `??0CStr@icu_73@@QEAA@AEBVUnicodeString@1@@Z` | `0x8470` |  |
| `??0CacheKeyBase@icu_73@@QEAA@AEBV01@@Z` | `0x13750` |  |
| `??0CacheKeyBase@icu_73@@QEAA@XZ` | `0xdea0` |  |
| `??0CanonicalIterator@icu_73@@QEAA@AEBVUnicodeString@1@AEAW4UErrorCode@@@Z` | `0x13410` |  |
| `??0Char16Ptr@icu_73@@QEAA@$$T@Z` | `0x111f0` |  |

RTTI sample: `.?AUCapitalizationContextSink@LocaleDisplayNamesImpl@icu_73@@`, `.?AUGetAllChildrenSink@?A0x81b22168@@`, `.?AV?$ConvertingIterator@PEAPEBDVLocaleFromTag@?A0x1e293879@@@Locale@icu_73@@`, `.?AVAppendable@icu_73@@`, `.?AVArray1D@icu_73@@`, `.?AVArray2D@icu_73@@`, `.?AVAvailableLocalesSink@?A0xe4f9ffaf@@`, `.?AVAvailableLocalesStringEnumeration@?A0xe4f9ffaf@@`, `.?AVBMPSet@icu_73@@`, `.?AVBTLinearMatchNode@BytesTrieBuilder@icu_73@@`, `.?AVBranchHeadNode@StringTrieBuilder@icu_73@@`, `.?AVBranchNode@StringTrieBuilder@icu_73@@`, `.?AVBreakCache@RuleBasedBreakIterator@icu_73@@`, `.?AVBreakIterator@icu_73@@`, `.?AVBurmeseBreakEngine@icu_73@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `GB18030`, `ISO_2022`, `SystemFunction036`, `gb18030`, `iso2022`, `pap_029`, `rfc4011`, `rfc4013`.

## v8_libbase.dll

Path: `game/bin/win64/v8_libbase.dll`  
SHA-256: `3dc9686891b1f563102a1865499984d18e72432b43efd9137579534273054829`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x15c0`.  
Exports: 720; RTTI types: 112; x64 unwind entries: 3663.

PDB reference: `E:\p4_temp\thirdpartycode\nonredist\v8\v8-11.9.169.6\v8\out.gn\x64.release\v8_libbase.dll.pdb`; GUID `450f90ab-a5db-4e20-a44d-d86c6f8d02b9`; age 1; lookup key `450F90ABA5DB4E20A44DD86C6F8D02B91`.

Normal import modules: `dbghelp.dll`, `WINMM.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??$MakeCheckOpString@HH@base@v8@@YAPEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HHPEBD@Z` | `0x50b0` |  |
| `??$MakeCheckOpString@II@base@v8@@YAPEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@IIPEBD@Z` | `0x15280` |  |
| `??$MakeCheckOpString@JJ@base@v8@@YAPEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@JJPEBD@Z` | `0x2cd0` |  |
| `??$MakeCheckOpString@KK@base@v8@@YAPEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@KKPEBD@Z` | `0x15310` |  |
| `??$MakeCheckOpString@PEBXPEBX@base@v8@@YAPEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBX0PEBD@Z` | `0xf9d0` |  |
| `??$MakeCheckOpString@_J_J@base@v8@@YAPEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J0PEBD@Z` | `0x3e10` |  |
| `??$MakeCheckOpString@_K_K@base@v8@@YAPEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_K0PEBD@Z` | `0x1280` |  |
| `??$PrintCheckOperand@C@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@C@Z` | `0xa710` |  |
| `??$PrintCheckOperand@D@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@D@Z` | `0x4750` |  |
| `??$PrintCheckOperand@E@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@E@Z` | `0xe130` |  |
| `??$PrintCheckOperand@H@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@H@Z` | `0xeac0` |  |
| `??$PrintCheckOperand@I@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@I@Z` | `0xed20` |  |
| `??$PrintCheckOperand@J@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@J@Z` | `0x12320` |  |
| `??$PrintCheckOperand@K@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@K@Z` | `0x113e0` |  |
| `??$PrintCheckOperand@PEAC@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEAC@Z` | `0x1780` |  |
| `??$PrintCheckOperand@PEAD@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEAD@Z` | `0xd2c0` |  |
| `??$PrintCheckOperand@PEAE@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEAE@Z` | `0xa920` |  |
| `??$PrintCheckOperand@PEBC@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBC@Z` | `0x1b90` |  |
| `??$PrintCheckOperand@PEBD@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBD@Z` | `0xea10` |  |
| `??$PrintCheckOperand@PEBE@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBE@Z` | `0x8550` |  |
| `??$PrintCheckOperand@PEBX@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBX@Z` | `0x13690` |  |
| `??$PrintCheckOperand@_J@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_J@Z` | `0x9ab0` |  |
| `??$PrintCheckOperand@_K@base@v8@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_K@Z` | `0x5930` |  |
| `??$SignedDivisionByConstant@I$00@base@v8@@YA?AU?$MagicNumbersForDivision@I@01@I@Z` | `0x14380` |  |
| `??$SignedDivisionByConstant@_K$00@base@v8@@YA?AU?$MagicNumbersForDivision@_K@01@_K@Z` | `0x6c20` |  |
| `??$UnsignedDivisionByConstant@I@base@v8@@YA?AU?$MagicNumbersForDivision@I@01@II@Z` | `0x157c0` |  |
| `??$UnsignedDivisionByConstant@_K@base@v8@@YA?AU?$MagicNumbersForDivision@_K@01@_KI@Z` | `0xd1d0` |  |
| `??0?$MagicNumbersForDivision@I@base@v8@@QEAA@II_N@Z` | `0x13540` |  |
| `??0?$MagicNumbersForDivision@_K@base@v8@@QEAA@_KI_N@Z` | `0x59f0` |  |
| `??0?$TimeBase@VThreadTicks@base@v8@@@time_internal@base@v8@@IEAA@_J@Z` | `0x5630` |  |

RTTI sample: `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AUmessages_base@std@@`, `.?AUmoney_base@std@@`, `.?AUtime_base@std@@`, `.?AV?$_Func_base@X$$V@std@@`, `.?AV?$_Func_base@X_K_K@std@@`, `.?AV?$_Func_impl_no_alloc@V<lambda_1>@?1???0VirtualAddressSubspace@base@v8@@AEAA@VAddressSpaceReservation@34@PEAVVirtualAddressSpaceBase@34@W4PagePermissions@4@@Z@X_K_K@std@@`, `.?AV?$_Func_impl_no_alloc@V<lambda_1>@?4???$CallOnce@PEAXX@base@v8@@YAXPEAU?$atomic@E@std@@P6AXPEAX@Z1@Z@X$$V@std@@`, `.?AV?$_Func_impl_no_alloc@V<lambda_2>@?1???0VirtualAddressSubspace@base@v8@@AEAA@VAddressSpaceReservation@34@PEAVVirtualAddressSpaceBase@34@W4PagePermissions@4@@Z@X_K_K@std@@`, `.?AV?$_Iosb@H@std@@`, `.?AV?$_Mpunct@D@std@@`, `.?AV?$_Mpunct@G@std@@`, `.?AV?$_Mpunct@_W@std@@`, `.?AV?$basic_filebuf@DU?$char_traits@D@std@@@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## v8_libplatform.dll

Path: `game/bin/win64/v8_libplatform.dll`  
SHA-256: `210ac10fb2ad747b2d59aec2289bf10c3b47644816240842f00761eb2320f9ce`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1540`.  
Exports: 201; RTTI types: 123; x64 unwind entries: 3274.

PDB reference: `E:\p4_temp\thirdpartycode\nonredist\v8\v8-11.9.169.6\v8\out.gn\x64.release\v8_libplatform.dll.pdb`; GUID `1a832890-7d8f-4e29-ab9f-86c8dab27970`; age 1; lookup key `1A8328907D8F4E29AB9F86C8DAB279701`.

Normal import modules: `ADVAPI32.dll`, `KERNEL32.dll`, `v8_libbase.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `??0?$enable_shared_from_this@VDefaultJobState@platform@v8@@@std@@IEAA@AEBV01@@Z` | `0xb250` |  |
| `??0?$enable_shared_from_this@VDefaultJobState@platform@v8@@@std@@IEAA@XZ` | `0xcc00` |  |
| `??0DefaultForegroundTaskRunner@platform@v8@@QEAA@W4IdleTaskSupport@12@P6ANXZ@Z` | `0x9170` |  |
| `??0DefaultJobHandle@platform@v8@@QEAA@V?$shared_ptr@VDefaultJobState@platform@v8@@@std@@@Z` | `0x13510` |  |
| `??0DefaultJobState@platform@v8@@QEAA@PEAVPlatform@2@V?$unique_ptr@VJobTask@v8@@U?$default_delete@VJobTask@v8@@@std@@@std@@W4TaskPriority@2@_K@Z` | `0x133b0` |  |
| `??0DefaultPlatform@platform@v8@@QEAA@HW4IdleTaskSupport@12@V?$unique_ptr@VTracingController@v8@@U?$default_delete@VTracingController@v8@@@std@@@std@@W4PriorityMode@12@@Z` | `0x5c40` |  |
| `??0DefaultThreadIsolatedAllocator@platform@v8@@QEAA@AEBV012@@Z` | `0x11b80` |  |
| `??0DefaultThreadIsolatedAllocator@platform@v8@@QEAA@XZ` | `0x11260` |  |
| `??0DefaultWorkerThreadsTaskRunner@platform@v8@@QEAA@IP6ANXZW4Priority@Thread@base@2@@Z` | `0x6f20` |  |
| `??0DelayedTaskQueue@platform@v8@@QEAA@P6ANXZ@Z` | `0x6390` |  |
| `??0Recorder@tracing@platform@v8@@QEAA@XZ` | `0x9690` |  |
| `??0TaskQueue@platform@v8@@QEAA@XZ` | `0x4ab0` |  |
| `??0TraceBuffer@tracing@platform@v8@@QEAA@XZ` | `0x10a10` |  |
| `??0TraceBufferChunk@tracing@platform@v8@@QEAA@I@Z` | `0x8260` |  |
| `??0TraceConfig@tracing@platform@v8@@QEAA@XZ` | `0x3660` |  |
| `??0TraceObject@tracing@platform@v8@@QEAA@XZ` | `0x11ec0` |  |
| `??0TraceWriter@tracing@platform@v8@@QEAA@XZ` | `0x4100` |  |
| `??0TracingController@tracing@platform@v8@@QEAA@XZ` | `0x4be0` |  |
| `??0WorkerThread@platform@v8@@QEAA@PEAVTaskQueue@12@@Z` | `0x4800` |  |
| `??1?$enable_shared_from_this@VDefaultJobState@platform@v8@@@std@@IEAA@XZ` | `0x2240` |  |
| `??1DefaultForegroundTaskRunner@platform@v8@@UEAA@XZ` | `0x10910` |  |
| `??1DefaultJobHandle@platform@v8@@UEAA@XZ` | `0xabc0` |  |
| `??1DefaultJobState@platform@v8@@UEAA@XZ` | `0x7180` |  |
| `??1DefaultPlatform@platform@v8@@UEAA@XZ` | `0x3410` |  |
| `??1DefaultThreadIsolatedAllocator@platform@v8@@UEAA@XZ` | `0x31f0` |  |
| `??1DefaultWorkerThreadsTaskRunner@platform@v8@@UEAA@XZ` | `0x12c10` |  |
| `??1DelayedTaskQueue@platform@v8@@QEAA@XZ` | `0x4990` |  |
| `??1Recorder@tracing@platform@v8@@QEAA@XZ` | `0x10f80` |  |
| `??1TaskQueue@platform@v8@@QEAA@XZ` | `0x11190` |  |
| `??1TraceBuffer@tracing@platform@v8@@UEAA@XZ` | `0x4cc0` |  |

RTTI sample: `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AUmessages_base@std@@`, `.?AUmoney_base@std@@`, `.?AUtime_base@std@@`, `.?AV?$_Iosb@H@std@@`, `.?AV?$_Mpunct@D@std@@`, `.?AV?$_Mpunct@G@std@@`, `.?AV?$_Mpunct@_W@std@@`, `.?AV?$_Ref_count_obj2@VDefaultForegroundTaskRunner@platform@v8@@@std@@`, `.?AV?$_Ref_count_obj2@VDefaultJobState@platform@v8@@@std@@`, `.?AV?$_Ref_count_obj2@VDefaultWorkerThreadsTaskRunner@platform@v8@@@std@@`, `.?AV?$basic_ios@DU?$char_traits@D@std@@@std@@`, `.?AV?$basic_iostream@DU?$char_traits@D@std@@@std@@`, `.?AV?$basic_istream@DU?$char_traits@D@std@@@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## v8_zlib.dll

Path: `game/bin/win64/v8_zlib.dll`  
SHA-256: `74b51ce1445ea70c52e342d678717d0103f377fe60adc9271cb7232deed15e5a`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x1380`.  
Exports: 86; RTTI types: 10; x64 unwind entries: 1779.

PDB reference: `E:\p4_temp\thirdpartycode\nonredist\v8\v8-11.9.169.6\v8\out.gn\x64.release\v8_zlib.dll.pdb`; GUID `6f05328e-f93d-4780-8aa8-e219e2598323`; age 1; lookup key `6F05328EF93D47808AA8E219E25983231`.

Normal import modules: `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `Cr_z_adler32` | `0x1cb0` |  |
| `Cr_z_adler32_combine` | `0x46c0` |  |
| `Cr_z_adler32_z` | `0x4fa0` |  |
| `Cr_z_compress` | `0x4530` |  |
| `Cr_z_compress2` | `0x3bb0` |  |
| `Cr_z_compressBound` | `0x1940` |  |
| `Cr_z_crc32` | `0x86b0` |  |
| `Cr_z_crc32_combine` | `0x48b0` |  |
| `Cr_z_crc32_combine_gen` | `0x3700` |  |
| `Cr_z_crc32_combine_op` | `0x2810` |  |
| `Cr_z_crc32_z` | `0xa6c0` |  |
| `Cr_z_deflate` | `0xab90` |  |
| `Cr_z_deflateBound` | `0x3bf0` |  |
| `Cr_z_deflateCopy` | `0x6450` |  |
| `Cr_z_deflateEnd` | `0x4d80` |  |
| `Cr_z_deflateGetDictionary` | `0x1110` |  |
| `Cr_z_deflateInit2_` | `0x65b0` |  |
| `Cr_z_deflateInit_` | `0x59c0` |  |
| `Cr_z_deflateParams` | `0x1100` |  |
| `Cr_z_deflatePending` | `0x1f80` |  |
| `Cr_z_deflatePrime` | `0x8700` |  |
| `Cr_z_deflateReset` | `0x9870` |  |
| `Cr_z_deflateResetKeep` | `0x4c30` |  |
| `Cr_z_deflateSetDictionary` | `0x7900` |  |
| `Cr_z_deflateSetHeader` | `0x3580` |  |
| `Cr_z_deflateTune` | `0x9e10` |  |
| `Cr_z_get_crc_table` | `0x5540` |  |
| `Cr_z_gzbuffer` | `0x66d0` |  |
| `Cr_z_gzclearerr` | `0x6f60` |  |
| `Cr_z_gzclose` | `0x1d60` |  |

RTTI sample: `.?AVDNameNode@@`, `.?AVDNameStatusNode@@`, `.?AVbad_alloc@std@@`, `.?AVbad_exception@std@@`, `.?AVcharNode@@`, `.?AVexception@std@@`, `.?AVpDNameNode@@`, `.?AVpairNode@@`, `.?AVpcharNode@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## v8system.dll

Path: `game/bin/win64/v8system.dll`  
SHA-256: `545528ac7390d5d164ba8e7e0f24a4801d720b8f9c1fda1cecec4ed3e91cb8c7`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x15128`.  
Exports: 6; RTTI types: 21; x64 unwind entries: 737.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\v8system\_vpc_v8system\default\win64\_msvc_\Retail\v8system.pdb`; GUID `e76db2b5-b10d-456c-87bd-99bf34b7924e`; age 8; lookup key `E76DB2B5B10D456C87BD99BF34B7924E8`.

Normal import modules: `tier0.dll`, `v8.dll`, `v8_libplatform.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x2b10` |  |
| `CreateInterface` | `0x12ca0` |  |
| `ExtractModuleMetadata` | `0x2f60` |  |
| `GetResourceManifestCount` | `0x14680` |  |
| `GetResourceManifests` | `0x14690` |  |
| `InstallSchemaBindings` | `0x148e0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VISource2V8System@@@@`, `.?AV?$CTier0AppSystem@VISource2V8System@@$0A@@@`, `.?AV?$CTier1AppSystem@VISource2V8System@@$0A@@@`, `.?AV?$CTier2AppSystem@VISource2V8System@@$0A@@@`, `.?AVAllocator@ArrayBuffer@v8@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCJavaScriptTypeManager@@`, `.?AVCKV3ResourceBlockHelper@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCSchemaInstallCallback@@`, `.?AVCSource2V8System@@`, `.?AVCTypeScriptTypeManager@@`, `.?AVIAppSystem@@`, `.?AVIAsyncProcessedDataResult@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## valve_avi.dll

Path: `game/bin/win64/valve_avi.dll`  
SHA-256: `a7aa5d02d0d248fa7d910ce4eefa485e52e56dd528294bc83748d845fc064c9e`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xe548`.  
Exports: 6; RTTI types: 15; x64 unwind entries: 641.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\avi\_vpc_valve_avi\default\win64\_msvc_\Retail\valve_avi.pdb`; GUID `6aff0c4c-046b-459d-a0dc-545e417da7e3`; age 10; lookup key `6AFF0C4C046B459DA0DC545E417DA7E3A`.

Normal import modules: `USER32.dll`, `GDI32.dll`, `AVIFIL32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x4ca0` |  |
| `CreateInterface` | `0x6010` |  |
| `ExtractModuleMetadata` | `0x50f0` |  |
| `GetResourceManifestCount` | `0xda80` |  |
| `GetResourceManifests` | `0xda90` |  |
| `InstallSchemaBindings` | `0xdce0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIAvi@@@@`, `.?AV?$CTier0AppSystem@VIAvi@@$0A@@@`, `.?AV?$CTier1AppSystem@VIAvi@@$0A@@@`, `.?AV?$CTier2AppSystem@VIAvi@@$0A@@@`, `.?AV?$CTier3AppSystem@VIAvi@@$0A@@@`, `.?AVCAvi@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCSchemaInstallCallback@@`, `.?AVIAppSystem@@`, `.?AVIAvi@@`, `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## valve_webm.dll

Path: `game/bin/win64/valve_webm.dll`  
SHA-256: `9e8ca4d76b7c6bb5a57250dec0ac5f2350aab3c3c86f9e35e82449c039faa507`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x613638`.  
Exports: 6; RTTI types: 150; x64 unwind entries: 13287.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\valve_webm\_vpc_valve_webm\default\win64\_msvc_\Retail\valve_webm.pdb`; GUID `1e5d127f-7684-49f2-b715-ae4db00a4c65`; age 11; lookup key `1E5D127F768449F2B715AE4DB00A4C65B`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xa0070` |  |
| `CreateInterface` | `0x606f10` |  |
| `ExtractModuleMetadata` | `0xa04e0` |  |
| `GetResourceManifestCount` | `0x60eda0` |  |
| `GetResourceManifests` | `0x60edb0` |  |
| `InstallSchemaBindings` | `0x60f000` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AU_Anon@?$_Associated_state@H@std@@`, `.?AU_Anon@?$_Associated_state@I@std@@`, `.?AU_TaskProcHandle@details@Concurrency@@`, `.?AU_Task_impl_base@details@Concurrency@@`, `.?AUscheduler_interface@Concurrency@@`, `.?AV<lambda_5a9ef80d79df7873994019ff15dceae5>@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## valve_wmf.dll

Path: `game/bin/win64/valve_wmf.dll`  
SHA-256: `65e52e105aad6142aa98635aba78ae7c5dd543d3ae91856ebf3a9b5ccb664ec3`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xb618`.  
Exports: 6; RTTI types: 15; x64 unwind entries: 552.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\valve_wmf\_vpc_valve_wmf\default\win64\_msvc_\Retail\valve_wmf.pdb`; GUID `40b9464d-217a-4feb-88b3-e9a948c1c694`; age 8; lookup key `40B9464D217A4FEB88B3E9A948C1C6948`.

Normal import modules: `MFReadWrite.dll`, `MFPlat.DLL`, `ole32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x2480` |  |
| `CreateInterface` | `0x3110` |  |
| `ExtractModuleMetadata` | `0x28d0` |  |
| `GetResourceManifestCount` | `0xab50` |  |
| `GetResourceManifests` | `0xab60` |  |
| `InstallSchemaBindings` | `0xadb0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIMediaFoundation@@@@`, `.?AV?$CTier0AppSystem@VIMediaFoundation@@$0A@@@`, `.?AV?$CTier1AppSystem@VIMediaFoundation@@$0A@@@`, `.?AV?$CTier2AppSystem@VIMediaFoundation@@$0A@@@`, `.?AV?$CTier3AppSystem@VIMediaFoundation@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCMediaFoundation@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCSchemaInstallCallback@@`, `.?AVIAppSystem@@`, `.?AVIMediaFoundation@@`, `.?AVbad_exception@std@@`, `.?AVexception@std@@`, `.?AVtype_info@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## vconcomm.dll

Path: `game/bin/win64/vconcomm.dll`  
SHA-256: `1ba37938370e031778a3617a934df88e52952ec74238e6ba39ed15c626e14724`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x19948`.  
Exports: 6; RTTI types: 38; x64 unwind entries: 896.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\vconcomm\_vpc_vconcomm\default\win64\_msvc_\Retail\vconcomm.pdb`; GUID `32b04f3c-f5ea-4f27-8a5f-999639a2fc74`; age 8; lookup key `32B04F3CF5EA4F278A5F999639A2FC748`.

Normal import modules: `USER32.dll`, `WS2_32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xd6a0` |  |
| `CreateInterface` | `0x10fe0` |  |
| `ExtractModuleMetadata` | `0xdaf0` |  |
| `GetResourceManifestCount` | `0x18e80` |  |
| `GetResourceManifests` | `0x18e90` |  |
| `InstallSchemaBindings` | `0x190e0` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIAppSystem@@@@`, `.?AV?$CBaseAppSystem@VIVConComm@@@@`, `.?AV?$CConCommandMemberAccessor@VCVConComm@@@@`, `.?AV?$CTier0AppSystem@VIVConComm@@$0A@@@`, `.?AV?$CTier1AppSystem@VIVConComm@@$0A@@@`, `.?AV?$CTier2AppSystem@VIVConComm@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCKV3TransferContextBase@@`, `.?AVCKV3TransferSaveContext@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCPostConnectCallback@@`, `.?AVCSchemaInstallCallback@@`, `.?AVCServicePollThread@@`, `.?AVCThread@@`, `.?AVCVConActiveStateMessage@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## vconsole2.exe

Path: `game/bin/win64/vconsole2.exe`  
SHA-256: `cf4494ba43d3e3b95adb092804cb92fb5812480430730bf43c3217730d4cd8e0`  
Architecture: PE32+ x86-64; preferred base `0x140000000`; entry RVA `0x2247b8`.  
Exports: 5; RTTI types: 647; x64 unwind entries: 13920.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\vconsole2\_vpc_vconsole2\default\win64\_msvc_\Retail\vconsole2.pdb`; GUID `60afbace-ef00-4333-ad38-5eca228e858b`; age 14; lookup key `60AFBACEEF004333AD385ECA228E858BE`.

Normal import modules: `USER32.dll`, `tier0.dll`, `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll`, `WS2_32.dll`, `steam_api64.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x6f820` |  |
| `ExtractModuleMetadata` | `0x6fd80` |  |
| `GetResourceManifestCount` | `0x1c55b0` |  |
| `GetResourceManifests` | `0x1c55c0` |  |
| `InstallSchemaBindings` | `0x1c51d0` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUCPulseCell_BooleanSwitchState_DebugInfo_t@@`, `.?AUCPulseCell_Inflow_Wait_DebugInfo_t@@`, `.?AUCPulseCell_IntervalTimer_DebugInfo_t@@`, `.?AUCPulseCell_Timeline_DebugInfo_t@@`, `.?AUVDataInitInfo_t@@`, `.?AU_Anon@?$_Associated_state@H@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## vfx_dx11.dll

Path: `game/bin/win64/vfx_dx11.dll`  
SHA-256: `d67f5de837508a1d4a0f83a69ac95977aaf56f6d411d84b3e31b998a0a5a8ac8`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x5e768`.  
Exports: 3; RTTI types: 21; x64 unwind entries: 1917.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\vfx\vfx_dx\_vpc_vfx_dx11\default\win64\_msvc_\Retail\vfx_dx11.pdb`; GUID `34363695-f122-4ebd-9af0-b21215a87346`; age 11; lookup key `34363695F1224EBD9AF0B21215A87346B`.

Normal import modules: `slang.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x215a0` |  |
| `CreateInterface` | `0x3fea0` |  |
| `ExtractModuleMetadata` | `0x219e0` |  |

RTTI sample: `.?AUID3DInclude@@`, `.?AUISlangBlob@@`, `.?AUISlangUnknown@@`, `.?AV?$CRefCountServiceBase@VCRefMT@@@@`, `.?AV?$CRefCounted@V?$CRefCountServiceBase@VCRefMT@@@@@@`, `.?AVCExpressionParserCallback@@`, `.?AVCHlslParser@@`, `.?AVCShaderParamBuilder@@`, `.?AVCSlangBlob@SlangHelper@@`, `.?AVCStreamRewriter@@`, `.?AVCUtlStackMachineBuilder@@`, `.?AVCVfxD3dInclude@@`, `.?AVCVfxDx11@@`, `.?AVCVfxStackMachineBuilder@@`, `.?AVIExpressionShaderVariableCallback@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `VFX_DLL_001`, `VFileSystem017`.

## video64.dll

Path: `game/bin/win64/video64.dll`  
SHA-256: `bd0b26c5ce10647ab04bdedc8d33d7850e678f1a285748b46290c669e2f187fb`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x129e00`.  
Exports: 57; RTTI types: 379; x64 unwind entries: 11966.

PDB reference: `c:\buildslave\steam_main_win64\build\src\common\video\win64\videoRelease\video64.pdb`; GUID `c9f064cc-9c9f-4cf8-a448-c966e117a517`; age 19; lookup key `C9F064CC9C9F4CF8A448C966E117A51713`.

Normal import modules: `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`, `SHELL32.dll`, `OLEAUT32.dll`, `WS2_32.dll`, `PSAPI.DLL`, `libavcodec-58.dll`, `libavformat-58.dll`, `libavresample-4.dll`, `libavutil-56.dll`, `libswscale-5.dll`, `VERSION.dll`, `CRYPT32.dll`, `WSOCK32.dll`, `bcrypt.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `AllocImageYUV` | `0x91390` |  |
| `BConvertYUVtoRGB` | `0x91440` |  |
| `BScaleImageNV12` | `0x91620` |  |
| `BScaleImageYUV` | `0x91b00` |  |
| `CreateAudioFileDecoder` | `0x7f7b0` |  |
| `CreateAudioResampler` | `0x91ce0` |  |
| `CreateInterface` | `0x9aeb0` |  |
| `CreateRTMPStream` | `0x90f20` |  |
| `CreateVideoPlayer` | `0x8ee40` |  |
| `DeleteAudioFileDecoder` | `0x7f880` |  |
| `DeleteAudioResampler` | `0x91da0` |  |
| `DeleteRTMPStream` | `0x7f880` |  |
| `DeleteVideoDecoder` | `0x7fcf0` |  |
| `DeleteVideoEncoder` | `0x7fcf0` |  |
| `DeleteVideoPlayer` | `0x7f880` |  |
| `FreeImageYUV` | `0x91e00` |  |
| `RescaleBase` | `0x91e50` |  |
| `ValidateImageYUV` | `0x91e60` |  |
| `VideoPauseForValidation` | `0x903d0` |  |
| `VideoPlaybackInitialize` | `0x90430` |  |
| `VideoPlaybackRunFrame` | `0x905b0` |  |
| `VideoPlaybackShutdown` | `0x90720` |  |
| `VideoResumeFromValidation` | `0x90850` |  |
| `VideoValidate` | `0x908a0` |  |
| `VideoValidateStatics` | `0x837e0` |  |
| `XMLNormalizeBuf` | `0x16a0` |  |
| `XMLParser_Create` | `0x2ad0` |  |
| `XMLParser_Free` | `0x1740` |  |
| `XMLParser_GetCurrentColumn` | `0x1950` |  |
| `XMLParser_GetCurrentEntity` | `0x18b0` |  |

RTTI sample: `.?AUAddrConnectionInfo_t@CHTTPClient@@`, `.?AUCachedFileLRUData_t@CHTTPRequestCache@@`, `.?AUElementBucket_t@?$CUtlHashMapLRU@VCIPAndPort@@UProxyCacheEntry_t@CHTTPClient@@V?$CDefEquals@VCIPAndPort@@@@U?$HashFunctor@VCIPAndPort@@@@@@`, `.?AUHostLookup_t@CHTTPClient@@`, `.?AURecvState_t@CTCPConnection@@`, `.?AURequestWaitingValidationData_t@CHTTPRequestCache@@`, `.?AUSSLContextLookup_t@CHTTPClient@@`, `.?AUSSLNetConnectionData_t@CNetConnection@@`, `.?AUVideoAdaptationSet_t@@`, `.?AUVideoRepresentation_t@@`, `.?AUVideoSegmentTemplate_t@@`, `.?AUVideoSegment_t@@`, `.?AV?$CAutoRelease@VCCookieContainer@@@@`, `.?AV?$CAutoRelease@VCMetrics@@@@`, `.?AV?$CAutoRelease@VCMetricsPayload@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`, `WIN2000`, `crashhandler002`, `crashhandler003`, `ffdhe2048`, `ffdhe3072`, `ffdhe4096`, `gost2001`, `macos1010`, `macos1011`, `macos1012`, `macos1013`.

## visbuilder.dll

Path: `game/bin/win64/visbuilder.dll`  
SHA-256: `5c1f51afc4972ff07ffe29d1d4e064624b74d49970469756ac77dfe2168bfad3`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x12fad0`.  
Exports: 6; RTTI types: 218; x64 unwind entries: 5676.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\utils\visbuilder\_vpc_visbuilder\default\win64\_msvc_\Retail\visbuilder.pdb`; GUID `a14e4570-f5d4-48f0-8561-e149ef362857`; age 11; lookup key `A14E4570F5D448F08561E149EF362857B`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x4e1a0` |  |
| `CreateInterface` | `0x12bad0` |  |
| `ExtractModuleMetadata` | `0x4e680` |  |
| `GetResourceManifestCount` | `0x12d900` |  |
| `GetResourceManifests` | `0x12d910` |  |
| `InstallSchemaBindings` | `0x12d4f0` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$CBaseAppSystem@VIMapBuilderMgr@@@@`, `.?AV?$CBaseAppSystem@VIResourceTypeCompilerDynamicLoad@@@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1???$ParallelForEach@AEAV?$CUtlVector@UClusterPoints_t@CBoundaryPointsRayGenerator@@HV?$CUtlVectorMemory_Growable@UClusterPoints_t@CBoundaryPointsRayGenerator@@H$0A@@@@@V<lambda_1>@?1??BuildTracePointsForClusters@CBoundaryPointsRayGenerator@@QEAAXXZ@@@YAXAEAV?$CUtlVector@UClusterPoints_t@CBoundaryPointsRayGenerator@@HV?$CUtlVectorMemory_Growable@UClusterPoints_t@CBoundaryPointsRayGenerator@@H$0A@@@@@PEBD$$QEAV1?1??BuildTracePointsForClusters@CBoundaryPointsRayGenerator@@QEAAXXZ@HHW4JobPriority_t@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1???$ParallelForEach@AEAV?$CUtlVector@URegionStatus_t@@HV?$CUtlVectorMemory_Growable@URegionStatus_t@@H$0A@@@@@V<lambda_1>@?1??ComputeRegionStatus@CVoxelSampler3@@QEAAXXZ@@@YAXAEAV?$CUtlVector@URegionStatus_t@@HV?$CUtlVectorMemory_Growable@URegionStatus_t@@H$0A@@@@@PEBD$$QEAV1?1??ComputeRegionStatus@CVoxelSampler3@@QEAAXXZ@HHW4JobPriority_t@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1??AdaptivelySampleBorders@CVoxelSampler3@@QEAAXPEAVIResourceCompilerContext@@@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1??MergeBestCandidates@CBoxMerge@@QEAA_NMM@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1??RecomputeClusterCostLists@CVisClusterList@@AEAAIXZ@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?1??RecomputeClusterCostListsForPartition@CVisClusterList@@AEAAIH@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?2??MergeClusterSet@CVoxelSampler3@@QEAAMAEAVCVoxClusterSet@@IM@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_1>@?2??MergeInsideRegions@CVoxelSampler3@@QEAAXI@Z@@CParallelProcessLauncher@@`, `.?AV?$CParallelLambdaJob@V<lambda_2>@?5??MergeClusterSet@CVoxelSampler3@@QEAAMAEAVCVoxClusterSet@@IM@Z@@CParallelProcessLauncher@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## vphysics2.dll

Path: `game/bin/win64/vphysics2.dll`  
SHA-256: `226262075f0fccde39da4040c2d9da911c45a384a5ddacc859b70af9f1ffdeca`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x355ab4`.  
Exports: 25; RTTI types: 339; x64 unwind entries: 14814.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\vphysics2\_vpc_vphysics2\default\win64\_msvc_\Retail\vphysics2.pdb`; GUID `ecaa296d-8dd4-431d-aa7d-a3f0854c1bb5`; age 11; lookup key `ECAA296D8DD4431DAA7DA3F0854C1BB5B`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x85900` |  |
| `CreateInterface` | `0x350120` |  |
| `ExtractModuleMetadata` | `0x85d90` |  |
| `GetResourceManifestCount` | `0x352390` |  |
| `GetResourceManifests` | `0x3523a0` |  |
| `InstallSchemaBindings` | `0x3526d0` |  |
| `RnCompoundClone` | `0x1ac3e0` |  |
| `RnCompoundDestroy` | `0x1ac580` |  |
| `RnHullClone` | `0x1ac5b0` |  |
| `RnHullCreate` | `0x1ac5c0` |  |
| `RnHullCreateBox` | `0x1ac890` |  |
| `RnHullCreateCone` | `0x1acfd0` |  |
| `RnHullCreateCylinder` | `0x1ad370` |  |
| `RnHullCreateRuntime` | `0x1ad740` |  |
| `RnHullCreateRuntimeFromPlanes` | `0x1ad7b0` |  |
| `RnHullDestroy` | `0x1ad860` |  |
| `RnMeshClone` | `0x1ad890` |  |
| `RnMeshCreate` | `0x1ad8a0` |  |
| `RnMeshDestroy` | `0x1ae9a0` |  |
| `RnMeshHideTriangles` | `0x1ae9d0` |  |
| `RnMeshUpdateChangedVertices` | `0x1af6f0` |  |
| `RnMeshUpdateTriangleAABBs` | `0x1af9b0` |  |
| `RnShutdown` | `0x9030` |  |
| `RnStartup` | `0x1afc50` |  |
| `RnUnitTest` | `0x9030` |  |

RTTI sample: `.?AUAsyncProcessResult@CBaseIntrospectedToKV3TypeManager@@`, `.?AUCMorphSetData@@`, `.?AUPhysicsAsyncProcessResult@@`, `.?AV?$CBaseAppSystem@VIVPhysics2@@@@`, `.?AV?$CDefaultKV3TransferResourceTypeManager@$0GGHCHFHDHG@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsAnyJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsConicalJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsGenericJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsPrismaticJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsPulleyJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsRevoluteJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsSphericalJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsSplineJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsSpringJoint@@@@`, `.?AV?$CFnPhysicsJointBaseImpl@VIFnPhysicsWeldJoint@@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## vscript.dll

Path: `game/bin/win64/vscript.dll`  
SHA-256: `4df65c1b4edf39f96e10a976d47ab44aa6cf525695650cb5e8c001f7e4330aab`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xbbc14`.  
Exports: 183; RTTI types: 30; x64 unwind entries: 3283.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\vscript\_vpc_vscript\default\win64\_msvc_\Retail\vscript.pdb`; GUID `c6f39375-0502-467c-8e8e-8588fe192e67`; age 10; lookup key `C6F393750502467C8E8E8588FE192E67A`.

Normal import modules: `WS2_32.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x3f810` |  |
| `CreateInterface` | `0x63f60` |  |
| `ExtractModuleMetadata` | `0x3fd00` |  |
| `GetResourceManifestCount` | `0x647a0` |  |
| `GetResourceManifests` | `0x647b0` |  |
| `InstallSchemaBindings` | `0x64a00` |  |
| `__swprintf_l` | `0x6f840` |  |
| `__vswprintf_l` | `0x6f8a0` |  |
| `_fprintf_l` | `0x6f910` |  |
| `_fprintf_p` | `0x6f960` |  |
| `_fprintf_p_l` | `0x6f9b0` |  |
| `_fprintf_s_l` | `0x6fa00` |  |
| `_fscanf_l` | `0x6fa50` |  |
| `_fscanf_s_l` | `0x6faa0` |  |
| `_fwprintf_l` | `0x6faf0` |  |
| `_fwprintf_p` | `0x6fb40` |  |
| `_fwprintf_p_l` | `0x6fb90` |  |
| `_fwprintf_s_l` | `0x6fbe0` |  |
| `_fwscanf_l` | `0x6fc30` |  |
| `_fwscanf_s_l` | `0x6fc80` |  |
| `_printf_l` | `0x6fcd0` |  |
| `_printf_p` | `0x6fd30` |  |
| `_printf_p_l` | `0x6fd90` |  |
| `_printf_s_l` | `0x6fdf0` |  |
| `_scanf_l` | `0x6fe50` |  |
| `_scanf_s_l` | `0x6fea0` |  |
| `_scprintf` | `0x6ff00` |  |
| `_scprintf_l` | `0x6ff60` |  |
| `_scprintf_p` | `0x6ffc0` |  |
| `_scprintf_p_l` | `0x70020` |  |

RTTI sample: `.?AV?$CBaseAppSystem@VIScriptManager@@@@`, `.?AV?$CTier0AppSystem@VIScriptManager@@$0A@@@`, `.?AV?$CTier1AppSystem@VIScriptManager@@$0A@@@`, `.?AV?$CTier2AppSystem@VIScriptManager@@$0A@@@`, `.?AVCBaseVModuleMetadataProvider@@`, `.?AVCLuaDBGp@@`, `.?AVCLuaDbg@@`, `.?AVCLuaDbgInspector@@`, `.?AVCLuaVM@@`, `.?AVCModuleMetadataProvider_ResourceManifests@@`, `.?AVCNopVM@@`, `.?AVCPostConnectCallback@@`, `.?AVCSHA1@@`, `.?AVCSchemaInstallCallback@@`, `.?AVCScriptManager@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## worldrenderer.dll

Path: `game/bin/win64/worldrenderer.dll`  
SHA-256: `ffad0bc72f64228a36aa2e370b8e6e3b29ae6b0747d42436d40b2ce25be5723d`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x104490`.  
Exports: 6; RTTI types: 246; x64 unwind entries: 6033.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\worldrenderer\_vpc_worldrenderer\default\win64\_msvc_\Retail\worldrenderer.pdb`; GUID `68d4cf47-8df4-4af4-bf27-21932a8dca06`; age 12; lookup key `68D4CF478DF44AF4BF2721932A8DCA06C`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x2ec60` |  |
| `CreateInterface` | `0xfd680` |  |
| `ExtractModuleMetadata` | `0x2f0b0` |  |
| `GetResourceManifestCount` | `0xff260` |  |
| `GetResourceManifests` | `0xff270` |  |
| `InstallSchemaBindings` | `0xff5a0` |  |

RTTI sample: `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AUAsyncProcessResult@CBaseIntrospectedToKV3TypeManager@@`, `.?AUCPulseCell_Inflow_Wait_DebugInfo_t@@`, `.?AUEntityKeyValueData_t@@`, `.?AUPermEntityLumpData_t@@`, `.?AU_Anon@?$_Associated_state@H@std@@`, `.?AU_Anon@?$_Associated_state@I@std@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## csgo_legacy_app.exe

Path: `game/csgo/bin/legacy/csgo_legacy_app.exe`  
SHA-256: `de96c3b37e3aaedcf4d6a24c5f6c30acd60f3593e8d452dd9857d682ea3fb114`  
Architecture: PE32 x86; preferred base `0x400000`; entry RVA `0xf3ed`.  
Exports: 7; RTTI types: 191; x64 unwind entries: None.

PDB reference: `c:\buildslave\csgo_pcbeta_win32\build\src\launcher_main\Release\csgo.pdb`; GUID `ac54e31a-f29d-4407-9667-86c6cdb8bad3`; age 8; lookup key `AC54E31AF29D4407966786C6CDB8BAD38`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `AmdPowerXpressRequestHighPerformance` | `0x7888c` |  |
| `BSecureAllowed` | `0x1500` |  |
| `CountFilesCompletedTrustCheck` | `0x14f0` |  |
| `CountFilesNeedTrustCheck` | `0x14e0` |  |
| `GetTotalFilesLoaded` | `0x14d0` |  |
| `NvOptimusEnablement` | `0x78888` |  |
| `RuntimeCheck` | `0x1550` |  |

RTTI sample: `.?AUEncryptionStandard@CryptoPP@@`, `.?AUFilterPutSpaceHelper@CryptoPP@@`, `.?AUInitializeInteger@CryptoPP@@`, `.?AUInputRejected@?$InputRejecting@VBufferedTransformation@CryptoPP@@@CryptoPP@@`, `.?AUInputRejected@?$InputRejecting@VFilter@CryptoPP@@@CryptoPP@@`, `.?AUNoChannelSupport@BufferedTransformation@CryptoPP@@`, `.?AURijndael_Info@CryptoPP@@`, `.?AV?$ASN1CryptoMaterial@VPrivateKey@CryptoPP@@@CryptoPP@@`, `.?AV?$ASN1CryptoMaterial@VPublicKey@CryptoPP@@@CryptoPP@@`, `.?AV?$AbstractEuclideanDomain@VInteger@CryptoPP@@@CryptoPP@@`, `.?AV?$AbstractGroup@VInteger@CryptoPP@@@CryptoPP@@`, `.?AV?$AbstractRing@VInteger@CryptoPP@@@CryptoPP@@`, `.?AV?$AlgorithmImpl@V?$IteratedHash@IU?$EnumToType@W4ByteOrder@CryptoPP@@$00@CryptoPP@@$0EA@VHashTransformation@2@@CryptoPP@@VSHA1@2@@CryptoPP@@`, `.?AV?$AlgorithmImpl@V?$IteratedHash@IU?$EnumToType@W4ByteOrder@CryptoPP@@$00@CryptoPP@@$0EA@VHashTransformation@2@@CryptoPP@@VSHA256@2@@CryptoPP@@`, `.?AV?$AlgorithmImpl@V?$SimpleKeyingInterfaceImpl@V?$TwoBases@VBlockCipher@CryptoPP@@URijndael_Info@2@@CryptoPP@@V12@@CryptoPP@@V12@@CryptoPP@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `SystemFunction036`.

## client.dll

Path: `game/csgo/bin/win64/client.dll`  
SHA-256: `99a6720b29d8b477d55f0a2d7fd4333b5df13f354ba723804e996bead1231aa8`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x19f3f20`.  
Exports: 6; RTTI types: 7354; x64 unwind entries: 127791.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\game\client\_vpc_client_csgo\unity\win64\_msvc_\Retail\client.pdb`; GUID `419b8bb8-78e2-4868-8a21-181abc984349`; age 56; lookup key `419B8BB878E248688A21181ABC98434938`.

Normal import modules: `USER32.dll`, `ADVAPI32.dll`, `embree3.dll`, `steam_api64.dll`, `tier0.dll`, `v8.dll`, `video64.dll`, `KERNEL32.dll`, `WS2_32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xc27580` |  |
| `CreateInterface` | `0x19c5ca0` |  |
| `ExtractModuleMetadata` | `0xc27d80` |  |
| `GetResourceManifestCount` | `0x19d1610` |  |
| `GetResourceManifests` | `0x19d1620` |  |
| `InstallSchemaBindings` | `0x19d3400` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_5a9ef80d79df7873994019ff15dceae5>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_60c5de84e69e46aa92419d47e2f3ad51>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_85226a1ade6d27c19b16702c44b1a394>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_PPLTaskHandle@EU?$_InitialTaskHandle@XV<lambda_e7905d7dbd7cab7f9b19199433c4f5d3>@@U_TypeSelectorNoAsync@details@Concurrency@@@?$task@E@Concurrency@@U_TaskProcHandle@details@3@@details@Concurrency@@`, `.?AU?$_Task_impl@E@details@Concurrency@@`, `.?AU?$default_delete@VCMouseTraceCoordReadback@@@std@@`, `.?AUAI_BaseNPC_DebugSnapshotData_t@@`, `.?AUAI_DefaultNPC_DebugSnapshotData_t@@`, `.?AUAI_GroundRootMotionMotor_DebugSnapshotData_t@@`, `.?AUActiveModelConfig_t@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `CWeaponHKP2000`, `CWeaponXM1014`.

## host.dll

Path: `game/csgo/bin/win64/host.dll`  
SHA-256: `e0decb8e7fbd4968acc28eddef21db25faf570804e99c6f1e0e58fc3c105c582`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0xcf714`.  
Exports: 6; RTTI types: 240; x64 unwind entries: 4077.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\game\host\_vpc_host_csgo\default\win64\_msvc_\Retail\host.pdb`; GUID `4760610d-0b0b-4af4-936f-edfb46614e22`; age 12; lookup key `4760610D0B0B4AF4936FEDFB46614E22C`.

Normal import modules: `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x1f6b0` |  |
| `CreateInterface` | `0xc44c0` |  |
| `ExtractModuleMetadata` | `0x1fb90` |  |
| `GetResourceManifestCount` | `0xcd890` |  |
| `GetResourceManifests` | `0xcd8a0` |  |
| `InstallSchemaBindings` | `0xcd480` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AUIVTableIndexCalculator@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$CAnimScriptFactory@VEmptyTestScript@@@@`, `.?AV?$CBaseAppSystem@VIDebugDrawQueueMgr@@@@`, `.?AV?$CBaseAppSystem@VIGameModelInfo@@@@`, `.?AV?$CBaseAppSystem@VIGameSystem2HostHook@@@@`, `.?AV?$CBaseAppSystem@VIHostUtils@@@@`, `.?AV?$CBaseAppSystem@VIPredictionDiffMgr@@@@`, `.?AV?$CBaseAppSystem@VISaveRestoreDataMgr@@@@`, `.?AV?$CBaseAppSystem@VISinglePlayerSharedMemory@@@@`, `.?AV?$CBaseAppSystem@VISource2Host@@@@`, `.?AV?$CGameAppSystem@VIDebugDrawQueueMgr@@$0A@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## matchmaking.dll

Path: `game/csgo/bin/win64/matchmaking.dll`  
SHA-256: `f57f03f489715326a96ce90355b45d19cb51ea3f1b60ba7f81274f2b80769780`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x112754`.  
Exports: 6; RTTI types: 378; x64 unwind entries: 6297.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\matchmaking\_vpc_matchmaking_csgo\default\win64\_msvc_\Retail\matchmaking.pdb`; GUID `9982f2b8-fd4b-44df-ab38-a17e4d39470b`; age 13; lookup key `9982F2B8FD4B44DFAB38A17E4D39470BD`.

Normal import modules: `steam_api64.dll`, `tier0.dll`, `KERNEL32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0x3e880` |  |
| `CreateInterface` | `0x108af0` |  |
| `ExtractModuleMetadata` | `0x3ed60` |  |
| `GetResourceManifestCount` | `0x110650` |  |
| `GetResourceManifests` | `0x110660` |  |
| `InstallSchemaBindings` | `0x1108b0` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AUKeyValue@ExtensionSet@internal@protobuf@google@@`, `.?AU_Crt_new_delete@std@@`, `.?AUctype_base@std@@`, `.?AV?$CBaseAppSystem@VIGameTypes@@@@`, `.?AV?$CBaseAppSystem@VIMatchFramework@@@@`, `.?AV?$CCallResult@VCLeaderboardRequestQueue@@ULeaderboardFindResult_t@@@@`, `.?AV?$CCallResult@VCLeaderboardRequestQueue@@ULeaderboardScoresDownloaded_t@@@@`, `.?AV?$CCallResult@VCMatchSearcher@@ULobbyMatchList_t@@@@`, `.?AV?$CCallResult@VCSteamLeaderboardWriter@@ULeaderboardFindResult_t@@@@`, `.?AV?$CCallResult@VCSteamLeaderboardWriter@@ULeaderboardScoreUploaded_t@@@@`, `.?AV?$CCallResult@VCSysSessionHost@@ULobbyCreated_t@@@@`, `.?AV?$CCallback@VCGCClient@GCSDK@@UGCMessageAvailable_t@@$0A@@@`, `.?AV?$CCallback@VCGCClient@GCSDK@@USteamServerConnectFailure_t@@$0A@@@`, `.?AV?$CCallback@VCGCClient@GCSDK@@USteamServersConnected_t@@$0A@@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `ClientLocalServerAccess001`, `ClientServerSharedHandleSystem001`.

## server.dll

Path: `game/csgo/bin/win64/server.dll`  
SHA-256: `84d596644ad574ac70f427777c89f9006a8023f493c46f4c215f40bceef13cbb`  
Architecture: PE32+ x86-64; preferred base `0x180000000`; entry RVA `0x16a5bcc`.  
Exports: 6; RTTI types: 6255; x64 unwind entries: 108298.

PDB reference: `C:\buildworker\csgo_rel_win64\build\_build_\game\server\_vpc_server_csgo\unity\win64\_msvc_\Retail\server.pdb`; GUID `db2052bd-a6d3-45a6-9e4b-5a492a3598ae`; age 44; lookup key `DB2052BDA6D345A69E4B5A492A3598AE2C`.

Normal import modules: `steam_api64.dll`, `tier0.dll`, `v8.dll`, `KERNEL32.dll`, `USER32.dll`, `ADVAPI32.dll`, `WS2_32.dll`.

Export sample (first 30 entries; complete table in inventory.json):

| Name | RVA | Forwarder |
|---|---|---|
| `BinaryProperties_GetValue` | `0xfe4090` |  |
| `CreateInterface` | `0x1690cf0` |  |
| `ExtractModuleMetadata` | `0xfe4810` |  |
| `GetResourceManifestCount` | `0x169da80` |  |
| `GetResourceManifests` | `0x169da90` |  |
| `InstallSchemaBindings` | `0x169fb30` |  |

RTTI sample: `.?AU?$Container@VUnknownFieldSet@protobuf@google@@@InternalMetadata@internal@protobuf@google@@`, `.?AU?$default_delete@VCNavGenPolyProcessorStageBase@@@std@@`, `.?AUAI_BaseNPC_DebugSnapshotData_t@@`, `.?AUAI_DefaultNPC_DebugSnapshotData_t@@`, `.?AUAI_GroundRootMotionMotor_DebugSnapshotData_t@@`, `.?AUActiveModelConfig_t@@`, `.?AUAmmoTypeInfo_t@@`, `.?AUAnimGraph2SerializedPoseRecipeSlot_t@@`, `.?AUCDefinition@CNmAimCSNode@@`, `.?AUCDefinition@CNmAndNode@@`, `.?AUCDefinition@CNmAnimationPoseNode@@`, `.?AUCDefinition@CNmBlend1DNode@@`, `.?AUCDefinition@CNmBlend2DNode@@`, `.?AUCDefinition@CNmBodyGroupNode@@`, `.?AUCDefinition@CNmBoneMaskBlendNode@@`.

Interface-like string sample (unconfirmed; shared constants and false positives included): `AnimGraphEditorUtils001`, `AnimationSystemUtils_001`, `AnimationSystem_001`, `AssetBrowserSystem_001`, `AssetPreviewSystem_001`, `AssetRenameSystem_001`, `AssetSystem001`, `AssetSystemTest001`, `BakedLODBuilderMgr001`, `BenchmarkService001`, `CWeaponHKP2000`, `CWeaponXM1014`.
