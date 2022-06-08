const ltcwrapper = require("../dist/binding.js").default;
const assert = require("assert");
const fs = require('fs');
assert(ltcwrapper, "The expected function is undefined");

let file = fs.readFileSync(__dirname + "/../timecode.raw");

function testBasic()
{
    const result = new ltcwrapper({
        apv: 1920,
        queueSize: 32,
        audioFormat: "u8"
    });
    //assert.strictEqual(result, "world", "Unexpected value returned");

    let wrote = result.write(file.slice(0,1024));
    wrote = result.write(file.slice(1024,2048));
    assert.strictEqual(wrote, undefined, "Should have returned undefined");

    let frame = result.read();
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");