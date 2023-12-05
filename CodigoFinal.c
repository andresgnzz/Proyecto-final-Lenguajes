#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef char cadena[30];

typedef struct
{
    cadena nombre;
    long atr;
    long sig;
    long data;
}Entidad;

typedef struct
{
    cadena nombre;
    int tipo;
    char iskp;
    long sig;
    int tam;
    cadena descripcion;
}Atributo;

void menuPrincipal();
void nuevoDiccionario(FILE *f);
void abrirDiccionario(FILE *f);
void cerrarDiccionario(FILE *f);

//Entidades

void menuEntidades(FILE *f);
void altaEntidad(FILE *f);
Entidad capturaEntidad();
Entidad leeEntidad(FILE  *f,long dir);
long buscaEntidad(FILE *f, cadena entNom);
long escribeEntidad(FILE *f, Entidad ent);
void escribeCabEntidades(FILE *f);
long getCabEntidades(FILE *f);
void insertaEntidad(FILE *f,Entidad nueva, long dir);
void reescribeEntidad(FILE *f, Entidad ent, long dir); //Referencia
void consultaEntidad(FILE *f);
void bajaEntidad(FILE *f);
long eliminaEntidad(FILE *f, cadena nomb);
void modificaEntidad(FILE *f);
void rescribeCabEntidades(FILE *f,long dir);
void pideNomEnt(cadena nombEnt);

//Atributos

void menuAtributos(FILE *f, Entidad entAct, long direntAct);
int opcAtr();
long seleccionaTabla(FILE *f, Entidad *entAct, long *direntAct);
void altaAtributo(FILE *f, Entidad *entAct, long direntAct);
Atributo capturaAtributo();
long buscaAtributo(FILE *f, cadena atrNom, Entidad entAct);
long escribeAtributo(FILE *f, Atributo atr);
void insertaAtributo(FILE *f, Atributo atr, long dir, Entidad *entAct, long direntAct);
Atributo leeAtributo(FILE *f, long dir);
void reescribeAtributo(FILE *f, Atributo atr, long dir);
void bajaAtributo(FILE *f, Entidad *entAct, long direntAct);
long eliminaAtributo(FILE *f, cadena atrNom, Entidad *entAct, long direntAct);
void modificaAtributo(FILE *f, Entidad *entAct, long direntAct);
void consultaAtributo(FILE *f, Entidad entAct);
void pideNombAtr(cadena nombAtr);

//Bloques

int opcBlq();
void menuBloques(FILE *f, Entidad entAct, long direntAct, Atributo *arrAtr, int nAtr, void* b, long tamBloque);
void consultaBloque(FILE *f, Entidad entAct, Atributo *arrAtr, int nAtr, void* b, long tamBloque);
bool existeISKP(FILE *f, Entidad entAct);
long cargaAtributos(FILE *f, Entidad entAct, Atributo *arrAtr, int *nAtr);
double comparaBloques(Atributo *arrAtr, void* b1, void* b2);
void* capturaBloque(Atributo *arrAtr, long tamBloque, int nAtr);
void* capturaBloqueClave(Atributo *arrAtr, long tamBloque, int nAtr);
void insertaBloque(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, void* b, long tamBloque, long dir);
void altaSecuencial(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, int nAtr, long tamBloque);
long eliminaBloque(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, void* b, long tamBloque);
void bajaSecuencial(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, int nAtr, long tamBloque);
void* leeBloque(FILE *f, long dir, long tamBloque);
long escribeBloque(FILE *f, void* b, long tamBloque);
long buscaBloque(FILE *f, Entidad entAct, Atributo *arrAtr, void* b, long tamBloque);
void modificaBloque(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, int nAtr, long tamBloque);
void reescribeBloque(FILE *f, void* b, long dir, long tamBloque);
long existeBloqueDif(FILE *f, Entidad entAct, Atributo *arrAtr, void* b, void* bNuevo, long tamBloque);



int main()
{
    FILE *f = NULL;

    menuPrincipal(f);
    return 0;

}

//Menu Principal del diccionario de datos
void menuPrincipal(FILE *f)
{
    int opc = 0;

    printf("\n-------- MENU PRINCIPAL --------\n");
    printf("1. Nuevo Diccionario\n");
    printf("2. Abrir Diccionario\n");
    printf("3. Salir\n");
    printf("--------------------------------\n");

    printf("Seleccione una opcion:");
    scanf("%d", &opc);

    do
    {
        switch (opc)
        {
            case 1:
                nuevoDiccionario(f);
                break;
            case 2:
                abrirDiccionario(f);
                break;
            case 3:
                cerrarDiccionario(f);
                exit(-1);
            default:
                printf("Ingrese una de las opciones validas 1 - 3.\n");
        }
    } while (opc != 3);
}

