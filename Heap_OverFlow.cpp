#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
  // hello
#include <stdint.h>
using namespace std;

struct User;
static void hello(User*);


static const uint8_t enc_flag[] = {
    // FLAG 在這
};
static const size_t enc_flag_len = sizeof(enc_flag);

static const uint8_t key[] = { }; // secret 在這
static const size_t key_len = sizeof(key);

std::string decrypt_flag_xor(const uint8_t* data, size_t data_len,
                             const uint8_t* k, size_t k_len) {
    std::string out;
    out.resize(data_len);

    for (size_t i = 0; i < data_len; i++) {
        out[i] = static_cast<char>(data[i] ^ k[i % k_len]);
    }
    return out;
}

struct User {
    char name[16];     // overflow target (can replace 10 bytes to make difaficult)
    void(*cb)(User*);       // overwrite this
    char pad[8];
    void edit();
    void action();
    void show();
    User() : cb(nullptr) {
        std::memset(name, 0, sizeof(name));
    }
};
static User* g = nullptr;
void create(){
    if(g){
        cout<<"g is already created!"<<endl;
        return; 
    }

    g = new User();
    g->cb = hello;

    cout<<"Enter your name:";
    cin.getline(g->name,64);

    cout<<"User "<< g->name <<" created!"<<endl;
}
void User::edit(){
    if(!g){
        cout<<"g is not created yet!"<<endl;
        return; 
    }
    cout<<"Enter your new name:";
    cin.getline(g->name,64);
    cout<<"User renamed to "<< g->name <<"!"<<endl;

}
void User::action(){
    if(!g){
        cout<<"g is not created yet!"<<endl;
        return; 
    }
    g->cb(g);
}
void User::show(){
    if(!g){
        cout<<"g is not created yet!"<<endl;
        return; 
    }
    std::cout << "\n[debug] action @ " << reinterpret_cast<void*>(g->cb) << "\n";
}

static void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

void hello(User * u){
    cout<< "Hello," << u->name << "!" << endl;
}
void win(){
    std::string flag = decrypt_flag_xor(enc_flag, enc_flag_len, key, key_len);
    std::cout << "Congratulations! Here is your flag: " << flag << "\n";
}
 
static void menu() {
    std::cout << "1) create\n2) edit\n3) call\n4) show \n5) exit\n> ";
}

int main(){
    
    while(1){
        std::cout << "\n[debug] win @ " << reinterpret_cast<void*>(win) << "\n";
        menu();

        int choice;
        cin >> choice;
        cin.ignore();
        switch(choice){
            case 1:
                create();
                break;
            case 2:
                g->edit();
                break;
            case 3:
                g->action();
                break;
            case 4:
                g->show();
                break;
            case 5:
                cout<<"Goodbye!"<<endl;
                return 0;
            default:
                cout<<"Invalid choice!"<<endl;
                break;
        }
    }
}
