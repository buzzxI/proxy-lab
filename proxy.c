#include <csapp.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_connection_hdr = "Proxy-Connection: close\r\n";

void serve_client(int client_fd);
void parse_url(char* url, char* host, char* port, char* uri);

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stdout, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int listen_fd = Open_listenfd(argv[1]);
    struct sockaddr_storage client_add;
    socklen_t add_len;
    char host[MAXBUF];
    char port[MAXBUF];

    while (1) {
        add_len = sizeof(client_add);
        int client_fd = Accept(listen_fd, (SA*)&client_add, &add_len);
        Getnameinfo((SA*)&client_add, add_len, host, MAXBUF, port, MAXBUF, 0);
        serve_client(client_fd);
    }
    Close(listen_fd);
    return 0;
}

void serve_client(int client_fd) {
    char client_buff[MAXBUF], server_buff[MAXBUF];
    rio_t read_client;
    Rio_readinitb(&read_client, client_fd);
    Rio_readlineb(&read_client, client_buff, MAXBUF);
    char method[MAXBUF], url[MAXBUF], version[MAXBUF];
    sscanf(client_buff, "%s %s %s", method, url, version);
    if (!strcmp(method, "GET")) {
        // parse url into: host, port, uri
        char host[MAXBUF], port[MAXBUF], uri[MAXBUF];
        parse_url(url, host, port, uri);
        
        // consturct HTTP request
        // request line
        sprintf(server_buff, "%s %s HTTP/1.0\r\n", method, uri);

        // request header
        sprintf(server_buff, "%sHost: %s\r\n", (char*)server_buff, host);
        sprintf(server_buff, "%s%s%s%s", (char*)server_buff, user_agent_hdr, connection_hdr, proxy_connection_hdr);
        // read headers from client
        Rio_readlineb(&read_client, client_buff, MAXBUF);
        while (strcmp(client_buff, "\r\n")) {
            char* has_connection_hdr = strstr(client_buff, "Connection:");
            char* has_proxy_connection_hdr = strstr(client_buff, "Proxy-Connection:");
            char* has_host_hdr = strstr(client_buff, "Host:");
            if (has_connection_hdr == NULL && has_proxy_connection_hdr && has_host_hdr) sprintf(server_buff, "%s%s", (char*)server_buff, (char*)client_buff);
            Rio_readlineb(&read_client, client_buff, MAXBUF);
        }

        // HTTP empty line
        sprintf(server_buff, "%s\r\n", (char*)server_buff);

        // write to server
        int server_fd = Open_clientfd(host, port);
        Rio_writen(server_fd, server_buff, strlen((char*)server_buff));

        // read HTTP response from server
        rio_t read_server;
        rio_readinitb(&read_server, server_fd);

        ssize_t n;
        while ((n = Rio_readnb(&read_server, client_buff, MAXBUF))) Rio_writen(client_fd, client_buff, n);
    }
}

void parse_url(char* url, char* host, char* port, char* uri) {
    char* has_protocol = strstr(url, "//");
    // url with `http(s)://` need to be trimed
    if (has_protocol != NULL) url = has_protocol + 2;
    char* has_port = strstr(url, ":");
    // url with port
    if (has_port != NULL) {
        *has_port = '\0';
        strcpy(host, url);
        url = has_port + 1;
        char* split = strstr(url, "/");
        if (split == NULL) {
            strcpy(port, url);
            sprintf(uri, "%s", "/");
        } else {
            strcpy(uri, split);
            *split = '\0';
            strcpy(port, url);
        }
    } else {
        sprintf(port, "%d", 80);
        char* split = strstr(url, "/");
        if (split == NULL) {
            strcpy(host, url);
            sprintf(uri, "%s", "/");
        }
        else {
            strcpy(uri, split);
            *split = '\0';
            strcpy(host, url);
        }
    }
}