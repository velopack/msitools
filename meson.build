project('wixl-standalone', 'c', 'vala',
  version: '0.103',
  meson_version: '>= 0.62',
  default_options: [
    'default_library=static',
    'prefer_static=true',
    'warning_level=0',
  ],
)

gnome = import('gnome')

# --- External dependencies (static) ---
glib    = dependency('glib-2.0',    version: '>= 2.12', static: true)
gobject = dependency('gobject-2.0', version: '>= 0.9.4', static: true)
gio     = dependency('gio-2.0',     version: '>= 2.14', static: true)
zlib    = dependency('zlib',        static: true)

add_project_arguments('-w', language: 'c')

# MSYS2 CLANGARM64 has intermittent false-positive stack-protector failures,
# likely due to __stack_chk_guard TLS initialization ordering in the toolchain.
if meson.get_compiler('c').get_id() == 'clang' and host_machine.cpu_family() == 'aarch64'
  add_project_arguments('-fno-stack-protector', language: 'c')
endif

if host_machine.system() == 'windows'
  add_project_arguments(
    '-DGLIB_STATIC_COMPILATION',
    '-DGOBJECT_STATIC_COMPILATION',
    '-DGIO_STATIC_COMPILATION',
    '-DGMODULE_STATIC_COMPILATION',
    '-DLIBXML_STATIC',
    language: 'c',
  )
endif

# --- libxml2 ---
# Build from source on macOS (Homebrew doesn't ship static libxml2.a) and
# Linux (Alpine's libxml2.a isn't compiled with -fPIC, so it can't be embedded
# into a shared object like libmsi-interop.so).
if host_machine.system() == 'darwin' or host_machine.system() == 'linux'
  cc = meson.get_compiler('c')

  xml2_version_config = configuration_data()
  xml2_version_config.set('VERSION', '2.16.0')
  xml2_version_config.set('LIBXML_VERSION_NUMBER', '21600')
  xml2_version_config.set('LIBXML_VERSION_EXTRA', '')
  xml2_version_config.set('WITH_THREADS', 1)
  xml2_version_config.set('WITH_THREAD_ALLOC', 0)
  xml2_version_config.set('WITH_OUTPUT', 1)
  xml2_version_config.set('WITH_PUSH', 1)
  xml2_version_config.set('WITH_READER', 1)
  xml2_version_config.set('WITH_PATTERN', 0)
  xml2_version_config.set('WITH_WRITER', 1)
  xml2_version_config.set('WITH_SAX1', 0)
  xml2_version_config.set('WITH_HTTP', 0)
  xml2_version_config.set('WITH_VALID', 1)
  xml2_version_config.set('WITH_HTML', 0)
  xml2_version_config.set('WITH_C14N', 0)
  xml2_version_config.set('WITH_CATALOG', 0)
  xml2_version_config.set('WITH_WINPATH', 0)
  xml2_version_config.set('WITH_XPATH', 0)
  xml2_version_config.set('WITH_XPTR', 0)
  xml2_version_config.set('WITH_XINCLUDE', 0)
  xml2_version_config.set('WITH_ICONV', 1)
  xml2_version_config.set('WITH_ICU', 0)
  xml2_version_config.set('WITH_ISO8859X', 1)
  xml2_version_config.set('WITH_DEBUG', 0)
  xml2_version_config.set('WITH_REGEXPS', 0)
  xml2_version_config.set('WITH_RELAXNG', 0)
  xml2_version_config.set('WITH_SCHEMAS', 0)
  xml2_version_config.set('WITH_SCHEMATRON', 0)
  xml2_version_config.set('WITH_MODULES', 0)
  xml2_version_config.set('WITH_ZLIB', 1)
  xml2_version_config.set('MODULE_EXTENSION', '')

  xml2_version_h = configure_file(
    input: 'libxml2/include/libxml/xmlversion.h.in',
    output: 'xmlversion.h',
    configuration: xml2_version_config,
  )

  run_command('mkdir', '-p', meson.project_build_root() / 'libxml', check: true)
  run_command('cp', meson.project_build_root() / 'xmlversion.h',
    meson.project_build_root() / 'libxml' / 'xmlversion.h', check: true)

  xml2_config = configuration_data()
  xml2_config.set('HAVE_STDINT_H', 1)
  xml2_config.set('HAVE_DECL_GETENTROPY', cc.has_function('getentropy', prefix: '#include <sys/random.h>') ? 1 : 0)
  xml2_config.set('HAVE_DECL_GLOB', cc.has_function('glob', prefix: '#include <glob.h>') ? 1 : 0)
  xml2_config.set('HAVE_DECL_MMAP', cc.has_function('mmap', prefix: '#include <sys/mman.h>') ? 1 : 0)
  xml2_config.set('HAVE_FUNC_ATTRIBUTE_DESTRUCTOR', cc.compiles('void __attribute__((destructor)) f(void) {} int main(void) { return 0; }') ? 1 : false)
  xml2_config.set('HAVE_DLOPEN', cc.has_function('dlopen') ? 1 : false)
  xml2_config.set('XML_THREAD_LOCAL', '_Thread_local')

  xml2_config_h = configure_file(
    output: 'xml2-config.h',
    configuration: xml2_config,
  )

  libxml2 = static_library('xml2',
    'libxml2/buf.c',
    'libxml2/chvalid.c',
    'libxml2/dict.c',
    'libxml2/encoding.c',
    'libxml2/entities.c',
    'libxml2/error.c',
    'libxml2/globals.c',
    'libxml2/hash.c',
    'libxml2/list.c',
    'libxml2/parser.c',
    'libxml2/parserInternals.c',
    'libxml2/SAX2.c',
    'libxml2/threads.c',
    'libxml2/tree.c',
    'libxml2/uri.c',
    'libxml2/valid.c',
    'libxml2/xmlIO.c',
    'libxml2/xmlmemory.c',
    'libxml2/xmlstring.c',
    'libxml2/xmlsave.c',
    'libxml2/xmlreader.c',
    'libxml2/xmlwriter.c',
    xml2_config_h,
    xml2_version_h,
    include_directories: include_directories('libxml2', 'libxml2/include'),
    dependencies: [zlib],
    c_args: ['-include', meson.project_build_root() / 'xml2-config.h',
             '-I' + meson.project_build_root()],
    pic: true,
  )

  libxml = declare_dependency(
    link_with: libxml2,
    include_directories: include_directories('libxml2', 'libxml2/include'),
    dependencies: [zlib],
    compile_args: ['-I' + meson.project_build_root()],
  )
