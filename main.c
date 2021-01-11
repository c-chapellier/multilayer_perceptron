#include "header.h"

static int          follow(bool from_backup, bool make_backup);
static dataframe_t  *get_df(const char *filename);
static double       **get_labels(dataframe_t df);
static double       **get_features(dataframe_t df);
static bool         free_all(dataframe_t *df, double **labels, double **features, dataframe_t *df_test,  double **features_test, double **labels_test);

char                *columns[] = {"id","diagnosis","radius_mean","texture_mean","perimeter_mean","area_mean","smoothness_mean","compactness_mean","concavity_mean","concave points_mean","symmetry_mean","fractal_dimension_mean","radius_se","texture_se","perimeter_se","area_se","smoothness_se","compactness_se","concavity_se","concave points_se","symmetry_se","fractal_dimension_se","radius_worst","texture_worst","perimeter_worst","area_worst","smoothness_worst","compactness_worst","concavity_worst","concave points_worst","symmetry_worst","fractal_dimension_worst",NULL};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("usage: ./a.out [from_backup] [make_backup]\n");
        return (EXIT_FAILURE);
    }
    return (follow(strcmp(argv[1], "from_backup") == 0, strcmp(argv[2], "make_backup") == 0));
}

static int          follow(bool from_backup, bool make_backup)
{
    dataframe_t         *df, *df_test;
    neural_network_t    *nn;
    double              **labels, **labels_test;
    double              **features, **features_test;

    // srand(time(NULL));
    if ((df = get_df("./data/training_data.csv")) == NULL)
        return (EXIT_FAILURE);
    if ((labels = get_labels(*df)) == NULL && free_all(df, NULL, NULL, NULL, NULL, NULL))
        return (EXIT_FAILURE);
    if ((features = get_features(*df)) == NULL && free_all(df, labels, NULL, NULL, NULL, NULL))
        return (EXIT_FAILURE);

    if ((df_test = get_df("./data/testing_data.csv")) == NULL && free_all(df, labels, features, NULL, NULL, NULL))
        return (EXIT_FAILURE);
    if ((labels_test = get_labels(*df_test)) == NULL && free_all(df, labels, features, df_test, NULL, NULL))
        return (EXIT_FAILURE);
    if ((features_test = get_features(*df_test)) == NULL && free_all(df, labels, features, df_test, labels_test, NULL)) 
        return (EXIT_FAILURE);

    normalize(features_test, df_test->shape[1], features);
    normalize(features, df->shape[1], features);

    if (from_backup)
    {
        if ((nn = neural_network_init_from_backup()) == NULL && free_all(df, labels, features, df_test, labels_test, features_test))
            return (EXIT_FAILURE);
    }
    else
    {
        if ((nn = neural_network_init()) == NULL && free_all(df, labels, features, df_test, labels_test, features_test))
            return (EXIT_FAILURE);
    }

    neural_network_train(*nn, features, labels, features_test, labels_test);
    neural_network_print(*nn);
    neural_network_test(*nn, features_test, labels_test, true);
    
    if (make_backup)
        neural_network_save(*nn);

    neural_network_free(nn, 14);
    free_all(df, labels, features, df_test, labels_test, features_test);
    return (EXIT_SUCCESS);
}

static dataframe_t  *get_df(const char *filename)
{
    dataframe_t *df;

    if ((df = df_from_csv(filename, columns, false, 0)) == NULL)
        return (NULL);
    df_shuffle(*df);
    return (df);
}

static double   **get_labels(dataframe_t df)
{
    double  **labels;

    if ((labels = malloc(3 * sizeof(double *))) == NULL)
        return (NULL);
    labels[0] = df_get_column(df, "diagnosis");
    if ((labels[1] = malloc(df.shape[0] * sizeof(double))) == NULL)
        return (NULL);
    for (uint i = 0; i < df.shape[0]; ++i)
    {
        labels[1][i] = (labels[0][i] == 1.0) ? 0.0 : 1.0;
    }
    labels[2] = NULL;
    return (labels);
}

static double   **get_features(dataframe_t df)
{
    double  **features;

    df_drop_column(df, "diagnosis");
    df_drop_column(df, "id");
    df_drop_column(df, "texture_mean");
    features = df_get_data(df);
    return (features);
}

static bool free_all(dataframe_t *df, double **labels, double **features, dataframe_t *df_test,  double **features_test, double **labels_test)
{
    df_free(df), df_free(df_test);
    for (uint i = 0; labels[i] != NULL; ++i)
        free(labels[i]);
    for (uint i = 0; labels_test[i] != NULL; ++i)
        free(labels_test[i]);
    free(labels), free(labels_test);
    for (uint i = 0; features[i] != NULL; ++i)
        free(features[i]);
    for (uint i = 0; features_test[i] != NULL; ++i)
        free(features_test[i]);
    free(features), free(features_test);
    return (true);
}