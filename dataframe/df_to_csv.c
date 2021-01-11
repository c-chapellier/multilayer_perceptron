#include "df.h"

char    *strjoin(char *s1, const char *s2)
{
    char    *str;

    if (s1 == NULL)
    {
        if ((str = malloc(strlen(s2) + 1)) == NULL)
        {
            printf("ERROR: df_write_to_csv: strjoin: malloc\n");
            return (NULL);
        }
        strcpy(str, s2);
        return (str);
    }
    if ((str = malloc(strlen(s1) + strlen(s2) + 1)) == NULL)
    {
        printf("ERROR: df_write_to_csv: strjoin: malloc\n");
        return (NULL);
    }
    strcat(strcpy(str, s1), s2);
    return (str);
}

static void write_csv(const char *filename, const char *content)
{
    FILE    *fptr;

    if ((fptr = fopen(filename, "w")) == NULL)
    {
        printf("ERROR: df_write_to_csv: fopen\n");
        return ;
    }
    fprintf(fptr, "%s", content);
    fclose(fptr);
}

void    df_to_csv(const dataframe_t df, const char *filename)
{
    char    *content = NULL, *tmp = NULL, tmp_double[50];

    if (df.columns != NULL)
    {
        for (uint i = 0; i < df.shape[1]; ++i)
        {
            if (df.columns[i] != NULL)
            {
                if ((tmp = strjoin(content, df.columns[i])) == NULL)
                {
                    free(content);
                    return ;
                }
                free(content);
                content = tmp;
                if (i != df.shape[1] - 1)
                {
                    if ((tmp = strjoin(content, ",")) == NULL)
                    {
                        free(content);
                        return ;
                    }
                    free(content);
                    content = tmp;
                }
            }
        }
    }
    for (uint i = 0; i < df.shape[0]; ++i)
    {
        for (uint j = 0; j < df.shape[1]; ++j)
        {
            if (j == df.shape[1] - 1)
                sprintf(tmp_double, "%f", df.data[i][j]);
            else
                sprintf(tmp_double, "%f,", df.data[i][j]);
            if ((tmp = strjoin(content, tmp_double)) == NULL)
            {
                free(content);
                return ;
            }
            free(content);
            content = tmp;
        }
        if ((tmp = strjoin(content, "\n")) == NULL)
        {
            free(content);
            return ;
        }
        free(content);
        content = tmp;
    }
    write_csv(filename, content);
    free(content);
}