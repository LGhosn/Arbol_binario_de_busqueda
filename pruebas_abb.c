#include "abb.h"
#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* *****************************************************************
 *                          AUXLIARES
 * *****************************************************************/
void destruir_wrapper(void *elem)
{
    pila_destruir(elem);
}

// 'VISITAR' PARA EL ITERADOR INTERNO
bool sumar_todos(const char *clave, void *dato, void *extra)
{
    *(int *)extra += *(int *)dato;
    return true;
}

bool imprimir_3_datos(const char* clave, void* valor, void* extra) {
    if (*(size_t*) extra == 3) return false;
    printf("%d\n", *(int*) valor);
    *(size_t*) extra +=1;
    return true;
}


static ssize_t buscar(const char *clave, char *claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++)
    {
        if (strcmp(clave, claves[i]) == 0)
            return (ssize_t)i;
    }
    return -1;
}
/* *****************************************************************
 *                          PRUEBAS
 * *****************************************************************/
static void prueba_crear_abb_vacio()
{
    printf("\n----------------PRUEBA CREAR ABB VACIO----------------\n");
    abb_t *abb = abb_crear(NULL, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    // print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_iterar_abb_vacio()
{
    printf("\n----------------PRUEBA ITER EXTERNO ABB VACIO----------------\n");
    abb_t *abb = abb_crear(NULL, NULL);
    abb_iter_t *iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_insertar()
{
    printf("\n----------------PRUEBA ABB INSERTAR----------------\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gatitos", *valor2 = "miaus";
    char *clave3 = "vaca", *valor3 = "mu";

    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    printf("Clave >>> %s\n", (char*)abb_obtener(abb, clave1));
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));

    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 3", abb_cantidad(abb) == 3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_abb_reemplazar()
{
    printf("\n----------------PRUEBA ABB REEMPLAZAR----------------\n");

    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir()
{
    printf("\n----------------PRUEBA ABB REEMPLAZAR CON DESTRUIR----------------\n");
    abb_t *abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

static void prueba_abb_destruir_con_funcion_de_destruccion()
{
    printf("\n----------------PRUEBA ABB DESTRUIR CON FUNCION DE DESTRUCCION----------------\n");
    abb_t *abb = abb_crear(strcmp, destruir_wrapper);

    char *clave1 = "perro";
    char *clave2 = "gatitos";
    char *clave3 = "vaca";

    pila_t *p1 = pila_crear();
    pila_t *p2 = pila_crear();
    pila_t *p3 = pila_crear();

    print_test("Se interta inserta clave1, valor pila", abb_guardar(abb, clave1, p1));
    print_test("Se inserta inserta clave2, valor pila", abb_guardar(abb, clave2, p2));
    print_test("Se inserta inserta clave3, valor pila", abb_guardar(abb, clave3, p3));

    abb_destruir(abb);
}

static void prueba_iterador_interno(void)
{
    printf("\n----------------PRUEBA ABB ITER INTERNO ----------------\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    int numeros[] = {1, 2, 3, 4};
    char *clave1 = "a";
    char *clave2 = "b";
    char *clave3 = "c";
    char *clave4 = "d";
    abb_guardar(abb, clave1, &numeros[0]);
    abb_guardar(abb, clave2, &numeros[1]);
    abb_guardar(abb, clave3, &numeros[2]);
    abb_guardar(abb, clave4, &numeros[3]);

    size_t suma = 0;
    abb_in_order(abb, sumar_todos, &suma);
    print_test("La iteracion con 'sumar_todos' es correcta (itera sin condición de corte)", suma == 10);
    size_t prints = 0;
    abb_in_order(abb, imprimir_3_datos, &prints);
    print_test("Se imprimieron 3 datos con el iterador.", prints == 3);
    abb_destruir(abb);
}

static void prueba_iterador_externo()
{
    printf("\n----------------PRUEBA ABB ITER EXTERNO----------------\n");

    abb_t *abb = abb_crear(strcmp, NULL);

    int numeros[] = {1, 2, 3, 4};
    char *claves[] = {"a", "b", "c", "e", "f"};
    abb_guardar(abb, claves[3], &numeros[0]);
    abb_guardar(abb, claves[1], &numeros[1]);
    abb_guardar(abb, claves[0], &numeros[2]);
    abb_guardar(abb, claves[2], &numeros[3]);
    abb_guardar(abb, claves[4], &numeros[0]);

    abb_iter_t *iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    /* Primer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    printf("\n PRIMERO ELEMENTO\n");
    print_test("Se crea el iterador", iter);
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Segundo valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    printf("\n SEGUNDO ELEMENTO\n");
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Tercer valor */
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    printf("\n TERCER ELEMENTO\n");
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Cuarto valor */
    clave = abb_iter_in_ver_actual(iter);
    printf("\n CUARTO ELEMENTO\n");
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    /* Quinto valor */
    printf("\n QUINTO ELEMENTO\n");
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    /* Vuelve a tratar de avanzar, por las dudas */
    printf("\n NINGUN ELEMENTO\n");
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_borrar_raiz_sin_hijos()
{
    printf("\n----------------PRUEBA ABB BORRAR RAIZ SIN HIJOS----------------\n");

    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave = "a";
    int valor = 1;

    print_test("Se guarda la clave 'a' ", abb_guardar(abb, clave, &valor));
    print_test("La cantidad del arbol es 1", abb_cantidad(abb) == 1);
    print_test("Borrar devuelve el valor de 'a' ", *(int *)abb_borrar(abb, clave) == 1);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb, clave));
    print_test("La cantidad del arbol es 0", abb_cantidad(abb) == 0);
    abb_destruir(abb);
}

static void prueba_borrar_nodo_sin_hijos()
{
    printf("\n----------------PRUEBA ABB BORRAR NODO SIN HIJOS----------------\n");

    abb_t *abb = abb_crear(strcmp, NULL);
    int numeros[] = {1, 2};
    char *claves[] = {"a", "b"};
    print_test("Se guarda la clave 'a' ", abb_guardar(abb, claves[0], &numeros[0]));
    print_test("Se guarda la clave 'b' ", abb_guardar(abb, claves[1], &numeros[1]));
    print_test("La cantidad del arbol es 2", abb_cantidad(abb) == 2);
    print_test("Borrar devuelve el valor de 'b' ", *(int *)abb_borrar(abb, claves[1]) == 2);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb, claves[1]));
    print_test("La cantidad del arbol es 1", abb_cantidad(abb) == 1);
    abb_destruir(abb);

    abb_t *abb2 = abb_crear(strcmp, NULL);
    print_test("Se guarda la clave 'b' ", abb_guardar(abb2, claves[1], &numeros[1]));
    print_test("Se guarda la clave 'a' ", abb_guardar(abb2, claves[0], &numeros[0]));
    print_test("La cantidad del arbol es 2", abb_cantidad(abb2) == 2);
    print_test("Borrar devuelve el valor de 'a' ", *(int *)abb_borrar(abb2, claves[0]) == 1);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb2, claves[0]));
    print_test("La cantidad del arbol es 1", abb_cantidad(abb2) == 1);
    print_test("Borrar devuelve el valor de 'b' ", *(int *)abb_borrar(abb2, claves[1]) == 2);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb2, claves[1]));
    print_test("La cantidad del arbol es 0", abb_cantidad(abb2) == 0);
    abb_destruir(abb2);
}

