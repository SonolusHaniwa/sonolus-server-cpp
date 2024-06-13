void quickGUICommunity(http_request request, string type, string name, argvar& argList, string& detailsIcon) {
    bool hasCommunity = appConfig[type + ".enableLike"].asBool()
        || appConfig[type + ".enableComment"].asBool()
        || appConfig[type + ".enableRating"].asBool();
    argList["html.enableCommunity"] = hasCommunity ? "" : "none";
    if (hasCommunity) detailsIcon += fetchIconButton("##COMMUNITY", "{{icon.community}}").output();
    bool isLogin = checkLogin(request);
    bool isLike = isLogin && (db.query("SELECT uid FROM LikeTable WHERE targetType = \"" + type + "\" AND targetName = \"" + name + "\" AND uid = \"" + getUserProfile(request).id + "\"").size());
    if (appConfig[type + ".enableLike"].asBool()) {
        if (isLike) argList["html.community.button"] += fetchIconTextButton("unlike()", "heart", "#DISLIKE").output();
        else argList["html.community.button"] += fetchIconTextButton("like()", "heartHollow", "#LIKE").output();
    }
    if (appConfig[type + ".enableComment"].asBool()) argList["html.community.button"] += fetchIconTextButton("comment()", "comment", "#COMMENT").output();
    if (appConfig[type + ".enableRating"].asBool()) argList["html.community.button"] += fetchIconTextButton("rating()", "star", "#RATING").output();
    argList["comment.list.url"] = "/" + type + "/" + name + "/community/comments/list";
}