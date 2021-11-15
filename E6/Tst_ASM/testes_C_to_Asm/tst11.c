int utilfunc(int a, int b, int c);
int myfunc(int a, int b, int c, int d,
            int e, int f, int g, int h)
{
    int xx = a * b * c * d * e * f * g * h;
    int yy = a + b + c + d + e + f + g + h;
    int zz = utilfunc(xx, yy, xx % yy);
    return zz + 20;
}

int main(int a, int b, int c, int d,
            int e, int f, int g, int h)
{
    int xx = a * b * c * d * e * f * g * h;
    int yy = a + b + c + d + e + f + g + h;
    int zz = utilfunc(xx, yy, xx % yy);
    return zz + 20;
}