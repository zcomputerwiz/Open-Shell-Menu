# Compatibility Matrix

## `shlwapi.dll`

| Function | Compatibility | Notes | Solution |
|---|---|---|---|
| `SHFillRectClr` | Likely Incompatible | This function is hooked using `SetIatHook`, which is a fragile technique that relies on undocumented implementation details of the Windows loader. It is unlikely to work in ReactOS or Wine. | Implement a custom drawing function that replicates the behavior of `SHFillRectClr` and use that instead of hooking. |

## `shell32.dll`

| Function | Compatibility | Notes | Solution |
|---|---|---|---|
| `SHCreateFilter` | Likely Incompatible | This function is loaded dynamically using `GetProcAddress` and is not a documented part of the Shell API. It is unlikely to be available in ReactOS or Wine. | Implement a custom filtering mechanism or find an alternative, documented API for filtering shell items. |
| Icons | Incompatible | The code uses hardcoded icon indexes to load icons from `shell32.dll`. These indexes are not guaranteed to be the same in ReactOS or Wine, and may change between Windows versions. | Replace the hardcoded icon indexes with calls to `SHGetStockIconInfo` or other documented APIs for retrieving shell icons. |

## `uxtheme.dll`

| Function | Compatibility | Notes | Solution |
|---|---|---|---|
| `DrawThemeBackground` | Likely Incompatible | Hooked using `SetIatHook`. See `SHFillRectClr` above. | Implement a custom theme-aware drawing function that replicates the behavior of `DrawThemeBackground`. |
| `DrawThemeText` | Likely Incompatible | Hooked using `SetIatHook`. See `SHFillRectClr` above. | Implement a custom theme-aware drawing function that replicates the behavior of `DrawThemeText`. |
| `DrawThemeTextEx` | Likely Incompatible | Hooked using `SetIatHook`. See `SHFillRectClr` above. | Implement a custom theme-aware drawing function that replicates the behavior of `DrawThemeTextEx`. |
