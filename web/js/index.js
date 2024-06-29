var xhr = new XMLHttpRequest();
xhr.upload.onprogress = function(e){};
var xhrOnProgress = function(fun) {
    xhrOnProgress.onprogress = fun;
    return function() {
        var xhr = $.ajaxSettings.xhr();
        if (typeof xhrOnProgress.onprogress !== 'function') return xhr;
        if (xhrOnProgress.onprogress && xhr.upload) xhr.upload.onprogress = xhrOnProgress.onprogress;
        return xhr;
    }
};

async function uploader(context) {
    array = new Uint8Array(context); context = "";
    for (i = 0; i < array.length; i++) context += String.fromCharCode(array[i]);
    context = window.btoa(context); hash = "";
    inner = document.getElementById("uploader-progress");
    inner.style.width = "0%";
    e = document.getElementsByClassName("uploader-progress")[0];
    e.style.opacity = 1; var finish = 0;
    $.ajax({
        url: "/uploader",
        type: "POST",
        async: true,
        data: "file=" + context,
        xhr: xhrOnProgress(function(e){
            var per = 100 * e.loaded / e.total;
            inner.style.width = per + "%";
        }),
        success: function(msg) {
            console.log(msg);
            hash = msg["hash"];
            finish = 1;
        },
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            if (XMLHttpRequest["responseJSON"] != null) alert("Upload Failed, " + XMLHttpRequest["responseJSON"]["msg"]);
            else alert("Upload Failed, " + XMLHttpRequest["status"] + " " + XMLHttpRequest["statusText"]);
            e = document.getElementsByClassName("uploader-progress")[0];
            e.style.opacity = 0;
            return false;
        }
    });
    while(!finish) await sleep(10);
    e = document.getElementsByClassName("uploader-progress")[0];
    e.style.opacity = 0;
    return hash;
}

