#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define V 1
#define F 0
#define FILEGLADE "inicio.glade"
#define FILEEDITOR "Editor.glade"
#define FILEAYUDA "Ayuda.glade"
#define FILEACERCADE "acercade.glade"
#define FILECODIGOC "CodigoC.glade"

typedef struct Inicio // Ventana de Inicio
{
    GtkWidget       *window;
    GtkWidget       *boton1;
    GtkWidget       *boton2;
    GtkWidget       *label1;
    GtkWidget       *text1;
}Win1;

typedef struct Editor // Segunda ventana
{
    GtkWidget * window;


    //Barra de MENU

    GtkWidget * archivoabrir;
    GtkWidget * archivoguardar;
    GtkWidget * archivoguardarcomo;
    GtkWidget * archivonuevo;
    GtkWidget * archivosalir;

    GtkWidget * editarcopiar;
    GtkWidget * editarpegar;
    GtkWidget * editarcortar;
    GtkWidget * editarbuscar;
    GtkWidget * editarreemplazar;
    GtkWidget * editareliminar;

    GtkWidget * ejecutarcompilar;
    GtkWidget * ejecutarejecutar;
    GtkWidget * ejecutarcompilaryejecutar;

    GtkWidget * ayudainformacion;
    GtkWidget * ayudaacercade;

    //Botones rapidos

    GtkWidget * nuevo;
    GtkWidget * abrir;
    GtkWidget * guardar;
    GtkWidget * guardarcomo;

    GtkWidget * copiar;
    GtkWidget * pegar;
    GtkWidget * buscar;
    GtkWidget * cortar;
    GtkWidget * limpiar;
    GtkWidget * reemplazar;

    GtkWidget * compilar;
    GtkWidget * ejecutar;

    GtkWidget * ayuda;
    GtkWidget * cerrar;

    //Caja grande de texto

    GtkWidget * scrolled_win;
    GtkWidget * textview;
    GtkTextBuffer * buffer;

    //Caja de texto del compilador
    GtkWidget * scrolled_winc;
    GtkWidget * textviewc;
    GtkTextBuffer * bufferc;

    //Status bar archivo

    GtkWidget * barraestado;
    guint barraestado_context_id;
    gchar * nombreArchivo;
    gchar * nomdialogo;

    //Status bar filas y columnas

    GtkWidget * barralineas;
    guint barralineas_context_id;
    gchar * nombrelineas;

}Win2;


typedef struct ErroresSintaxis
{
    char nombre[100];
    int linea;
    struct Error * siguiente;
}Error;

typedef struct VarDeclaradas
{
    char variable[50];
    char tipo[50];
    char tam[10];
    struct nodovar * siguiente;
}nodovar;

typedef struct ConstructorDeC
{
    char instruccion[100];
    struct nodoc * siguiente;
}nodoc;


nodovar * variables;
nodoc * sentencias;
Error * errores;

Win2       *obj1;

gboolean TextoInsertado;

int ini = 1;
int fin = 1;
int continicioprograma = 0;
int contfinprograma = 0;
char comilla[1];
int contpara = 0;
int continiciosi = 0;
int contfinsi = 0;
int contcasocontrario = 0;
int contmientras = 0;
int var = 0;


nodovar * BuscarVariableNodo(char nombre[100], nodovar * lista)
{
		nodovar * aux;
		aux = NULL;
		if(variables==NULL)
		{
			return aux;
		}
		else
		{
			aux = variables;
			while(aux!= NULL)
			{
				int x = strlen(nombre);
                int ret = strncmp(aux->variable, nombre, x);
                if(ret == 0)
                {
                    return aux;
                }
				aux = aux-> siguiente;
			}
		}
		return aux;
}

void EstadoVariables()
{
	if(variables == NULL)
	{
		printf("No hay variables declaradas");
	}
	else
	{
		nodovar * aux;
		aux = variables;
		printf("Las variables son:\n");
		printf("______________________________________________\n");
		while(aux!=NULL)
		{
			printf("\nNombre: .%s.", aux->variable);
			printf("\nTipo: %s", aux->tipo);
			printf("\nDimension: %s", aux->tam);
			printf("\n______________________________________________\n");
			aux = aux->siguiente;
		}
	}
}

void EstadoSentencias()
{
	if(sentencias == NULL)
	{
		printf("No hay sentencias escritas");
	}
	else
	{
		nodoc * aux;
		aux = sentencias;
		printf("Las sentencias son:\n\n");
		printf("______________________________________________\n");
		while(aux!=NULL)
		{
			printf("\n |%s", aux->instruccion);
			aux = aux->siguiente;
		}
		printf("\n______________________________________________\n");
	}
}

void Estado(Error * lista)
{
	if(lista == NULL)
	{
		printf("\nNO HAY ERRORES EN COMPILACION\n");
	}
	else
	{
		Error * aux;
		aux = lista;
		printf("Lista:\n");
		printf("______________________________________________\n");
		while(aux!=NULL)
		{
			printf("\nDescripcion: %s", aux->nombre);
			printf("\nLinea: %d", aux->linea);
			printf("\n______________________________________________\n");
			aux = aux->siguiente;
		}
	}
}

Error * IngresoCola(Error * lista, char nombre[100], int linea)
{
	Error * nuevo;
	nuevo = (Error*)malloc(sizeof(Error));
	strcpy(nuevo->nombre, nombre);
    nuevo->linea = linea;
	nuevo ->siguiente = NULL;
	if(lista == NULL)
	{
		lista = nuevo;
	}
	else
	{
		Error * aux;
		aux = lista;
		while(aux->siguiente != NULL)
		{
			aux = aux->siguiente;
		}
		aux -> siguiente = nuevo;
	}
	return lista;
}

nodovar * IngresoVariable(char nombre[50], char tipo[50], char dimension[10])
{
	nodovar * nuevo;
	nuevo = (nodovar*)malloc(sizeof(nodovar));
	strcpy(nuevo->variable, nombre);
	strcpy(nuevo->tipo, tipo);
	strcpy(nuevo->tam, dimension);
	nuevo ->siguiente = NULL;
	if(variables == NULL)
	{
		variables = nuevo;
	}
	else
	{
		nodovar * aux;
		aux = variables;
		while(aux->siguiente != NULL)
		{
			aux = aux->siguiente;
		}
		aux -> siguiente = nuevo;
	}
	return variables;
}

nodoc * IngresoSentencia(char nombre[50])
{
	nodoc * nuevo;
	nuevo = (nodoc*)malloc(sizeof(nodoc));
	strcpy(nuevo->instruccion, nombre);
	nuevo ->siguiente = NULL;
	if(sentencias == NULL)
	{
		sentencias = nuevo;
	}
	else
	{
		nodoc * aux;
		aux = sentencias;
		while(aux->siguiente != NULL)
		{
			aux = aux->siguiente;
		}
		aux -> siguiente = nuevo;
	}
	return sentencias;
}

Error * Desencolar(char buscar[100], Error * lista)
{
	if(lista == NULL)
	{
		printf("La lista esta vacia.");
	}
	else
	{
		Error * auxborrar;
		Error * anterior;
		anterior = NULL;
		auxborrar = lista;
		while(auxborrar != NULL)
		{
            int x = strlen(buscar);
			int ret = strncmp(auxborrar -> nombre, buscar, x);
			if(ret == 0)
			{
				break;
			}
			else
			{
				anterior = auxborrar;
				auxborrar = auxborrar -> siguiente;
			}
		}
		if(auxborrar == NULL)
		{
			printf("Nodo no encontrado");
		}
		else if(anterior == NULL)
		{
			lista = lista -> siguiente;
			free(auxborrar);
		}
		else
		{
			anterior -> siguiente = auxborrar-> siguiente;
			free(auxborrar);
		}
	}
	return lista;
}

int posicion(char patron[],char texto[],int k,int m){
	int i, cont = 0;

	for( i = 0; i < m; i++){
		if(texto[k] == patron[i]){
			cont = i+1;
		}
	}

return cont;
}

int buscar(char texto[], char patron[]){
	int n = strlen(texto) - 1, m = strlen(patron) - 1, k = m, j = m,aux = 0;
	while(k <= n)
	{

		if(texto[k - ( m - j )] == patron[j]){
			j--;
		}else{
			k = k + (m + 1 - posicion(patron,texto, k, m));
			j = m;
		}
		if( j == -1){
			aux = 1;
			//printf("La palabra se encuentra en el caracter %d y la posicion %d\n", k - m ,k-m+1);

			j = m;
			k = k + (m + 1 - posicion(patron,texto, k, m));

		}
	}
	if(aux == 0){
		//printf("\n NOEXISTE ESA PALABRA EN EL TEXTO\n\n");

	}
	return aux;

}

int Campo(char texto[], char patron[]){
	int n = strlen(texto) - 1, m = strlen(patron) - 1, k = m, j = m,aux = 0;
	while(k <= n)
	{

		if(texto[k - ( m - j )] == patron[j]){
			j--;
		}else{
			k = k + (m + 1 - posicion(patron,texto, k, m));
			j = m;
		}
		if( j == -1){
			aux = 1;
			//printf("La palabra se encuentra en el caracter %d y la posicion %d\n", k - m ,k-m+1);
			return k-m;

			j = m;
			k = k + (m + 1 - posicion(patron,texto, k, m));

		}
	}
	if(aux == 0){
		//printf("\n NOEXISTE ESA PALABRA EN EL TEXTO\n\n");

	}

}

int BuscarError(char nombre[100], Error * lista)
{

        Error * aux;
		aux = NULL;
		if(lista==NULL)
		{
			printf("La lista esta vacia. ");
		}
        else
		{

			aux = lista;
            while(aux!= NULL)
			{
                int x = strlen(nombre);
                int ret = strncmp(aux->nombre, nombre, x);
                if(ret == 0)
                {
                    return 1;
                }
                aux = aux-> siguiente;
			}
		}

		return 0;
}

int BuscarVariable(char nombre[50])
{

        nodovar * aux;
		aux = NULL;
		if(variables==NULL)
		{
			return 0;
		}
        else
		{

			aux = variables;
            while(aux!= NULL)
			{
                int x = strlen(nombre);
                int ret = strncmp(aux->variable, nombre, x);
                if(ret == 0)
                {
                    int i = strlen(nombre);
                    int j = strlen(aux->variable);
                    if(i == j)
                    {
                        return 1;
                    }

                }
                aux = aux-> siguiente;
			}
		}

		return 0;
}
int BuscarTipoVariable(char nombre[50])
{

        nodovar * aux;
		aux = NULL;
		if(variables==NULL)
		{
			return 0;
		}
        else
		{

			aux = variables;
            while(aux!= NULL)
			{
                int x = strlen(nombre);
                int ret = strncmp(aux->variable, nombre, x);
                if(ret == 0)
                {
                    x = strlen("ENTERO");
                    ret = strncmp(aux->tipo, "ENTERO", x);
                    if(ret == 0)
                    {
                        return 1;
                    }
                    x = strlen("DOBLE");
                    ret = strncmp(aux->tipo, "DOBLE", x);
                    if(ret == 0)
                    {
                        return 2;
                    }
                    x = strlen("CADENA");
                    ret = strncmp(aux->tipo, "CADENA", x);
                    if(ret == 0)
                    {
                        return 3;
                    }
                }
                aux = aux-> siguiente;
			}
		}

		return 0;
}

