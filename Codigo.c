
//Codigo generado por el Lenguaje ED
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
int main()
{
 
 
    int num,i,j;
    int suma;
    int factorial;
    int suma_fac;
    int tmp;
    double nousado;
    char texto[100]; 
 
    while(num<=0) 
    {
    strcpy(texto, "\t\n\t\t Digite un numero entero positivo: ");
    printf("%s", texto);
    scanf("%d", &num);
    printf("\n" );
    }
 
    for(i=1;num==1; i=1+=+1)
    {
    tmp = (num/2);
 
    if(num==tmp)
    {
    suma = suma+i;
    factorial = 1;
    for(j=1;j==i; j=1+=+1)
    {
    factorial = factorial*j;
    }
    suma_fac = (suma_fac+factorial);
 
    }
    if(suma==num)
    {
    printf("%d", num);
    printf("\n\t Es un numero es perfecto" );
    printf("\n\t\t La suma de sus divisores es:" );
    printf("%d", suma);
    printf("\n\t\t La suma de los factoriales de sus divisores es:" );
    printf("%d", suma_fac);
    }
    else
    {
    printf("%d", num);
    printf("\n\t\tEl numero no es un numero perfecto" );
    }
    }
 
 
    return 0;
}