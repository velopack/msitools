[CCode (cheader_filename = "libmsi.h")]
namespace Libmsi {

	[CCode (cprefix = "LIBMSI_RESULT_", type_id = "libmsi_result_error_get_type ()")]
	public errordomain ResultError {
		SUCCESS,
		ACCESS_DENIED,
		INVALID_HANDLE,
		NOT_ENOUGH_MEMORY,
		INVALID_DATA,
		OUTOFMEMORY,
		INVALID_PARAMETER,
		OPEN_FAILED,
		CALL_NOT_IMPLEMENTED,
		MORE_DATA,
		NOT_FOUND,
		CONTINUE,
		UNKNOWN_PROPERTY,
		BAD_QUERY_SYNTAX,
		INVALID_FIELD,
		FUNCTION_FAILED,
		INVALID_TABLE,
		DATATYPE_MISMATCH,
		INVALID_DATATYPE;
	}

	[CCode (cprefix = "LIBMSI_DB_ERROR_", type_id = "libmsi_db_error_get_type ()")]
	public errordomain DBError {
		SUCCESS,
		INVALIDARG,
		MOREDATA,
		FUNCTIONERROR,
		DUPLICATEKEY,
		REQUIRED,
		BADLINK,
		OVERFLOW,
		UNDERFLOW,
		NOTINSET,
		BADVERSION,
		BADCASE,
		BADGUID,
		BADWILDCARD,
		BADIDENTIFIER,
		BADLANGUAGE,
		BADFILENAME,
		BADPATH,
		BADCONDITION,
		BADFORMATTED,
		BADTEMPLATE,
		BADDEFAULTDIR,
		BADREGPATH,
		BADCUSTOMSOURCE,
		BADPROPERTY,
		MISSINGDATA,
		BADCATEGORY,
		BADKEYTABLE,
		BADMAXMINVALUES,
		BADCABINET,
		BADSHORTCUT,
		STRINGOVERFLOW,
		BADLOCALIZEATTRIB;
	}

	[CCode (cprefix = "LIBMSI_PROPERTY_TYPE_", type_id = "libmsi_property_type_get_type ()")]
	public enum PropertyType {
		EMPTY,
		INT,
		STRING,
		FILETIME;
	}

	[CCode (cprefix = "LIBMSI_COL_INFO_", type_id = "libmsi_col_info_get_type ()")]
	public enum ColInfo {
		NAMES,
		TYPES;
	}

	[CCode (cprefix = "LIBMSI_DB_FLAGS_", type_id = "libmsi_db_flags_get_type ()")]
	[Flags]
	public enum DbFlags {
		READONLY,
		CREATE,
		TRANSACT,
		PATCH;
	}

	[CCode (cprefix = "LIBMSI_PROPERTY_", type_id = "libmsi_property_get_type ()")]
	public enum Property {
		DICTIONARY,
		CODEPAGE,
		TITLE,
		SUBJECT,
		AUTHOR,
		KEYWORDS,
		COMMENTS,
		TEMPLATE,
		LASTAUTHOR,
		UUID,
		EDITTIME,
		LASTPRINTED,
		CREATED_TM,
		LASTSAVED_TM,
		VERSION,
		SOURCE,
		RESTRICT,
		THUMBNAIL,
		APPNAME,
		SECURITY;
	}

	[CCode (type_id = "libmsi_database_get_type ()")]
	public class Database : GLib.Object {
		[CCode (has_construct_function = false)]
		public Database (string path, uint flags, string? persist) throws GLib.Error;
		public bool is_readonly ();
		public Record get_primary_keys (string table) throws GLib.Error;
		public bool apply_transform (string file) throws GLib.Error;
		public bool export (string table, int fd) throws GLib.Error;
		public bool import (string path) throws GLib.Error;
		public bool is_table_persistent (string table) throws GLib.Error;
		public bool merge (Database merge, string? table) throws GLib.Error;
		public bool commit () throws GLib.Error;
		public bool set_codepage (uint codepage) throws GLib.Error;
	}

	[CCode (type_id = "libmsi_query_get_type ()")]
	public class Query : GLib.Object {
		[CCode (has_construct_function = false)]
		public Query (Database database, string query) throws GLib.Error;
		public bool execute (Record? rec = null) throws GLib.Error;
		public Record? fetch () throws GLib.Error;
		public bool close () throws GLib.Error;
		public void get_error (out string? column) throws GLib.Error;
		public Record get_column_info (ColInfo info) throws GLib.Error;
	}

	[CCode (type_id = "libmsi_record_get_type ()")]
	public class Record : GLib.Object {
		[CCode (has_construct_function = false)]
		public Record (uint count);
		public bool clear ();
		public uint get_field_count ();
		public bool is_null (uint field);
		public bool set_int (uint field, int val);
		public int get_int (uint field);
		public bool set_string (uint field, string val);
		public string get_string (uint field);
		public bool load_stream (uint field, string filename);
		public bool set_stream (uint field, GLib.InputStream input, size_t count, GLib.Cancellable? cancellable = null) throws GLib.Error;
		public GLib.InputStream get_stream (uint field);
	}

	[CCode (type_id = "libmsi_summary_info_get_type ()")]
	public class SummaryInfo : GLib.Object {
		[CCode (has_construct_function = false)]
		public SummaryInfo (Database? database, uint update_count) throws GLib.Error;
		public PropertyType get_property_type (Property prop) throws GLib.Error;
		public unowned string get_string (Property prop) throws GLib.Error;
		public int get_int (Property prop) throws GLib.Error;
		public uint64 get_filetime (Property prop) throws GLib.Error;
		public bool set_string (Property prop, string value) throws GLib.Error;
		public bool set_int (Property prop, int value) throws GLib.Error;
		public bool set_filetime (Property prop, uint64 value) throws GLib.Error;
		public bool persist () throws GLib.Error;
		public bool save (Database db) throws GLib.Error;
		public GLib.Array get_properties ();
	}
}
