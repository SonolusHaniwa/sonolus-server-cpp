const express = require('express');
const fs = require('fs');
function getFiles(path) {
    return fs.readdirSync(path);
}
console.log(__dirname, getFiles(__dirname));
console.log(__dirname + "/../public", getFiles(__dirname + "/../public"));
let wasm = fs.readFileSync(__dirname + '/../public/libsonolus.wasm');
console.log(wasm.length);
var bodyParser = require('body-parser')
const factory = require('../public/libsonolus.js');
const app = express();

const BR = '\r\n';
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())

function requestHeadersToTuples(rawHeaders, headersToSkip = new Set()){
    const tuples = [];
    for (let i = 0; i < rawHeaders.length; i += 2) {
        if (!headersToSkip.has(rawHeaders[i].toLowerCase())) {
            tuples.push({ name: rawHeaders[i], value: rawHeaders[i+1] || '' });
        }
    }
    return tuples;
};

function isJsonFormat(rawHeaders) {
    for (let i = 0; i < rawHeaders.length; i += 2) {
        if (rawHeaders[i].toLowerCase() == "content-type" && rawHeaders[i+1].toLowerCase().includes("application/json")) return true;
    }
    return false;
}

function reconstructRequest(req) {
    const methodLine = `${req.method} ${req.url} HTTP/${req.httpVersion}`;
    const tuples = requestHeadersToTuples(req.rawHeaders);
    var data = req.body;
    if (isJsonFormat(req.rawHeaders)) data = JSON.stringify(data);
    return methodLine + BR + tuples.map(({ name, value }) => `${name}: ${value}`).join(BR) + BR + BR + data;
};

function parseRawResponse(rawResponse, res) {
    if (rawResponse.substr(0, 4) != "HTTP") {
        res.status(500);
        res.send("Invalid response");
        return;
    }

    // 分割响应字符串为行
    const lines = rawResponse.split(BR);
    // 第一行是状态行，包含 HTTP 版本、状态码和状态消息
    const statusLine = lines[0].split(' ');
    const statusCode = statusLine[1];
    // 设置状态码
    res.status(parseInt(statusCode));

    // 从第二行开始是头部，直到遇到一个空行
    let i = 1, len = 0;
    while (lines[i] !== '') {
        const [name, value] = lines[i].split(': ');
        // 设置头部
        res.set(name, value);
        if (name == "Content-Length") len = parseInt(value);
        i++;
    }

    // 剩下的行是正文，再用 Content-Length 截一下
    var body = "";
    for (var j = i + 1; j < lines.length; j++) body += lines[j] + BR;

    // 设置正文
    res.send(body);
    res.end();
}

function randomId() {
    var map = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", res = "";
    for (var i = 0; i < 16; i++) res += map[Math.floor(Math.random() * 62)];
    return res;
}

app.all("*", async (req, res2) => {
	try {
	    var requestId = randomId();
	    console.log("[INFO] [tid:-1] New Connection: " + req.method + " " + req.url + " [" + req.ip + "]");
	    var header = reconstructRequest(req);
	    var inst = await factory();
	    inst.FS.writeFile("/request_" + requestId, header);
	    inst.FS.writeFile("/response_" + requestId, "");
		var ptr = inst.stringToNewUTF8(requestId); // 将字符串转为 char* 指针
		inst._cgi(ptr, requestId.length);
        while (inst.FS.readFile("/response_" + requestId) == "") await new Promise(r => setTimeout(r, 100));
	    var dat = inst.FS.readFile("/response_" + requestId, { encoding: 'utf8' });
	    parseRawResponse(dat, res2);
        if (inst.db != undefined && inst.db != null) inst.db.close();
        if (inst.connection != undefined && inst.connection != null) inst.connection.end();
    } catch (error) {
    	let obj = {
    		error: true,
    		msg: error.message,
    		stack: error.stack
    	}
        console.log(obj)
    	res2.send(obj)
    	res2.end()
    }
});

app.listen(8080, () => {
    console.log("[INFO] [tid:-1] Listening on http://0.0.0.0:8080/");
});