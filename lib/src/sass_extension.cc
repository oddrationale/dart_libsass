#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libsass/sass_context.h"
#include "include/dart_api.h"
#include "include/dart_native_api.h"

/****************************************************************************
 * Boilerplate: Copied from sample_extension.cc.
 * Refer to:
 * https://www.dartlang.org/articles/native-extensions-for-standalone-dart-vm/
 ***************************************************************************/
Dart_NativeFunction ResolveName(Dart_Handle name,
                                int argc,
                                bool* auto_setup_scope);

DART_EXPORT Dart_Handle sass_extension_Init(Dart_Handle parent_library) {
  if (Dart_IsError(parent_library)) {
    return parent_library;
  }

  Dart_Handle result_code =
      Dart_SetNativeResolver(parent_library, ResolveName, NULL);
  if (Dart_IsError(result_code)) {
    return result_code;
  }

  return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) {
    Dart_PropagateError(handle);
  }
  return handle;
}

/****************************************************************************
 * Create and initialize an option struct
 ***************************************************************************/
void sass_make_options(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  Dart_Handle result;

  Dart_EnterScope();
  options = sass_make_options();
  result = HandleError(Dart_NewInteger((int64_t)options));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

/****************************************************************************
 * Create and initialize a specific context
 ***************************************************************************/
void sass_make_file_context(Dart_NativeArguments arguments) {
  struct Sass_File_Context* file_ctx;
  const char* input_path;
  Dart_Handle dh_input_path;
  Dart_Handle result;

  Dart_EnterScope();
  dh_input_path = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_StringToCString(dh_input_path, &input_path));
  file_ctx = sass_make_file_context(input_path);
  result = HandleError(Dart_NewInteger((int64_t)file_ctx));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_make_data_context(Dart_NativeArguments arguments) {
  struct Sass_Data_Context* data_ctx;
  const char* source_string;
  Dart_Handle dh_source_string;
  Dart_Handle result;

  Dart_EnterScope();
  dh_source_string = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_StringToCString(dh_source_string, &source_string));
  data_ctx = sass_make_data_context((char*)source_string);
  result = HandleError(Dart_NewInteger((int64_t)data_ctx));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

/****************************************************************************
 * Call the compilation step for the specific context
 ***************************************************************************/
void sass_compile_file_context(Dart_NativeArguments arguments) {
  struct Sass_File_Context* file_ctx;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  file_ctx = (struct Sass_File_Context*)peer;
  result = HandleError(Dart_NewInteger(sass_compile_file_context(file_ctx)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_compile_data_context(Dart_NativeArguments arguments) {
  struct Sass_Data_Context* data_ctx;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  data_ctx = (struct Sass_Data_Context*)peer;
  result = HandleError(Dart_NewInteger(sass_compile_data_context(data_ctx)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

/****************************************************************************
 * Getters for context options from Sass_Context
 ***************************************************************************/
void sass_file_context_get_options(Dart_NativeArguments arguments) {
  struct Sass_File_Context* file_ctx;
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  file_ctx = (struct Sass_File_Context*)peer;
  options = sass_file_context_get_options(file_ctx);
  result = HandleError(Dart_NewInteger((int64_t)options));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_data_context_get_options(Dart_NativeArguments arguments) {
  struct Sass_Data_Context* data_ctx;
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  data_ctx = (struct Sass_Data_Context*)peer;
  options = sass_data_context_get_options(data_ctx);
  result = HandleError(Dart_NewInteger((int64_t)options));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_file_context_set_options(Dart_NativeArguments arguments) {
  struct Sass_File_Context* file_ctx;
  struct Sass_Options* opt;
  int64_t peer;
  int64_t options;
  Dart_Handle dh_handle;
  Dart_Handle dh_options;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_options = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_IntegerToInt64(dh_options, &options));
  file_ctx = (struct Sass_File_Context*)peer;
  opt = (struct Sass_Options*)options;
  sass_file_context_set_options(file_ctx, opt);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_data_context_set_options(Dart_NativeArguments arguments) {
  struct Sass_Data_Context* data_ctx;
  struct Sass_Options* opt;
  int64_t peer;
  int64_t options;
  Dart_Handle dh_handle;
  Dart_Handle dh_options;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_options = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_IntegerToInt64(dh_options, &options));
  data_ctx = (struct Sass_Data_Context*)peer;
  opt = (struct Sass_Options*)options;
  sass_data_context_set_options(data_ctx, opt);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

/****************************************************************************
 * Getters for options
 ***************************************************************************/
void sass_option_get_precision(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  result = HandleError(Dart_NewInteger(sass_option_get_precision(options)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_output_style(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  result = HandleError(Dart_NewInteger(sass_option_get_output_style(options)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_source_comments(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  result = HandleError(Dart_NewBoolean(sass_option_get_source_comments(options)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_source_map_embed(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  result = HandleError(Dart_NewBoolean(sass_option_get_source_map_embed(options)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_source_map_contents(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  result = HandleError(Dart_NewBoolean(sass_option_get_source_map_contents(options)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_omit_source_map_url(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  result = HandleError(Dart_NewBoolean(sass_option_get_omit_source_map_url(options)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_is_indented_syntax_src(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  result = HandleError(Dart_NewBoolean(sass_option_get_is_indented_syntax_src(options)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_input_path(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  str = sass_option_get_input_path(options);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString(str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_output_path(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  str = sass_option_get_output_path(options);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString(str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_include_path(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  str = sass_option_get_include_path(options);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString(str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_option_get_source_map_file(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  options = (struct Sass_Options*)peer;
  str = sass_option_get_source_map_file(options);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString(str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

/****************************************************************************
 * Setters for options
 ***************************************************************************/
void sass_option_set_precision(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  int64_t arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_IntegerToInt64(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_precision(options, (int)arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_output_style(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  int64_t arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_IntegerToInt64(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_output_style(options, (enum Sass_Output_Style)arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_source_comments(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  bool arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_BooleanValue(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_source_comments(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_source_map_embed(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  bool arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_BooleanValue(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_source_map_embed(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_source_map_contents(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  bool arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_BooleanValue(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_source_map_contents(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_omit_source_map_url(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  bool arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_BooleanValue(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_omit_source_map_url(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_is_indented_syntax_src(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  bool arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_BooleanValue(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_is_indented_syntax_src(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_input_path(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_StringToCString(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_input_path(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_output_path(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_StringToCString(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_output_path(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_include_path(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_StringToCString(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_include_path(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

void sass_option_set_source_map_file(Dart_NativeArguments arguments) {
  struct Sass_Options* options;
  int64_t peer;
  const char* arg;
  Dart_Handle dh_handle;
  Dart_Handle dh_arg;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_arg = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  HandleError(Dart_StringToCString(dh_arg, &arg));
  options = (struct Sass_Options*)peer;
  sass_option_set_source_map_file(options, arg);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

/****************************************************************************
 * Getter for context
 ***************************************************************************/
void sass_context_get_output_string(Dart_NativeArguments arguments) {
  struct Sass_Context* ctx;
  int64_t peer;
  const char* str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  ctx = (struct Sass_Context*)peer;
  str = sass_context_get_output_string(ctx);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString(str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_context_get_error_status(Dart_NativeArguments arguments) {
  struct Sass_Context* ctx;
  int64_t peer;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  ctx = (struct Sass_Context*)peer;
  result = HandleError(Dart_NewInteger(sass_context_get_error_status(ctx)));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_context_get_error_json(Dart_NativeArguments arguments) {
  struct Sass_Context* ctx;
  int64_t peer;
  const char* str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  ctx = (struct Sass_Context*)peer;
  str = sass_context_get_error_json(ctx);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString(str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_context_get_source_map_string(Dart_NativeArguments arguments) {
  struct Sass_Context* ctx;
  int64_t peer;
  const char* str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  ctx = (struct Sass_Context*)peer;
  str = sass_context_get_source_map_string(ctx);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString(str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void sass_context_get_included_files(Dart_NativeArguments arguments) {
  struct Sass_Context* ctx;
  int64_t peer;
  char** str;
  Dart_Handle dh_handle;
  Dart_Handle result;

  Dart_EnterScope();
  dh_handle = HandleError(Dart_GetNativeArgument(arguments, 1));
  HandleError(Dart_IntegerToInt64(dh_handle, &peer));
  ctx = (struct Sass_Context*)peer;
  str = sass_context_get_included_files(ctx);
  if (str != NULL) { // check if the string is not NULL
    result = HandleError(Dart_NewStringFromCString((char*)str));
  } else {
    result = Dart_Null();
  }
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

/****************************************************************************
 * Miscellaneous
 ***************************************************************************/
void libsass_version(Dart_NativeArguments arguments) {
  Dart_EnterScope();
  Dart_Handle result = HandleError(Dart_NewStringFromCString(libsass_version()));
  Dart_SetReturnValue(arguments, result);
  Dart_ExitScope();
}

void peer_finalizer(void* isolate_callback_data, Dart_WeakPersistentHandle handle, void *peer) {
  // TODO: Clean up object
}

void peer_register(Dart_NativeArguments arguments) {
  int64_t peer;
  Dart_Handle dh_object;
  Dart_Handle dh_peer;

  Dart_EnterScope();
  dh_object = HandleError(Dart_GetNativeArgument(arguments, 1));
  dh_peer = HandleError(Dart_GetNativeArgument(arguments, 2));
  HandleError(Dart_IntegerToInt64(dh_peer, &peer));
  Dart_NewWeakPersistentHandle(dh_object, (void*)peer, 1, peer_finalizer);
  Dart_SetReturnValue(arguments, Dart_Null());
  Dart_ExitScope();
}

/****************************************************************************
 * Function lookup and name resolver
 ***************************************************************************/
struct FunctionLookup {
  const char* name;
  Dart_NativeFunction function;
};

FunctionLookup function_list[] = {
  // Create and initialize an option struct
  {"sass_make_options", sass_make_options},
  // Create and initialize a specific context
  {"sass_make_file_context", sass_make_file_context},
  {"sass_make_data_context", sass_make_data_context},

  // Call the compilation step for the specific context
  {"sass_compile_file_context", sass_compile_file_context},
  {"sass_compile_data_context", sass_compile_data_context},

  // Getters for context options from Sass_Context
  {"sass_file_context_get_options", sass_file_context_get_options},
  {"sass_data_context_get_options", sass_data_context_get_options},
  {"sass_file_context_set_options", sass_file_context_set_options},
  {"sass_data_context_set_options", sass_data_context_set_options},

  // Getters for options
  {"sass_option_get_precision", sass_option_get_precision},
  {"sass_option_get_output_style", sass_option_get_output_style},
  {"sass_option_get_source_comments", sass_option_get_source_comments},
  {"sass_option_get_source_map_embed", sass_option_get_source_map_embed},
  {"sass_option_get_source_map_contents", sass_option_get_source_map_contents},
  {"sass_option_get_omit_source_map_url", sass_option_get_omit_source_map_url},
  {"sass_option_get_is_indented_syntax_src", sass_option_get_is_indented_syntax_src},
  {"sass_option_get_input_path", sass_option_get_input_path},
  {"sass_option_get_output_path", sass_option_get_output_path},
  {"sass_option_get_include_path", sass_option_get_include_path},
  {"sass_option_get_source_map_file", sass_option_get_source_map_file},

  // Setters for options
  {"sass_option_set_precision", sass_option_set_precision},
  {"sass_option_set_output_style", sass_option_set_output_style},
  {"sass_option_set_source_comments", sass_option_set_source_comments},
  {"sass_option_set_source_map_embed", sass_option_set_source_map_embed},
  {"sass_option_set_source_map_contents", sass_option_set_source_map_contents},
  {"sass_option_set_omit_source_map_url", sass_option_set_omit_source_map_url},
  {"sass_option_set_is_indented_syntax_src", sass_option_set_is_indented_syntax_src},
  {"sass_option_set_input_path", sass_option_set_input_path},
  {"sass_option_set_output_path", sass_option_set_output_path},
  {"sass_option_set_include_path", sass_option_set_include_path},
  {"sass_option_set_source_map_file", sass_option_set_source_map_file},
  
  // Getter for context
  {"sass_context_get_output_string", sass_context_get_output_string},
  {"sass_context_get_error_status", sass_context_get_error_status},
  {"sass_context_get_error_json", sass_context_get_error_json},
  {"sass_context_get_source_map_string", sass_context_get_source_map_string},
  {"sass_context_get_included_files", sass_context_get_included_files},

  // Miscellaneous
  {"libsass_version", libsass_version},
  {"peer_register", peer_register},
  {NULL, NULL}
};

Dart_NativeFunction ResolveName(Dart_Handle name,
                                int argc,
                                bool* auto_setup_scope) {
  if (!Dart_IsString(name)) {
    return NULL;
  }
  Dart_NativeFunction result = NULL;
  if (auto_setup_scope == NULL) {
    return NULL;
  }

  Dart_EnterScope();
  const char* cname;
  HandleError(Dart_StringToCString(name, &cname));

  for (int i=0; function_list[i].name != NULL; ++i) {
    if (strcmp(function_list[i].name, cname) == 0) {
      *auto_setup_scope = true;
      result = function_list[i].function;
      break;
    }
  }

  Dart_ExitScope();
  return result;
}