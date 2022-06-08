/// <reference types="node" />
export default class ltcwrapper {
    apv: number;
    queueSize: number;
    audioFormat: 'u8' | 'u16' | 's16' | 'float';
    obj: any;
    constructor(opts: {
        apv: number;
        queueSize: number;
        audioFormat: 'u8' | 'u16' | 's16' | 'float';
    });
    write(buffer: Buffer): any;
    read(): any;
}
