#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>


static const uint8_t enc_flag[] = {
    0x00, 0x09, 0x0a, 0x1f, 0x00, 0x36, 0x25, 0x36, 0x18, 0x30, 0x10, 0x12, 
    0x15, 0x00, 0x11, 0x2d, 0x2a, 0x02, 0x16, 0x17, 0x25, 0x1e, 0x0a, 0x03, 
    0x0e
};
static const size_t enc_flag_len = sizeof(enc_flag);


static const uint8_t key[] = { 0x73, 0x65, 0x63, 0x72, 0x65, 0x74 }; // "secret"
static const size_t key_len = sizeof(key);

char* decrypt_flag_xor(const uint8_t* data, size_t data_len,
                       const uint8_t* k, size_t k_len) {
    char* out = (char*)malloc(data_len + 1);
    if (!out) return NULL;
    out[data_len] = '\0';

    for (size_t i = 0; i < data_len; i++) {
        out[i] = (char)(data[i] ^ k[i % k_len]);
    }
    return out;
}


char *gets(char *s); 
static void setup(void){
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin,  NULL, _IONBF, 0);

}

static void win(){
    char* flag = decrypt_flag_xor(enc_flag, enc_flag_len, key, key_len);
    if (flag) {
        printf("Congratulations! Here is your flag: %s\n", flag);
        free(flag);
    } else {
        printf("Error decrypting flag.\n");
    }
}

static void message(){
    puts("Welcome to the buffer overflow challenge!");
    puts("Try to overflow the buffer and call the win function.");
}


static void vuln(void){
    char msg[10];

    puts("Enter your message: ");
    gets(msg);
    printf("\nYou says: %s\n", msg);

    void *rbp = __builtin_frame_address(0);
    void *saved_rbp = *(void **)rbp;
    void *ret_addr  = *((void **)rbp + 1);

    printf("rbp value        = %p\n", rbp);
    printf("saved rbp (slot) = %p\n", saved_rbp);
    printf("ret addr (slot)  = %p\n", ret_addr);
    printf("&ret slot addr   = %p\n", (void *)((void **)rbp + 1));
}

int main(void){
    setup();
    message();

    printf("Address of win function: %p\n", win);

    vuln();           
    puts("Goodbye!\n");
    return 0;
}
