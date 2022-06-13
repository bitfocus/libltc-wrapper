const LTCDecoder = require("../dist/binding.js").LTCDecoder;
const LTCEncoder = require("../dist/binding.js").LTCEncoder;
const assert = require("assert");
const fs = require("fs");
const { LTC_USE_DATE, LTC_TC_CLOCK } = require("../dist/binding.js");

assert(LTCDecoder, "LTCDecoder is undefined");
assert(LTCEncoder, "LTCEncoder is undefined");

let testframe = Buffer.from(
  "AHd3emMAAAAAAEhkdHt8hXa9////saAAAAAlS6P///+2oIB7enSBRQAAAEFfc3p+gXyd////v6SEfHp2fVcAAAA5XXF5f4CAkP///8anh3x6eXlqAAAAL1pveX9+g4T////Ktz4AAABNYP///9OqjX15e3R7AAAAIk6X////tqkAAAAcVWt3f3yHde7//+eulX16fHKECwAADlFodn59h3Te///xsJl+e3xzhRwAAAVOZnZ9foZ2yf///LSegHx8dIU0AAAASGN1fH+FebH///+5ooJ8e3aCSAAAAEJgdHt/g32i////v6WFfXt4flsAAAA6XnJ6gIGBj////8ioiX17eXtoAAAAMlxxeoCAhIX////Qqox+e3t4dQAAAChZb3mAfod7////3KyRfnt8dYAAAAAbVmx4gH6Id/P//+aulX57fXSEBwAAElNqeH9+iHXc///zsZuAfH1zhhsAAAZPZ3d+foh2y////LSegX19dIYzAAAASWR2fX+GerP///+6o4N9fHaDRAAAAERidXyAhH6g////wKaGfXx4gFcAAAA8X3N7gIKBk////8eoiX57entqAAAAMlxxeoGAhYX////Rq41+e3t4dQAAAClab3mBf4h8////2LhXAAAAS1f4///dtmYAAABGU/H//+G1bgAAAERR5f//6LN5AAAAP07Z///usoQAAAA6Tcn///exkQAAADNMvP///bKXAAAAL0y0////sZ4AAAAoTKf///+ypAAAACFMm////7OrAAAAGE2M////tq8IAAATToH///+5shcAAApOd////8Smhnx5eHplAAAAOEzD///5sZIAAAAxTLL///+woAAAACVKqf///7KfAAAAJk2e////s6YAAAAdVWt2fnyGdfT//+Ksk3x5e3KCBQAAEVJodn18h3Td///wr5l+entyhRgAAAZOZnV9fYZ0zP///LGYAAAALUuw////tJ6Ae3t0gzoAAABFYXR7foR5rf///7mignx7dYFLAAAAQF9zen+Bfpn////CpoZ8enh9XQAAADhdcnp/gIKN////yaiJfXp6eW8AAAAyTbT///+zmwAAAC1OqP///7SgAAAAKE6g////tKYAAAAfVmx3f32Hd/b//+OtlH16fHOCBAAAE1Npd359iHTi///ur5h+e3xzhRcAAAdPZ3Z9fod2yv///LOegHx8c4UsAAAATmD////QuEcAAABIY3V8f4R6rP///7ujg3x7doFNAAAAP19ze3+CfZ3////ApoZ9e3h+XAAAADlecnqAgIKM////yqmKfXt6emwAAAA1Trb///+0mAAAACtab3mAfoZ8////1bdWAAAASln+///brJB+enx0gQAAABdVa3d/fYh26v//6q+Xfnt9c4UQAAAMUWh3fn6Iddn///Sxm398fXOGIAAAA01mdn5+h3fB////tqCBfHx1hTkAAABHY3V8f4V6sf///7qjg318doJJAAAAQmF0e4CDf5v////CpoZ9e3l+XwAAADhecnqAgYKP////yamJfnt7em4AAAAvW3B6gH+Fg////9KrjX57fHd5AAAAJFhueYB+iHr+///frZN+e311gAAAABtWbHiAfol37///6K+Wf3x9dIYPAAANUml3f36Iddv///Sym4B8fXSHIAAABE5nd35/h3fC////tqCCfX11hjQAAABJZHZ9f4Z6sv///7qjg318d4JMAAAAQWF0fICDfqD////BpoZ+fHh/WwAAADtfc3uBgYOO////yqmKfnt7em0AAAAwW3F6gICFhP///9Krjn57fHd5AAAAKU6l////tKcAAAAiT5X////VqwAAAB1Qjv///7ivBQAAFlCD////u7MQAAAPUHn///+/tSAAAAZQbf///8W3LQAAAE9o////yLc2AAAAT2H////Nt0IAAABNW////9O3UAAAAEtV9f//3LVhAAAAR1Lt///htGwAAABDT97//+qzeAAAADhccXl+f3+P////v7UlAAACTmv////KqIl8eXl3bwAAACpZbnh/fYV8////1qqOfHl6dHoAAAAjTpn///+1qQAAABtVa3d+fId18P//5a2UfXp8coQMAAANUWh2fn2HdNz///GwmX57fHKFHAAAAVBn////ybc5AAAAS2R1fH2Fd7n///+2oIF8e3SDOwAAAEVidHt+hHqq////u6ODfHt2gFIAAAA9X3N6f4J+mP///8Kmhn17eHxhAAAANl1xeoCAg4j////HtzYAAABOZ////8m3PgAAAE1h////zrdKAAAAS1v////Yq499ent0fgAAABtVa3d/fYh27v//566Wfnt8c4QIAAAQUml3fn2Idd7///Gwmn97fHOGHwAAA05mdn1+h3bJ///+spkAAAAuTLH///+1n4B8e3WDPgAAAEVidHx/hHuo////vKSEfHt2gU4AAAA/YHN7f4J/mv///8Kmhn17eXxjAAAANV1xeoCAg4v////FtzQAAABOaP///86pi316e3d3AAAAKk6k////taMAAAAjWG14gH2IePn//+Gtk357fHSC",
  "base64"
);

