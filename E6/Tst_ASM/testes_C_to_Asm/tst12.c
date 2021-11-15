long global = 2;
long main(){
    long x;
    long y;
    long local = 2;
    x = local + local *  (local *  (local + local));
    y = global + global *  (global *  (global + global));
    // x = vsf + 5 * (2 * (2 + 1));

    return 0;

}