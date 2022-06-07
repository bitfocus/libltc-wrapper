"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const stream_1 = require("stream");
const addon = require('../build/Release/ltc-native');
class ltcwrapper extends stream_1.Duplex {
    constructor(opts) {
        super();
        this.apv = opts.apv;
        this.queueSize = opts.queueSize;
        this.audioFormat = opts.audioFormat;
        this.obj = addon.createLTCObject(this.apv, this.queueSize, this.audioFormat);
    }
}
exports.default = ltcwrapper;
