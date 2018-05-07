#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  
  normalize(light[LOCATION]);
  normalize(normal);
  normalize(view);
  
  color a = calculate_ambient(alight,areflect);
  color d = calculate_diffuse(light,dreflect,normal);
  color s = calculate_specular(light,sreflect,view,normal);

  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;
  limit_color(&i);

  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  limit_color(&a);

  
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  
  double dp = dot_product(light[LOCATION], normal);

  d.red = light[COLOR][RED] * dreflect[RED] * dp;
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * dp;
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * dp;
  limit_color(&d);
  
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;

  double *t = (double *)malloc(3 * sizeof(double));
  int i;
  for(i = 0; i < 3; i++)
    t[i] = dot_product(light[LOCATION], normal) * normal[i];

  double *r = (double *)malloc(3 * sizeof(double));
  for(i = 0; i < 3; i++)
    r[i] = 2*t[i] - light[LOCATION][i];

  double cosalpha = dot_product(r, view);
  
  if(cosalpha < 0){
    s.red = 0;
    s.green = 0;
    s.blue = 0;
  }
  else{
    s.red = light[COLOR][RED] * sreflect[RED] * pow(cosalpha,15);
    s.green = light[COLOR][GREEN] * sreflect[GREEN] * pow(cosalpha,15);
    s.blue = light[COLOR][BLUE] * sreflect[BLUE] * pow(cosalpha,15);
  }
  free(t);
  free(r);
  
  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red > 255)
    c->red = 255;
  if (c->green > 255)
    c->red = 255;
  if (c->blue > 255)
    c->red = 255;
  if (c->red < 0)
    c->red = 0;
  if (c->green < 0)
    c->red = 0;
  if (c->blue < 0)
    c->red = 0;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double tmp = sqrt(pow(vector[0], 2) + pow(vector[1],2) + pow(vector[2], 2));
  int i = 0;
  while (i < 3){
    vector[i] = vector[i] / tmp;
    i++;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
