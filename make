# Environment: Linux codespaces-b06e3b 6.2.0-1019-azure #19~22.04.1-Ubuntu SMP Wed Jan 10 22:57:03 UTC 2024 x86_64 x86_64 x86_64 GNU/Linux
# GNU G++: g++ (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0

# Colors
WHITE='\033[0m'
YELLOW='\033[0;33m'

# Compile
echo -e "${YELLOW}Compiling Sonolus Server using g++...${WHITE}"
g++ main.cpp -o sonolus\
    -ldl -lpthread -lcrypto -lssl -ljsoncpp -std=c++14 -lz -g -w \
    -DENABLE_SONOLUS -DENABLE_MYSQL -lmysqlclient -DENABLE_SQLITE -lsqlite3 -DENABLE_CURL -lcurl -DENABLE_ZIP -lzip
# echo -e "${YELLOW}Compiling libsonolush using g++...${WHITE}"
# g++ plugins/libsonolush/libsonolush.cpp -o plugins/libsonolush.so \
#     -g -std=c++17 -fPIC -shared -w\
#     -DENABLE_SONOLUS -DENABLE_MYSQL -DENABLE_SQLITE -DENABLE_CURL -DENABLE_ZIP
# echo -e "${YELLOW}Compiling libonedrive using g++...${WHITE}"
# g++ plugins/libonedrive/libonedrive.cpp -o plugins/libonedrive.so \
#     -ldl -lpthread -lcrypto -lssl -ljsoncpp -std=c++17 -fPIC -shared -g -w\
#     -DENABLE_SONOLUS -DENABLE_MYSQL -DENABLE_SQLITE -DENABLE_CURL -DENABLE_ZIP
echo -e "${YELLOW}Done.${WHITE}"
echo -e ""
echo -e "You can type \`./sonolus serve\` to start your server."
