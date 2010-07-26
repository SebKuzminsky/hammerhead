// arduinoport.h

int arduino_connect(char *conn);
void arduino_disconnect(void);
int arduino_write(char cmd);
int get_arduino_fd(void);
int arduino_read_until(char* buf, char until);