else
  libxml = dependency('libxml-2.0', version: '>= 2.7', static: true)
endif

bison = find_program('bison')

# --- config.h ---
config = configuration_data()
config.set_quoted('PACKAGE_NAME', 'msitools')
config.set_quoted('PACKAGE_VERSION', meson.project_version())
config.set_quoted('PACKAGE_STRING', 'msitools ' + meson.project_version())
config.set_quoted('GETTEXT_PACKAGE', 'msitools')
config.set_quoted('PACKAGE_BUGREPORT', 'https://gitlab.gnome.org/GNOME/msitools/issues')
config.set_quoted('LOCALEDIR', '')
config.set_quoted('DATADIR', '.')

config_h = configure_file(
  output: 'config.h',
  configuration: config,
)

# --- gsf-config.h (for libgsf) ---
gsf_config = configuration_data()
gsf_config.set_quoted('GETTEXT_PACKAGE', 'libgsf')
gsf_config.set('GLIB_VERSION_MIN_REQUIRED', 'GLIB_VERSION_2_62')
gsf_config.set('GLIB_VERSION_MAX_ALLOWED', 'GLIB_VERSION_2_62')
gsf_config.set_quoted('GSFLOCALEDIR', '')

gsf_config_h = configure_file(
  output: 'gsf-config.h',
  configuration: gsf_config,
)

# --- libgsf version.c ---
gsf_version = configuration_data()
gsf_version.set('LIBGSF_MAJOR_VERSION', '1')
gsf_version.set('LIBGSF_MINOR_VERSION', '14')
gsf_version.set('LIBGSF_MICRO_VERSION', '59')

gsf_version_c = configure_file(
  input: 'libgsf/gsf/version.c.in',
  output: 'gsf-version.c',
  configuration: gsf_version,
)

# --- libgsf (static, built from source) ---
gsf_inc_dirs = include_directories(
  'libgsf',
  'libgsf/gsf',
)

