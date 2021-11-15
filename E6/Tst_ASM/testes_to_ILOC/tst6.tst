int global;
int main(){
    int local;
    int x;
    int y;
    global = 2;
    local = 2;
    x = local + local *  (local *  (local + local));
    y = global + global *  (global *  (global + global));
    // x = vsf + 5 * (2 * (2 + 1));

    return 0;

}