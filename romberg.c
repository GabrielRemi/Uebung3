/***********************************************************************
 *
 * kompilieren mit
 *
 *   gcc -O2 -Wall -pedantic romberg.c -o romberg -lm
 *
 * ausführen mit
 *   ./romberg
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

  return( ( f(xe, p) + f(xa, p) ) * 0.5 * ( xe - xa ) );

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

  /* Beitraege der Zwischenwerte */
  for ( int i = 0; i <= nstep ; i++ ) {
    /* fprintf ( stdout, "# [trapez_integratio_restarted] h = %e x = %e f = %e\n", h, x, f(x,p) ); */
    tsum += f( x, p );
    x += h;
  }
  /* mal Schrittweite */
  tsum *= h;

  return( tsum );

} /* end of trapez_integration_restarted */

/***********************************************************************
 * Neville-Schema fuer Polynom-Interpolation,
 *
 * in-place, erseezte Eintraege in y selbst
 *
 * Eingabe:
 *   y = Stützordinaten
 *   x = Stützstellen
 *   m = Grad des Polynoms
 *
 ***********************************************************************/
double neville ( double * const y, double const * x , unsigned int const m )
{

  unsigned int k = 1;

  /* Setze das Rekursionsschema von Neville ein
   * für Polynomgrad m 
   * und Auswertung bei z = 0
   *
   * Vergleiche neville-aitken.c in Übung 2
   */

  while ( k <= m ) 
  {
    /* ??? */
    k++;
  }

  return ( y[0] );

} /* end of neville */


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

  /* Schleife über z-Werte, für die integriert werden soll
   */
  for ( int iz = 0; iz <= znum; iz++ )
  {

    /* aktueller z-Wert, linear zwischen zmin und zmax 
     */
    double z = zmin + ( zmax - zmin ) / (double)znum * iz ;

    /* void-Zeiger p zeigt auf die Stack-Adress von z */
    void * p = &z;
    
    /* Startwerte für die Berechnung des Integrals
     */
    double diffrel = 2 * int_epsrel;  /* > int_epsrel, damit die Iteration überhaupt startet */

    double h = int_xe - int_xa;  /* Startwert der Schrittweite, entspricht nstep = 1 */

    unsigned int nstep = 1;

    /* Speicherbereich für Stützordinaten und Stützstellen
     */
    double * y = (double*) malloc( nstep * sizeof( double ) );
    double * x = (double*) malloc( nstep * sizeof( double ) );

  
    /* Start der Integrations-Iteration mit elementarem Trapez-Schritt 
     */
    y[nstep-1] = trapez ( f, int_xa, int_xe, p );  /* nur Beiträge von xa, xe */
    x[nstep-1] = h;

    /* Startwert für das Integral;
     * das wird unser Integralwert und wir benutzen diese
     * Variable zur Kontrolle der relativen Abweichung
     */
    double val = y[nstep-1];

    /* Funktion trapez hat Integral mit nstep = 1 geliefert; 
     * erhöhe nstep für nächste Iteration
     */
    nstep++;

    /*
     * Stopp-Kriterum: epsrel = relative Abweichung von
     * einer Iteration zur nächsten
     *
     * OPTIONAL: Füge eine Kontrolle der maximalen Schrittzahl nstep ein,
     * um zu viele Iterationsschritte zu vermeiden
     *
     * ???
     *
     */
    while ( diffrel > int_epsrel )
    {

      /* Ausdehnung des Speicherbereichs für Stützordinaten und Stützstellen
       */
      y = (double*) realloc( y, nstep * sizeof( double ) );
      x = (double*) realloc( x, nstep * sizeof( double ) );

      /* berechne die neuen Intervallgrenzen
       */
      double const xa = int_xa + h / 2.;
      double const xe = int_xe - h / 2.; 

      /* Addition der Beiträge von den neuen Zwischenstellen;
       *
       * Skalierung mit 0.5 = 1 / 2, weil die neue Schrittweite h / 2 ist
       */
      y[nstep-1] = 0.5 * ( y[nstep-2] + trapez_integration_restarted ( f, xa, xe, h, p ) );

      /* neue Schrittweite */
      h *= 0.5;

      x[nstep-1] = h;

      /* Setze die Auswertung des Interpolationspolynoms mit Grad nstep-1 
       * bei h = 0 ein
       *
       * ???
       */
      double val_new = 0.;


      /* relative Abweichung zum alten Wert
       */
      diffrel = fabs( val - val_new ) / fabs( val + val_new ) * 2.;
    
      /* aktualisiere val 
       */
      val = val_new;

      nstep++;

    }  /* end of nstep, epsrel iteration */
    
    /* wir haben nstep am Ende der Schleife noch einmal erhöht;
     * der tatsächlich verwendete Wert ist nstep - 1
     */
    nstep--;

    /* Ausgabe des Integralwertes für Parameter z
     */
    fprintf ( stdout, "z %25.16e   f %25.16e   n %4u  epsrel %e\n", z, val, nstep, diffrel );

    /* Speicher für x und y freigeben
     */
    free ( x );
    free ( y );


  }  /* end of loop on z */

  return ( 0 );
}
