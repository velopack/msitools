/*
 * msi_interop.h - Portable cross-platform MSI API header
 *
 * Drop-in replacement for the Windows SDK's Msi.h and MsiQuery.h.
 * Compiles on Windows (MinGW), Linux (GCC/musl), and macOS (Clang)
 * without any Windows SDK headers.
 */

#ifndef MSI_INTEROP_H
#define MSI_INTEROP_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/* Platform types                                                             */
/* ========================================================================== */

#ifdef _WIN32
#include <windows.h>
typedef unsigned int MSIHANDLE;
#else
typedef unsigned int    MSIHANDLE;
typedef unsigned int    UINT;
typedef unsigned int    DWORD;
typedef int             BOOL;
typedef int             INT;
typedef long            HRESULT;
typedef unsigned short  WORD;
typedef unsigned short  LANGID;
typedef unsigned char   BYTE;
typedef void*           LPVOID;
typedef void*           HWND;
typedef void*           HKEY;
typedef void**          PHKEY;
typedef void*           HANDLE;

typedef char*           LPSTR;
typedef const char*     LPCSTR;
typedef unsigned char*  LPBYTE;
typedef DWORD*          LPDWORD;
typedef int*            LPINT;
typedef UINT*           PUINT;
typedef WORD*           LPWORD;

