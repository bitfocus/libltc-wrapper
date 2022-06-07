"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const stream_1 = require("stream");
const addon = require('../build/Release/ltc-native');
class ltcwrapper extends stream_1.Duplex {
}
exports.default = ltcwrapper;
