struct xyarray {
    int x,y;
};

int C(n,i)
int n,i;
{
    int j,a;
    a = 1;
    for (j=i+1; j <= n; j++)
	a *= j;
    for (j=1; j <= (n-i); j++)
	a /= j;
    return(a);
}

double BBlend(i,n,u)
int i,n;
double u;
{
    int j;
    double v;
    
    v = C(n,i);
    for (j=1; j <= i; j++)
	v *= u;
    for (j = 1; j <= (n-i); j++)
	v *= (1. - u);
    return(v);
}

Bezier(x,y,u,n,p)
double *x,*y,u;
int n;
struct xyarray *p;
{
    int i;
    double b;
    
    *x = 0; *y = 0;
    for (i=0; i<= n; i++) {
	b = BBlend(i,n,u);
	x += p[i].x * b;
	y += p[i].y * b;
    }
}

    
	