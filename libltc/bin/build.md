# Building libltc for mac

```bash
CFLAGS="-arch arm64 -arch x86_64" ./configure
make -j 8
cp src/.libs/libltc.11.dylib ../libltc-wrapper/bin/libltc.11.dylib
```