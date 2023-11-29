#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void menuBloques(FILE *f, Entidad entAct, long direntAct);
void insertaBloque(FILE *f);
long eliminaBloque(FILE *f);
void* leeBloque(FILE *f, long dir);
long escribeBloque(FILE *f, void* b);
long buscaBloque(FILE *f, Entidad entAct, void* b);
long comparaBloques(FILE *f, void* b1, void* b2);



int main()
{
    FILE *f = NULL;

    menuPrincipal(f);
    return 0;

}

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

void cerrarDiccionario(FILE *f)
{
    if(f != NULL)
        fclose(f);
}

//Funciones de Entidades

void menuEntidades(FILE *f)
{
    int opc;
    Entidad entAct;
    long direntAct;

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
            case 7:
                menuPrincipal(f);
            default:
                printf("Ingrese una de las opciones validas 1 - 7.\n");
        }
    } while (opc != 7);

}

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

void altaEntidad(FILE *f)
{
    Entidad ent;
    long pos;

    ent = capturaEntidad();  //Se captura la entidad que se desea agregar al archivo

    if(buscaEntidad(f, ent.nombre) == -1)  //Verifica si la entidad existe o no
    {
        pos = escribeEntidad(f, ent); //Escribe la entidad en el archivo pero no es accesible
        insertaEntidad(f, ent, pos);  //Crea los enlaces necesarios para acceder a la entidad
        fseek(f, 0, SEEK_END);
    }
    else
        printf("La Entidad ya existe.\n");
}

Entidad capturaEntidad()
{
    Entidad ent;

    printf("Nombre de la Entidad:");
    scanf("%s", ent.nombre);

    ent.sig = ent.atr = ent.data = (long) - 1;

    return ent;
}

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

long escribeEntidad(FILE *f, Entidad ent)
{
    long dir;

    fseek(f, 0, SEEK_END);
    dir = ftell(f);
    fwrite(&ent, sizeof (Entidad), 1, f);

    return dir;
}

void escribeCabEntidades(FILE *f)
{
    long cab = -1;

    fseek(f, 0, SEEK_SET);
    fwrite(&cab, sizeof (long), 1, f);
}

long getCabEntidades(FILE *f)
{
    long cab;
    fseek(f,0,SEEK_SET);
    fread(&cab,sizeof (long),1,f);

    return cab;
}

Entidad leeEntidad(FILE  *f,long dir)
{
    Entidad ent;
    fseek(f,dir,SEEK_SET);
    fread(&ent,sizeof(Entidad),1,f);
    return ent;
}

void reescribeEntidad(FILE *f,Entidad ent, long dir)
{
    fseek(f,dir,SEEK_SET);
    fwrite(&ent,sizeof(Entidad),1,f);
}

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

void rescribeCabEntidades(FILE *f,long dir)
{
    long cab = dir;
    fseek(f,0 , SEEK_SET);
    fwrite(&cab,sizeof(long),1,f);
}

void modificaEntidad(FILE *f)
{
    cadena nombEnt, nombEntNueva;
    Entidad nuevaEnt;
    long dir;

    pideNomEnt(nombEnt);

    if(buscaEntidad(f, nombEnt) != -1)
    {
        pideNomEnt(nombEntNueva);
        if(buscaEntidad(f, nombEntNueva) == -1)
        {
            nuevaEnt = capturaEntidad();
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

void pideNomEnt(cadena nombEnt)
{
    printf("Ingresa el nombre de la Entidad:");
    scanf("%s", nombEnt);
    printf("\n");
}




//Funciones de Atributos

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
    scanf("%s", nuevoAtributo.descripcion);

    nuevoAtributo.sig = -1;

    return nuevoAtributo;
}

long buscaAtributo(FILE *f, cadena atrNom, Entidad entAct)
{
    long cab;
    Atributo atr;

    printf("\nCabecera: %ld\n", entAct.atr);
    cab = entAct.atr;

    while(cab != -1)
    {
        printf("\nCabecera: %ld\n", entAct.atr);
        atr = leeAtributo(f, cab);

        if(strcmp(atr.nombre, atrNom) == 0)
            return cab;

        cab = atr.sig;
    }
    return -1;
}
void reescribeAtributo(FILE *f, Atributo atr, long dir)
{
    fseek(f,dir,SEEK_SET);
    fwrite(&atr,sizeof (atr),1,f);
}

Atributo leeAtributo(FILE *f, long dir)
{
    Atributo atr;

    fseek(f, dir, SEEK_SET);
    fread(&atr, sizeof(Atributo), 1, f);

    return atr;
}
long escribeAtributo(FILE *f, Atributo atr)
{
    long pos;

    fseek(f,0,SEEK_END);
    pos = ftell(f);
    fwrite(&atr,sizeof(atr),1,f);

    return pos;
}

void altaAtributo(FILE *f, Entidad *entAct, long direntAct)
{
    Atributo nuevoAtr;
    long dirNuevo;

    nuevoAtr = capturaAtributo();
    printf("\nCaptura lista\n");

    if(buscaAtributo(f, nuevoAtr.nombre, *entAct) == -1)
    {
        printf("\nTermine de Buscar\n");
        dirNuevo = escribeAtributo(f, nuevoAtr);
        printf("Escribi en %ld", dirNuevo);
        insertaAtributo(f, nuevoAtr, dirNuevo, entAct, direntAct);
    }
    else
        printf("Error. El Atributo que desea agregar ya existe.");
}

void insertaAtributo(FILE *f, Atributo atr, long dir, Entidad *entAct, long direntAct)
{
    long cab, dirAnt;
    Atributo atrAux, atrAct, atrAnt;

    cab = entAct->atr;

    printf("\nCabecera en inserta: %ld\n", cab);

    if(cab == -1)
    {
        printf("\nCaso 1\n");
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

void modificaAtributo(FILE *f, Entidad *entAct, long direntAct)
{
    Atributo nuevoAtr;
    cadena nombAtr;
    long dirAtr;

    pideNombAtr(nombAtr);

    if(buscaAtributo(f, nombAtr, *entAct) != -1)
    {
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

void pideNombAtr(cadena nombAtr)
{
    printf("Ingresa el nombre del atributo:");
    scanf("%s", nombAtr);
    printf("\n");
}




//Funciones de Bloques

/*
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

void menuBloques(FILE *f, Entidad entAct, long direntAct)
{
    int opc;

    do {
        opc=opcAtr();

        switch(opc)
        {
            case 1:
                //insertaBloque
                break;
            case 2:
                //consultaBloque
                break;
            case 3:
                //eliminaBloque
                break;
            case 4:
                //modificaBloque
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

void insertaBloque(FILE *f)
{

}

long eliminaBloque(FILE *f)
{

}

void* leeBloque(FILE *f, long dir)
{
    long tamBloque;
    void* b = malloc(tamBloque);

    fseek(f, dir, SEEK_SET);
    fread(b, tamBloque, 1, f);

    return b;
}

long escribeBloque(FILE *f, void* b)
{
    long pos, tamBloque;

    fseek(f, 0, SEEK_END);
    pos = ftell(f);
    fwrite(b, tamBloque, 1, f);

    return pos;
}

long buscaBloque(FILE *f, Entidad entAct, void* b)
{
    void* data;
    long cab;

    cab = entAct.data;

    while (cab != -1)
    {
        data = leeBloque(f, cab);

        if(comparaBloques(f, data, b) == 0)
        {
            free(data);
            return cab;
        }

        cab = *((long *) data);
        free(data);
    }
    return -1;
}
*/