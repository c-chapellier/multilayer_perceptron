#ifndef DF_H
# define DF_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <stdbool.h>

# define uint unsigned int

typedef struct      dataframe_s
{
    char    **columns;
    uint    *shape;
    double  **data;
}                   dataframe_t;

dataframe_t *df_from_csv(const char *dataset_path, char **columns, bool are_columns_in_csv, uint nbr_of_columns);
dataframe_t *df_from_array(uint x, uint y, double **data);
void        df_to_csv(const dataframe_t df, const char *filename);
bool        df_drop_column(dataframe_t df, const char *column_name);
double      **df_get_data(const dataframe_t df);
double      *df_get_column(const dataframe_t df, const char *column_name);
int         df_get_index(const dataframe_t df, const char *column_name);
void        df_shuffle(dataframe_t df);
bool        df_free(dataframe_t *df);
void        df_print(const dataframe_t df);
void        df_head(const dataframe_t df, uint size);

void        *ft_malloc(uint size, const char *error_message);
char        *strjoin(char *s1, const char *s2);

#endif