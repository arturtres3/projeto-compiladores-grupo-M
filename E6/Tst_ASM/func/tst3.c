long q, w, e;

long f(long y, long z){
    y = 11;
    z = 22;
    return y;
}

    long a, b, x;
long main(){
    long i;

    i = 0;
    x = 0;
    a = 0;
    b = 1;

    for(i = 0 ; i < 11 ; i = i + 1){
        x = b + a;
        a = b;
        b = x;
    }

    return b;
}