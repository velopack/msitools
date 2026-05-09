/*
 * stubs.c - Stub implementations for MSI API functions not supported by libmsi
 *
 * These functions require the Windows Installer Service, installation sessions,
 * or Windows-specific functionality that libmsi does not provide.
 * Each stub returns an appropriate error/default value.
 */

#include "msi_interop.h"
#include <stddef.h>

/* ========================================================================== */
/* UI / Logging                                                               */
/* ========================================================================== */

MSI_INTEROP_EXPORT INSTALLUILEVEL WINAPI MsiSetInternalUI(INSTALLUILEVEL dwUILevel, HWND *phWnd) {
    (void)dwUILevel; (void)phWnd;
    return INSTALLUILEVEL_NOCHANGE;
}

MSI_INTEROP_EXPORT INSTALLUI_HANDLERA WINAPI MsiSetExternalUIA(INSTALLUI_HANDLERA puiHandler, DWORD dwMessageFilter, LPVOID pvContext) {
    (void)puiHandler; (void)dwMessageFilter; (void)pvContext;
    return NULL;
}

MSI_INTEROP_EXPORT INSTALLUI_HANDLERW WINAPI MsiSetExternalUIW(INSTALLUI_HANDLERW puiHandler, DWORD dwMessageFilter, LPVOID pvContext) {
    (void)puiHandler; (void)dwMessageFilter; (void)pvContext;
    return NULL;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetExternalUIRecord(INSTALLUI_HANDLER_RECORD puiHandler, DWORD dwMessageFilter, LPVOID pvContext, PINSTALLUI_HANDLER_RECORD ppuiPrevHandler) {
    (void)puiHandler; (void)dwMessageFilter; (void)pvContext; (void)ppuiPrevHandler;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnableLogA(DWORD dwLogMode, LPCSTR szLogFile, DWORD dwLogAttributes) {
    (void)dwLogMode; (void)szLogFile; (void)dwLogAttributes;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnableLogW(DWORD dwLogMode, LPCWSTR szLogFile, DWORD dwLogAttributes) {
    (void)dwLogMode; (void)szLogFile; (void)dwLogAttributes;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Product queries                                                            */
/* ========================================================================== */

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryProductStateA(LPCSTR szProduct) {
    (void)szProduct;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryProductStateW(LPCWSTR szProduct) {
    (void)szProduct;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoA(LPCSTR szProduct, LPCSTR szAttribute, LPSTR lpValueBuf, LPDWORD pcchValueBuf) {
    (void)szProduct; (void)szAttribute; (void)lpValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoW(LPCWSTR szProduct, LPCWSTR szAttribute, LPWSTR lpValueBuf, LPDWORD pcchValueBuf) {
    (void)szProduct; (void)szAttribute; (void)lpValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoExA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szProperty, LPSTR szValue, LPDWORD pcchValue) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szProperty; (void)szValue; (void)pcchValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoExW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szProperty, LPWSTR szValue, LPDWORD pcchValue) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szProperty; (void)szValue; (void)pcchValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Installation                                                               */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiInstallProductA(LPCSTR szPackagePath, LPCSTR szCommandLine) {
    (void)szPackagePath; (void)szCommandLine;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiInstallProductW(LPCWSTR szPackagePath, LPCWSTR szCommandLine) {
    (void)szPackagePath; (void)szCommandLine;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductA(LPCSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState) {
    (void)szProduct; (void)iInstallLevel; (void)eInstallState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductW(LPCWSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState) {
    (void)szProduct; (void)iInstallLevel; (void)eInstallState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductExA(LPCSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState, LPCSTR szCommandLine) {
    (void)szProduct; (void)iInstallLevel; (void)eInstallState; (void)szCommandLine;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductExW(LPCWSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState, LPCWSTR szCommandLine) {
    (void)szProduct; (void)iInstallLevel; (void)eInstallState; (void)szCommandLine;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallProductA(LPCSTR szProduct, DWORD szReinstallMode) {
    (void)szProduct; (void)szReinstallMode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallProductW(LPCWSTR szProduct, DWORD szReinstallMode) {
    (void)szProduct; (void)szReinstallMode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Advertising                                                                */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductA(LPCSTR szPackagePath, LPCSTR szScriptfilePath, LPCSTR szTransforms, LANGID lgidLanguage) {
    (void)szPackagePath; (void)szScriptfilePath; (void)szTransforms; (void)lgidLanguage;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductW(LPCWSTR szPackagePath, LPCWSTR szScriptfilePath, LPCWSTR szTransforms, LANGID lgidLanguage) {
    (void)szPackagePath; (void)szScriptfilePath; (void)szTransforms; (void)lgidLanguage;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductExA(LPCSTR szPackagePath, LPCSTR szScriptfilePath, LPCSTR szTransforms, LANGID lgidLanguage, DWORD dwPlatform, DWORD dwOptions) {
    (void)szPackagePath; (void)szScriptfilePath; (void)szTransforms; (void)lgidLanguage; (void)dwPlatform; (void)dwOptions;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductExW(LPCWSTR szPackagePath, LPCWSTR szScriptfilePath, LPCWSTR szTransforms, LANGID lgidLanguage, DWORD dwPlatform, DWORD dwOptions) {
    (void)szPackagePath; (void)szScriptfilePath; (void)szTransforms; (void)lgidLanguage; (void)dwPlatform; (void)dwOptions;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProcessAdvertiseScriptA(LPCSTR szScriptFile, LPCSTR szIconFolder, HKEY hRegData, BOOL fShortcuts, BOOL fRemoveItems) {
    (void)szScriptFile; (void)szIconFolder; (void)hRegData; (void)fShortcuts; (void)fRemoveItems;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProcessAdvertiseScriptW(LPCWSTR szScriptFile, LPCWSTR szIconFolder, HKEY hRegData, BOOL fShortcuts, BOOL fRemoveItems) {
    (void)szScriptFile; (void)szIconFolder; (void)hRegData; (void)fShortcuts; (void)fRemoveItems;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseScriptA(LPCSTR szScriptFile, DWORD dwFlags, PHKEY phRegData, BOOL fRemoveItems) {
    (void)szScriptFile; (void)dwFlags; (void)phRegData; (void)fRemoveItems;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseScriptW(LPCWSTR szScriptFile, DWORD dwFlags, PHKEY phRegData, BOOL fRemoveItems) {
    (void)szScriptFile; (void)dwFlags; (void)phRegData; (void)fRemoveItems;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoFromScriptA(LPCSTR szScriptFile, LPSTR lpProductBuf39, LPWORD plgidLanguage, LPDWORD pdwVersion, LPSTR lpNameBuf, LPDWORD pcchNameBuf, LPSTR lpPackageBuf, LPDWORD pcchPackageBuf) {
    (void)szScriptFile; (void)lpProductBuf39; (void)plgidLanguage; (void)pdwVersion; (void)lpNameBuf; (void)pcchNameBuf; (void)lpPackageBuf; (void)pcchPackageBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoFromScriptW(LPCWSTR szScriptFile, LPWSTR lpProductBuf39, LPWORD plgidLanguage, LPDWORD pdwVersion, LPWSTR lpNameBuf, LPDWORD pcchNameBuf, LPWSTR lpPackageBuf, LPDWORD pcchPackageBuf) {
    (void)szScriptFile; (void)lpProductBuf39; (void)plgidLanguage; (void)pdwVersion; (void)lpNameBuf; (void)pcchNameBuf; (void)lpPackageBuf; (void)pcchPackageBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Product/component queries                                                  */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductCodeA(LPCSTR szComponent, LPSTR lpBuf39) {
    (void)szComponent; (void)lpBuf39;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductCodeW(LPCWSTR szComponent, LPWSTR lpBuf39) {
    (void)szComponent; (void)lpBuf39;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT USERINFOSTATE WINAPI MsiGetUserInfoA(LPCSTR szProduct, LPSTR lpUserNameBuf, LPDWORD pcchUserNameBuf, LPSTR lpOrgNameBuf, LPDWORD pcchOrgNameBuf, LPSTR lpSerialBuf, LPDWORD pcchSerialBuf) {
    (void)szProduct; (void)lpUserNameBuf; (void)pcchUserNameBuf; (void)lpOrgNameBuf; (void)pcchOrgNameBuf; (void)lpSerialBuf; (void)pcchSerialBuf;
    return USERINFOSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT USERINFOSTATE WINAPI MsiGetUserInfoW(LPCWSTR szProduct, LPWSTR lpUserNameBuf, LPDWORD pcchUserNameBuf, LPWSTR lpOrgNameBuf, LPDWORD pcchOrgNameBuf, LPWSTR lpSerialBuf, LPDWORD pcchSerialBuf) {
    (void)szProduct; (void)lpUserNameBuf; (void)pcchUserNameBuf; (void)lpOrgNameBuf; (void)pcchOrgNameBuf; (void)lpSerialBuf; (void)pcchSerialBuf;
    return USERINFOSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiCollectUserInfoA(LPCSTR szProduct) {
    (void)szProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiCollectUserInfoW(LPCWSTR szProduct) {
    (void)szProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Patches                                                                    */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiApplyPatchA(LPCSTR szPatchPackage, LPCSTR szInstallPackage, INSTALLTYPE eInstallType, LPCSTR szCommandLine) {
    (void)szPatchPackage; (void)szInstallPackage; (void)eInstallType; (void)szCommandLine;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiApplyPatchW(LPCWSTR szPatchPackage, LPCWSTR szInstallPackage, INSTALLTYPE eInstallType, LPCWSTR szCommandLine) {
    (void)szPatchPackage; (void)szInstallPackage; (void)eInstallType; (void)szCommandLine;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoA(LPCSTR szPatch, LPCSTR szAttribute, LPSTR lpValueBuf, LPDWORD pcchValueBuf) {
    (void)szPatch; (void)szAttribute; (void)lpValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoW(LPCWSTR szPatch, LPCWSTR szAttribute, LPWSTR lpValueBuf, LPDWORD pcchValueBuf) {
    (void)szPatch; (void)szAttribute; (void)lpValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesA(LPCSTR szProduct, DWORD iPatchIndex, LPSTR lpPatchBuf, LPSTR lpTransformsBuf, LPDWORD pcchTransformsBuf) {
    (void)szProduct; (void)iPatchIndex; (void)lpPatchBuf; (void)lpTransformsBuf; (void)pcchTransformsBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesW(LPCWSTR szProduct, DWORD iPatchIndex, LPWSTR lpPatchBuf, LPWSTR lpTransformsBuf, LPDWORD pcchTransformsBuf) {
    (void)szProduct; (void)iPatchIndex; (void)lpPatchBuf; (void)lpTransformsBuf; (void)pcchTransformsBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiRemovePatchesA(LPCSTR szPatchList, LPCSTR szProductCode, INSTALLTYPE eUninstallType, LPCSTR szPropertyList) {
    (void)szPatchList; (void)szProductCode; (void)eUninstallType; (void)szPropertyList;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiRemovePatchesW(LPCWSTR szPatchList, LPCWSTR szProductCode, INSTALLTYPE eUninstallType, LPCWSTR szPropertyList) {
    (void)szPatchList; (void)szProductCode; (void)eUninstallType; (void)szPropertyList;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiExtractPatchXMLDataA(LPCSTR szPatchPath, DWORD dwReserved, LPSTR szXMLData, LPDWORD pcchXMLData) {
    (void)szPatchPath; (void)dwReserved; (void)szXMLData; (void)pcchXMLData;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiExtractPatchXMLDataW(LPCWSTR szPatchPath, DWORD dwReserved, LPWSTR szXMLData, LPDWORD pcchXMLData) {
    (void)szPatchPath; (void)dwReserved; (void)szXMLData; (void)pcchXMLData;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoExA(LPCSTR szPatchCode, LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szProperty, LPSTR lpValue, LPDWORD pcchValue) {
    (void)szPatchCode; (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szProperty; (void)lpValue; (void)pcchValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoExW(LPCWSTR szPatchCode, LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szProperty, LPWSTR lpValue, LPDWORD pcchValue) {
    (void)szPatchCode; (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szProperty; (void)lpValue; (void)pcchValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiApplyMultiplePatchesA(LPCSTR szPatchPackages, LPCSTR szProductCode, LPCSTR szPropertiesList) {
    (void)szPatchPackages; (void)szProductCode; (void)szPropertiesList;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiApplyMultiplePatchesW(LPCWSTR szPatchPackages, LPCWSTR szProductCode, LPCWSTR szPropertiesList) {
    (void)szPatchPackages; (void)szProductCode; (void)szPropertiesList;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiDeterminePatchSequenceA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOA pPatchInfo) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)cPatchInfo; (void)pPatchInfo;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiDeterminePatchSequenceW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOW pPatchInfo) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)cPatchInfo; (void)pPatchInfo;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiDetermineApplicablePatchesA(LPCSTR szProductPackagePath, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOA pPatchInfo) {
    (void)szProductPackagePath; (void)cPatchInfo; (void)pPatchInfo;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiDetermineApplicablePatchesW(LPCWSTR szProductPackagePath, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOW pPatchInfo) {
    (void)szProductPackagePath; (void)cPatchInfo; (void)pPatchInfo;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesExA(LPCSTR szProductCode, LPCSTR szUserSid, DWORD dwContext, DWORD dwFilter, DWORD dwIndex, LPSTR szPatchCode, LPSTR szTargetProductCode, MSIINSTALLCONTEXT *pdwTargetProductContext, LPSTR szTargetUserSid, LPDWORD pcchTargetUserSid) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)dwFilter; (void)dwIndex; (void)szPatchCode; (void)szTargetProductCode; (void)pdwTargetProductContext; (void)szTargetUserSid; (void)pcchTargetUserSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesExW(LPCWSTR szProductCode, LPCWSTR szUserSid, DWORD dwContext, DWORD dwFilter, DWORD dwIndex, LPWSTR szPatchCode, LPWSTR szTargetProductCode, MSIINSTALLCONTEXT *pdwTargetProductContext, LPWSTR szTargetUserSid, LPDWORD pcchTargetUserSid) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)dwFilter; (void)dwIndex; (void)szPatchCode; (void)szTargetProductCode; (void)pdwTargetProductContext; (void)szTargetUserSid; (void)pcchTargetUserSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Features                                                                   */
/* ========================================================================== */

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryFeatureStateA(LPCSTR szProduct, LPCSTR szFeature) {
    (void)szProduct; (void)szFeature;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryFeatureStateW(LPCWSTR szProduct, LPCWSTR szFeature) {
    (void)szProduct; (void)szFeature;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiQueryFeatureStateExA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szFeature, INSTALLSTATE *pdwState) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szFeature; (void)pdwState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiQueryFeatureStateExW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szFeature, INSTALLSTATE *pdwState) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szFeature; (void)pdwState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureA(LPCSTR szProduct, LPCSTR szFeature) {
    (void)szProduct; (void)szFeature;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureW(LPCWSTR szProduct, LPCWSTR szFeature) {
    (void)szProduct; (void)szFeature;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureExA(LPCSTR szProduct, LPCSTR szFeature, DWORD dwInstallMode, DWORD dwReserved) {
    (void)szProduct; (void)szFeature; (void)dwInstallMode; (void)dwReserved;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureExW(LPCWSTR szProduct, LPCWSTR szFeature, DWORD dwInstallMode, DWORD dwReserved) {
    (void)szProduct; (void)szFeature; (void)dwInstallMode; (void)dwReserved;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureUsageA(LPCSTR szProduct, LPCSTR szFeature, LPDWORD pdwUseCount, LPWORD pwDateUsed) {
    (void)szProduct; (void)szFeature; (void)pdwUseCount; (void)pwDateUsed;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureUsageW(LPCWSTR szProduct, LPCWSTR szFeature, LPDWORD pdwUseCount, LPWORD pwDateUsed) {
    (void)szProduct; (void)szFeature; (void)pdwUseCount; (void)pwDateUsed;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureFeatureA(LPCSTR szProduct, LPCSTR szFeature, INSTALLSTATE eInstallState) {
    (void)szProduct; (void)szFeature; (void)eInstallState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureFeatureW(LPCWSTR szProduct, LPCWSTR szFeature, INSTALLSTATE eInstallState) {
    (void)szProduct; (void)szFeature; (void)eInstallState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallFeatureA(LPCSTR szProduct, LPCSTR szFeature, DWORD dwReinstallMode) {
    (void)szProduct; (void)szFeature; (void)dwReinstallMode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallFeatureW(LPCWSTR szProduct, LPCWSTR szFeature, DWORD dwReinstallMode) {
    (void)szProduct; (void)szFeature; (void)dwReinstallMode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Components                                                                 */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideComponentA(LPCSTR szProduct, LPCSTR szFeature, LPCSTR szComponent, DWORD dwInstallMode, LPSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szProduct; (void)szFeature; (void)szComponent; (void)dwInstallMode; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideComponentW(LPCWSTR szProduct, LPCWSTR szFeature, LPCWSTR szComponent, DWORD dwInstallMode, LPWSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szProduct; (void)szFeature; (void)szComponent; (void)dwInstallMode; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentA(LPCSTR szCategory, LPCSTR szQualifier, DWORD dwInstallMode, LPSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szCategory; (void)szQualifier; (void)dwInstallMode; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentW(LPCWSTR szCategory, LPCWSTR szQualifier, DWORD dwInstallMode, LPWSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szCategory; (void)szQualifier; (void)dwInstallMode; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentExA(LPCSTR szCategory, LPCSTR szQualifier, DWORD dwInstallMode, LPCSTR szProduct, DWORD dwUnused1, DWORD dwUnused2, LPSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szCategory; (void)szQualifier; (void)dwInstallMode; (void)szProduct; (void)dwUnused1; (void)dwUnused2; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentExW(LPCWSTR szCategory, LPCWSTR szQualifier, DWORD dwInstallMode, LPCWSTR szProduct, DWORD dwUnused1, DWORD dwUnused2, LPWSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szCategory; (void)szQualifier; (void)dwInstallMode; (void)szProduct; (void)dwUnused1; (void)dwUnused2; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathA(LPCSTR szProduct, LPCSTR szComponent, LPSTR lpPathBuf, LPDWORD pcchBuf) {
    (void)szProduct; (void)szComponent; (void)lpPathBuf; (void)pcchBuf;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathW(LPCWSTR szProduct, LPCWSTR szComponent, LPWSTR lpPathBuf, LPDWORD pcchBuf) {
    (void)szProduct; (void)szComponent; (void)lpPathBuf; (void)pcchBuf;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathExA(LPCSTR szProductCode, LPCSTR szComponentCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPSTR lpOutPathBuf, LPDWORD pcchOutPathBuf) {
    (void)szProductCode; (void)szComponentCode; (void)szUserSid; (void)dwContext; (void)lpOutPathBuf; (void)pcchOutPathBuf;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathExW(LPCWSTR szProductCode, LPCWSTR szComponentCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPWSTR lpOutPathBuf, LPDWORD pcchOutPathBuf) {
    (void)szProductCode; (void)szComponentCode; (void)szUserSid; (void)dwContext; (void)lpOutPathBuf; (void)pcchOutPathBuf;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideAssemblyA(LPCSTR szAssemblyName, LPCSTR szAppContext, DWORD dwInstallMode, DWORD dwAssemblyInfo, LPSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szAssemblyName; (void)szAppContext; (void)dwInstallMode; (void)dwAssemblyInfo; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiProvideAssemblyW(LPCWSTR szAssemblyName, LPCWSTR szAppContext, DWORD dwInstallMode, DWORD dwAssemblyInfo, LPWSTR lpPathBuf, LPDWORD pcchPathBuf) {
    (void)szAssemblyName; (void)szAppContext; (void)dwInstallMode; (void)dwAssemblyInfo; (void)lpPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiQueryComponentStateA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szComponentCode, INSTALLSTATE *pdwState) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szComponentCode; (void)pdwState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiQueryComponentStateW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szComponentCode, INSTALLSTATE *pdwState) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)szComponentCode; (void)pdwState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Enumeration                                                                */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsA(DWORD iProductIndex, LPSTR lpProductBuf) {
    (void)iProductIndex; (void)lpProductBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsW(DWORD iProductIndex, LPWSTR lpProductBuf) {
    (void)iProductIndex; (void)lpProductBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsExA(LPCSTR szProductCode, LPCSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPSTR szInstalledProductCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPSTR szSid, LPDWORD pcchSid) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)dwIndex; (void)szInstalledProductCode; (void)pdwInstalledContext; (void)szSid; (void)pcchSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsExW(LPCWSTR szProductCode, LPCWSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPWSTR szInstalledProductCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPWSTR szSid, LPDWORD pcchSid) {
    (void)szProductCode; (void)szUserSid; (void)dwContext; (void)dwIndex; (void)szInstalledProductCode; (void)pdwInstalledContext; (void)szSid; (void)pcchSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumRelatedProductsA(LPCSTR lpUpgradeCode, DWORD dwReserved, DWORD iProductIndex, LPSTR lpProductBuf) {
    (void)lpUpgradeCode; (void)dwReserved; (void)iProductIndex; (void)lpProductBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumRelatedProductsW(LPCWSTR lpUpgradeCode, DWORD dwReserved, DWORD iProductIndex, LPWSTR lpProductBuf) {
    (void)lpUpgradeCode; (void)dwReserved; (void)iProductIndex; (void)lpProductBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumFeaturesA(LPCSTR szProduct, DWORD iFeatureIndex, LPSTR lpFeatureBuf, LPSTR lpParentBuf) {
    (void)szProduct; (void)iFeatureIndex; (void)lpFeatureBuf; (void)lpParentBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumFeaturesW(LPCWSTR szProduct, DWORD iFeatureIndex, LPWSTR lpFeatureBuf, LPWSTR lpParentBuf) {
    (void)szProduct; (void)iFeatureIndex; (void)lpFeatureBuf; (void)lpParentBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsA(DWORD iComponentIndex, LPSTR lpComponentBuf) {
    (void)iComponentIndex; (void)lpComponentBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsW(DWORD iComponentIndex, LPWSTR lpComponentBuf) {
    (void)iComponentIndex; (void)lpComponentBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsExA(LPCSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPSTR szInstalledComponentCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPSTR szSid, LPDWORD pcchSid) {
    (void)szUserSid; (void)dwContext; (void)dwIndex; (void)szInstalledComponentCode; (void)pdwInstalledContext; (void)szSid; (void)pcchSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsExW(LPCWSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPWSTR szInstalledComponentCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPWSTR szSid, LPDWORD pcchSid) {
    (void)szUserSid; (void)dwContext; (void)dwIndex; (void)szInstalledComponentCode; (void)pdwInstalledContext; (void)szSid; (void)pcchSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsA(LPCSTR szComponent, DWORD iProductIndex, LPSTR lpProductBuf) {
    (void)szComponent; (void)iProductIndex; (void)lpProductBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsW(LPCWSTR szComponent, DWORD iProductIndex, LPWSTR lpProductBuf) {
    (void)szComponent; (void)iProductIndex; (void)lpProductBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsExA(LPCSTR szComponent, LPCSTR szUserSid, DWORD dwContext, DWORD dwProductIndex, LPSTR szProductBuf, MSIINSTALLCONTEXT *pdwInstalledContext, LPSTR szSid, LPDWORD pcchSid) {
    (void)szComponent; (void)szUserSid; (void)dwContext; (void)dwProductIndex; (void)szProductBuf; (void)pdwInstalledContext; (void)szSid; (void)pcchSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsExW(LPCWSTR szComponent, LPCWSTR szUserSid, DWORD dwContext, DWORD dwProductIndex, LPWSTR szProductBuf, MSIINSTALLCONTEXT *pdwInstalledContext, LPWSTR szSid, LPDWORD pcchSid) {
    (void)szComponent; (void)szUserSid; (void)dwContext; (void)dwProductIndex; (void)szProductBuf; (void)pdwInstalledContext; (void)szSid; (void)pcchSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentQualifiersA(LPCSTR szComponent, DWORD iIndex, LPSTR lpQualifierBuf, LPDWORD pcchQualifierBuf, LPSTR lpApplicationDataBuf, LPDWORD pcchApplicationDataBuf) {
    (void)szComponent; (void)iIndex; (void)lpQualifierBuf; (void)pcchQualifierBuf; (void)lpApplicationDataBuf; (void)pcchApplicationDataBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentQualifiersW(LPCWSTR szComponent, DWORD iIndex, LPWSTR lpQualifierBuf, LPDWORD pcchQualifierBuf, LPWSTR lpApplicationDataBuf, LPDWORD pcchApplicationDataBuf) {
    (void)szComponent; (void)iIndex; (void)lpQualifierBuf; (void)pcchQualifierBuf; (void)lpApplicationDataBuf; (void)pcchApplicationDataBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Package / Product                                                          */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiOpenProductA(LPCSTR szProduct, MSIHANDLE *phProduct) {
    (void)szProduct; (void)phProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiOpenProductW(LPCWSTR szProduct, MSIHANDLE *phProduct) {
    (void)szProduct; (void)phProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageA(LPCSTR szPackagePath, MSIHANDLE *hProduct) {
    (void)szPackagePath; (void)hProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageW(LPCWSTR szPackagePath, MSIHANDLE *hProduct) {
    (void)szPackagePath; (void)hProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageExA(LPCSTR szPackagePath, DWORD dwOptions, MSIHANDLE *hProduct) {
    (void)szPackagePath; (void)dwOptions; (void)hProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageExW(LPCWSTR szPackagePath, DWORD dwOptions, MSIHANDLE *hProduct) {
    (void)szPackagePath; (void)dwOptions; (void)hProduct;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchFileListA(LPCSTR szProductCode, LPCSTR szPatchPackages, LPDWORD pcFiles, MSIHANDLE **pphFileRecords) {
    (void)szProductCode; (void)szPatchPackages; (void)pcFiles; (void)pphFileRecords;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchFileListW(LPCWSTR szProductCode, LPCWSTR szPatchPackages, LPDWORD pcFiles, MSIHANDLE **pphFileRecords) {
    (void)szProductCode; (void)szPatchPackages; (void)pcFiles; (void)pphFileRecords;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductPropertyA(MSIHANDLE hProduct, LPCSTR szProperty, LPSTR lpValueBuf, LPDWORD pcchValueBuf) {
    (void)hProduct; (void)szProperty; (void)lpValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductPropertyW(MSIHANDLE hProduct, LPCWSTR szProperty, LPWSTR lpValueBuf, LPDWORD pcchValueBuf) {
    (void)hProduct; (void)szProperty; (void)lpValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureInfoA(MSIHANDLE hProduct, LPCSTR szFeature, LPDWORD lpAttributes, LPSTR lpTitleBuf, LPDWORD pcchTitleBuf, LPSTR lpHelpBuf, LPDWORD pcchHelpBuf) {
    (void)hProduct; (void)szFeature; (void)lpAttributes; (void)lpTitleBuf; (void)pcchTitleBuf; (void)lpHelpBuf; (void)pcchHelpBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureInfoW(MSIHANDLE hProduct, LPCWSTR szFeature, LPDWORD lpAttributes, LPWSTR lpTitleBuf, LPDWORD pcchTitleBuf, LPWSTR lpHelpBuf, LPDWORD pcchHelpBuf) {
    (void)hProduct; (void)szFeature; (void)lpAttributes; (void)lpTitleBuf; (void)pcchTitleBuf; (void)lpHelpBuf; (void)pcchHelpBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Missing components                                                         */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingComponentA(LPCSTR szProduct, LPCSTR szComponent, INSTALLSTATE eInstallState) {
    (void)szProduct; (void)szComponent; (void)eInstallState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingComponentW(LPCWSTR szProduct, LPCWSTR szComponent, INSTALLSTATE eInstallState) {
    (void)szProduct; (void)szComponent; (void)eInstallState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingFileA(LPCSTR szProduct, LPCSTR szFile) {
    (void)szProduct; (void)szFile;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingFileW(LPCWSTR szProduct, LPCWSTR szFile) {
    (void)szProduct; (void)szFile;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiLocateComponentA(LPCSTR szComponent, LPSTR lpPathBuf, LPDWORD pcchBuf) {
    (void)szComponent; (void)lpPathBuf; (void)pcchBuf;
    return INSTALLSTATE_UNKNOWN;
}

MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiLocateComponentW(LPCWSTR szComponent, LPWSTR lpPathBuf, LPDWORD pcchBuf) {
    (void)szComponent; (void)lpPathBuf; (void)pcchBuf;
    return INSTALLSTATE_UNKNOWN;
}

/* ========================================================================== */
/* Source list                                                                 */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllA(LPCSTR szProduct, LPCSTR szUserName, DWORD dwReserved) {
    (void)szProduct; (void)szUserName; (void)dwReserved;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllW(LPCWSTR szProduct, LPCWSTR szUserName, DWORD dwReserved) {
    (void)szProduct; (void)szUserName; (void)dwReserved;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceA(LPCSTR szProduct, LPCSTR szUserName, DWORD dwReserved, LPCSTR szSource) {
    (void)szProduct; (void)szUserName; (void)dwReserved; (void)szSource;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceW(LPCWSTR szProduct, LPCWSTR szUserName, DWORD dwReserved, LPCWSTR szSource) {
    (void)szProduct; (void)szUserName; (void)dwReserved; (void)szSource;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionA(LPCSTR szProduct, LPCSTR szUserName, DWORD dwReserved) {
    (void)szProduct; (void)szUserName; (void)dwReserved;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionW(LPCWSTR szProduct, LPCWSTR szUserName, DWORD dwReserved) {
    (void)szProduct; (void)szUserName; (void)dwReserved;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceExA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szSource, DWORD dwIndex) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szSource; (void)dwIndex;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceExW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szSource, DWORD dwIndex) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szSource; (void)dwIndex;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddMediaDiskA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId, LPCSTR szVolumeLabel, LPCSTR szDiskPrompt) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwDiskId; (void)szVolumeLabel; (void)szDiskPrompt;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddMediaDiskW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId, LPCWSTR szVolumeLabel, LPCWSTR szDiskPrompt) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwDiskId; (void)szVolumeLabel; (void)szDiskPrompt;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearSourceA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szSource) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szSource;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearSourceW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szSource) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szSource;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearMediaDiskA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwDiskId;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearMediaDiskW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwDiskId;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllExA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllExW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionExA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionExW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListSetInfoA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szProperty, LPCSTR szValue) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szProperty; (void)szValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListSetInfoW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szProperty, LPCWSTR szValue) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szProperty; (void)szValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListGetInfoA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szProperty, LPSTR szValue, LPDWORD pcchValue) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szProperty; (void)szValue; (void)pcchValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListGetInfoW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szProperty, LPWSTR szValue, LPDWORD pcchValue) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)szProperty; (void)szValue; (void)pcchValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumSourcesA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPSTR szSource, LPDWORD pcchSource) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwIndex; (void)szSource; (void)pcchSource;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumSourcesW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPWSTR szSource, LPDWORD pcchSource) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwIndex; (void)szSource; (void)pcchSource;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumMediaDisksA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPDWORD pdwDiskId, LPSTR szVolumeLabel, LPDWORD pcchVolumeLabel, LPSTR szDiskPrompt, LPDWORD pcchDiskPrompt) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwIndex; (void)pdwDiskId; (void)szVolumeLabel; (void)pcchVolumeLabel; (void)szDiskPrompt; (void)pcchDiskPrompt;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumMediaDisksW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPDWORD pdwDiskId, LPWSTR szVolumeLabel, LPDWORD pcchVolumeLabel, LPWSTR szDiskPrompt, LPDWORD pcchDiskPrompt) {
    (void)szProductCodeOrPatchCode; (void)szUserSid; (void)dwContext; (void)dwOptions; (void)dwIndex; (void)pdwDiskId; (void)szVolumeLabel; (void)pcchVolumeLabel; (void)szDiskPrompt; (void)pcchDiskPrompt;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Utilities                                                                  */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileVersionA(LPCSTR szFilePath, LPSTR lpVersionBuf, LPDWORD pcchVersionBuf, LPSTR lpLangBuf, LPDWORD pcchLangBuf) {
    (void)szFilePath; (void)lpVersionBuf; (void)pcchVersionBuf; (void)lpLangBuf; (void)pcchLangBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileVersionW(LPCWSTR szFilePath, LPWSTR lpVersionBuf, LPDWORD pcchVersionBuf, LPWSTR lpLangBuf, LPDWORD pcchLangBuf) {
    (void)szFilePath; (void)lpVersionBuf; (void)pcchVersionBuf; (void)lpLangBuf; (void)pcchLangBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileHashA(LPCSTR szFilePath, DWORD dwOptions, PMSIFILEHASHINFO pHash) {
    (void)szFilePath; (void)dwOptions; (void)pHash;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileHashW(LPCWSTR szFilePath, DWORD dwOptions, PMSIFILEHASHINFO pHash) {
    (void)szFilePath; (void)dwOptions; (void)pHash;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT HRESULT WINAPI MsiGetFileSignatureInformationA(LPCSTR szSignedObjectPath, DWORD dwFlags, LPVOID *ppCertContext, LPBYTE pbHashData, LPDWORD pcbHashData) {
    (void)szSignedObjectPath; (void)dwFlags; (void)ppCertContext; (void)pbHashData; (void)pcbHashData;
    return (HRESULT)0x80004001L; /* E_NOTIMPL */
}

MSI_INTEROP_EXPORT HRESULT WINAPI MsiGetFileSignatureInformationW(LPCWSTR szSignedObjectPath, DWORD dwFlags, LPVOID *ppCertContext, LPBYTE pbHashData, LPDWORD pcbHashData) {
    (void)szSignedObjectPath; (void)dwFlags; (void)ppCertContext; (void)pbHashData; (void)pcbHashData;
    return (HRESULT)0x80004001L; /* E_NOTIMPL */
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetShortcutTargetA(LPCSTR szShortcutPath, LPSTR szProductCode, LPSTR szFeatureId, LPSTR szComponentCode) {
    (void)szShortcutPath; (void)szProductCode; (void)szFeatureId; (void)szComponentCode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetShortcutTargetW(LPCWSTR szShortcutPath, LPWSTR szProductCode, LPWSTR szFeatureId, LPWSTR szComponentCode) {
    (void)szShortcutPath; (void)szProductCode; (void)szFeatureId; (void)szComponentCode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiIsProductElevatedA(LPCSTR szProduct, BOOL *pfElevated) {
    (void)szProduct; (void)pfElevated;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiIsProductElevatedW(LPCWSTR szProduct, BOOL *pfElevated) {
    (void)szProduct; (void)pfElevated;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiNotifySidChangeA(LPCSTR pOldSid, LPCSTR pNewSid) {
    (void)pOldSid; (void)pNewSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiNotifySidChangeW(LPCWSTR pOldSid, LPCWSTR pNewSid) {
    (void)pOldSid; (void)pNewSid;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* ========================================================================== */
/* Transaction management                                                     */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiBeginTransactionA(LPCSTR szName, DWORD dwTransactionAttributes, MSIHANDLE *phTransactionHandle, HANDLE *phChangeOfOwnerEvent) {
    (void)szName; (void)dwTransactionAttributes; (void)phTransactionHandle; (void)phChangeOfOwnerEvent;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiBeginTransactionW(LPCWSTR szName, DWORD dwTransactionAttributes, MSIHANDLE *phTransactionHandle, HANDLE *phChangeOfOwnerEvent) {
    (void)szName; (void)dwTransactionAttributes; (void)phTransactionHandle; (void)phChangeOfOwnerEvent;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEndTransaction(DWORD dwTransactionState) {
    (void)dwTransactionState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiJoinTransaction(MSIHANDLE hTransactionHandle, DWORD dwTransactionAttributes, HANDLE *phChangeOfOwnerEvent) {
    (void)hTransactionHandle; (void)dwTransactionAttributes; (void)phChangeOfOwnerEvent;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/* Transform generation stubs are in database.c */

/* ========================================================================== */
/* Install session functions (MsiQuery.h)                                     */
/* ========================================================================== */

MSI_INTEROP_EXPORT MSIHANDLE WINAPI MsiGetActiveDatabase(MSIHANDLE hInstall) {
    (void)hInstall;
    return 0;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetPropertyA(MSIHANDLE hInstall, LPCSTR szName, LPCSTR szValue) {
    (void)hInstall; (void)szName; (void)szValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetPropertyW(MSIHANDLE hInstall, LPCWSTR szName, LPCWSTR szValue) {
    (void)hInstall; (void)szName; (void)szValue;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPropertyA(MSIHANDLE hInstall, LPCSTR szName, LPSTR szValueBuf, LPDWORD pcchValueBuf) {
    (void)hInstall; (void)szName; (void)szValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetPropertyW(MSIHANDLE hInstall, LPCWSTR szName, LPWSTR szValueBuf, LPDWORD pcchValueBuf) {
    (void)hInstall; (void)szName; (void)szValueBuf; (void)pcchValueBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT LANGID WINAPI MsiGetLanguage(MSIHANDLE hInstall) {
    (void)hInstall;
    return 0;
}

MSI_INTEROP_EXPORT BOOL WINAPI MsiGetMode(MSIHANDLE hInstall, MSIRUNMODE eRunMode) {
    (void)hInstall; (void)eRunMode;
    return FALSE;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetMode(MSIHANDLE hInstall, MSIRUNMODE eRunMode, BOOL fState) {
    (void)hInstall; (void)eRunMode; (void)fState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiDoActionA(MSIHANDLE hInstall, LPCSTR szAction) {
    (void)hInstall; (void)szAction;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiDoActionW(MSIHANDLE hInstall, LPCWSTR szAction) {
    (void)hInstall; (void)szAction;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSequenceA(MSIHANDLE hInstall, LPCSTR szTable, int iSequenceMode) {
    (void)hInstall; (void)szTable; (void)iSequenceMode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSequenceW(MSIHANDLE hInstall, LPCWSTR szTable, int iSequenceMode) {
    (void)hInstall; (void)szTable; (void)iSequenceMode;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT int WINAPI MsiProcessMessage(MSIHANDLE hInstall, INSTALLMESSAGE eMessageType, MSIHANDLE hRecord) {
    (void)hInstall; (void)eMessageType; (void)hRecord;
    return -1;
}

MSI_INTEROP_EXPORT MSICONDITION WINAPI MsiEvaluateConditionA(MSIHANDLE hInstall, LPCSTR szCondition) {
    (void)hInstall; (void)szCondition;
    return MSICONDITION_ERROR;
}

MSI_INTEROP_EXPORT MSICONDITION WINAPI MsiEvaluateConditionW(MSIHANDLE hInstall, LPCWSTR szCondition) {
    (void)hInstall; (void)szCondition;
    return MSICONDITION_ERROR;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureStateA(MSIHANDLE hInstall, LPCSTR szFeature, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction) {
    (void)hInstall; (void)szFeature; (void)piInstalled; (void)piAction;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureStateW(MSIHANDLE hInstall, LPCWSTR szFeature, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction) {
    (void)hInstall; (void)szFeature; (void)piInstalled; (void)piAction;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureStateA(MSIHANDLE hInstall, LPCSTR szFeature, INSTALLSTATE iState) {
    (void)hInstall; (void)szFeature; (void)iState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureStateW(MSIHANDLE hInstall, LPCWSTR szFeature, INSTALLSTATE iState) {
    (void)hInstall; (void)szFeature; (void)iState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureAttributesA(MSIHANDLE hInstall, LPCSTR szFeature, DWORD dwAttributes) {
    (void)hInstall; (void)szFeature; (void)dwAttributes;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureAttributesW(MSIHANDLE hInstall, LPCWSTR szFeature, DWORD dwAttributes) {
    (void)hInstall; (void)szFeature; (void)dwAttributes;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetComponentStateA(MSIHANDLE hInstall, LPCSTR szComponent, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction) {
    (void)hInstall; (void)szComponent; (void)piInstalled; (void)piAction;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetComponentStateW(MSIHANDLE hInstall, LPCWSTR szComponent, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction) {
    (void)hInstall; (void)szComponent; (void)piInstalled; (void)piAction;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetComponentStateA(MSIHANDLE hInstall, LPCSTR szComponent, INSTALLSTATE iState) {
    (void)hInstall; (void)szComponent; (void)iState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetComponentStateW(MSIHANDLE hInstall, LPCWSTR szComponent, INSTALLSTATE iState) {
    (void)hInstall; (void)szComponent; (void)iState;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureCostA(MSIHANDLE hInstall, LPCSTR szFeature, MSICOSTTREE iCostTree, INSTALLSTATE iState, LPINT piCost) {
    (void)hInstall; (void)szFeature; (void)iCostTree; (void)iState; (void)piCost;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureCostW(MSIHANDLE hInstall, LPCWSTR szFeature, MSICOSTTREE iCostTree, INSTALLSTATE iState, LPINT piCost) {
    (void)hInstall; (void)szFeature; (void)iCostTree; (void)iState; (void)piCost;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentCostsA(MSIHANDLE hInstall, LPCSTR szComponent, DWORD dwIndex, INSTALLSTATE iState, LPSTR szDriveBuf, LPDWORD pcchDriveBuf, LPINT piCost, LPINT piTempCost) {
    (void)hInstall; (void)szComponent; (void)dwIndex; (void)iState; (void)szDriveBuf; (void)pcchDriveBuf; (void)piCost; (void)piTempCost;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentCostsW(MSIHANDLE hInstall, LPCWSTR szComponent, DWORD dwIndex, INSTALLSTATE iState, LPWSTR szDriveBuf, LPDWORD pcchDriveBuf, LPINT piCost, LPINT piTempCost) {
    (void)hInstall; (void)szComponent; (void)dwIndex; (void)iState; (void)szDriveBuf; (void)pcchDriveBuf; (void)piCost; (void)piTempCost;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetInstallLevel(MSIHANDLE hInstall, int iInstallLevel) {
    (void)hInstall; (void)iInstallLevel;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureValidStatesA(MSIHANDLE hInstall, LPCSTR szFeature, LPDWORD lpInstallStates) {
    (void)hInstall; (void)szFeature; (void)lpInstallStates;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureValidStatesW(MSIHANDLE hInstall, LPCWSTR szFeature, LPDWORD lpInstallStates) {
    (void)hInstall; (void)szFeature; (void)lpInstallStates;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetSourcePathA(MSIHANDLE hInstall, LPCSTR szFolder, LPSTR szPathBuf, LPDWORD pcchPathBuf) {
    (void)hInstall; (void)szFolder; (void)szPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetSourcePathW(MSIHANDLE hInstall, LPCWSTR szFolder, LPWSTR szPathBuf, LPDWORD pcchPathBuf) {
    (void)hInstall; (void)szFolder; (void)szPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetTargetPathA(MSIHANDLE hInstall, LPCSTR szFolder, LPSTR szPathBuf, LPDWORD pcchPathBuf) {
    (void)hInstall; (void)szFolder; (void)szPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetTargetPathW(MSIHANDLE hInstall, LPCWSTR szFolder, LPWSTR szPathBuf, LPDWORD pcchPathBuf) {
    (void)hInstall; (void)szFolder; (void)szPathBuf; (void)pcchPathBuf;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetTargetPathA(MSIHANDLE hInstall, LPCSTR szFolder, LPCSTR szFolderPath) {
    (void)hInstall; (void)szFolder; (void)szFolderPath;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiSetTargetPathW(MSIHANDLE hInstall, LPCWSTR szFolder, LPCWSTR szFolderPath) {
    (void)hInstall; (void)szFolder; (void)szFolderPath;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiVerifyDiskSpace(MSIHANDLE hInstall) {
    (void)hInstall;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiEnableUIPreview(MSIHANDLE hDatabase, MSIHANDLE *phPreview) {
    (void)hDatabase; (void)phPreview;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewDialogA(MSIHANDLE hPreview, LPCSTR szDialogName) {
    (void)hPreview; (void)szDialogName;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewDialogW(MSIHANDLE hPreview, LPCWSTR szDialogName) {
    (void)hPreview; (void)szDialogName;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewBillboardA(MSIHANDLE hPreview, LPCSTR szControlName, LPCSTR szBillboard) {
    (void)hPreview; (void)szControlName; (void)szBillboard;
    return ERROR_CALL_NOT_IMPLEMENTED;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewBillboardW(MSIHANDLE hPreview, LPCWSTR szControlName, LPCWSTR szBillboard) {
    (void)hPreview; (void)szControlName; (void)szBillboard;
    return ERROR_CALL_NOT_IMPLEMENTED;
}
