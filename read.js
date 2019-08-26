const SerialPort = require('serialport')
const ByteLength = require('@serialport/parser-byte-length')
const osc = require('./node_modules/node-osc/lib/');
const path = "/dev/tty.wchusbserial14410"
const port = new SerialPort(path, {
  baudRate: 115200
})
const parser = port.pipe(new ByteLength({
  length: 9
}))

const oscPort = 3333;

const client = new osc.Client('127.0.0.1', oscPort);



parser.on('data', line => {


  charArray = line.toString('binary').split('')
  var uint = charArray.map(l => l.charCodeAt());

  var LEDs = {
    left: bytesToFloat(uint.slice(0, 4)),
    right: bytesToFloat(uint.slice(4, 8))
  }
  var pipe = uint[uint.length - 1] + 1


  Object.keys(LEDs).forEach(function(key) {

    const m = new osc.Message(`/device${pipe}/${key}`);

    m.append({
      type: 'Float',
      value: LEDs[key]
    });
    client.send(m, (err) => {
      if (err) {
        console.error(new Error(err));
      }
      //client.close();
    });



  })



})


function bytesToFloat(bytes) {
  // JavaScript bitwise operators yield a 32 bits integer, not a float.
  // Assume LSB (least significant byte first).
  var bits = bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
  var sign = (bits >>> 31 === 0) ? 1.0 : -1.0;
  var e = bits >>> 23 & 0xff;
  var m = (e === 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
  var f = sign * m * Math.pow(2, e - 150);
  return f;
}



//port.write('ROBOT POWER ON\n')
//> ROBOT ONLINE