function addLoadEvent(func) {
    var oldonload = window.onload;
    if (typeof window.onload != 'function') window.onload = func;
    else {  
        window.onload = function() {
            oldonload();
            func();
        };
    }
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

async function displayLanguage() {
    document.getElementById("languageOptions").style.display = "flex";
    await sleep(10);
    document.getElementById("languageOptions").style.opacity = "1";
}

async function hideLanguage() {
    document.getElementById("languageOptions").style.opacity = "0";
    await sleep(150);
    document.getElementById("languageOptions").style.display = "none";
}

function changeLanguage(name) {
    setCookie("lang", name);
    location.href = location.href;
}

async function displayOpenInSonolus(e) {
    generateQRCode();
    document.getElementById("openInSonolus").style.display = "flex";
    await sleep(10);
    document.getElementById("openInSonolus").style.opacity = "1";
    location.href = e.getAttribute("data-href");
}

async function hideOpenInSonolus() {
    document.getElementById("openInSonolus").style.opacity = "0";
    await sleep(150);
    document.getElementById("openInSonolus").style.display = "none";
}

async function displayLogin(e) {
    generateQRCode("qrcode-login");
    document.getElementById("login").style.display = "flex";
    await sleep(10);
    document.getElementById("login").style.opacity = "1";
}

async function hideLogin() {
    document.getElementById("login").style.opacity = "0";
    await sleep(150);
    document.getElementById("login").style.display = "none";
}

async function displayJump() {
    document.getElementById("bottomBar").style.display = "flex";
    await sleep(10);
    document.getElementById("bottomBar").style.opacity = "1";
}

async function hideJump() {
    document.getElementById("bottomBar").style.opacity = "0";
    await sleep(150);
    document.getElementById("bottomBar").style.display = "none";
}

var checkFunc = {};

async function create(path, type, return_path, id = -1) {
    postdata = "type=" + type + "&";
    var ok = true;
    for (index in searchConfig) {
        if (index.substr(0, type.length + 1) != type + "_") continue;
        if (typeof checkFunc[index] == "function" && !checkFunc[index]()) ok = false;
        var tmp_index = index.substr(type.length + 1);
        postdata += tmp_index + "=" + searchConfig[index] + "&";
    }
    if (!ok) return;
    postdata += "id=" + id;
    $.ajax({
        url: path,
        type: "POST",
        async: true,
        data: postdata,
        success: async function(msg) {
            if (msg["code"] != 200) {
                alert("Create Failed, " + msg["msg"]);
                return false;
            } document.getElementsByTagName("main")[0].style.opacity = "0";
            document.getElementsByTagName("nav")[0].style.transform = "translateY(-100%)";
            await sleep(150);
            location.href = return_path + msg["name"]; 
        },
        error: function(XMLHttpRequest, textStatus, errorThrown) {
            if (XMLHttpRequest["responseJSON"] != null) alert("Create Failed, " + XMLHttpRequest["responseJSON"]["msg"]);
            else alert("Create Failed, " + XMLHttpRequest["status"] + " " + XMLHttpRequest["statusText"]);
            return false;
        }
    });
}

async function search(path, type) {
    path += "?"; path += "type=" + type + "&";
    for (index in searchConfig) {
        if (index.substr(0, type.length + 1) != type + "_") continue;
        if (searchConfig[index] == "") continue;
        var tmp_index = index.substr(type.length + 1);
        path += tmp_index + "=" + searchConfig[index] + "&";
    } path = path.substr(0, path.length - 1);
    document.getElementsByTagName("main")[0].style.opacity = "0";
    document.getElementsByTagName("nav")[0].style.transform = "translateY(-100%)";
    await sleep(150);
    location.href = path;
}

function setCookie(cname, cvalue, exdays = 30) {
    var d = new Date();
    d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
    var expires = "expires=" + d.toGMTString();
    document.cookie = cname + "=" + cvalue + "; " + expires + "; path=/;";
}

function deleteCookie(cname) {
    document.cookie = cname + "=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
}

function logout() {
    deleteCookie("sessionId");
    location.href = location.href;
}

const width = window.innerWidth;
const height = window.innerHeight;
const size = Math.min(width, height) * 0.125;
const xCount = Math.ceil(width / size / 2 - 0.5);
const yCount = Math.ceil(height / size / 2 - 0.5);
var nonce = 0;
var items = new Array;
const pointer_events_none = "<div class=\"pointer-events-none\" id=\"pointer-events-none\"></div>";
addLoadEvent(function(){
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
});

/* 图片加载 */
addLoadEvent(function(){
    var img = document.getElementsByTagName("img");
    for (i = 0; i < img.length; i++) {
        img[i].src = img[i].getAttribute("data-src");
        img[i].removeAttribute("data-src");
    }
});

/* 链接事件更新 */
addLoadEvent(async function(){
    var a = document.getElementsByTagName("a");
    for (i = 0; i < a.length; i++) {
        const href = a[i].href;
        if (href.length == 0) continue;
        if (href.substr(0, 11) == "javascript:") continue;
        if (href.substr(0, location.origin.length) == location.origin) continue;
        a[i].onclick = async function() {
            document.getElementsByTagName("main")[0].style.opacity = "0";
            document.getElementsByTagName("nav")[0].style.transform = "translateY(-100%)";
            await sleep(150);
            location.href = href;
        };
        a[i].removeAttribute("href");
    }
});

/* 监听返回页面事件 */
addLoadEvent(async function(){
    window.addEventListener("pageshow", async function(){
        await sleep(10);
        document.getElementsByTagName("main")[0].style.opacity = "1";
        document.getElementsByTagName("nav")[0].style.transform = "translateY(0%)";
    });
});

/* 清除空 Icon */
addLoadEvent(function(){
    var icons = document.getElementsByClassName("tagIcon");
    for (var i = 0; i < icons.length; i++)
        if (icons[i].innerHTML == "") {
            icons[i].style.display = "none";
            icons[i].parentElement.innerHTML = icons[i].outerHTML + icons[i].parentElement.innerText.substr(1);
        }
});

async function displayStudiosSkin() {
    document.getElementById("createStudiosSkin").style.display = "flex";
    await sleep(10);
    document.getElementById("createStudiosSkin").style.opacity = "1";
}

async function hideStudiosSkin() {
    var mouseX = window.event.clientX, mouseY = window.event.clientY;
    var divL = document.getElementById("createStudiosSkinInline").offsetLeft;
    var divT = document.getElementById("createStudiosSkinInline").offsetTop;
    var divR = document.getElementById("createStudiosSkinInline").offsetWidth + divL;
    var divD = document.getElementById("createStudiosSkinInline").offsetHeight + divT;
    if (divL <= mouseX && mouseX <= divR && divT <= mouseY && mouseY <= divD) return false;
    document.getElementById("createStudiosSkin").style.opacity = "0";
    await sleep(150);
    document.getElementById("createStudiosSkin").style.display = "none";
}

async function displayStudiosBackground() {
    document.getElementById("createStudiosBackground").style.display = "flex";
    await sleep(10);
    document.getElementById("createStudiosBackground").style.opacity = "1";
}

async function hideStudiosBackground() {
    var mouseX = window.event.clientX, mouseY = window.event.clientY;
    var divL = document.getElementById("createStudiosBackgroundInline").offsetLeft;
    var divT = document.getElementById("createStudiosBackgroundInline").offsetTop;
    var divR = document.getElementById("createStudiosBackgroundInline").offsetWidth + divL;
    var divD = document.getElementById("createStudiosBackgroundInline").offsetHeight + divT;
    if (divL <= mouseX && mouseX <= divR && divT <= mouseY && mouseY <= divD) return false;
    document.getElementById("createStudiosBackground").style.opacity = "0";
    await sleep(150);
    document.getElementById("createStudiosBackground").style.display = "none";
}
 
async function displayStudiosEffect() {
    document.getElementById("createStudiosEffect").style.display = "flex";
    await sleep(10);
    document.getElementById("createStudiosEffect").style.opacity = "1";
}
 
async function hideStudiosEffect() {
    var mouseX = window.event.clientX, mouseY = window.event.clientY;
    var divL = document.getElementById("createStudiosEffectInline").offsetLeft;
    var divT = document.getElementById("createStudiosEffectInline").offsetTop;
    var divR = document.getElementById("createStudiosEffectInline").offsetWidth + divL;
    var divD = document.getElementById("createStudiosEffectInline").offsetHeight + divT;
    if (divL <= mouseX && mouseX <= divR && divT <= mouseY && mouseY <= divD) return false;
    document.getElementById("createStudiosEffect").style.opacity = "0";
    await sleep(150);
    document.getElementById("createStudiosEffect").style.display = "none";
}

function generateQRCode(id = "qrcode") {
    var qrcode = document.getElementById(id);
    QRCode.toCanvas(qrcode, qrcode.getAttribute("value"), {
        width: 192
    }, function (error) {
        if (error) console.error(error);
        console.log('success!');
    });
}