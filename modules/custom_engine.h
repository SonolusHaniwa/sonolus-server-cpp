using namespace std;

string package_json = 
"{\n"
"  \"name\": \"{{dir_name}}\",\n"
"  \"version\": \"0.0.1\",\n"
"  \"description\": \"\",\n"
"  \"title\": \"\",\n"
"  \"subtitle\": \"\",\n"
"  \"author\": \"\",\n"
"  \"skin\": \"\",\n"
"  \"background\": \"\",\n"
"  \"effect\": \"\",\n"
"  \"particle\": \"\",\n"
"  \"repository\": \"github:\",\n"
"  \"license\": \"MIT\",\n"
"  \"keywords\": [\n"
"    \"Sonolus\"\n"
"  ],\n"
"  \"main\": \"dist\",\n"
"  \"files\": [\n"
"    \"dist\"\n"
"  ],\n"
"  \"scripts\": {\n"
"    \"build\": \"ts-node ./src/build && tsc -p ./tsconfig.lib.json\"\n"
"  },\n"
"  \"devDependencies\": {\n"
"    \"@tsconfig/node16\": \"^1.0.3\",\n"
"    \"@types/fs-extra\": \"^9.0.13\",\n"
"    \"fs-extra\": \"^10.1.0\",\n"
"    \"prettier\": \"^2.7.1\",\n"
"    \"prettier-plugin-organize-imports\": \"^3.2.0\",\n"
"    \"sonolus-core\": \"^6.0.0\",\n"
"    \"sonolus.js\": \"^8.0.0\",\n"
"    \"ts-node\": \"^10.9.1\",\n"
"    \"ts-node-dev\": \"^2.0.0\",\n"
"    \"typescript\": \"^4.9.3\"\n"
"  }\n"
"}";
string tsconfig_lib_json = 
"{\n"
"  \"extends\": \"./tsconfig\",\n"
"  \"compilerOptions\": {\n"
"    \"outDir\": \"./dist\",\n"
"    \"rootDir\": \"./src/lib\",\n"
"    \"declaration\": true\n"
"  },\n"
"  \"include\": [\n"
"    \"./src/lib\"\n"
"  ]\n"
"}";
string tsconfig_json = 
"{\n"
"  \"extends\": \"@tsconfig/node16/tsconfig.json\"\n"
"}";
string index_ts = 
"import { build } from 'sonolus.js'\n"
"import { engineConfiguration } from './engine/configuration'\n"
"import { engineData } from './engine/data'\n"
"\n"
"export const buildOutput = build({\n"
"    engine: {\n"
"        configuration: engineConfiguration,\n"
"        data: engineData,\n"
"    },\n"
"\n"
"    level: {\n"
"        data: {\n"
"            bgmOffset: 0,\n"
"            entities: []\n"
"        },\n"
"    },\n"
"})";
string build_ts = 
"import { copySync, emptyDirSync, outputFileSync, outputJsonSync } from 'fs-extra'\n"
"import { buildOutput } from '.'\n"
"import { archetypes } from './engine/data/archetypes'\n"
"\n"
"const distPath = './dist'\n"
"\n"
"emptyDirSync(distPath)\n"
"copySync('./src/res', distPath)\n"
"\n"
"outputFileSync(`${distPath}/EngineConfiguration`, buildOutput.engine.configuration.buffer)\n"
"\n"
"outputFileSync(`${distPath}/EngineData`, buildOutput.engine.data.buffer)\n"
"\n"
"outputJsonSync(\n"
"    `${distPath}/archetypes.json`,\n"
"    Object.fromEntries(Object.entries(archetypes).filter(([key]) => key.endsWith('Index')))\n"
")";
string lib_index_ts = 
"import { EngineInfo, LevelData } from 'sonolus-core'\n"
"import { Resource } from './Resource'\n"
"\n"
"const archetypes = require('./archetypes')\n"
"\n"
"export const version = ''\n"
"\n"
"export const engineInfo = {\n"
"    name: '{{dir_name}}',\n"
"    version: 7,\n"
"    title: {\n"
"        en: ''\n"
"    },\n"
"    subtitle: {\n"
"        en: ''\n"
"    },\n"
"    author: {\n"
"        en: '',\n"
"    },\n"
"    description: {\n"
"        en: [\n"
"\n"
"        ].join('\\n'),\n"
"    },\n"
"} as const satisfies Partial<EngineInfo>\n"
"\n"
"export const engineConfiguration = new Resource('EngineConfiguration')\n"
"export const engineData = new Resource('EngineData')\n"
"export const engineThumbnail = new Resource('thumbnail.png')";
string lib_Resource_ts =
"import { createHash } from 'crypto'\n"
"import { readFileSync } from 'fs'\n"
"import { resolve } from 'path'\n"
"\n"
"export class Resource {\n"
"    public readonly path: string\n"
"\n"
"    private _hash?: string\n"
"    private _buffer?: Buffer\n"
"\n"
"    public constructor(path: string) {\n"
"        this.path = resolve(__dirname, path)\n"
"    }\n"
"\n"
"    public get hash(): string {\n"
"        if (!this._hash) {\n"
"            this._hash = createHash('sha1').update(this.buffer).digest('hex')\n"
"        }\n"
"\n"
"        return this._hash\n"
"    }\n"
"\n"
"    public get buffer(): Buffer {\n"
"        if (!this._buffer) {\n"
"            this._buffer = readFileSync(this.path)\n"
"        }\n"
"\n"
"        return this._buffer\n"
"    }\n"
"}\n";