int ValVariables(char variable[50])
{
    int x = 0, ret = 0, i = 0, j = 0;

    x = strlen("PROGRAMA");
    ret = strncmp(variable, "PROGRAMA", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("PROGRAMA");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("programa");
    ret = strncmp(variable, "programa", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("programa");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("FINPROGRAMA");
    ret = strncmp(variable, "FINPROGRAMA", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("FINPROGRAMA");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("finprograma");
    ret = strncmp(variable, "finprograma", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("finprograma");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("VAR");
    ret = strncmp(variable, "VAR", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("VAR");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("var");
    ret = strncmp(variable, "var", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("var");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("ENTERO");
    ret = strncmp(variable, "ENTERO", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("ENTERO");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("entero");
    ret = strncmp(variable, "entero", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("entero");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("CADENA");
    ret = strncmp(variable, "CADENA", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("CADENA");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("cadena");
    ret = strncmp(variable, "cadena", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("cadena");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("DOBLE");
    ret = strncmp(variable, "DOBLE", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("DOBLE");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("doble");
    ret = strncmp(variable, "doble", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("doble");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("SI");
    ret = strncmp(variable, "SI", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("SI");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("FINSI");
    ret = strncmp(variable, "FINSI", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("FINSI");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("si");
    ret = strncmp(variable, "si", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("si");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("finsi");
    ret = strncmp(variable, "finsi", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("finsi");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("ENTONCES");
    ret = strncmp(variable, "ENTONCES", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("ENTONCES");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("entonces");
    ret = strncmp(variable, "entonces", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("entonces");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("CASOCONTRARIO");
    ret = strncmp(variable, "CASOCONTRARIO", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("CASOCONTRARIO");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("casocontrario");
    ret = strncmp(variable, "casocontrario", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("casocontrario");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("MIENTRAS");
    ret = strncmp(variable, "MIENTRAS", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("MIENTRAS");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("mientras");
    ret = strncmp(variable, "mientras", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("mientras");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("FINMIENTRAS");
    ret = strncmp(variable, "FINMIENTRAS", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("FINMIENTRAS");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("finmientras");
    ret = strncmp(variable, "finmientras", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("finmientras");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("PARA");
    ret = strncmp(variable, "PARA", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("PARA");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("para");
    ret = strncmp(variable, "para", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("para");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("FINPARA");
    ret = strncmp(variable, "FINPARA", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("FINPARA");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen("finpara");
    ret = strncmp(variable, "finpara", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("finpara");
        if(i == j)
        {
            return 1;
        }
    }

    //Palabras reservadas de C
    x = strlen("int");
    ret = strncmp(variable, "int", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("int");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("double");
    ret = strncmp(variable, "double", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("double");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("float");
    ret = strncmp(variable, "float", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("float");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("char");
    ret = strncmp(variable, "char", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("char");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("if");
    ret = strncmp(variable, "if", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("if");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("for");
    ret = strncmp(variable, "for", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("for");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("while");
    ret = strncmp(variable, "while", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("while");
        if(i == j)
        {
            return 1;
        }
    }
    x = strlen(";");
    ret = strncmp(variable, ";", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen(";");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("{");
    ret = strncmp(variable, "{", x);
    if(ret == 0)
    {
            return 2;
    }
    x = strlen("}");
    ret = strncmp(variable, "}", x);
    if(ret == 0)
    {
            return 2;
    }

    x = strlen("scanf");
    ret = strncmp(variable, "scanf", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("scanf");
        if(i == j)
        {
            return 2;
        }
    }
    x = strlen("printf");
    ret = strncmp(variable, "printf", x);
    if(ret == 0)
    {
        i = strlen(variable);
        j = strlen("printf");
        if(i == j)
        {
            return 2;
        }
    }

    return 0;

}

Error * ValidacionSintaxis(char nombre[100], Error * errores, int linea)
{

    if(buscar(nombre, "ESCRIBIR")!=0)//________________________________________________________________________
    {
        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);

        int val = 0;
        int i;
        int p;
        int tam;
        p = Campo(nombre, "ESCRIBIR");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL ESCRIBIR LA VARIABLE", linea);
                return errores;
            }
        }
        tam  = strlen("ESCRIBIR");
        if(buscar(nombre, ";")==0)
        {

            errores = IngresoCola(errores, "ERROR, FALTA UN PUNTO Y COMA EN LA SENTENCIA ESCRIBIR", linea);
            return errores;
        }
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        if(buscar(nombre, ";")!=0)
        {
            p = Campo(nombre, ";");
            nombre[p] = ' ';

            int primera, segunda;

            if(buscar(nombre, ")") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS AL ESCRIBIR", linea);
                return errores;
            }

            if(buscar(nombre, "(") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS AL ESCRIBIR", linea);
                return errores;
            }

            primera = Campo(nombre, "(");
            segunda = Campo(nombre, ")");
            if(segunda<primera)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS AL ESCRIBIR", linea);
                return errores;
            }

            nombre[primera] = ' ';
            nombre[segunda] = ' ';

            int px;
            int k;
            int vale = 0;
            for(k = primera; k< segunda; k++)
            {
                if(nombre[k] != ' ')
                {
                    vale= 1;

                    break;
                }

            }
            if(vale == 0)
            {
                errores = IngresoCola(errores, "ERROR, NO HAY NADA QUE ESCRIBIR", linea);
                return errores;
            }

            primera = k;

            if(nombre[k] == comilla[0])
            {
                int i = k;
                nombre[k] = ' ';
                for(i = k+1; i< segunda; i++)
                {
                    if(nombre[i] == comilla[0])
                    {
                        nombre[i] = ' ';

                        tam = strlen(nombre);

                        for(k = 0; k< tam-1; k++)
                        {
                            if(nombre[k]!=' ')
                            {
                                errores = IngresoCola(errores, "ERROR AL ESCRIBIR UN CONJUNTO DE CARACTERES", linea);
                                return errores;
                            }
                        }
                        return errores;


                    }
                    nombre[i] = ' ';

                }
                errores = IngresoCola(errores, "ERROR CON LAS COMILLAS AL ESCRIBIR CONJUNTO DE CARACTERES", linea);
                return errores;
            }
            char variable[50];
            int l = 0;



            for(k = primera; k<= segunda; k++)
            {
                variable[l] = nombre[k];
                nombre[k] = ' ';
                l++;
                if(nomaux[k] == ' ' || nomaux[k] == ')')
                {
                    variable[l-1] = '\0';
                    if(BuscarVariable(variable) == 1)
                    {
                        break;
                    }
                    else
                    {
                        errores = IngresoCola(errores, "ERROR AL ESCRIBIR, ESA VARIABLE NO EXISTE", linea);
                        return errores;
                    }
                    break;
                }

            }


            tam = strlen(nombre);
            int j;
            for(j = 0; j<tam-1; j++)
            {
                if(nombre[j]!=' ')
                {
                    val = 1;
                }
            }

        }
        if(buscar(nombre, ";")==1)
        {

            errores = IngresoCola(errores, "ERROR, HAY UN PUNTO Y COMA DE MAS EN ESCRIBIR", linea);
            return errores;
        }
        if(val == 0)
        {

        }
        else
        {

            errores = IngresoCola(errores, "ERROR EN SENTENCIA ESCRIBIR, CARACTERES DE MAS", linea);
            return errores;
        }
        var--;
        return errores;
    }
    else if(buscar(nombre, ":=")!=0)//________________________________________________________________
    {
        int i, j, k, p, primero;
        char nomaux[100];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);
        if(buscar(nombre, ";")==0)
        {
            errores = IngresoCola(errores, "FALTA EL PUNTO Y COMA EN LA SENTENCIA DE ASIGNACION", linea);
            return errores;
        }
        p = Campo(nombre, ";");
        nombre[p] = ' ';
        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "ERROR CON EL PUNTO Y COMA EN LA SENTENCIA DE ASIGNACION", linea);
            return errores;
        }
        primero = Campo(nombre, ":=");
        for(k = 0; k< primero; k++)
        {
            if(nombre[k] !=' ')
            {
                break;
            }
        }
        if(nombre[k]==':')
        {
            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, NO SE ESPECIFICADO LA VARIABLE A ASIGNAR", linea);
            return errores;
        }
        char variable[30];
        variable[0] = '\0';
        int l = 0;
        for(k = k; k<=primero; k++)
        {
            variable[l] = nombre[k];
            nombre[k] = ' ';
            l++;
            if(nomaux[k]==' ' || nomaux[k]==':' )
            {
                variable[l-1] = '\0';
                if(BuscarVariable(variable) == 1)
                {
                    break;
                }
                else
                {
                    errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, ESA VARIABLE NO EXISTE", linea);
                    return errores;
                }
            }
        }
        for(k = k; k<primero; k++)
        {
            if(nombre[k]!=' ')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, CARACTERES DE MAS", linea);
                return errores;
            }

        }
        nombre[primero] = ' ';
        nombre[primero+1] = ' ';
        primero = primero +2;
        for(k = primero; k<strlen(nomaux); k++)
        {
            if(nomaux[k]!=' ')
            {
                break;
            }
        }

        if(BuscarTipoVariable(variable) == 3)
        {
            if(nomaux[k]==comilla[0])
            {
                nombre[k] = ' ';
                if(buscar(nombre, comilla)==0)
                {
                    errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, FALTAN LAS SEGUNDAS COMILLAS", linea);
                    return errores;
                }

                int segunda = Campo(nombre, comilla);
                nombre[segunda] = ' ';

                for(i = k; i<segunda; i++)
                {
                    nombre[i] = ' ';
                }

                for(i=segunda; i<strlen(nombre)-1; i++)
                {
                    if(nombre[i]!=' ')
                    {
                        errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, CARACTERES DE MAS", linea);
                        return errores;
                    }
                }
                return errores;
            }
            else
            {
                char varcad[50];
                varcad[0] = '\0';
                int ld = 0;
                for(k = k; k<=p; k++)
                {
                    varcad[ld] = nomaux[k];
                    nombre[k] = ' ';
                    ld++;
                    if(nomaux[k]==' ' || nomaux[k]==';')
                    {
                        varcad[ld-1] = '\0';
                        if(varcad[0] == '\0')
                        {
                            errores = IngresoCola(errores, "NO SE PUEDE REALIZAR LA OPERACION EN LA ASIGNACION, VARIABLE CADENA VACIA", linea);
                            return errores;
                        }

                        if(BuscarVariable(varcad) == 1)
                        {
                            if(BuscarTipoVariable(varcad) != 3)
                            {
                                    errores = IngresoCola(errores, "NO SE PUEDE ASIGNAR A UNA VARIABLE CADENA, OTRA QUE NO SEA CADENA EN LA ASIGNACION", linea);
                                    return errores;
                            }
                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, LA VARIABLE NO EXISTE", linea);
                            return errores;
                        }
                        break;
                    }
                }//FIn for de variable a asignar
                for(k = k; k<strlen(nombre)-1; k++)
                {
                    if(nombre[k]!=' ')
                    {
                        errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, CARACTERES DE MAS", linea);
                        return errores;
                    }
                }

            }//FIn tipo de asignacion :=
            return errores;


        }//Fin buscar variable de tipo cadena
        else
        {

            int parentesis = 0;
            if(nombre[k]==comilla[0])
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, SOLO SE PUEDEN ASIGNAR CARACTERES A VARAIBLES DE TIPO CADENA", linea);
                return errores;
            }
            if(nombre[k]=='+')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, EL OPERADOR + ESTA EN LA POSICION INCORRECTA", linea);
                return errores;
            }
            if(nombre[k]=='-')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, EL OPERADOR - ESTA EN LA POSICION INCORRECTA", linea);
                return errores;
            }
            if(nombre[k]=='*')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, EL OPERADOR * ESTA EN LA POSICION INCORRECTA", linea);
                return errores;
            }
            if(nombre[k]=='/')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, EL OPERADOR / ESTA EN LA POSICION INCORRECTA", linea);
                return errores;
            }
            if(nombre[k]=='(')
            {
                nombre[k]=' ';
                parentesis++;
                for(k = k+1; k<=p; k++)
                {
                    if(nombre[k]!=' ')
                    {
                        break;
                    }
                }

            }
            if(nombre[k]==')')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, ESTA NO PUEDE INICIAR CON UN PARENTESIS CERRADO", linea);
                return errores;
            }
            int primeralinea = 0;

            char var[50];
            var[0] = '0';
            int l = 0;
            for(k = k; k<=p; k++)
            {

                var[l] = nombre[k];
                l++;
                nombre[k] = ' ';
                if(nomaux[k]==' ' || nomaux[k]=='+' || nomaux[k]=='-' || nomaux[k]=='*' || nomaux[k]=='/' || nomaux[k]=='(' || nomaux[k]==')' || nomaux[k]==';')
                {
                    var[l-1] = '\0';

                    if(var[0] == '\0')
                    {
                        errores = IngresoCola(errores, "NO SE PUEDE REALIZAR LA OPERACION EN LA ASIGNACION, VARIABLE O NUMEROS VACIOS", linea);
                        return errores;
                    }

                    if(BuscarVariable(var) == 1)
                    {
                        if(BuscarTipoVariable(var) == 3)
                        {
                                errores = IngresoCola(errores, "NO SE PUEDE ASIGNAR A UNA VARIABLE NUMERICA, UNA VARIABLE CADENA EN LA ASIGNACION", linea);
                                return errores;
                        }
                    }
                    else
                    {
                        int mn = 0;
                        for(mn = 0; mn<strlen(var)-1; mn++)
                        {
                            if(var[mn] == '1')
                            {

                            }
                            else if(var[mn] == '2')
                            {

                            }
                            else if(var[mn] == '3')
                            {

                            }
                            else if(var[mn] == '4')
                            {

                            }
                            else if(var[mn] == '5')
                            {

                            }
                            else if(var[mn] == '6')
                            {

                            }
                            else if(var[mn] == '7')
                            {

                            }
                            else if(var[mn] == '8')
                            {

                            }
                            else if(var[mn] == '9')
                            {

                            }
                            else if(var[mn] == '0')
                            {

                            }
                            else if(var[mn] == '.')
                            {

                            }
                            else
                            {
                                    errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, LA VARIABLE NO EXISTE", linea);
                                    return errores;
                            }
                        }
                        //Fin validacion del numero


                    }

                    var[0] = '\0';
                    l = 0;
                    //FIn de la busqueda de la variable

                    if(nomaux[k]==' ')
                    {
                        for(k = k+1; k<p-1; k++)
                        {
                            nombre[k]==' ';
                            if(nomaux[k]!=' ')
                            {
                                break;
                            }
                        }
                    }

                    if(nomaux[k] == ';')
                    {
                        nombre[k] = ' ';
                        break;
                    }
                    if(nomaux[k] =='(')
                    {
                            errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, LA UBICACION DEL PARENTESIS ( ES INCORRECTA", linea);
                            return errores;
                    }



                    int val = 0;
                    int auxk = k;

                    if(nomaux[k] == ')')
                    {
                        nombre[k] = ' ';
                        while(val == 0)
                        {
                            if(parentesis<=0)
                            {
                                errores = IngresoCola(errores, "ERROR EN ASIGNACION, SE ESTA CERRANDO UN PARENTESIS SIN HABER PUESTO UNO PREVIAMENTE", linea);
                                return errores;
                            }
                            for(j = k+1; j<p; j++)
                            {
                                if(nomaux[j]!=' ')
                                {
                                    break;
                                }
                            }
                            if(nomaux[j] != '+' && nomaux[j] != '-' && nomaux[j] != '*' && nomaux[j] != '/' && nomaux[j]!=';'&& nomaux[j]!=')')
                            {


                                errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, NO HAY NINGUN OPERADOR POSTERIOR AL )", linea);
                                return errores;
                            }
                            k = j;
                            parentesis--;
                            if(nomaux[k]!=')')
                            {
                                break;
                            }
                            else
                            {
                                nombre[k] = ' ';
                            }
                        }

                    }

                    val = 0;
                    if(nomaux[k] =='+' || nomaux[k]=='-' || nomaux[k]=='*' || nomaux[k]=='/')
                    {
                        nombre[k] = ' ';
                        for(j = k+1; j<p;j++)
                        {
                            if(nomaux[j]!=' ')
                            {
                                break;
                            }
                        }
                        if(nomaux[j] =='+' || nomaux[j]=='-' || nomaux[j]=='*' || nomaux[j]=='/')
                        {
                            errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, OPERADORES JUNTOS", linea);
                            return errores;
                        }

                        if(nomaux[j] == ')')
                        {
                            errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, PREVIO AL CIERRE DE UN PARENTESIS HAY UN OPERADOR", linea);
                            return errores;
                        }

                        if(nomaux[j] == '(')
                        {
                            k = j;
                        }
                        else
                        {
                            k = j-1;
                            continue;
                        }
                    }



                    if(nomaux[k] == '(')
                    {

                        j = k;
                        while(val==0)
                        {
                            if(nomaux[k] == '(')
                            {
                                parentesis++;
                                nombre[k] = ' ';
                            }
                            for(j=k+1; j<p; j++)
                            {
                                if(nomaux[j]!=' ')
                                {
                                    break;
                                }
                            }
                            if(nomaux[j]=='+')
                            {
                                errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, EXISTE UN OPERADOR + DESPUES DEL PARENTESIS", linea);
                                return errores;
                            }
                            if(nomaux[j]=='-')
                            {
                                errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, EXISTE UN OPERADOR - DESPUES DEL PARENTESIS", linea);
                                return errores;
                            }
                            if(nomaux[j]=='*')
                            {
                                errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, EXISTE UN OPERADOR * DESPUES DEL PARENTESIS", linea);
                                return errores;
                            }
                            if(nomaux[j]=='/')
                            {
                                errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, EXISTE UN OPERADOR / DESPUES DEL PARENTESIS", linea);
                                return errores;
                            }
                            if(nomaux[j]==')')
                            {
                                errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, SE ESTA CERRANDO UN PARENTESIS SIN NINGUN CONTENIDO (  )", linea);
                                return errores;
                            }
                            if(nomaux[j] != '(')
                            {
                                break;
                            }

                            k = j;
                        }
                        for(k = k+1; k<p; k++)
                        {
                            if(nomaux[k]!=' ')
                            {
                                break;
                            }
                        }

                        k--;
                        continue;
                        //Fin del ciclo while
                    }



                }
                //FIn de la validacion de la variable o numero
            }
            //Fin de todo el bucle

            for(k = 0; k<strlen(nombre)-1; k++)
            {
                if(nombre[k]!=' ')
                {
                    errores = IngresoCola(errores, "ERROR EN LA SENTENCIA DE ASIGNACION, CARACTERES DE MAS", linea);
                    return errores;
                }
            }
            if(parentesis>0)
            {
                errores = IngresoCola(errores, "ERROR EN LA ASIGNACION, NO SE ESTAN CERRANDO TODOS LOS PARENTESIS", linea);
                return errores;
            }
        }


        var--;
        return errores;
    }
    else if(buscar(nombre, "VAR")!=0)//________________________________________________________________________
    {
        int val = 0;
        int i;
        int p;

        p = Campo(nombre, "VAR");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL INICIALIZAR LA DECLARACION DE VARIABLES, CARACTERES DE MAS", linea);
                return errores;
            }
        }


        if(buscar(nombre, ";")>0)
        {

            errores = IngresoCola(errores, "ERROR, EN ESTE TIPO DE SENTENCIAS (VAR) NO VA PUNTO Y COMA", linea);
            return errores;
        }

        for(i = p + strlen("VAR"); i<strlen(nombre)-1; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL INICIALIZAR LA DECLARACION DE VARIABLES, CARACTERES DE MAS", linea);
                return errores;
            }
        }

        var = 1;
        return errores;
    }
    else if(buscar(nombre, "ENTERO")!=0)//________________________________________________________________________
    {

        if(var<=0)
        {
            errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE ENTERA, NO SE HA INICIALIZADO LA DECLARACION DE VARIABLES (VAR;)", linea);
            return errores;
        }
        int p;
        int tam;
        int i;
        p = Campo(nombre, "ENTERO");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL DECLARAR LA VARIABLE ENTERA", linea);
                return errores;
            }
        }
        tam  = strlen("ENTERA");
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        if(buscar(nombre, ";")==0)
        {
            errores = IngresoCola(errores, "ERROR, FALTA UN PUNTO Y COMA ; PARA DECLARAR LA VARIABLE ENTERA", linea);
            return errores;
        }
        p = Campo(nombre, ";");
        nombre[p] = ' ';
        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "ERROR CON EL PUNTO Y COMA AL DECLARAR VARIABLE ENTERA", linea);
            return errores;
        }
        nombre[p] = ';';
        for(i = max; i<p; i++)
        {
            if(nombre[i]!=' ')
            {
                break;
            }
        }

        max = i;
        char variable[50];
        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);
        int k = 0;

        for(i= max; i<=p; i++)
        {

                variable[k] = nombre[i];
                k++;
            if(nombre[i]==' ' || nombre[i]==',' || nombre[i]==';')
            {
                variable[k-1] = '\0';
                int valvar = 0;

                int lp = strlen(variable);
                int ss;
                for(ss = i-lp; ss<i; ss++)
                {
                    nombre[ss] = ' ';
                }

                if(variable[0]!='\0')
                {
                    if(BuscarVariable(variable) == 1)
                    {

                        errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE ENTERA, ESA YA EXISTE", linea);
                        return errores;
                    }
                    else
                    {
                        int existencia = 0;
                        existencia = ValVariables(variable);
                        if(existencia == 1)
                        {
                            errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE ENTERA, EL NOMBRE DE LA VARIABLE ES UNA PALABRA RESERVADA DE ED", linea);
                            return errores;
                        }
                        if(existencia == 2)
                        {
                            errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE ENTERA, EL NOMBRE DE LA VARIABLE ES UNA PALABRA RESERVADA DE C", linea);
                            return errores;
                        }


                        int w = i;
                        char tamano[10];
                        tamano[0] = '\0';

                        int cierre = 0;
                        int y = i;


                        for(y = i; y<strlen(nombre); y++)
                        {

                            if(nombre[y]!=' ')
                            {
                                if(nombre[y] == ',')
                                {
                                    break;
                                }
                                else if (y == p)
                                {
                                    break;
                                }
                                else
                                {

                                    errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE ENTERA, CARACTERES DE MAS", linea);

                                    return errores;
                                }
                            }
                        }



                        w = y;
                        int tipoc = 0;

                        if(nombre[w]==',')
                        {
                            nombre[w] = ' ';

                        }



                        if(variable[0] == '0')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '1')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '2')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '3')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '4')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '5')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '6')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '7')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '8')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '9')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE ENTERA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }

                        int d;
                        for(d =0; d<strlen(variable); d++)
                        {
                            if(variable[d] == '+')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS +", linea);
                                return errores;
                            }
                            if(variable[d] == '-')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS -", linea);
                                return errores;
                            }
                            if(variable[d] == '=')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS =", linea);
                                return errores;
                            }
                            if(variable[d] == '*')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS *", linea);
                                return errores;
                            }
                            if(variable[d] == '/')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS /", linea);
                                return errores;
                            }
                            if(variable[d] == '%')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS %", linea);
                                return errores;
                            }
                            if(variable[d] == '$')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS $", linea);
                                return errores;
                            }
                            if(variable[d] == '!')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS !", linea);
                                return errores;
                            }
                            if(variable[d] == '@')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS @", linea);
                                return errores;
                            }
                            if(variable[d] == '^')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS ^", linea);
                                return errores;
                            }
                            if(variable[d] == '(')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS (", linea);
                                return errores;
                            }
                            if(variable[d] == ')')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS )", linea);
                                return errores;
                            }
                            if(variable[d] == '?')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS ?", linea);
                                return errores;
                            }
                            if(variable[d] == '<')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS <", linea);
                                return errores;
                            }
                            if(variable[d] == '>')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS >", linea);
                                return errores;
                            }
                            if(variable[d] == comilla[0])
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS ''", linea);
                                return errores;
                            }
                            if(variable[d] == '[')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS [", linea);
                                return errores;
                            }
                            if(variable[d] == ']')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS ]", linea);
                                return errores;
                            }
                            if(variable[d] == '{')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS {", linea);
                                return errores;
                            }
                            if(variable[d] == '}')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE ENTERA NO SON PERMITIDOS }", linea);
                                return errores;
                            }
                        }

                        variables = IngresoVariable(variable, "ENTERA", "0");

                    }
                    //Fin del else


                }
                //FIn de la declaracion de variables

                k=0;
            }

        }
        nombre[p] = ' ';
        int x = p;
        for(p = 0; p<strlen(nombre)-1; p++)
        {

            if(nombre[p]!=' ')
            {

                errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE ENTERA, CARACTERES DE MAS", linea);
                return errores;
            }
        }

        p = Campo(nomaux,";");

        for(p = p-1; p>0; p--)
        {

            if(nomaux[p]!=' ')
            {
                if(nomaux[p]==',')
                {
                errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE ENTERA, CARACTERES DE MAS 2", linea);
                return errores;
                }
                break;
            }
        }
        return errores;


    }
    else if(buscar(nombre, "CADENA")!=0)//________________________________________________________________________
    {
        if(var<=0)
        {
            errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE CADENA, NO SE HA INICIALIZADO LA DECLARACION DE VARIABLES (VAR;)", linea);
            return errores;
        }
        int p;
        int tam;
        int i;
        p = Campo(nombre, "CADENA");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL DECLARAR LA VARIABLE CADENA", linea);
                return errores;
            }
        }
        tam  = strlen("CADENA");
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        if(buscar(nombre, ";")==0)
        {
            errores = IngresoCola(errores, "ERROR, FALTA UN PUNTO Y COMA ; PARA DECLARAR LA VARIABLE CADENA", linea);
            return errores;
        }
        p = Campo(nombre, ";");
        nombre[p] = ' ';
        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "ERROR CON EL PUNTO Y COMA AL DECLARAR VARIABLE CADENA", linea);
            return errores;
        }
        nombre[p] = ';';
        for(i = max; i<p; i++)
        {
            if(nombre[i]!=' ')
            {
                break;
            }
        }

        max = i;
        char variable[50];
        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);
        int k = 0;

        for(i= max; i<=p; i++)
        {

                variable[k] = nombre[i];
                k++;
            if(nombre[i]==' ' || nombre[i]==',' || nombre[i]==';' || nombre[i]=='[')
            {
                variable[k-1] = '\0';
                int valvar = 0;

                int lp = strlen(variable);
                int ss;
                for(ss = i-lp; ss<i; ss++)
                {
                    nombre[ss] = ' ';
                }

                if(variable[0]!='\0')
                {
                    if(BuscarVariable(variable) == 1)
                    {

                        errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE CADENA, ESA YA EXISTE", linea);
                        return errores;
                    }
                    else
                    {
                        int existencia = 0;
                        existencia = ValVariables(variable);
                        if(existencia == 1)
                        {
                            errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE CADENA, EL NOMBRE DE LA VARIABLE ES UNA PALABRA RESERVADA DE ED", linea);
                            return errores;
                        }
                        if(existencia == 2)
                        {
                            errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE CADENA, EL NOMBRE DE LA VARIABLE ES UNA PALABRA RESERVADA DE C", linea);
                            return errores;
                        }


                        int w = i;
                        char tamano[10];
                        tamano[0] = '\0';

                        int cierre = 0;
                        int y = i;


                        for(y = i; y<strlen(nombre); y++)
                        {

                            if(nombre[y]!=' ')
                            {
                                if(nombre[y] == ',')
                                {
                                    break;
                                }
                                else if (y == p)
                                {
                                    break;
                                }
                                else if(nombre[y] == '[')
                                {
                                    break;
                                }
                                else
                                {

                                    errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE CADENA, CARACTERES DE MAS", linea);
                                    //
                                    return errores;
                                }
                                                             }
                        }



                        w = y;
                        int tipoc = 0;

                        if(nombre[w]==',')
                        {
                            nombre[w] = ' ';

                        }
                        else if(nombre[w] == '[')
                        {
                            nombre[w] = ' ';
                            tipoc = 1;
                            int ll = 0;
                            for(y=w+1; y<=p; y++)
                            {
                                tamano[ll] = nombre[y];
                                ll++;
                                if(nombre[y] == ']')
                                {
                                    tamano[ll-1] = '\0';
                                    cierre = 1;
                                    nombre[y] = ' ';
                                    break;
                                }
                                nombre[y] = ' ';
                            }


                        }
                        //fin validacion [


                        if(tipoc == 1)
                        {
                            if(cierre == 0)
                            {
                                errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE CADENA, NO SE HA CERRADO EL CORCHETE ]", linea);
                                return errores;
                            }
                            nombre[i] = ' ';

                            i = y;
                            for(y = i+1; y<strlen(nombre); y++)
                            {
                                if(nombre[y]!=' ')
                                {
                                    if(nombre[y] == ',' || y == p)
                                    {

                                        nombre[y] = ' ';
                                        break;
                                    }
                                    else
                                    {
                                        errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE CADENA DESPUES DE CERRAR EL CORCHETE", linea);
                                        return errores;
                                    }



                                }

                            }

                        }
                        else if(tipoc == 0)
                        {
                            tamano[0] = '/0';
                            strcpy(tamano, "100");
                        }

                        if(variable[0] == '0')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '1')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '2')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '3')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '4')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '5')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '6')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '7')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '8')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '9')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE CADENA, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }


                        int d;
                        for(d =0; d<strlen(variable); d++)
                        {
                            if(variable[d] == '+')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS +", linea);
                                return errores;
                            }
                            if(variable[d] == '-')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS -", linea);
                                return errores;
                            }
                            if(variable[d] == '=')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS =", linea);
                                return errores;
                            }
                            if(variable[d] == '*')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS *", linea);
                                return errores;
                            }
                            if(variable[d] == '/')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS /", linea);
                                return errores;
                            }
                            if(variable[d] == '%')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS %", linea);
                                return errores;
                            }
                            if(variable[d] == '$')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS $", linea);
                                return errores;
                            }
                            if(variable[d] == '!')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS !", linea);
                                return errores;
                            }
                            if(variable[d] == '@')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS @", linea);
                                return errores;
                            }
                            if(variable[d] == '^')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS ^", linea);
                                return errores;
                            }
                            if(variable[d] == '(')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS (", linea);
                                return errores;
                            }
                            if(variable[d] == ')')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS )", linea);
                                return errores;
                            }
                            if(variable[d] == '?')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS ?", linea);
                                return errores;
                            }
                            if(variable[d] == '<')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS <", linea);
                                return errores;
                            }
                            if(variable[d] == '>')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS >", linea);
                                return errores;
                            }
                            if(variable[d] == comilla[0])
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS ''", linea);
                                return errores;
                            }
                            if(variable[d] == '[')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS [", linea);
                                return errores;
                            }
                            if(variable[d] == ']')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS ]", linea);
                                return errores;
                            }
                            if(variable[d] == '{')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS {", linea);
                                return errores;
                            }
                            if(variable[d] == '}')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE CADENA NO SON PERMITIDOS }", linea);
                                return errores;
                            }
                        }

                        for(d =0; d<strlen(tamano); d++)
                        {
                            if(tamano[d]=='1')
                            {

                            }
                            else if(tamano[d]=='2')
                            {

                            }
                            else if(tamano[d]=='3')
                            {

                            }
                            else if(tamano[d]=='4')
                            {

                            }
                            else if(tamano[d]=='5')
                            {

                            }
                            else if(tamano[d]=='6')
                            {

                            }
                            else if(tamano[d]=='7')
                            {

                            }
                            else if(tamano[d]=='8')
                            {

                            }
                            else if(tamano[d]=='9')
                            {

                            }
                            else if(tamano[d]=='0')
                            {

                            }
                            else
                            {
                                errores = IngresoCola(errores, "ERROR EN LA DIMESION DE LA CADENA, SOLO DEBEN IR NUMEROS NO LETRAS NI ESPACIOS", linea);
                                return errores;
                            }



                        }
                        variables = IngresoVariable(variable, "CADENA", tamano);

                    }
                    //Fin del else
                }
                //FIn de la declaracion de variables

                k=0;
            }

        }
        nombre[p] = ' ';
        int x = p;
        for(p = 0; p<strlen(nombre)-1; p++)
        {

            if(nombre[p]!=' ')
            {

                errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE CADENA, CARACTERES DE MAS", linea);
                return errores;
            }
        }

        p = Campo(nomaux,";");

        for(p = p-1; p>0; p--)
        {

            if(nomaux[p]!=' ')
            {
                if(nomaux[p]==',')
                {
                errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE CADENA, CARACTERES DE MAS 2", linea);
                return errores;
                }
                break;
            }
        }
        return errores;

    }
    else if(buscar(nombre, "DOBLE")!=0)//________________________________________________________________________
    {
        if(var<=0)
        {
            errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE DOBLE, NO SE HA INICIALIZADO LA DECLARACION DE VARIABLES (VAR;)", linea);
            return errores;
        }
        int p;
        int tam;
        int i;
        p = Campo(nombre, "DOBLE");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL DECLARAR LA VARIABLE DOBLE", linea);
                return errores;
            }
        }
        tam  = strlen("DOBLE");
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        if(buscar(nombre, ";")==0)
        {
            errores = IngresoCola(errores, "ERROR, FALTA UN PUNTO Y COMA ; PARA DECLARAR LA VARIABLE DOBLE", linea);
            return errores;
        }
        p = Campo(nombre, ";");
        nombre[p] = ' ';
        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "ERROR CON EL PUNTO Y COMA AL DECLARAR VARIABLE DOBLE", linea);
            return errores;
        }
        nombre[p] = ';';
        for(i = max; i<p; i++)
        {
            if(nombre[i]!=' ')
            {
                break;
            }
        }

        max = i;
        char variable[50];
        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);
        int k = 0;

        for(i= max; i<=p; i++)
        {

                variable[k] = nombre[i];
                k++;
            if(nombre[i]==' ' || nombre[i]==',' || nombre[i]==';')
            {
                variable[k-1] = '\0';
                int valvar = 0;

                int lp = strlen(variable);
                int ss;
                for(ss = i-lp; ss<i; ss++)
                {
                    nombre[ss] = ' ';
                }

                if(variable[0]!='\0')
                {
                    if(BuscarVariable(variable) == 1)
                    {

                        errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE DOBLE, ESA YA EXISTE", linea);
                        return errores;
                    }
                    else
                    {
                        int existencia = 0;
                        existencia = ValVariables(variable);
                        if(existencia == 1)
                        {
                            errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE DOBLE, EL NOMBRE DE LA VARIABLE ES UNA PALABRA RESERVADA DE ED", linea);
                            return errores;
                        }
                        if(existencia == 2)
                        {
                            errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE DOBLE, EL NOMBRE DE LA VARIABLE ES UNA PALABRA RESERVADA DE C", linea);
                            return errores;
                        }


                        int w = i;
                        char tamano[10];
                        tamano[0] = '\0';

                        int cierre = 0;
                        int y = i;


                        for(y = i; y<strlen(nombre); y++)
                        {

                            if(nombre[y]!=' ')
                            {
                                if(nombre[y] == ',')
                                {
                                    break;
                                }
                                else if (y == p)
                                {
                                    break;
                                }
                                else
                                {

                                    errores = IngresoCola(errores, "ERROR AL CREAR VARIABLE DOBLE, CARACTERES DE MAS", linea);

                                    return errores;
                                }
                            }
                        }



                        w = y;
                        int tipoc = 0;

                        if(nombre[w]==',')
                        {
                            nombre[w] = ' ';

                        }



                        if(variable[0] == '0')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '1')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '2')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '3')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '4')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '5')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '6')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '7')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '8')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }
                        if(variable[0] == '9')
                        {
                                errores = IngresoCola(errores, "ERROR EN LA VARIABLE DOBLE, EL NOMBRE DE LA VARAIBLE NO PUEDE EMPEZAR CON NUMEROS", linea);
                                return errores;
                        }

                        int d;
                        for(d =0; d<strlen(variable); d++)
                        {
                            if(variable[d] == '+')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS +", linea);
                                return errores;
                            }
                            if(variable[d] == '-')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS -", linea);
                                return errores;
                            }
                            if(variable[d] == '=')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS =", linea);
                                return errores;
                            }
                            if(variable[d] == '*')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS *", linea);
                                return errores;
                            }
                            if(variable[d] == '/')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS /", linea);
                                return errores;
                            }
                            if(variable[d] == '%')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS %", linea);
                                return errores;
                            }
                            if(variable[d] == '$')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS $", linea);
                                return errores;
                            }
                            if(variable[d] == '!')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS !", linea);
                                return errores;
                            }
                            if(variable[d] == '@')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS @", linea);
                                return errores;
                            }
                            if(variable[d] == '^')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS ^", linea);
                                return errores;
                            }
                            if(variable[d] == '(')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS (", linea);
                                return errores;
                            }
                            if(variable[d] == ')')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS )", linea);
                                return errores;
                            }
                            if(variable[d] == '?')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS ?", linea);
                                return errores;
                            }
                            if(variable[d] == '<')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS <", linea);
                                return errores;
                            }
                            if(variable[d] == '>')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS >", linea);
                                return errores;
                            }
                            if(variable[d] == comilla[0])
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS ''", linea);
                                return errores;
                            }
                            if(variable[d] == '[')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS [", linea);
                                return errores;
                            }
                            if(variable[d] == ']')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS ]", linea);
                                return errores;
                            }
                            if(variable[d] == '{')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS {", linea);
                                return errores;
                            }
                            if(variable[d] == '}')
                            {
                                errores = IngresoCola(errores, "ERROR, ESOS CARACTERES PARA EL NOMBRE DE LA VARIABLE DOBLE NO SON PERMITIDOS }", linea);
                                return errores;
                            }
                        }

                        variables = IngresoVariable(variable, "DOBLE", "0");

                    }
                    //Fin del else


                }
                //FIn de la declaracion de variables

                k=0;
            }

        }
        nombre[p] = ' ';
        int x = p;
        for(p = 0; p<strlen(nombre)-1; p++)
        {

            if(nombre[p]!=' ')
            {

                errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE DOBLE, CARACTERES DE MAS", linea);
                return errores;
            }
        }

        p = Campo(nomaux,";");

        for(p = p-1; p>0; p--)
        {

            if(nomaux[p]!=' ')
            {
                if(nomaux[p]==',')
                {
                errores = IngresoCola(errores, "ERROR AL DECLARAR VARIABLE DOBLE, CARACTERES DE MAS ,", linea);
                return errores;
                }
                break;
            }
        }
        return errores;


    }
    else if(buscar(nombre, "LEER")!=0)//________________________________________________________________
    {
        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);

        int val = 0;
        int i;
        int p;
        int tam;
        p = Campo(nombre, "LEER");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL LEER LA VARIABLE", linea);
                return errores;
            }
        }
        tam  = strlen("LEER");
        if(buscar(nombre, ";")==0)
        {

            errores = IngresoCola(errores, "ERROR, FALTA UN PUNTO Y COMA AL LEER", linea);
            return errores;
        }
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        if(buscar(nombre, ";")!=0)
        {

            p = Campo(nombre, ";");
            nombre[p] = ' ';


            int primera, segunda;

            if(buscar(nombre, ")") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS AL LEER", linea);
                return errores;
            }

            if(buscar(nombre, "(") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS AL LEER", linea);
                return errores;
            }

            primera = Campo(nombre, "(");
            segunda = Campo(nombre, ")");
            if(segunda<primera)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS AL LEER", linea);
                return errores;
            }

            nombre[primera] = ' ';
            nombre[segunda] = ' ';

            int px;
            int k;
            int vale = 0;
            for(k = primera; k< segunda; k++)
            {
                if(nombre[k] != ' ')
                {
                    vale= 1;
                    break;
                }

            }
            if(vale == 0)
            {
                errores = IngresoCola(errores, "ERROR, NO HAY NADA QUE LEER", linea);
                return errores;
            }
            primera = k;
            char variable[50];
            int l = 0;

            for(k = primera; k<= segunda; k++)
            {
                variable[l] = nombre[k];
                nombre[k] = ' ';
                l++;
                if(nomaux[k] == ' ' || nomaux[k] == ')')
                {
                    variable[l-1] = '\0';
                    if(BuscarVariable(variable) == 1)
                    {
                        break;
                    }
                    else
                    {
                        errores = IngresoCola(errores, "ERROR AL LEER, ESA VARIABLE NO EXISTE", linea);
                        return errores;
                    }
                }
            }

            tam = strlen(nombre);
            int j;
            for(j = 0; j<tam-1; j++)
            {
                if(nombre[j]!=' ')
                {
                    val = 1;
                }
            }

        }
        if(buscar(nombre, ";")==1)
        {

            errores = IngresoCola(errores, "ERROR, HAY UN PUNTO Y COMA DE MAS EN LEER", linea);
            return errores;
        }
        if(val == 0)
        {

        }
        else
        {

            errores = IngresoCola(errores, "ERROR EN SENTENCIA LEER", linea);
            return errores;
        }
        var--;
        return errores;

    }
    else if(buscar(nombre, "FINSI")!=0)//________________________________________________________________
    {
        if(contfinsi<=0)
        {
            errores = IngresoCola(errores, "ERROR, SE ESTA FINALIZANDO UN SI INEXISTENTE", linea);
            return errores;
        }

            int m = 0;
            int i = 0;
            int t = 0;

            m = Campo(nombre, "FINSI");
            for(i = 0; i<m; i++)
            {
                if(nombre[i] != ' ')
                {
                    errores = IngresoCola(errores, "ERROR AL FINALIZAR EL SI", linea);
                    return errores;
                }
            }
            t = strlen("FINSI");

            for(i = m; i<(t+m);i++)
            {
                nombre[i] = ' ';
            }
            if(buscar(nombre, ";")!=0)
            {
                errores = IngresoCola(errores, "ERROR AL CERRAR EL SI, NO ES NECESARIO PONER PUNTO Y COMA", linea);
                return errores;
            }
            for(i = 0; i<strlen(nombre)-1;i++)
            {
                if(nombre[i]==' ')
                {

                }
                else if(nombre[i]!=' ')
                {
                    errores = IngresoCola(errores, "ERROR AL CERRAR EL SI, CARACTERES DE MAS", linea);
                    return errores;
                }
            }

        contfinsi--;
        var--;
        return errores;
    }
    else if(buscar(nombre, "SI")!=0)//________________________________________________________________
    {
        int m = 0;
        int i = 0;
        int t = 0;
        int val = 0;
        int d = strlen(nombre);

        char nomaux[50];
            strcpy(nomaux, nombre);
        int p;
        int tam;
        p = Campo(nombre, "SI");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL DECLARAR LA SENTENCIA SI", linea);
                return errores;
            }
        }
        tam  = strlen("SI");
        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "EN ESTE TIPO DE SENTENCIAS (SI), NO VA PUNTO Y COMA", linea);
            return errores;
        }
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }

            int primera, segunda;

            if(buscar(nombre, ")") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL SI", linea);
                return errores;
            }

            if(buscar(nombre, "(") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL SI", linea);
                return errores;
            }

            primera = Campo(nombre, "(");
            segunda = Campo(nombre, ")");
            if(segunda<primera)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL SI", linea);
                return errores;
            }

            nombre[primera] = ' ';
            nombre[segunda] = ' ';

            int px;
            int k;
            int vale = 0;
            for(k = primera; k< segunda; k++)
            {
                if(nombre[k] == '+' || nombre[k] == '-' || nombre[k] == '*' || nombre[k] == '/' || nombre[k] == ',' || nombre[k] == ')' || nombre[k] == '(' )
                {
                    errores = IngresoCola(errores, "ERROR EN EL LA INICIALIZACION DEL SI, CARACTERES INCORRECTOS", linea);
                    return errores;
                }
                if(nombre[k] != ' ')
                {
                    vale= 1;

                    break;
                }

            }
            if(vale == 0)
            {
                errores = IngresoCola(errores, "ERROR, NO HAY NADA DENTRO DE LA SENTENCIA SI", linea);
                return errores;
            }
            primera = k;

            char variable[50];
            int l = 0;

            char tipovar[50];
            tipovar[0] = '\0';
            int f;
            int valnumero = 0;
            for(f = k; f<segunda; f++)
            {
                if(nomaux[f] == '1')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '2')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '3')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '4')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '5')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '6')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '7')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '8')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '9')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '0')
                {
                    valnumero++;
                }
                else
                {
                    break;
                }
            }
            int xx;
            if(valnumero >0)
            {

                k = f;
                xx = k;
                        if(nombre[xx]==' ' || nombre[xx]=='>' ||nombre[xx]=='<' ||nombre[xx]=='=')
                        {

                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA SI, SOLO SE PUEDEN USAR LOS OPERADORES: >, <, =, <=, >= O <>", linea);

                            return errores;
                        }


                            k = xx;
                            if(nombre[xx] == ' ')
                            {
                                for(k = xx; k<segunda; k++)
                                {
                                    if(nombre[k]!=' ')
                                    {
                                        break;
                                    }
                                }
                            }

                            if(nombre[k]=='=')
                            {
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx] == ')' || nombre[xx] == '(')
                                    {
                                        errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN LA SENTENCIA SI", linea);
                                        return errores;
                                    }
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA SI =", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='>')
                            {
                                if(nombre[k+1]=='=')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA SI >", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='<')
                            {
                                if(nombre[k+1]=='=' || nombre[k+1]=='>')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DEL SI <", linea);
                                    return errores;
                                }
                                k = xx;
                            }

            }
            else
            {


                for(xx = k; xx<segunda; xx++)
                {
                    variable[l] = nomaux[xx];
                    l++;
                    if(nombre[xx] == '>' || nombre[xx] == '<' || nombre[xx] == '=' || nombre[xx] == '/' || nombre[xx] == ',' || nombre[xx] == '+' || nombre[xx] == '-' || nombre[xx] == '*' || nombre[xx] == ' ')
                    {
                        variable[l-1] = '\0';

                        if(BuscarVariable(variable) == 1)
                        {
                            if(BuscarTipoVariable(variable) == 3)
                            {
                                    errores = IngresoCola(errores, "ERROR EN SENTENCIA SI, NO SE PUEDEN HACER COMPARACIONES CON CADENAS", linea);
                                    return errores;
                            }
                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA SI, LA VARIABLE DE COMPARACION NO EXISTE", linea);
                            return errores;
                        }

                        if(nombre[xx]==' ' || nombre[xx]=='>' ||nombre[xx]=='<' ||nombre[xx]=='=')
                        {

                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA SI, SOLO SE PUEDEN USAR LOS OPERADORES: >, <, =, <=, >= O <>", linea);

                            return errores;
                        }


                            k = xx;
                            if(nombre[xx] == ' ')
                            {
                                for(k = xx; k<segunda; k++)
                                {
                                    if(nombre[k]!=' ')
                                    {
                                        break;
                                    }
                                }
                            }

                            if(nombre[k]=='=')
                            {
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx] == ')' || nombre[xx] == '(')
                                    {
                                        errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN LA SENTENCIA SI", linea);
                                        return errores;
                                    }
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA SI =", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='>')
                            {
                                if(nombre[k+1]=='=')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA SI >", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='<')
                            {
                                if(nombre[k+1]=='=' || nombre[k+1]=='>')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DEL SI <", linea);
                                    return errores;
                                }
                                k = xx;
                            }

                        break;
                    }
                    //FIn de validaciones

                }
                //Fin for para leer la variable
            }


            f = k;
            valnumero = 0;
            for(f = k; f<segunda; f++)
            {
                if(nomaux[f] == '1')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '2')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '3')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '4')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '5')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '6')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '7')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '8')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '9')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '0')
                {
                    valnumero++;
                }
                else
                {
                    break;
                }
            }

            if(valnumero >0)
            {

                k = f;

            }
            else
            {

                int xx;

                for(xx = k; xx<=segunda; xx++)
                {
                    variable[l] = nomaux[xx];
                    l++;
                    if(nombre[xx] == '>' || nombre[xx] == '<' || nombre[xx] == '=' || nombre[xx] == '/' || nombre[xx] == ',' || nombre[xx] == '+' || nombre[xx] == '-' || nombre[xx] == '*' || nombre[xx] == ' ' || nombre[xx] == ')' || nombre[xx] == '(')
                    {
                        variable[l-1] = '\0';

                        if(BuscarVariable(variable) == 1)
                        {
                            if(BuscarTipoVariable(variable) == 3)
                            {
                                    errores = IngresoCola(errores, "ERROR EN SENTENCIA SI, NO SE PUEDEN HACER COMPARACIONES CON CADENAS 2", linea);
                                    return errores;
                            }
                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA SI, LA VARIABLE DE COMPARACION NO EXISTE 2", linea);
                            return errores;
                        }
                        if(nomaux[xx] == ')')
                        {
                            k = xx;
                            k++;
                            break;
                        }
                        if(nomaux[xx]==' ')
                        {

                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA SI, SIMBOLOS EN POSICION INCORRECTA", linea);
                            return errores;
                        }

                        k=xx;
                        k++;
                        break;
                    }
                    //FIn de validaciones
                }
                //Fin for para leer la variable
            }


        for(f = k; f<segunda; f++)
        {
            if(nomaux[f]!=' ')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA SI, CARACTERES DE MAS", linea);
                return errores;
            }

        }
        for(f = primera; f<=segunda; f++)
        {
            nombre[f]=' ';
        }

        for(f = 0; f<d-1; f++)
        {
            if(nombre[f]!=' ')
            {

                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA SI, CARACTERES DE MAS", linea);
                return errores;
            }

        }

        continiciosi++;
        contfinsi++;
        var--;
        return errores;
    }
    else if(buscar(nombre, "ENTONCES")!=0)//________________________________________________________________
    {

        if(continiciosi<=0)
        {
            errores = IngresoCola(errores, "ERROR, SE ESTA INICIALIZANDO UNA SENTENCIA SI INEXISTENTE", linea);
            return errores;
        }

            int m = 0;
            int i = 0;
            int t = 0;

            m = Campo(nombre, "ENTONCES");
            for(i = 0; i<m; i++)
            {
                if(nombre[i] != ' ')
                {
                    errores = IngresoCola(errores, "ERROR EN LA SENTENCIA ENTONCES", linea);
                    return errores;
                }
            }
            t = strlen("ENTONCES");

            for(i = m; i<(t+m);i++)
            {
                nombre[i] = ' ';
            }
            if(buscar(nombre, ";")!=0)
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA ENTONCES, NO ES NECESARIO PONER PUNTO Y COMA", linea);
                return errores;
            }
            for(i = 0; i<strlen(nombre)-1;i++)
            {
                if(nombre[i]==' ')
                {

                }
                else if(nombre[i]!=' ')
                {
                    errores = IngresoCola(errores, "ERROR EN LA SENTENCIA ENTONCES, CARACTERES DE MAS", linea);
                    return errores;
                }
            }

        continiciosi--;
        var--;
        return errores;
    }
    else if(buscar(nombre, "CASOCONTRARIO")!=0)//________________________________________________________________
    {
        if(contfinsi<=0)
        {
            errores = IngresoCola(errores, "ERROR, SE ESTA HACIENDO UN CASO CONTRARIO DE UNA SENTENCIA SI INEXISTENTE", linea);
            return errores;
        }

            int m = 0;
            int i = 0;
            int t = 0;

            m = Campo(nombre, "CASOCONTRARIO");
            for(i = 0; i<m; i++)
            {
                if(nombre[i] != ' ')
                {
                    errores = IngresoCola(errores, "ERROR EN LA SENTENCIA CASOCONTRARIO", linea);
                    return errores;
                }
            }
            t = strlen("CASOCONTRARIO");

            for(i = m; i<(t+m);i++)
            {
                nombre[i] = ' ';
            }
            if(buscar(nombre, ";")!=0)
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA CASOCONTRARIO, NO ES NECESARIO PONER PUNTO Y COMA", linea);
                return errores;
            }
            for(i = 0; i<strlen(nombre)-1;i++)
            {
                if(nombre[i]==' ')
                {

                }
                else if(nombre[i]!=' ')
                {
                    errores = IngresoCola(errores, "ERROR EN LA SENTENCIA CASOCONTRARIO, CARACTERES DE MAS", linea);
                    return errores;
                }
            }

        contcasocontrario++;
        var--;
        return errores;
    }

    else if(buscar(nombre, "FINMIENTRAS")!=0)//________________________________________________________________
    {

        if(contmientras<=0)
        {
            errores = IngresoCola(errores, "ERROR, SE ESTA FINALIZANDO UN BUCLE MIENTRAS INEXISTENTE", linea);
            return errores;
        }

         int m = 0;
        int i = 0;
        int t = 0;

        m = Campo(nombre, "FINMIENTRAS");
            for(i = 0; i<m; i++)
            {

                if(nombre[i] != ' ')
                {
                    errores = IngresoCola(errores, "ERROR AL FINALIZAR EL BUCLE MIENTRAS", linea);
                    return errores;
                }
            }
            t = strlen("FINMIENTRAS");



            for(i = m; i<(t+m);i++)
            {
                nombre[i] = ' ';
            }
            if(buscar(nombre, ";")!=0)
            {
                errores = IngresoCola(errores, "ERROR AL FINALIZAR EL BUCLE MIENTRAS, NO ES NECESARIO PONER PUNTO Y COMA", linea);
                return errores;
            }

            for(i = 0; i<strlen(nombre)-1;i++)
            {
                if(nombre[i]==' ')
                {

                }
                else if(nombre[i]!=' ')
                {
                    errores = IngresoCola(errores, "ERROR AL FINALIZAR EL CICLO MIENTRAS, CARACTERES DE MAS", linea);
                    return errores;
                }
            }



        contmientras--;
        var--;
        return errores;
    }
    else if(buscar(nombre, "MIENTRAS")!=0)//________________________________________________________________
    {

        int m = 0;
        int i = 0;
        int t = 0;
        int val = 0;
        int d = strlen(nombre);

        char nomaux[50];
            strcpy(nomaux, nombre);
        int p;
        int tam;
        p = Campo(nombre, "MIENTRAS");
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL DECLARAR LA SENTENCIA MIENTRAS", linea);
                return errores;
            }
        }
        tam  = strlen("MIENTRAS");
        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "EN ESTE TIPO DE SENTENCIAS (MIENTRAS), NO VA PUNTO Y COMA", linea);
            return errores;
        }
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }

            int primera, segunda;

            if(buscar(nombre, ")") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL MIENTRAS", linea);
                return errores;
            }

            if(buscar(nombre, "(") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL MIENTRAS", linea);
                return errores;
            }

            primera = Campo(nombre, "(");
            segunda = Campo(nombre, ")");
            if(segunda<primera)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL MIENTRAS", linea);
                return errores;
            }

            nombre[primera] = ' ';
            nombre[segunda] = ' ';

            int px;
            int k;
            int vale = 0;
            for(k = primera; k< segunda; k++)
            {
                if(nombre[k] == '+' || nombre[k] == '-' || nombre[k] == '*' || nombre[k] == '/' || nombre[k] == ',' || nombre[k] == ')' || nombre[k] == '(' )
                {
                    errores = IngresoCola(errores, "ERROR EN EL LA INICIALIZACION DEL MIENTRAS, CARACTERES INCORRECTOS", linea);
                    return errores;
                }
                if(nombre[k] != ' ')
                {
                    vale= 1;

                    break;
                }

            }
            if(vale == 0)
            {
                errores = IngresoCola(errores, "ERROR, NO HAY NADA DENTRO DE LA SENTENCIA MIENTRAS", linea);
                return errores;
            }
            primera = k;

            char variable[50];
            int l = 0;

            char tipovar[50];
            tipovar[0] = '\0';
            int f;
            int valnumero = 0;
            for(f = k; f<segunda; f++)
            {
                if(nomaux[f] == '1')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '2')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '3')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '4')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '5')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '6')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '7')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '8')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '9')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '0')
                {
                    valnumero++;
                }
                else
                {
                    break;
                }
            }
            int xx;
            if(valnumero >0)
            {

                k = f;
                xx = k;
                        if(nombre[xx]==' ' || nombre[xx]=='>' ||nombre[xx]=='<' ||nombre[xx]=='=')
                        {

                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA MIENTRAS, SOLO SE PUEDEN USAR LOS OPERADORES: >, <, =, <=, >= O <>", linea);

                            return errores;
                        }


                            k = xx;
                            if(nombre[xx] == ' ')
                            {
                                for(k = xx; k<segunda; k++)
                                {
                                    if(nombre[k]!=' ')
                                    {
                                        break;
                                    }
                                }
                            }

                            if(nombre[k]=='=')
                            {
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx] == ')' || nombre[xx] == '(')
                                    {
                                        errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN LA SENTENCIA MIENTRAS", linea);
                                        return errores;
                                    }
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA MIENTRAS =", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='>')
                            {
                                if(nombre[k+1]=='=')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA MIENTRAS >", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='<')
                            {
                                if(nombre[k+1]=='=' || nombre[k+1]=='>')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DEL MIENTRAS <", linea);
                                    return errores;
                                }
                                k = xx;
                            }

            }
            else
            {


                for(xx = k; xx<segunda; xx++)
                {
                    variable[l] = nomaux[xx];
                    l++;
                    if(nombre[xx] == '>' || nombre[xx] == '<' || nombre[xx] == '=' || nombre[xx] == '/' || nombre[xx] == ',' || nombre[xx] == '+' || nombre[xx] == '-' || nombre[xx] == '*' || nombre[xx] == ' ')
                    {
                        variable[l-1] = '\0';

                        if(BuscarVariable(variable) == 1)
                        {
                            if(BuscarTipoVariable(variable) == 3)
                            {
                                    errores = IngresoCola(errores, "ERROR EN SENTENCIA MIENTRAS, NO SE PUEDEN HACER COMPARACIONES CON CADENAS", linea);
                                    return errores;
                            }
                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA MIENTRAS, LA VARIABLE DE COMPARACION NO EXISTE", linea);
                            return errores;
                        }

                        if(nombre[xx]==' ' || nombre[xx]=='>' ||nombre[xx]=='<' ||nombre[xx]=='=')
                        {

                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA MIENTRAS, SOLO SE PUEDEN USAR LOS OPERADORES: >, <, =, <=, >= O <>", linea);

                            return errores;
                        }


                            k = xx;
                            if(nombre[xx] == ' ')
                            {
                                for(k = xx; k<segunda; k++)
                                {
                                    if(nombre[k]!=' ')
                                    {
                                        break;
                                    }
                                }
                            }

                            if(nombre[k]=='=')
                            {
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx] == ')' || nombre[xx] == '(')
                                    {
                                        errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN LA SENTENCIA MIENTRAS", linea);
                                        return errores;
                                    }
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA MIENTRAS =", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='>')
                            {
                                if(nombre[k+1]=='=')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DE LA SENTENCIA MIENTRAS >", linea);
                                    return errores;
                                }
                                k = xx;
                            }
                            else if(nombre[k]=='<')
                            {
                                if(nombre[k+1]=='=' || nombre[k+1]=='>')
                                {
                                    k++;
                                }
                                for(xx = k+1; xx<segunda; xx++ )
                                {
                                    if(nombre[xx]!=' ')
                                    {
                                        break;
                                    }
                                }

                                if(nombre[xx]=='>' || nombre[xx]=='<' || nombre[xx]=='=' || nombre[xx]=='+' || nombre[xx]=='-' || nombre[xx]=='*' || nombre[xx]=='/' || nombre[xx]==')' || nombre[xx]=='(')
                                {
                                    errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DEL MIENTRAS <", linea);
                                    return errores;
                                }
                                k = xx;
                            }

                        break;
                    }
                    //FIn de validaciones

                }
                //Fin for para leer la variable
            }


            f = k;
            valnumero = 0;
            for(f = k; f<segunda; f++)
            {
                if(nomaux[f] == '1')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '2')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '3')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '4')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '5')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '6')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '7')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '8')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '9')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '0')
                {
                    valnumero++;
                }
                else
                {
                    break;
                }
            }

            if(valnumero >0)
            {

                k = f;

            }
            else
            {

                int xx;

                for(xx = k; xx<=segunda; xx++)
                {
                    variable[l] = nomaux[xx];
                    l++;
                    if(nombre[xx] == '>' || nombre[xx] == '<' || nombre[xx] == '=' || nombre[xx] == '/' || nombre[xx] == ',' || nombre[xx] == '+' || nombre[xx] == '-' || nombre[xx] == '*' || nombre[xx] == ' ' || nombre[xx] == ')' || nombre[xx] == '(')
                    {
                        variable[l-1] = '\0';

                        if(BuscarVariable(variable) == 1)
                        {
                            if(BuscarTipoVariable(variable) == 3)
                            {
                                    errores = IngresoCola(errores, "ERROR EN SENTENCIA MIENTRAS, NO SE PUEDEN HACER COMPARACIONES CON CADENAS 2", linea);
                                    return errores;
                            }
                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA MIENTRAS, LA VARIABLE DE COMPARACION NO EXISTE 2", linea);
                            return errores;
                        }
                        if(nomaux[xx] == ')')
                        {
                            k = xx;
                            k++;
                            break;
                        }
                        if(nomaux[xx]==' ')
                        {

                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA MIENTRAS, SIMBOLOS EN POSICION INCORRECTA", linea);
                            return errores;
                        }

                        k=xx;
                        k++;
                        break;
                    }
                    //FIn de validaciones
                }
                //Fin for para leer la variable
            }


        for(f = k; f<segunda; f++)
        {
            if(nomaux[f]!=' ')
            {
                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA MIENTRAS, CARACTERES DE MAS", linea);
                return errores;
            }

        }
        for(f = primera; f<=segunda; f++)
        {
            nombre[f]=' ';
        }

        for(f = 0; f<d-1; f++)
        {
            if(nombre[f]!=' ')
            {

                errores = IngresoCola(errores, "ERROR EN LA SENTENCIA MIENTRAS, CARACTERES DE MAS", linea);
                return errores;
            }

        }


        contmientras++;
        var--;
        return errores;
    }

    else if(buscar(nombre, "FINPARA")!=0)//________________________________________________________________
    {
        int m = 0;
        int i = 0;
        int t = 0;

        m = Campo(nombre, "FINPARA");
            for(i = 0; i<m; i++)
            {

                if(nombre[i] != ' ')
                {
                    errores = IngresoCola(errores, "ERROR AL FINALIZAR EL BUCLE PARA", linea);
                    return errores;
                }
            }
            t = strlen("FINPARA");

            for(i = m; i<(t+m);i++)
            {
                nombre[i] = ' ';
            }
            if(buscar(nombre, ";")!=0)
            {
                errores = IngresoCola(errores, "ERROR AL FINALIZAR EL BUCLE PARA, NO ES NECESARIO PONER PUNTO Y COMA", linea);
                return errores;
            }

            for(i = 0; i<strlen(nombre)-1;i++)
            {
                if(nombre[i]==' ')
                {

                }
                else if(nombre[i]!=' ')
                {
                    errores = IngresoCola(errores, "ERROR AL FINALIZAR EL CICLO PARA, CARACTERES DE MAS", linea);
                    return errores;
                }
            }

        contpara--;
        return errores;
        var--;
    }
    else if(buscar(nombre, "PARA")!=0)//________________________________________________________________
    {
        int m = 0;
        int i = 0;
        int t = 0;
        if(buscar(nombre, "FINPARA")!=0)
        {
            m = Campo(nombre, "FINPARA");
            for(i = 0; i<m; i++)
            {
                if(nombre[i] != ' ')
                {
                    errores = IngresoCola(errores, "ERROR AL FINALIZAR EL BUCLE PARA", linea);
                    return errores;
                }
            }
            t = strlen(nombre);

            for(i = (m+strlen("FINPARA")); i<t;i++)
            {
                if(nombre[i]!=' ')
                {
                    errores = IngresoCola(errores, "ERROR AL FINALIZAR EL BUCLE PARA", linea);
                    return errores;
                }
            }

            contpara--;
            return errores;
        }


        int val = 0;

        int p;
        int tam;
        p = Campo(nombre, "PARA");
        char varfor[50];
        varfor[0] = '\0';
        for(i = 0; i<p; i++)
        {
            if(nombre[i] != ' ')
            {
                errores = IngresoCola(errores, "ERROR AL INICIALIZAR LA SENTENCIA PARA", linea);
                return errores;
            }
        }
        tam  = strlen("PARA");
        if(buscar(nombre, ";")!=0)
        {

            errores = IngresoCola(errores, "EN ESTE TIPO DE SENTENCIAS (PARA), NO VA PUNTO Y COMA", linea);
            return errores;
        }
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }

            int primera, segunda;

            if(buscar(nombre, ")") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL PARA", linea);
                return errores;
            }

            if(buscar(nombre, "(") == 0)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL PARA", linea);
                return errores;
            }

            primera = Campo(nombre, "(");
            segunda = Campo(nombre, ")");
            if(segunda<primera)
            {
                errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL PARA", linea);
                return errores;
            }

            nombre[primera] = ' ';
            nombre[segunda] = ' ';

            int px;
            int k;
            int vale = 0;
            for(k = primera; k< segunda; k++)
            {
                if(nombre[k] != ' ')
                {
                    vale= 1;
                    break;
                }

            }
            if(vale == 0)
            {
                errores = IngresoCola(errores, "ERROR, NO HAY NADA DENTRO DE LA SENTENCIA PARA", linea);
                return errores;
            }
            primera = k;
            char variable[50];
            int l = 0;
            char nomaux[50];
            strcpy(nomaux, nombre);
            char tipovar[50];
            tipovar[0] = '\0';
            for(k = primera; k< segunda; k++)
            {
                variable[l] = nombre[k];
                nombre[k] = ' ';
                l++;
                if(nomaux[k] == ' ' || nomaux==',' || nomaux[k] == '=' || nomaux[k] == '+' || nomaux[k] == '-' || nomaux[k] == '*' || nomaux[k] == '/' || nomaux[k] == ')' || nomaux[k] == '(')
                {
                        variable[l-1] = '\0';

                        if(BuscarVariable(variable) == 1)
                        {
                            if(BuscarTipoVariable(variable) == 3)
                            {
                                errores = IngresoCola(errores, "NO SE PUEDE INICIALIZAR EL PARA CON UNA VARIABLE DE TIPO CADENA", linea);
                                return errores;
                            }
                            strcpy(varfor, variable);
                            break;
                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL FOR, ESA VARIABLE NO EXISTE", linea);
                            return errores;
                        }
                }
            }
            int f = k;
            int valinicio = 0;
            for(f = k; f<segunda; f++)
            {
                if(nomaux[f] == '=')
                {
                    valinicio = 1;
                }
            }
            if(valinicio == 0)
            {
                errores = IngresoCola(errores, "NO HAY NADA QUE INICIALIZAR EN SENTENCIA PARA", linea);
                return errores;
            }
            for(f = k; f<segunda; f++)
            {
                if(nomaux[f] != ' ')
                {
                    break;
                }
            }
            if(nomaux[f]!='=')
            {
                errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL CICLO PARA", linea);
                return errores;
            }
            for(k = f+1; k<segunda; k++)
            {

                if(nomaux[k] != ' ')
                {
                    break;
                }
            }

            int valnumero = 0;
            for(f = k; f<segunda; f++)
            {
                if(nomaux[f] == '1')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '2')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '3')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '4')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '5')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '6')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '7')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '8')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '9')
                {
                    valnumero++;
                }
                else if(nomaux[f] == '0')
                {
                    valnumero++;
                }
                else
                {
                    break;
                }
            }
            if(valnumero >0)
            {
                //NUmeros
            }
            else if(valnumero ==0)
            {
                if(nomaux[f] == '+' || nomaux[f] == '-' || nomaux[f] == '*' || nomaux[f] == '/' || nomaux[f] == ',' )
                {
                    errores = IngresoCola(errores, "ERROR AL INICIAR CICLO PARA 2", linea);
                    return errores;
                }
                int xx = f;
                l = 0;
                variable[0] = '\0';
                for(xx = f; xx<segunda; xx++)
                {
                    variable[l] = nomaux[xx];
                    l++;
                    if(nomaux[xx] == '+' || nomaux[xx] == '-' || nomaux[xx] == '*' || nomaux[xx] == '/' || nomaux[xx] == ',' || nomaux[xx] == ' ')
                    {
                        variable[l-1] = '\0';

                        if(BuscarVariable(variable) == 1)
                        {
                            if(BuscarTipoVariable(variable) == 3)
                            {
                                errores = IngresoCola(errores, "NO SE PUEDE ASIGNAR A UNA VARIABLE NUMERICA UNA VARIABLE CADENA 2 EN LA INICIALIZACION DEL PARA", linea);
                                return errores;
                            }
                            break;
                        }
                        else
                        {
                            errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL PARA, ESA VARIABLE NO EXISTE 2", linea);
                            return errores;
                        }

                    }
                }
                f = xx;
            }

            for(k = f; k<segunda; k++)
            {
                if(nomaux[k]!=' ')
                {
                    break;
                }
            }


            f = k;
            if(nomaux[k]==',')
            {
                //FIn de la primera inicializacion
            }
            else
            {
                if(nomaux[k]!='+' && nomaux[k]!='-' && nomaux[k]!='*' && nomaux[k]!='/')
                {
                    errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL PARA, OPERADORES DESCONOCIDOS", linea);
                    return errores;
                }
                for(f=k; f<segunda; f++)
                {
                    if(nomaux[f]!=' ')
                    {
                        break;
                    }

                }
                valnumero = 0;
                for(f = f; f<segunda; f++)
                {
                    if(nomaux[f] == '1')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '2')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '3')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '4')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '5')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '6')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '7')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '8')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '9')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '0')
                    {
                        valnumero++;
                    }
                    else
                    {
                        break;
                    }
                }
                //FIn for de numeros

                if(valnumero >0)
                {
                    //NUmeros
                }
                else if(valnumero ==0)
                {
                    f = k;
                    if(nomaux[f] == '=' || nomaux[f] == ',' || nomaux[f] == ')' || nomaux[f] == '(' )
                    {
                        errores = IngresoCola(errores, "ERROR AL INICIAR CICLO PARA 3", linea);
                        return errores;
                    }
                    int xx = f;
                    l = 0;
                    variable[0] = '\0';

                    for(xx = f+1; xx<segunda; xx++)
                    {
                        if(nomaux[xx] == ')' || nomaux[xx] == '(')
                        {
                            errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL CICLO PARA", linea);
                            return errores;
                        }
                        if(nomaux[xx]!=' ')
                        {
                            break;
                        }

                    }
                    if(nomaux[xx]==',' || nomaux[xx] == '-'|| nomaux[xx] == '*'|| nomaux[xx] == '/'|| nomaux[xx] == '+' || nomaux[xx] == ')' || nomaux[xx] == '(')
                    {
                        errores = IngresoCola(errores, "ERROR AL INICIAR CICLO PARA, LOS OPERADORES ESTAN JUNTOS", linea);
                        return errores;
                    }

                    f=xx;
                    for(xx = f; xx<segunda; xx++)
                    {
                        variable[l] = nomaux[xx];
                        l++;
                        if(nomaux[xx] == ',' || nomaux[xx] == ' ' || nomaux[xx] == '*' || nomaux[xx] == '/' || nomaux[xx] == '+' || nomaux[xx] == '-')
                        {
                            variable[l-1] = '\0';

                            if(BuscarVariable(variable) == 1)
                            {
                                if(BuscarTipoVariable(variable) == 3)
                                {
                                    errores = IngresoCola(errores, "NO SE PUEDE ASIGNAR A UNA VARIABLE NUMERICA UNA VARIABLE CADENA 3", linea);
                                    return errores;
                                }
                                break;
                            }
                            else
                            {
                                int mn = 0;
                                for(mn = 0; mn<strlen(variable); mn++)
                                {
                                    if(variable[mn] == '1')
                                    {

                                    }
                                    else if(variable[mn] == '2')
                                    {

                                    }
                                    else if(variable[mn] == '3')
                                    {

                                    }
                                    else if(variable[mn] == '4')
                                    {

                                    }
                                    else if(variable[mn] == '5')
                                    {

                                    }
                                    else if(variable[mn] == '6')
                                    {

                                    }
                                    else if(variable[mn] == '7')
                                    {

                                    }
                                    else if(variable[mn] == '8')
                                    {

                                    }
                                    else if(variable[mn] == '9')
                                    {

                                    }
                                    else if(variable[mn] == '0')
                                    {

                                    }
                                    else
                                    {
                                        errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL FOR, ESA VARIABLE NO EXISTE 3", linea);
                                        return errores;
                                    }
                                }
                                break;

                            }

                        }
                    }
                    //Fin for de busqueda de variable 3
                    f = xx;
                    for(f = xx; f<segunda; f++)
                    {
                        if(nomaux[xx] == ')' || nomaux[xx] == '(')
                        {
                            errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL CICLO PARA", linea);
                            return errores;
                        }
                        if(nomaux[f]!=' ')
                        {
                            break;
                        }
                    }

                    if(nomaux[f]!= ',')
                    {
                        errores = IngresoCola(errores, "ERROR EN SINTAXIS SENTENCIA PARA, FALTA LA PRIMERA COMA", linea);
                        return errores;
                    }
                }
                //Fin de las validaciones de variable 3
            }
            //Fin else de operadores + - * / en la declaracion inicial

            //PARTE INTERMEDIA--------------------------------------------------------------


            for(k=f+1; k<segunda; k++)
            {

                if(nomaux[k] == ')' || nomaux[k] == '(')
                {
                    errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL CICLO PARA", linea);
                    return errores;
                }
                if(nomaux[k]!=' ')
                {
                    break;
                }
            }

            l = 0;
            variable[0] = '\0';
            int xx;
            for(xx = k; xx<segunda; xx++)
            {
                variable[l] = nomaux[xx];
                l++;
                if(nomaux[xx] == '>' || nomaux[xx] == '<' || nomaux[xx] == '=' || nomaux[xx] == '/' || nomaux[xx] == ',' || nomaux[xx] == '+' || nomaux[xx] == '-' || nomaux[xx] == '*' || nomaux[xx] == ' ')
                {
                    variable[l-1] = '\0';

                    if(BuscarVariable(variable) == 1)
                    {
                        if(BuscarTipoVariable(variable) == 3)
                        {
                                errores = IngresoCola(errores, "NO SE PUEDE ASIGNAR A UNA VARIABLE NUMERICA UNA VARIABLE CADENA PARA LA COMPARACION", linea);
                                return errores;
                        }
                    }
                    else
                    {

                        int mn = 0;
                        for(mn = 0; mn<strlen(variable); mn++)
                        {
                            if(variable[mn] == '1')
                            {

                            }
                            else if(variable[mn] == '2')
                            {

                            }
                            else if(variable[mn] == '3')
                            {

                            }
                            else if(variable[mn] == '4')
                            {

                            }
                            else if(variable[mn] == '5')
                            {

                            }
                            else if(variable[mn] == '6')
                            {

                            }
                            else if(variable[mn] == '7')
                            {

                            }
                            else if(variable[mn] == '8')
                            {

                            }
                            else if(variable[mn] == '9')
                            {

                            }
                            else if(variable[mn] == '0')
                            {

                            }
                            else
                            {
                                errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL PARA, ESA VARIABLE PARA LA COMPARACION NO EXISTE", linea);
                                return errores;
                            }
                        }


                    }

                    if(nomaux[xx]==' ' || nomaux[xx]=='>' ||nomaux[xx]=='<' ||nomaux[xx]=='=')
                    {

                    }
                    else
                    {
                        errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL FOR, PARA LA COMPARACION SOLO PUEDE USAR LOS OPERADORES: >, < o =", linea);

                        return errores;
                    }


                        k = xx;
                        if(nomaux[xx] == ' ')
                        {
                            for(k = xx; k<segunda; k++)
                            {
                                if(nomaux[k]!=' ')
                                {
                                    break;
                                }
                            }
                        }

                        if(nomaux[k]=='=')
                        {
                            for(xx = k+1; xx<segunda; xx++ )
                            {
                                if(nomaux[xx] == ')' || nomaux[xx] == '(')
                                {
                                    errores = IngresoCola(errores, "ERROR CON LOS PARENTESIS EN EL CICLO PARA", linea);
                                    return errores;
                                }
                                if(nomaux[xx]!=' ')
                                {
                                    break;
                                }
                            }

                            if(nomaux[xx]=='>' || nomaux[xx]=='<' || nomaux[xx]=='=' || nomaux[xx]=='+' || nomaux[xx]=='-' || nomaux[xx]=='*' || nomaux[xx]=='/' || nomaux[xx]==')' || nomaux[xx]=='(')
                            {
                                errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DEL PARA =", linea);
                                return errores;
                            }
                            k = xx;
                        }
                        else if(nomaux[k]=='>')
                        {
                            if(nomaux[k+1]=='=')
                            {
                                k++;
                            }
                            for(xx = k+1; xx<segunda; xx++ )
                            {
                                if(nomaux[xx]!=' ')
                                {
                                    break;
                                }
                            }

                            if(nomaux[xx]=='>' || nomaux[xx]=='<' || nomaux[xx]=='=' || nomaux[xx]=='+' || nomaux[xx]=='-' || nomaux[xx]=='*' || nomaux[xx]=='/' || nomaux[xx]==')' || nomaux[xx]=='(')
                            {
                                errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DEL PARA >", linea);
                                return errores;
                            }
                            k = xx;
                        }
                        else if(nomaux[k]=='<')
                        {
                            if(nomaux[k+1]=='=' || nomaux[k+1]=='>')
                            {
                                k++;
                            }
                            for(xx = k+1; xx<segunda; xx++ )
                            {
                                if(nomaux[xx]!=' ')
                                {
                                    break;
                                }
                            }

                            if(nomaux[xx]=='>' || nomaux[xx]=='<' || nomaux[xx]=='=' || nomaux[xx]=='+' || nomaux[xx]=='-' || nomaux[xx]=='*' || nomaux[xx]=='/' || nomaux[xx]==')' || nomaux[xx]=='(')
                            {
                                errores = IngresoCola(errores, "ERROR EN LAS COMPARACIONES DEL PARA <", linea);
                                return errores;
                            }
                            k = xx;
                        }

                    break;
                }
                //FIn de validaciones

            }
            //Fin for para leer la variable


            valnumero = 0;
            for(f = k; f<segunda; f++)
                {
                    if(nomaux[f] == '1')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '2')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '3')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '4')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '5')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '6')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '7')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '8')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '9')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '0')
                    {
                        valnumero++;
                    }
                    else
                    {
                        break;
                    }
                }
                //FIn for de numeros



                if(valnumero >0)
                {

                    if(nomaux[f]==',' || nomaux[f]==' ')
                    {

                    }
                    else
                    {
                        errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL PARA, CARACTER DESCONOCIDO", linea);
                        return errores;
                    }
                }
                else
                {
                    f = k;
                    if(nomaux[f] == '-' || nomaux[f] == ',' || nomaux[f] == ')' || nomaux[f] == '(' || nomaux[f] == '*' || nomaux[f] == '/' || nomaux[f] == '+' )
                    {
                        errores = IngresoCola(errores, "ERROR AL INICIAR CICLO PARA 4", linea);
                        return errores;
                    }
                    int xx = f;
                    l = 0;
                    variable[0] = '\0';
                    f=xx;
                    for(xx = f; xx<segunda; xx++)
                    {
                        variable[l] = nomaux[xx];
                        l++;
                        if(nomaux[xx] == ',' || nomaux[xx] == ' ' || nomaux[xx] == '*' || nomaux[xx] == '/' || nomaux[xx] == '+' || nomaux[xx] == '-')
                        {
                            variable[l-1] = '\0';

                            if(BuscarVariable(variable) == 1)
                            {
                                if(BuscarTipoVariable(variable) == 3)
                                {
                                    errores = IngresoCola(errores, "NO SE PUEDE ASIGNAR A UNA VARIABLE NUMERICA UNA VARIABLE CADENA PARA LA COMPARACION", linea);
                                    return errores;
                                }
                                f = xx;
                                break;
                            }
                            else
                            {
                                errores = IngresoCola(errores, "ERROR AL INICIALIZAR EL FOR, ESA VARIABLE NO EXISTE 4", linea);
                                return errores;
                            }

                        }


                }
            }

            for(k = f; k<segunda; k++)
            {

                if(nomaux[k]!=' ')
                {

                    break;
                }
            }
            f = k;
            f+= +1;
            int valvacio = 0;
            for(k = f; k<segunda; k++)
            {
                if(nomaux[k]==")")
                {
                    valvacio = 0;
                    break;

                }
                else if(nomaux[k]!=' ')
                {
                    valvacio = 1;
                    break;
                }
            }

            if(valvacio == 0)
            {
                errores = IngresoCola(errores, "NO SE HA ESPECIFICADO EL INCREMENTO EN LA DECLARACION DEL PARA", linea);
                return errores;
            }

            k = f;
            for(k = f; k<segunda; k++)
            {

                if(nomaux[k]!=' ')
                {
                    break;
                }
            }

            if(nomaux[k] == '+' || nomaux[k] == '-' )
            {

            }
            else
            {
                errores = IngresoCola(errores, "SIGNO INCORRECTO EN LA INICIALIZACION DEL CICLO PARA", linea);
                return errores;
            }
            k++;
            f = k;
            valnumero = 0;
            for(f = k; f<segunda; f++)
                {
                    if(nomaux[f] == '1')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '2')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '3')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '4')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '5')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '6')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '7')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '8')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '9')
                    {
                        valnumero++;
                    }
                    else if(nomaux[f] == '0')
                    {
                        valnumero++;
                    }
                    else
                    {
                        break;
                    }
                }
                if(valnumero == 0)
                {
                    errores = IngresoCola(errores, "ERROR AL ESPECIFICAR EL INCREMENTO DEL CICLO PARA", linea);
                    return errores;
                }
            k = f;
            f = k;

            for(k = primera; k<f; k++)
            {
                nombre[k] = ' ';
            }

            tam = strlen(nombre);
            int j;
            for(j = 0; j<tam-1; j++)
            {
                if(nombre[j]!=' ')
                {
                    val = 1;
                }
            }


        if(val == 0)
        {

        }
        else
        {

            errores = IngresoCola(errores, "ERROR EN LA SENTENCIA PARA", linea);
            return errores;
        }
        contpara++;
        var--;
        return errores;

    }
    else if(buscar(nombre, "FINPROGRAMA")!=0)//________________________________________________________________________
    {
        if(contfinprograma>0)
        {
            errores = IngresoCola(errores, "ERROR EN SENTENCIA FINPROGRAMA, ESTA SE ESCRIBE SOLO UNA VEZ AL FINAL DEL PROGRAMA", linea);
            return errores;
        }

        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "ERROR, EN ESTA SENTENCIA DE TIPO (FINPROGRAMA) NO VA PUNTO Y COMA", linea);
            return errores;
        }


        int p = Campo(nombre, "FINPROGRAMA");
        int k;

        for(k = 0; k<p; k++)
        {
            if(nombre[k]!=' ')
            {
                errores = IngresoCola(errores, "ERROR, CARACTES DE MAS EN LA SENTENCIA FINPROGRAMA", linea);
                return errores;
            }
        }
        for(k = p + strlen("FINPROGRAMA") +1; k<strlen(nombre); k++)
        {
            if(nombre[k]!=' ')
            {
                errores = IngresoCola(errores, "ERROR, CARACTES DE MAS EN LA SENTENCIA FINPROGRAMA ", linea);
                return errores;
            }
        }
        contfinprograma++;
        return errores;
    }
    else if(buscar(nombre, "PROGRAMA")!=0)//________________________________________________________________________
    {


        if(continicioprograma>0)
        {
            errores = IngresoCola(errores, "ERROR EN SENTENCIA PROGRAMA, ESTA SE USA UNICAMENTE PARA INDICAR EL INICIO DEL PROGRAMA", linea);
            return errores;
        }

        if(buscar(nombre, ";")!=0)
        {
            errores = IngresoCola(errores, "ERROR, EN ESTA SENTENCIA DE TIPO (PROGRAMA) NO VA PUNTO Y COMA", linea);
            return errores;
        }


        int p = Campo(nombre, "PROGRAMA");
        int k;

        for(k = 0; k<p; k++)
        {
            if(nombre[k]!=' ')
            {
                errores = IngresoCola(errores, "ERROR, CARACTES DE MAS EN LA SENTENCIA PROGRAMA", linea);
                return errores;
            }
        }
        for(k = p + strlen("PROGRAMA") +1; k<strlen(nombre); k++)
        {
            if(nombre[k]!=' ')
            {
                errores = IngresoCola(errores, "ERROR, CARACTES DE MAS EN LA SENTENCIA PROGRAMA ", linea);
                return errores;
            }
        }
        continicioprograma++;

        return errores;
    }
    else //Sentencia no identificada
    {
        int k = 0;
        int p = strlen(nombre);
        for(k=0; k<p-1; k++)
        {
            if(nombre[k]!=' ')
            {
                errores = IngresoCola(errores, "SENTENCIA NO IDENTIFICADA", linea);
                return errores;


            }
        }
        return errores;
    }

}

