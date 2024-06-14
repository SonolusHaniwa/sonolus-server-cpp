void quickGUICommunity(http_request request, string type, string name, argvar& argList, string& detailsIcon) {
    bool hasCommunity = appConfig[type + ".enableLike"].asBool()
        || appConfig[type + ".enableComment"].asBool()
        || appConfig[type + ".enableRating"].asBool();
    argList["html.enableCommunity"] = hasCommunity ? "" : "none";
    if (hasCommunity) detailsIcon += fetchIconButton("##COMMUNITY", "{{icon.community}}").output();
    bool isLogin = checkLogin(request);
    bool isLike = isLogin && (db.query("SELECT uid FROM LikeTable WHERE targetType = \"" + type + "\" AND targetName = \"" + name + "\" AND uid = \"" + getUserProfile(request).id + "\"", "LikeTable").size());
    argList["html.community.button"] = "";
    argList["html.community.text"] = "";
    argList["type"] = type;
    argList["name"] = name;
    if (appConfig[type + ".enableLike"].asBool()) {
        if (isLike) argList["html.community.button"] += fetchIconTextButton("unlike('" + type + "', '" + name + "')", "heart", "#DISLIKE").output();
        else argList["html.community.button"] += fetchIconTextButton("like('" + type + "', '" + name + "')", "heartHollow", "#LIKE").output();
        int likes = db.query("SELECT uid FROM LikeTable WHERE targetType = \"" + type + "\" AND targetName = \"" + name + "\"", "LikeTable").size();
        argList["html.community.text"] += ItemCommunityComment("", "System", 0, "Likes: " + to_string(likes), {}).toHTMLObject().output();
    }
    if (appConfig[type + ".enableComment"].asBool()) {
        argList["html.community.button"] += fetchIconTextButton("comment()", "comment", "#COMMENT").output();
        int comments = commentsNumber("targetType = \"" + type + "\" AND targetName = \"" + name + "\"");
        argList["html.community.text"] += ItemCommunityComment("", "System", 0, "Comments: " + 
            to_string(comments) + " / " + to_string((comments - 1) / appConfig[type + ".pageSize.community"].asInt() + 1) + " pages", {}).toHTMLObject().output();
    }
    if (appConfig[type + ".enableRating"].asBool()) {
        argList["html.community.button"] += fetchIconTextButton("rating()", "star", "#RATING").output();
        int ratings = db.query("SELECT uid FROM Rating WHERE targetType = \"" + type + "\" AND targetName = \"" + name + "\"", "Rating").size();
        double rating = atof(db.query("SELECT AVG(rating) FROM Rating WHERE targetType = \"" + type + "\" AND targetName = \"" + name + "\"", "Rating")[0]["AVG(rating)"].c_str());
        argList["html.community.text"] += ItemCommunityComment("", "System", 0, "Rating: " + to_string(rating) + "(" + to_string(ratings) + ")", {}).toHTMLObject().output();
        auto myRating = db.query("SELECT rating FROM Rating WHERE targetType = \"" + type + "\" AND targetName = \"" + name + "\" AND uid = \"" + getUserProfile(request).id + "\"", "Rating");
        int defaultRating = myRating.size() == 0 ? 5 : atoi(myRating[0]["rating"].c_str());
        argList["search.rating"] = to_string(defaultRating);
    }
    argList["comment.list.url"] = "/" + type + "/" + name + "/community/comments/list";
}