//Funcion para crear un nuevo diccionario en forma wb+
void nuevoDiccionario(FILE *f)
{
    cadena nombArch;

    cerrarDiccionario(f);
    printf("Ingrese el Nombre del Diccionario:");
    scanf("%s", nombArch);

    f = fopen(nombArch, "rb+");

    if(f != NULL)
    {
        printf("Ya existe este diccionario.\n");
        fclose(f);
        menuPrincipal(f);
    }
    else
    {
        f = fopen(nombArch, "wb+");
        escribeCabEntidades(f);
        menuEntidades(f);
    }
}

//Funcion para abrir un nuevo diccionario en forma rb+
void abrirDiccionario(FILE *f)
{
    cadena nombArch;

    cerrarDiccionario(f);

    printf("Ingrese el nombre del Diccionario:");
    scanf("%s", nombArch);

    f = fopen(nombArch, "rb+");

    if(f != NULL)
    {
        printf("Se abrio correctamente el diccionario.\n");
        menuEntidades(f);
    }
    else
    {
        printf("El Diccionario no existe.\n");
        fclose(f);
        menuPrincipal(f);
    }
}

//Funcion para cerrar el archivo
void cerrarDiccionario(FILE *f)
{
    if(f != NULL)
        fclose(f);
}




//Funciones de Entidades

//Menu principal de las entidades, se mandan llamar todas las funciones de entidades, menu de aributos y menu bloques.
void menuEntidades(FILE *f)
{
    int opc, nAtr;
    Entidad entAct;
    long direntAct;
    Atributo arrAtr[50];
    long tamBloque;
    void* b;

    do {
        printf("\n-------- MENU ENTIDADES --------\n");
        printf("1. Nueva Entidad\n");
        printf("2. Consultar Entidades\n");
        printf("3. Eliminar Entidades\n");
        printf("4. Modificar Entidades\n");
        printf("5. Menu Atributos\n");
        printf("6. Menu Datos\n");
        printf("7. Menu Anterior\n");
        printf("--------------------------------\n");

        printf("Seleccione una Opcion:");
        scanf("%d", &opc);

        switch (opc)
        {
            case 1:
                altaEntidad(f);
                break;
            case 2:
                consultaEntidad(f);
                break;
            case 3:
                bajaEntidad(f);
                break;
            case 4:
                modificaEntidad(f);
                break;
            case 5:
                seleccionaTabla(f, &entAct, &direntAct);
                printf("\nApuntador atr: %ld", entAct.atr);
                printf("\nDireccion entAct: %ld", direntAct);
                menuAtributos(f, entAct, direntAct);
            case 6:
                seleccionaTabla(f, &entAct, &direntAct);
                if(existeISKP(f, entAct) == true)
                {
                    tamBloque = cargaAtributos(f, entAct, arrAtr, &nAtr);
                    menuBloques(f, entAct, direntAct, arrAtr, nAtr, b, tamBloque);
                }
                break;
            case 7:
                menuPrincipal(f);
            default:
                printf("Ingrese una de las opciones validas 1 - 7.\n");
        }
    } while (opc != 7);

}

//Inserta una entidad y verifica en que caso se va a insertar, al inicio sin datos, al inicio con datos en medio o al final.
void insertaEntidad(FILE *f, Entidad nueva, long dir)
{
    Entidad aux, entAnt;
    long cab, dirAnt;
    cab = getCabEntidades(f);

    if(cab == -1)   //caso 1
        rescribeCabEntidades(f, dir);
    else
    {
        aux = leeEntidad(f,cab);

        if(strcmp(nueva.nombre,aux.nombre) < 0)
        {
            nueva.sig = cab;
            rescribeCabEntidades(f, dir);
            reescribeEntidad(f, nueva, dir);
        }
        else
        {
            while(cab!=-1 && strcmp(nueva.nombre,aux.nombre) > 0)
            {
                entAnt = aux;
                dirAnt = cab;
                cab=aux.sig;
                if(cab != -1)
                    aux = leeEntidad(f,cab);
            }
            entAnt.sig = dir;
            reescribeEntidad(f,entAnt,dirAnt);
            if(cab != -1)
            {
                nueva.sig = cab;
                reescribeEntidad(f,nueva,dir);
            }
        }
    }
}

//Verifica si la funcion que deseamos insertar existe o no. Si no existe la inserta.
void altaEntidad(FILE *f)
{
    Entidad ent;
    long pos;

    ent = capturaEntidad();

    if(buscaEntidad(f, ent.nombre) == -1)
    {
        pos = escribeEntidad(f, ent);
        insertaEntidad(f, ent, pos);
        fseek(f, 0, SEEK_END);
    }
    else
        printf("La Entidad ya existe.\n");
}

//Se encarga de capturar el nombre de la entidad e iniciar sus apuntadores en -1.
Entidad capturaEntidad()
{
    Entidad ent;

    printf("Nombre de la Entidad:");
    scanf("%s", ent.nombre);

    ent.sig = ent.atr = ent.data = (long) - 1;

    return ent;
}

//Busca la entidad en la lista por su nombre y regresa la direcciion en la que se encuentra.
long buscaEntidad(FILE *f, cadena entNom)
{
    Entidad ent;
    long cab;

    cab = getCabEntidades(f);

    while(cab != -1)
    {
        ent = leeEntidad(f,cab);

        if(strcmp(entNom, ent.nombre) == 0)
            return cab;

        cab = ent.sig;
    }
    return -1;
}

