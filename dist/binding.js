"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.LTCDecoder = exports.LTCEncoder = exports.LTC_NO_PARITY = exports.LTC_BGF_DONT_TOUCH = exports.LTC_TC_CLOCK = exports.LTC_USE_DATE = void 0;
const addon = require("../build/Release/ltc-native");
const LTC_USE_DATE = 1;
exports.LTC_USE_DATE = LTC_USE_DATE;
const LTC_TC_CLOCK = 2;
exports.LTC_TC_CLOCK = LTC_TC_CLOCK;
const LTC_BGF_DONT_TOUCH = 4;
exports.LTC_BGF_DONT_TOUCH = LTC_BGF_DONT_TOUCH;
const LTC_NO_PARITY = 8;
exports.LTC_NO_PARITY = LTC_NO_PARITY;
class LTCEncoder {
    /**
     * Create a LTC Encoder instance
     *
     * @param sampleRate Sample rate of the audio stream (for example 48000)
     * @param framerate Frame rate of the LTC stream (for example 25)
     * @param flags Flags for the encoder (see LTC_USE_DATE, LTC_TC_CLOCK, LTC_BGF_DONT_TOUCH, LTC_NO_PARITY)
     */
    constructor(sampleRate, framerate, flags = LTC_USE_DATE) {
        this.sampleRate = sampleRate;
        this.frameRate = framerate;
        this.flags = flags;
        if (flags > 8 || flags < 0) {
            throw new Error("Invalid flags");
        }
        this.encoder = addon.createLTCEncoder(sampleRate, framerate, flags);
    }
    /**
     * Set the volume of the generated LTC signal
     *
     * Typically LTC is sent at 0dBu ; in EBU callibrated systems that corresponds to -18dBFS. - by default libltc creates -3dBFS
     * since libltc generates 8bit audio-data, the minimum dBFS is about -42dB which corresponds to 1 bit.
     * 0dB corresponds to a signal range of 127 1..255 with 128 at the center.
     *
     * @param dBFS volume in dBFS
     */
    setVolume(dBFS) {
        addon.encoderSetVolume(this.encoder, dBFS);
    }
}
exports.LTCEncoder = LTCEncoder;
class LTCDecoder {
    /**
     * Create a LTC decoder instance
     *
     * @param sampleRate sample rate in Hz
     * @param framerate frame rate, should not be above 30
     * @param audioFormat u8 (unsigned 8 bit), u16 (unsigned 16 bit), s16 (signed 16 bit), float (floating point)
     * @param queueSize maximum amount of frames queued before you read() them, defaults to 32
     */
    constructor(sampleRate, framerate, audioFormat, queueSize) {
        this.apv = sampleRate / framerate;
        this.queueSize = queueSize || 32;
        this.audioFormat = audioFormat;
        if (framerate > 30) {
            throw new Error("Framerate cannot be above 30");
        }
        if (!["u8", "u16", "s16", "float"].includes(audioFormat)) {
            throw new Error("Audio format must be u8, u16, s16 or float");
        }
        this.obj = addon.createLTCDecoder(this.apv, this.queueSize, this.audioFormat);
    }
    /**
     * Write audio data to the decoder, should be full frames, but any number of frames.
     *
     * @param buffer audio buffer to decode
     */
    write(buffer) {
        return addon.writeAudio(this.obj, buffer);
    }
    /**
     * Reads the next frame from the decoder queue. If there are no frames in the queue, returns undefined.
     *
     * @returns LTCFrame
     */
    read() {
        return addon.readFrame(this.obj);
    }
}
exports.LTCDecoder = LTCDecoder;
