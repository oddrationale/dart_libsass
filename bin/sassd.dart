// Copyright (c) 2015, Dariel Dato-on <oddrationale@gmail.com>. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//import 'dart:async';
//import 'dart:convert';
import 'dart:io';

import 'package:args/args.dart';
import 'package:libsass/libsass.dart';

const String SASSD_VERSION = "0.0.1";

void main(List<String> arguments) {
  final argParser = new ArgParser()
      ..addFlag('stdin', abbr: 's', negatable: false, help: 'Read input from standard input instead of an input file.')
      ..addOption('style', abbr: 't', help: 'Output style.', allowed: ['nested', 'compressed'], defaultsTo: 'nested')
      ..addFlag('line-numbers', abbr: 'l', negatable: false, help: 'Emit comments showing original line numbers.')
      ..addFlag('line-comments', negatable: false)
      ..addOption('load-path', abbr: 'I', help: 'Set Sass import path.', valueHelp: 'path')
      ..addFlag('sourcemap', abbr: 'm', negatable: false, help: 'Emit source map.')
      ..addFlag('omit-map-comment', abbr: 'M', negatable: false, help: 'Omits the source map url comment.')
      ..addOption('precision', abbr: 'p', help: 'Set the precision for numbers.', valueHelp: 'int')
      ..addFlag('version', abbr: 'v', negatable: false, help: 'Display versions.')
      ..addFlag('help', abbr: 'h', negatable: false, help: 'Display this help message.');

  // Make sure options are valid
  ArgResults args;
  try {
    args = argParser.parse(arguments);
  } on FormatException catch(e) {
    stderr.writeln("${e.message}\nSee 'sassd -h'");
    exit(2);
  }

  if (args.rest.length > 2) {
    stderr.writeln("Error: Too many arguments.\nSee 'sassd -h'");
    exit(2);
  }

  // Exit early if using 'help' or 'version'
  if (args['help']) {
    print("Usage: ./sassc [options] [INPUT] [OUTPUT]\n\nOptions:");
    print(argParser.usage);
    exit(0);
  }

  if (args['version']) {
    print("sassd: $SASSD_VERSION\nlibsass: ${Sass.libsassVersion()}");
    exit(0);
  }

  // Population the SassOptions object based on the input
  var sassOptions = new SassOptions();
  bool fromStdin = false;
  bool generateSourceMap = false;

  if (args['stdin']) {
    fromStdin = true;
  }

  if (args['style'] != null) {
    switch (args['style']) {
      case 'nested':
        sassOptions.outputStyle = 0;
        break;
      case 'compressed':
        sassOptions.outputStyle = 2;
        break;
    }
  }

  if (args['line-numbers'] || args['line-comments']) {
    sassOptions.sourceComments = true;
  }

  if (args['load-path'] != null) {
    sassOptions.includePath = args['load-path'];
  }

  if (args['sourcemap']) {
    generateSourceMap = true;
  }

  if (args['omit-map-comment']) {
    sassOptions.omitSourceMapUrl = true;
  }

  if (args['precision'] != null) {
    try {
      sassOptions.precision = args['precision'] as int;
    } on Exception {
      stderr.writeln("Precision must be an integer.");
      exit(2);
    }
  }
}