Error * AnalisisCodigo()
{
    FILE *fp;
    char * c, cad[100];
    char auxcad[100];
    auxcad[0] = '\0';
    fp=fopen("Codigo.ced", "r");
    int valfin = 0;
    int linea = 1;
    int valinicio = 0;
    contpara = 0;

    contmientras = 0;
    contcasocontrario = 0;
    continiciosi = 0;
    contfinsi = 0;
    continicioprograma = 0;
    contfinprograma = 0;
    errores = NULL;
    variables = NULL;

    int valterminar = 0;
    valfin = 0;
    int lincon = 0;
    while(c=fgets(cad,100,fp))
    {
        valterminar = 0;
        if(cad[1] != '\0')
        {
            if(buscar(cad, "PROGRAMA") != 0 &&valinicio == 0)
            {
                valinicio = 1;
            }
            else if(buscar(cad, "PROGRAMA") == 0 &&valinicio == 0)
            {
                errores = IngresoCola(errores, "NO SE HA INICIALIZADO CORRECTAMENTE EL CODIGO, FALTA SENTENCIA PROGRAMA", linea);
                valinicio = 1;
            }

			if(buscar(cad, "FINPROGRAMA") != 0)
            {
                valfin = 1;
                valterminar = 1;

            }
            errores = ValidacionSintaxis(cad, errores, linea);
			linea++;
			lincon++;

        }
        else{
            linea++;
        }


    }

    if(contpara>0)
    {
        errores = IngresoCola(errores, "EXISTEN SENTENCIAS PARA SIN CERRAR", linea);
    }
    if(contpara<0)
    {
        errores = IngresoCola(errores, "EXCESO, SE ESTAN CERRANDO SENTENCIAS PARA DE MAS", linea);
    }

    if(lincon == 0 &&valinicio == 0)
    {
        errores = IngresoCola(errores, "NO SE HA INICIALIZADO CORRECTAMENTE EL CODIGO, FALTA SENTENCIA PROGRAMA", linea);
    }
    if(valfin == 0)
    {
        errores = IngresoCola(errores, "NO SE HA CERRADO CORRECTAMENTE EL CODIGO, FALTA SENTENCIA FINPROGRAMA", linea);
    }
    if(valterminar == 0 && valfin>0)
    {
        errores = IngresoCola(errores, "NO SE HA CERRADO CORRECTAMENTE EL CODIGO, LA SENTENCIA FINPROGRAMA NO ES LA ULTIMA EN EL PROGRAMA", linea);
    }



    if(contmientras>0)
    {
        errores = IngresoCola(errores, "EXISTEN SENTENCIAS MIENTRAS SIN CERRAR", linea);
    }
    if(contmientras<0)
    {
        errores = IngresoCola(errores, "EXCESO, SE ESTAN CERRANDO SENTENCIAS MIENTRAS DE MAS", linea);
    }
    if(continiciosi>0)
    {
        errores = IngresoCola(errores, "EXSISTEN SENTENCIAS SI QUE NO SE HAN ABIERTO", linea);
    }
    if(continiciosi<0)
    {
        errores = IngresoCola(errores, "EXCESO, SE ESTAN ABRIENDO SENTENCIAS DE MAS EN EL SI", linea);
    }
    if(contfinsi>0)
    {
        errores = IngresoCola(errores, "EXSISTEN SENTENCIAS SI QUE NO SE HAN CERRADO", linea);
    }
    if(contfinsi<0)
    {
        errores = IngresoCola(errores, "EXCESO, SE ESTAN CERRANDO SENTENCIAS DE MAS EN EL SI", linea);
    }


    fclose(fp);


}

