import { Duplex, DuplexOptions } from "stream";
const addon = require('../build/Release/ltc-native');

export default class ltcwrapper {
	apv: number;
	queueSize: number;
	audioFormat: 'u8' | 'u16' | 's16' | 'float';
	obj: any;


	constructor(opts: { apv: number, queueSize: number, audioFormat: 'u8' | 'u16' | 's16' | 'float' }) {
		this.apv = opts.apv;
		this.queueSize = opts.queueSize;
		this.audioFormat = opts.audioFormat;

		this.obj = addon.createLTCObject(this.apv, this.queueSize, this.audioFormat);
	}

	write(buffer: Buffer) {
		return addon.writeAudio(this.obj, buffer);
	}

	read() {
		return addon.readFrame(this.obj);
	}
}

