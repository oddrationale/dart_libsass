#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libsass/sass_context.h"

#include "include/dart_api.h"
#include "include/dart_native_api.h"


Dart_NativeFunction ResolveName(Dart_Handle name,
                                int argc,
                                bool* auto_setup_scope);


DART_EXPORT Dart_Handle sass_extension_Init(Dart_Handle parent_library) {
  if (Dart_IsError(parent_library)) return parent_library;

  Dart_Handle result_code =
      Dart_SetNativeResolver(parent_library, ResolveName, NULL);
  if (Dart_IsError(result_code)) return result_code;

  return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
 if (Dart_IsError(handle)) Dart_PropagateError(handle);
 return handle;
}

void version(Dart_NativeArguments arguments) {
  Dart_Handle result = HandleError(Dart_NewStringFromCString(libsass_version()));
  Dart_SetReturnValue(arguments, result);
}

Dart_NativeFunction ResolveName(Dart_Handle name,
                                int argc,
                                bool *auto_setup_scope) {
  if (!Dart_IsString(name)) return NULL;
  Dart_NativeFunction result = NULL;
  const char* cname;
  HandleError(Dart_StringToCString(name, &cname));

  if (strcmp("version", cname) == 0) result = version;

  return result;
}