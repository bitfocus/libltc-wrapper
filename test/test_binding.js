const ltcwrapper = require("../dist/binding.js").default;
const assert = require("assert");

assert(ltcwrapper, "The expected function is undefined");

function testBasic()
{
    const result = new ltcwrapper({
        apv: 1920,
        queueSize: 32,
        audioFormat: "kake"
    });
    //assert.strictEqual(result, "world", "Unexpected value returned");

}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");