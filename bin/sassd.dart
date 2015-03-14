/// A sassc compatible executable.
import 'dart:convert';
import 'dart:io';

import 'package:args/args.dart';
import 'package:libsass/libsass.dart';

const String SASSD_VERSION = "0.0.1";

/// Read synchronously from the stdin until end of file is reached.
String readInputSync({Encoding encoding: SYSTEM_ENCODING}) {
  final List<int> input = [];
  while (true) {
    int byte = stdin.readByteSync();
    if (byte < 0) {
      if (input.isEmpty) return null;
      break;
    }
    input.add(byte);
  }
  return encoding.decode(input);
}

void setOptions(SassContext ctx, ArgResults options) {
  if (options['style'] == "compressed") {
    ctx.options.outputStyle = 3;
  }
  if (options['line-numbers'] || options['line-comments']) {
    ctx.options.sourceComments = true;
  }
  if (options['load-path'] != null) {
    ctx.options.includePath = options['load-path'];
  }
  if (options['sourcemap']) {
    if (options['stdin'] && options.rest.isNotEmpty) {
      ctx.options.sourceMapFile = "${options.rest[0]}.map";
    } else if (!options['stdin'] && options.rest.length == 2) {
      ctx.options.sourceMapFile = "${options.rest[1]}.map";
    }
  }
  if (options['omit-map-comment']) {
    ctx.options.omitSourceMapUrl = true;
  }
  if (options['precision'] != null) {
    ctx.options.precision = int.parse(options['precision']);
  }
}

void writeFiles(SassContext ctx, String outputPath, ArgResults options) {
  new File(outputPath).writeAsStringSync(ctx.outputString);
  if (options['sourcemap']) {
    new File(ctx.options.sourceMapFile).writeAsStringSync(ctx.sourceMapString);
  }
}

void main(List<String> arguments) {
  final argParser = new ArgParser()
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
          valueHelp: 'int',
          defaultsTo: "0")
      ..addFlag(
          'version',
          abbr: 'v',
          negatable: false,
          help: 'Display versions.')
      ..addFlag(
          'help',
          abbr: 'h',
          negatable: false,
          help: 'Display this help message.');

  // Make sure options are valid
  ArgResults argResults;
  try {
    argResults = argParser.parse(arguments);
  } on FormatException catch (e) {
    stderr.writeln("${e.message}\nSee 'sassd -h'");
    exit(2);
  }

  try {
    int.parse(argResults['precision']);
  } on Exception {
    stderr.writeln("Precision must be an integer.");
    exit(2);
  }

  if (argResults.rest.length > 2) {
    stderr.writeln("Error: Too many arguments.\nSee 'sassd -h'");
    exit(2);
  }

  // Exit early if using 'help' or 'version'
  if (argResults['help']) {
    print("Usage: sassd [options] [INPUT] [OUTPUT]\n\nOptions:");
    print(argParser.usage);
    exit(0);
  }

  if (argResults['version']) {
    print("sassd: $SASSD_VERSION\nlibsass: ${Sass.libsassVersion()}");
    exit(0);
  }

  // Define inputs and outputs
  String inputPath;
  String outputPath;
  String sourceString;

  SassContext ctx;

  if (argResults['stdin'] || argResults.rest.isEmpty) {
    sourceString = readInputSync();
    ctx = new SassDataContext(sourceString);
  } else {
    inputPath = argResults.rest[0];
    ctx = new SassFileContext(inputPath);
  }
  setOptions(ctx, argResults);
  ctx.compile();

  if (ctx.errorStatus == 0) { // no errors
    if (argResults['stdin'] && argResults.rest.isNotEmpty) {
      outputPath = argResults.rest[0];
      writeFiles(ctx, outputPath, argResults);
      exit(0);
    } else if (!argResults['stdin'] && argResults.rest.length == 2) {
      outputPath = argResults.rest[1];
      writeFiles(ctx, outputPath, argResults);
      exit(0);
    } else {
      stdout.writeln(ctx.outputString);
      exit(0);
    }
  } else { // print error message and exit
    var error = JSON.decode(ctx.errorJson);
    stderr.writeln("${error['file']}:${error['line']}: ${error['message']}");
    exit(2);
  }
}
