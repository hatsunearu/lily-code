#define VCP_SERIAL
#define DEBUG

#define VCP_PRINT(s) chnWrite(&SD2, (const uint8_t *)s, sizeof(s)/sizeof(s[0]));