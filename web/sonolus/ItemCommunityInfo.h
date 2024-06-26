auto SonolusCommunityInfo = [](client_conn conn, http_request request, param argv) {
    auto $_GET = getParam(request);
    bool isLogin = checkLogin(request);
    Json::Value ItemDetails;
    ItemDetails["actions"].resize(0);
    bool isLike = isLogin && (db.query("SELECT uid FROM LikeTable WHERE targetType = \"" + string(argv[0]) + "\" AND targetName = \"" + string(argv[1]) + "\" AND uid = \"" + getUserProfile(request).id + "\"", "LikeTable").size());
    if (appConfig[string(argv[0]) + ".enableLike"].asBool()) ItemDetails["actions"].append((isLike ? unlikeCommunityObject : likeCommunityObject).toJsonObject());
    if (appConfig[string(argv[0]) + ".enableComment"].asBool()) ItemDetails["actions"].append(commentCommunityObject.toJsonObject());
    if (appConfig[string(argv[0]) + ".enableRating"].asBool()) {
        int defaultRating = 5;
        if (isLogin) {
            auto myRating = db.query("SELECT rating FROM Rating WHERE targetType = \"" + string(argv[0]) + "\" AND targetName = \"" + string(argv[1]) + "\" AND uid = \"" + getUserProfile(request).id + "\"", "Rating");
            if (myRating.size() != 0) defaultRating = atoi(myRating[0]["rating"].c_str());
        } Search ratingSearch = ratingCommunityObject;
        ratingSearch.options[0].slider.def = defaultRating;
        ItemDetails["actions"].append(ratingSearch.toJsonObject());
    }
    ItemDetails["topComments"].resize(0);
    if (appConfig[string(argv[0]) + ".enableLike"].asBool()) {
        int likes = db.query("SELECT uid FROM LikeTable WHERE targetType = \"" + string(argv[0]) + "\" AND targetName = \"" + string(argv[1]) + "\"", "LikeTable").size();
        ItemDetails["topComments"].append(ItemCommunityComment("", "System", 0, "Likes: " + to_string(likes), {}).toJsonObject());
    }
    if (appConfig[string(argv[0]) + ".enableComment"].asBool()) {
        int comments = commentsNumber("targetType = \"" + argv[0] + "\" AND targetName = \"" + argv[1] + "\"");
        ItemDetails["topComments"].append(ItemCommunityComment("", "System", 0, "Comments: " + to_string(comments) + " / " + to_string((comments - 1) / appConfig[argv[0] + ".pageSize.community"].asInt() + 1) + " pages", {}).toJsonObject());
    }
    if (appConfig[string(argv[0]) + ".enableRating"].asBool()) {
        int ratings = db.query("SELECT uid FROM Rating WHERE targetType = \"" + string(argv[0]) + "\" AND targetName = \"" + string(argv[1]) + "\"", "Rating").size();
        double rating = atof(db.query("SELECT AVG(rating) FROM Rating WHERE targetType = \"" + string(argv[0]) + "\" AND targetName = \"" + string(argv[1]) + "\"", "Rating")[0]["AVG(rating)"].c_str());
        ItemDetails["topComments"].append(ItemCommunityComment("", "System", 0, "Rating: " + to_string(rating) + "(" + to_string(ratings) + ")", {}).toJsonObject());
    }
    quickSendObj(ItemDetails);
};