#include "ReactOS_COM_Shim.h"

HRESULT CReactOSShellFolder::CreateInstance(REFIID riid, void **ppv, FolderType folderType)
{
    *ppv = NULL;
    CReactOSShellFolder *pFolder = new CReactOSShellFolder(folderType);
    if (!pFolder)
    {
        return E_OUTOFMEMORY;
    }
    HRESULT hr = pFolder->QueryInterface(riid, ppv);
    pFolder->Release();
    return hr;
}

// IUnknown
IFACEMETHODIMP CReactOSShellFolder::QueryInterface(REFIID riid, void **ppv)
{
    *ppv = NULL;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IShellFolder) || IsEqualIID(riid, IID_IShellFolder2))
    {
        *ppv = static_cast<IShellFolder2*>(this);
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

IFACEMETHODIMP_(ULONG) CReactOSShellFolder::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) CReactOSShellFolder::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }
    return cRef;
}

// CReactOSShellFolderEnum
IFACEMETHODIMP CReactOSShellFolderEnum::QueryInterface(REFIID riid, void **ppv)
{
    *ppv = NULL;
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IEnumIDList))
    {
        *ppv = static_cast<IEnumIDList*>(this);
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

IFACEMETHODIMP_(ULONG) CReactOSShellFolderEnum::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) CReactOSShellFolderEnum::Release()
{
    long cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }
    return cRef;
}

CReactOSShellFolderEnum::CReactOSShellFolderEnum(CReactOSShellFolder *pFolder, FolderType folderType) : m_cRef(1), m_index(0), m_pFolder(pFolder)
{
    m_pFolder->AddRef();
    switch (folderType)
    {
    case FOLDER_TYPE_DESKTOP:
        m_items.push_back(L"Programs");
        m_items.push_back(L"Control Panel");
        m_items.push_back(L"Settings");
        break;
    case FOLDER_TYPE_PROGRAMS:
        m_items.push_back(L"Accessories");
        m_items.push_back(L"Games");
        m_items.push_back(L"Startup");
        break;
    case FOLDER_TYPE_CONTROLPANEL:
        m_items.push_back(L"Appearance and Personalization");
        m_items.push_back(L"Hardware and Sound");
        break;
    case FOLDER_TYPE_SETTINGS:
        m_items.push_back(L"System");
        m_items.push_back(L"Devices");
        break;
    }
}

IFACEMETHODIMP CReactOSShellFolderEnum::Next(ULONG celt, PITEMID_CHILD *rgelt, ULONG *pceltFetched)
{
    ULONG celtFetched = 0;
    if (pceltFetched)
        *pceltFetched = 0;

    while (celt > 0 && m_index < m_items.size())
    {
        if (SUCCEEDED(m_pFolder->CreateChildID(m_items[m_index].c_str(), &rgelt[celtFetched])))
        {
            celtFetched++;
            m_index++;
        }
        celt--;
    }

    if (pceltFetched)
        *pceltFetched = celtFetched;

    return (celtFetched == celt) ? S_OK : S_FALSE;
}

IFACEMETHODIMP CReactOSShellFolderEnum::Skip(ULONG celt)
{
    m_index += celt;
    return S_OK;
}

IFACEMETHODIMP CReactOSShellFolderEnum::Reset()
{
    m_index = 0;
    return S_OK;
}

IFACEMETHODIMP CReactOSShellFolderEnum::Clone(IEnumIDList **ppenum)
{
    return E_NOTIMPL;
}

static HRESULT StringToStrRet(PCWSTR pszName, STRRET *pStrRet)
{
    pStrRet->uType = STRRET_WSTR;
    return SHStrDup(pszName, &pStrRet->pOleStr);
}