//Escribe la entidad dentro del archivo en la posicion que indique ftell.
long escribeEntidad(FILE *f, Entidad ent)
{
    long dir;

    fseek(f, 0, SEEK_END);
    dir = ftell(f);
    fwrite(&ent, sizeof (Entidad), 1, f);

    return dir;
}

//Cuando se crea el archivo nuevo, esta funcion escribe un -1 al inicio del archivo.
void escribeCabEntidades(FILE *f)
{
    long cab = -1;

    fseek(f, 0, SEEK_SET);
    fwrite(&cab, sizeof (long), 1, f);
}

// Nos regresa en que posicion se encuentra la primera entidad del archivo.
long getCabEntidades(FILE *f)
{
    long cab;

    fseek(f,0,SEEK_SET);
    fread(&cab,sizeof (long),1,f);

    return cab;
}

//Lee la entidad del archivo dependiendo de la direccion que reciba.
Entidad leeEntidad(FILE  *f,long dir)
{
    Entidad ent;

    fseek(f,dir,SEEK_SET);
    fread(&ent,sizeof(Entidad),1,f);

    return ent;
}

//Sobreescribe una entidad en la smisma direccion con otra.
void reescribeEntidad(FILE *f,Entidad ent, long dir)
{
    fseek(f,dir,SEEK_SET);
    fwrite(&ent,sizeof(Entidad),1,f);
}

//Se encarga de imprimir todas las entiddes del archivo.
void consultaEntidad(FILE *f)
{
    Entidad  ent;
    long cab = getCabEntidades(f);

    printf("\n------------ENTIDADES------------\n");

    while (cab != -1)
    {
        ent = leeEntidad(f,cab);
        printf("%s\n", ent.nombre);
        cab = ent.sig;
    }

    printf("---------------------------------\n\n");
}

//Verifica si la entidad que queremos eliminar existe o no.
void bajaEntidad(FILE *f)
{
    long dir;
    cadena nombEnt;

    pideNomEnt(nombEnt);

    if(buscaEntidad(f, nombEnt) != -1)
    {
        dir = eliminaEntidad(f, nombEnt);
    }
    else
        printf("La entidad %s no existe.", nombEnt);
}

//Verifica donde esta la entidad que se quiere eliminar y se aplican cada caso correspondiente, al inicio, al medio o al final.
long eliminaEntidad(FILE *f, cadena nomb)
{
    long posEntAnt;
    Entidad entAnt;
    long cab = getCabEntidades(f);
    Entidad ent = leeEntidad(f,cab);

    if(strcmpi(ent.nombre, nomb) == 0)
    {
        rescribeCabEntidades(f, ent.sig);
        return cab;
    }
    else
    {
        while (cab != -1 && strcmpi(ent.nombre, nomb) < 0)
        {
            posEntAnt = cab;
            entAnt = ent;
            cab = ent.sig;

            if(cab != -1)
                ent = leeEntidad(f,cab);
        }

        if(strcmpi(ent.nombre, nomb) == 0)
        {
            entAnt.sig = ent.sig;
            reescribeEntidad(f, entAnt, posEntAnt);
        }
    }
    return cab;
}

//Se encarga de modificar la cabecera de las entidades en el archivo.
void rescribeCabEntidades(FILE *f,long dir)
{
    long cab = dir;
    fseek(f,0 , SEEK_SET);
    fwrite(&cab,sizeof(long),1,f);
}

//Nos sirve para modificar una entidad con otra si es que la que queremos modificar existe y la nueva no existe.
void modificaEntidad(FILE *f)
{
    cadena nombEnt, nombEntNueva;
    Entidad nuevaEnt;
    long dir;

    printf("\nEntidad que desea modificar\n");
    pideNomEnt(nombEnt);

    if(buscaEntidad(f, nombEnt) != -1)
    {
        printf("\nNueva Entidad\n");
        nuevaEnt = capturaEntidad();

        if(buscaEntidad(f, nuevaEnt.nombre) == -1)
        {
            dir = eliminaEntidad(f, nombEnt);
            strcpy(nombEnt, nombEntNueva);
            reescribeEntidad(f, nuevaEnt, dir);
            insertaEntidad(f, nuevaEnt, dir);
        }
        else
            printf("Error. La entidad que desea modificar ya existe.\n");
    }
    else
        printf("Error. La entidad que desea modificar no existe.\n");
}

//Unicamente regresa un nombre.
void pideNomEnt(cadena nombEnt)
{
    printf("Ingresa el nombre de la Entidad:");
    scanf("%s", nombEnt);
    printf("\n");
}




//Funciones de Atributos

