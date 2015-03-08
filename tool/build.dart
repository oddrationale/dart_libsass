import 'dart:io';

void main() {
  var uri = Platform.script;
  print(uri.toFilePath());
}