// IShellFolder
IFACEMETHODIMP CReactOSShellFolder::ParseDisplayName(HWND hwnd, IBindCtx *pbc, PWSTR pszDisplayName, ULONG *pchEaten, PIDLIST_RELATIVE *ppidl, ULONG *pdwAttributes) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::EnumObjects(HWND hwnd, SHCONTF grfFlags, IEnumIDList **ppenumIDList) { *ppenumIDList = new CReactOSShellFolderEnum(this, m_folderType); return S_OK; }
IFACEMETHODIMP CReactOSShellFolder::BindToObject(PCUIDLIST_RELATIVE pidl, IBindCtx *pbc, REFIID riid, void **ppv)
{
    *ppv = NULL;
    if (!pidl || !pidl->mkid.cb)
        return E_INVALIDARG;

    const wchar_t* name = (const wchar_t*)((BYTE*)pidl + sizeof(USHORT));
    FolderType subFolderType;

    if (wcscmp(name, L"Programs") == 0)
        subFolderType = FOLDER_TYPE_PROGRAMS;
    else if (wcscmp(name, L"Control Panel") == 0)
        subFolderType = FOLDER_TYPE_CONTROLPANEL;
    else if (wcscmp(name, L"Settings") == 0)
        subFolderType = FOLDER_TYPE_SETTINGS;
    else
        return E_INVALIDARG;

    return CreateInstance(riid, ppv, subFolderType);
}
IFACEMETHODIMP CReactOSShellFolder::BindToStorage(PCUIDLIST_RELATIVE pidl, IBindCtx *pbc, REFIID riid, void **ppv) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::CompareIDs(LPARAM lParam, PCUIDLIST_RELATIVE pidl1, PCUIDLIST_RELATIVE pidl2) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::CreateViewObject(HWND hwndOwner, REFIID riid, void **ppv) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::GetAttributesOf(UINT cidl, PCUITEMID_CHILD_ARRAY apidl, SFGAOF *rgfInOut) { *rgfInOut = SFGAO_FOLDER | SFGAO_HASSUBFOLDER | SFGAO_BROWSABLE; return S_OK; }
IFACEMETHODIMP CReactOSShellFolder::GetUIObjectOf(HWND hwndOwner, UINT cidl, PCUITEMID_CHILD_ARRAY apidl, REFIID riid, UINT *rgfInOut, void **ppv) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::GetDisplayNameOf(PCUITEMID_CHILD pidl, SHGDNF uFlags, STRRET *pName)
{
    if (!pidl || !pidl->mkid.cb)
        return E_INVALIDARG;

    const wchar_t* name = (const wchar_t*)((BYTE*)pidl + sizeof(USHORT));
    return StringToStrRet(name, pName);
}
IFACEMETHODIMP CReactOSShellFolder::SetNameOf(HWND hwnd, PCUITEMID_CHILD pidl, LPCWSTR pszName, SHGDNF uFlags, PITEMID_CHILD *ppidlOut) { return E_NOTIMPL; }

// IShellFolder2
IFACEMETHODIMP CReactOSShellFolder::GetDefaultSearchGUID(GUID *pguid) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::EnumSearches(IEnumExtraSearch **ppenum) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::GetDefaultColumn(DWORD dwRes, ULONG *pSort, ULONG *pDisplay) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::GetDefaultColumnState(UINT iColumn, SHCOLSTATEF *pcsFlags) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::GetDetailsEx(PCUITEMID_CHILD pidl, const SHCOLUMNID *pscid, VARIANT *pv) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::GetDetailsOf(PCUITEMID_CHILD pidl, UINT iColumn, SHELLDETAILS *psd) { return E_NOTIMPL; }
IFACEMETHODIMP CReactOSShellFolder::MapColumnToSCID(UINT iColumn, SHCOLUMNID *pscid) { return E_NOTIMPL; }

HRESULT CReactOSShellFolder::CreateChildID(PCWSTR pszName, PITEMID_CHILD *ppidl)
{
    // A basic implementation that creates a simple ID based on the name
    size_t nameBytes = (wcslen(pszName) + 1) * sizeof(wchar_t);
    USHORT totalSize = sizeof(USHORT) + nameBytes;

    *ppidl = (PITEMID_CHILD)CoTaskMemAlloc(totalSize + sizeof(USHORT));
    if (!*ppidl)
        return E_OUTOFMEMORY;

    (*ppidl)->mkid.cb = totalSize;
    memcpy((*ppidl)->mkid.abID, pszName, nameBytes);
    ((USHORT*)((BYTE*)*ppidl + totalSize))[0] = 0; // Null terminator for the PIDL
    return S_OK;
}