//Menu principal de los atributos, nos da acceso a todas las funciones de estos.
void menuAtributos(FILE *f, Entidad entAct, long direntAct)
{
    int opc;

    do {
        opc=opcAtr();

        switch(opc)
        {
            case 1:
                altaAtributo(f,&entAct,direntAct);
                break;
            case 2:
                consultaAtributo(f,entAct);
                break;
            case 3:
                bajaAtributo(f,&entAct,direntAct);
                break;
            case 4:
                modificaAtributo(f,&entAct,direntAct);
                break;
            case 5:
                printf("\nRegresando a menu de entidades...\n ");
                menuEntidades(f);
                break;
            default:
                printf("Ingresa una opcion del 1 al 5 ");
        }
    }while(opc != 5);
}

//Seleccion de opcion para el menu de atributos.
int opcAtr()
{
    int op;

    printf("\n-------- MENU ATRIBUTOS --------\n");
    printf("1. Alta Atributo\n");
    printf("2. Consultar Atributo\n");
    printf("3. Eliminar Atributo\n");
    printf("4. Modificar Atributo\n");
    printf("5. Menu Anterior\n");
    printf("----------------------------------\n");

    scanf("%d", &op);

    return op;
}

//Con esta funcion indicamos con que entidad queremos trabajar y si existe o no.
long seleccionaTabla(FILE *f, Entidad *entAct, long *direntAct)
{
    printf("Con que Entidad deseas trabajar:");
    scanf("%s", entAct->nombre);

    *direntAct = buscaEntidad(f, entAct->nombre);

    if(*direntAct != -1)
    {
        *entAct = leeEntidad(f, *direntAct);
        return 1;
    }
    else
        printf("Error. La entidad no existe.\n");

    return -1;
}

//Funcion para capturar atributos.
Atributo capturaAtributo()
{
    Atributo nuevoAtributo;
    int tamCad;

    printf("Ingrese el nombre del atributo:");
    scanf("%s", nuevoAtributo.nombre);

    printf("Ingrese el tipo del atributo:\n 1. Cadena\n 2. Entero\n 3. Float\n 4. Double\n 5. Long\n");
    scanf("%d", &nuevoAtributo.tipo);

    switch (nuevoAtributo.tipo)
    {
        case 1:
            printf("Tamanio de la cadena:");
            scanf("%d", &tamCad);

            nuevoAtributo.tam = (tamCad * sizeof(char));
            break;
        case 2:
            nuevoAtributo.tam = (sizeof (int));
            break;
        case 3:
            nuevoAtributo.tam = (sizeof(float));
            break;
        case 4:
            nuevoAtributo.tam = (sizeof(double));
            break;
        case 5:
            nuevoAtributo.tam = (sizeof(long));
        default:
            printf("Tipo de dato no valido.\n");
    }

    printf("Iskp del atributo (clave unica):\n S. SI\n N. NO\n");
    scanf(" %c", &nuevoAtributo.iskp);

    printf("Ingrese la descripcion del atributo:");
    fflush(stdin);
    fgets(nuevoAtributo.descripcion, sizeof(cadena), stdin);

    nuevoAtributo.sig = -1;

    return nuevoAtributo;
}

//Se encarga de buscar un atributo dentro del archivo y si este existe o no.
long buscaAtributo(FILE *f, cadena atrNom, Entidad entAct)
{
    long cab;
    Atributo atr;

    cab = entAct.atr;

    while(cab != -1)
    {
        atr = leeAtributo(f, cab);

        if(strcmp(atr.nombre, atrNom) == 0)
            return cab;

        cab = atr.sig;
    }
    return -1;
}

//Sobreescribe un atributo en otro con la direccion dada.
void reescribeAtributo(FILE *f, Atributo atr, long dir)
{
    fseek(f,dir,SEEK_SET);
    fwrite(&atr,sizeof (atr),1,f);
}

//Lee un atributo en el archivo en una direccion dada.
Atributo leeAtributo(FILE *f, long dir)
{
    Atributo atr;

    fseek(f, dir, SEEK_SET);
    fread(&atr, sizeof(Atributo), 1, f);

    return atr;
}

//Escribe el atributo dentro del archivo dependiendo de la posicion.
long escribeAtributo(FILE *f, Atributo atr)
{
    long pos;

    fseek(f,0,SEEK_END);
    pos = ftell(f);
    fwrite(&atr,sizeof(atr),1,f);

    return pos;
}

//Verifica si el atributo que queremos insertar existe o no.
void altaAtributo(FILE *f, Entidad *entAct, long direntAct)
{
    Atributo nuevoAtr;
    long dirNuevo;

    nuevoAtr = capturaAtributo();

    if(buscaAtributo(f, nuevoAtr.nombre, *entAct) == -1)
    {
        dirNuevo = escribeAtributo(f, nuevoAtr);
        insertaAtributo(f, nuevoAtr, dirNuevo, entAct, direntAct);
    }
    else
        printf("Error. El Atributo que desea agregar ya existe.");
}

