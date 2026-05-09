# MSI Interop - Not Implemented Functions

These functions from the Windows MSI API (Msi.h / MsiQuery.h) are declared and exported
by msi-interop but return `ERROR_CALL_NOT_IMPLEMENTED` (or equivalent error value).
They cannot be meaningfully implemented because they require the Windows Installer Service,
which is a system service that manages installed products, features, and components on the machine.

libmsi only provides MSI database file manipulation capabilities (open, query, modify, commit).
It does not implement the Windows Installer engine.

## Implemented Functions (Database Authoring API)

The following functions ARE fully implemented via libmsi:

- MsiOpenDatabaseA/W, MsiDatabaseCommit, MsiGetDatabaseState
- MsiDatabaseOpenViewA/W, MsiViewExecute, MsiViewFetch, MsiViewModify
- MsiViewGetColumnInfo, MsiViewClose, MsiViewGetErrorA/W
- MsiCreateRecord, MsiRecordIsNull, MsiRecordDataSize
- MsiRecordSetInteger, MsiRecordSetStringA/W, MsiRecordGetInteger, MsiRecordGetStringA/W
- MsiRecordGetFieldCount, MsiRecordSetStreamA/W, MsiRecordReadStream, MsiRecordClearData
- MsiGetSummaryInformationA/W, MsiSummaryInfoGetPropertyCount
- MsiSummaryInfoSetPropertyA/W, MsiSummaryInfoGetPropertyA/W, MsiSummaryInfoPersist
- MsiCloseHandle, MsiCloseAllHandles, MsiGetLastErrorRecord
- MsiFormatRecordA/W, MsiVerifyPackageA/W
- MsiDatabaseGetPrimaryKeysA/W, MsiDatabaseIsTablePersistentA/W
- MsiDatabaseImportA/W, MsiDatabaseExportA/W
- MsiDatabaseMergeA/W, MsiDatabaseApplyTransformA/W

## Not Implemented - Categories

### Product/Feature/Component State Queries

These functions query the installation state of products, features, and components as
tracked by the Windows Installer Service in its internal registration database (registry).
libmsi has no access to this data.

- MsiQueryProductStateA/W
- MsiGetProductInfoA/W
- MsiGetProductInfoExA/W
- MsiGetProductCodeA/W
- MsiGetUserInfoA/W
- MsiCollectUserInfoA/W
- MsiQueryFeatureStateA/W
- MsiQueryFeatureStateExA/W
- MsiUseFeatureA/W
- MsiUseFeatureExA/W
- MsiGetFeatureUsageA/W
- MsiGetComponentPathA/W
- MsiGetComponentPathExA/W
- MsiQueryComponentStateA/W
- MsiLocateComponentA/W
- MsiIsProductElevatedA/W
- MsiGetProductPropertyA/W
- MsiGetFeatureInfoA/W

### Installation/Configuration

These functions trigger product installation, configuration, or reinstallation through
the Windows Installer engine. They execute installation sequences, run custom actions,
and modify the system. libmsi cannot perform any of these operations.

- MsiInstallProductA/W
- MsiConfigureProductA/W
- MsiConfigureProductExA/W
- MsiReinstallProductA/W
- MsiConfigureFeatureA/W
- MsiReinstallFeatureA/W
- MsiInstallMissingComponentA/W
- MsiInstallMissingFileA/W

### Advertising

These functions manage Windows Installer advertisement scripts, which allow products to
be "advertised" (registered for on-demand installation) without being fully installed.
This is a Windows Installer Service feature with no equivalent in libmsi.

- MsiAdvertiseProductA/W
- MsiAdvertiseProductExA/W
- MsiProcessAdvertiseScriptA/W
- MsiAdvertiseScriptA/W
- MsiGetProductInfoFromScriptA/W

### Patch Management

These functions apply, enumerate, and manage patches (.msp files) through the Windows
Installer Service, which tracks patch application state in its registration database.
libmsi does not implement patch application logic.

- MsiApplyPatchA/W
- MsiGetPatchInfoA/W
- MsiEnumPatchesA/W
- MsiRemovePatchesA/W
- MsiExtractPatchXMLDataA/W
- MsiGetPatchInfoExA/W
- MsiApplyMultiplePatchesA/W
- MsiDeterminePatchSequenceA/W
- MsiDetermineApplicablePatchesA/W
- MsiEnumPatchesExA/W
- MsiGetPatchFileListA/W

