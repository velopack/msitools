[CCode (cheader_filename = "libgcab.h")]
namespace GCab {

	[CCode (cprefix = "GCAB_COMPRESSION_", type_id = "gcab_compression_get_type ()")]
	public enum Compression {
		NONE,
		MSZIP,
		QUANTUM,
		LZX,
		MASK;
	}

	[CCode (cprefix = "GCAB_FILE_ATTRIBUTE_", type_id = "gcab_file_attribute_get_type ()")]
	[Flags]
	public enum FileAttribute {
		RDONLY,
		HIDDEN,
		SYSTEM,
		ARCH,
		EXEC,
		NAME_IS_UTF;
	}

	[CCode (cprefix = "GCAB_ERROR_")]
	public errordomain Error {
		FORMAT,
		FAILED,
		NOT_SUPPORTED,
		INVALID_DATA;
	}

	[CCode (has_target = false)]
	public delegate bool FileCallback (File file);

	[CCode (type_id = "gcab_cabinet_get_type ()")]
	public class Cabinet : GLib.Object {
		[CCode (has_construct_function = false)]
		public Cabinet ();
		public bool add_folder (Folder folder) throws GLib.Error;
		public bool write (GLib.OutputStream stream, [CCode (delegate_target = false)] FileCallback? file_callback, [CCode (delegate_target = false)] GLib.FileProgressCallback? progress_callback, void* user_data, GLib.Cancellable? cancellable = null) throws GLib.Error;
		public bool write_simple (GLib.OutputStream stream, [CCode (delegate_target = false)] FileCallback? file_callback, void* user_data, GLib.Cancellable? cancellable = null) throws GLib.Error;
		public bool load (GLib.InputStream stream, GLib.Cancellable? cancellable = null) throws GLib.Error;
		public GLib.GenericArray<Folder> get_folders ();
		public uint32 get_size ();
	}

	[CCode (type_id = "gcab_folder_get_type ()")]
	public class Folder : GLib.Object {
		[CCode (has_construct_function = false)]
		public Folder (int comptype);
		public bool add_file (File cabfile, bool recurse, GLib.Cancellable? cancellable = null) throws GLib.Error;
		public uint get_nfiles ();
		public GLib.SList<File> get_files ();
		public int get_comptype ();
	}

	[CCode (type_id = "gcab_file_get_type ()")]
	public class File : GLib.Object {
		[CCode (has_construct_function = false)]
		public File.with_file (string name, GLib.File file);
		[CCode (has_construct_function = false)]
		public File.with_bytes (string name, GLib.Bytes bytes);
		public unowned GLib.File get_file ();
		public unowned GLib.Bytes get_bytes ();
		public void set_bytes (GLib.Bytes bytes);
		public unowned string get_name ();
		public uint32 get_size ();
		public uint32 get_attributes ();
		public void set_attributes (uint32 attr);
		public unowned string get_extract_name ();
		public void set_extract_name (string name);
	}
}