void ConstruirArchivoC(char nombre[100])
{

    if(buscar(nombre, "ESCRIBIR")!=0)//________________________________________________________________
    {
        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);

        int val = 0;
        int i;
        int p;
        int tam;
        int primera, segunda;


        primera = Campo(nombre, "(");
        segunda = Campo(nombre, ")");
        int px;
        int k;
        for(k = primera+1; k< segunda; k++)
        {
            if(nombre[k] != ' ')
            {
                break;
            }

        }
        primera = k;

        char variable[50];
        variable[0] = '\0';
        int l = 0;

        char tipo[20];
        tipo[0] = '\0';
        char instruccion[100];
        instruccion[0] = '\0';

        int varet;


        if(nombre[k] == comilla[0]) //CASO IMPRIMIR CADENA
        {
            int uno, dos;
            uno = Campo(nombre, comilla);
            nombre[uno] = ' ';
            dos = Campo(nombre, comilla);
            strcat(instruccion, "    printf(");
            for(i = k; i<dos; i++)
            {
                variable[l] = nomaux[i];
                l++;
            }
            variable[l] = '\0';
            strcat(instruccion, variable);
            strcat(instruccion, comilla);
            strcat(instruccion, " );");
            sentencias = IngresoSentencia(instruccion);
            return;
        }
        l = 0;
        for(k = primera; k<= segunda; k++)//CASO IMPRIMIR VARIABLE
        {
            variable[l] = nombre[k];
            nombre[k] = ' ';
            l++;
            if(nomaux[k] == ' ' || nomaux[k] ==')')
            {
                    variable[l-1] = '\0';
                    nodovar * var;
                    var = NULL;
                    var = BuscarVariableNodo(variable, variables);
                    tipo[0] = '\0';
                    strcpy(tipo, "ENTERA");
                    varet = strncmp(tipo, var->tipo, strlen(tipo));
                    if(varet == 0)
                    {
                        strcat(instruccion, "    printf(");
                        strcat(instruccion, comilla);
                        strcat(instruccion, "%d");
                        strcat(instruccion, comilla);
                        strcat(instruccion, ", ");
                        strcat(instruccion, variable);
                        strcat(instruccion, ");");
                        sentencias = IngresoSentencia(instruccion);
                        return;
                    }

                    tipo[0] = '\0';
                    strcpy(tipo, "CADENA");
                    varet = strncmp(tipo, var->tipo, strlen(tipo));
                    if(varet == 0)
                    {
                        strcat(instruccion, "    printf(");
                        strcat(instruccion, comilla);
                        strcat(instruccion, "%s");
                        strcat(instruccion, comilla);
                        strcat(instruccion, ", ");
                        strcat(instruccion, variable);
                        strcat(instruccion, ");");
                        sentencias = IngresoSentencia(instruccion);
                        return;
                    }

                    tipo[0] = '\0';
                    strcpy(tipo, "DOBLE");
                    varet = strncmp(tipo, var->tipo, strlen(tipo));
                    if(varet == 0)
                    {
                        strcat(instruccion, "    printf(");
                        strcat(instruccion, comilla);
                        strcat(instruccion, "%f");
                        strcat(instruccion, comilla);
                        strcat(instruccion, ", ");
                        strcat(instruccion, variable);
                        strcat(instruccion, ");");
                        sentencias = IngresoSentencia(instruccion);
                        return;
                    }
                    break;
            }

        }

        return;
    }
    else if(buscar(nombre, ":=")!=0)//________________________________________________________________
    {

        int i, j, k, p, primero;
        char nomaux[100];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);
        p = Campo(nombre, ";");
        nombre[p] = ' ';

        primero = Campo(nombre, ":=");
        for(k = 0; k< primero; k++)
        {
            if(nombre[k] !=' ')
            {
                break;
            }
        }
        char variable[30];
        variable[0] = '\0';
        int l = 0;
        for(k = k; k<=primero; k++)
        {
            variable[l] = nombre[k];
            nombre[k] = ' ';
            l++;
            if(nomaux[k]==' ' || nomaux[k]==':' )
            {
                variable[l-1] = '\0';
                if(BuscarVariable(variable) == 1)
                {
                    break;
                }
            }
        }

        primero = primero +2;
        for(k = primero; k<strlen(nomaux); k++)
        {
            if(nomaux[k]!=' ')
            {
                break;
            }
        }

        if(BuscarTipoVariable(variable) == 3) //Caso CADENA------------------------------------------------------------------
        {
            if(nomaux[k]==comilla[0])//Asignacion de una cadena
            {

                char instruccion[100], mensaje[90];
                instruccion[0] = '\0';
                mensaje[0] = '\0';
                int ld = 0;
                for(i = k+1; i<strlen(nomaux)-1; i++)
                {
                    mensaje[ld] = nomaux[i];
                    ld++;
                }
                mensaje[ld-1] = '\0';
                strcat(instruccion, "    strcpy(");
                strcat(instruccion, variable);
                strcat(instruccion, ", ");
                strcat(instruccion, comilla);
                strcat(instruccion, mensaje);
                strcat(instruccion, ");");
                sentencias = IngresoSentencia(instruccion);
                return;
            }
            else // Igualacion de cadenas
            {

                char varcad[50];
                varcad[0] = '\0';
                int ld = 0;
                for(k = k; k<=p; k++)
                {
                    varcad[ld] = nomaux[k];
                    nombre[k] = ' ';
                    ld++;
                    if(nomaux[k]==' ' || nomaux[k]==';')
                    {
                        varcad[ld-1] = '\0';
                        char instruccion[100];
                        instruccion[0] = '\0';
                        strcat(instruccion, "    strcpy(");
                        strcat(instruccion, variable);
                        strcat(instruccion, ", ");
                        strcat(instruccion, varcad);
                        strcat(instruccion, ");");
                        sentencias = IngresoSentencia(instruccion);

                        //Segunda variable cadena
                        break;
                    }
                }//FIn for de variable a asignar
            }//FIn tipo de asignacion :=
            return;

        }//Fin buscar variable de tipo cadena
        else//CASO ASIGNACION DE NUMEROS ENTEROS O DOBLES------------------------------------------------------------
        {
            char instruccion[100], mensaje[90];
            instruccion[0] = '\0';
            mensaje[0] = '\0';
            int ld = 0;
            for(i = k; i<strlen(nomaux)-1; i++)
            {
                mensaje[ld] = nomaux[i];
                ld++;
            }
            mensaje[ld-1] = '\0';
            strcat(instruccion, "    ");
            strcat(instruccion, variable);
            strcat(instruccion, " = ");
            strcat(instruccion, mensaje);
            strcat(instruccion, ";");
            sentencias = IngresoSentencia(instruccion);
            return;
        }
    }
    else if(buscar(nombre, "VAR")!=0)//________________________________________________________________
    {

        return;
    }
    else if(buscar(nombre, "ENTERO")!=0)//________________________________________________________________
    {

		int p;
        int tam;
        int i;
        p = Campo(nombre, "ENTERO");
        tam  = strlen("ENTERO");
        int max = tam + p;
        char instruccion[100];
        char vars[100];
        instruccion[0] = '\0';
        vars[0] = '\0';
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        strcat(instruccion, "    int ");
        p = Campo(nombre, ";");
        nombre[p] = ' ';

        for(i = max; i<p; i++)
        {
            if(nombre[i]!=' ')
            {
                break;
            }
        }
        max = i;
        char variable[50];

        int k = 0;
        for(i= max; i<=p; i++)
        {
                variable[k] = nombre[i];

                k++;
            if(nombre[i]==' ' || nombre[i]==',')
            {

                variable[k-1] = '\0';
                if(variable[0]!='\0')
                {
                	strcat(vars, variable);
                	strcat(vars, ",");

                }
                k=0;
            }
        }
        strcat(instruccion, vars);
        instruccion[strlen(instruccion)-1] = ';';
        sentencias = IngresoSentencia(instruccion);
        return;
    }
    else if(buscar(nombre, "CADENA")!=0)//________________________________________________________________
    {

        int p;
        int tam;
        int i;
        p = Campo(nombre, "CADENA");
        tam  = strlen("CADENA");
        int max = tam + p;
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        p = Campo(nombre, ";");
        nombre[p] = ' ';
        nombre[p] = ';';
        for(i = max; i<p; i++)
        {
            if(nombre[i]!=' ')
            {
                break;
            }
        }

        max = i;
        char variable[50];
        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);
        int k = 0;

        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    char ");
        for(i= max; i<=p; i++)
        {

            variable[k] = nombre[i];
            k++;
            if(nombre[i]==' ' || nombre[i]==',' || nombre[i]==';' || nombre[i]=='[')
            {

                variable[k-1] = '\0';

                if(variable[0] == '\0')
                {
                    continue;
                }
                nodovar * varl;
                varl = NULL;
                varl = BuscarVariableNodo(variable, variables);
                strcat(instruccion, variable);
                strcat(instruccion, "[");
                strcat(instruccion, varl->tam);
                strcat(instruccion, "]");
                k = 0;
                variable[0] = '\0';

                if(nombre[i]==',')
                {
                    strcat(instruccion, ", ");
                    for(i = i+1; i<p; i++)
                    {
                        if(nombre[i]!=' ')
                        {
                            break;
                        }
                    }
                    i--;
                    continue;
                }
                for(i= i +1; i<p; i++)
                {
                    if(nombre[i]==',')
                    {
                        break;
                    }
                }
                if(nombre[i] == ',')
                {
                    strcat(instruccion, ", ");
                    for(i = i+1; i<p; i++)
                    {
                        if(nombre[i]!=' ')
                        {
                            break;
                        }
                    }
                    i--;
                    continue;
                }



            }
            //FIn de la validacion de la variable

        }
        //FIn del bucle de variables
        strcat(instruccion, "; ");
        sentencias = IngresoSentencia(instruccion);
        return;

    }
    else if(buscar(nombre, "DOBLE")!=0)//________________________________________________________________
    {

		int p;
        int tam;
        int i;
        p = Campo(nombre, "DOBLE");
        tam  = strlen("DOBLE");
        int max = tam + p;
        char instruccion[100];
        char vars[100];
        instruccion[0] = '\0';
        vars[0] = '\0';
        for(i= p; i < max; i++)
        {
            nombre[i] = ' ';
        }
        strcat(instruccion, "    double ");
        p = Campo(nombre, ";");
        nombre[p] = ' ';

        for(i = max; i<p; i++)
        {
            if(nombre[i]!=' ')
            {
                break;
            }
        }
        max = i;
        char variable[50];

        int k = 0;
        for(i= max; i<=p; i++)
        {
                variable[k] = nombre[i];

                k++;
            if(nombre[i]==' ' || nombre[i]==',')
            {

                variable[k-1] = '\0';
                if(variable[0]!='\0')
                {
                	strcat(vars, variable);
                	strcat(vars, ",");

                }
                k=0;
            }
        }
        strcat(instruccion, vars);
        instruccion[strlen(instruccion)-1] = ';';
        sentencias = IngresoSentencia(instruccion);
        return;
    }
    else if(buscar(nombre, "LEER")!=0)//________________________________________________________________
    {

        char nomaux[50];
        nomaux[0] = '\0';
        strcpy(nomaux, nombre);
		int val = 0;
        int i;
        int p;
        int tam;
        int primera, segunda;
        primera = Campo(nombre, "(");
        segunda = Campo(nombre, ")");
        int px;
        int k;
        for(k = primera+1; k< segunda; k++)
        {
            if(nombre[k] != ' ')
            {
                break;
            }

        }
        primera = k;
        char variable[50];
        int l = 0;

        char tipo[20];
        tipo[0] = '\0';
        char instruccion[100];
        instruccion[0] = '\0';

        int varet = 1;

        for(k = primera; k<= segunda; k++)
        {
            variable[l] = nombre[k];
            nombre[k] = ' ';
            l++;
            if(nomaux[k] == ' ' || nomaux[k] == ')')
            {
                    variable[l-1] = '\0';
                    nodovar * var;
                    var = NULL;
                    var = BuscarVariableNodo(variable, variables);
                    tipo[0] = '\0';
                    strcpy(tipo, "ENTERA");
                    varet = strncmp(tipo, var->tipo, strlen(tipo));
                    if(varet == 0)
                    {
                        strcat(instruccion, "    scanf(");
                        strcat(instruccion, comilla);
                        strcat(instruccion, "%d");
                        strcat(instruccion, comilla);
                        strcat(instruccion, ", &");
                        strcat(instruccion, variable);
                        strcat(instruccion, ");");
                        sentencias = IngresoSentencia(instruccion);
                        return;
                    }
                    tipo[0] = '\0';
                    strcpy(tipo, "CADENA");
                    varet = strncmp(tipo, var->tipo, strlen(tipo));
                    if(varet == 0)
                    {
                        strcat(instruccion, "    scanf(");
                        strcat(instruccion, comilla);
                        strcat(instruccion, "%s");
                        strcat(instruccion, comilla);
                        strcat(instruccion, ", &");
                        strcat(instruccion, variable);
                        strcat(instruccion, ");");
                        sentencias = IngresoSentencia(instruccion);
                        return;
                    }
                    tipo[0] = '\0';
                    strcpy(tipo, "DOBLE");
                    varet = strncmp(tipo, var->tipo, strlen(tipo));
                    if(varet == 0)
                    {
                        strcat(instruccion, "    scanf(");
                        strcat(instruccion, comilla);
                        strcat(instruccion, "%f");
                        strcat(instruccion, comilla);
                        strcat(instruccion, ", &");
                        strcat(instruccion, variable);
                        strcat(instruccion, ");");
                        sentencias = IngresoSentencia(instruccion);
                        return;
                    }
                    break;
            }

        }

        return;
    }
    else if(buscar(nombre, "FINSI")!=0)//________________________________________________________________
    {

        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    }");
        sentencias = IngresoSentencia(instruccion);
        return;

    }
    else if(buscar(nombre, "SI")!=0)//________________________________________________________________
    {
        if(buscar(nombre, "FINSI")!= 0)
        {
            char instruccion[100];
            instruccion[0] = '\0';
            strcat(instruccion, "    }");
            sentencias = IngresoSentencia(instruccion);
            return;
        }

        int val = 0;
        int i;
        int p;
        int tam;
        p = Campo(nombre, "(");
        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    if");
        char condicion[50];
        condicion[0] = '\0';
        int x = strlen(nombre);
        int k = 0;
        for(i = p; i<x-1; i++)
        {
            condicion[k] = nombre[i];
            if(nombre[i] == '<')
            {
                if(nombre[i+1]=='>')
                {
                    condicion[k] = '!';
                    condicion[k+1] = '=';
                    k++;
                    i++;
                }
            }
            if(nombre[i] == '=')
            {
                if(nombre[i-1]!='<' && nombre[i-1]!='>')
                {
                    condicion[k+1] = '=';
                    k++;
                }

            }
            k++;
        }
        condicion[k] = '\0';
        strcat(instruccion, condicion);
        sentencias = IngresoSentencia(instruccion);
        return;
    }
    else if(buscar(nombre, "ENTONCES")!=0)//________________________________________________________________
    {

        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    {");
        sentencias = IngresoSentencia(instruccion);
        return;

    }
    else if(buscar(nombre, "CASOCONTRARIO")!=0)//________________________________________________________________
    {

        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    }");
        sentencias = IngresoSentencia(instruccion);
        instruccion[0] = '\0';
        strcat(instruccion, "    else");
        sentencias = IngresoSentencia(instruccion);
        instruccion[0] = '\0';
        strcat(instruccion, "    {");
        sentencias = IngresoSentencia(instruccion);
        return;
    }
    else if(buscar(nombre, "FINMIENTRAS")!=0)//________________________________________________________________
    {

        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    }");
        sentencias = IngresoSentencia(instruccion);
        return;

    }
    else if(buscar(nombre, "MIENTRAS")!=0)//________________________________________________________________
    {
        if(buscar(nombre, "FINMIENTRAS")!= 0)
        {
            char instruccion[100];
            instruccion[0] = '\0';
            strcat(instruccion, "    }");
            sentencias = IngresoSentencia(instruccion);
            return;
        }

        int val = 0;
        int i;
        int p;
        int tam;
        p = Campo(nombre, "(");
        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    while");
        char condicion[50];
        condicion[0] = '\0';
        int x = strlen(nombre);
        int k = 0;
        for(i = p; i<x-1; i++)
        {
            condicion[k] = nombre[i];
            if(nombre[i] == '<')
            {
                if(nombre[i+1]=='>')
                {
                    condicion[k] = '!';
                    condicion[k+1] = '=';
                    k++;
                    i++;
                }
            }
            if(nombre[i] == '=')
            {
                if(nombre[i-1]!='<' && nombre[i-1]!='>')
                {
                    condicion[k+1] = '=';
                    k++;
                }

            }
            k++;
        }
        condicion[k] = '\0';
        strcat(instruccion, condicion);
        sentencias = IngresoSentencia(instruccion);
        instruccion[0] = '\0';
        strcat(instruccion, "    {");
        sentencias = IngresoSentencia(instruccion);
        return;
    }
    else if(buscar(nombre, "FINPARA")!=0)//________________________________________________________________
    {

        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    }");
        sentencias = IngresoSentencia(instruccion);
        return;
    }
    else if(buscar(nombre, "PARA")!=0)//________________________________________________________________
    {
        if(buscar(nombre, "FINPARA")!= 0)
        {
            char instruccion[100];
            instruccion[0] = '\0';
            strcat(instruccion, "    }");
            sentencias = IngresoSentencia(instruccion);
            return;
        }

        int val = 0;
        int i;
        int p;
        int tam;
        p = Campo(nombre, "(");
        char instruccion[100];
        instruccion[0] = '\0';
        strcat(instruccion, "    for");
        char condicion[50];
        condicion[0] = '\0';
        int x = strlen(nombre);
        int k = 0;
        int contcoma = 0;
        char variable[20];
        variable[0] = '\0';
        int l = 0;
        for(i = p+1; i<x-1; i++)
        {
            variable[l] = nombre[i];
            l++;
            if(nombre[i] == ' ' || nombre[i] == ',')
            {
                variable[l-1] = '\0';
                break;
            }
        }


        for(i = p; i<x-1; i++)
        {

            condicion[k] = nombre[i];
            if(nombre[i] == ',')
            {
                condicion[k] = ';';
                contcoma++;
                if(contcoma == 2)
                {
                    k++;
                    break;
                }
            }
            if(contcoma==1 && nombre[i]=='=')
            {

                if(nombre[i-1]!='<' && nombre[i-1]!='>')
                {
                    condicion[k+1] = '=';
                    k++;
                }

            }
            if(nombre[i]=='<')
            {
                if(nombre[i+1]=='>')
                {

                    condicion[k] = '!';
                    condicion[k+1] = '=';
                    k++;
                    i++;
                }
            }
            k++;
        }
        for(i = i+1; i<x-1; i++)
        {
            if(nombre[i]!=' ')
            {
                break;
            }
        }

        condicion[k] = '\0';
        strcat(instruccion, condicion);
        strcat(instruccion, " ");
        strcat(instruccion, variable);
        strcat(instruccion, "+=");
        condicion[0] = '\0';
        l = 0;
        k = i;
        for(i = k; i<x-1; i++)
        {
            condicion[l] = nombre[i];
            l++;
        }
        condicion[l-1] = '\0';
        strcat(instruccion, condicion);
        strcat(instruccion, ")");
        sentencias = IngresoSentencia(instruccion);
        instruccion[0] = '\0';
        strcat(instruccion, "    {");
        sentencias = IngresoSentencia(instruccion);
        return;

    }
    else
    {
        sentencias = IngresoSentencia(" ");
        return;
    }
}