//Verifica en que caso se va a insertar el atributo, al inicio sin datos, al inicio con datos, al medio o al final.
void insertaAtributo(FILE *f, Atributo atr, long dir, Entidad *entAct, long direntAct)
{
    long cab, dirAnt;
    Atributo atrAux, atrAct, atrAnt;

    cab = entAct->atr;

    if(cab == -1)
    {
        entAct->atr = dir;
        reescribeEntidad(f, *entAct, direntAct);
    }
    else
    {
        atrAct = leeAtributo(f, cab);

        if(strcmpi(atr.nombre, atrAct.nombre) < 0)
        {
            atr.sig = cab;
            entAct->atr = dir;
            reescribeAtributo(f, atr, dir);
            reescribeEntidad(f, *entAct, direntAct);
        }
        else
        {
            while (cab != -1 && strcmpi(atr.nombre, atrAct.nombre) > 0)
            {
                atrAnt = atrAct;
                dirAnt = cab;
                cab = atrAct.sig;

                if(cab != -1)
                    atrAct = leeAtributo(f, cab);
            }
            if (cab != -1)
            {
                atr.sig = atrAnt.sig;
                reescribeAtributo(f, atr, dir);
            }
            // Inserta el nuevo atributo en la posición de manera ordenada
            atrAnt.sig = dir;
            reescribeAtributo(f, atrAnt, dirAnt);
        }
    }
}

//Verifica si el atributo que queremos eliminar existe o no.
void bajaAtributo(FILE *f, Entidad *entAct, long direntAct)
{
    //Localiza el atributo y si no existe marca un error
    cadena nomAtr;
    Atributo atr;
    long cab;

    printf("Nombre del Atributo a eliminar:");
    scanf("%s", nomAtr);

    if(buscaAtributo(f, nomAtr, *entAct) != -1)
    {
        eliminaAtributo(f, nomAtr, entAct, direntAct);
        printf("Se elimino correctamente el atributo.\n");
    }
    else
        printf("El atributo no existe.\n");

}

//Verifica en que caso se va a eliminar el atributo.
long eliminaAtributo(FILE *f, cadena atrNom, Entidad *entAct, long direntAct)
{
    long cab, cabAnt;
    Atributo atr, atrAnt;

    cab = entAct->atr;
    atr = leeAtributo(f, cab);

    if(strcmpi(atr.nombre, atrNom) == 0)
    {
        entAct->atr = atr.sig;
        reescribeEntidad(f, *entAct, direntAct);
    }
    else
    {
        while(cab != -1 && strcmpi(atr.nombre, atrNom) < 0)
        {
            cabAnt = cab;
            atrAnt = atr;
            cab = atr.sig;

            if (cab != -1)
                atr = leeAtributo(f, cab);
        }
        atrAnt.sig = atr.sig;
        reescribeAtributo(f, atrAnt, cabAnt);
    }
    return cab;
}

//Nos permite modificar un atributo y verifica si el que queremos modifcar existe y que el que queremos insertar no exista.
void modificaAtributo(FILE *f, Entidad *entAct, long direntAct)
{
    Atributo nuevoAtr;
    cadena nombAtr;
    long dirAtr;

    printf("\nNombre del atributo que desea modificar\n");
    pideNombAtr(nombAtr);

    if(buscaAtributo(f, nombAtr, *entAct) != -1)
    {
        printf("\nNuevo atributo\n");
        nuevoAtr = capturaAtributo();

        if(buscaAtributo(f, nuevoAtr.nombre, *entAct) == -1)
        {
            dirAtr = eliminaAtributo(f, nombAtr, entAct, direntAct);
            reescribeAtributo(f, nuevoAtr, dirAtr);
            insertaAtributo(f, nuevoAtr, dirAtr, entAct, direntAct);
        }
        else
            printf("Error. No se puede modificar el atributo porque YA existe el Atributo.\n");
    }
    else
        printf("Error. No se puede modificar el atributo porque NO existe.\n");
}

//Funcion que nos permite visualizar los atributos.
void consultaAtributo(FILE *f, Entidad entAct)
{
    Atributo atr;
    long cab = entAct.atr;

    printf("--------------------------------------------------- ATRIBUTOS ---------------------------------------------------\n");
    printf("Entidad activa: %s\n\n", entAct.nombre);

    printf("NOMBRE\t\t TIPO\t TAMANIO\t CLAVE PRIMARIA\t SIGUIENTE\t DESCRIPCION\n");

    while (cab != -1)
    {
        atr = leeAtributo(f, cab);
        printf("%s\t\t", atr.nombre);

        switch (atr.tipo)
        {
            case 1:
                printf("Cadena\t\t");
                break;
            case 2:
                printf("Entero\t\t");
                break;
            case 3:
                printf("Float\t\t");
                break;
            case 4:
                printf("Double\t\t");
                break;
            case 5:
                printf("Long\t\t");
                break;
        }

        printf("%d\t\t", atr.tam);
        printf("%c\t\t", atr.iskp);
        printf("%ld\t\t", atr.sig);
        printf("%s\n", atr.descripcion);

        cab = atr.sig;
    }
    printf("-----------------------------------------------------------------------------------------------------------------\n");
}

//Nos pide un nombre de un atributo.
void pideNombAtr(cadena nombAtr)
{
    printf("Ingresa el nombre del atributo:");
    scanf("%s", nombAtr);
    printf("\n");
}




