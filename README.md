# libltc wrapper

This module gives you access to the libltc library from node.js.

This module is released with the MIT license, but libltc in it self is a LGPL library. But as long as you install the `libltc` library as a separate library, the license should not infer with your project, it being open source or closed source.

# Bits per sample

Internally librtc uses 8 bit unsigned audio data, but the decoder supports reading uint16, sint16 and float too. The encoder however only supports uint8.

# Installation

You need to install the `librtc` library as a dynamic library on your computer before using this module.

## MacOS

On a mac you need to use Homebrew to install the library with the command `brew install librtc`

## Ubuntu / Debian

On debian based systems the library is usually installed using `sudo apt install librtc-dev`

# State

* Working encoding and decoding LTC audio
* Tested and working on Macbook Pro M1 (arm64) and Ubuntu 20.04 (x64)

# Examples

## Decoding

```javascript
    const { LTCDecoder } = require('ltc-wrapper');
    const decoder = new LTCDecoder(48000, 25, "u8"); // 48khz, 25 fps, unsigned 8 bit

    // Write audio buffer to the decoder
    decoder.write(audioframes);

    // Check if there is any ltc frames ready
    let frame = decoder.read();

    if (frame !== undefined) {
        // found a valid frame
        console.log("Frame: ", frame);
    }
```

## Encoding

```javascript
    const { LTCEncoder, LTC_USE_DATE } = require('ltc-wrapper');

    const encoder = new LTCEncoder(48000, 25, LTC_USE_DATE); // 48khz, 25 fps, LTC_USE_DATE flag
    
    let time = new Date();

    encoder.setTimecode({
      hours: time.getHours(),
      minutes: time.getMinutes(),
      seconds: time.getSeconds(),
      frames: time.getMilliseconds() / (1000 / 25),
      days: time.getDate(),
      months: time.getMonth() + 1,
      years: time.getFullYear() % 100,
      timezone: "+0000"
    })

    // Every frame (25 times per second) increase the current frame
    encoder.incrementTimecode();

    // Get 1 frame worth of LTC audio (48khz 25fps would be 40ms audio)
    let buffer = encoder.getBuffer();
```