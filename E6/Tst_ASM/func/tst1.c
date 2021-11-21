long f_result;
long a, b, c;

long f(long x, long y, long z, long w,
        long x1, long y1, long z1, long w1,
        long x2, long y2, long z2, long w2, long lmao){

    x = x + 1;
    y = y + 2;
    z = z + 3;
    w = w + 4;
    x1 = x1 + 5;
    y1 = y1 + 6;

    z1 = z1 + 7;
    w1 = w1 + 8;
    x2 = x2 + 9;
    y2 = y2 + 10;
    z2 = z2 + 11;
    w2 = w2 + 12;
    lmao = lmao + 13;
    //f_result = y1 + z1 + w1 + x2;

    return y1;
}

long g(long x){
    long e;
    e = 55;
    e++;
    if(e == 150){
        return 10;
    }
    if(e == 140){
        e++;
    }
    e = e + 8;
    e = e + 77;

    return e;
}


long main(){
    a = 1;

    b = f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);

    c = g(b);

    return a + b + c;
    //return a;



}