typedef unsigned short*        LPWSTR;
typedef const unsigned short*  LPCWSTR;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef struct {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;
#endif /* !_WIN32 */

/* ========================================================================== */
/* Export / calling convention macros                                          */
/* ========================================================================== */

#ifdef _WIN32
#define MSI_INTEROP_EXPORT __declspec(dllexport)
/* WINAPI is already defined by windows.h */
#else
#define MSI_INTEROP_EXPORT __attribute__((visibility("default")))
#define WINAPI
#endif

/* ========================================================================== */
/* MSI constants                                                              */
/* ========================================================================== */

#define MSI_NULL_INTEGER    0x80000000
#define MAX_GUID_CHARS      38
#define MAX_FEATURE_CHARS   38

/* ========================================================================== */
/* Error codes                                                                */
/* ========================================================================== */

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS                   0L
#endif
#ifndef ERROR_ACCESS_DENIED
#define ERROR_ACCESS_DENIED             5L
#endif
#ifndef ERROR_INVALID_HANDLE
#define ERROR_INVALID_HANDLE            6L
#endif
#ifndef ERROR_INVALID_DATA
#define ERROR_INVALID_DATA              13L
#endif
#ifndef ERROR_GEN_FAILURE
#define ERROR_GEN_FAILURE               31L
#endif
#ifndef ERROR_INVALID_PARAMETER
#define ERROR_INVALID_PARAMETER         87L
#endif
#ifndef ERROR_CALL_NOT_IMPLEMENTED
#define ERROR_CALL_NOT_IMPLEMENTED      120L
#endif
#ifndef ERROR_BAD_ARGUMENTS
#define ERROR_BAD_ARGUMENTS             160L
#endif
#ifndef ERROR_MORE_DATA
#define ERROR_MORE_DATA                 234L
#endif
#ifndef ERROR_NO_MORE_ITEMS
#define ERROR_NO_MORE_ITEMS             259L
#endif
#ifndef ERROR_INSTALL_USEREXIT
#define ERROR_INSTALL_USEREXIT          1602L
#endif
#ifndef ERROR_INSTALL_FAILURE
#define ERROR_INSTALL_FAILURE           1603L
#endif
#ifndef ERROR_INSTALL_SUSPEND
#define ERROR_INSTALL_SUSPEND           1604L
#endif
#ifndef ERROR_UNKNOWN_PRODUCT
#define ERROR_UNKNOWN_PRODUCT           1605L
#endif
#ifndef ERROR_UNKNOWN_FEATURE
#define ERROR_UNKNOWN_FEATURE           1606L
#endif
#ifndef ERROR_UNKNOWN_COMPONENT
#define ERROR_UNKNOWN_COMPONENT         1607L
#endif
#ifndef ERROR_UNKNOWN_PROPERTY
#define ERROR_UNKNOWN_PROPERTY          1608L
#endif
#ifndef ERROR_INVALID_HANDLE_STATE
#define ERROR_INVALID_HANDLE_STATE      1609L
#endif
#ifndef ERROR_BAD_CONFIGURATION
#define ERROR_BAD_CONFIGURATION         1610L
#endif
#ifndef ERROR_INDEX_ABSENT
#define ERROR_INDEX_ABSENT              1611L
#endif
#ifndef ERROR_INSTALL_SOURCE_ABSENT
#define ERROR_INSTALL_SOURCE_ABSENT     1612L
#endif
#ifndef ERROR_INSTALL_PACKAGE_VERSION
#define ERROR_INSTALL_PACKAGE_VERSION   1613L
#endif
#ifndef ERROR_PRODUCT_UNINSTALLED
#define ERROR_PRODUCT_UNINSTALLED       1614L
#endif
#ifndef ERROR_BAD_QUERY_SYNTAX
#define ERROR_BAD_QUERY_SYNTAX          1615L
#endif
#ifndef ERROR_INVALID_FIELD
#define ERROR_INVALID_FIELD             1616L
#endif
#ifndef ERROR_INSTALL_ALREADY_RUNNING
#define ERROR_INSTALL_ALREADY_RUNNING   1618L
#endif
#ifndef ERROR_INSTALL_PACKAGE_OPEN_FAILED
#define ERROR_INSTALL_PACKAGE_OPEN_FAILED 1619L
#endif
#ifndef ERROR_INSTALL_PACKAGE_INVALID
#define ERROR_INSTALL_PACKAGE_INVALID   1620L
#endif
#ifndef ERROR_INSTALL_UI_FAILURE
#define ERROR_INSTALL_UI_FAILURE        1621L
#endif
#ifndef ERROR_INSTALL_LOG_FAILURE
#define ERROR_INSTALL_LOG_FAILURE       1622L
#endif
#ifndef ERROR_INSTALL_LANGUAGE_UNSUPPORTED
#define ERROR_INSTALL_LANGUAGE_UNSUPPORTED 1623L
#endif
#ifndef ERROR_INSTALL_TRANSFORM_FAILURE
#define ERROR_INSTALL_TRANSFORM_FAILURE 1624L
#endif
#ifndef ERROR_INSTALL_PACKAGE_REJECTED
#define ERROR_INSTALL_PACKAGE_REJECTED  1625L
#endif
#ifndef ERROR_FUNCTION_NOT_CALLED
#define ERROR_FUNCTION_NOT_CALLED       1626L
#endif
#ifndef ERROR_FUNCTION_FAILED
#define ERROR_FUNCTION_FAILED           1627L
#endif
#ifndef ERROR_INVALID_TABLE
#define ERROR_INVALID_TABLE             1628L
#endif
#ifndef ERROR_DATATYPE_MISMATCH
#define ERROR_DATATYPE_MISMATCH         1629L
#endif
#ifndef ERROR_UNSUPPORTED_TYPE
#define ERROR_UNSUPPORTED_TYPE          1630L
#endif
#ifndef ERROR_CREATE_FAILED
#define ERROR_CREATE_FAILED             1631L
#endif
#ifndef ERROR_INSTALL_TEMP_UNWRITABLE
#define ERROR_INSTALL_TEMP_UNWRITABLE   1632L
#endif
#ifndef ERROR_INSTALL_PLATFORM_UNSUPPORTED
#define ERROR_INSTALL_PLATFORM_UNSUPPORTED 1633L
#endif
#ifndef ERROR_INSTALL_NOTUSED
#define ERROR_INSTALL_NOTUSED           1634L
#endif
#ifndef ERROR_PATCH_PACKAGE_OPEN_FAILED
#define ERROR_PATCH_PACKAGE_OPEN_FAILED 1635L
#endif
#ifndef ERROR_PATCH_PACKAGE_INVALID
#define ERROR_PATCH_PACKAGE_INVALID     1636L
#endif
#ifndef ERROR_PATCH_PACKAGE_UNSUPPORTED
#define ERROR_PATCH_PACKAGE_UNSUPPORTED 1637L
#endif
#ifndef ERROR_PRODUCT_VERSION
#define ERROR_PRODUCT_VERSION           1638L
#endif
#ifndef ERROR_INVALID_COMMAND_LINE
#define ERROR_INVALID_COMMAND_LINE      1639L
#endif
#ifndef ERROR_INSTALL_REMOTE_DISALLOWED
#define ERROR_INSTALL_REMOTE_DISALLOWED 1640L
#endif
#ifndef ERROR_SUCCESS_REBOOT_INITIATED
#define ERROR_SUCCESS_REBOOT_INITIATED  1641L
#endif
#ifndef ERROR_PATCH_TARGET_NOT_FOUND
#define ERROR_PATCH_TARGET_NOT_FOUND    1642L
#endif
#ifndef ERROR_PATCH_PACKAGE_REJECTED
#define ERROR_PATCH_PACKAGE_REJECTED    1643L
#endif
#ifndef ERROR_INSTALL_TRANSFORM_REJECTED
#define ERROR_INSTALL_TRANSFORM_REJECTED 1644L
#endif
#ifndef ERROR_INSTALL_REMOTE_PROHIBITED
#define ERROR_INSTALL_REMOTE_PROHIBITED 1645L
#endif
#ifndef ERROR_PATCH_REMOVAL_UNSUPPORTED
#define ERROR_PATCH_REMOVAL_UNSUPPORTED 1646L
#endif
#ifndef ERROR_UNKNOWN_PATCH
#define ERROR_UNKNOWN_PATCH             1647L
#endif
#ifndef ERROR_PATCH_NO_SEQUENCE
#define ERROR_PATCH_NO_SEQUENCE         1648L
#endif
#ifndef ERROR_PATCH_REMOVAL_DISALLOWED
#define ERROR_PATCH_REMOVAL_DISALLOWED  1649L
#endif
#ifndef ERROR_INVALID_PATCH_XML
#define ERROR_INVALID_PATCH_XML         1650L
#endif
#ifndef ERROR_PATCH_MANAGED_ADVERTISED_PRODUCT
#define ERROR_PATCH_MANAGED_ADVERTISED_PRODUCT 1651L
#endif
#ifndef ERROR_INSTALL_SERVICE_SAFEBOOT
#define ERROR_INSTALL_SERVICE_SAFEBOOT  1652L
#endif
#ifndef ERROR_INSTALL_REJECTED
#define ERROR_INSTALL_REJECTED          1654L
#endif
#ifndef ERROR_INVALID_DATATYPE
#define ERROR_INVALID_DATATYPE          1804L
#endif

/* ========================================================================== */
/* Msi.h enums                                                                */
/* ========================================================================== */

typedef enum {
    INSTALLMESSAGE_FATALEXIT        = 0x00000000L,
    INSTALLMESSAGE_ERROR            = 0x01000000L,
    INSTALLMESSAGE_WARNING          = 0x02000000L,
    INSTALLMESSAGE_USER             = 0x03000000L,
    INSTALLMESSAGE_INFO             = 0x04000000L,
    INSTALLMESSAGE_FILESINUSE       = 0x05000000L,
    INSTALLMESSAGE_RESOLVESOURCE    = 0x06000000L,
    INSTALLMESSAGE_OUTOFDISKSPACE   = 0x07000000L,
    INSTALLMESSAGE_ACTIONSTART      = 0x08000000L,
    INSTALLMESSAGE_ACTIONDATA       = 0x09000000L,
    INSTALLMESSAGE_PROGRESS         = 0x0A000000L,
    INSTALLMESSAGE_COMMONDATA       = 0x0B000000L,
    INSTALLMESSAGE_INITIALIZE       = 0x0C000000L,
    INSTALLMESSAGE_TERMINATE        = 0x0D000000L,
    INSTALLMESSAGE_SHOWDIALOG       = 0x0E000000L,
    INSTALLMESSAGE_PERFORMANCE      = 0x0F000000L,
    INSTALLMESSAGE_RMFILESINUSE     = 0x19000000L,
    INSTALLMESSAGE_INSTALLSTART     = 0x1A000000L,
    INSTALLMESSAGE_INSTALLEND       = 0x1B000000L
} INSTALLMESSAGE;

typedef enum {
    INSTALLUILEVEL_NOCHANGE     = 0,
    INSTALLUILEVEL_DEFAULT      = 1,
    INSTALLUILEVEL_NONE         = 2,
    INSTALLUILEVEL_BASIC        = 3,
    INSTALLUILEVEL_REDUCED      = 4,
    INSTALLUILEVEL_FULL         = 5,
    INSTALLUILEVEL_ENDDIALOG    = 0x0080,
    INSTALLUILEVEL_PROGRESSONLY = 0x0040,
    INSTALLUILEVEL_HIDECANCEL   = 0x0020,
    INSTALLUILEVEL_SOURCERESONLY = 0x0100,
    INSTALLUILEVEL_UACONLY      = 0x0200
} INSTALLUILEVEL;

typedef enum {
    INSTALLSTATE_NOTUSED      = -7,
    INSTALLSTATE_BADCONFIG    = -6,
    INSTALLSTATE_INCOMPLETE   = -5,
    INSTALLSTATE_SOURCEABSENT = -4,
    INSTALLSTATE_MOREDATA     = -3,
    INSTALLSTATE_INVALIDARG   = -2,
    INSTALLSTATE_UNKNOWN      = -1,
    INSTALLSTATE_BROKEN       =  0,
    INSTALLSTATE_ADVERTISED   =  1,
    INSTALLSTATE_REMOVED      =  1,
    INSTALLSTATE_ABSENT       =  2,
    INSTALLSTATE_LOCAL        =  3,
    INSTALLSTATE_SOURCE       =  4,
    INSTALLSTATE_DEFAULT      =  5
} INSTALLSTATE;

typedef enum {
    USERINFOSTATE_MOREDATA   = -3,
    USERINFOSTATE_INVALIDARG = -2,
    USERINFOSTATE_UNKNOWN    = -1,
    USERINFOSTATE_ABSENT     =  0,
    USERINFOSTATE_PRESENT    =  1
} USERINFOSTATE;

typedef enum {
    INSTALLLEVEL_DEFAULT = 0,
    INSTALLLEVEL_MINIMUM = 1,
    INSTALLLEVEL_MAXIMUM = 0xFFFF
} INSTALLLEVEL;

typedef enum {
    REINSTALLMODE_REPAIR           = 0x00000001,
    REINSTALLMODE_FILEMISSING      = 0x00000002,
    REINSTALLMODE_FILEOLDERVERSION = 0x00000004,
    REINSTALLMODE_FILEEQUALVERSION = 0x00000008,
    REINSTALLMODE_FILEEXACT        = 0x00000010,
    REINSTALLMODE_FILEVERIFY       = 0x00000020,
    REINSTALLMODE_FILEREPLACE      = 0x00000040,
    REINSTALLMODE_MACHINEDATA      = 0x00000080,
    REINSTALLMODE_USERDATA         = 0x00000100,
    REINSTALLMODE_SHORTCUT         = 0x00000200,
    REINSTALLMODE_PACKAGE          = 0x00000400
} REINSTALLMODE;

typedef enum {
    INSTALLLOGMODE_FATALEXIT      = (1 << (INSTALLMESSAGE_FATALEXIT      >> 24)),
    INSTALLLOGMODE_ERROR          = (1 << (INSTALLMESSAGE_ERROR          >> 24)),
    INSTALLLOGMODE_WARNING        = (1 << (INSTALLMESSAGE_WARNING        >> 24)),
    INSTALLLOGMODE_USER           = (1 << (INSTALLMESSAGE_USER           >> 24)),
    INSTALLLOGMODE_INFO           = (1 << (INSTALLMESSAGE_INFO           >> 24)),
    INSTALLLOGMODE_RESOLVESOURCE  = (1 << (INSTALLMESSAGE_RESOLVESOURCE  >> 24)),
    INSTALLLOGMODE_OUTOFDISKSPACE = (1 << (INSTALLMESSAGE_OUTOFDISKSPACE >> 24)),
    INSTALLLOGMODE_ACTIONSTART    = (1 << (INSTALLMESSAGE_ACTIONSTART    >> 24)),
    INSTALLLOGMODE_ACTIONDATA     = (1 << (INSTALLMESSAGE_ACTIONDATA     >> 24)),
    INSTALLLOGMODE_COMMONDATA     = (1 << (INSTALLMESSAGE_COMMONDATA     >> 24)),
    INSTALLLOGMODE_PROPERTYDUMP   = (1 << (INSTALLMESSAGE_PROGRESS       >> 24)),
    INSTALLLOGMODE_VERBOSE        = (1 << (INSTALLMESSAGE_INITIALIZE     >> 24)),
    INSTALLLOGMODE_EXTRADEBUG     = (1 << (INSTALLMESSAGE_TERMINATE      >> 24)),
    INSTALLLOGMODE_LOGONLYONERROR = (1 << (INSTALLMESSAGE_SHOWDIALOG     >> 24)),
    INSTALLLOGMODE_LOGPERFORMANCE = (1 << (INSTALLMESSAGE_PERFORMANCE    >> 24)),
    INSTALLLOGMODE_PROGRESS       = (1 << (INSTALLMESSAGE_PROGRESS       >> 24)),
    INSTALLLOGMODE_INITIALIZE     = (1 << (INSTALLMESSAGE_INITIALIZE     >> 24)),
    INSTALLLOGMODE_TERMINATE      = (1 << (INSTALLMESSAGE_TERMINATE      >> 24)),
    INSTALLLOGMODE_SHOWDIALOG     = (1 << (INSTALLMESSAGE_SHOWDIALOG     >> 24)),
    INSTALLLOGMODE_FILESINUSE     = (1 << (INSTALLMESSAGE_FILESINUSE     >> 24)),
    INSTALLLOGMODE_RMFILESINUSE   = (1 << (INSTALLMESSAGE_RMFILESINUSE   >> 24)),
    INSTALLLOGMODE_INSTALLSTART   = (1 << (INSTALLMESSAGE_INSTALLSTART   >> 24)),
    INSTALLLOGMODE_INSTALLEND     = (1 << (INSTALLMESSAGE_INSTALLEND     >> 24))
} INSTALLLOGMODE;

typedef enum {
    INSTALLLOGATTRIBUTES_APPEND          = (1 << 0),
    INSTALLLOGATTRIBUTES_FLUSHEACHLINE   = (1 << 1)
} INSTALLLOGATTRIBUTES;

typedef enum {
    INSTALLFEATUREATTRIBUTE_FAVORLOCAL             = 1 << 0,
    INSTALLFEATUREATTRIBUTE_FAVORSOURCE            = 1 << 1,
    INSTALLFEATUREATTRIBUTE_FOLLOWPARENT           = 1 << 2,
    INSTALLFEATUREATTRIBUTE_FAVORADVERTISE         = 1 << 3,
    INSTALLFEATUREATTRIBUTE_DISALLOWADVERTISE      = 1 << 4,
    INSTALLFEATUREATTRIBUTE_NOUNSUPPORTEDADVERTISE = 1 << 5
} INSTALLFEATUREATTRIBUTE;

typedef enum {
    INSTALLMODE_NODETECTION_ANY    = -4,
    INSTALLMODE_NOSOURCERESOLUTION = -3,
    INSTALLMODE_NODETECTION        = -2,
    INSTALLMODE_EXISTING           = -1,
    INSTALLMODE_DEFAULT            =  0
} INSTALLMODE;

typedef enum {
    MSIPATCHSTATE_INVALID   = 0,
    MSIPATCHSTATE_APPLIED   = 1,
    MSIPATCHSTATE_SUPERSEDED = 2,
    MSIPATCHSTATE_OBSOLETED = 4,
    MSIPATCHSTATE_REGISTERED = 8,
    MSIPATCHSTATE_ALL       = (MSIPATCHSTATE_APPLIED | MSIPATCHSTATE_SUPERSEDED | MSIPATCHSTATE_OBSOLETED | MSIPATCHSTATE_REGISTERED)
} MSIPATCHSTATE;

typedef enum {
    MSIINSTALLCONTEXT_FIRSTVISIBLE   = 0,
    MSIINSTALLCONTEXT_NONE           = 0,
    MSIINSTALLCONTEXT_USERMANAGED    = 1,
    MSIINSTALLCONTEXT_USERUNMANAGED  = 2,
    MSIINSTALLCONTEXT_MACHINE        = 4,
    MSIINSTALLCONTEXT_ALL            = (MSIINSTALLCONTEXT_USERMANAGED | MSIINSTALLCONTEXT_USERUNMANAGED | MSIINSTALLCONTEXT_MACHINE),
    MSIINSTALLCONTEXT_ALLUSERMANAGED = 8
} MSIINSTALLCONTEXT;

typedef enum {
    SCRIPTFLAGS_CACHEINFO                = 0x00000001L,
    SCRIPTFLAGS_SHORTCUTS                = 0x00000004L,
    SCRIPTFLAGS_MACHINEASSIGN            = 0x00000008L,
    SCRIPTFLAGS_REGDATA_CNFGINFO         = 0x00000020L,
    SCRIPTFLAGS_VALIDATE_TRANSFORMS_LIST = 0x00000040L,
    SCRIPTFLAGS_REGDATA_CLASSINFO        = 0x00000080L,
    SCRIPTFLAGS_REGDATA_EXTENSIONINFO    = 0x00000100L,
    SCRIPTFLAGS_REGDATA_APPINFO          = SCRIPTFLAGS_REGDATA_CNFGINFO
                                         | SCRIPTFLAGS_REGDATA_CLASSINFO
                                         | SCRIPTFLAGS_REGDATA_EXTENSIONINFO,
    SCRIPTFLAGS_REGDATA                  = SCRIPTFLAGS_REGDATA_APPINFO
                                         | 0x00000010L
} SCRIPTFLAGS;

typedef enum {
    ADVERTISEFLAGS_MACHINEASSIGN = 0,
    ADVERTISEFLAGS_USERASSIGN    = 1
} ADVERTISEFLAGS;

typedef enum {
    INSTALLTYPE_DEFAULT        = 0,
    INSTALLTYPE_NETWORK_IMAGE  = 1,
    INSTALLTYPE_SINGLE_INSTANCE = 2
} INSTALLTYPE;

typedef enum {
    MSIOPENPACKAGEFLAGS_IGNOREMACHINESTATE = 0x00000001L
} MSIOPENPACKAGEFLAGS;

typedef enum {
    MSISOURCETYPE_UNKNOWN = 0x00000000L,
    MSISOURCETYPE_NETWORK = 0x00000001L,
    MSISOURCETYPE_URL     = 0x00000002L,
    MSISOURCETYPE_MEDIA   = 0x00000004L
} MSISOURCETYPE;

typedef enum {
    MSICODE_PRODUCT = 0x00000000L,
    MSICODE_PATCH   = 0x40000000L
} MSICODE;

/* ========================================================================== */
/* MsiQuery.h enums                                                           */
/* ========================================================================== */

typedef enum {
    MSIDBSTATE_ERROR = -1,
    MSIDBSTATE_READ  =  0,
    MSIDBSTATE_WRITE =  1
} MSIDBSTATE;

typedef enum {
    MSIMODIFY_SEEK             = -1,
    MSIMODIFY_REFRESH          =  0,
    MSIMODIFY_INSERT           =  1,
    MSIMODIFY_UPDATE           =  2,
    MSIMODIFY_ASSIGN           =  3,
    MSIMODIFY_REPLACE          =  4,
    MSIMODIFY_MERGE            =  5,
    MSIMODIFY_DELETE           =  6,
    MSIMODIFY_INSERT_TEMPORARY =  7,
    MSIMODIFY_VALIDATE         =  8,
    MSIMODIFY_VALIDATE_NEW     =  9,
    MSIMODIFY_VALIDATE_FIELD   = 10,
    MSIMODIFY_VALIDATE_DELETE  = 11
} MSIMODIFY;

typedef enum {
    MSICOLINFO_NAMES = 0,
    MSICOLINFO_TYPES = 1
} MSICOLINFO;

typedef enum {
    MSICONDITION_FALSE  = 0,
    MSICONDITION_TRUE   = 1,
    MSICONDITION_NONE   = 2,
    MSICONDITION_ERROR  = 3
} MSICONDITION;

typedef enum {
    MSICOSTTREE_SELFONLY = 0,
    MSICOSTTREE_CHILDREN = 1,
    MSICOSTTREE_PARENTS  = 2,
    MSICOSTTREE_RESERVED = 3
} MSICOSTTREE;

typedef enum {
    MSIDBERROR_INVALIDARG        = -3,
    MSIDBERROR_MOREDATA          = -2,
    MSIDBERROR_FUNCTIONERROR     = -1,
    MSIDBERROR_NOERROR           =  0,
    MSIDBERROR_DUPLICATEKEY      =  1,
    MSIDBERROR_REQUIRED          =  2,
    MSIDBERROR_BADLINK           =  3,
    MSIDBERROR_OVERFLOW          =  4,
    MSIDBERROR_UNDERFLOW         =  5,
    MSIDBERROR_NOTINSET          =  6,
    MSIDBERROR_BADVERSION        =  7,
    MSIDBERROR_BADCASE           =  8,
    MSIDBERROR_BADGUID           =  9,
    MSIDBERROR_BADWILDCARD       = 10,
    MSIDBERROR_BADIDENTIFIER     = 11,
    MSIDBERROR_BADLANGUAGE       = 12,
    MSIDBERROR_BADFILENAME       = 13,
    MSIDBERROR_BADPATH           = 14,
    MSIDBERROR_BADCONDITION      = 15,
    MSIDBERROR_BADFORMATTED      = 16,
    MSIDBERROR_BADTEMPLATE       = 17,
    MSIDBERROR_BADDEFAULTDIR     = 18,
    MSIDBERROR_BADREGPATH        = 19,
    MSIDBERROR_BADCUSTOMSOURCE   = 20,
    MSIDBERROR_BADPROPERTY       = 21,
    MSIDBERROR_MISSINGDATA       = 22,
    MSIDBERROR_BADCATEGORY       = 23,
    MSIDBERROR_BADKEYTABLE       = 24,
    MSIDBERROR_BADMAXMINVALUES   = 25,
    MSIDBERROR_BADCABINET        = 26,
    MSIDBERROR_BADSHORTCUT       = 27,
    MSIDBERROR_STRINGOVERFLOW    = 28,
    MSIDBERROR_BADLOCALIZEATTRIB = 29
} MSIDBERROR;

typedef enum {
    MSIRUNMODE_ADMIN            =  0,
    MSIRUNMODE_ADVERTISE        =  1,
    MSIRUNMODE_MAINTENANCE      =  2,
    MSIRUNMODE_ROLLBACKENABLED  =  3,
    MSIRUNMODE_LOGENABLED       =  4,
    MSIRUNMODE_OPERATIONS       =  5,
    MSIRUNMODE_REBOOTATEND      =  6,
    MSIRUNMODE_REBOOTNOW        =  7,
    MSIRUNMODE_CABINET          =  8,
    MSIRUNMODE_SOURCESHORTNAMES =  9,
    MSIRUNMODE_TARGETSHORTNAMES = 10,
    MSIRUNMODE_RESERVED11       = 11,
    MSIRUNMODE_WINDOWS9X        = 12,
    MSIRUNMODE_ZAWENABLED       = 13,
    MSIRUNMODE_RESERVED14       = 14,
    MSIRUNMODE_RESERVED15       = 15,
    MSIRUNMODE_SCHEDULED        = 16,
    MSIRUNMODE_ROLLBACK         = 17,
    MSIRUNMODE_COMMIT           = 18
} MSIRUNMODE;

typedef enum {
    MSITRANSFORM_ERROR_ADDEXISTINGROW   = 0x00000001,
    MSITRANSFORM_ERROR_DELMISSINGROW    = 0x00000002,
    MSITRANSFORM_ERROR_ADDEXISTINGTABLE = 0x00000004,
    MSITRANSFORM_ERROR_DELMISSINGTABLE  = 0x00000008,
    MSITRANSFORM_ERROR_UPDATEMISSINGROW = 0x00000010,
    MSITRANSFORM_ERROR_CHANGECODEPAGE   = 0x00000020,
    MSITRANSFORM_ERROR_VIEWTRANSFORM    = 0x00000100
} MSITRANSFORM_ERROR;

typedef enum {
    MSITRANSFORM_VALIDATE_LANGUAGE                  = 0x00000001,
    MSITRANSFORM_VALIDATE_PRODUCT                   = 0x00000002,
    MSITRANSFORM_VALIDATE_PLATFORM                  = 0x00000004,
    MSITRANSFORM_VALIDATE_MAJORVERSION              = 0x00000008,
    MSITRANSFORM_VALIDATE_MINORVERSION              = 0x00000010,
    MSITRANSFORM_VALIDATE_UPDATEVERSION             = 0x00000020,
    MSITRANSFORM_VALIDATE_NEWLESSBASEVERSION        = 0x00000040,
    MSITRANSFORM_VALIDATE_NEWLESSEQUALBASEVERSION   = 0x00000080,
    MSITRANSFORM_VALIDATE_NEWEQUALBASEVERSION       = 0x00000100,
    MSITRANSFORM_VALIDATE_NEWGREATEREQUALBASEVERSION = 0x00000200,
    MSITRANSFORM_VALIDATE_NEWGREATERBASEVERSION     = 0x00000400,
    MSITRANSFORM_VALIDATE_UPGRADECODE               = 0x00000800
} MSITRANSFORM_VALIDATE;

/* ========================================================================== */
/* MSIDBOPEN constants                                                        */
/* ========================================================================== */

#define MSIDBOPEN_READONLY     ((LPCWSTR)(uintptr_t)0)
#define MSIDBOPEN_TRANSACT     ((LPCWSTR)(uintptr_t)1)
#define MSIDBOPEN_DIRECT       ((LPCWSTR)(uintptr_t)2)
#define MSIDBOPEN_CREATE       ((LPCWSTR)(uintptr_t)3)
#define MSIDBOPEN_CREATEDIRECT ((LPCWSTR)(uintptr_t)4)

/* ========================================================================== */
/* Callback types                                                             */
/* ========================================================================== */

typedef int (WINAPI *INSTALLUI_HANDLERA)(LPVOID pvContext, UINT iMessageType, LPCSTR szMessage);
typedef int (WINAPI *INSTALLUI_HANDLERW)(LPVOID pvContext, UINT iMessageType, LPCWSTR szMessage);

typedef int (WINAPI *INSTALLUI_HANDLER_RECORD)(LPVOID pvContext, UINT iMessageType, MSIHANDLE hRecord);
typedef INSTALLUI_HANDLER_RECORD PINSTALLUI_HANDLER_RECORD;

/* ========================================================================== */
/* Structs                                                                    */
/* ========================================================================== */

typedef struct {
    DWORD dwFileHashInfoSize;
    DWORD dwData[4];
} MSIFILEHASHINFO, *PMSIFILEHASHINFO;

typedef enum {
    MSIPATCH_DATATYPE_PATCHFILE = 0,
    MSIPATCH_DATATYPE_XMLPATH   = 1,
    MSIPATCH_DATATYPE_XMLBLOB   = 2
} MSIPATCHDATATYPE;

typedef struct {
    LPCSTR  szPatchData;
    MSIPATCHDATATYPE ePatchDataType;
    LPSTR   szPatchFile;
    DWORD   dwOrder;
    UINT    uStatus;
} MSIPATCHSEQUENCEINFOA, *PMSIPATCHSEQUENCEINFOA;

typedef struct {
    LPCWSTR szPatchData;
    MSIPATCHDATATYPE ePatchDataType;
    LPWSTR  szPatchFile;
    DWORD   dwOrder;
    UINT    uStatus;
} MSIPATCHSEQUENCEINFOW, *PMSIPATCHSEQUENCEINFOW;

/* ========================================================================== */
/* Database functions                                                         */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiOpenDatabaseA(LPCSTR szDatabasePath, LPCSTR szPersist, MSIHANDLE *phDatabase);
MSI_INTEROP_EXPORT UINT WINAPI MsiOpenDatabaseW(LPCWSTR szDatabasePath, LPCWSTR szPersist, MSIHANDLE *phDatabase);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseOpenViewA(MSIHANDLE hDatabase, LPCSTR szQuery, MSIHANDLE *phView);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseOpenViewW(MSIHANDLE hDatabase, LPCWSTR szQuery, MSIHANDLE *phView);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseCommit(MSIHANDLE hDatabase);
MSI_INTEROP_EXPORT MSIDBSTATE WINAPI MsiGetDatabaseState(MSIHANDLE hDatabase);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseGetPrimaryKeysA(MSIHANDLE hDatabase, LPCSTR szTableName, MSIHANDLE *phRecord);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseGetPrimaryKeysW(MSIHANDLE hDatabase, LPCWSTR szTableName, MSIHANDLE *phRecord);
MSI_INTEROP_EXPORT MSICONDITION WINAPI MsiDatabaseIsTablePersistentA(MSIHANDLE hDatabase, LPCSTR szTableName);
MSI_INTEROP_EXPORT MSICONDITION WINAPI MsiDatabaseIsTablePersistentW(MSIHANDLE hDatabase, LPCWSTR szTableName);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseImportA(MSIHANDLE hDatabase, LPCSTR szFolderPath, LPCSTR szFileName);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseImportW(MSIHANDLE hDatabase, LPCWSTR szFolderPath, LPCWSTR szFileName);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseExportA(MSIHANDLE hDatabase, LPCSTR szTableName, LPCSTR szFolderPath, LPCSTR szFileName);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseExportW(MSIHANDLE hDatabase, LPCWSTR szTableName, LPCWSTR szFolderPath, LPCWSTR szFileName);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseMergeA(MSIHANDLE hDatabase, MSIHANDLE hDatabaseMerge, LPCSTR szTableName);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseMergeW(MSIHANDLE hDatabase, MSIHANDLE hDatabaseMerge, LPCWSTR szTableName);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseApplyTransformA(MSIHANDLE hDatabase, LPCSTR szTransformFile, int iErrorConditions);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseApplyTransformW(MSIHANDLE hDatabase, LPCWSTR szTransformFile, int iErrorConditions);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseGenerateTransformA(MSIHANDLE hDatabase, MSIHANDLE hDatabaseReference, LPCSTR szTransformFile, int iReserved1, int iReserved2);
MSI_INTEROP_EXPORT UINT WINAPI MsiDatabaseGenerateTransformW(MSIHANDLE hDatabase, MSIHANDLE hDatabaseReference, LPCWSTR szTransformFile, int iReserved1, int iReserved2);
MSI_INTEROP_EXPORT UINT WINAPI MsiCreateTransformSummaryInfoA(MSIHANDLE hDatabase, MSIHANDLE hDatabaseReference, LPCSTR szTransformFile, int iErrorConditions, int iValidation);
MSI_INTEROP_EXPORT UINT WINAPI MsiCreateTransformSummaryInfoW(MSIHANDLE hDatabase, MSIHANDLE hDatabaseReference, LPCWSTR szTransformFile, int iErrorConditions, int iValidation);

/* ========================================================================== */
/* View functions                                                             */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiViewExecute(MSIHANDLE hView, MSIHANDLE hRecord);
MSI_INTEROP_EXPORT UINT WINAPI MsiViewFetch(MSIHANDLE hView, MSIHANDLE *phRecord);
MSI_INTEROP_EXPORT UINT WINAPI MsiViewModify(MSIHANDLE hView, MSIMODIFY eModifyMode, MSIHANDLE hRecord);
MSI_INTEROP_EXPORT UINT WINAPI MsiViewGetColumnInfo(MSIHANDLE hView, MSICOLINFO eColumnInfo, MSIHANDLE *phRecord);
MSI_INTEROP_EXPORT UINT WINAPI MsiViewClose(MSIHANDLE hView);
MSI_INTEROP_EXPORT MSIDBERROR WINAPI MsiViewGetErrorA(MSIHANDLE hView, LPSTR szColumnNameBuffer, LPDWORD pcchBuf);
MSI_INTEROP_EXPORT MSIDBERROR WINAPI MsiViewGetErrorW(MSIHANDLE hView, LPWSTR szColumnNameBuffer, LPDWORD pcchBuf);

/* ========================================================================== */
/* Record functions                                                           */
/* ========================================================================== */

MSI_INTEROP_EXPORT MSIHANDLE WINAPI MsiCreateRecord(UINT cParams);
MSI_INTEROP_EXPORT BOOL WINAPI MsiRecordIsNull(MSIHANDLE hRecord, UINT iField);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordDataSize(MSIHANDLE hRecord, UINT iField);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordSetInteger(MSIHANDLE hRecord, UINT iField, int iValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordSetStringA(MSIHANDLE hRecord, UINT iField, LPCSTR szValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordSetStringW(MSIHANDLE hRecord, UINT iField, LPCWSTR szValue);
MSI_INTEROP_EXPORT int WINAPI MsiRecordGetInteger(MSIHANDLE hRecord, UINT iField);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordGetStringA(MSIHANDLE hRecord, UINT iField, LPSTR szValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordGetStringW(MSIHANDLE hRecord, UINT iField, LPWSTR szValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordGetFieldCount(MSIHANDLE hRecord);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordSetStreamA(MSIHANDLE hRecord, UINT iField, LPCSTR szFilePath);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordSetStreamW(MSIHANDLE hRecord, UINT iField, LPCWSTR szFilePath);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordReadStream(MSIHANDLE hRecord, UINT iField, char *szDataBuf, LPDWORD pcbDataBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiRecordClearData(MSIHANDLE hRecord);

/* ========================================================================== */
/* Summary information functions                                              */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiGetSummaryInformationA(MSIHANDLE hDatabase, LPCSTR szDatabasePath, UINT uiUpdateCount, MSIHANDLE *phSummaryInfo);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetSummaryInformationW(MSIHANDLE hDatabase, LPCWSTR szDatabasePath, UINT uiUpdateCount, MSIHANDLE *phSummaryInfo);
MSI_INTEROP_EXPORT UINT WINAPI MsiSummaryInfoGetPropertyCount(MSIHANDLE hSummaryInfo, PUINT puiPropertyCount);
MSI_INTEROP_EXPORT UINT WINAPI MsiSummaryInfoSetPropertyA(MSIHANDLE hSummaryInfo, UINT uiProperty, UINT uiDataType, int iValue, FILETIME *pftValue, LPCSTR szValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiSummaryInfoSetPropertyW(MSIHANDLE hSummaryInfo, UINT uiProperty, UINT uiDataType, int iValue, FILETIME *pftValue, LPCWSTR szValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiSummaryInfoGetPropertyA(MSIHANDLE hSummaryInfo, UINT uiProperty, PUINT puiDataType, LPINT piValue, FILETIME *pftValue, LPSTR szValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiSummaryInfoGetPropertyW(MSIHANDLE hSummaryInfo, UINT uiProperty, PUINT puiDataType, LPINT piValue, FILETIME *pftValue, LPWSTR szValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiSummaryInfoPersist(MSIHANDLE hSummaryInfo);

/* ========================================================================== */
/* Handle / error functions                                                   */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiCloseHandle(MSIHANDLE hAny);
MSI_INTEROP_EXPORT UINT WINAPI MsiCloseAllHandles(void);
MSI_INTEROP_EXPORT MSIHANDLE WINAPI MsiGetLastErrorRecord(void);
MSI_INTEROP_EXPORT UINT WINAPI MsiFormatRecordA(MSIHANDLE hInstall, MSIHANDLE hRecord, LPSTR szResultBuf, LPDWORD pcchResultBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiFormatRecordW(MSIHANDLE hInstall, MSIHANDLE hRecord, LPWSTR szResultBuf, LPDWORD pcchResultBuf);

/* ========================================================================== */
/* Package functions                                                          */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiVerifyPackageA(LPCSTR szPackagePath);
MSI_INTEROP_EXPORT UINT WINAPI MsiVerifyPackageW(LPCWSTR szPackagePath);

/* ========================================================================== */
/* File utility functions                                                     */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileVersionA(LPCSTR szFilePath, LPSTR lpVersionBuf, LPDWORD pcchVersionBuf, LPSTR lpLangBuf, LPDWORD pcchLangBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileVersionW(LPCWSTR szFilePath, LPWSTR lpVersionBuf, LPDWORD pcchVersionBuf, LPWSTR lpLangBuf, LPDWORD pcchLangBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileHashA(LPCSTR szFilePath, DWORD dwOptions, PMSIFILEHASHINFO pHash);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileHashW(LPCWSTR szFilePath, DWORD dwOptions, PMSIFILEHASHINFO pHash);

/* ========================================================================== */
/* NOT IMPLEMENTED - UI / Logging                                             */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT INSTALLUILEVEL WINAPI MsiSetInternalUI(INSTALLUILEVEL dwUILevel, HWND *phWnd);
MSI_INTEROP_EXPORT INSTALLUI_HANDLERA WINAPI MsiSetExternalUIA(INSTALLUI_HANDLERA puiHandler, DWORD dwMessageFilter, LPVOID pvContext);
MSI_INTEROP_EXPORT INSTALLUI_HANDLERW WINAPI MsiSetExternalUIW(INSTALLUI_HANDLERW puiHandler, DWORD dwMessageFilter, LPVOID pvContext);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetExternalUIRecord(INSTALLUI_HANDLER_RECORD puiHandler, DWORD dwMessageFilter, LPVOID pvContext, PINSTALLUI_HANDLER_RECORD ppuiPrevHandler);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnableLogA(DWORD dwLogMode, LPCSTR szLogFile, DWORD dwLogAttributes);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnableLogW(DWORD dwLogMode, LPCWSTR szLogFile, DWORD dwLogAttributes);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Product / Feature / Component state queries              */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryProductStateA(LPCSTR szProduct);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryProductStateW(LPCWSTR szProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoA(LPCSTR szProduct, LPCSTR szAttribute, LPSTR lpValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoW(LPCWSTR szProduct, LPCWSTR szAttribute, LPWSTR lpValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoExA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szProperty, LPSTR szValue, LPDWORD pcchValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoExW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szProperty, LPWSTR szValue, LPDWORD pcchValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductCodeA(LPCSTR szComponent, LPSTR lpBuf39);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductCodeW(LPCWSTR szComponent, LPWSTR lpBuf39);
MSI_INTEROP_EXPORT USERINFOSTATE WINAPI MsiGetUserInfoA(LPCSTR szProduct, LPSTR lpUserNameBuf, LPDWORD pcchUserNameBuf, LPSTR lpOrgNameBuf, LPDWORD pcchOrgNameBuf, LPSTR lpSerialBuf, LPDWORD pcchSerialBuf);
MSI_INTEROP_EXPORT USERINFOSTATE WINAPI MsiGetUserInfoW(LPCWSTR szProduct, LPWSTR lpUserNameBuf, LPDWORD pcchUserNameBuf, LPWSTR lpOrgNameBuf, LPDWORD pcchOrgNameBuf, LPWSTR lpSerialBuf, LPDWORD pcchSerialBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiCollectUserInfoA(LPCSTR szProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiCollectUserInfoW(LPCWSTR szProduct);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryFeatureStateA(LPCSTR szProduct, LPCSTR szFeature);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiQueryFeatureStateW(LPCWSTR szProduct, LPCWSTR szFeature);
MSI_INTEROP_EXPORT UINT WINAPI MsiQueryFeatureStateExA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szFeature, INSTALLSTATE *pdwState);
MSI_INTEROP_EXPORT UINT WINAPI MsiQueryFeatureStateExW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szFeature, INSTALLSTATE *pdwState);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureA(LPCSTR szProduct, LPCSTR szFeature);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureW(LPCWSTR szProduct, LPCWSTR szFeature);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureExA(LPCSTR szProduct, LPCSTR szFeature, DWORD dwInstallMode, DWORD dwReserved);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiUseFeatureExW(LPCWSTR szProduct, LPCWSTR szFeature, DWORD dwInstallMode, DWORD dwReserved);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureUsageA(LPCSTR szProduct, LPCSTR szFeature, LPDWORD pdwUseCount, LPWORD pwDateUsed);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureUsageW(LPCWSTR szProduct, LPCWSTR szFeature, LPDWORD pdwUseCount, LPWORD pwDateUsed);
MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureFeatureA(LPCSTR szProduct, LPCSTR szFeature, INSTALLSTATE eInstallState);
MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureFeatureW(LPCWSTR szProduct, LPCWSTR szFeature, INSTALLSTATE eInstallState);
MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallFeatureA(LPCSTR szProduct, LPCSTR szFeature, DWORD dwReinstallMode);
MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallFeatureW(LPCWSTR szProduct, LPCWSTR szFeature, DWORD dwReinstallMode);
MSI_INTEROP_EXPORT UINT WINAPI MsiQueryComponentStateA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szComponentCode, INSTALLSTATE *pdwState);
MSI_INTEROP_EXPORT UINT WINAPI MsiQueryComponentStateW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szComponentCode, INSTALLSTATE *pdwState);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Installation / Configuration                             */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiInstallProductA(LPCSTR szPackagePath, LPCSTR szCommandLine);
MSI_INTEROP_EXPORT UINT WINAPI MsiInstallProductW(LPCWSTR szPackagePath, LPCWSTR szCommandLine);
MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductA(LPCSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState);
MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductW(LPCWSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState);
MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductExA(LPCSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState, LPCSTR szCommandLine);
MSI_INTEROP_EXPORT UINT WINAPI MsiConfigureProductExW(LPCWSTR szProduct, int iInstallLevel, INSTALLSTATE eInstallState, LPCWSTR szCommandLine);
MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallProductA(LPCSTR szProduct, DWORD szReinstallMode);
MSI_INTEROP_EXPORT UINT WINAPI MsiReinstallProductW(LPCWSTR szProduct, DWORD szReinstallMode);
MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingComponentA(LPCSTR szProduct, LPCSTR szComponent, INSTALLSTATE eInstallState);
MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingComponentW(LPCWSTR szProduct, LPCWSTR szComponent, INSTALLSTATE eInstallState);
MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingFileA(LPCSTR szProduct, LPCSTR szFile);
MSI_INTEROP_EXPORT UINT WINAPI MsiInstallMissingFileW(LPCWSTR szProduct, LPCWSTR szFile);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Advertising                                              */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductA(LPCSTR szPackagePath, LPCSTR szScriptfilePath, LPCSTR szTransforms, LANGID lgidLanguage);
MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductW(LPCWSTR szPackagePath, LPCWSTR szScriptfilePath, LPCWSTR szTransforms, LANGID lgidLanguage);
MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductExA(LPCSTR szPackagePath, LPCSTR szScriptfilePath, LPCSTR szTransforms, LANGID lgidLanguage, DWORD dwPlatform, DWORD dwOptions);
MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseProductExW(LPCWSTR szPackagePath, LPCWSTR szScriptfilePath, LPCWSTR szTransforms, LANGID lgidLanguage, DWORD dwPlatform, DWORD dwOptions);
MSI_INTEROP_EXPORT UINT WINAPI MsiProcessAdvertiseScriptA(LPCSTR szScriptFile, LPCSTR szIconFolder, HKEY hRegData, BOOL fShortcuts, BOOL fRemoveItems);
MSI_INTEROP_EXPORT UINT WINAPI MsiProcessAdvertiseScriptW(LPCWSTR szScriptFile, LPCWSTR szIconFolder, HKEY hRegData, BOOL fShortcuts, BOOL fRemoveItems);
MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseScriptA(LPCSTR szScriptFile, DWORD dwFlags, PHKEY phRegData, BOOL fRemoveItems);
MSI_INTEROP_EXPORT UINT WINAPI MsiAdvertiseScriptW(LPCWSTR szScriptFile, DWORD dwFlags, PHKEY phRegData, BOOL fRemoveItems);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoFromScriptA(LPCSTR szScriptFile, LPSTR lpProductBuf39, LPWORD plgidLanguage, LPDWORD pdwVersion, LPSTR lpNameBuf, LPDWORD pcchNameBuf, LPSTR lpPackageBuf, LPDWORD pcchPackageBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductInfoFromScriptW(LPCWSTR szScriptFile, LPWSTR lpProductBuf39, LPWORD plgidLanguage, LPDWORD pdwVersion, LPWSTR lpNameBuf, LPDWORD pcchNameBuf, LPWSTR lpPackageBuf, LPDWORD pcchPackageBuf);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Patch management                                         */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiApplyPatchA(LPCSTR szPatchPackage, LPCSTR szInstallPackage, INSTALLTYPE eInstallType, LPCSTR szCommandLine);
MSI_INTEROP_EXPORT UINT WINAPI MsiApplyPatchW(LPCWSTR szPatchPackage, LPCWSTR szInstallPackage, INSTALLTYPE eInstallType, LPCWSTR szCommandLine);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoA(LPCSTR szPatch, LPCSTR szAttribute, LPSTR lpValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoW(LPCWSTR szPatch, LPCWSTR szAttribute, LPWSTR lpValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesA(LPCSTR szProduct, DWORD iPatchIndex, LPSTR lpPatchBuf, LPSTR lpTransformsBuf, LPDWORD pcchTransformsBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesW(LPCWSTR szProduct, DWORD iPatchIndex, LPWSTR lpPatchBuf, LPWSTR lpTransformsBuf, LPDWORD pcchTransformsBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiRemovePatchesA(LPCSTR szPatchList, LPCSTR szProductCode, INSTALLTYPE eUninstallType, LPCSTR szPropertyList);
MSI_INTEROP_EXPORT UINT WINAPI MsiRemovePatchesW(LPCWSTR szPatchList, LPCWSTR szProductCode, INSTALLTYPE eUninstallType, LPCWSTR szPropertyList);
MSI_INTEROP_EXPORT UINT WINAPI MsiExtractPatchXMLDataA(LPCSTR szPatchPath, DWORD dwReserved, LPSTR szXMLData, LPDWORD pcchXMLData);
MSI_INTEROP_EXPORT UINT WINAPI MsiExtractPatchXMLDataW(LPCWSTR szPatchPath, DWORD dwReserved, LPWSTR szXMLData, LPDWORD pcchXMLData);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoExA(LPCSTR szPatchCode, LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCSTR szProperty, LPSTR lpValue, LPDWORD pcchValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchInfoExW(LPCWSTR szPatchCode, LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPCWSTR szProperty, LPWSTR lpValue, LPDWORD pcchValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiApplyMultiplePatchesA(LPCSTR szPatchPackages, LPCSTR szProductCode, LPCSTR szPropertiesList);
MSI_INTEROP_EXPORT UINT WINAPI MsiApplyMultiplePatchesW(LPCWSTR szPatchPackages, LPCWSTR szProductCode, LPCWSTR szPropertiesList);
MSI_INTEROP_EXPORT UINT WINAPI MsiDeterminePatchSequenceA(LPCSTR szProductCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOA pPatchInfo);
MSI_INTEROP_EXPORT UINT WINAPI MsiDeterminePatchSequenceW(LPCWSTR szProductCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOW pPatchInfo);
MSI_INTEROP_EXPORT UINT WINAPI MsiDetermineApplicablePatchesA(LPCSTR szProductPackagePath, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOA pPatchInfo);
MSI_INTEROP_EXPORT UINT WINAPI MsiDetermineApplicablePatchesW(LPCWSTR szProductPackagePath, DWORD cPatchInfo, PMSIPATCHSEQUENCEINFOW pPatchInfo);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesExA(LPCSTR szProductCode, LPCSTR szUserSid, DWORD dwContext, DWORD dwFilter, DWORD dwIndex, LPSTR szPatchCode, LPSTR szTargetProductCode, MSIINSTALLCONTEXT *pdwTargetProductContext, LPSTR szTargetUserSid, LPDWORD pcchTargetUserSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumPatchesExW(LPCWSTR szProductCode, LPCWSTR szUserSid, DWORD dwContext, DWORD dwFilter, DWORD dwIndex, LPWSTR szPatchCode, LPWSTR szTargetProductCode, MSIINSTALLCONTEXT *pdwTargetProductContext, LPWSTR szTargetUserSid, LPDWORD pcchTargetUserSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchFileListA(LPCSTR szProductCode, LPCSTR szPatchPackages, LPDWORD pcFiles, MSIHANDLE **pphFileRecords);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPatchFileListW(LPCWSTR szProductCode, LPCWSTR szPatchPackages, LPDWORD pcFiles, MSIHANDLE **pphFileRecords);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Component provisioning / location                        */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideComponentA(LPCSTR szProduct, LPCSTR szFeature, LPCSTR szComponent, DWORD dwInstallMode, LPSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideComponentW(LPCWSTR szProduct, LPCWSTR szFeature, LPCWSTR szComponent, DWORD dwInstallMode, LPWSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentA(LPCSTR szCategory, LPCSTR szQualifier, DWORD dwInstallMode, LPSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentW(LPCWSTR szCategory, LPCWSTR szQualifier, DWORD dwInstallMode, LPWSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentExA(LPCSTR szCategory, LPCSTR szQualifier, DWORD dwInstallMode, LPCSTR szProduct, DWORD dwUnused1, DWORD dwUnused2, LPSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideQualifiedComponentExW(LPCWSTR szCategory, LPCWSTR szQualifier, DWORD dwInstallMode, LPCWSTR szProduct, DWORD dwUnused1, DWORD dwUnused2, LPWSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathA(LPCSTR szProduct, LPCSTR szComponent, LPSTR lpPathBuf, LPDWORD pcchBuf);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathW(LPCWSTR szProduct, LPCWSTR szComponent, LPWSTR lpPathBuf, LPDWORD pcchBuf);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathExA(LPCSTR szProductCode, LPCSTR szComponentCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPSTR lpOutPathBuf, LPDWORD pcchOutPathBuf);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiGetComponentPathExW(LPCWSTR szProductCode, LPCWSTR szComponentCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, LPWSTR lpOutPathBuf, LPDWORD pcchOutPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideAssemblyA(LPCSTR szAssemblyName, LPCSTR szAppContext, DWORD dwInstallMode, DWORD dwAssemblyInfo, LPSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiProvideAssemblyW(LPCWSTR szAssemblyName, LPCWSTR szAppContext, DWORD dwInstallMode, DWORD dwAssemblyInfo, LPWSTR lpPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiLocateComponentA(LPCSTR szComponent, LPSTR lpPathBuf, LPDWORD pcchBuf);
MSI_INTEROP_EXPORT INSTALLSTATE WINAPI MsiLocateComponentW(LPCWSTR szComponent, LPWSTR lpPathBuf, LPDWORD pcchBuf);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Enumeration                                              */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsA(DWORD iProductIndex, LPSTR lpProductBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsW(DWORD iProductIndex, LPWSTR lpProductBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsExA(LPCSTR szProductCode, LPCSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPSTR szInstalledProductCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPSTR szSid, LPDWORD pcchSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumProductsExW(LPCWSTR szProductCode, LPCWSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPWSTR szInstalledProductCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPWSTR szSid, LPDWORD pcchSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumRelatedProductsA(LPCSTR lpUpgradeCode, DWORD dwReserved, DWORD iProductIndex, LPSTR lpProductBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumRelatedProductsW(LPCWSTR lpUpgradeCode, DWORD dwReserved, DWORD iProductIndex, LPWSTR lpProductBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumFeaturesA(LPCSTR szProduct, DWORD iFeatureIndex, LPSTR lpFeatureBuf, LPSTR lpParentBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumFeaturesW(LPCWSTR szProduct, DWORD iFeatureIndex, LPWSTR lpFeatureBuf, LPWSTR lpParentBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsA(DWORD iComponentIndex, LPSTR lpComponentBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsW(DWORD iComponentIndex, LPWSTR lpComponentBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsExA(LPCSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPSTR szInstalledComponentCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPSTR szSid, LPDWORD pcchSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentsExW(LPCWSTR szUserSid, DWORD dwContext, DWORD dwIndex, LPWSTR szInstalledComponentCode, MSIINSTALLCONTEXT *pdwInstalledContext, LPWSTR szSid, LPDWORD pcchSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsA(LPCSTR szComponent, DWORD iProductIndex, LPSTR lpProductBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsW(LPCWSTR szComponent, DWORD iProductIndex, LPWSTR lpProductBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsExA(LPCSTR szComponent, LPCSTR szUserSid, DWORD dwContext, DWORD dwProductIndex, LPSTR szProductBuf, MSIINSTALLCONTEXT *pdwInstalledContext, LPSTR szSid, LPDWORD pcchSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumClientsExW(LPCWSTR szComponent, LPCWSTR szUserSid, DWORD dwContext, DWORD dwProductIndex, LPWSTR szProductBuf, MSIINSTALLCONTEXT *pdwInstalledContext, LPWSTR szSid, LPDWORD pcchSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentQualifiersA(LPCSTR szComponent, DWORD iIndex, LPSTR lpQualifierBuf, LPDWORD pcchQualifierBuf, LPSTR lpApplicationDataBuf, LPDWORD pcchApplicationDataBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentQualifiersW(LPCWSTR szComponent, DWORD iIndex, LPWSTR lpQualifierBuf, LPDWORD pcchQualifierBuf, LPWSTR lpApplicationDataBuf, LPDWORD pcchApplicationDataBuf);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Package / Product opening                                */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiOpenProductA(LPCSTR szProduct, MSIHANDLE *phProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiOpenProductW(LPCWSTR szProduct, MSIHANDLE *phProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageA(LPCSTR szPackagePath, MSIHANDLE *hProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageW(LPCWSTR szPackagePath, MSIHANDLE *hProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageExA(LPCSTR szPackagePath, DWORD dwOptions, MSIHANDLE *hProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiOpenPackageExW(LPCWSTR szPackagePath, DWORD dwOptions, MSIHANDLE *hProduct);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductPropertyA(MSIHANDLE hProduct, LPCSTR szProperty, LPSTR lpValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetProductPropertyW(MSIHANDLE hProduct, LPCWSTR szProperty, LPWSTR lpValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureInfoA(MSIHANDLE hProduct, LPCSTR szFeature, LPDWORD lpAttributes, LPSTR lpTitleBuf, LPDWORD pcchTitleBuf, LPSTR lpHelpBuf, LPDWORD pcchHelpBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureInfoW(MSIHANDLE hProduct, LPCWSTR szFeature, LPDWORD lpAttributes, LPWSTR lpTitleBuf, LPDWORD pcchTitleBuf, LPWSTR lpHelpBuf, LPDWORD pcchHelpBuf);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Source list management                                   */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllA(LPCSTR szProduct, LPCSTR szUserName, DWORD dwReserved);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllW(LPCWSTR szProduct, LPCWSTR szUserName, DWORD dwReserved);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceA(LPCSTR szProduct, LPCSTR szUserName, DWORD dwReserved, LPCSTR szSource);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceW(LPCWSTR szProduct, LPCWSTR szUserName, DWORD dwReserved, LPCWSTR szSource);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionA(LPCSTR szProduct, LPCSTR szUserName, DWORD dwReserved);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionW(LPCWSTR szProduct, LPCWSTR szUserName, DWORD dwReserved);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceExA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szSource, DWORD dwIndex);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddSourceExW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szSource, DWORD dwIndex);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddMediaDiskA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId, LPCSTR szVolumeLabel, LPCSTR szDiskPrompt);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListAddMediaDiskW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId, LPCWSTR szVolumeLabel, LPCWSTR szDiskPrompt);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearSourceA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szSource);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearSourceW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szSource);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearMediaDiskA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearMediaDiskW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwDiskId);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllExA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListClearAllExW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionExA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListForceResolutionExW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListSetInfoA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szProperty, LPCSTR szValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListSetInfoW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szProperty, LPCWSTR szValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListGetInfoA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCSTR szProperty, LPSTR szValue, LPDWORD pcchValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListGetInfoW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, LPCWSTR szProperty, LPWSTR szValue, LPDWORD pcchValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumSourcesA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPSTR szSource, LPDWORD pcchSource);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumSourcesW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPWSTR szSource, LPDWORD pcchSource);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumMediaDisksA(LPCSTR szProductCodeOrPatchCode, LPCSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPDWORD pdwDiskId, LPSTR szVolumeLabel, LPDWORD pcchVolumeLabel, LPSTR szDiskPrompt, LPDWORD pcchDiskPrompt);
MSI_INTEROP_EXPORT UINT WINAPI MsiSourceListEnumMediaDisksW(LPCWSTR szProductCodeOrPatchCode, LPCWSTR szUserSid, MSIINSTALLCONTEXT dwContext, DWORD dwOptions, DWORD dwIndex, LPDWORD pdwDiskId, LPWSTR szVolumeLabel, LPDWORD pcchVolumeLabel, LPWSTR szDiskPrompt, LPDWORD pcchDiskPrompt);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Remaining utility functions                              */
/* Requires Windows Installer Service or Windows-specific functionality       */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT HRESULT WINAPI MsiGetFileSignatureInformationA(LPCSTR szSignedObjectPath, DWORD dwFlags, LPVOID *ppCertContext, LPBYTE pbHashData, LPDWORD pcbHashData);
MSI_INTEROP_EXPORT HRESULT WINAPI MsiGetFileSignatureInformationW(LPCWSTR szSignedObjectPath, DWORD dwFlags, LPVOID *ppCertContext, LPBYTE pbHashData, LPDWORD pcbHashData);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetShortcutTargetA(LPCSTR szShortcutPath, LPSTR szProductCode, LPSTR szFeatureId, LPSTR szComponentCode);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetShortcutTargetW(LPCWSTR szShortcutPath, LPWSTR szProductCode, LPWSTR szFeatureId, LPWSTR szComponentCode);
MSI_INTEROP_EXPORT UINT WINAPI MsiIsProductElevatedA(LPCSTR szProduct, BOOL *pfElevated);
MSI_INTEROP_EXPORT UINT WINAPI MsiIsProductElevatedW(LPCWSTR szProduct, BOOL *pfElevated);
MSI_INTEROP_EXPORT UINT WINAPI MsiNotifySidChangeA(LPCSTR pOldSid, LPCSTR pNewSid);
MSI_INTEROP_EXPORT UINT WINAPI MsiNotifySidChangeW(LPCWSTR pOldSid, LPCWSTR pNewSid);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Transaction management                                   */
/* Requires Windows Installer Service                                         */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiBeginTransactionA(LPCSTR szName, DWORD dwTransactionAttributes, MSIHANDLE *phTransactionHandle, HANDLE *phChangeOfOwnerEvent);
MSI_INTEROP_EXPORT UINT WINAPI MsiBeginTransactionW(LPCWSTR szName, DWORD dwTransactionAttributes, MSIHANDLE *phTransactionHandle, HANDLE *phChangeOfOwnerEvent);
MSI_INTEROP_EXPORT UINT WINAPI MsiEndTransaction(DWORD dwTransactionState);
MSI_INTEROP_EXPORT UINT WINAPI MsiJoinTransaction(MSIHANDLE hTransactionHandle, DWORD dwTransactionAttributes, HANDLE *phChangeOfOwnerEvent);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Install session functions (MsiQuery.h)                   */
/* Requires an active installation session                                    */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT MSIHANDLE WINAPI MsiGetActiveDatabase(MSIHANDLE hInstall);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetPropertyA(MSIHANDLE hInstall, LPCSTR szName, LPCSTR szValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetPropertyW(MSIHANDLE hInstall, LPCWSTR szName, LPCWSTR szValue);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPropertyA(MSIHANDLE hInstall, LPCSTR szName, LPSTR szValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetPropertyW(MSIHANDLE hInstall, LPCWSTR szName, LPWSTR szValueBuf, LPDWORD pcchValueBuf);
MSI_INTEROP_EXPORT LANGID WINAPI MsiGetLanguage(MSIHANDLE hInstall);
MSI_INTEROP_EXPORT BOOL WINAPI MsiGetMode(MSIHANDLE hInstall, MSIRUNMODE eRunMode);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetMode(MSIHANDLE hInstall, MSIRUNMODE eRunMode, BOOL fState);
MSI_INTEROP_EXPORT UINT WINAPI MsiDoActionA(MSIHANDLE hInstall, LPCSTR szAction);
MSI_INTEROP_EXPORT UINT WINAPI MsiDoActionW(MSIHANDLE hInstall, LPCWSTR szAction);
MSI_INTEROP_EXPORT UINT WINAPI MsiSequenceA(MSIHANDLE hInstall, LPCSTR szTable, int iSequenceMode);
MSI_INTEROP_EXPORT UINT WINAPI MsiSequenceW(MSIHANDLE hInstall, LPCWSTR szTable, int iSequenceMode);
MSI_INTEROP_EXPORT int WINAPI MsiProcessMessage(MSIHANDLE hInstall, INSTALLMESSAGE eMessageType, MSIHANDLE hRecord);
MSI_INTEROP_EXPORT MSICONDITION WINAPI MsiEvaluateConditionA(MSIHANDLE hInstall, LPCSTR szCondition);
MSI_INTEROP_EXPORT MSICONDITION WINAPI MsiEvaluateConditionW(MSIHANDLE hInstall, LPCWSTR szCondition);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Feature / Component state (session)                      */
/* Requires an active installation session                                    */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureStateA(MSIHANDLE hInstall, LPCSTR szFeature, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureStateW(MSIHANDLE hInstall, LPCWSTR szFeature, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureStateA(MSIHANDLE hInstall, LPCSTR szFeature, INSTALLSTATE iState);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureStateW(MSIHANDLE hInstall, LPCWSTR szFeature, INSTALLSTATE iState);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureAttributesA(MSIHANDLE hInstall, LPCSTR szFeature, DWORD dwAttributes);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetFeatureAttributesW(MSIHANDLE hInstall, LPCWSTR szFeature, DWORD dwAttributes);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetComponentStateA(MSIHANDLE hInstall, LPCSTR szComponent, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetComponentStateW(MSIHANDLE hInstall, LPCWSTR szComponent, INSTALLSTATE *piInstalled, INSTALLSTATE *piAction);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetComponentStateA(MSIHANDLE hInstall, LPCSTR szComponent, INSTALLSTATE iState);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetComponentStateW(MSIHANDLE hInstall, LPCWSTR szComponent, INSTALLSTATE iState);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Feature / Component cost and paths (session)             */
/* Requires an active installation session                                    */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureCostA(MSIHANDLE hInstall, LPCSTR szFeature, MSICOSTTREE iCostTree, INSTALLSTATE iState, LPINT piCost);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureCostW(MSIHANDLE hInstall, LPCWSTR szFeature, MSICOSTTREE iCostTree, INSTALLSTATE iState, LPINT piCost);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentCostsA(MSIHANDLE hInstall, LPCSTR szComponent, DWORD dwIndex, INSTALLSTATE iState, LPSTR szDriveBuf, LPDWORD pcchDriveBuf, LPINT piCost, LPINT piTempCost);
MSI_INTEROP_EXPORT UINT WINAPI MsiEnumComponentCostsW(MSIHANDLE hInstall, LPCWSTR szComponent, DWORD dwIndex, INSTALLSTATE iState, LPWSTR szDriveBuf, LPDWORD pcchDriveBuf, LPINT piCost, LPINT piTempCost);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetInstallLevel(MSIHANDLE hInstall, int iInstallLevel);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureValidStatesA(MSIHANDLE hInstall, LPCSTR szFeature, LPDWORD lpInstallStates);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetFeatureValidStatesW(MSIHANDLE hInstall, LPCWSTR szFeature, LPDWORD lpInstallStates);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetSourcePathA(MSIHANDLE hInstall, LPCSTR szFolder, LPSTR szPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetSourcePathW(MSIHANDLE hInstall, LPCWSTR szFolder, LPWSTR szPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetTargetPathA(MSIHANDLE hInstall, LPCSTR szFolder, LPSTR szPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiGetTargetPathW(MSIHANDLE hInstall, LPCWSTR szFolder, LPWSTR szPathBuf, LPDWORD pcchPathBuf);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetTargetPathA(MSIHANDLE hInstall, LPCSTR szFolder, LPCSTR szFolderPath);
MSI_INTEROP_EXPORT UINT WINAPI MsiSetTargetPathW(MSIHANDLE hInstall, LPCWSTR szFolder, LPCWSTR szFolderPath);
MSI_INTEROP_EXPORT UINT WINAPI MsiVerifyDiskSpace(MSIHANDLE hInstall);
*/

/* ========================================================================== */
/* NOT IMPLEMENTED - Preview / Dialog (session)                               */
/* Requires an active installation session                                    */
/* ========================================================================== */

/* NOT IMPLEMENTED
MSI_INTEROP_EXPORT UINT WINAPI MsiEnableUIPreview(MSIHANDLE hDatabase, MSIHANDLE *phPreview);
MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewDialogA(MSIHANDLE hPreview, LPCSTR szDialogName);
MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewDialogW(MSIHANDLE hPreview, LPCWSTR szDialogName);
MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewBillboardA(MSIHANDLE hPreview, LPCSTR szControlName, LPCSTR szBillboard);
MSI_INTEROP_EXPORT UINT WINAPI MsiPreviewBillboardW(MSIHANDLE hPreview, LPCWSTR szControlName, LPCWSTR szBillboard);
*/

#ifdef __cplusplus
}
#endif

#endif /* MSI_INTEROP_H */
