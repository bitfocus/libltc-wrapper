/// <reference types="node" />
import { Duplex } from "stream";
export default class ltcwrapper extends Duplex {
    apv: number;
    queueSize: number;
    audioFormat: 'u8' | 'u16' | 's16' | 'float';
    obj: any;
    constructor(opts: {
        apv: number;
        queueSize: number;
        audioFormat: 'u8' | 'u16' | 's16' | 'float';
    });
}