//Funciones de Bloques

//Verifica si en los atributos existe una clave primaria
bool existeISKP(FILE *f, Entidad entAct)
{
    int cont = 0;
    long cab = entAct.atr;
    Atributo atr;

    while (cab != -1)
    {
        atr = leeAtributo(f, cab);

        if(atr.iskp == 'S')
            cont++;

        cab = atr.sig;
    }

    if(cont == 1)
        return true;
    else if(cont > 1)
        printf("\nError. Existe mas de una clave\n");
    else
        printf("\nError. No existe clave.\n");

    return false;
}

//Crea un arreglo de atributos e inserta en la posicion [0] la clave primaria, regresa su tamaño y el numero de atributos.
long cargaAtributos(FILE *f, Entidad entAct, Atributo *arrAtr, int *nAtr)
{
    long tamBloque = sizeof(long);
    long cab = entAct.atr;
    int cont = 1;
    Atributo atr;

    while(cab != -1)
    {
        atr = leeAtributo(f, cab);

        if(atr.iskp == 'S')
        {
            arrAtr[0] = atr;
            cont--;
        }

        if(atr.iskp == 'N')
            arrAtr[cont] = atr;

        cont++;

        cab = atr.sig;
        tamBloque += atr.tam;
    }

    *nAtr = cont;
    return tamBloque;
}

//Compara la clave primaria de dos bloque para verificar si son iguales o diferentes.
double comparaBloques(Atributo *arrAtr, void* b1, void* b2)
{
    switch (arrAtr[0].tipo)
    {
        case 1:
            return strcmpi((char *)(b1 + sizeof(long)), (char *)(b2 + sizeof(long)));
        case 2:
            return *((int *)(b1 + sizeof(long))) - *((int *)(b2 + sizeof(long)));
        case 3:
            return *((float *)(b1 + sizeof(long))) - *((float *)(b2 + sizeof(long)));
        case 4:
            return *((double *)(b1 + sizeof(long))) - *((double *)(b2 + sizeof(long)));
        case 5:
            return *((long *)(b1 + sizeof(long))) - *((long *)(b2 + sizeof(long)));
    }
}

//Unicamente soolo regresa la opcion del menu de bloques.
int opcBlq()
{
    int opc;

    printf("\n-------- MENU DATOS --------\n");
    printf("1. Nuevo Registro\n");
    printf("2. Consultar Registro\n");
    printf("3. Eliminar Registro\n");
    printf("4. Modificar Registro\n");
    printf("5. Menu Anterior\n");
    printf("----------------------------------\n");

    scanf("%d", &opc);

    return opc;
}

//Menu de bloques se encarga de mandar llamar todas las funciones de bloques, es accesible unicamente si existe una unica clave primaria dentro de los atributos.
void menuBloques(FILE *f, Entidad entAct, long direntAct, Atributo *arrAtr, int nAtr, void* b, long tamBloque)
{
    int opc;
    do {
        opc = opcBlq();

        switch(opc)
        {
            case 1:
                altaSecuencial(f, &entAct, direntAct, arrAtr, nAtr, tamBloque);
                break;
            case 2:
                consultaBloque(f, entAct, arrAtr, nAtr, b, tamBloque);
                break;
            case 3:
                bajaSecuencial(f, &entAct, direntAct, arrAtr, nAtr, tamBloque);
                break;
            case 4:
                //PENDIENTE
                modificaBloque(f,&entAct, direntAct, arrAtr, nAtr, tamBloque);
                break;
            case 5:
                printf("\nRegresando a menu de entidades...\n ");
                menuEntidades(f);
                break;
            default:
                printf("Ingresa una opcion del 1 al 5 ");
        }
    }while(opc != 5);
}

//Captura los datos del bloque de datos los cuales corresponden a los atributos.
void* capturaBloque(Atributo *arrAtr, long tamBloque, int nAtr)
{
    void* p = malloc(tamBloque);
    *((long*)p+0) = (long) - 1;
    long des = sizeof(long);
    int i = 0;

    while(i < nAtr)
    {
        printf("Ingrese el/la %s:", arrAtr[i].nombre);

        switch (arrAtr[i].tipo)
        {
            case 1:
            {
                char cad[500];
                fflush(stdin);
                scanf("%s", cad);
                cad[arrAtr[i].tam - 1] = '\0';
                strcpy((char *)(p+des),cad);
                break;
            }
            case 2:
            {
                int entero;
                scanf("%d", &entero);
                *((int*)(p+des)) = entero;
                break;
            }
            case 3:
            {
                float flotante;
                scanf("%f", &flotante);
                *((float*)(p+des)) = flotante;
                break;
            }
            case 4:
            {
                double doble;
                scanf("%lf", &doble);
                *(double*)(p+des) = doble;
                break;
            }
            case 5:
            {
                long largo;
                scanf("%ld", &largo);
                *((long*)(p+des)) = largo;
                break;
            }
        }
        des += arrAtr[i].tam;
        i++;
    }
    return p;
}