void writeFile(string path, string content) {
    ofstream fout(path.c_str());
    fout << content;
    fout.close();
}

void touch(string path) {
    ofstream fout(path.c_str());
    fout.close();
}

void MKDIR(string path, int mode = 0777) {
    #ifdef __linux__
    mkdir(path.c_str(), mode);
    #else
    _mkdir(path.c_str());
    #endif
}

void initCustomEngine(char** argv) {
    string root_dir = string(argv[2]);
    MKDIR((root_dir + "/").c_str(), 0777);
    MKDIR((root_dir + "/src").c_str(), 0777);
    MKDIR((root_dir + "/src/lib").c_str(), 0777);
    MKDIR((root_dir + "/src/res").c_str(), 0777);
    MKDIR((root_dir + "/src/engine").c_str(), 0777);
    MKDIR((root_dir + "/src/engine/configuration").c_str(), 0777);
    MKDIR((root_dir + "/src/engine/data").c_str(), 0777);
    writeFile((root_dir + "/package.json").c_str(), str_replace("{{dir_name}}", argv[2], package_json));
    writeFile((root_dir + "/tsconfig.json").c_str(), tsconfig_json);
    writeFile((root_dir + "/tsconfig.lib.json").c_str(), tsconfig_lib_json);
    writeFile((root_dir + "/src/build.ts").c_str(), build_ts);
    writeFile((root_dir + "/src/index.ts").c_str(), index_ts);
    writeFile((root_dir + "/src/lib/index.ts").c_str(), str_replace("{{dir_name}}", argv[2], lib_index_ts));
    writeFile((root_dir + "/src/lib/Resource.ts").c_str(), lib_Resource_ts);
    touch((root_dir + "/src/res/thumbnail.jpg").c_str());
    touch((root_dir + "/src/engine/configuration/index.ts").c_str());
    touch((root_dir + "/src/engine/configuration/options.ts").c_str());
    touch((root_dir + "/src/engine/configuration/ui.ts").c_str());
    touch((root_dir + "/src/engine/data/archetypes.ts").c_str());
    touch((root_dir + "/src/engine/data/buckets.ts").c_str());
    touch((root_dir + "/src/engine/data/index.ts").c_str());
    system(("cd \"" + root_dir + "\" && npm install").c_str());
}

bool fileExist(string path) {
    ifstream fin(path.c_str());
    return fin.is_open();
}
string uploadFile(string path) {
    ifstream fin(path.c_str());
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* filePointerBeg = new char[len];
    fin.read(filePointerBeg, len);
    unsigned char* fileSha1 = sha1(filePointerBeg, len);
    stringstream buffer;
    for (int i = 0; i < 20; i++)
        buffer << hex << setw(2) << setfill('0') << int(fileSha1[i]);
    ofstream fout(("./data/" + buffer.str()).c_str());
    fout.write(filePointerBeg, len); fout.close();
    free(filePointerBeg); free(fileSha1);
    return buffer.str();
}

void initBuild(string path) {
    int res = system(("cd \"" + path + "\" && npm run build").c_str());
    if (res) exit(3);

    string package_json = readFile((path + "/package.json").c_str());
    Json::Value arr; json_decode(package_json, arr);
    SkinItem skin; BackgroundItem background; EffectItem effect; ParticleItem particle;
    auto tmp = skinsList("name = \"" + arr["skin"].asString() + "\"");
    if (tmp.items.size() == 0) writeLog(LOG_LEVEL_ERROR, "Failed to find skin \"" + arr["skin"].asString() + "\""), exit(0);
    skin = tmp.items[0];
    auto tmp2 = backgroundsList("name = \"" + arr["background"].asString() + "\"");
    if (tmp2.items.size() == 0) writeLog(LOG_LEVEL_ERROR, "Failed to find background \"" + arr["background"].asString() + "\""), exit(0);
    background = tmp2.items[0];
    auto tmp3 = effectsList("name = \"" + arr["effect"].asString() + "\"");
    if (tmp3.items.size() == 0) writeLog(LOG_LEVEL_ERROR, "Failed to find effect \"" + arr["effect"].asString() + "\""), exit(0);
    effect = tmp3.items[0];
    auto tmp4 = particlesList("name = \"" + arr["particle"].asString() + "\"");
    if (tmp4.items.size() == 0) writeLog(LOG_LEVEL_ERROR, "Failed to find particle \"" + arr["particle"].asString() + "\""), exit(0);
    particle = tmp4.items[0];

    string engineData = uploadFile((path + "/dist/EngineData").c_str());
    string engineConfiguration = uploadFile((path + "/dist/EngineConfiguration").c_str());
    string engineThumbnail = fileExist((path + "/dist/thumbnail.jpg").c_str()) ?
        uploadFile((path + "/dist/thumbnail.jpg").c_str()) : uploadFile((path + "/dist/thumbnail.png").c_str()); 

    engineCreate(EngineItem(-1, arr["name"].asString(), arr["title"].asString(), arr["subtitle"].asString(), arr["author"].asString(), 
        skin, background, effect, particle, SRL<EngineThumbnail>(engineThumbnail, ""), SRL<EngineData>(engineData, ""), 
        SRL<EngineConfiguration>(engineConfiguration, ""), SRL<EngineRom>("", "")), true);
}