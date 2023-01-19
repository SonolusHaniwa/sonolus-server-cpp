using namespace std;

auto web_levels_create = [](client_conn conn, http_request request, param argv){
    putRequest(conn, 200, __default_response);

    string html = "";
    ifstream fin("./web/html/levels_create.html");
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    for (int i = 0; i < len; i++) html.push_back(ch[i]);
    Section<EngineItem> engines = engineList("");
    Section<SkinItem> skins = skinList("");
    Section<BackgroundItem> backgrounds = backgroundList("");
    Section<EffectItem> effects = effectList("");
    Section<ParticleItem> particles = particleList("");
    stringstream level_engine_options;
    stringstream level_skin_options;
    stringstream level_background_options;
    stringstream level_effect_options;
    stringstream level_particle_options;
    for (int i = 0; i < engines.items.size(); i++)
        level_engine_options << "<option value=\"" << engines.items[i].id << "\">" << engines.items[i].title << "</option>";
    for (int i = 0; i < skins.items.size(); i++)
        level_skin_options << "<option value=\"" << skins.items[i].id << "\">" << skins.items[i].title << "</option>";
    for (int i = 0; i < backgrounds.items.size(); i++)
        level_background_options << "<option value=\"" << backgrounds.items[i].id << "\">" << backgrounds.items[i].title << "</option>";
    for (int i = 0; i < effects.items.size(); i++)
        level_effect_options << "<option value=\"" << effects.items[i].id << "\">" << effects.items[i].title << "</option>";
    for (int i = 0; i < particles.items.size(); i++)
        level_particle_options << "<option value=\"" << particles.items[i].id << "\">" << particles.items[i].title << "</option>";
    html = str_replace("{{ level-engine-options }}", level_engine_options.str(), html);
    html = str_replace("{{ level-skin-options }}", level_skin_options.str(), html);
    html = str_replace("{{ level-background-options }}", level_background_options.str(), html);
    html = str_replace("{{ level-effect-options }}", level_effect_options.str(), html);
    html = str_replace("{{ level-particle-options }}", level_particle_options.str(), html);

    send(conn, html);
    exitRequest(conn);
};