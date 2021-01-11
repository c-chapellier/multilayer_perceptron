#include "df.h"

static int	nbr_of_words(char const *s, char c)
{
	int		nbr_words;
	int		i;
	char	c_bef;

	nbr_words = 0;
	c_bef = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c && c_bef != c && c_bef != 0)
		{
			nbr_words++;
		}
		c_bef = s[i];
		i++;
	}
	if (c_bef != c && c_bef != 0)
	{
		nbr_words++;
	}
	return (nbr_words);
}

static void	init_tab(int *a, int *b, int *c, int *x)
{
	*a = 0;
	*b = -1;
	*c = -1;
	*x = 0;
}

static int	alloc_words(char **tab, char const *s, char c)
{
	int		tab2[4];

	init_tab(&tab2[2], &tab2[0], &tab2[1], &tab2[3]);
	while (s[++tab2[0]])
	{
		if (s[tab2[0]] != c)
			tab2[2]++;
		else
		{
			if (tab2[3] != (int)c && tab2[3] != 0)
			{
				if (!(tab[++tab2[1]] = malloc((tab2[2] + 1) * sizeof(char))))
					return (0);
				tab[tab2[1]][tab2[2]] = 0;
				tab2[2] = 0;
			}
		}
		tab2[3] = s[tab2[0]];
	}
	if (tab2[3] != (int)c && tab2[3] != 0)
		if (!(tab[++tab2[1]] = malloc(((tab2[2] + 1) * sizeof(char)))))
			return (0);
	if (tab2[3] != (int)c && tab2[3] != 0)
		tab[tab2[1]][tab2[2]] = 0;
	return (1);
}

static void	fill_words(char **tab, char const *s, char c)
{
	int		i;
	int		j;
	int		k;
	char	char_bef;

	char_bef = 0;
	i = 0;
	j = 0;
	k = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			tab[k][j] = s[i];
			j++;
		}
		if (s[i] == c && char_bef != c && char_bef != 0)
		{
			j = 0;
			k++;
		}
		char_bef = s[i];
		i++;
	}
}

static char **ft_split(char const *s, char c)
{
	char	**tab;
	int		nbr_words;
	int		i;

	if (s == 0)
		return (0);
	nbr_words = nbr_of_words(s, c);
	if (!(tab = malloc((nbr_words + 1) * sizeof(char *))))
		return (0);
	i = 0;
	while (i < nbr_words + 1)
		tab[i++] = 0;
	if (!alloc_words(tab, s, c))
	{
		i = 0;
		while (tab[i])
			free(tab[i++]);
		free(tab);
		return (0);
	}
	fill_words(tab, s, c);
	return (tab);
}

void    *ft_malloc(uint size, const char *error_message)
{
    void    *alloc;

    if ((alloc = malloc(size)) == NULL)
    {
        printf("%s", error_message);
        return (NULL);
    }
    return (alloc);
}

static char **get_columns_from_file(const char *dataset_path)
{
    char    **columns;
    FILE        *fp;
    char        *line = NULL;
    size_t      buffer_size = 0;

    if ((fp = fopen(dataset_path, "r")) == NULL)
    {
        printf("ERROR: df_from_csv: get_columns_from_file: fopen\n");
        return (NULL);
    }
    if (getline(&line, &buffer_size, fp) == -1)
    {
        printf("ERROR: df_from_csv: get_columns_from_file: getline\n");
        return (NULL);
    }
    if ((columns = ft_split(line, ',')) == NULL)
    {
        fclose(fp);
        free(line);
        printf("ERROR: df_from_csv: get_columns_from_file: ft_split\n");
        return (NULL);
    }
    fclose(fp);
    free(line);
    return (columns);
}

static uint array_len(char **array)
{
    uint len = 0;

    if (array == NULL)
        return (0);
    while (array[len] != NULL)
        ++len;
    return (len);
}

static char **array_copy(char **array)
{
    char    **copy;
    uint    len = array_len(array);
    
    if ((copy = ft_malloc((len + 1) * sizeof(char *), "ERROR: df_from_csv: copy_array: malloc\n")) == NULL)
        return (NULL);
    if (array != NULL)
    {
        for (uint i = 0; array[i] != NULL; ++i)
        {
            if ((copy[i] = strdup(array[i])) == NULL)
            {
                printf("ERROR: df_from_csv: copy_array: strdup\n");
                return (NULL);
            }
        }
    }
    copy[len] = NULL;
    return (copy);
}

