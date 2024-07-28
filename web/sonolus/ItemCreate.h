auto SonolusCreate = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") quickSendMsg(405);
    if (!checkLogin(request)) quickSendMsg(401);
    
	Json::Value CreateItemResponse;
    if (request.postdata[0] == '{') request.postdata = json_decode(request.postdata)["values"].asString();
	auto $_POST = postParam(request);
	auto $_GET = getParam(request);
	for (auto v : $_POST) $_POST[v.first] = urldecode(v.second);
	int id = -1;
	for (int i = 0; i < appConfig[argv[0] + ".creates"].size(); i++)
		if (appConfig[argv[0] + ".creates"][i]["type"].asString() == $_POST["type"]) id = i;
	if (argv[0] == "rooms") id = 0;
	if (id == -1) quickSendMsg(404);
	auto CreateItem = appConfig[argv[0] + ".creates"][id];
	CreateItemResponse["hashes"].resize(0);
	CreateItemResponse["key"] = generateSession();
	for (int i = 0; i < CreateItem["options"].size(); i++) if (CreateItem["options"][i]["type"] == "file")
		if ($_POST[CreateItem["options"][i]["query"].asString()] != "")
			CreateItemResponse["hashes"].append($_POST[CreateItem["options"][i]["query"].asString()]);
	
	string localization = $_GET["localization"] == "" ? appConfig["language.default"].asString() : $_GET["localization"];
	argvar argList = argResolver($_POST, CreateItem["options"], localization);

	Json::Value::Members members = CreateItem["values"].getMemberNames();
	argvar args = argvar();
	for (int i = 0; i < members.size(); i++) args[members[i]] = str_replace(CreateItem["values"][members[i]].asString(), argList);
	for (auto v : args) if (v.first != "levels") args[v.first] = quote_encode(v.second);

    int raws = 0;
	id = args["id"] == "" ? -1 : atoi(args["id"].c_str());
    string name = args["name"];
    int rating = atoi(args["rating"].c_str());
    string title = args["title"];
    string artists = args["artists"];
    string subtitle = args["subtitle"];
    time_t time = getMilliSeconds();
    string author = args["author"];
    string engine = args["engine"];
    string skin = args["skin"];
    string background = args["background"];
    string effect = args["effect"];
    string particle = args["particle"];
    string level = args["level"];
    vector<Tag> tags = deserializeTagString(args["tags"]);
    Json::Value levels; json_decode(args["levels"] == "" ? "[]" : args["levels"], levels);
    string description = args["description"];
    localization = args["localization"];
    
	if (engine != "" && enginesNumber("name = \"" + engine + "\"") == 0) quickSendMsg(404);
	if (skin != "" && skinsNumber("name = \"" + skin + "\"") == 0) quickSendMsg(404);
	if (background != "" && backgroundsNumber("name = \"" + background + "\"") == 0) quickSendMsg(404);
	if (effect != "" && effectsNumber("name = \"" + effect + "\"") == 0) quickSendMsg(404);
	if (particle != "" && particlesNumber("name = \"" + particle + "\"") == 0) quickSendMsg(404);
	if (level != "" && levelsNumber("name = \"" + level + "\"") == 0) quickSendMsg(404);
	for (int i = 0; i < levels.size(); i++) if (levelsNumber("name = \"" + levels[i].asString() + "\"") == 0) quickSendMsg(404);

    if (argv[0] == "levels") raws = levelsCreate(LevelItem(id,
	    	name, rating, title, artists, author, 
	    	enginesList("name = \"" + engine + "\"", "")[0],
	    	(skin == "" ? UseItem<SkinItem>(true, SkinItem()) : UseItem<SkinItem>(false, skinsList("name = \"" + skin + "\"", "")[0])),
	    	(background == "" ? UseItem<BackgroundItem>(true, BackgroundItem()) : UseItem<BackgroundItem>(false, backgroundsList("name = \"" + background + "\"", "")[0])),
	    	(effect == "" ? UseItem<EffectItem>(true, EffectItem()) : UseItem<EffectItem>(false, effectsList("name = \"" + effect + "\"", "")[0])),
	    	(particle == "" ? UseItem<ParticleItem>(true, ParticleItem()) : UseItem<ParticleItem>(false, particlesList("name = \"" + particle + "\"", "")[0])),
	    	SRL<LevelCover>(args["cover"], ""), SRL<LevelBgm>(args["bgm"], ""),
	    	SRL<LevelData>(args["data"], ""), SRL<LevelPreview>(args["preview"], ""),
	    	tags, description
	    ), localization);
	else if (argv[0] == "skins") raws = skinsCreate(SkinItem(id,
			name, title, subtitle, author,
			SRL<SkinThumbnail>(args["thumbnail"], ""), SRL<SkinData>(args["data"], ""),
			SRL<SkinTexture>(args["texture"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "backgrounds") raws = backgroundsCreate(BackgroundItem(id,
			name, title, subtitle, author,
			SRL<BackgroundThumbnail>(args["thumbnail"], ""), SRL<BackgroundData>(args["data"], ""),
			SRL<BackgroundImage>(args["image"], ""), SRL<BackgroundConfiguration>(args["configuration"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "effects") raws = effectsCreate(EffectItem(id,
			name, title, subtitle, author,
			SRL<EffectThumbnail>(args["thumbnail"], ""), SRL<EffectData>(args["data"], ""),
			SRL<EffectAudio>(args["audio"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "particles") raws = particlesCreate(ParticleItem(id,
			name, title, subtitle, author,
			SRL<ParticleThumbnail>(args["thumbnail"], ""), SRL<ParticleData>(args["data"], ""),
			SRL<ParticleTexture>(args["texture"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "engines") raws = enginesCreate(EngineItem(id,
	    	name, title, subtitle, author, 
	 		skinsList("name = \"" + skin + "\"", "")[0],
	    	backgroundsList("name = \"" + background + "\"", "")[0],
	    	effectsList("name = \"" + effect + "\"", "")[0],
	    	particlesList("name = \"" + particle + "\"", "")[0],
	    	SRL<EngineThumbnail>(args["thumbnail"], ""),
	    	SRL<EngineData>(args["data"], ""),
	    	SRL<EngineTutorialData>(args["tutorialData"], ""),
	    	SRL<EnginePreviewData>(args["previewData"], ""),
	    	SRL<EngineWatchData>(args["watchData"], ""),
	    	SRL<EngineConfiguration>(args["configuration"], ""),
	    	tags,
	    	SRL<EngineRom>(args["rom"], ""),
	    	description
	    ), localization);
	else if (argv[0] == "replays") raws = replaysCreate(ReplayItem(id,
	    	name, title, subtitle, author, 
	    	levelsList("name = \"" + level + "\"", "")[0],
	    	SRL<ReplayData>(args["data"], ""), SRL<ReplayConfiguration>(args["configuration"], ""),
	    	tags, description
	    ), localization);
	else if (argv[0] == "posts") raws = postsCreate(PostItem(id,
			name, title, time, author,
	    	SRL<PostThumbnail>(args["thumbnail"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "playlists") raws = playlistsCreate(PlaylistItem(id,
			name, title, subtitle, author, [&](){
				vector<LevelItem> items;
				for (int i = 0; i < levels.size(); i++) items.push_back(levelsList("name = \"" + levels[i].asString() + "\"", "")[0]);
				return items;
			}(), SRL<PlaylistThumbnail>(args["thumbnail"], ""),
	    	tags, description
		), localization);
	else if (argv[0] == "rooms") {
		Json::Value CreateRoomRequest;
		json_decode(request.postdata, CreateRoomRequest);
		string name = generateSession();
		string key = generateSession();
		auto user = getUserProfile(request);
		raws = roomsCreate(RoomItem(id, 
			name, user.name + "#" + user.handle + "'s Room", "", user.id, 
			SRL<Unknown>("", ""), 
			SRL<Unknown>("", ""), 
			SRL<Unknown>("", ""),
			vector<Tag>()
		), "default");
		CreateItemResponse = Json::Value();
		CreateItemResponse["name"] = name;
		CreateItemResponse["key"] = key;
		db.execute("UPDATE Room SET creatorId = \"" + user.id + "\" WHERE name = \"" + name + "\"", "Room");
	}
	else quickSendMsg(404);

	if (argv[0] != "rooms") CreateItemResponse["name"] = name;
	CreateItemResponse["code"] = 200;
	if (raws) { quickSendObj(CreateItemResponse); }
	else { quickSendMsg(400); }
};
