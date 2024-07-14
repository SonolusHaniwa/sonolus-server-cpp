auto SonolusCommunityCommentSubmit = [](client_conn conn, http_request request, param argv) {
    request.postdata = json_decode(request.postdata)["values"].asString();
    auto $_POST = postParam(request);
    if (!checkLogin(request)) quickSendMsg(401);
    UserProfile profile = getUserProfile(request);
    Json::Value SubmitItemCommunityActionResponse;
    SubmitItemCommunityActionResponse["shouldUpdateCommunity"] = true;
    if ($_POST["type"] == "reply" && appConfig[argv[0] + ".enableComment"].asBool()) {
        string commentName = argv[2];
        ItemCommunityComment original = commentsList("name = \"" + commentName + "\"", "time ASC", 1, 1)[0];
        string content = str_replace("\n", "\n> ", "> " + original.content) + "\n\n" + urldecode($_POST["content"]);
        content = str_replace("\"", "\"\"", content);
        string name = getRef();
        db.execute("INSERT INTO Comment (name, targetType, targetName, uid, time, content) VALUES (\"" + name + "\", \"" + argv[0] + "\", \"" + argv[1] + "\", \"" + profile.id + "\", " + to_string(getMilliSeconds()) + ", \"" + content + "\")", "Comment");
        int comments = commentsNumber("targetType = \"" + argv[0] + "\" AND targetName = \"" + argv[1] + "\"");
        SubmitItemCommunityActionResponse["shouldNavigateCommentsToPage"] = (comments - 1) / appConfig[argv[0] + ".pageSize.community"].asInt();
    }
    SubmitItemCommunityActionResponse["key"] = "";
    SubmitItemCommunityActionResponse["hashes"].resize(0);
    quickSendObj(SubmitItemCommunityActionResponse);
};