//Verifica si el bloque que queremos insertar no existe.
void altaSecuencial(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, int nAtr, long tamBloque)
{
    void *nuevo;
    long dirnuevo;

    nuevo = capturaBloque(arrAtr, tamBloque, nAtr);

    if(buscaBloque(f, *entAct, arrAtr, nuevo, tamBloque) == -1)
    {
        dirnuevo = escribeBloque(f, nuevo, tamBloque);
        insertaBloque(f, entAct, direntAct, arrAtr, nuevo, tamBloque, dirnuevo);
    }
    else
        printf("\nError. El bloque YA existe.\n");
}

//Verifica en donde va acomodado el dato dependiendo de su clave primaria.
void insertaBloque(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, void* b, long tamBloque, long dir)
{
    long cab = entAct->data;
    long dirAnt;
    void* bAct;
    void* bAnt;

    if(cab == -1)
    {
        entAct->data = dir;
        reescribeEntidad(f,*entAct,direntAct);
    }
    else
     {
        bAct = leeBloque(f, cab, tamBloque);

        if(comparaBloques(arrAtr, bAct,b) > 0)
        {
            *((long*)(b+0))=cab;
            reescribeBloque(f, b, dir, tamBloque);
            entAct->data = dir;
            reescribeEntidad(f,*entAct,direntAct);
            free(bAct);
        }
        else
        {
            while(cab != -1 && comparaBloques(arrAtr, bAct,b) < 0)
            {
                if(bAnt != NULL)
                    free(bAnt);

                bAnt = bAct;
                dirAnt = cab;
                cab = *((long*)(bAct+0));

                if(cab!=-1)
                    bAct = leeBloque(f, cab, tamBloque);
            }
            if(cab!=-1)
            {
                *((long*)(b+0)) = *((long*)(bAnt+0));
                reescribeBloque(f, b, dir, tamBloque);
            }

            *((long*)(bAnt+0)) = dir;
            reescribeBloque(f, bAnt, dirAnt, tamBloque);

            if(bAct != bAnt)
            {
                free(bAct);
                free(bAnt);
            }
        }
    }
}

//Lee el bloque dentro del archivo en la direccion y el tamaño del bloque que le pasemos
void* leeBloque(FILE *f, long dir, long tamBloque)
{
    void* b = malloc(tamBloque);

    fseek(f, dir, SEEK_SET);
    fread(b, tamBloque, 1, f);

    return b;
}

//Escribe el bloque en la posicion que regrese fell con el tamaño del bloque.
long escribeBloque(FILE *f, void* b, long tamBloque)
{
    long pos;

    fseek(f, 0, SEEK_END);
    pos = ftell(f);
    fwrite(b, tamBloque, 1, f);

    return pos;
}

//Verifica si un bloque existe y regresa su direccion.
long buscaBloque(FILE *f, Entidad entAct, Atributo *arrAtr, void* b, long tamBloque)
{
    void* data;
    long cab;

    cab = entAct.data;

    while (cab != -1)
    {
        data = leeBloque(f, cab, tamBloque);

        if(comparaBloques(arrAtr, data, b) == 0)
        {
            free(data);
            return cab;
        }

        cab = *((long *) data);
        free(data);
    }
    return -1;
}

//Sobreescribe un bloque dentro de un archivo con otro.
void reescribeBloque(FILE *f, void* b, long dir, long tamBloque)
{
    fseek(f,dir,SEEK_SET);
    fwrite(b,tamBloque,1,f);
}

//Verifica si los dos bloques que recibe son validos o no.
long existeBloqueDif(FILE *f, Entidad entAct, Atributo *arrAtr, void* b, void* bMod, long tamBloque)
{
    long cab;
    void* data;
    long des = sizeof(long);
    cab = entAct.data;

    while(cab != -1)
    {
        data = leeBloque(f, cab, tamBloque);

        //printf("\nclave %d\n", *((int*)(data+des)));

        if(comparaBloques(arrAtr, data, b) == 0 )
        {
            //printf("\nclave %d\n", *((int*)(b+des)));
            //printf("\nclave %d\n", *((int*)(bMod+des)));
            if(comparaBloques(arrAtr, data,bMod) == 0)
            {
                free(data);
                return -1;
            }
            else
                return cab;
        }
        cab = *((long*)data);
    }
    return -1;
}

//Se encarga de modificar los bloques, y verifica si existe o no cada bloque.
void modificaBloque(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, int nAtr,  long tamBloque)
{
    long dir;
    void *b, *nuevo;
    long resp;

    b = capturaBloqueClave(arrAtr, tamBloque, nAtr);

    if(buscaBloque(f, *entAct, arrAtr, b, tamBloque) != -1)
    {
        printf("\nBloque Nuevo:\n");
        nuevo = capturaBloque(arrAtr, tamBloque, nAtr);
        resp = existeBloqueDif(f, *entAct, arrAtr, nuevo, b, tamBloque);
        //printf("\n respuesta %ld\n", resp);

        if( resp == -1)
        {
            dir = eliminaBloque(f, entAct, direntAct, arrAtr, b, tamBloque);
            reescribeBloque(f, nuevo, dir, tamBloque);
            insertaBloque(f, entAct, direntAct, arrAtr, nuevo, tamBloque, dir);
        }
        else
            printf("\nError. El bloque YA existe.\n");
    }
    else
        printf("\nError. El bloque NO existe.\n");
}

