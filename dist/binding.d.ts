/// <reference types="node" />
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
    dropped_frame: boolean;
};
export declare class LTCDecoder {
    apv: number;
    queueSize: number;
    audioFormat: 'u8' | 'u16' | 's16' | 'float';
    obj: any;
    /**
     * Create a LTC decoder instance
     *
     * @param sampleRate sample rate in Hz
     * @param framerate frame rate, should not be above 30
     * @param audioFormat u8 (unsigned 8 bit), u16 (unsigned 16 bit), s16 (signed 16 bit), float (floating point)
     * @param queueSize maximum amount of frames queued before you read() them, defaults to 32
     */
    constructor(sampleRate: number, framerate: number, audioFormat: 'u8' | 'u16' | 's16' | 'float', queueSize?: number);
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
