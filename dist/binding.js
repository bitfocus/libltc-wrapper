"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.LTCDecoder = void 0;
const addon = require('../build/Release/ltc-native');
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
            throw new Error('Framerate cannot be above 30');
        }
        if (!['u8', 'u16', 's16', 'float'].includes(audioFormat)) {
            throw new Error('Audio format must be u8, u16, s16 or float');
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
