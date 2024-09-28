#define quickGUILeaderboardRecord(name1, name2) { \
    string filter = str_replace(item["filter"].asString(), itemArgs); \
    string order = str_replace(item["order"].asString(), itemArgs); \
    auto records = recordsList<name1##Item>( \
        item["groupFunction"].asString(), item["groupVariable"].asString(), \
        filter, order, 1, appConfig[defineToString(name2)".pageSize.leaderboard.topRecord"].asInt() \
    ); \
    for (int i = 0; i < records.size(); i++) { \
        argvar args = records[i].fetchParamList(); \
        args["order"] = to_string(i + 1); \
        argvar args2; \
        args2["name"] = records[i].name; \
        args2["rank"] = str_replace(item["constructor"]["rank"].asString(), args); \
        args2["player"] = str_replace(item["constructor"]["player"].asString(), args); \
        args2["value"] = str_replace(item["constructor"]["value"].asString(), args); \
        args2["replays"] = records[i].replay.toHTMLObject().output(); \
        args2["leaderboard"] = item["name"].asString(); \
        single += fetchRecord(args2).output(); \
    } \
}

void quickGUILeaderboard(http_request request, string type, string name, argvar& argList, string& detailsIcon, argvar itemArgs) {
    bool hasLeaderboard = appConfig[type + ".leaderboards"].size();
    argList["html.enableLeaderboard"] = hasLeaderboard ? "" : "none";
    if (hasLeaderboard) detailsIcon += fetchIconButton("##LEADERBOARD", "{{icon.ranking}}").output();
    argList["html.leaderboardSelection"] = "";
    for (int i = 0; i < appConfig[type + ".leaderboards"].size(); i++) {
        auto item = appConfig[type + ".leaderboards"][i];
        argList["html.leaderboardSelection"] += "<option value=\"" + item["name"].asString() + "\" class=\"bg-sonolus-ui-surface\">&nbsp;" + item["title"].asString() + "</option>";
    }
    argList["html.leaderboards"] = "";
    for (int i = 0; i < appConfig[type + ".leaderboards"].size(); i++) {
        string single = "";
        auto item = appConfig[type + ".leaderboards"][i];
        single += "<p>" + item["description"].asString() +"</p>";
        if (type == "levels") { quickGUILeaderboardRecord(Level, levels); }
        else if (type == "skins") { quickGUILeaderboardRecord(Skin, skins); }
        else if (type == "backgrounds") { quickGUILeaderboardRecord(Background, backgrounds); }
        else if (type == "effects") { quickGUILeaderboardRecord(Effect, effects); }
        else if (type == "particles") { quickGUILeaderboardRecord(Particle, particles); }
        else if (type == "engines") { quickGUILeaderboardRecord(Engine, engines); }
        else if (type == "replays") { quickGUILeaderboardRecord(Replay, replays); }
        else if (type == "posts") { quickGUILeaderboardRecord(Post, posts); }
        else if (type == "playlists") { quickGUILeaderboardRecord(Playlist, playlists); }
        else if (type == "rooms") { quickGUILeaderboardRecord(Room, rooms); }
        single += 
        "<button class=\"flex select-none space-x-2 p-2 transition-colors sm:space-x-3 sm:p-3 cursor-pointer bg-sonolus-ui-button-normal hover:bg-sonolus-ui-button-highlighted active:bg-sonolus-ui-button-pressed\" style=\"margin: auto; margin-top: 10px;\" onclick=\"location.href='/" + type + "/" + name + "/leaderboard/" + item["name"].asString() + "/list'\" id=\"info-more\">"
            "<div style=\"width:24px;fill:white\">{{icon.more}}</div>"
            "<span class=\"min-w-[4.5rem] flex-grow text-center sm:min-w-[6rem]\">{{language.more}}</span>"
        "</button>";
        argList["html.leaderboards"] += "<div class=\"" + string(i ? "leaderboard2" : "leaderboard1") + "\" id=\"leaderboard-" + item["name"].asString() + "\">" + single + "</div>";

    }
}