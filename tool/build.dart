import 'dart:async';
import 'dart:io';

import 'package:cli_util/cli_util.dart' as cli_util;
import 'package:path/path.dart' as path;

String makeJobsArgument = '-j${(Platform.numberOfProcessors+1).toString()}';
String dartSdkArch;

Directory dartSdkDir = cli_util.getSdkDir().absolute;
Directory projectRoot = new Directory.fromUri(Platform.script).parent.parent;
Directory extensionSourceDir = new Directory(path.join(projectRoot.path, 'lib', 'src'));
Directory libsassSourceDir = new Directory(path.join(extensionSourceDir.path, 'libsass'));
Directory libsassLibDir = new Directory(path.join(libsassSourceDir.path, 'lib'));

File dartBin = new File(path.join(dartSdkDir.absolute.path, 'bin', 'dart'));
File pubBin = new File(path.join(dartSdkDir.absolute.path, 'bin', 'pub'));

ProcessArgs pubGetProcess = new ProcessArgs(pubBin.path, ['install'], wD: projectRoot);
ProcessArgs gitSubmoduleUpdate = new ProcessArgs('git', ['submodule', 'update', '--init', '--recursive'], wD: projectRoot);
ProcessArgs makeLibSassProcess = new ProcessArgs('make', [makeJobsArgument], wD: libsassSourceDir);

void main() {
  // stderr, really? Does not seem right, but that's where it's printed
  String dartVersionString = Process.runSync(dartBin.path, ['--version']).stderr;
  print('''
ENV: 
  dartSdkVersion     = $dartVersionString
  dartSdkDir         = $dartSdkDir
  projectRoot        = $projectRoot
  extensionSourceDir = $extensionSourceDir
  libsassSourceDir   = $libsassSourceDir
  libsassLibDir      = $libsassLibDir
''');

  projectRootPrepare()
    ..then((_) {
    if(Platform.isLinux) {
      if(new RegExp(r"linux_x64").firstMatch(dartVersionString) != null) {
        dartSdkArch = '64';
      } else {
        dartSdkArch = '32';
      }
      extensionCompileNix();
    } else if(Platform.isMacOS) {
      if(new RegExp(r"macos_x64").firstMatch(dartVersionString) != null) {
        dartSdkArch = 'x86_64';
      } else {
        dartSdkArch = 'i386';
      }
      extensionCompileNix();
    } else {
      print('unsupported dart-sdk: "$dartVersionString" located: "${dartSdkDir.path}"');
      exit(1);
    }
  })
  ..catchError((Exception _) => print('Could not prepare environment. ${_.toString()}'));
}

Future projectRootPrepare() {
  final Completer ac = new Completer();
  Directory.current = projectRoot;
  pubGetProcess.run()
    ..then((_) {
    gitSubmoduleUpdate.run().then((_) => ac.complete());
  })
  ..catchError((_) => ac.completeError(new Exception('pubGet failed!')));
  return ac.future;
}

Future extensionCompileNix() {
  Completer ac = new Completer();
  ProcessArgs makeExtensionProcessS1;
  ProcessArgs makeExtensionProcessS2;

  // Assemble Compilation Process 1
  makeExtensionProcessS1 = new ProcessArgs('g++',
  ['-fPIC',
  '-I${dartSdkDir.path}',
  '-I${libsassLibDir.path}',
  '-c', 'sass_extension.cc'],
  wD: extensionSourceDir);
  makeExtensionProcessS2 = new ProcessArgs('gcc', ['-fPIC'], wD: extensionSourceDir);

  // Find .o files in libsass source directory
  RegExp oFileReg = new RegExp(r".o$");
  List<FileSystemEntity> libsassOutFiles = libsassSourceDir.listSync(recursive: false).where((_) => oFileReg.firstMatch(_.path) != null);
  if(Platform.isLinux) {
    makeExtensionProcessS1.arguments.addAll([
        '-m$dartSdkArch',
        '-lsass'
    ]);
    makeExtensionProcessS2.arguments.addAll([
        '-shared',
        '-m$dartSdkArch',
        '-Wl,-soname,libsass_extension.so',
        '-o', '../libsass_extension.so',
        '-lc'
    ]);
  } else if(Platform.isMacOS) {
    makeExtensionProcessS1.arguments.addAll([
        '-arch', dartSdkArch
    ]);
    makeExtensionProcessS2.arguments.addAll([
        '-dynamiclib',
        '-undefined', 'dynamic_lookup',
        '-arch', dartSdkArch,
        '-Wl,-install_name,libsass_extension.dylib',
        '-o', '../libsass_extension.dylib',
        '-lc'
    ]);
  }
  // append .o files to process-arguments
  makeExtensionProcessS2.arguments.addAll(libsassOutFiles.map((_) => _.absolute.path));

  makeLibSassProcess.run()
    ..then((_) {
    makeExtensionProcessS1.run()
      ..then((_) {
      makeExtensionProcessS2.run()
        ..then((_) {
        print('Done. Everything went fine.');
        ac.complete();
      })
        ..catchError((_) => ac.completeError(new Exception('Compilation failed in stage 2. See console output.'))
      );
    })
      ..catchError((_) => ac.completeError(new Exception('Compilation failed in stage 1. See console output.')));
  })
    ..catchError((_) => ac.completeError(new Exception('Make of libsass failed. See console output.')));
  return ac.future;
}

class ProcessArgs {
  String executable;
  List<String> arguments;
  Directory wD;

  ProcessArgs(this.executable, this.arguments, {this.wD: null});

  Future run() {
    final Completer ac = new Completer();
    if (wD != null) Directory.current = wD;
    print('# Running: "$executable ${arguments.join(' ')}" in "${Directory.current.path}"');
    Process.start(executable, arguments, runInShell: true).then((Process _) {
      _.stderr.transform(SYSTEM_ENCODING.decoder).listen(print);
      _.stdout.transform(SYSTEM_ENCODING.decoder).listen(print);
      _.exitCode.then((_) {
        if (_ == 0) {
          ac.complete(_);
          print('# Success!');
        } else ac.completeError(new Exception('Exit code was: $_'));
      });
    });
    return ac.future;
  }
}