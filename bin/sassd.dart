// Copyright (c) 2015, Dariel Dato-on <oddrationale@gmail.com>. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//import 'dart:async';
//import 'dart:convert';
//import 'dart:io';

import 'package:args/args.dart';
import 'package:libsass/libsass.dart';

ArgResults argResults;

ArgParser createParser() {
  return new ArgParser()
      ..addFlag(
          'stdin',
          abbr: 's',
          negatable: false,
          help: 'Read input from standard input instead of an input file.')
      ..addOption(
          'style',
          abbr: 't',
          help: 'Output style.',
          allowed: ['nested', 'compressed'],
          defaultsTo: 'nested')
      ..addFlag(
          'line-numbers',
          abbr: 'l',
          negatable: false,
          help: 'Emit comments showing original line numbers.')
      ..addFlag('line-comments', negatable: false)
      ..addOption(
          'load-path',
          abbr: 'I',
          help: 'Set Sass import path.',
          valueHelp: 'path')
      ..addFlag(
          'sourcemap',
          abbr: 'm',
          negatable: false,
          help: 'Emit source map.')
      ..addFlag(
          'omit-map-comment',
          abbr: 'M',
          negatable: false,
          help: 'Omits the source map url comment.')
      ..addOption(
          'precision',
          abbr: 'p',
          help: 'Set the precision for numbers.',
          valueHelp: 'int')
      ..addFlag(
          'version',
          abbr: 'v',
          negatable: false,
          help: 'Display sassd and compiled libsass versions.')
      ..addFlag(
          'help',
          abbr: 'h',
          negatable: false,
          help: 'Display this help message.');
}

void main(List<String> arguments) {
  final parser = createParser();
  argResults = parser.parse(arguments);

  print(Sass.libsassVersion());
}