### Enumeration (Products, Features, Components)

These functions enumerate installed products, features, components, and their relationships
as registered in the Windows Installer Service database. libmsi has no access to the
system's product registration data.

- MsiEnumProductsA/W
- MsiEnumProductsExA/W
- MsiEnumRelatedProductsA/W
- MsiEnumFeaturesA/W
- MsiEnumComponentsA/W
- MsiEnumComponentsExA/W
- MsiEnumClientsA/W
- MsiEnumClientsExA/W
- MsiEnumComponentQualifiersA/W

### Source List Management

These functions manage the source list for installed products, controlling where the
Windows Installer Service looks for installation media. This data is stored in the
system registry by the installer service.

- MsiSourceListClearAllA/W
- MsiSourceListAddSourceA/W
- MsiSourceListForceResolutionA/W
- MsiSourceListAddSourceExA/W
- MsiSourceListAddMediaDiskA/W
- MsiSourceListClearSourceA/W
- MsiSourceListClearMediaDiskA/W
- MsiSourceListClearAllExA/W
- MsiSourceListForceResolutionExA/W
- MsiSourceListSetInfoA/W
- MsiSourceListGetInfoA/W
- MsiSourceListEnumSourcesA/W
- MsiSourceListEnumMediaDisksA/W

### UI Functions

These functions configure the Windows Installer UI subsystem, including setting the
UI level, registering external UI handlers, and enabling logging. They are meaningless
without the Windows Installer engine running an installation session.

- MsiSetInternalUI
- MsiSetExternalUIA/W
- MsiSetExternalUIRecord
- MsiEnableLogA/W
- MsiEnableUIPreview
- MsiPreviewDialogA/W
- MsiPreviewBillboardA/W

### Install Session Functions

These functions operate on an active installation session (MSIHANDLE to an install session)
managed by the Windows Installer engine. They are used by custom actions during installation
to query and modify the installation state. libmsi does not create or manage install sessions.

- MsiGetActiveDatabase
- MsiSetPropertyA/W
- MsiGetPropertyA/W
- MsiGetLanguage
- MsiGetMode
- MsiSetMode
- MsiDoActionA/W
- MsiSequenceA/W
- MsiProcessMessage
- MsiEvaluateConditionA/W
- MsiGetFeatureStateA/W
- MsiSetFeatureStateA/W
- MsiSetFeatureAttributesA/W
- MsiGetComponentStateA/W
- MsiSetComponentStateA/W
- MsiGetFeatureCostA/W
- MsiEnumComponentCostsA/W
- MsiSetInstallLevel
- MsiGetFeatureValidStatesA/W
- MsiGetSourcePathA/W
- MsiGetTargetPathA/W
- MsiSetTargetPathA/W
- MsiVerifyDiskSpace

### Package/Product Opening

These functions open an installed product or package through the Windows Installer Service,
creating an installation session. Unlike MsiOpenDatabase (which opens the .msi file directly),
these functions interact with the installer service to resolve product registration.

- MsiOpenProductA/W
- MsiOpenPackageA/W
- MsiOpenPackageExA/W

### Utility Functions

These functions provide utility operations that depend on Windows-specific APIs
(file version resources, cryptographic signature verification, shell shortcuts)
rather than MSI database content.

- MsiGetFileVersionA/W - requires Windows version resource APIs (GetFileVersionInfo)
- MsiGetFileHashA/W - requires Windows file hashing APIs
- MsiGetFileSignatureInformationA/W - requires Windows cryptographic certificate APIs (returns E_NOTIMPL)
- MsiGetShortcutTargetA/W - requires Windows shell shortcut (IShellLink) APIs
- MsiNotifySidChangeA/W - requires Windows Installer Service SID migration

### Transform Generation

These functions generate transform files (.mst) by comparing two databases, and create
transform summary information. While libmsi can apply transforms, it does not expose
the transform generation functionality.

- MsiDatabaseGenerateTransformA/W
- MsiCreateTransformSummaryInfoA/W

### Transaction Management

These functions manage multi-package installation transactions, a feature of the Windows
Installer engine that allows atomic installation of multiple packages.

- MsiBeginTransactionA/W
- MsiEndTransaction
- MsiJoinTransaction
