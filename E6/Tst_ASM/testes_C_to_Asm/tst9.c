int global = 2;
int main(){
    int x;
    int y;
    int local = 2;
    x = local + local *  (local *  (local + local));
    y = global + global *  (global *  (global + global));
    // x = vsf + 5 * (2 * (2 + 1));

    return 0;

}