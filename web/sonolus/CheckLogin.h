auto CheckLogin = [](client_conn conn, http_request request, param argv){
    Json::Value isLogin;
    isLogin["isLogin"] = checkLogin(request);
    quickSendObj(isLogin);
};