g++ main.cpp -o sonolus\
    -ldl -lpthread -lcrypto -lssl -ljsoncpp -std=c++14 -g \
    -DENABLE_SONOLUS -DENABLE_MYSQL -lmysqlclient -DENABLE_SQLITE -lsqlite3 -DENABLE_CURL -lcurl -DENABLE_ZIP -lzip
# g++ plugins/libsonolush/libsonolush.cpp -o plugins/libsonolush.so \
#     -g -std=c++17 -fPIC -shared\
#     -DENABLE_SONOLUS -DENABLE_MYSQL -DENABLE_SQLITE -DENABLE_CURL -DENABLE_ZIP
# g++ plugins/libonedrive/libonedrive.cpp -o plugins/libonedrive.so \
#     -ldl -lpthread -lcrypto -lssl -ljsoncpp -std=c++17 -fPIC -shared -g \
#     -DENABLE_SONOLUS -DENABLE_MYSQL -lmysqlclient -DENABLE_SQLITE -lsqlite3 -DENABLE_CURL -lcurl -DENABLE_ZIP -lzip