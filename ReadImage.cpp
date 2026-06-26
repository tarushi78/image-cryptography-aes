#include <iostream>
#include <fstream>
using namespace std;
int main() {
    ifstream file("test.jpg", ios::binary);
    if (!file) {
    cout << "File nahi mili!" << endl;
    return 1;
    }
    cout << "File khul gayi!" << endl;
    int count=0;
    char byte;
    while(file.read(&byte,1)&&count<10)
    {
        cout<<(int)(unsigned char)byte<<" ";
        count++;
    }

    return 0;
}