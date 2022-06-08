import { Duplex, DuplexOptions } from "stream";
const addon = require('../build/Release/ltc-native');

export default class ltcwrapper {
	apv: number;
	queueSize: number;
	audioFormat: 'u8' | 'u16' | 's16' | 'float';
	framerate: number;
	obj: any;


	constructor(opts: { apv: number, queueSize: number, audioFormat: 'u8' | 'u16' | 's16' | 'float', framerate: number }) {
		this.apv = opts.apv;
		this.queueSize = opts.queueSize;
		this.audioFormat = opts.audioFormat;
		this.framerate = opts.framerate;

		this.obj = addon.createLTCObject(this.apv, this.queueSize, this.audioFormat, this.framerate);
	}

	write(buffer: Buffer) {
		return addon.writeAudio(this.obj, buffer);
	}

	read() {
		return addon.readFrame(this.obj);
	}
}

