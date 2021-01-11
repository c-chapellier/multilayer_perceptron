#include "neural_network.h"

void    copy_1d(double *dst, double *src, uint x)
{
    for (uint i = 0; i < x; ++i)
    {
        dst[i] = src[i];
    }
}

void    copy_2d(double **dst, double **src, uint x, uint y)
{
    for (uint i = 0; i < x; ++i)
    {
        copy_1d(dst[i], src[i], y);
    }
}

void    copy_3d(double ***dst, double ***src, uint x, uint y, uint z)
{
    for (uint i = 0; i < x; ++i)
    {
        copy_2d(dst[i], src[i], y, z);
    }
}

bool    alloc_1d(double **to_alloc, uint x)
{
    if ((*to_alloc = malloc(x * sizeof(double))) == NULL)
    {
        printf("ERROR: alloc_1d: malloc\n");
        return (false);
    }
    return (true);
}

bool    alloc_2d(double ***to_alloc, uint x, uint y)
{
    if ((*to_alloc = malloc(x * sizeof(double *))) == NULL)
    {
        printf("ERROR: alloc2d: malloc\n");
        return (false);
    }

    for (uint i = 0; i < x; ++i)
    {
        if (alloc_1d(&to_alloc[0][i], y) == false)
            return (false);
    }
    return (true);
}

bool    alloc_3d(double ****to_alloc, uint x, uint y, uint z)
{
    if ((*to_alloc = malloc(x * sizeof(double *))) == NULL)
    {
        printf("ERROR: alloc3d: malloc\n");
        return (false);
    }

    for (uint i = 0; i < x; ++i)
    {
        if (alloc_2d(&to_alloc[0][i], y, z) == false)
            return (false);
    }
    return (true);
}

void    free_2d(double **to_free, uint x)
{
    if (to_free != NULL)
    {
        for (uint i = 0; i < x; ++i)
        {
            free(to_free[i]);
        }
        free(to_free);
    }
}

void    free_3d(double ***to_free, uint x, uint y)
{
    if (to_free != NULL)
    {
        for (uint i = 0; i < x; ++i)
        {
            free_2d(to_free[i], y);
        }
        free(to_free);
    }
}

void    fill_randomly_1d(double *to_fill, uint x)
{
    for (uint i = 0; i < x; ++i)
    {
        to_fill[i] = random_double();
    }
}

void    fill_randomly_2d(double **to_fill, uint x, uint y)
{
    for (uint i = 0; i < x; ++i)
    {
        fill_randomly_1d(to_fill[i], y);
    }
}

void    fill_randomly_3d(double ***to_fill, uint x, uint y, uint z)
{
    for (uint i = 0; i < x; ++i)
    {
        fill_randomly_2d(to_fill[i], y, z);
    }
}

double  random_double()
{
    return (((((double)rand()) / RAND_MAX) * 2.0) - 1.0);
}

void    fill_of_zeros_1d(double *to_fill, uint x)
{
    for (uint i = 0; i < x; ++i)
    {
        to_fill[i] = 0.0;
    }
}

void    fill_of_zeros_2d(double **to_fill, uint x, uint y)
{
    for (uint i = 0; i < x; ++i)
    {
        fill_of_zeros_1d(to_fill[i], y);
    }
}

void    fill_of_zeros_3d(double ***to_fill, uint x, uint y, uint z)
{
    for (uint i = 0; i < x; ++i)
    {
        fill_of_zeros_2d(to_fill[i], y, z);
    }
}

void    print_1d(double *array, uint x)
{
    for (uint i = 0; i < x; ++i)
    {
        printf(" %f", array[i]);
    }
    printf("\n");
}

void    print_2d(double **array, uint x, uint y)
{
    for (uint i = 0; i < x; ++i)
    {
        print_1d(array[i], y);
    }
    printf("\n");
}

void    print_3d(double ***array, uint x, uint y, uint z)
{
    for (uint i = 0; i < x; ++i)
    {
        print_2d(array[i], y, z);
    }
    printf("\n");
}

