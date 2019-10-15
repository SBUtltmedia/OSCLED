const SerialPort = require('serialport')
const ByteLength = require('@serialport/parser-byte-length')
const osc = require('./node_modules/node-osc/lib/');
const path = "/dev/cu.usbmodem14421"


const oscPort = 3333;

const client = new osc.Client('127.0.0.1', oscPort);
makeParser()

function makeParser(){

  let port = new SerialPort(path, {
    baudRate: 115200
  }, function (err) {
    if (err) {
      return console.log('Error: ', err.message)
    }
  })

let parser = port.pipe(new ByteLength({
  length: 5
}))


parser.on('data', line => {


  charArray = line.toString('binary').split('')
  var uint = charArray.map(l => l.charCodeAt());

  var LEDs = {
    left: shortFromBytes(uint.slice(0, 2)),
    right: shortFromBytes(uint.slice(2, 4))
  }
  var pipe = uint[uint.length - 1] + 1
 if(pipe>4){
   console.log("reset")
   port.close();
   makeParser()
 }

  Object.keys(LEDs).forEach(function(key) {
 // if(Math.random()>.95)console.log(pipe,key,LEDs[key])
    const m = new osc.Message(`/device/${pipe}/${key}`,  LEDs[key]);

    // m.append({
    //
    //   value: LEDs[key]
    // });
    client.send(m, (err) => {
      if (err) {
        console.error(new Error(err));
      }
      //client.close();
    });



  })



})
}

function shortFromBytes( x ){
 //if(Math.random()>.95)console.log(x
    return x[0]+(x[1]<<8)
}


// function bytesToFloat(bytes) {
//   // JavaScript bitwise operators yield a 32 bits integer, not a float.
//   // Assume LSB (least significant byte first).
//   var bits = bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
//   var sign = (bits >>> 31 === 0) ? 1.0 : -1.0;
//   var e = bits >>> 23 & 0xff;
//   var m = (e === 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
//   var f = sign * m * Math.pow(2, e - 150);
//   return f;
// }



//port.write('ROBOT POWER ON\n')
//> ROBOT ONLINE