//verifica si el bloque que queremos eliminar existe o no.
void bajaSecuencial(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, int nAtr, long tamBloque)
{
    void *b;
    long dir;

    b = capturaBloqueClave(arrAtr, tamBloque, nAtr);

    if(buscaBloque(f, *entAct, arrAtr, b, tamBloque) != -1)
    {
        eliminaBloque(f, entAct, direntAct, arrAtr, b, tamBloque);
        printf("\nSe elimino correctamente.\n");
    }
    else
        printf("\nError. El bloque NO existe.\n");
}

//Se encarga de eliminar los bloques y en que caso se elimina.
long eliminaBloque(FILE *f, Entidad *entAct, long direntAct, Atributo *arrAtr, void* b, long tamBloque)
{
    long cab = entAct->data;
    long dirAnt;
    void* bAnt = NULL;
    void* bAux;

    if(cab != -1)
    {
        bAux = leeBloque(f, cab, tamBloque);

        if (comparaBloques(arrAtr, b, bAux) == 0)
        {
            entAct->data = *((long *) bAux);
            reescribeEntidad(f, *entAct, direntAct);
            free(bAux);
            free(b);
            return cab;
        }
        else
        {
            while (cab != -1 && comparaBloques(arrAtr, b, bAux) > 0)
            {
                if (bAnt != NULL)
                    free(bAnt);

                bAnt = bAux;
                dirAnt = cab;
                cab = *((long *) bAux);

                if (cab != -1)
                    bAux = leeBloque(f, cab, tamBloque);
            }

            if (cab != -1)
            {
                *((long *) bAnt) = *((long *) bAux);
                reescribeBloque(f, bAnt, dirAnt, tamBloque);
            }
        }
        free(b);
        free(bAnt);
        return cab;
    }
    return -1;
}

//Se encarga de imprimir y mostrar los bloques de datos dentro de la entidad.
void consultaBloque(FILE *f, Entidad entAct, Atributo *arrAtr, int nAtr, void* b, long tamBloque)
{
    long des = sizeof(long);
    long cab = entAct.data;

    printf("\n--------------------------------------------------- BLOQUES ---------------------------------------------------\n");
    printf("Entidad activa: %s\n\n", entAct.nombre);

    for (int i = 0; i < nAtr; ++i)
        printf("%s\t\t", arrAtr[i].nombre);

    printf("\n");

    while (cab != -1)
    {
        des = sizeof(long);
        b = leeBloque(f, cab, tamBloque);

        for (int i = 0; i < nAtr; ++i)
        {
            switch (arrAtr[i].tipo)
            {
                case 1:
                    printf("%s\t\t", (char *)(b+des));
                    break;
                case 2:
                {
                    int ent;
                    ent = *((int*)(b+des));
                    printf("%d\t\t", ent);
                    break;
                }
                case 3:
                {
                    float flot;
                    flot = *((float*)(b+des));
                    printf("%0.2f\t\t", flot);
                    break;
                }
                case 4:
                {
                    double db;
                    db = *((double*)(b+des));
                    printf("%lf\t\t", db);
                    break;
                }
                case 5:
                {
                    long l;
                    l = *((long*)(b+des));
                    printf("%ld\t\t", l);
                    break;
                }
            }
            des += arrAtr[i].tam;
        }
        printf("\n");
        cab = *((long*)(b+0));
    }

    printf("\n-----------------------------------------------------------------------------------------------------------------\n");
}

//Captura unicamete la clav eprimaria de un bloque.
void* capturaBloqueClave(Atributo *arrAtr, long tamBloque, int nAtr)
{
    void* p = malloc(tamBloque);
    *((long*)p+0) = (long) - 1;
    long des = sizeof(long);

        printf("\nBloque que desea modificar:\n", arrAtr[0].nombre);

        switch (arrAtr[0].tipo)
        {
            case 1:
            {
                char cad[500];
                fflush(stdin);
                scanf("%s", cad);
                cad[arrAtr[0].tam - 1] = '\0';
                strcpy((char *)(p+des),cad);
                break;
            }
            case 2:
            {
                int entero;
                scanf("%d", &entero);
                *((int*)(p+des)) = entero;
                break;
            }
            case 3:
            {
                float flotante;
                scanf("%f", &flotante);
                *((float*)(p+des)) = flotante;
                break;
            }
            case 4:
            {
                double doble;
                scanf("%lf", &doble);
                *(double*)(p+des) = doble;
                break;
            }
            case 5:
            {
                long largo;
                scanf("%ld", &largo);
                *((long*)(p+des)) = largo;
                break;
            }
        }
    return p;
}