libgsf_sources = files(
  'libgsf/gsf/gsf-utils.c',
  'libgsf/gsf/gsf-priv.c',
  'libgsf/gsf/gsf-libxml.c',
  'libgsf/gsf/gsf-doc-meta-data.c',
  'libgsf/gsf/gsf-docprop-vector.c',
  'libgsf/gsf/gsf-msole-utils.c',
  'libgsf/gsf/gsf-open-pkg-utils.c',
  'libgsf/gsf/gsf-opendoc-utils.c',
  'libgsf/gsf/gsf-timestamp.c',
  'libgsf/gsf/gsf-zip-utils.c',
  'libgsf/gsf/gsf-input.c',
  'libgsf/gsf/gsf-input-gzip.c',
  'libgsf/gsf/gsf-input-http.c',
  'libgsf/gsf/gsf-input-iochannel.c',
  'libgsf/gsf/gsf-input-memory.c',
  'libgsf/gsf/gsf-input-proxy.c',
  'libgsf/gsf/gsf-input-stdio.c',
  'libgsf/gsf/gsf-input-textline.c',
  'libgsf/gsf/gsf-infile.c',
  'libgsf/gsf/gsf-infile-msole.c',
  'libgsf/gsf/gsf-infile-msvba.c',
  'libgsf/gsf/gsf-infile-stdio.c',
  'libgsf/gsf/gsf-infile-tar.c',
  'libgsf/gsf/gsf-infile-zip.c',
  'libgsf/gsf/gsf-output.c',
  'libgsf/gsf/gsf-output-csv.c',
  'libgsf/gsf/gsf-output-gzip.c',
  'libgsf/gsf/gsf-output-iconv.c',
  'libgsf/gsf/gsf-output-iochannel.c',
  'libgsf/gsf/gsf-output-memory.c',
  'libgsf/gsf/gsf-output-stdio.c',
  'libgsf/gsf/gsf-outfile.c',
  'libgsf/gsf/gsf-outfile-msole.c',
  'libgsf/gsf/gsf-outfile-stdio.c',
  'libgsf/gsf/gsf-outfile-zip.c',
  'libgsf/gsf/gsf-shared-memory.c',
  'libgsf/gsf/gsf-structured-blob.c',
  'libgsf/gsf/gsf-blob.c',
  'libgsf/gsf/gsf-clip-data.c',
  'libgsf/gsf/gsf-input-bzip.c',
  'libgsf/gsf/gsf-output-bzip.c',
  'libgsf/gsf/gsf-input-gio.c',
  'libgsf/gsf/gsf-output-gio.c',
)

libgsf_deps = [glib, gobject, gio, libxml, zlib]

libgsf = static_library('gsf-1',
  libgsf_sources,
  gsf_version_c,
  gsf_config_h,
  include_directories: gsf_inc_dirs,
  dependencies: libgsf_deps,
  c_args: [
    '-DG_LOG_DOMAIN="libgsf"',
    '-DGSF_DISABLE_DEPRECATED',
  ],
  pic: true,
)

libgsf_dep = declare_dependency(
  link_with: libgsf,
  include_directories: gsf_inc_dirs,
  dependencies: libgsf_deps,
)

# --- Include directories ---
inc_dirs = include_directories(
  'msitools/include',
  'msitools/libmsi',
)

gcab_inc_dirs = include_directories(
  'gcab/libgcab',
  'gcab',
)

# --- libgcab (static, built from source) ---
gcab_enum_headers = files(
  'gcab/libgcab/gcab-folder.h',
  'gcab/libgcab/gcab-file.h',
)

gcab_enums = gnome.mkenums('gcab-enums',
  h_template: 'gcab/libgcab/gcab-enums.h.etemplate',
  c_template: 'gcab/libgcab/gcab-enums.c.etemplate',
  sources: gcab_enum_headers,
  identifier_prefix: 'GCab',
  symbol_prefix: 'gcab',
)

gcab_deps = [gio, zlib]

