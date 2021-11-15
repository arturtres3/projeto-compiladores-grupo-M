int global;

int main(){
    int local;
    int x;
    int y;
    int z;
    global = 2;
    local = 2;
    x = local + local *  local *  local + local;
    y = global + global *  (global *  (global + global));

    z = local + local + local + local;

    return 0;

}