#include "../lib/samp-plugin-sdk/amx/amx.h"
#include <cstdio>

extern void *pAMXFunctions;

int AMXAPI amx_FindNative(AMX *amx, const char *name, int *index);

int CallNative(AMX *amx, const char* native_name, int param)
{
    int native_idx = -1;
    int err = amx_FindNative(amx, native_name, &native_idx);
    if (err != AMX_ERR_NONE || native_idx == -1) {
        printf("Native '%s' no encontrada! Error: %d\n", native_name, err);
        return -1;
    }

    cell amx_addr = 0;
    cell *phys_addr = nullptr;

    err = amx_Allot(amx, 2, &amx_addr, &phys_addr);  // 2 = número de "longs" que se reservan: 1 para tamaño y 1 para parámetro
    if (err != AMX_ERR_NONE) {
        printf("Error amx_Allot: %d\n", err);
        return -1;
    }

    cell *amx_params = &amx_addr;

    amx_params[0] = 1 * sizeof(cell); // número de bytes de los parámetros (1 parámetro * tamaño cell)
    amx_params[1] = param;            // primer parámetro

    cell ret_val = 0;

    // Llamamos a amx_Exec sin pasar amx_params
    err = amx_Exec(amx, &ret_val, native_idx);
    if (err != AMX_ERR_NONE) {
        printf("Error al ejecutar la nativa %s: %d\n", native_name, err);
        return -1;
    }

    return ret_val;
}
