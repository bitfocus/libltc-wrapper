/// <reference types="node" />
export default class ltcwrapper {
    apv: number;
    queueSize: number;
    audioFormat: 'u8' | 'u16' | 's16' | 'float';
    framerate: number;
    obj: any;
    constructor(opts: {
        apv: number;
        queueSize: number;
        audioFormat: 'u8' | 'u16' | 's16' | 'float';
        framerate: number;
    });
    write(buffer: Buffer): any;
    read(): any;
}
