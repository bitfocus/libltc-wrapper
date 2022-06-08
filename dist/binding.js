"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const addon = require('../build/Release/ltc-native');
class ltcwrapper {
    constructor(opts) {
        this.apv = opts.apv;
        this.queueSize = opts.queueSize;
        this.audioFormat = opts.audioFormat;
        this.obj = addon.createLTCObject(this.apv, this.queueSize, this.audioFormat);
    }
    write(buffer) {
        return addon.writeAudio(this.obj, buffer);
    }
    read() {
        return addon.readFrame(this.obj);
    }
}
exports.default = ltcwrapper;