static void prueba_borrar_raiz_con_un_hijo()
{
    printf("\n----------------PRUEBA ABB BORRAR RAIZ CON UN HIJO----------------\n");

    abb_t *abb = abb_crear(strcmp, NULL);

    int numeros[] = {1, 2, 3};
    char *claves[] = {"a", "b", "c"};

    print_test("Se guarda la clave 'a' ", abb_guardar(abb, claves[0], &numeros[0]));
    print_test("Se guarda la clave 'b' ", abb_guardar(abb, claves[1], &numeros[1]));
    print_test("La cantidad del arbol es 2", abb_cantidad(abb) == 2);
    print_test("Borrar devuelve el valor de 'a' ", *(int *)abb_borrar(abb, claves[0]) == 1);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb, claves[0]));
    print_test("La cantidad del arbol es 1", abb_cantidad(abb) == 1);
    print_test("La clave 'b' sigue en el arbol ", abb_pertenece(abb, claves[1]));
    abb_destruir(abb);
}

static void prueba_borrar_nodo_con_un_hijo()
{
    printf("\n----------------PRUEBA ABB BORRAR NODO CON UN HIJO----------------\n");

    abb_t *abb = abb_crear(strcmp, NULL);

    int numeros[] = {1, 2, 3, 4};
    char *claves[] = {"a", "b", "c", "d"};

    print_test("Se guarda la clave 'a' ", abb_guardar(abb, claves[0], &numeros[0]));
    print_test("Se guarda la clave 'b' ", abb_guardar(abb, claves[1], &numeros[1]));
    print_test("Se guarda la clave 'c' ", abb_guardar(abb, claves[2], &numeros[2]));
    print_test("La cantidad del arbol es 3", abb_cantidad(abb) == 3);
    print_test("Borrar devuelve el valor de 'b' ", *(int *)abb_borrar(abb, claves[1]) == 2);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb, claves[1]));
    print_test("La cantidad del arbol es 2", abb_cantidad(abb) == 2);
    print_test("La clave 'c' sigue en el arbol ", abb_pertenece(abb, claves[2]));
    abb_destruir(abb);

    abb_t *abb1 = abb_crear(strcmp, NULL);

    print_test("Se guarda la clave 'c' ", abb_guardar(abb1, claves[2], &numeros[2]));
    print_test("Se guarda la clave 'b' ", abb_guardar(abb1, claves[1], &numeros[1]));
    print_test("Se guarda la clave 'a' ", abb_guardar(abb1, claves[0], &numeros[0]));
    print_test("La cantidad del arbol es 3", abb_cantidad(abb1) == 3);
    print_test("Borrar devuelve el valor de 'b' ", *(int *)abb_borrar(abb1, claves[1]) == 2);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb1, claves[1]));
    print_test("La cantidad del arbol es 2", abb_cantidad(abb1) == 2);
    print_test("La clave 'a' sigue en el arbol ", abb_pertenece(abb1, claves[0]));
    abb_destruir(abb1);

    abb_t *abb2 = abb_crear(strcmp, NULL);
    print_test("Se guarda la clave 'c' ", abb_guardar(abb2, claves[2], &numeros[2]));
    print_test("Se guarda la clave 'b' ", abb_guardar(abb2, claves[1], &numeros[1]));
    print_test("Se guarda la clave 'a' ", abb_guardar(abb2, claves[0], &numeros[0]));
    print_test("Se guarda la clave 'd' ", abb_guardar(abb2, claves[3], &numeros[3]));
    print_test("La cantidad del arbol es 4", abb_cantidad(abb2) == 4);
    print_test("Borrar devuelve el valor de 'b' ", *(int *)abb_borrar(abb2, claves[1]) == 2);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb2, claves[1]));
    print_test("La cantidad del arbol es 3", abb_cantidad(abb2) == 3);
    print_test("La clave 'a' sigue en el arbol ", abb_pertenece(abb2, claves[0]));
    print_test("La clave 'd' sigue en el arbol ", abb_pertenece(abb2, claves[3]));
    abb_destruir(abb2);
}