void ConstruirC()
{
    sentencias = NULL;

    sentencias = IngresoSentencia("//Codigo generado por el Lenguaje ED");
    sentencias = IngresoSentencia(" ");
    sentencias = IngresoSentencia("#include <stdlib.h>");
    sentencias = IngresoSentencia("#include <stdio.h>");
    sentencias = IngresoSentencia("#include <string.h>");
    sentencias = IngresoSentencia(" ");
    sentencias = IngresoSentencia("int main()");
    sentencias = IngresoSentencia("{");

    FILE *fp;
    char * c, cad[100];
    fp=fopen("Codigo.ced", "r");
    int val;
    int linea = 1;
    while(c=fgets(cad,100,fp))
    {
        ConstruirArchivoC(cad);
    }
    fclose(fp);

    sentencias = IngresoSentencia("    return 0;");
    sentencias = IngresoSentencia("}");

    printf("\n");
    EstadoSentencias();

    FILE * ag;
    ag=fopen("Codigo.c", "w");

    if(sentencias == NULL)
	{
		fprintf(ag, "//Codigo generado por lenguaje ED");
		fprintf(ag, "\n");
		fprintf(ag, "\n");
	}
	else
	{
		nodoc * aux;
		aux = sentencias;
		while(aux!=NULL)
		{
			fprintf(ag, "\n%s", aux->instruccion);
			aux = aux->siguiente;
		}
	}


    fclose(ag);
}