function testDecoder() {
  const result = new LTCDecoder(48000, 25, "u8");
  assert.strictEqual(result.apv, 1920, "apv should be 1920");

  let wrote = result.write(testframe);
  assert.strictEqual(wrote, undefined, "Should have returned undefined");

  let frame = result.read();
  assert(frame, "The frame was undefined");
  assert.strictEqual(
    frame.drop_frame_format,
    false,
    "The frame should have been marked as nondrop frame format"
  );
  assert.strictEqual(frame.hours, 0, "Should have been decoded as 0 hours");
  assert.strictEqual(frame.minutes, 5, "Should have been decoded as 5 minutes");
  assert.strictEqual(
    frame.seconds,
    27,
    "Should have been decoded as 27 seconds"
  );
  assert.strictEqual(
    frame.timezone,
    "+0000",
    "Should have been decoded as +0000"
  );
}

function testEncoder() {
    const result = new LTCEncoder(48000, 25, LTC_USE_DATE | LTC_TC_CLOCK);
    assert.strictEqual(result.sampleRate, 48000, "sampleRate should be 48000");

    
    assert.throws(() => result.setTimecode({ hours: 'test' }), "Should throw because hours is not a number");
    result.setTimecode({ hours: 12 });
    result.setTimecode({ });

    assert.throws(() => result.setVolume(1), "Should have thrown an error");
    result.setVolume(-3);

    result.setFilter(45);

    result.encodeFrame();

    result.incrementTimecode();

    let obj = result.getTimecode();

    assert.strictEqual(obj.frame, 1, "Should have been 1 frame");

    result.decrementTimecode();
    obj = result.getTimecode();

    assert.strictEqual(obj.frame, 0, "Should have been 0 frame");

    assert.strictEqual(result.getBuffer().length, 1920, "Buffer should be 1920 bytes")
}

assert.doesNotThrow(testDecoder, undefined, "testDecoder threw an exception");
assert.doesNotThrow(testEncoder, undefined, "testEncoder threw an exception");

console.log("Tests passed- everything looks OK!");
