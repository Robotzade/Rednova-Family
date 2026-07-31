const fs = require("fs");

const [inputPath, outputPath, productName, vidText, bootPidText, sketchPidText] = process.argv.slice(2);
if (!inputPath || !outputPath || !productName || !vidText || !bootPidText || !sketchPidText) {
  throw new Error("Usage: node patch_caterina.js input.hex output.hex \"Product Name\" 0xVID 0xBOOT_PID 0xSKETCH_PID");
}

const vid = Number(vidText);
const bootPid = Number(bootPidText);
const sketchPid = Number(sketchPidText);
if (
  !Number.isInteger(vid) || !Number.isInteger(bootPid) || !Number.isInteger(sketchPid) ||
  vid < 0 || vid > 0xffff || bootPid < 0 || bootPid > 0xffff ||
  sketchPid < 0 || sketchPid > 0xffff
) {
  throw new Error("VID and PIDs must be 16-bit numbers");
}

function paddedUtf16(text, length) {
  if (text.length > length) {
    throw new Error(`Text is longer than ${length} characters: ${text}`);
  }
  return Buffer.from(text.padEnd(length, " "), "utf16le");
}

const lines = fs.readFileSync(inputPath, "utf8").trim().split(/\r?\n/);
let upperAddress = 0;
const records = [];
const memory = new Map();

for (const line of lines) {
  if (!line.startsWith(":")) throw new Error(`Invalid Intel HEX line: ${line}`);
  const raw = Buffer.from(line.slice(1), "hex");
  const length = raw[0];
  const offset = raw.readUInt16BE(1);
  const type = raw[3];
  const data = Buffer.from(raw.subarray(4, 4 + length));

  if (type === 0x04) {
    upperAddress = data.readUInt16BE(0) << 16;
  } else if (type === 0x00) {
    for (let i = 0; i < data.length; i++) {
      memory.set(upperAddress + offset + i, data[i]);
    }
  }
  records.push({ length, offset, type, data, upperAddress });
}

function patchAllBytes(search, replacement, label) {
  const addresses = [...memory.keys()].sort((a, b) => a - b);
  let count = 0;
  for (const start of addresses) {
    let matches = true;
    for (let i = 0; i < search.length; i++) {
      if (memory.get(start + i) !== search[i]) {
        matches = false;
        break;
      }
    }
    if (matches) {
      for (let i = 0; i < replacement.length; i++) {
        memory.set(start + i, replacement[i]);
      }
      count++;
    }
  }
  if (count === 0) throw new Error(`${label} descriptor was not found`);
  return count;
}

patchAllBytes(paddedUtf16("Arduino Leonardo", 16), paddedUtf16(productName, 16), "Product");
patchAllBytes(paddedUtf16("Arduino LLC", 11), paddedUtf16("Rednova", 11), "Manufacturer");
patchAllBytes(
  Buffer.from([0x41, 0x23, 0x36, 0x00]),
  Buffer.from([vid & 0xff, vid >> 8, bootPid & 0xff, bootPid >> 8]),
  "Bootloader VID/PID"
);
patchAllBytes(
  Buffer.from([0x41, 0x23, 0x36, 0x80]),
  Buffer.from([vid & 0xff, vid >> 8, sketchPid & 0xff, sketchPid >> 8]),
  "Sketch VID/PID"
);

function encodeRecord(record) {
  let data = record.data;
  if (record.type === 0x00) {
    data = Buffer.alloc(record.length);
    const base = record.upperAddress + record.offset;
    for (let i = 0; i < record.length; i++) data[i] = memory.get(base + i);
  }
  const body = Buffer.alloc(4 + data.length);
  body[0] = data.length;
  body.writeUInt16BE(record.offset, 1);
  body[3] = record.type;
  data.copy(body, 4);
  let sum = 0;
  for (const byte of body) sum = (sum + byte) & 0xff;
  const checksum = (-sum) & 0xff;
  return `:${body.toString("hex").toUpperCase()}${checksum.toString(16).padStart(2, "0").toUpperCase()}`;
}

fs.writeFileSync(outputPath, records.map(encodeRecord).join("\r\n") + "\r\n");
