/// Dart class-based wrappers for the libsass C/C++ Sass compiler.
///
/// This library implements the libsass C/C++ Sass compiler using Dart classes.
/// The classes closely follow the functions laid out in the `sass_context.h`
/// file in the original libsass library. For further documentation, please
/// refer to the
/// [libsass implementation guide](https://github.com/sass/libsass/wiki).
library libsass;

import 'dart-ext:sass_extension';

abstract class Sass {
  /// The version of the compiled libsass C/C++ library.
  static String libsassVersion() native "libsass_version";
}

class SassOptions {
  int _handle;

  SassOptions() {
    _handle = _sassMakeOptions();
    _peerRegister(this, _handle);
  }

  SassOptions.fromHandle(this._handle);

  int get handle => _handle;

  int get precision => _sassOptionGetPrecision(_handle);
  int get outputStyle => _sassOptionGetOutputStyle(_handle);
  bool get sourceComments => _sassOptionGetSourceComments(_handle);
  bool get sourceMapEmbed => _sassOptionGetSourceMapEmbed(_handle);
  bool get sourceMapContents => _sassOptionGetSourceMapContents(_handle);
  bool get omitSourceMapUrl => _sassOptionGetOmitSourceMapUrl(_handle);
  bool get isIndentedSyntaxSrc => _sassOptionGetIsIndentedSyntaxSrc(_handle);
  String get inputPath => _sassOptionGetInputPath(_handle);
  String get outputPath => _sassOptionGetOutputPath(_handle);
  String get includePath => _sassOptionGetIncludePath(_handle);
  String get sourceMapFile => _sassOptionGetSourceMapFile(_handle);

  void set precision(int precision) =>
      _sassOptionSetPrecision(_handle, precision);
  void set outputStyle(int outputStyle) =>
      _sassOptionSetOutputStyle(_handle, outputStyle);
  void set sourceComments(bool sourceComments) =>
      _sassOptionSetSourceComments(_handle, sourceComments);
  void set sourceMapEmbed(bool sourceMapEmbed) =>
      _sassOptionSetSourceMapEmbed(_handle, sourceMapEmbed);
  void set sourceMapContents(bool sourceMapContents) =>
      _sassOptionSetSourceMapContents(_handle, sourceMapContents);
  void set omitSourceMapUrl(bool omitSourceMapUrl) =>
      _sassOptionSetOmitSourceMapUrl(_handle, omitSourceMapUrl);
  void set isIndentedSyntaxSrc(bool isIndentedSyntaxSrc) =>
      _sassOptionSetIsIndentedSyntaxSrc(_handle, isIndentedSyntaxSrc);
  void set inputPath(String inputPath) =>
      _sassOptionSetInputPath(_handle, inputPath);
  void set outputPath(String outputPath) =>
      _sassOptionSetOutputPath(_handle, outputPath);
  void set includePath(String includePath) =>
      _sassOptionSetIncludePath(_handle, includePath);
  void set sourceMapFile(String sourceMapFile) =>
      _sassOptionSetSourceMapFile(_handle, sourceMapFile);

  // Native functions
  int _sassMakeOptions() native "sass_make_options";
  void _peerRegister(Object object, int handle) native "peer_register";

  int _sassOptionGetPrecision(int handle) native "sass_option_get_precision";
  int _sassOptionGetOutputStyle(int handle) native
      "sass_option_get_output_style";
  bool _sassOptionGetSourceComments(int handle) native
      "sass_option_get_source_comments";
  bool _sassOptionGetSourceMapEmbed(int handle) native
      "sass_option_get_source_map_embed";
  bool _sassOptionGetSourceMapContents(int handle) native
      "sass_option_get_source_map_contents";
  bool _sassOptionGetOmitSourceMapUrl(int handle) native
      "sass_option_get_omit_source_map_url";
  bool _sassOptionGetIsIndentedSyntaxSrc(int handle) native
      "sass_option_get_is_indented_syntax_src";
  String _sassOptionGetInputPath(int handle) native
      "sass_option_get_input_path";
  String _sassOptionGetOutputPath(int handle) native
      "sass_option_get_output_path";
  String _sassOptionGetIncludePath(int handle) native
      "sass_option_get_include_path";
  String _sassOptionGetSourceMapFile(int handle) native
      "sass_option_get_source_map_file";

