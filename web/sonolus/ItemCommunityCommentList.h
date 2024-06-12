auto SonolusCommunityCommentList = [](client_conn conn, http_request request, param argv) {
    auto $_GET = getParam(request);
    int page = atoi($_GET["page"].c_str()) + 1;
    int itemsPerPage = appConfig[string(argv[0]) + ".pageSize.community"].asInt();
    int l = (page - 1) * itemsPerPage + 1, r = page * itemsPerPage;
    int pageCount = (commentsNumber("targetType=\"" + argv[0] + "\" AND targetName=\"" + argv[1] + "\"") - 1) / itemsPerPage + 1;
    auto comments = commentsList("targetType=\"" + argv[0] + "\" AND targetName=\"" + argv[1] + "\"", "time ASC", l, r);
    Json::Value ItemCommunityCommentList;
    ItemCommunityCommentList["pageCount"] = pageCount;
    ItemCommunityCommentList["comments"].resize(0);
    for (int i = 0; i < comments.size(); i++)
        ItemCommunityCommentList["comments"].append(comments[i].toJsonObject());
    quickSendObj(ItemCommunityCommentList);
};