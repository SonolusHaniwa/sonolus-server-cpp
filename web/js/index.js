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