# Building libltc for mac

```bash
CFLAGS="-arch arm64 -arch x86_64" ./configure
make -j 8
cp src/.libs/libltc.11.dylib ../libltc-wrapper/bin/libltc.11.dylib
```

# Building libltc for Windows

* Create a dll-project, add all .c/.h files
* Add libltc.def as linker definition file in project config
* Disable specific error 4996 in C/C++ compiler advanced section
* Disable precompiled headers in project C/C++ compiler options
* Remove default class example files