addToLibrary({
    __builtin_emscripten_prepare: (config) => {
        let configJson = UTF8ToString(config)
        let appConfig = JSON.parse(configJson)
        if (ENVIRONMENT_IS_NODE) {
            if (appConfig["database"] == "sqlite") {
                let sqlite3 = require("../api/node_modules/better-sqlite3/lib");
                Module.db = new sqlite3(__dirname + "/../public/" + appConfig["sqlite.dbfile"])
            } else if (appConfig['database'] == "mysql") {
                let mysql = require("../api/node_modules/mysql")
                Module.connection = mysql.createConnection({
                    host: appConfig["mysql.hostname"],
                    user: appConfig["mysql.username"],
                    password: appConfig["mysql.password"],
                    database: appConfig["mysql.database"],
                    port: appConfig["mysql.port"]
                })
                Module.connection.connect();
            }
        } else {
            console.error("Database function is only supported in node environment")
        }
    },
    __builtin_emscripten_query: (sql, config) => {
        return Asyncify.handleAsync(async () => {
            let configJson = UTF8ToString(config)
            let appConfig = JSON.parse(configJson)
            sql = UTF8ToString(sql)
            sql = sql.replace(/\'/g, "\'\'")
            sql = sql.replace(/\"/g, "'")
            if (ENVIRONMENT_IS_NODE) {
                if (appConfig["database"] == "sqlite") {
                    let result = Module.db.prepare(sql).all()
                    return stringToNewUTF8(JSON.stringify(result))
                } else if (appConfig['database'] == "mysql") {
                    await new Promise((resolve, reject) => {
                        Module.connection.query("set sql_mode=\"NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION\";", (err) => {
                            if (err) {
                                reject(err)
                            }
                            resolve()
                        })
                    });
                    let result = await new Promise((resolve, reject) => {
                        Module.connection.query(sql, (err, result) => {
                            if (err) {
                                reject(err)
                            } else {
                                resolve(result)
                            }
                        })
                    })
                    return stringToNewUTF8(JSON.stringify(result))
                }
            } else {
                console.error("Database query is only supported in node environment")
                return stringToNewUTF8("[]")
            }
            return stringToNewUTF8("[]")
        })
    },
    __builtin_emscripten_execute: (sql, config) => {
        return Asyncify.handleAsync(async () => {
            let configJson = UTF8ToString(config)
            let appConfig = JSON.parse(configJson)
            sql = UTF8ToString(sql)
            sql = sql.replace(/\'/g, "\'\'")
            sql = sql.replace(/\"/g, "'")
            if (ENVIRONMENT_IS_NODE) {
                if (appConfig["database"] == "sqlite") {
                    Module.db.exec(sql)
                    return 1
                } else if (appConfig['database'] == "mysql") {
                    await new Promise((resolve, reject) => {
                        Module.connection.query("set sql_mode=\"NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION\";", (err) => {
                            if (err) {
                                reject(err)
                            }
                            resolve()
                        })
                    });
                    let result = await new Promise((resolve, reject) => {
                        Module.connection.query(sql, (err, result) => {
                            if (err) {
                                reject(err)
                            } else {
                                resolve(result)
                            }
                        })
                    })
                    return stringToNewUTF8(JSON.stringify(result))
                }
            } else {
                console.error("Database query is only supported in node environment")
                return 0
            }
            return 0
        })
    }
})