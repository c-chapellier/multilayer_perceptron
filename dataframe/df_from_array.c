#include "df.h"

dataframe_t *df_from_array(uint x, uint y, double **data)
{
    dataframe_t *df;

    if ((df = ft_malloc(sizeof(dataframe_t), "ERROR: df_from_array: df_init: malloc\n")) == NULL)
        return (NULL);
    if ((df->shape = ft_malloc(2 * sizeof(uint), "ERROR: df_from_array: df_init: malloc\n")) == NULL && df_free(df))
        return (NULL);
    df->shape[0] = x;
    df->shape[1] = y;
    df->columns = NULL;

    if ((df->data = ft_malloc((df->shape[0] + 1) * sizeof(double *), "ERROR: df_from_array: df_init: malloc\n")) == NULL && df_free(df))
        return (NULL);
    for (uint i = 0; i < df->shape[0]; ++i)
    {
        if ((df->data[i] = ft_malloc(df->shape[1] * sizeof(double), "ERROR: df_from_array: df_init: malloc\n")) == NULL && df_free(df))
            return (NULL);
        for (uint j = 0; j < df->shape[1]; ++j)
        {
            df->data[i][j] = data[i][j];
        }
    }
    df->data[df->shape[0]] = NULL;
    return (df);
}