libgcab = static_library('gcab-1.0',
  'gcab/libgcab/gcab-cabinet.c',
  'gcab/libgcab/gcab-folder.c',
  'gcab/libgcab/gcab-file.c',
  'gcab/libgcab/cabinet.c',
  'gcab/libgcab/decomp.c',
  gcab_enums,
  config_h,
  include_directories: gcab_inc_dirs,
  dependencies: gcab_deps,
  c_args: [
    '-DG_LOG_DOMAIN="gcab"',
    '-include', meson.project_build_root() / 'config.h',
  ],
)

libgcab_dep = declare_dependency(
  link_with: libgcab,
  include_directories: gcab_inc_dirs,
  dependencies: gcab_deps,
)

# --- libmsi enum generation (uses glib-mkenums, NOT introspection) ---
libmsi_types_h = files('msitools/include/libmsi-types.h')

libmsi_enums = gnome.mkenums('libmsi-enums',
  h_template: 'msitools/include/libmsi-enums.h.etemplate',
  c_template: 'msitools/libmsi/libmsi-enums.c.etemplate',
  sources: libmsi_types_h,
)

# --- Bison SQL parser ---
sql_parser = custom_target('sql-parser',
  input: 'msitools/libmsi/sql-parser.y',
  output: ['sql-parser.c', 'sql-parser.h'],
  command: [bison, '@INPUT@', '-y', '-Wno-yacc', '--defines', '-o', '@OUTPUT0@'],
)

# --- libmsi (static) ---
libmsi_sources = files(
  'msitools/libmsi/alter.c',
  'msitools/libmsi/create.c',
  'msitools/libmsi/debug.c',
  'msitools/libmsi/delete.c',
  'msitools/libmsi/distinct.c',
  'msitools/libmsi/drop.c',
  'msitools/libmsi/insert.c',
  'msitools/libmsi/libmsi-database.c',
  'msitools/libmsi/libmsi-istream.c',
  'msitools/libmsi/libmsi-query.c',
  'msitools/libmsi/libmsi-record.c',
  'msitools/libmsi/libmsi-summary-info.c',
  'msitools/libmsi/select.c',
  'msitools/libmsi/storages.c',
  'msitools/libmsi/streams.c',
  'msitools/libmsi/string.c',
  'msitools/libmsi/table.c',
  'msitools/libmsi/tokenize.c',
  'msitools/libmsi/update.c',
  'msitools/libmsi/where.c',
)

libmsi_deps = [glib, gio, gobject, libgsf_dep]

libmsi = static_library('msi-1.0',
  libmsi_sources,
  libmsi_enums,
  sql_parser,
  config_h,
  include_directories: inc_dirs,
  dependencies: libmsi_deps,
  c_args: ['-include', meson.project_build_root() / 'config.h'],
  pic: true,
)

libmsi_dep = declare_dependency(
  link_with: libmsi,
  include_directories: inc_dirs,
  dependencies: libmsi_deps,
)

# --- libmsi shared library ---
libmsi_shared_link_args = []
if host_machine.system() == 'windows'
  libmsi_shared_link_args = [
    '-lws2_32', '-lole32', '-lshell32', '-lshlwapi',
    '-ladvapi32', '-luserenv', '-liphlpapi', '-ldnsapi',
    '-lbcrypt',
  ]
elif host_machine.system() == 'darwin'
  libmsi_shared_link_args = [
    '-framework', 'CoreFoundation',
    '-framework', 'Foundation',
    '-framework', 'AppKit',
    '-framework', 'Carbon',
  ]
endif

if host_machine.system() == 'linux'
  libmsi_shared_deps = [
    dependency('glib-2.0',    static: false),
    dependency('gio-2.0',     static: false),
    dependency('gobject-2.0', static: false),
    dependency('libxml-2.0',  static: false),
    dependency('zlib',        static: false),
  ]
else
  libmsi_shared_deps = [glib, gio, gobject, libxml, zlib]
endif

libmsi_shared = shared_library('msi',
  objects: [libmsi.extract_all_objects(), libgsf.extract_all_objects()],
  dependencies: libmsi_shared_deps,
  link_args: libmsi_shared_link_args,
)

# --- Platform-specific dependencies and link args ---
platform_link_deps = []