void CreaComilla()
{
	FILE *Com;
	comilla[0] = '\0';
    char * c, cad[2];
    Com=fopen("Comilla.txt", "r");
    int val;
    int linea = 1;
    cad[0] = '\0';
    while(c=fgets(cad,25,Com))
    {
        strcpy(comilla, cad);
        break;
    }
    fclose(Com);

}

void ConstruirArchivoConstructor()
{
    FILE * ag;
    ag=fopen("ErroresCompilacion.txt", "w");


        fprintf(ag, "          ___________________________________________________________________________________________________________________________________\n");
        fprintf(ag, "\n");
		fprintf(ag, "                                                                                                       CODIGO CONVERTIDO A C SATISFACTORIAMENTE");
		fprintf(ag, "\n");
		fprintf(ag, "          ___________________________________________________________________________________________________________________________________\n");
		fprintf(ag, "\n");
		fprintf(ag, "\n");


    fclose(ag);



}


void ConstruirArchivoErrores()
{
    FILE * ag;
    ag=fopen("ErroresCompilacion.txt", "w");

    if(errores == NULL)
	{
        fprintf(ag, "          ___________________________________________________________________________________________________________________________________\n");
        fprintf(ag, "\n");
		fprintf(ag, "                                                                                                     NO SE HAN ENCONTRADO ERRORES DE COMPILACION");
		fprintf(ag, "\n");
		fprintf(ag, "          ___________________________________________________________________________________________________________________________________\n");
		fprintf(ag, "\n");
		fprintf(ag, "\n");
	}
	else
	{
		Error * aux;
		aux = errores;
		fprintf(ag, "\nERRORES ENCONTRADOS EN LA COMPILACION: \n");
		fprintf(ag, "_________________________________________________________________________________________________________________________________________\n");
		while(aux!=NULL)
		{
			fprintf(ag, "\n Linea %d: %s.", aux->linea, aux->nombre);
			aux = aux->siguiente;
		}
		fprintf(ag, "\n_________________________________________________________________________________________________________________________________________\n");
	}


    fclose(ag);
}



gboolean verificar_guardado();

void cargar_archivo_gtk (Win2 * obj, gchar *nombreArchivo) {
	GError			*err=NULL;
	gchar			*estado,*texto;
	gboolean		resultado;
	GtkTextBuffer	*buffer,*buffer2,*buffer3;

	/*ponemos mensaje de estado, en la barra de estado*/
	estado = g_strdup_printf("Cargando %s, ",nombreArchivo);
	gtk_statusbar_push (GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id,estado);
	g_free(estado);

	while (gtk_events_pending()) gtk_main_iteration();

	/*rescatamos el contenido del archivo.*/
	resultado = g_file_get_contents (nombreArchivo, &texto, NULL, &err);

	if (resultado == FALSE)
	{
		/*manejo de errores*/
		error (err->message);
		g_error_free (err);
		g_free (nombreArchivo);

	}
	/*dejamos en blanco el textarea*/
	gtk_widget_set_sensitive (obj->textview,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textview));
	gtk_text_buffer_set_text (buffer,texto,-1);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textview,TRUE);
	g_free (texto);

	/*como el archivo ah sido cargado, ponemos su nombre en la barra de estado */
	if (obj->nombreArchivo != NULL ) g_free (obj->nombreArchivo);
	obj->nombreArchivo = nombreArchivo;
	/*reseteamos los mensajes de estado */
	gtk_statusbar_pop (GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id);
	EstadoArch(obj);
}

void gtk_my_patter_add(GtkWidget   *filechooser,
                  const gchar *title,
                  const gchar *pattern,
                  const gint   id)
{
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, title);
    gtk_file_filter_add_pattern(filter, pattern);
    g_object_set_data( G_OBJECT( filter ), "id", GINT_TO_POINTER( id ) );
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(filechooser), filter);
}

gchar* abrir_archivo_gtk (Win2 *obj) {



	GtkWidget		*abrirArchivo;
	gchar			*nombreArchivo=NULL;

	/*creamos ventana para abrir archivo*/
	abrirArchivo = gtk_file_chooser_dialog_new ("Abrir Achivo.",GTK_WINDOW(obj->window),GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_OPEN,GTK_RESPONSE_OK,NULL);


    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(abrirArchivo), TRUE);
    gtk_my_patter_add(abrirArchivo, "Archivo ED (*.ced)", "*.ced", 0);
    gtk_my_patter_add(abrirArchivo, "Todos los archivos (*.*)", "*", 1);

	/*verificamos la respuesta de la ventana*/
	if (gtk_dialog_run(GTK_DIALOG(abrirArchivo)) == GTK_RESPONSE_OK)
	{
		nombreArchivo = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (abrirArchivo));
	}
	gtk_widget_destroy(abrirArchivo);



	return nombreArchivo;
}

void ArchivoAbrir(GtkWidget * constructor, Win2 * obj)
{
//    EstadoBarra(obj, 1);

    if (verificar_guardado(obj) == TRUE)
	{
		ArchivoGuardar(NULL,obj);
	}

    gchar	*nombreArchivo;
	nombreArchivo = abrir_archivo_gtk (obj);
	if (nombreArchivo != NULL )cargar_archivo_gtk(obj,nombreArchivo);
	if(nombreArchivo== NULL)
	{
        if(obj->nombreArchivo == NULL)
        {
  //              EstadoBarra(obj, 0);
//
        }
        else{
  //          EstadoArch(obj);
        }
	}

	GtkTextBuffer	*buffer;
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textviewc));
	gtk_text_buffer_set_text (buffer,"",-1);
	gtk_text_buffer_set_modified (buffer,FALSE);


    if(obj1->nombreArchivo==NULL)
    {
        gtk_widget_set_sensitive (obj->nuevo,TRUE);
        gtk_widget_set_sensitive (obj->abrir,TRUE);
        gtk_widget_set_sensitive (obj->guardar,FALSE);
        gtk_widget_set_sensitive (obj->ayuda,TRUE);
        return;
    }

    gtk_widget_set_sensitive (obj->archivoguardar,FALSE);
    gtk_widget_set_sensitive (obj->archivoguardarcomo,TRUE);

    gtk_widget_set_sensitive (obj->ejecutarcompilar,TRUE);
    gtk_widget_set_sensitive (obj->ejecutarcompilaryejecutar,TRUE);
    gtk_widget_set_sensitive (obj->ejecutarejecutar,TRUE);

    gtk_widget_set_sensitive (obj->editarbuscar,TRUE);
    gtk_widget_set_sensitive (obj->editarreemplazar,TRUE);
    gtk_widget_set_sensitive (obj->editareliminar,TRUE);
    gtk_widget_set_sensitive (obj->editarcopiar,TRUE);
    gtk_widget_set_sensitive (obj->editarcortar,TRUE);
    gtk_widget_set_sensitive (obj->editarpegar,TRUE);


    gtk_widget_set_sensitive (obj->guardar,FALSE);
    gtk_widget_set_sensitive (obj->guardarcomo,TRUE);
    gtk_widget_set_sensitive (obj->compilar,TRUE);
    gtk_widget_set_sensitive (obj->ejecutar,TRUE);
    gtk_widget_set_sensitive (obj->copiar,TRUE);
    gtk_widget_set_sensitive (obj->cortar,TRUE);
    gtk_widget_set_sensitive (obj->pegar,TRUE);
    gtk_widget_set_sensitive (obj->buscar,TRUE);
    gtk_widget_set_sensitive (obj->limpiar,TRUE);
    gtk_widget_set_sensitive (obj->reemplazar,TRUE);
    gtk_widget_set_sensitive (obj->ayuda,TRUE);



    EstadoArch(obj);

    gtk_widget_grab_focus(obj->textview);
}
void ArchivoNuevo(GtkWidget * constructor, Win2 * obj)
{

    if (verificar_guardado(obj) == TRUE)
	{
		ArchivoGuardar(NULL,obj);
	}

	GtkTextBuffer	*buffer;



	/*dejamos en blanco el textarea*/
	gtk_widget_set_sensitive (obj->textview,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textview));
	gtk_text_buffer_set_text (buffer,"",-1);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textview,TRUE);

    //En blanco los errores

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textviewc));
	gtk_text_buffer_set_text (buffer,"",-1);
	gtk_text_buffer_set_modified (buffer,FALSE);



    gtk_widget_set_sensitive (obj->archivoguardar,FALSE);
    gtk_widget_set_sensitive (obj->archivoguardarcomo,TRUE);

    gtk_widget_set_sensitive (obj->ejecutarcompilar,TRUE);
    gtk_widget_set_sensitive (obj->ejecutarcompilaryejecutar,TRUE);
    gtk_widget_set_sensitive (obj->ejecutarejecutar,TRUE);

    gtk_widget_set_sensitive (obj->editarbuscar,TRUE);
    gtk_widget_set_sensitive (obj->editarreemplazar,TRUE);
    gtk_widget_set_sensitive (obj->editareliminar,TRUE);
    gtk_widget_set_sensitive (obj->editarcopiar,TRUE);
    gtk_widget_set_sensitive (obj->editarcortar,TRUE);
    gtk_widget_set_sensitive (obj->editarpegar,TRUE);


    gtk_widget_set_sensitive (obj->guardar,FALSE);
    gtk_widget_set_sensitive (obj->guardarcomo,TRUE);
    gtk_widget_set_sensitive (obj->compilar,FALSE);
    gtk_widget_set_sensitive (obj->ejecutar,FALSE);
    gtk_widget_set_sensitive (obj->copiar,TRUE);
    gtk_widget_set_sensitive (obj->cortar,TRUE);
    gtk_widget_set_sensitive (obj->pegar,TRUE);
    gtk_widget_set_sensitive (obj->buscar,TRUE);
    gtk_widget_set_sensitive (obj->limpiar,TRUE);
    gtk_widget_set_sensitive (obj->reemplazar,TRUE);
    gtk_widget_set_sensitive (obj->ayuda,TRUE);


    obj->nombreArchivo = NULL;
    EstadoArch(obj);

    gtk_widget_set_sensitive (obj->copiar,TRUE);
        gtk_widget_set_sensitive (obj->pegar,TRUE);
        gtk_widget_set_sensitive (obj->cortar,TRUE);
        gtk_widget_set_sensitive (obj->buscar,TRUE);
        gtk_widget_set_sensitive (obj->reemplazar,TRUE);

        gtk_widget_set_sensitive (obj->limpiar,TRUE);

    gtk_widget_grab_focus(obj->textview);



}

gchar* guardar_archivo_gtk (Win2 * obj) {
	GtkWidget 	*salvarArchivo;
	gchar		*nombreArchivo=NULL;
    gchar * nombre;
//    bbbbbbbbbbbbbbbb

    gtk_widget_set_sensitive (obj1->guardar,FALSE);
    gtk_widget_set_sensitive (obj1->guardarcomo,FALSE);
    gtk_widget_set_sensitive (obj1->abrir,FALSE);
    gtk_widget_set_sensitive (obj1->nuevo,FALSE);
    gtk_widget_set_sensitive (obj1->buscar,FALSE);
    gtk_widget_set_sensitive (obj1->reemplazar,FALSE);
    gtk_widget_set_sensitive (obj1->ayuda,FALSE);

    if(obj->nombreArchivo == NULL)
    {
        nombre = g_strdup_printf ("Sin titulo.ced");
    }
    else{
        nombre = g_strdup_printf(".ced");
    }


	/*creamos ventana para guardar archivo*/
	salvarArchivo = gtk_file_chooser_dialog_new ("Guardar Archivo.",GTK_WINDOW(obj->window),GTK_FILE_CHOOSER_ACTION_SAVE,
			GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,GTK_STOCK_SAVE,GTK_RESPONSE_OK,NULL);

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(salvarArchivo), nombre);



    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(salvarArchivo), TRUE);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(salvarArchivo), TRUE);
    gtk_my_patter_add(salvarArchivo, "Archivo ED (*.ced)", "*.ced", 0);
    gtk_my_patter_add(salvarArchivo, "Todos los archivos (*.*)", "*", 1);
	/*verificamos la respuesta de la ventana*/
	if (gtk_dialog_run(GTK_DIALOG(salvarArchivo)) == GTK_RESPONSE_OK)
	{
		nombreArchivo = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (salvarArchivo));
	}
	gtk_widget_destroy(salvarArchivo);

	gtk_widget_set_sensitive (obj1->guardar,TRUE);
    gtk_widget_set_sensitive (obj1->guardarcomo,TRUE);
    gtk_widget_set_sensitive (obj1->abrir,TRUE);
    gtk_widget_set_sensitive (obj1->nuevo,TRUE);
    gtk_widget_set_sensitive (obj1->buscar,TRUE);
    gtk_widget_set_sensitive (obj1->reemplazar,TRUE);
    gtk_widget_set_sensitive (obj1->ayuda,TRUE);


	return nombreArchivo;
}

void escribir_archivo (Win2 *obj, gchar *nombreArchivo) {
	GError			*err=NULL;
	gchar			*estado,*texto;
	gboolean		resultado;
	GtkTextBuffer	*buffer;
	GtkTextIter		inicio,fin;

	/*añadimos mensaje de informacion a la barra de estado */
	if (nombreArchivo != NULL)
	{
		estado = g_strdup_printf ("Guardando %s",nombreArchivo);
	}else{
		estado = g_strdup_printf ("Guardando %s",obj->nombreArchivo);
	}
	gtk_statusbar_push (GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id,estado);

	g_free (estado);		//liberamos memoria
	while (gtk_events_pending()) gtk_main_iteration();
	/*desabilitamos el area de texto y obtenemos el contenido del buffer de text*/
	gtk_widget_set_sensitive(obj->textview,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(obj->textview));
	gtk_text_buffer_get_start_iter (buffer,&inicio);
	gtk_text_buffer_get_end_iter (buffer,&fin);
	texto = gtk_text_buffer_get_text (buffer,&inicio,&fin,FALSE);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textview,TRUE);


	/*pasamos el contenido del archivo al texto desde el buffer*/
	if (nombreArchivo != NULL)
	{
		resultado = g_file_set_contents (nombreArchivo,texto,-1,&err);
	}else{
		resultado = g_file_set_contents (obj->nombreArchivo,texto,-1,&err);
	}

	if (resultado == FALSE)
	{
		/*error guardando el archivo */
		error (err->message);
		g_error_free (err);
	}
	g_free (texto);			//liberamos memoria

	if (nombreArchivo != NULL)
	{
		/*necesitamos borra de memoria el archivo previuamente abierto y redireccionarlo al nuevo*/
		if (obj->nombreArchivo != NULL) g_free (obj->nombreArchivo);
		obj->nombreArchivo = nombreArchivo;
	}

	/*borramos estados de la barra de estado y vovemos al por defecto*/
	gtk_statusbar_pop (GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id);
	EstadoArch(obj);

}

void ArchivoGuardar(GtkWidget *constructor, Win2 * obj)
{
    EstadoBarra(obj, 2);
    gchar			*nombreArchivo;



	if(obj->nombreArchivo == NULL)
	{
		nombreArchivo = guardar_archivo_gtk(obj);
		if (nombreArchivo != NULL) escribir_archivo(obj,nombreArchivo);
	}else
	{
		escribir_archivo (obj,NULL);
	}


    if(obj->nombreArchivo == NULL)
    {
        gtk_widget_set_sensitive (obj->ejecutarcompilar,FALSE);
        gtk_widget_set_sensitive (obj->ejecutarcompilaryejecutar,FALSE);
        gtk_widget_set_sensitive (obj->ejecutarejecutar,FALSE);
        gtk_widget_set_sensitive (obj->compilar,FALSE);
        gtk_widget_set_sensitive (obj->ejecutar,FALSE);
    }
    else
    {
        gtk_widget_set_sensitive (obj->compilar,TRUE);
        gtk_widget_set_sensitive (obj->copiar,TRUE);
        gtk_widget_set_sensitive (obj->pegar,TRUE);
        gtk_widget_set_sensitive (obj->cortar,TRUE);
        gtk_widget_set_sensitive (obj->buscar,TRUE);
        gtk_widget_set_sensitive (obj->reemplazar,TRUE);
        gtk_widget_set_sensitive (obj->ejecutar,TRUE);
        gtk_widget_set_sensitive (obj->limpiar,TRUE);

    }
}
void ArchivoGuardarComo(GtkWidget *constructor, Win2 * obj)
{
    EstadoBarra(obj, 2);


    gchar	*nombreArchivo;
    nombreArchivo = guardar_archivo_gtk(obj);
    if (nombreArchivo != NULL)
    {
        escribir_archivo(obj,nombreArchivo);
    }


    if(obj->nombreArchivo == NULL)
    {
        gtk_widget_set_sensitive (obj->ejecutarcompilar,FALSE);
        gtk_widget_set_sensitive (obj->ejecutarcompilaryejecutar,FALSE);
        gtk_widget_set_sensitive (obj->ejecutarejecutar,FALSE);
        gtk_widget_set_sensitive (obj->compilar,FALSE);
        gtk_widget_set_sensitive (obj->ejecutar,FALSE);
    }
    else
    {
        gtk_widget_set_sensitive (obj->compilar,TRUE);
        gtk_widget_set_sensitive (obj->copiar,TRUE);
        gtk_widget_set_sensitive (obj->pegar,TRUE);
        gtk_widget_set_sensitive (obj->cortar,TRUE);
        gtk_widget_set_sensitive (obj->buscar,TRUE);
        gtk_widget_set_sensitive (obj->reemplazar,TRUE);
        gtk_widget_set_sensitive (obj->ejecutar,TRUE);
        gtk_widget_set_sensitive (obj->limpiar,TRUE);
    }
}

gboolean verificar_guardado (Win2 *obj) {
	gboolean	respuesta = FALSE;
	GtkTextBuffer	*buffer;

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textview));

	if (gtk_text_buffer_get_modified (buffer) == TRUE) //si el buffer ah sido modificado
	{
		//llamamos a la ventana para guardar el archivo
		GtkWidget			*dialogo;
		const gchar *msj =	"¿Desea guardar los cambios realizados?";

		dialogo = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",msj);
		gtk_window_set_title (GTK_WINDOW(dialogo), "Guardar cambios");
		if(gtk_dialog_run (GTK_DIALOG(dialogo)) == GTK_RESPONSE_NO)
		{
			respuesta = FALSE;
		}else{
			respuesta = TRUE;
		}
		gtk_widget_destroy(dialogo);
	}

	return respuesta;

}


void ArchivoSalir(GtkWidget *constructor, Win2 * obj)
{
    if (verificar_guardado(obj) == TRUE)
	{
		ArchivoGuardar(NULL,obj);
	}
	gtk_main_quit();
}
void EditarCopiar(GtkWidget *constructor, Win2 * obj)
{
    GtkClipboard * clicpboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_text_buffer_copy_clipboard(obj->buffer, clicpboard);
}
void EditarPegar(GtkWidget *constructor, Win2 * obj)
{
    GtkClipboard * clicpboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_text_buffer_paste_clipboard(obj->buffer, clicpboard, NULL, TRUE);
}
void EditarCortar(GtkWidget *constructor, Win2 * obj)
{
    GtkClipboard * clicpboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_text_buffer_cut_clipboard(obj->buffer, clicpboard, TRUE);
}


