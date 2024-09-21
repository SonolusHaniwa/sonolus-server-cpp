#pragma onve
using namespace std;

class UserProfile {
    public:

	string id = "";
	string handle = "-1";
	string name = "";
	string avatarForegroundColor = "";
	string avatarBackgroundColor = "";
	string aboutMe = "";
	vector<pair<string, string> > socialLinks = {};
	vector<string> favorites = {};

    UserProfile(){}
    UserProfile(string id, string handle, string name, 
    	string avatarForegroundColor, string avatarBackgroundColor, string aboutMe,
    	vector<pair<string, string> > socialLinks, vector<string> favorites):
    	id(id), handle(handle), name(name), 
    	avatarForegroundColor(avatarForegroundColor), avatarBackgroundColor(avatarBackgroundColor), aboutMe(aboutMe),
    	socialLinks(socialLinks), favorites(favorites){}
    UserProfile(Json::Value arr) {
		id = arr["id"].asString();
		handle = arr["handle"].asString();
		name = arr["name"].asString();
		avatarForegroundColor = arr["avatarForegroundColor"].asString();
		avatarBackgroundColor = arr["avatarBackgroundColor"].asString();
		aboutMe = arr["aboutMe"].asString();
		for (int i = 0; i < arr["socialLinks"].size(); i++) socialLinks.push_back(
			{ arr["socialLinks"][i]["title"].asString(), arr["socialLinks"][i]["address"].asString() });
		for (int i = 0; i < arr["favorites"].size(); i++) favorites.push_back(arr["favorites"][i].asString());
    }

    Json::Value toJsonObject() {
    	Json::Value obj;
    	obj["id"] = id;
    	obj["handle"] = handle;
    	obj["name"] = name;
    	obj["avatarForegroundColor"] = avatarForegroundColor;
    	obj["avatarBackgroundColor"] = avatarBackgroundColor;
    	obj["aboutMe"] = aboutMe;
    	Json::Value arr;
    	for (int i = 0; i < socialLinks.size(); i++) {
    		Json::Value item;
    		item["title"] = socialLinks[i].first;
    		item["address"] = socialLinks[i].second;
    		arr.append(item);
    	} obj["socialLinks"] = arr;
    	arr = Json::Value();
    	for (int i = 0; i < favorites.size(); i++) arr.append(favorites[i]);
    	obj["favorites"] = arr;
    	return obj;
    }
};

int usersNumber(string filter) {
    itemNumberTemplate(UserProfile, filter);
    dbres res = db.query(sql.c_str(), "UserProfile");
    return atoi(res[0]["sum"].c_str());
}

vector<UserProfile> usersList(string filter, string order, int st = 1, int en = 20) {
    if (order == "") order = "id ASC";
	string sql = "SELECT * FROM UserProfile WHERE (" + filter + ") ORDER BY " + order + " LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);      

    auto res = db.query(sql.c_str(), "UserProfile");
    vector<UserProfile> list = {};
    
    for (int i = 0; i < res.size(); i++) {
    	vector<pair<string, string> > socialLinks;
		vector<string> favorites;
		Json::Value obj; json_decode(res[i]["socialLinks"], obj);
		for (int j = 0; j < obj.size(); j++) {
            if (obj[j] == Json::objectValue) socialLinks.push_back({obj[j]["title"].asString(), obj[j]["address"].asString()});
            else if (obj[j] == Json::arrayValue) socialLinks.push_back({obj[j][0].asString(), obj[j][1].asString()});
        }
		json_decode(res[i]["favorites"], obj);
		for (int j = 0; j < obj.size(); j++) favorites.push_back(obj[j].asString());
    	UserProfile data = UserProfile(
    		res[i]["id"],
    		res[i]["handle"],
    		res[i]["name"],
    		res[i]["avatarForegroundColor"],
    		res[i]["avatarBackgroundColor"],
    		res[i]["aboutMe"],
    		socialLinks,
    		favorites
    	);
        list.push_back(data);
    } return list;
}

int usersCreate(UserProfile item) {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM UserProfile WHERE id = \"" + item.id + "\"", "UserProfile");
    Json::Value obj = Json::Value();
    for (int i = 0; i < item.socialLinks.size(); i++) obj[i][0] = item.socialLinks[i].first, obj[i][1] = item.socialLinks[i].second;
    string socialLinks = json_encode(obj);
    obj = Json::Value();
    for (int i = 0; i < item.favorites.size(); i++) obj[i] = item.favorites[i];
    string favorites = json_encode(obj);
    string aboutMe = quote_encode(item.aboutMe);
    socialLinks = quote_encode(socialLinks);
    favorites = quote_encode(favorites);
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE UserProfile SET id=\"" << item.id << "\", handle=\"" << item.handle << "\", name=\"" << item.name;
        sqlbuffer << "\", avatarForegroundColor=\"" << item.avatarForegroundColor << "\", avatarBackgroundColor=\"" << item.avatarBackgroundColor;
        sqlbuffer << "\", aboutMe=\"" << aboutMe << "\", socialLinks=\"" << socialLinks << "\", favorites = \"" << favorites << "\" WHERE id=" << id;
    } else {
        sqlbuffer << "INSERT INTO UserProfile (id, handle, name, avatarForegroundColor, avatarBackgroundColor, aboutMe, socialLinks, favorites) VALUES (";
        sqlbuffer << "\"" << item.id << "\", \"" << item.handle << "\",  \"" << item.name << "\", ";
        sqlbuffer << "\"" << item.avatarForegroundColor << "\", \"" << item.avatarBackgroundColor << "\", ";
        sqlbuffer << "\"" << aboutMe << "\", \"" << socialLinks << "\", \"" << favorites << "\")";
    } return db.execute(sqlbuffer.str(), "UserProfile");
}