if host_machine.system() == 'linux'
  libeconf = static_library('econf',
    'libeconf/lib/econf_error.c',
    'libeconf/lib/getfilecontents.c',
    'libeconf/lib/get_value_def.c',
    'libeconf/lib/helpers.c',
    'libeconf/lib/keyfile.c',
    'libeconf/lib/libeconf.c',
    'libeconf/lib/libeconf_ext.c',
    'libeconf/lib/mergefiles.c',
    'libeconf/lib/readconfig.c',
    include_directories: include_directories('libeconf/include', 'libeconf/lib'),
    pic: true,
  )
  platform_link_deps = [libeconf]
  platform_link_args = ['-static']
elif host_machine.system() == 'darwin'
  platform_link_args = [
    '-framework', 'CoreFoundation',
    '-framework', 'Foundation',
    '-framework', 'AppKit',
    '-framework', 'Carbon',
  ]
elif host_machine.system() == 'windows'
  platform_link_args = [
    '-lws2_32', '-lole32', '-lshell32', '-lshlwapi',
    '-ladvapi32', '-luserenv', '-liphlpapi', '-ldnsapi',
    '-lbcrypt',
  ]
else
  platform_link_args = []
endif

# --- msi-interop shared library (Win32 MSI API compatibility layer) ---
msi_interop_sources = [
  'msi-interop/handle_table.c',
  'msi-interop/string_utils.c',
  'msi-interop/database.c',
  'msi-interop/query.c',
  'msi-interop/record.c',
  'msi-interop/summary.c',
  'msi-interop/file_utils.c',
  'msi-interop/msi_cab.c',
]

msi_interop_inc = include_directories('msi-interop')

msi_interop_deps = [glib, gio, gobject, libxml, zlib]
msi_interop_link_with = []
msi_interop_link_args = libmsi_shared_link_args

if host_machine.system() == 'linux'
  # Statically embed all dependencies into libmsi-interop.so so it has no
  # runtime system library dependencies (other than libc) when loaded via
  # P/Invoke from C#. This requires PIC-compiled static libs.
  # libeconf provides econf_* symbols required by libblkid (transitive dep
  # of glib's gio static link).
  msi_interop_link_with += [libeconf]
  msi_interop_link_args += [
    '-Wl,--exclude-libs,ALL',
  ]
endif

msi_interop_shared = shared_library('msi-interop',
  msi_interop_sources,
  libmsi_enums[1],
  objects: [libmsi.extract_all_objects(), libgsf.extract_all_objects(), libgcab.extract_all_objects()],
  include_directories: [inc_dirs, gsf_inc_dirs, gcab_inc_dirs, msi_interop_inc],
  dependencies: msi_interop_deps,
  link_with: msi_interop_link_with,
  link_args: msi_interop_link_args,
  gnu_symbol_visibility: 'hidden',
)

# --- wixl common library (util.vala) ---
libcommon = static_library('common',
  'msitools/tools/wixl/util.vala',
  vala_header: 'libcommon.h',
  vala_vapi: 'common.vapi',
  vala_args: [
    '--enable-experimental',
    '--pkg', 'gio-2.0',
    '--pkg', 'posix',
    '--library', 'common',
  ],
  dependencies: [glib, gio],
)

# --- wixl executable ---
vapi_dir = meson.project_source_root() / 'vapi'

wixl = executable('wixl',
  'msitools/tools/wixl/builder.vala',
  'msitools/tools/wixl/msi-default.vala',
  'msitools/tools/wixl/msi.vala',
  'msitools/tools/wixl/preprocessor.vala',
  'msitools/tools/wixl/wix.vala',
  'msitools/tools/wixl/wixl.vala',
  config_h,
  vala_args: [
    '-H', 'wixl.h',
    '--use-header',
    '--enable-experimental',
    '--pkg', 'config',
    '--pkg', 'posix',
    '--pkg', 'libmsi-1.0',
    '--pkg', 'libgcab-1.0',
    '--pkg', 'libxml-2.0',
    '--vapidir=' + vapi_dir,
    '--vapidir=' + meson.project_build_root(),
  ],
  c_args: ['-include', meson.project_build_root() / 'config.h'],
  include_directories: [inc_dirs, gcab_inc_dirs],
  link_with: [libcommon] + platform_link_deps,
  dependencies: [libmsi_dep, libgcab_dep, glib, gio, libxml],
  link_args: platform_link_args,
  install: true,
)
