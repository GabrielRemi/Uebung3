/***********************************************************************
 *
 * kompilieren mit
 *
 *   gcc -O2 -Wall -pedantic trapez.c -o trapez -lm
 *
 * ausführen mit
 *   ./trapez
 *
 ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Typdefinition: der Typ ist eine Funktion
 * mit zwei Argumenten double const und Zeiger void *
 * 
 * Rückgabewert vom Typ double
 *
 * Dieser Funktionstyp wird nachher als Integrand verwendet
 */
typedef double (*func_type ) ( double const x, void * p );

/***********************************************************************
 * elementarer Trapez-Schritt
 *
 * Eingabe:
 *   f = Integrand
 *   xa, xe = Intervallgrenzen
 *   p = Zeiger auf Speicherbereich für Parameter
 ***********************************************************************/
double trapez ( func_type f, double const xa, double const xe, void * p ) 
{

  /* Setze die Trapezintegration ein für das Intervall [ xa, xe ] mit
   * Schrittweite h = xe - xa
   * ??? 
   */
  return( 0 );

} /* end of trapez */

/***********************************************************************
 * Trapez-Integration
 *
 * Es werden die Zwischenwerte hinzugefügt.
 *
 * Eingabe:
 *   f = Integrand
 *   xa, xe = Intervallgrenzen
 *   h = Schrittweite
 *   p = Zeiger auf Parameter für Integrand f
 * 
 * Ausgabe:
 *   Trapez-Regel Integral in double;
 ***********************************************************************/
double trapez_integration_restarted ( func_type f, double const xa, double const xe, double const h, void * p ) 
{

  /* Anzahl der Zwischenschritte */
  int const nstep = ( xe - xa ) / h;

  /* fprintf( stdout, "# [trapez_integration_restarted] nstep = %4d\n", nstep ); */

  /* Initialisierung */
  double tsum = 0.;
  double x    = xa;

  /* Setze die Aufsummation der Beitraege der Zwischenwerte
   * ein
   */
  for ( int i = 0; i <= nstep ; i++ ) 
  {
    /* tsum += ??? */
    x += h;
  }
  /* multiplizeren mit der Schrittweite */
  tsum *= h;

  return( tsum );

} /* end of trapez_integration_restarted */

/***********************************************************************
 * Integrand für das Elliptische Integral
 *
 * Eingabe:
 *   x = double precision 
 *   p = Adresse des double Parameters für das Elliptische Integral
 ***********************************************************************/
double func ( double const x, void * p  )
{
  double const z = ((double*)p)[0];

  double const y = z * sin ( x );

  return( 1. / sqrt ( 1 - y*y ) );

}  /* end of func */

/***********************************************************************
 * MAIN PROGRAM
 ***********************************************************************/
int main(int argc, char **argv) 
{

  /* Integrand
   */ 
  func_type f = func;

  double int_xa = 0.;          /* untere Grenze des Integrationsintervalls */
  double int_xe = M_PI / 2.;   /* obere Grenze des Integrationsintervalls; M_PI ist definiert in math.h, siehe man math.h */

  double int_epsrel = 1.e-06;  /* geforderete max. relative Aenderung */

  /* Parameter für den z-Bereich 
   */
  double const zmin = 0.;
  double const zmax = 0.98;
  int const znum = 100;

  for ( int iz = 0; iz <= znum; iz++ )
  {

    double z = zmin + ( zmax - zmin ) / (double)znum * iz ;

    /* void-Zeiger p zeigt auf die Stack-Adress von z */
    void * p = &z;
    
    /* Startwerte für die Berechnung des Integrals
     */
    double diffrel = 2 * int_epsrel;  /* > int_epsrel, damit die Iteration überhaupt startet */

    double h = int_xe - int_xa;  /* Startwert der Schrittweite, entspricht nstep = 1 */

    unsigned int nstep = 1;
  
    /* Start der Integrations-Iteration mit elementarem Trapez-Schritt 
     */
    double int_val = trapez ( f, int_xa, int_xe, p );  /* nur Beitraege von xa, xe */

    /* Funktion trapez liefert Integral mit nstep = 1; 
     * erhöhe nstep für nächste Iteration
     */
    nstep++;

    /*
     * Stopp-Kriterum: epsrel 
     *
     */
    while ( diffrel > int_epsrel )
    {

      /* berechne die neuen Intervallgrenzen
       */
      double const xa = int_xa + h / 2.;
      double const xe = int_xe - h / 2.; 

      /* Setze die Addition der Beiträge von den neuen Zwischenstellen ein;
       *
       * Skalierung mit 0.5 = 1 / 2, weil die neue Schrittweite h / 2 ist
       *
       * ???
       */
      double const val_new = 0.;

      /* neue Schrittweite */
      h *= 0.5;

      /* relative Abweichung num alten Wert */
      diffrel = fabs( int_val - val_new ) / fabs( int_val + val_new ) * 2.;
    
      /* aktualisiere int_val 
       */
      int_val = val_new;

      nstep++;

    }  /* end of nstep, eps iteration */
    
    nstep--;
    fprintf ( stdout, "z %25.16e   f %25.16e   n %4u  epsrel %e\n", z, int_val, nstep, diffrel );

  }  /* end of loop on z */

  return ( 0 );
}