static char **get_columns(const char *dataset_path, char **arg, bool are_columns_in_csv, uint nbr_of_columns)
{
    char    **columns;

    if (arg == NULL && are_columns_in_csv)
        return (get_columns_from_file(dataset_path));
    else if (arg == NULL)
    {
        if ((columns = ft_malloc((nbr_of_columns + 1) * sizeof(char *), "ERROR: df_from_csv: df_init: malloc\n")) == NULL)
            return (NULL);
        for (uint i = 0; i < nbr_of_columns; ++i)
        {
            if ((columns[i] = ft_malloc(sizeof(char), "ERROR: df_from_csv: df_init: malloc\n")) == NULL)
                return (NULL);
            columns[i][0] = '\0';
        }
        columns[nbr_of_columns] = NULL;
        return (columns);
    }
    return (array_copy(arg));
}

static uint *get_shape(const char *dataset_path, char **columns)
{
    uint        *shape;
    FILE        *fp;
    char        *line = NULL;
    size_t      buffer_size = 0;

    if ((shape = ft_malloc(2 * sizeof(uint), "ERROR: df_from_csv: df_init: malloc\n")) == NULL)
        return (NULL);
    if ((fp = fopen(dataset_path, "r")) == NULL)
    {
        printf("ERROR: df_from_csv: get_number_of_lines: fopen\n");
        return (NULL);
    }
    while (getline(&line, &buffer_size, fp) != -1)
        ++shape[0];
    fclose(fp);
    free(line);
    if (columns == NULL)
        --shape[0];
    shape[1] = array_len(columns);
    return (shape);
}

static double   **alloc_data(uint shape[2])
{
    double **data;

    if ((data = ft_malloc((shape[0] + 1) * sizeof(double *), "ERROR: df_from_csv: df_init: malloc\n")) == NULL)
        return (NULL);
    for (uint i = 0; i < shape[0]; ++i)
        if ((data[i] = ft_malloc(shape[1] * sizeof(double), "ERROR: df_from_csv: df_init: malloc\n")) == NULL)
            return (NULL);
    data[shape[0]] = NULL;
    return (data);
}

static int  fill_data(dataframe_t *df, const char *dataset_path, char **columns)
{
    FILE        *fp;
    char        *line = NULL;
    size_t      buffer_size = 0;
    uint        i = 0;
    char        **values;

    if ((fp = fopen(dataset_path, "r")) == NULL)
    {
        printf("ERROR: df_from_csv: fopen\n");
        return (-1);
    }
    if (columns == NULL)
    {
        getline(&line, &buffer_size, fp);
    }
    while (getline(&line, &buffer_size, fp) != -1)
    {
        if ((values = ft_split(line, ',')) == NULL)
        {
            printf("ERROR: df_from_csv: ft_split\n");
            return (-1);
        }
        for (uint j = 0; values[j] != NULL; ++j)
        {
            if ((df->data[i][j] = strtod(values[j], NULL)) == 0.0)
            {
                if (values[j][0] != '0')
                {
                    if (values[j][0] == 'M')
                        df->data[i][j] = 1.0;
                    else if (values[j][0] == 'B')
                        df->data[i][j] = 0.0;
                    else
                        printf("ERROR: df_from_csv: strtod: bad csv\n");
                }
                if (errno == ERANGE)
                    printf("ERROR: df_from_csv: strtod: out of range\n");
            }
            free(values[j]);
        }
        free(values);
        ++i;
    }
    fclose(fp);
    free(line);
    return (0);
}

dataframe_t *df_from_csv(const char *dataset_path, char **columns, bool are_columns_in_csv, uint nbr_of_columns)
{
    dataframe_t *df;

    if ((df = ft_malloc(sizeof(dataframe_t), "ERROR: df_from_csv: df_init: malloc\n")) == NULL)
        return (NULL);
    if ((df->columns = get_columns(dataset_path, columns, are_columns_in_csv, nbr_of_columns)) == NULL && df_free(df))
        return (NULL);
    if ((df->shape = get_shape(dataset_path, df->columns)) == NULL && df_free(df))
        return (NULL);
    if ((df->data = alloc_data(df->shape)) == NULL && df_free(df))
        return (NULL);
    if (fill_data(df, dataset_path, df->columns) == -1 && df_free(df))
        return (NULL);
    return (df);
}