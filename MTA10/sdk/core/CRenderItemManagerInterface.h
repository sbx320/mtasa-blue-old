/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        CRenderItemManagerInterface.h
*  PURPOSE:
*  DEVELOPERS:  idiot
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

struct ID3DXFont;
struct IDirect3DTexture9;
struct IDirect3DSurface9;
struct ID3DXEffect;
typedef LPCSTR D3DXHANDLE;
class CGUIFont;
struct SRenderItem;
struct SMaterialItem;
struct SFontItem;
struct STextureItem;
struct SShaderItem;
struct SRenderTargetItem;
struct SScreenSourceItem;


//
// CRenderItemManagerInterface
//
// Interface for client module
//
class CRenderItemManagerInterface
{
public:
    virtual                     ~CRenderItemManagerInterface    ( void ) {}

    // CRenderItemManagerInterface
    virtual SFontItem*          CreateFont                  ( const SString& strFullFilePath, const SString& strFontName, uint uiSize, bool bBold ) = 0;
    virtual STextureItem*       CreateTexture               ( const SString& strFullFilePath ) = 0;
    virtual SShaderItem*        CreateShader                ( const SString& strFullFilePath, SString& strOutStatus ) = 0;
    virtual SRenderTargetItem*  CreateRenderTarget          ( uint uiSizeX, uint uiSizeY, bool bWithAlphaChannel ) = 0;
    virtual SScreenSourceItem*  CreateScreenSource          ( uint uiSizeX, uint uiSizeY ) = 0;
    virtual void                ReleaseRenderItem           ( SRenderItem* pItem ) = 0;
    virtual bool                SetShaderValue              ( SShaderItem* pItem, const SString& strName, STextureItem* pTextureItem ) = 0;
    virtual bool                SetShaderValue              ( SShaderItem* pItem, const SString& strName, bool bValue ) = 0;
    virtual bool                SetShaderValue              ( SShaderItem* pItem, const SString& strName, const float* pfValues, uint uiCount ) = 0;
    virtual bool                SetRenderTarget             ( SRenderTargetItem* pItem, bool bClear ) = 0;
    virtual bool                RestoreDefaultRenderTarget  ( void ) = 0;
    virtual void                UpdateBackBufferCopy        ( void ) = 0;
    virtual void                UpdateScreenSource          ( SScreenSourceItem* pScreenSourceItem ) = 0;
};



//
// SRenderItem + derived class
//
// Render item is the core's version of something renderable
//
enum eRenderItemClassTypes
{
    CLASS_SRenderItem,
    CLASS_SFontItem,
    CLASS_SMaterialItem,
    CLASS_STextureItem,
    CLASS_SShaderItem,
    CLASS_SRenderTargetItem,
    CLASS_SScreenSourceItem,
};

struct SRenderItem
{
    DECLARE_BASE_CLASS( SRenderItem )
    SRenderItem  ( void ) : ClassInit ( this ) {}
    virtual ~SRenderItem ( void ) {}
    void Release ( void ) { pManager->ReleaseRenderItem ( this ); }
    void AddRef  ( void ) { ++iRefCount; }

    CRenderItemManagerInterface* pManager;
    int             iRefCount;
    bool            bInQueue;
};

struct SFontItem : public SRenderItem
{
    DECLARE_CLASS( SFontItem, SRenderItem )
    SFontItem ( void ) : ClassInit ( this ) {}

    SString         strCEGUIFontName;
    CGUIFont*       pFntCEGUI;
    SString         strFullFilePath;
    ID3DXFont*      pFntNormal;
    ID3DXFont*      pFntBig;
};

struct SMaterialItem : public SRenderItem
{
    DECLARE_CLASS( SMaterialItem, SRenderItem )
    SMaterialItem ( void ) : ClassInit ( this ) {}

    uint uiSizeX;
    uint uiSizeY;
};


struct SShaderItem : public SMaterialItem
{
    DECLARE_CLASS( SShaderItem, SMaterialItem )
    SShaderItem ( void ) : ClassInit ( this ) {}

    ID3DXEffect* pD3DEffect;
    std::map < SString, D3DXHANDLE > parameterMap;
    D3DXHANDLE hWorld, hView, hProjection, hAll, hTime;
    D3DXHANDLE hFirstTexture;
};


struct STextureItem : public SMaterialItem
{
    DECLARE_CLASS( STextureItem, SMaterialItem )
    STextureItem ( void ) : ClassInit ( this ) {}

    IDirect3DTexture9* pD3DTexture;
};

struct SRenderTargetItem : public STextureItem
{
    DECLARE_CLASS( SRenderTargetItem, STextureItem )
    SRenderTargetItem ( void ) : ClassInit ( this ) {}

    IDirect3DSurface9* pD3DRenderTargetSurface;
    IDirect3DSurface9* pD3DZStencilSurface;
};

struct SScreenSourceItem : public STextureItem
{
    DECLARE_CLASS( SScreenSourceItem, STextureItem )
    SScreenSourceItem ( void ) : ClassInit ( this ) {}

    IDirect3DSurface9* pD3DRenderTargetSurface;
    uint uiRevision;
};
