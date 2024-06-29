auto SonolusCommunity = [](client_conn conn, http_request request, param argv) {
    if (request.method == "GET" || request.method == "get") SonolusCommunityInfo(conn, request, argv);
    else if (request.method == "POST" || request.method == "post") SonolusCommunitySubmit(conn, request, argv);
    else quickSendMsg(405);
};