addToLibrary({
    __builtin_emscripten_query: (sql) => {
        let configJson = FS.readFile("/config/config.json", { encoding: "utf8" }).toString()
        let appConfig = JSON.parse(configJson)
        sql = UTF8ToString(sql)
        sql = sql.replace(/\"/g, "'")
        if (ENVIRONMENT_IS_NODE) {
            if (appConfig["database"] == "sqlite") {
                let sqlite3 = require("../api/node_modules/better-sqlite3/lib");
                let db = new sqlite3(__dirname + "/../public/" + appConfig["sqlite.dbfile"], { readonly: true })
                let result = db.prepare(sql).all()
                db.close()
                return stringToNewUTF8(JSON.stringify(result))
            }
        } else {
            console.error("Database query is only supported in node environment")
            return stringToNewUTF8("[]")
        }
        return stringToNewUTF8("[]")
    },
    __builtin_emscripten_execute: (sql) => {
        return 0
        throw new Error("Cannot execute sql in vercel environment")
        let configJson = FS.readFile("/config/config.json", { encoding: "utf8" }).toString()
        let appConfig = JSON.parse(configJson)
        sql = UTF8ToString(sql)
        sql = sql.replace(/\"/g, "'")
        if (ENVIRONMENT_IS_NODE) {
            if (appConfig["database"] == "sqlite") {
                let sqlite3 = require("../api/node_modules/better-sqlite3/lib");
                let db = new sqlite3(__dirname + "/../public/" + appConfig["sqlite.dbfile"])
                db.exec(sql)
                db.close()
                return 1
            }
        } else {
            console.error("Database query is only supported in node environment")
            return 0
        }
        return 0
    }
})