# libltc wrapper

![Build](https://github.com/bitfocus/libltc-wrapper/workflows/Build/badge.svg)

This module gives you access to the [libltc](https://x42.github.io/libltc/) library from node.js.

# Bits per sample

Internally libltc uses 8 bit unsigned audio data, but the decoder supports reading uint16, sint16 and float too. The encoder however only supports uint8.

# Installation

The following architectures are supported:
 * linux-x64
 * linux-armhf
 * linux-arm64
 * macos-x64
 * macos-arm64 (M1)
 * windows-x64

Add the module to your project:
```bash
npm i libltc-wrapper
# or
yarn add libltc-wrapper
```

# State

* Successfully encodes and decodes LTC audio
* Tested successfully on Macbook Pro M1 (arm64) and Ubuntu 20.04 (x64)

# Examples

## Decoding

```javascript
    const { LTCDecoder } = require('libltc-wrapper');
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
    const { LTCEncoder, LTC_USE_DATE } = require('libltc-wrapper');

    const encoder = new LTCEncoder(48000, 25, LTC_USE_DATE); // 48khz, 25 fps, LTC_USE_DATE flag
    
    let time = new Date();

    encoder.setTimecode({
      hours: time.getHours(),
      minutes: time.getMinutes(),
      seconds: time.getSeconds(),
      frame: time.getMilliseconds() / (1000 / 25),
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

# Class methods

## LTCDecoder instance methods

### constructor(sampleRate: number, framerate: number, audioFormat: string)

Samplerate in hz, for example 48000. Framerate should be less than or equal to 30 fps. Audioformat must be one of `u8` for unsigned 8 bit, `u16` for unsigned 16 bit, `s16` for signed 16 bit, or `float` for 32 bit float.

### write(buffer: Buffer)

Write audio data to the decoder, should be full frames, but any number of frames. They will be buffered up until it finds a LTC frame

### read()

Reads the next frame from the decoder queue. If there are no frames in the queue it returns undefined. When a valid frame is found, it returns a LTCFrame object.

## LTCEncoder instance methods

### constructor(sampleRate: number, framerate: number, flags?: number);

Samplerate in hz, for example 48000. Framerate should be less than or equal to 30. Flags should be one, or a combination of the following flags: `LTC_USE_DATE`, `LTC_TC_CLOCK`, `LTC_BGF_DONT_TOUCH`, `LTC_NO_PARITY`. See the librtc documentation for more information about the flags. The default flag is `LTC_USE_DATE`.

### setVolume(dBFS: number)

Set the volume of the generated LTC signal, in dBFs

### setFilter(riseTime: number)

Set encoder signal rise-time / signal filtering, in uS. Default is 40.

### setTimecode(timecode: LTCTimecode)

Set encoder timecode, expects a LTCTimecode object. You only need to specify non-zero parameters. The default is 0 for any omitted object parameters.

### getTimecode()

Get the current encoder timecode. Returns a LTCTimecode object with the current position.

### encodeFrame()

Write the next frame to the audio buffer. Fetch the buffer using `getBuffer()`.

### incrementTimecode()

Increment the current timecode by one frame.

### decrementTimecode()

Decrement the timecode by one frame

### getBuffer()

Returns the audio buffer for the current frame. The size of this buffer would be sample_rate / fps. So at 48000 khz, 25fps, that would be 1920 bytes of unsigned 8 bit audio.

# License

This node module is licensed under the MIT license. And [libltc](https://x42.github.io/libltc/) is licensed under the GNU LGPL 3 license, but is included in this project in binary form and dynamically linked. So this module _should_ in theory be safe to use in any open source or closed source commercial products. I am not a lawyer. This must not be construed as legal advice. Read more here http://answers.google.com/answers/threadview/id/439136.html