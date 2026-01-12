#pragma once

#include <shobjidl.h>

#include <vector>
#include <string>

class CReactOSShellFolder;

class CReactOSShellFolderEnum : public IEnumIDList
{
public:
    CReactOSShellFolderEnum(CReactOSShellFolder *pFolder, FolderType folderType);

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IEnumIDList
    IFACEMETHODIMP Next(ULONG celt, PITEMID_CHILD *rgelt, ULONG *pceltFetched);
    IFACEMETHODIMP Skip(ULONG celt);
    IFACEMETHODIMP Reset();
    IFACEMETHODIMP Clone(IEnumIDList **ppenum);

private:
    LONG m_cRef;
    ULONG m_index;
    CReactOSShellFolder *m_pFolder;
    std::vector<std::wstring> m_items;
};

enum FolderType
{
    FOLDER_TYPE_DESKTOP,
    FOLDER_TYPE_PROGRAMS,
    FOLDER_TYPE_CONTROLPANEL,
    FOLDER_TYPE_SETTINGS,
};

class CReactOSShellFolder : public IShellFolder2
{
public:
    static HRESULT CreateInstance(REFIID riid, void **ppv, FolderType folderType = FOLDER_TYPE_DESKTOP);
    HRESULT CreateChildID(PCWSTR pszName, PITEMID_CHILD *ppidl);

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellFolder
    IFACEMETHODIMP ParseDisplayName(HWND hwnd, IBindCtx *pbc, PWSTR pszDisplayName, ULONG *pchEaten, PIDLIST_RELATIVE *ppidl, ULONG *pdwAttributes);
    IFACEMETHODIMP EnumObjects(HWND hwnd, SHCONTF grfFlags, IEnumIDList **ppenumIDList);
    IFACEMETHODIMP BindToObject(PCUIDLIST_RELATIVE pidl, IBindCtx *pbc, REFIID riid, void **ppv);
    IFACEMETHODIMP BindToStorage(PCUIDLIST_RELATIVE pidl, IBindCtx *pbc, REFIID riid, void **ppv);
    IFACEMETHODIMP CompareIDs(LPARAM lParam, PCUIDLIST_RELATIVE pidl1, PCUIDLIST_RELATIVE pidl2);
    IFACEMETHODIMP CreateViewObject(HWND hwndOwner, REFIID riid, void **ppv);
    IFACEMETHODIMP GetAttributesOf(UINT cidl, PCUITEMID_CHILD_ARRAY apidl, SFGAOF *rgfInOut);
    IFACEMETHODIMP GetUIObjectOf(HWND hwndOwner, UINT cidl, PCUITEMID_CHILD_ARRAY apidl, REFIID riid, UINT *rgfInOut, void **ppv);
    IFACEMETHODIMP GetDisplayNameOf(PCUITEMID_CHILD pidl, SHGDNF uFlags, STRRET *pName);
    IFACEMETHODIMP SetNameOf(HWND hwnd, PCUITEMID_CHILD pidl, LPCWSTR pszName, SHGDNF uFlags, PITEMID_CHILD *ppidlOut);

    // IShellFolder2
    IFACEMETHODIMP GetDefaultSearchGUID(GUID *pguid);
    IFACEMETHODIMP EnumSearches(IEnumExtraSearch **ppenum);
    IFACEMETHODIMP GetDefaultColumn(DWORD dwRes, ULONG *pSort, ULONG *pDisplay);
    IFACEMETHODIMP GetDefaultColumnState(UINT iColumn, SHCOLSTATEF *pcsFlags);
    IFACEMETHODIMP GetDetailsEx(PCUITEMID_CHILD pidl, const SHCOLUMNID *pscid, VARIANT *pv);
    IFACEMETHODIMP GetDetailsOf(PCUITEMID_CHILD pidl, UINT iColumn, SHELLDETAILS *psd);
    IFACEMETHODIMP MapColumnToSCID(UINT iColumn, SHCOLUMNID *pscid);

private:
    CReactOSShellFolder(FolderType folderType = FOLDER_TYPE_DESKTOP) : m_cRef(1), m_folderType(folderType) {}
    LONG m_cRef;
    FolderType m_folderType;
};
