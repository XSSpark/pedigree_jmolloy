#include <stdio.h>

#include <iostream>

using namespace std;

class Anus
{
public:
    Anus(int a) : anus(6) {anus = a;}
    ~Anus() {}
    void print() {printf("%d", anus);}
private:
    int anus;
};

Anus myanus(42);

int main(int argc, char **argv)
{
    printf("My anus says: ");
    myanus.print();
    printf("\n");

    cout << "Whee!" << endl;
    return 0;
}
