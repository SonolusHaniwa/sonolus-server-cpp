async function communityActions(type, name, value) {
    let response = await fetch("/sonolus/" + type + "/" + name + "/community", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ values: value })
    });
    if (response.ok) {
        let json = await response.json();
        if (json.shouldNavigateCommentsToPage != undefined)
            location.href = "/" + type + "/" + name + "/community/comments/list?page=" + json.shouldNavigateCommentsToPage;
        else if (json.shouldUpdateCommunity == true) location.href = location.href;
    } else {
        let json = await response.json();
        alert(json.msg);
    }
}

function like(type, name) {
    communityActions(type, name, "type=like");
}

function unlike(type, name) {
    communityActions(type, name, "type=dislike")
}

async function comment() {
    document.getElementById("commentInterface").style.visibility = "";
    await sleep(10);
    document.getElementById("commentInterface").style.opacity = 1;
}
async function closeComment() {
    document.getElementById("commentInterface").style.opacity = 0;
    await sleep(150);
    document.getElementById("commentInterface").style.visibility = "hidden";
}
async function sendComment(type, name) {
    communityActions(type, name, "type=comment&content=" + searchConfig["content"]);
}

async function rating() {
    document.getElementById("ratingInterface").style.visibility = "";
    await sleep(10);
    document.getElementById("ratingInterface").style.opacity = 1;
}
async function closeRating() {
    document.getElementById("ratingInterface").style.opacity = 0;
    await sleep(150);
    document.getElementById("ratingInterface").style.visibility = "hidden";
}
async function sendRating(type, name) {
    communityActions(type, name, "type=rating&rating=" + searchConfig["rating"]);
}

async function reply(name) {
    document.getElementById(name).style.visibility = "";
    await sleep(10);
    document.getElementById(name).style.opacity = 1;
}
async function closeReply(name) {
    document.getElementById(name).style.opacity = 0;
    await sleep(150);
    document.getElementById(name).style.visibility = "hidden";
}
async function sendReply(type, name, commentId) {
    communityActions(type, name, "type=reply-" + commentId + "&content=" + searchConfig["content-" + commentId]);
}