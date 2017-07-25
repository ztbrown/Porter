#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#define TRUE 1
#define FALSE 0

char *wwwroot;
char *mime_file;

int scan(char *input, char *output, int start, int max);
void init(int run_daemon, int *port, char **conf_file, char **log_file);
void handle(int connecting_socket);
int getRequestType(char *input);
int handle_http_get(char *input, int connecting_socket);
int send_string(char *message, int socket);
void send_header(char *status_code, char *content_Type, int total_size, int socket);

#endif // HTTP_UTILS_H
