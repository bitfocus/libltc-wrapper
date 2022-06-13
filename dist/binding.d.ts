/// <reference types="node" />
/**
 * Frame increment/decrement use date, also set BGF2 to '1' when encoder is initialized or re-initialized (unless LTC_BGF_DONT_TOUCH is given)
 */
declare const LTC_USE_DATE = 1;
/**
 * The Timecode is wall-clock aka freerun. This also sets BGF1 (unless LTC_BGF_DONT_TOUCH is given)
 */
declare const LTC_TC_CLOCK = 2;
/**
 * Wncoder init or re-init does not touch the BGF bits (initial values after initialization is zero)
 */
declare const LTC_BGF_DONT_TOUCH = 4;
/**
 * Parity bit is left untouched when setting or in/decrementing the encoder frame-number
 */
declare const LTC_NO_PARITY = 8;
export { LTC_USE_DATE, LTC_TC_CLOCK, LTC_BGF_DONT_TOUCH, LTC_NO_PARITY };
export declare type LTCFrame = {
    days: number;
    months: number;
    years: number;
    hours: number;
    minutes: number;
    seconds: number;
    frames: number;
    offset_start: number;
    reverse: boolean;
    volume: number;
    timezone: string;
    drop_frame_format: boolean;
};
export declare type LTCTimecode = {
    days: number;
    months: number;
    years: number;
    hours: number;
    minutes: number;
    seconds: number;
    frames: number;
    timezone: string;
};
export declare class LTCEncoder {
    encoder: any;
    sampleRate: number;
    frameRate: number;
    flags: number;
    /**
     * Create a LTC Encoder instance
     *
     * @param sampleRate Sample rate of the audio stream (for example 48000)
     * @param framerate Frame rate of the LTC stream (for example 25)
     * @param flags Flags for the encoder (see LTC_USE_DATE, LTC_TC_CLOCK, LTC_BGF_DONT_TOUCH, LTC_NO_PARITY)
     */
    constructor(sampleRate: number, framerate: number, flags?: number);
    /**
     * Set the volume of the generated LTC signal
     *
     * Typically LTC is sent at 0dBu ; in EBU callibrated systems that corresponds to -18dBFS. - by default libltc creates -3dBFS
     * since libltc generates 8bit audio-data, the minimum dBFS is about -42dB which corresponds to 1 bit.
     * 0dB corresponds to a signal range of 127 1..255 with 128 at the center.
     *
     * @param dBFS volume in dBFS
     */
    setVolume(dBFS: number): void;
    /**
     * Set encoder signal rise-time / signal filtering
     *
     * LTC signal should have a rise time of 40us +/- 10 us. by default the encoder honors this and low-pass filters the output depending on the sample-rate.
     *
     * If you want a perfect square wave, set 'rise_time' to 0.
     *
     * @param riseTime the signal rise-time in us (10^(-6) sec), set to 0 for perfect square wave, default 40.0
     */
    setFilter(riseTime: number): void;
    /**
     * Set encoder timecode
     *
     * @param timecode LTCTimecode object containing the timecode
     */
    setTimecode(timecode: Partial<LTCTimecode>): void;
    /**
     * Get the current encoder timecode
     *
     * @returns LTCTimecode object containing the current timecode
     */
    getTimecode(): LTCTimecode;
    /**
     * Write the next frame to the audio buffer
    */
    encodeFrame(): any;
    /**
     * Increment the timecode by one frame
     */
    incrementTimecode(): void;
    /**
     * Decrement the timecode by one frame
     */
    decrementTimecode(): void;
    /**
     * Get audio buffer for the current frame
     */
    getBuffer(): Buffer;
}
export declare class LTCDecoder {
    apv: number;
    queueSize: number;
    audioFormat: "u8" | "u16" | "s16" | "float";
    obj: any;
    /**
     * Create a LTC decoder instance
     *
     * @param sampleRate sample rate in Hz
     * @param framerate frame rate, should not be above 30
     * @param audioFormat u8 (unsigned 8 bit), u16 (unsigned 16 bit), s16 (signed 16 bit), float (floating point)
     * @param queueSize maximum amount of frames queued before you read() them, defaults to 32
     */
    constructor(sampleRate: number, framerate: number, audioFormat: "u8" | "u16" | "s16" | "float", queueSize?: number);
    /**
     * Write audio data to the decoder, should be full frames, but any number of frames.
     *
     * @param buffer audio buffer to decode
     */
    write(buffer: Buffer): any;
    /**
     * Reads the next frame from the decoder queue. If there are no frames in the queue, returns undefined.
     *
     * @returns LTCFrame
     */
    read(): LTCFrame | undefined;
}
