# Tools for development

## macOS

## build the CLAP plugin

```shell
./build/compile -i build/build.cpp -o dist/build -f clap -g nogui -d -a -v
```

### nm to debug entry points

`nm` is a command-line utility on Unix-like systems (including macOS) that displays the symbols from object files and shared libraries. You can use it to inspect the symbols exported by your plugin file. Here's a basic usage example:

```shell
nm -g <plugin_file>
```

```shell 
nm -g ./dist/build_bundle.clap/Contents/MacOS/build_bundle
```

### CLAP Info

```shell
cd ..
mkdir clap-info
cd clap-info
git clone --recurse-submodules https://github.com/surge-synthesizer/clap-info .
...
cmake -Bbuild
cmake --build build
...
cd ..
cd SA_Toolkit
../clap-info/build/clap-info dist/build_bundle.clap
```