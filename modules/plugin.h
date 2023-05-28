using namespace std;

class SonolusServerPlugin {
    public:

    virtual string onPluginName() const = 0;
    virtual string onPluginDescription() const = 0;
    virtual string onPluginVersion() const = 0;
    virtual string onPluginPlatformVersion() const = 0;
    virtual string onPluginAuthor() const = 0;
    virtual string onPluginWebsite() const = 0;
    virtual string onPluginLicense() const = 0;
    virtual vector<string> onPluginHelp(char** argv) const = 0;
    virtual void onPluginRunner(int argc, char** argv) const = 0;
    virtual void onPluginRouter(int argc, char** argv, application *app) const = 0;
};

PLUMA_PROVIDER_HEADER(SonolusServerPlugin);
PLUMA_PROVIDER_SOURCE(SonolusServerPlugin, 6, 3);
