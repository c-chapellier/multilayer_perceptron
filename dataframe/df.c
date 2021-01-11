#include "df.h"

bool    df_drop_column(dataframe_t df, const char *column_name)
{
    int     index;

    if ((index = df_get_index(df, column_name)) == -1)
        return (false);
    free(df.columns[index]);
    for (uint i = index; i < df.shape[1] - 1; ++i)
        df.columns[i] = df.columns[i + 1];
    for (uint i = 0; i < df.shape[0]; ++i)
    {
        for (uint j = index; j < df.shape[1] - 1; ++j)
            df.data[i][j] = df.data[i][j + 1];
        df.data[i][df.shape[1] - 1] = -1.0;
    }
    df.columns[df.shape[1] - 1] = NULL;
    df.shape[1] -= 1;
    return (true);
}

double  **df_get_data(const dataframe_t df)
{
    double  **data;
    
    if ((data = malloc(df.shape[0] * sizeof(double *))) == NULL)
        return (NULL);
    for (uint i = 0; i < df.shape[0]; ++i)
    {
        if ((data[i] = malloc(df.shape[1] * sizeof(double))) == NULL)
            return (NULL);
        for (uint j = 0; j < df.shape[1]; ++j)
            data[i][j] = df.data[i][j];
    }
    data[df.shape[0]] = NULL;
    return (data);
}

double  *df_get_column(const dataframe_t df, const char *column_name)
{
    double  *column;
    int     index;
    
    if ((index = df_get_index(df, column_name)) == -1)
        return (NULL);
    if ((column = malloc(df.shape[0] * sizeof(double))) == NULL)
        return (NULL);
    for (uint i = 0; i < df.shape[0]; ++i)
    {
        column[i] = df.data[i][index];
    }
    return (column);
}


int    df_get_index(const dataframe_t df, const char *column_name)
{
    int     index = -1;

    for (uint i = 0; df.columns[i] != NULL; ++i)
    {
        if (strcmp(df.columns[i], column_name) == 0)
            index = i;
    }
    return (index);
}

void    df_shuffle(dataframe_t df)
{
    uint    j;
    double  *tmp;

    if (df.shape[0] > 1) 
    {
        for (uint i = 0; i < df.shape[0] - 1; ++i) 
        {
            j = i + rand() / (RAND_MAX / (df.shape[0] - i) + 1);
            tmp = df.data[j];
            df.data[j] = df.data[i];
            df.data[i] = tmp;
        }
    }
}

bool    df_free(dataframe_t *df)
{
    if (df != NULL)
    {
        if (df->shape != NULL)
            free(df->shape);
        if (df->columns != NULL)
        {
            for (uint i = 0; df->columns[i] != NULL; ++i)
                free(df->columns[i]);
            free(df->columns);
        }
        if (df->data != NULL)
        {
            for (uint i = 0; df->data[i] != NULL; ++i)
                free(df->data[i]);
            free(df->data);
        }
        free(df);
    }
    return (true);
}

void df_print(const dataframe_t df)
{
    df_head(df, df.shape[0]);
}

void df_head(const dataframe_t df, uint size)
{
    printf("shape = (%u, %u)\n", df.shape[0], df.shape[1]);
    printf("columns =");
    for (uint i = 0; df.columns[i] != NULL; ++i)
        printf(" %s", df.columns[i]);
    printf("\n");
    printf("data =");
    if (size > df.shape[0])
        size = df.shape[0];
    for (uint i = 0; i < size; ++i) {
        for (uint j = 0; j < df.shape[1]; ++j)
            printf(" %f", df.data[i][j]);
        printf("\n");
    }
    printf("\n");
}