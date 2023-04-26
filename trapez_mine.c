#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef double (*func_type)(double, double*);


// lineare funktion. p[0] ist steigung, p[1] ist achsenabschnitt
// Zum test gedacht
double linear(double x, double *p)
{
    return p[0] * x + p[1];
}

//Betrag einer Zahl
double absd(double x)
{
    if (x < 0)
        return -1.0 * x;
    else
        return x;
}


// quadratische Funktion zum test
double quadratic(double x, double* p)
{
    return p[0]*x*x;
}


// Integrand für das elliptische Integral
double elliptic(const double x, double *p)
{
    const double z = p[0];
    return 1/(sqrt(1 - z*z*sin(x)*sin(x)));
}

double trapez(func_type f, double xa, double xe, double* p)
{
    double h = xe - xa;
    return h/2 * (f(xa, p) + f(xe, p));
}

// berechne integral
double integral(func_type f, double const a, double const b, double *p)
{
    
    
    // berechne fuer n = 1
    double h = b - a;
    double I = h/2 * (f(a, p) + f(b, p));  // Anfangswert der Integration 

    double eps_rel = 1e-6;
    double eps_current = 1;
    int n = 2;
    int n_max = 40;

    // Rekursion
    do
    {   
        double I_old = I;
        I /= 2;
        h /= 2;
        double term = 0; // zusätzliche terme die hinzu addiert werden
        for (int i = 1; i <= pow(2, n-1); i += 2)
        {
            term += f(a+i*h, p);
        } 

        I += h * term;
        eps_current = absd((I_old - I)/(I + I_old));
        n++;
    }
    while(eps_current > eps_rel);
    printf("Iterationen: %d\n", n - 2);

    return I;
}

int main(int args, char ** argv)
{
    double p[2] = {1.0, 0};
    printf("%.5f", integral(linear, 0, 2, p));
}