gboolean key_pressed(GtkWidget * window, GdkEventKey * event, GtkTextBuffer * buffer)
{
    GtkTextIter start_sel, end_sel;
    GtkTextIter start_find, end_find;
    GtkTextIter start_match, end_match;
    gboolean selected;
    gchar * text;


    if((event->type == GDK_KEY_PRESS)  &&  (event->state & GDK_CONTROL_MASK))
    {
        switch(event->keyval)
        {

            case GDK_KEY_b:
            case GDK_KEY_B:

            selected = gtk_text_buffer_get_selection_bounds(buffer, &start_sel, &end_sel);

            if(selected)
            {
                gtk_text_buffer_get_start_iter(buffer, &start_find);
                gtk_text_buffer_get_end_iter(buffer, &end_find);

                gtk_text_buffer_remove_tag_by_name(buffer, "green_bg", &start_find, &end_find);
                text = (gchar *) gtk_text_buffer_get_text(buffer, &start_sel, &end_sel, FALSE);

                while(gtk_text_iter_forward_search(&start_find, text, GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY, &start_match, &end_match, NULL))
                {
                    gtk_text_buffer_apply_tag_by_name(buffer, "green_bg", &start_match, &end_match);

                    gint offset = gtk_text_iter_get_offset(&end_match);

                    gtk_text_buffer_get_iter_at_offset(buffer, &start_find, offset);
                }

                g_free(text);

            }
            break;

            case GDK_KEY_l:
            case GDK_KEY_L:

            gtk_text_buffer_get_start_iter(buffer, &start_find);
            gtk_text_buffer_get_end_iter(buffer, &end_find);

            gtk_text_buffer_remove_tag_by_name(buffer, "green_bg", &start_find, &end_find);
            break;




        }
    }
return FALSE;

}

GtkWidget * VentanaEncontrar;
GtkWidget * chkTextCase;
GtkWidget * chkTextSelect;


void EncontrarTexto(gchar * texto)
{
    gboolean selected = FALSE;
    GtkTextIter start_find, end_find;
    GtkTextIter start_match, end_match;
    GtkTextSearchFlags xSearchFlag;
    int caso = -1;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkTextCase)))
    {
        caso = 0;
//        xSearchFlag = gtk_text_iter_forward_search(GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY);
    }
    else
    {
        caso = 1;
  //      xSearchFlag = GtkTextSearchFlags(GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY | GTK_TEXT_SEARCH_CASE_INSENSITIVE);
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkTextSelect)))
    {
        selected = gtk_text_buffer_get_selection_bounds(obj1->buffer, &start_find , &end_find);
    }
    else
    {
        gtk_text_buffer_get_start_iter(obj1->buffer, &start_find);
        gtk_text_buffer_get_end_iter(obj1->buffer, &end_find);
        //g_print("Encontrar en todo el texto.");
        //selected = gtk_text_buffer_get_selection_bounds(obj1->buffer, &start_find , &end_find);

    }

    gtk_text_buffer_remove_tag_by_name(obj1->buffer, "green_bg", &start_find, &end_find);
    int coincidencias = 0;
    if(caso == 0)
    {
        while(gtk_text_iter_forward_search(&start_find, texto, GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY, &start_match, &end_match, &end_find))
        {
            gtk_text_buffer_apply_tag_by_name(obj1->buffer, "green_bg", &start_match, &end_match);
            gint offset = gtk_text_iter_get_offset(&end_match);
            gtk_text_buffer_get_iter_at_offset(obj1->buffer, &start_find, offset);
            coincidencias++;

        }

    }
    else if(caso == 1)
    {
                while(gtk_text_iter_forward_search(&start_find, texto, GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY | GTK_TEXT_SEARCH_CASE_INSENSITIVE, &start_match, &end_match, &end_find))
            {
                gtk_text_buffer_apply_tag_by_name(obj1->buffer, "green_bg", &start_match, &end_match);
                gint offset = gtk_text_iter_get_offset(&end_match);
                gtk_text_buffer_get_iter_at_offset(obj1->buffer, &start_find, offset);
                coincidencias++;

            }

    }
    else
    {
        while(gtk_text_iter_forward_search(&start_find, texto,xSearchFlag , &start_match, &end_match, &end_find))
        {
            gtk_text_buffer_apply_tag_by_name(obj1->buffer, "green_bg", &start_match, &end_match);
            gint offset = gtk_text_iter_get_offset(&end_match);
            gtk_text_buffer_get_iter_at_offset(obj1->buffer, &start_find, offset);

            coincidencias++;
        }

    }



    if(coincidencias == 0)
    {
        GtkWidget * dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(VentanaEncontrar), GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "No se han encontrado coincidencias en el texto.");

        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

    }


    if(selected)
    {
        gtk_text_buffer_select_range(obj1->buffer, &start_find, &start_find);
    }
    gtk_widget_destroy(VentanaEncontrar);

}

void BuscarTexto(GtkEntry * texto)
{
    const gchar * gstrTexto;
    GtkWidget * dialog;
    gint16 giLongitud;
    giLongitud = gtk_entry_get_text_length(texto);
    if(giLongitud == 0)
    {
        dialog = gtk_message_dialog_new(GTK_WINDOW(VentanaEncontrar), GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Debe capturar el texto a Encontrar");

        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    else
    {
        gstrTexto = gtk_entry_get_text(texto);
        EncontrarTexto(gstrTexto);

    }
}

void EditarBuscar(Win2 * obj)
{

    GtkWidget * boxBuscar;
    GtkWidget * lblBuscar;
    GtkWidget * txtBuscar;
    GtkWidget * btnBuscar;
    GtkWidget * btnCancelar;

    //NUEVA VENTANA
    VentanaEncontrar = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //SIN REDIMENSION
    gtk_window_set_resizable(GTK_WINDOW(VentanaEncontrar), FALSE);

    //vENTANA DE DIALOGO
    gtk_window_set_type_hint(GTK_WINDOW(VentanaEncontrar), GDK_WINDOW_TYPE_HINT_MENU);

    //UBicacion de la ventana
    gtk_window_set_position(GTK_WINDOW(VentanaEncontrar), GTK_WIN_POS_CENTER);

    gtk_window_set_title(GTK_WINDOW(VentanaEncontrar), "Encontrar");
    gtk_container_set_border_width(GTK_CONTAINER(VentanaEncontrar), 10);
    gtk_widget_set_size_request(VentanaEncontrar, 350, 200);

    gtk_window_set_modal(GTK_WINDOW(VentanaEncontrar), TRUE);

    lblBuscar = gtk_label_new("Texto a encontrar: ");
    txtBuscar = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(txtBuscar), 40);
    btnBuscar = gtk_button_new_with_label("Encontrar");
    btnCancelar = gtk_button_new_with_label("Cancelar");

    chkTextCase = gtk_check_button_new_with_label("En texto especifico.");
    chkTextSelect = gtk_check_button_new_with_label("El texto seleccionado");

    boxBuscar = gtk_fixed_new();

    gtk_fixed_put(GTK_FIXED(boxBuscar),lblBuscar ,0 , 10);
    gtk_fixed_put(GTK_FIXED(boxBuscar), txtBuscar,0 ,30 );
    gtk_fixed_put(GTK_FIXED(boxBuscar), chkTextCase,0 ,70 );
    gtk_fixed_put(GTK_FIXED(boxBuscar), chkTextSelect, 0, 100);
    gtk_fixed_put(GTK_FIXED(boxBuscar), btnBuscar, 0, 140);
    gtk_fixed_put(GTK_FIXED(boxBuscar), btnCancelar, 263, 140 );

    //Anade contenedor a la ventana
    gtk_container_add(GTK_CONTAINER(VentanaEncontrar), boxBuscar);

    //MUestra los objetos
    gtk_widget_show_all(VentanaEncontrar);

    //Senales
    g_signal_connect_swapped(G_OBJECT(btnCancelar), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) VentanaEncontrar);

    g_signal_connect_swapped(G_OBJECT(btnBuscar), "clicked", G_CALLBACK(BuscarTexto), (gpointer) txtBuscar);

}


GtkWidget * chkTextReplace;
GtkWidget * chktxtReplace;
GtkWidget * txtReplace;

void ReemplazaTextoBuffer(const gchar * texto)
{
    gboolean selected = FALSE;
    GtkTextIter start_find, end_find;
    GtkTextIter start_match, end_match;
    GtkTextSearchFlags xSearchFlag;

    const gchar * gstrReplace;
    gboolean * bExacta;
    gchar * strTextoSeleccionado;
    const gchar * strTextReemplazado;
    int tipo = -1;

    //Obtiene el texto que se utilizara para reemplazar
    gstrReplace = gtk_entry_get_text(GTK_ENTRY(txtReplace));

    //Busqueda exacta
    //bExacta = gtk_toggle_action_get_active(GTK_TOGGLE_BUTTON(chkTextCase));

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkTextCase)))
    {
        tipo = 0;
    }
    else
    {
        tipo = 1;
    }
    int coindidencias = 0;
    if(gtk_toggle_action_get_active(GTK_TOGGLE_BUTTON(chkTextSelect)))
    {
        //Obtiene el texto seleccionado
        selected = gtk_text_buffer_get_selection_bounds(obj1->buffer, &start_find, &end_find);

        if(selected)
        {
            strTextoSeleccionado = (gchar * )gtk_text_buffer_get_text(obj1->buffer, &start_find, &end_find, FALSE);

            //Borra el texto seleccionado
            gtk_text_buffer_delete(obj1->buffer, &start_find, &end_find);

            //Ubica el cursor
            gtk_text_buffer_place_cursor(obj1->buffer, &start_find);

        }
    }
    else
    {
        gtk_text_buffer_get_start_iter(obj1->buffer, &start_find);
        gtk_text_buffer_get_end_iter(obj1->buffer, &end_find);
        if(tipo == 0)
        {

            while(gtk_text_iter_forward_search(&start_find, texto, GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY, &start_match, &end_match, &end_find))
            {

                //Borra el texto
                gtk_text_buffer_delete(obj1->buffer, &start_match, &end_match);

                //Ubica el cursor
                gtk_text_buffer_place_cursor(obj1->buffer, &start_match);

                //Inserta el nuevo texto en el buffer
                gtk_text_buffer_insert_at_cursor(obj1->buffer, gstrReplace, strlen(gstrReplace));

                //Obtiene el inicio y el fin del buffer
                gtk_text_buffer_get_start_iter(obj1->buffer, &start_find);
                gtk_text_buffer_get_end_iter(obj1->buffer, &end_find);
                coindidencias++;

            }

        }
        else if(tipo == 1)
        {
            while(gtk_text_iter_forward_search(&start_find, texto, GTK_TEXT_SEARCH_TEXT_ONLY | GTK_TEXT_SEARCH_VISIBLE_ONLY | GTK_TEXT_SEARCH_CASE_INSENSITIVE, &start_match, &end_match, &end_find))
            {

                //Borra el texto
                gtk_text_buffer_delete(obj1->buffer, &start_match, &end_match);

                //Ubica el cursor
                gtk_text_buffer_place_cursor(obj1->buffer, &start_match);

                //Inserta el nuevo texto en el buffer
                gtk_text_buffer_insert_at_cursor(obj1->buffer, gstrReplace, strlen(gstrReplace));

                //Obtiene el inicio y el fin del buffer
                gtk_text_buffer_get_start_iter(obj1->buffer, &start_find);
                gtk_text_buffer_get_end_iter(obj1->buffer, &end_find);
                coindidencias++;

            }

        }

    }

    if(coindidencias == 0)
    {
        GtkWidget * dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(VentanaEncontrar), GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "No se han encontrado coincidencias en el texto.");

        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

    }

    gtk_widget_destroy(VentanaEncontrar);

}

void ReemplazaTextoVal(GtkEntry *texto)
{
    const gchar * gstrTexto;

    GtkWidget * dialog;
    gint16 giLongitud;

    giLongitud = gtk_entry_get_text_length(texto);

    if(giLongitud == 0)
    {
        dialog = gtk_message_dialog_new(GTK_WINDOW(VentanaEncontrar), GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Debe ingresar el texto a reemplazar.");

        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    else
    {
        gstrTexto = gtk_entry_get_text(texto);
        ReemplazaTextoBuffer(gstrTexto);


    }
}



void EditarReemplazar()
{
    GtkWidget * boxBuscar;
    GtkWidget * lblBuscar;
    GtkWidget * lblReplace;
    GtkWidget * txtBuscar;
    GtkWidget * btnBuscar;
    GtkWidget * btnCancelar;

    //NUEVA VENTANA
    VentanaEncontrar = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //SIN REDIMENSION
    gtk_window_set_resizable(GTK_WINDOW(VentanaEncontrar), FALSE);

    //vENTANA DE DIALOGO
    gtk_window_set_type_hint(GTK_WINDOW(VentanaEncontrar), GDK_WINDOW_TYPE_HINT_MENU);

    //UBicacion de la ventana
    gtk_window_set_position(GTK_WINDOW(VentanaEncontrar), GTK_WIN_POS_CENTER);

    gtk_window_set_title(GTK_WINDOW(VentanaEncontrar), "Reemplazar");
    gtk_container_set_border_width(GTK_CONTAINER(VentanaEncontrar), 10);
    gtk_widget_set_size_request(VentanaEncontrar, 300, 200);

    gtk_window_set_modal(GTK_WINDOW(VentanaEncontrar), TRUE);

    lblBuscar = gtk_label_new("Texto exacto.");
    txtBuscar = gtk_entry_new();
    lblReplace = gtk_label_new("Texto a reemplazar");
    txtReplace = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(txtBuscar), 40);
    gtk_entry_set_width_chars(GTK_ENTRY(txtReplace), 40);
    btnBuscar = gtk_button_new_with_label("Reemplazar");
    btnCancelar = gtk_button_new_with_label("Cancelar");

    chkTextCase = gtk_check_button_new_with_label("El texto exacto");
    chkTextSelect = gtk_check_button_new_with_label("En el texto seleccionado");

    boxBuscar = gtk_fixed_new();

    gtk_fixed_put(GTK_FIXED(boxBuscar),lblBuscar ,0 , 10);
    gtk_fixed_put(GTK_FIXED(boxBuscar), txtBuscar,0 ,30 );
    gtk_fixed_put(GTK_FIXED(boxBuscar),lblReplace ,0 , 70);
    gtk_fixed_put(GTK_FIXED(boxBuscar), txtReplace,0 , 90);

    gtk_fixed_put(GTK_FIXED(boxBuscar), chkTextCase,0 ,130 );
    //gtk_fixed_put(GTK_FIXED(boxBuscar), chkTextSelect, 0, 160);
    gtk_fixed_put(GTK_FIXED(boxBuscar), btnBuscar, 0, 170);
    gtk_fixed_put(GTK_FIXED(boxBuscar), btnCancelar, 263, 170 );

    //Anade contenedor a la ventana
    gtk_container_add(GTK_CONTAINER(VentanaEncontrar), boxBuscar);

    //MUestra los objetos
    gtk_widget_show_all(VentanaEncontrar);

    //Senales
    g_signal_connect_swapped(G_OBJECT(btnCancelar), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) VentanaEncontrar);

    g_signal_connect_swapped(G_OBJECT(btnBuscar), "clicked", G_CALLBACK(ReemplazaTextoVal), (gpointer) txtBuscar);


}

void EditarEliminar(GtkWidget *constructor, Win2 * obj)
{
    GtkTextBuffer	*buffer;


	/*dejamos en blanco el textarea*/
	gtk_widget_set_sensitive (obj->textview,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textview));
	gtk_text_buffer_set_text (buffer,"",-1);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textview,TRUE);

    //En blanco los errores

	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textviewc));
	gtk_text_buffer_set_text (buffer,"",-1);
	gtk_text_buffer_set_modified (buffer,FALSE);
}


void CargarArchivoError(GtkWidget *constructor, Win2 * obj)
{

    GError			*err=NULL;
	gchar			*estado,*texto;
	gboolean		resultado;
	GtkTextBuffer	*buffer,*buffer2,*buffer3;
    gchar *dir;
    dir = g_strdup_printf("ErroresCompilacion.txt");


	while (gtk_events_pending()) gtk_main_iteration();

	/*rescatamos el contenido del archivo.*/
	resultado = g_file_get_contents (dir, &texto, NULL, &err);

	if (resultado == FALSE)
	{
		/*manejo de errores*/
		error (err->message);
		g_error_free (err);
		g_free (dir);

	}
	/*dejamos en blanco el textarea*/
	gtk_widget_set_sensitive (obj->textviewc,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->textviewc));
	gtk_text_buffer_set_text (buffer,texto,-1);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textviewc,FALSE);
	g_free (texto);

}

void EjecutarEjecutar(GtkWidget *constructor, Win2 * obj)
{
    EstadoBarra(obj, 3);


    GError			*err=NULL;
	gchar			*estado,*texto;
	gboolean		resultado;
	GtkTextBuffer	*buffer;
	GtkTextIter		inicio;


	while (gtk_events_pending()) gtk_main_iteration();
	/*desabilitamos el area de texto y obtenemos el contenido del buffer de text*/
	gtk_widget_set_sensitive(obj->textview,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(obj->textview));
	gtk_text_buffer_get_start_iter (buffer,&inicio);
	gtk_text_buffer_get_end_iter (buffer,&fin);
	texto = gtk_text_buffer_get_text (buffer,&inicio,&fin,FALSE);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textview,TRUE);

    gchar *dir;
    dir = g_strdup_printf("Codigo.ced");

	/*pasamos el contenido del archivo al texto desde el buffer*/
		resultado = g_file_set_contents (dir,texto,-1,&err);


	if (resultado == FALSE)
	{
		/*error guardando el archivo */
		error (err->message);
		g_error_free (err);
	}
    CreaComilla();
    errores = NULL;
    sentencias = NULL;

    variables = NULL;
    AnalisisCodigo();
    if(errores == NULL)
    {
        //Estado(errores);
        ConstruirC();
        ConstruirArchivoErrores();
    }
    else
    {
    	//Estado(errores);
    	ConstruirArchivoErrores();
	}

    fin = 1;
    ini = 1;

    CargarArchivoError(constructor, obj);
    EstadoArch(obj);


}


void EjecutarCompilar(GtkWidget *constructor, Win2 * obj)
{
  //  EstadoBarra(obj, 3);


    GError			*err=NULL;
	gchar			*estado,*texto;
	gboolean		resultado;
	GtkTextBuffer	*buffer;
	GtkTextIter		inicio;


	while (gtk_events_pending()) gtk_main_iteration();
	/*desabilitamos el area de texto y obtenemos el contenido del buffer de text*/
	gtk_widget_set_sensitive(obj->textview,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(obj->textview));
	gtk_text_buffer_get_start_iter (buffer,&inicio);
	gtk_text_buffer_get_end_iter (buffer,&fin);
	texto = gtk_text_buffer_get_text (buffer,&inicio,&fin,FALSE);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textview,TRUE);

    gchar *dir;
    dir = g_strdup_printf("Codigo.ced");

	/*pasamos el contenido del archivo al texto desde el buffer*/
		resultado = g_file_set_contents (dir,texto,-1,&err);


	if (resultado == FALSE)
	{
		/*error guardando el archivo */
		error (err->message);
		g_error_free (err);
	}
    CreaComilla();
    errores = NULL;
    sentencias = NULL;

    variables = NULL;
    AnalisisCodigo();
    if(errores == NULL)
    {
        //Estado(errores);
        ConstruirArchivoErrores();
    }
    else
    {
    	//Estado(errores);
    	ConstruirArchivoErrores();
	}

    fin = 1;
    ini = 1;

    errores = NULL;

    CargarArchivoError(constructor, obj);



}

void CopiarCodigoC(GtkWidget *constructor, GtkTextBuffer * buffer)
{


    GtkClipboard * clicpboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_text_buffer_copy_clipboard(buffer, clicpboard);

}

void VentanaCodigoC()
{
    GtkBuilder  *builder;
    GError      *err=NULL;
    /* Localidad de memoria utilizada para la estructura Tutorial */

    /* Creación del constructor (builder) para después crear los widgets a partir del archivo XML
    * creado desde glade */
    builder = gtk_builder_new();
    if ( gtk_builder_add_from_file (builder, FILECODIGOC, &err) == 0 )
    {
        error_message (err->message);
        g_error_free (err);
        return (1);
    }
    /* Obtener las referencias a los Widgets para almacenarlos en la estructura y poder manipularlos */
    /* Primero obtenemos la referencia a la ventana principal */

    GtkWidget * ventana;
    GtkWidget * salir;
    GtkTextBuffer * buffer;
    GtkWidget * texto;
    GtkWidget * copiar;
    ventana = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    salir = GTK_WIDGET (gtk_builder_get_object (builder, "cerrar"));
    texto = GTK_WIDGET (gtk_builder_get_object (builder, "textview"));
    copiar = GTK_WIDGET (gtk_builder_get_object (builder, "copiar"));

    gtk_window_set_modal(GTK_WINDOW(ventana), TRUE);



    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
    gtk_widget_set_sensitive (texto,TRUE);
    g_signal_connect_swapped(G_OBJECT(salir), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) ventana);
    g_signal_connect(G_OBJECT(copiar), "clicked", G_CALLBACK(CopiarCodigoC), buffer);


    //*****

    GError			*erra=NULL;
    gchar			*estadoa,*textoa;
	gboolean		resultadoa;
    gchar *dira;
    dira = g_strdup_printf("Codigo.c");


	while (gtk_events_pending()) gtk_main_iteration();

	/*rescatamos el contenido del archivo.*/
	resultadoa = g_file_get_contents (dira, &textoa, NULL, &erra);

	if (resultadoa == FALSE)
	{
		/*manejo de errores*/
		error (erra->message);
		g_error_free (erra);
		g_free (dira);

	}
	/*dejamos en blanco el textarea*/

	gtk_widget_set_sensitive (texto,FALSE);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (texto));
    gtk_text_buffer_set_text(buffer,textoa,-1);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (texto,TRUE);
	g_free (textoa);



    gtk_widget_show_all(ventana);

}

