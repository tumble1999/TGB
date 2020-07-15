const fs = require('fs');
var o = "";
var opcodesJson = require("./opcodes.json").unprefixed;
var functions = [];

function paramSort(p) {
	p = p.replace("(","$").replace(")","");
	if(p.endsWith("+")) p = p.replace("+","I");
	if(p.endsWith("-")) p = p.replace("-","D");
	return p.replace("+","_").replace("-","_")
}

for (let i = 0; i < 0x100; i++) {
	var addr = "0x"+i.toString(16)
	var instr = opcodesJson[addr];
	if(!instr) {
		o += `\n{},`;
		functions.push("NULL");
		continue;
	}
	var params = [];
	if(instr.operand1) params.push(paramSort(instr.operand1));
	if(instr.operand2) params.push(paramSort(instr.operand2));
	var fun = instr.mnemonic;
	if(params.length>0) {
		fun += "_" + params.join("_");
		 params = '"'+params.join('","')+'"';
	}
	functions.push(fun);
	var flags = instr.flags.map(f=>{
		switch (f) {
			case "Z":
			case "N":
			case "H":
			case "C":
				return 2;
			case "1":
				return 1;
			case "0":
				return 0;
			default:
				return -1;
		}
	}).join(",");
	// o += `\n{"${instr.mnemonic}",{${params}},${instr.length-1},{${flags}},${instr.mnemonic}},`
	o += `\n{"${instr.mnemonic}",{${params}},${instr.length-1},{${flags}},&GB::${fun}},`
}
o+="\n//====================="
var f;
for (var a = 0;a<functions.length;a++) {
	f = functions[a];
	if(f=="NULL") continue;
	o += "\nvoid " + f + "(); // 0x" + a.toString(16);
}
o+="\n//====================="

for (var a = 0;a<functions.length;a++) {
	f = functions[a];
	if(f=="NULL") continue;
	o += "\nvoid GB::" + f + "() { // 0x" + a.toString(16) + "\n\n}\n";
}

fs.writeFile("hmm.txt",o,function(){})

//console.log(o);