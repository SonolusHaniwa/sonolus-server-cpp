auto SonolusCreate = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") quickSendMsg(405);
    if (!checkLogin(request)) quickSendMsg(401);
    
    auto $_POST = argvar();
    if (request.postdata[0] == '{') request.postdata = json_decode(request.postdata)["value"].asString();
	$_POST = postParam(request);
    int raws = 0;
	int id = $_POST["id"] == "" ? -1 : atoi($_POST["id"].c_str());
    string name = $_POST["name"];
    int rating = atoi($_POST["rating"].c_str());
    string title = $_POST["title"];
    string artists = $_POST["artists"];
    string subtitle = $_POST["subtitle"];
    time_t time = atol($_POST["time"].c_str());
    string author = $_POST["author"];
    int engine = $_POST["engine"] == "" ? -1 : atoi($_POST["engine"].c_str());
    int skin = $_POST["skin"] == "" ? -1 : atoi($_POST["skin"].c_str());
    int background = $_POST["background"] == "" ? -1 : atoi($_POST["background"].c_str());
    int effect = $_POST["effect"] == "" ? -1 : atoi($_POST["effect"].c_str());
    int particle = $_POST["particle"] == "" ? -1 : atoi($_POST["particle"].c_str());
    int level = $_POST["level"] == "" ? -1 : atoi($_POST["level"].c_str());
    vector<Tag> tags = deserializeTagString($_POST["tags"]);
    // cout << tags.size() << endl;
    Json::Value levels; json_decode($_POST["levels"] == "" ? "[]" : $_POST["levels"], levels);
    string description = $_POST["description"];
    string localization = $_POST["localization"];
    if (localization == "0" || localization == "") localization = "default";
    else localization = i18n_index[atoi(localization.c_str()) - 1];
    
	if (engine != -1 && enginesNumber("id = " + to_string(engine)) == 0) quickSendMsg(404);
	if (skin != -1 && skin != 0 && skinsNumber("id = " + to_string(skin)) == 0) quickSendMsg(404);
	if (background != -1 && background != 0 && backgroundsNumber("id = " + to_string(background)) == 0) quickSendMsg(404);
	if (effect != -1 && effect != 0 && effectsNumber("id = " + to_string(effect)) == 0) quickSendMsg(404);
	if (particle != -1 && particle != 0 && particlesNumber("id = " + to_string(particle)) == 0) quickSendMsg(404);
	if (level != -1 && levelsNumber("id = " + to_string(level)) == 0) quickSendMsg(404);
	for (int i = 0; i < levels.size(); i++) if (levelsNumber("id = " + levels[i].asString()) == 0) quickSendMsg(404);

	Json::Value obj = msg[200];
    if (argv[0] == "levels") raws = levelsCreate(LevelItem(id,
	    	name, rating, title, artists, author, 
	    	enginesList("id = " + to_string(engine), "")[0],
	    	(skin == 0 ? UseItem<SkinItem>(true, SkinItem()) : UseItem<SkinItem>(false, skinsList("id = " + to_string(skin), "")[0])),
	    	(background == 0 ? UseItem<BackgroundItem>(true, BackgroundItem()) : UseItem<BackgroundItem>(false, backgroundsList("id = " + to_string(background), "")[0])),
	    	(effect == 0 ? UseItem<EffectItem>(true, EffectItem()) : UseItem<EffectItem>(false, effectsList("id = " + to_string(effect), "")[0])),
	    	(particle == 0 ? UseItem<ParticleItem>(true, ParticleItem()) : UseItem<ParticleItem>(false, particlesList("id = " + to_string(particle), "")[0])),
	    	SRL<LevelCover>($_POST["cover"], ""), SRL<LevelBgm>($_POST["bgm"], ""),
	    	SRL<LevelData>($_POST["data"], ""), SRL<LevelPreview>($_POST["preview"], ""),
	    	tags, description
	    ), localization);
	else if (argv[0] == "skins") raws = skinsCreate(SkinItem(id,
			name, title, subtitle, author,
			SRL<SkinThumbnail>($_POST["thumbnail"], ""), SRL<SkinData>($_POST["data"], ""),
			SRL<SkinTexture>($_POST["texture"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "backgrounds") raws = backgroundsCreate(BackgroundItem(id,
			name, title, subtitle, author,
			SRL<BackgroundThumbnail>($_POST["thumbnail"], ""), SRL<BackgroundData>($_POST["data"], ""),
			SRL<BackgroundImage>($_POST["image"], ""), SRL<BackgroundConfiguration>($_POST["configuration"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "effects") raws = effectsCreate(EffectItem(id,
			name, title, subtitle, author,
			SRL<EffectThumbnail>($_POST["thumbnail"], ""), SRL<EffectData>($_POST["data"], ""),
			SRL<EffectAudio>($_POST["audio"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "particles") raws = particlesCreate(ParticleItem(id,
			name, title, subtitle, author,
			SRL<ParticleThumbnail>($_POST["thumbnail"], ""), SRL<ParticleData>($_POST["data"], ""),
			SRL<ParticleTexture>($_POST["texture"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "engines") raws = enginesCreate(EngineItem(id,
	    	name, title, subtitle, author, 
	 		skinsList("id = " + to_string(skin), "")[0],
	    	backgroundsList("id = " + to_string(background), "")[0],
	    	effectsList("id = " + to_string(effect), "")[0],
	    	particlesList("id = " + to_string(particle), "")[0],
	    	SRL<EngineThumbnail>($_POST["thumbnail"], ""),
	    	SRL<EngineData>($_POST["data"], ""),
	    	SRL<EngineTutorialData>($_POST["tutorialData"], ""),
	    	SRL<EnginePreviewData>($_POST["previewData"], ""),
	    	SRL<EngineWatchData>($_POST["watchData"], ""),
	    	SRL<EngineConfiguration>($_POST["configuration"], ""),
	    	tags,
	    	SRL<EngineRom>($_POST["rom"], ""),
	    	description
	    ), localization);
	else if (argv[0] == "replays") raws = replaysCreate(ReplayItem(id,
	    	name, title, subtitle, author, 
	    	levelsList("id = " + to_string(level), "")[0],
	    	SRL<ReplayData>($_POST["data"], ""), SRL<ReplayConfiguration>($_POST["configuration"], ""),
	    	tags, description
	    ), localization);
	else if (argv[0] == "posts") raws = postsCreate(PostItem(id,
			name, title, time, author,
	    	SRL<PostThumbnail>($_POST["thumbnail"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "playlists") raws = playlistsCreate(PlaylistItem(id,
			name, title, subtitle, author, [&](){
				vector<LevelItem> items;
				for (int i = 0; i < levels.size(); i++) items.push_back(levelsList("id = " + levels[i].asString(), "")[0]);
				return items;
			}(), SRL<PlaylistThumbnail>($_POST["thumbnail"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "rooms") {
		Json::Value CreateRoomRequest;
		json_decode(request.postdata, CreateRoomRequest);
		string name = generateSession();
		string key = generateSession();
		auto user = getUserProfile(request);
		raws = roomsCreate(RoomItem(-1, 
			name, user.name + "#" + user.handle + "'s Room", "", user.id, 
			SRL<Unknown>("", ""), 
			SRL<Unknown>("", ""), 
			SRL<Unknown>("", ""),
			vector<Tag>()
		), "default");
		obj = Json::Value();
		obj["name"] = name;
		obj["key"] = name;
		obj["creates"].resize(0);
		db.execute("UPDATE Room SET creatorId = \"" + user.id + "\" WHERE name = \"" + name + "\"", "Room");
		for (int i = 0; i < RoomCreate.size(); i++) obj["creates"].append(RoomCreate[i].toJsonObject());
	}
	else quickSendMsg(404);

	if (raws) { quickSendObj(obj); }
	else { quickSendMsg(400); }
};
