int a, b, c;

int f(){}

int g(int x){
    x = x +1;
    return x;
}

int main(){
    a = 5;
    a = g(a);
}