void EjecutarCompilarYEjecutar(GtkWidget *constructor, Win2 * obj)
{
    EstadoBarra(obj, 5);
    EstadoBarra(obj, 3);

    GError			*err=NULL;
	gchar			*estado,*texto;
	gboolean		resultado;
	GtkTextBuffer	*buffer;
	GtkTextIter		inicio;


	while (gtk_events_pending()) gtk_main_iteration();
	/*desabilitamos el area de texto y obtenemos el contenido del buffer de text*/
	gtk_widget_set_sensitive(obj->textview,FALSE);
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(obj->textview));
	gtk_text_buffer_get_start_iter (buffer,&inicio);
	gtk_text_buffer_get_end_iter (buffer,&fin);
	texto = gtk_text_buffer_get_text (buffer,&inicio,&fin,FALSE);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (obj->textview,TRUE);

    gchar *dir;
    dir = g_strdup_printf("Codigo.ced");

	/*pasamos el contenido del archivo al texto desde el buffer*/
		resultado = g_file_set_contents (dir,texto,-1,&err);


	if (resultado == FALSE)
	{
		/*error guardando el archivo */
		error (err->message);
		g_error_free (err);
	}
    CreaComilla();
    errores = NULL;
    sentencias = NULL;

    variables = NULL;
    AnalisisCodigo();
    if(errores == NULL)
    {
        //Estado(errores);
        ConstruirC();
        ConstruirArchivoConstructor();
        CargarArchivoError(constructor, obj);
    }
    else
    {
    	//Estado(errores);
    	ConstruirArchivoErrores();
    	CargarArchivoError(constructor, obj);
    	return;
	}

    fin = 1;
    ini = 1;


    VentanaCodigoC();

    errores = NULL;


    EstadoArch(obj);

}
void AyudaAcercaDe(GtkWidget *constructor, Win2 *obj)
{
    GtkBuilder  *builder;
    GError      *err=NULL;
    /* Localidad de memoria utilizada para la estructura Tutorial */

    /* Creación del constructor (builder) para después crear los widgets a partir del archivo XML
    * creado desde glade */
    builder = gtk_builder_new();
    if ( gtk_builder_add_from_file (builder, FILEACERCADE, &err) == 0 )
    {
        error_message (err->message);
        g_error_free (err);
        return (1);
    }
    /* Obtener las referencias a los Widgets para almacenarlos en la estructura y poder manipularlos */
    /* Primero obtenemos la referencia a la ventana principal */

    GtkWidget * ventana;
    GtkWidget * salir;
    ventana = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    salir = GTK_WIDGET (gtk_builder_get_object (builder, "cerrar"));

    gtk_window_set_modal(GTK_WINDOW(ventana), TRUE);

    g_signal_connect_swapped(G_OBJECT(salir), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) ventana);

    gtk_widget_show_all(ventana);

}

void AyudaInformacion()
{

    GtkBuilder  *builder;
    GError      *err=NULL;
    /* Localidad de memoria utilizada para la estructura Tutorial */

    /* Creación del constructor (builder) para después crear los widgets a partir del archivo XML
    * creado desde glade */
    builder = gtk_builder_new();
    if ( gtk_builder_add_from_file (builder, FILEAYUDA, &err) == 0 )
    {
        error_message (err->message);
        g_error_free (err);
        return (1);
    }
    /* Obtener las referencias a los Widgets para almacenarlos en la estructura y poder manipularlos */
    /* Primero obtenemos la referencia a la ventana principal */

    GtkWidget * ventana;
    GtkWidget * salir;
    GtkTextBuffer * buffer;
    GtkWidget * texto;
    GtkWidget * acercade;
    ventana = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    salir = GTK_WIDGET (gtk_builder_get_object (builder, "cerrar"));
    texto = GTK_WIDGET (gtk_builder_get_object (builder, "textview"));
    acercade = GTK_WIDGET (gtk_builder_get_object (builder, "acercade"));

    gtk_window_set_modal(GTK_WINDOW(ventana), TRUE);



    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(texto));
    gtk_widget_set_sensitive (texto,FALSE);
    g_signal_connect_swapped(G_OBJECT(salir), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) ventana);
    g_signal_connect_swapped(G_OBJECT(acercade), "clicked", G_CALLBACK(AyudaAcercaDe), (gpointer) ventana);


    //*****

    GError			*erra=NULL;
    gchar			*estadoa,*textoa;
	gboolean		resultadoa;
    gchar *dira;
    dira = g_strdup_printf("AyudaInformacion.txt");


	while (gtk_events_pending()) gtk_main_iteration();

	/*rescatamos el contenido del archivo.*/
	resultadoa = g_file_get_contents (dira, &textoa, NULL, &erra);

	if (resultadoa == FALSE)
	{
		/*manejo de errores*/
		error (erra->message);
		g_error_free (erra);
		g_free (dira);

	}
	/*dejamos en blanco el textarea*/

	gtk_widget_set_sensitive (texto,FALSE);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (texto));
    gtk_text_buffer_set_text(buffer,textoa,-1);
	gtk_text_buffer_set_modified (buffer,FALSE);
	gtk_widget_set_sensitive (texto,FALSE);
	g_free (textoa);

    gtk_widget_show_all(ventana);


}

void PresionBoton1(GtkWidget *widget, Win1 *demo)
{
    gtk_main_quit ();

}
void PresionBoton2(GtkWidget *constructor, Win1 * obj)
{
   gtk_main_quit();

}

void error_message (const gchar *message)
{
    GtkWidget               *dialog;

    /* Muestra el mensaje en la ventana (terminal) de comandos */
    g_warning ("%s",message);

/* Crea un dialogo de mensaje de error y lo despliega de forma modal */
    dialog = gtk_message_dialog_new (NULL,
    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_ERROR,
    GTK_BUTTONS_OK,
    "%s",message);

    gtk_window_set_title (GTK_WINDOW (dialog), "Error!");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

int Bienvenida(int argc, char *argv[])
{

    /* Declaramos las variables a utilizar */
    Win1       *obj1;
    GtkBuilder  *builder;
    GError      *err=NULL;
    /* Localidad de memoria utilizada para la estructura Tutorial */

    obj1 = g_slice_new (Win1);

    gtk_init (&argc, &argv);

    /* Creación del constructor (builder) para después crear los widgets a partir del archivo XML
    * creado desde glade */
    builder = gtk_builder_new ();
    if ( gtk_builder_add_from_file (builder, FILEGLADE, &err) == 0 )
    {
        error_message (err->message);
        g_error_free (err);
        return (1);
    }
    /* Obtener las referencias a los Widgets para almacenarlos en la estructura y poder manipularlos */
    /* Primero obtenemos la referencia a la ventana principal */
    obj1->window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    /* Primero obtenemos la referencia a los botones */

    //obj1->boton1 = GTK_WIDGET (gtk_builder_get_object (builder, "salir"));
    obj1->boton2 = GTK_WIDGET (gtk_builder_get_object (builder, "continuar"));

    /* Se conectan las señales generadas por los objetos desde el builder con los widgets de la estructura */
    gtk_builder_connect_signals (builder, obj1);

    /* Liberar memoria utilizada por el objeto GtkBuilder */
    g_object_unref (G_OBJECT (builder));
    gtk_widget_show (obj1->window);

    /* La otra forma de conectar a las señales directamente desde el código sin utilizar glade */

    g_signal_connect_swapped(G_OBJECT(obj1->boton2), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) obj1->window);
    //Pulsar X
    g_signal_connect(G_OBJECT(obj1->window), "destroy", G_CALLBACK(PresionBoton2), (gpointer) obj1);

    gtk_main ();

    /* Libera la memoria reservada para la estructura Demo1 */
    //g_slice_free (Win1, obj1);

    return 0;
}

void EstadoLinea(GtkWidget *constructor, Win2 * obj)
{
    gchar * msg;
    gint row, col;
    GtkTextIter iter;
    gboolean bResult;

    bResult = gtk_text_buffer_get_has_selection(obj->buffer);


    //Donde esta el cursor
    gtk_text_buffer_get_iter_at_mark(obj->buffer, &iter, gtk_text_buffer_get_mark(obj->buffer, "insert"));
    if(bResult == TRUE)
    {
        gtk_widget_set_sensitive (obj->editarcopiar,TRUE);
        gtk_widget_set_sensitive (obj->editarcortar,TRUE);
    }
    else
    {
        gtk_widget_set_sensitive (obj->editarcopiar,FALSE);
        gtk_widget_set_sensitive (obj->editarcortar,FALSE);
    }

    //get Fila y Columna
    row = gtk_text_iter_get_line(&iter);
    col = gtk_text_iter_get_line_offset(&iter);

    //Construccion del mensaje
    msg = g_strdup_printf("Linea: %d.  -  Columna: %d.", row+1, col+1);


	//gtk_statusbar_pop (GTK_STATUSBAR (obj->barralineas),obj->barralineas_context_ido_context_id);
	gtk_statusbar_push(GTK_STATUSBAR (obj->barralineas),obj->barralineas_context_id,msg);
	g_free(msg);


    gtk_widget_set_sensitive (obj->archivoguardar,TRUE);
    gtk_widget_set_sensitive (obj->guardar,TRUE);
    gtk_widget_set_sensitive (obj->archivoguardarcomo,TRUE);
    gtk_widget_set_sensitive (obj->guardarcomo,TRUE);

}

void EstadoBarra (Win2 * obj, int estado) {
	gchar * comentario;

	if (estado == 0)
	{
		comentario = g_strdup("Esto es Lenguaje ED.");
	}
	else if (estado == 1)
	{
        comentario = g_strdup("Abriendo un archivo.");
	}
	else if(estado == 2)
	{
        comentario = g_strdup("Guardando un archivo.");
	}
	else if(estado == 3)
	{
        comentario = g_strdup("Compilando...");
	}
	else if(estado == 4)
	{
        comentario = g_strdup("Ejecutanto...");
	}
	else if(estado == 5)
	{
        comentario = g_strdup("Compilando y ejecutando...");
	}

	//comentario = g_strdup_printf ("Archivo: %s", archivo);

	gtk_statusbar_pop (GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id);
	gtk_statusbar_push(GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id,comentario);
	g_free(comentario);
}
void EstadoEditar(Win2 * obj)
{
    gtk_widget_set_sensitive (obj->archivoguardar,TRUE);
    gtk_widget_set_sensitive (obj->guardar,TRUE);
}

void EstadoArch (Win2 * obj) {
	gchar * archivo;
	gchar * estado;
	gchar * titulo;

	if (obj->nombreArchivo == NULL)
	{
		archivo = g_strdup("Sin titulo.");
	}
	else
	{
		archivo = g_path_get_basename (obj->nombreArchivo);
		gtk_widget_set_sensitive (obj->archivoguardar,TRUE);
        gtk_widget_set_sensitive (obj->archivoguardarcomo,TRUE);
        gtk_widget_set_sensitive (obj->guardar,TRUE);
        gtk_widget_set_sensitive (obj->ejecutarcompilar,TRUE);
        gtk_widget_set_sensitive (obj->ejecutarcompilaryejecutar,TRUE);
        gtk_widget_set_sensitive (obj->ejecutarejecutar,TRUE);
        gtk_widget_set_sensitive (obj->editarbuscar,TRUE);
        gtk_widget_set_sensitive (obj->editareliminar,TRUE);
        gtk_widget_set_sensitive (obj->ejecutar,TRUE);
	}
	estado = g_strdup_printf ("Archivo: %s", archivo);
	titulo = g_strdup_printf ("%s - [Lenguaje ED] - 1.0", archivo);

	gtk_statusbar_pop (GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id);
	gtk_statusbar_push(GTK_STATUSBAR (obj->barraestado),obj->barraestado_context_id,estado);
	gtk_window_set_title(GTK_WINDOW(obj->window), titulo);
	g_free(estado);
	g_free(archivo);

}

int EditorED()
{
/* Declaramos las variables a utilizar */

    GtkBuilder  *builder;
    GError      *err=NULL;
    /* Localidad de memoria utilizada para la estructura Tutorial */

    obj1 = g_slice_new (Win2);


    //gtk_init (&argc, &argv);

    /* Creación del constructor (builder) para después crear los widgets a partir del archivo XML
    * creado desde glade */
    builder = gtk_builder_new ();
    if ( gtk_builder_add_from_file (builder, FILEEDITOR, &err) == 0 )
    {
        error_message (err->message);
        g_error_free (err);
        return (1);
    }
    /* Obtener las referencias a los Widgets para almacenarlos en la estructura y poder manipularlos */
    /* Primero obtenemos la referencia a la ventana principal */
    obj1->window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    //Menu:

    obj1->archivoabrir = GTK_WIDGET (gtk_builder_get_object (builder, "archivoabrir"));
    obj1->archivoguardar = GTK_WIDGET (gtk_builder_get_object (builder, "archivoguardar"));
    obj1->archivoguardarcomo = GTK_WIDGET (gtk_builder_get_object (builder, "archivoguardarcomo"));
    obj1->archivonuevo = GTK_WIDGET (gtk_builder_get_object (builder, "archivonuevo"));
    obj1->archivosalir = GTK_WIDGET (gtk_builder_get_object (builder, "archivosalir"));

    obj1->editarcopiar = GTK_WIDGET (gtk_builder_get_object (builder, "editarcopiar"));
    obj1->editarpegar = GTK_WIDGET (gtk_builder_get_object (builder, "editarpegar"));
    obj1->editarcortar = GTK_WIDGET (gtk_builder_get_object (builder, "editarcortar"));
    obj1->editarbuscar = GTK_WIDGET (gtk_builder_get_object (builder, "editarbuscar"));
    obj1->editarreemplazar = GTK_WIDGET (gtk_builder_get_object (builder, "editarreemplazar"));
    obj1->editareliminar = GTK_WIDGET (gtk_builder_get_object (builder, "editareliminar"));

    obj1->ejecutarcompilar = GTK_WIDGET (gtk_builder_get_object (builder, "ejecutarcompliar"));
    obj1->ejecutarejecutar = GTK_WIDGET (gtk_builder_get_object (builder, "ejecutarejecutar"));
    obj1->ejecutarcompilaryejecutar = GTK_WIDGET (gtk_builder_get_object (builder, "ejecutarcompilaryejecutar"));

    obj1->ayudainformacion = GTK_WIDGET (gtk_builder_get_object (builder, "ayudainformacion"));
    obj1->ayudaacercade = GTK_WIDGET (gtk_builder_get_object (builder, "ayudaacercade"));
    //Botones:
    obj1->nuevo = GTK_WIDGET (gtk_builder_get_object (builder, "nuevo"));
    obj1->abrir = GTK_WIDGET (gtk_builder_get_object (builder, "abrir"));
    obj1->guardar = GTK_WIDGET (gtk_builder_get_object (builder, "guardar"));
    obj1->guardarcomo = GTK_WIDGET (gtk_builder_get_object (builder, "guardarcomo"));

    obj1->copiar = GTK_WIDGET (gtk_builder_get_object (builder, "copiar"));
    obj1->pegar = GTK_WIDGET (gtk_builder_get_object (builder, "pegar"));
    obj1->cortar = GTK_WIDGET (gtk_builder_get_object (builder, "cortar"));
    obj1->buscar = GTK_WIDGET (gtk_builder_get_object (builder, "buscar"));
    obj1->limpiar = GTK_WIDGET (gtk_builder_get_object (builder, "limpiar"));
    obj1->reemplazar = GTK_WIDGET (gtk_builder_get_object (builder, "reemplazar"));

    obj1->compilar = GTK_WIDGET (gtk_builder_get_object (builder, "compilar"));
    obj1->ejecutar = GTK_WIDGET (gtk_builder_get_object (builder, "ejecutar"));

    obj1->ayuda = GTK_WIDGET (gtk_builder_get_object (builder, "ayuda"));
    obj1->cerrar = GTK_WIDGET (gtk_builder_get_object (builder, "cerrar"));

    //Statusbar

    obj1->barraestado = GTK_WIDGET (gtk_builder_get_object (builder, "statusbar"));
    obj1->barralineas = GTK_WIDGET (gtk_builder_get_object (builder, "statusbarlineas"));

    //Caja grande de texto

    obj1->textview = GTK_WIDGET (gtk_builder_get_object (builder, "textview1"));
    obj1->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(obj1->textview));
    obj1->scrolled_win = GTK_WIDGET (gtk_builder_get_object (builder, "scrolled"));


    gtk_text_buffer_create_tag(obj1->buffer, "green_bg", "background", "lightgreen", NULL);
    g_signal_connect(G_OBJECT(obj1->window), "key-press-event", G_CALLBACK(key_pressed), obj1->buffer);


    //Caja del compilador

    gtk_widget_show_all(obj1->window);
    obj1->textviewc = GTK_WIDGET (gtk_builder_get_object (builder, "textviewc"));
    obj1->bufferc = gtk_text_view_get_buffer(GTK_TEXT_VIEW(obj1->textviewc));
    obj1->scrolled_winc = GTK_WIDGET (gtk_builder_get_object (builder, "scrolledc"));


    /* Primero obtenemos la referencia a los botones */

    /* Se conectan las señales generadas por los objetos desde el builder con los widgets de la estructura */
    gtk_builder_connect_signals (builder, obj1);

    /* Liberar memoria utilizada por el objeto GtkBuilder */
    g_object_unref (G_OBJECT (builder));
    gtk_widget_show (obj1->window);

    //SENALES Y ACCIONES

    //Cambio de lineas y columnas en el texto
    //g_signal_connect(obj1->textview, "select-all", G_CALLBACK(EstadoEditar), obj1);

    //texto inactivo
    gtk_widget_set_sensitive (obj1->textview,FALSE);
    gtk_widget_set_sensitive (obj1->textviewc,FALSE);



    g_signal_connect(G_OBJECT(obj1->archivoabrir), "activate", G_CALLBACK(ArchivoAbrir), obj1);
    g_signal_connect(G_OBJECT(obj1->archivoguardar), "activate", G_CALLBACK(ArchivoGuardar), obj1);
    g_signal_connect(G_OBJECT(obj1->archivonuevo), "activate", G_CALLBACK(ArchivoNuevo), obj1);
    g_signal_connect(G_OBJECT(obj1->archivoguardarcomo), "activate", G_CALLBACK(ArchivoGuardarComo), obj1);
    g_signal_connect(G_OBJECT(obj1->archivosalir), "activate", G_CALLBACK(ArchivoSalir), obj1);



    g_signal_connect(G_OBJECT(obj1->editarcopiar), "activate", G_CALLBACK(EditarCopiar), obj1);
    g_signal_connect(G_OBJECT(obj1->editarpegar), "activate", G_CALLBACK(EditarPegar), obj1);
    g_signal_connect(G_OBJECT(obj1->editarcortar), "activate", G_CALLBACK(EditarCortar), obj1);
    g_signal_connect(G_OBJECT(obj1->editarbuscar), "activate", G_CALLBACK(EditarBuscar), obj1);
    g_signal_connect(G_OBJECT(obj1->editarreemplazar), "activate", G_CALLBACK(EditarReemplazar), obj1);
    g_signal_connect(G_OBJECT(obj1->editareliminar), "activate", G_CALLBACK(EditarEliminar), obj1);

    g_signal_connect(G_OBJECT(obj1->ejecutarcompilar), "activate", G_CALLBACK(EjecutarCompilar), obj1);
    g_signal_connect(G_OBJECT(obj1->ejecutarejecutar), "activate", G_CALLBACK(EjecutarCompilarYEjecutar), obj1);
    g_signal_connect(G_OBJECT(obj1->ejecutarcompilaryejecutar), "activate", G_CALLBACK(EjecutarCompilarYEjecutar), obj1);

    g_signal_connect(G_OBJECT(obj1->ayudainformacion), "activate", G_CALLBACK(AyudaInformacion), obj1);
    g_signal_connect(G_OBJECT(obj1->ayudaacercade), "activate", G_CALLBACK(AyudaAcercaDe), obj1);


    g_signal_connect(obj1->nuevo, "clicked", G_CALLBACK(ArchivoNuevo), obj1);
    g_signal_connect(obj1->abrir, "clicked", G_CALLBACK(ArchivoAbrir), obj1);
    g_signal_connect(obj1->guardar, "clicked", G_CALLBACK(ArchivoGuardar), obj1);
    g_signal_connect(obj1->guardarcomo, "clicked", G_CALLBACK(ArchivoGuardarComo), obj1);

    g_signal_connect(obj1->ejecutar, "clicked", G_CALLBACK(EjecutarCompilarYEjecutar), obj1);
    g_signal_connect(obj1->compilar, "clicked", G_CALLBACK(EjecutarCompilar), obj1);

    g_signal_connect(obj1->copiar, "clicked", G_CALLBACK(EditarCopiar), obj1);
    g_signal_connect(obj1->pegar, "clicked", G_CALLBACK(EditarPegar), obj1);
    g_signal_connect(obj1->cortar, "clicked", G_CALLBACK(EditarCortar), obj1);
    g_signal_connect(obj1->buscar, "clicked", G_CALLBACK(EditarBuscar), obj1);
    g_signal_connect(obj1->reemplazar, "clicked", G_CALLBACK(EditarReemplazar), obj1);
    g_signal_connect(obj1->limpiar, "clicked", G_CALLBACK(EditarEliminar), obj1);

    g_signal_connect(obj1->ayuda, "clicked", G_CALLBACK(AyudaInformacion), obj1);
    g_signal_connect(obj1->cerrar, "clicked", G_CALLBACK(ArchivoSalir), obj1);

    gtk_window_set_title(GTK_WINDOW(obj1->window), "[Lenguaje ED] - 1.0");
    EstadoArch(obj1);
    EstadoBarra(obj1, 0);
    obj1->nombreArchivo = NULL;

    //Pulsar X
    g_signal_connect(G_OBJECT(obj1->window), "destroy", G_CALLBACK(ArchivoSalir), (gpointer) obj1);


    //Busquedas

    gtk_widget_add_events(obj1->textview, GDK_BUTTON_PRESS_MASK);

    //g_signal_connect_object(obj1->buffer, "mark_set", G_CALLBACK(EstadoLinea), obj1, G_CONNECT_AFTER);
    g_signal_connect(obj1->buffer, "changed", G_CALLBACK(EstadoLinea), obj1);

    //Archivo no creado -- opciones inactivas

    gtk_widget_set_sensitive (obj1->archivoguardar,FALSE);
    gtk_widget_set_sensitive (obj1->archivoguardarcomo,FALSE);

    gtk_widget_set_sensitive (obj1->ejecutarcompilar,FALSE);
    gtk_widget_set_sensitive (obj1->ejecutarcompilaryejecutar,FALSE);
    gtk_widget_set_sensitive (obj1->ejecutarejecutar,FALSE);

    gtk_widget_set_sensitive (obj1->editarbuscar,FALSE);
    gtk_widget_set_sensitive (obj1->editarreemplazar,FALSE);
    gtk_widget_set_sensitive (obj1->editareliminar,FALSE);
    gtk_widget_set_sensitive (obj1->editarcopiar,FALSE);
    gtk_widget_set_sensitive (obj1->editarcortar,FALSE);
    gtk_widget_set_sensitive (obj1->editarpegar,FALSE);

    gtk_widget_set_sensitive (obj1->guardar,FALSE);
    gtk_widget_set_sensitive (obj1->guardarcomo,FALSE);
    gtk_widget_set_sensitive (obj1->compilar,FALSE);
    gtk_widget_set_sensitive (obj1->ejecutar,FALSE);
    gtk_widget_set_sensitive (obj1->copiar,FALSE);
    gtk_widget_set_sensitive (obj1->cortar,FALSE);
    gtk_widget_set_sensitive (obj1->pegar,FALSE);
    gtk_widget_set_sensitive (obj1->buscar,FALSE);
    gtk_widget_set_sensitive (obj1->limpiar,FALSE);
    gtk_widget_set_sensitive (obj1->reemplazar,FALSE);
    gtk_widget_set_sensitive (obj1->ayuda,TRUE);
    //Cambios en el archivo guardado

    gtk_main ();

    /* Libera la memoria reservada para la estructura Demo1 */
    g_slice_free (Win2, obj1);
    gtk_main_quit ();

    return 0;
}


int main (int argc, char *argv[])
{

    CreaComilla();
    Bienvenida(argc, argv);
    EditorED();
    return 0;

}
