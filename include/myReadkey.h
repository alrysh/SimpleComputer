#ifndef MYREADKEY_H
#define MYREADKEY_H

enum keys {
    KEY_ESC, KEY_ENTER,
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
    KEY_F5, KEY_F6,
    KEY_L, KEY_S, KEY_R, KEY_T, KEY_I,
    KEY_0, KEY_1, /* ... */ KEY_9,
    KEY_PLUS, KEY_MINUS,
    KEY_UNKNOWN
};

int rk_readkey(enum keys *key);
int rk_mytermsave(void);
int rk_mytermrestore(void);
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);
int rk_readvalue(int *value, int timeout);
int rk_readvalue_simple(int *value, int timeout);

#endif