static void prueba_borrar_raiz_con_dos_hijos(){
    printf("\n----------------PRUEBA ABB BORRAR RAIZ CON DOS HIJO----------------\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    int numeros[] = {1, 2, 3};
    char *claves[] = {"a", "b", "c"};
    print_test("Se guarda la clave 'b' ", abb_guardar(abb, claves[1], &numeros[1]));
    print_test("Se guarda la clave 'c' ", abb_guardar(abb, claves[2], &numeros[2]));
    print_test("Se guarda la clave 'a' ", abb_guardar(abb, claves[0], &numeros[0]));
    print_test("La cantidad del arbol es 3", abb_cantidad(abb) == 3);
    print_test("Borrar devuelve el valor de 'b' ", *(int*)abb_borrar(abb, claves[1]) == 2);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb, claves[1]));
    print_test("La cantidad del arbol es 2", abb_cantidad(abb) == 2);
    print_test("La clave 'a' sigue en el arbol ", abb_pertenece(abb, claves[0]));
    print_test("La clave 'd' sigue en el arbol ", abb_pertenece(abb, claves[2]));

    abb_destruir(abb);
}

static void prueba_borrar_nodo_con_dos_hijos(){
    printf("\n----------------PRUEBA ABB BORRAR NODO CON DOS HIJO----------------\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    int numeros[] = {1, 2, 3, 4, 5};
    char *claves[] = {"a", "d", "c", "e", "f"};
    print_test("Se guarda la clave 'e' ", abb_guardar(abb, claves[3], &numeros[3]));
    print_test("Se guarda la clave 'c' ", abb_guardar(abb, claves[2], &numeros[2]));
    print_test("Se guarda la clave 'd' ", abb_guardar(abb, claves[1], &numeros[1]));
    print_test("Se guarda la clave 'a' ", abb_guardar(abb, claves[0], &numeros[0]));
    print_test("Se guarda la clave 'f' ", abb_guardar(abb, claves[4], &numeros[4]));
    print_test("La cantidad del arbol es 5", abb_cantidad(abb) == 5);
    print_test("Borrar devuelve el valor de 'c' ", *(int *)abb_borrar(abb, claves[2]) == 3);
    print_test("La cantidad del arbol es 4", abb_cantidad(abb) == 4);
    print_test("La clave ya no se encuentra en el arbol.", !abb_pertenece(abb, claves[2]));
    print_test("La clave 'f' sigue en el arbol ", abb_pertenece(abb, claves[4]));
    print_test("La clave 'e' sigue en el arbol ", abb_pertenece(abb, claves[3]));
    print_test("La clave 'd' sigue en el arbol ", abb_pertenece(abb, claves[1]));
    print_test("La clave 'a' sigue en el arbol ", abb_pertenece(abb, claves[0]));

    abb_destruir(abb);
}

static void prueba_abb_clave_vacia()
{
    printf("\n----------------PRUEBA ABB CLAVE VACIA----------------\n");
    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null()
{
    printf("\n----------------PRUEBA ABB VALOR NULL----------------\n");

    abb_t *abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_volumen(size_t largo, bool debug)
{
    printf("\n----------------PRUEBA ABB VOLUMEN----------------\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    //  Inserta 'largo' parejas en el abb 
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%d", rand());
        *valores[i] = i;
    }
    bool ok = true;
    for (unsigned j = 0; j < largo; j++){
        ok = abb_guardar(abb, claves[j], valores[j]); 
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    //  Verifica que devuelva los valores correctos 
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        
        ok = abb_obtener(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    //  Verifica que borre y devuelva los valores correctos 
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb borrar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    // Destruye el abb y crea uno nuevo que sí libera 
    abb_destruir(abb);
    abb_t *abb_2 = abb_crear(strcmp, free);

    //  Inserta 'largo' parejas en el abb 
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb_2, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

    //  Destruye el abb - debería liberar los enteros 
    abb_destruir(abb_2);

}

void pruebas_abb_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_abb_vacio();
    prueba_iterar_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_destruir_con_funcion_de_destruccion();
    prueba_iterador_interno();
    prueba_iterador_externo();
    prueba_borrar_raiz_sin_hijos();
    prueba_borrar_nodo_sin_hijos();
    prueba_borrar_raiz_con_un_hijo();
    prueba_borrar_nodo_con_un_hijo();
    prueba_borrar_raiz_con_dos_hijos();
    prueba_borrar_nodo_con_dos_hijos();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_volumen(5000, true);
}

#ifndef CORRECTOR // Para que no dé conflicto con el main() del corrector.

int main(void){
    pruebas_abb_estudiante();
    return failure_count() > 0; // Indica si falló alguna prueba.
}

#endif
