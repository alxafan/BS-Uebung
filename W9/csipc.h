#define C_MSG "Client: "
#define S_MSG "Server: "
#define MSG_LENGTH_MAX 80

typedef struct {
int orator;
int msgLength;
char msg[MSG_LENGTH_MAX];
} segment_T;