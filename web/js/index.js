async function uploader(context) {
    array = new Uint8Array(context);
    context = "";
    for (i = 0; i < array.length; i++) context += String.fromCharCode(array[i]);
    context = window.btoa(context);
    console.log(context);
    hash = "";
    $.ajax({
        url: "/uploader",
        type: "POST", 
        async: false,
        data: "file=" + context,
        success: function(msg) {
            msg = JSON.parse(msg);
            console.log(msg);
            hash = msg["hash"];
        }
    });
    return hash;
}

function sleep(milliseconds) {
    return new Promise(resolve => {
        setTimeout(() => {
            resolve();
        }, milliseconds);
    });
}

async function readBinaryFile(FileObject) {
    var reader = new FileReader();
    var result; var finish = false;
    reader.readAsArrayBuffer(FileObject);
    reader.onload = function(e) {
        finish = true;
        result = this.result;
    }
    while (!finish) await sleep(10);
    return result;
}

async function upload_level() {
    var name = document.getElementById("level-name").value;
    var rating = document.getElementById("level-rating").value;
    var title = document.getElementById("level-title").value;
    var artists = document.getElementById("level-artists").value;
    var author = document.getElementById("level-author").value;
    var engine = document.getElementById("level-engine").value;
    var skin = document.getElementById("level-skin").value;
    var background = document.getElementById("level-background").value;
    var effect = document.getElementById("level-effect").value;
    var particle = document.getElementById("level-particle").value;
    var cover = await uploader(await readBinaryFile(document.getElementById("level-cover").files[0]));
    var bgm = await uploader(await readBinaryFile(document.getElementById("level-bgm").files[0]));
    var data = await uploader(await readBinaryFile(document.getElementById("level-data").files[0]));
    var postdata = "name=" + name + "&rating=" + rating + "&title=" + title +
    "&artists=" + artists + "&author=" + author + "&engine=" + engine + "&skin=" + skin +
    "&background=" + background + "&effect=" + effect + "&particle=" + particle +
    "&cover=" + cover + "&bgm=" + bgm + "&data=" + data;
    $.ajax({
        url: "/sonolus/levels/create",
        type: "POST", 
        data: postdata,
        success: function(msg) {
            console.log(msg);
            alert(msg["msg"]);
        }
    });
}

async function displayLanguage() {
    document.getElementById("languageOptions").style.display = "flex";
    await sleep(10);
    document.getElementById("languageOptions").style.opacity = "1";
}

async function hideLanguage() {
    document.getElementById("languageOptions").style.opacity = "0";
    await sleep(200);
    document.getElementById("languageOptions").style.display = "none";
}

function setCookie(cname, cvalue, exdays = 30) {
    var d = new Date();
    d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
    var expires = "expires=" + d.toGMTString();
    document.cookie = cname + "=" + cvalue + "; " + expires;
}

function changeLanguage(name) {
    setCookie("lang", name);
    location.href = location.href;
}

const width = window.innerWidth;
const height = window.innerHeight;
const size = Math.min(width, height) * 0.125;
const xCount = Math.ceil(width / size / 2 - 0.5);
const yCount = Math.ceil(height / size / 2 - 0.5);
let nonce = 0;
let items = new Array;
const pointer_events_none = "<div class=\"pointer-events-none\" id=\"pointer-events-none\"></div>";
window.onload = function(){
    document.getElementById("app").innerHTML = pointer_events_none + document.getElementById("app").innerHTML;
    setInterval(async function(){
        const time = Date.now();
        while (items.length && items[0].endTime > time) items.shift();
        const x = Math.floor(Math.random() * (2 * xCount + 1) - xCount);
        const y = Math.floor(Math.random() * (2 * yCount + 1) - yCount);
        if (items.some(function(arg){return arg.x == x && arg.y == y;})) return;
        const duration = (0.5 + Math.random()) * 2;
        const value = Math.random() * 0.25;
        const id = nonce++;
        items.push({
            id: id,
            x: x,
            y: y,
            duration: duration,
            value: value,
            endTime: time + duration * 1000,
        })
        e = document.createElement("div");
        e.className = "glow fixed -z-50 bg-sonolus-glow";
        e.style =
            "left: " + (width / 2 + (x - 0.5) * size) + "px; " +
            "top: " + (height / 2 + (y - 0.5) * size) + "px; " +
            "width: " + size + "px; " + 
            "height: " + size + "px; " +
            "animation-duration: " + duration + "s; " +
            "--scale: " + (0.75 + value) + "; " +
            "--brightness: " + value + ";"
        ;
        e.id = "background-block-" + id;
        document.getElementById("pointer-events-none").append(e);
        await sleep(duration * 1000);
        e = document.getElementById("background-block-" + id);
        document.getElementById("pointer-events-none").removeChild(e);
    }, 100);
}