  void _sassOptionSetPrecision(int handle, int precision) native
      "sass_option_set_precision";
  void _sassOptionSetOutputStyle(int handle, int outputStyle) native
      "sass_option_set_output_style";
  void _sassOptionSetSourceComments(int handle, bool sourceComments) native
      "sass_option_set_source_comments";
  void _sassOptionSetSourceMapEmbed(int handle, bool sourceMapEmbed) native
      "sass_option_set_source_map_embed";
  void _sassOptionSetSourceMapContents(int handle,
      bool sourceMapContents) native "sass_option_set_source_map_contents";
  void _sassOptionSetOmitSourceMapUrl(int handle, bool omitSourceMapUrl) native
      "sass_option_set_omit_source_map_url";
  void _sassOptionSetIsIndentedSyntaxSrc(int handle,
      bool isIndentedSyntaxSrc) native "sass_option_set_is_indented_syntax_src";
  void _sassOptionSetInputPath(int handle, String inputPath) native
      "sass_option_set_input_path";
  void _sassOptionSetOutputPath(int handle, String outputPath) native
      "sass_option_set_output_path";
  void _sassOptionSetIncludePath(int handle, String includePath) native
      "sass_option_set_include_path";
  void _sassOptionSetSourceMapFile(int handle, String sourceMapFile) native
      "sass_option_set_source_map_file";
}

abstract class SassContext {
  int _handle;
  SassOptions _options;

  int compile();

  SassOptions get options => _options;
  void set options(SassOptions options);

  String get outputString => _sassContextGetOutputString(_handle);
  int get errorStatus => _sassContextGetErrorStatus(_handle);
  String get errorJson => _sassContextGetErrorJson(_handle);
  String get sourceMapString => _sassContextGetSourceMapString(_handle);
  String get includedFiles => _sassContextGetIncludedFiles(_handle);

  // Native functions
  void _peerRegister(Object object, int handle) native "peer_register";

  String _sassContextGetOutputString(int handle) native
      "sass_context_get_output_string";
  int _sassContextGetErrorStatus(int handle) native
      "sass_context_get_error_status";
  String _sassContextGetErrorJson(int handle) native
      "sass_context_get_error_json";
  String _sassContextGetSourceMapString(int handle) native
      "sass_context_get_source_map_string";
  String _sassContextGetIncludedFiles(int handle) native
      "sass_context_get_included_files";
}

class SassFileContext extends SassContext {

  SassFileContext(String inputPath) {
    _handle = _sassMakeFileContext(inputPath);
    _peerRegister(this, _handle);
    _options = new SassOptions.fromHandle(_sassFileContextGetOptions(_handle));
  }

  int compile() => _sassCompileFileContext(_handle);

  void set options(SassOptions options) {
    _sassFileContextSetOptions(_handle, options.handle);
    _options = new SassOptions.fromHandle(_sassFileContextGetOptions(_handle));
  }

  // Native functions
  int _sassMakeFileContext(String inputPath) native "sass_make_file_context";
  int _sassCompileFileContext(int handle) native "sass_compile_file_context";
  int _sassFileContextGetOptions(int handle) native
      "sass_file_context_get_options";
  void _sassFileContextSetOptions(int handle, int options) native
      "sass_file_context_set_options";
}

class SassDataContext extends SassContext {

  SassDataContext(String sourceString) {
    _handle = _sassMakeDataContext(sourceString);
    _peerRegister(this, _handle);
    _options = new SassOptions.fromHandle(_sassDataContextGetOptions(_handle));
  }

  int compile() => _sassCompileDataContext(_handle);

  void set options(SassOptions options) {
    _sassDataContextSetOptions(_handle, options.handle);
    _options = new SassOptions.fromHandle(_sassDataContextGetOptions(_handle));
  }

  // Native functions
  int _sassMakeDataContext(String sourceString) native "sass_make_data_context";
  int _sassCompileDataContext(int handle) native "sass_compile_data_context";
  int _sassDataContextGetOptions(int handle) native
      "sass_data_context_get_options";
  void _sassDataContextSetOptions(int handle, int options) native
      "sass_data_context_set_options";
}
