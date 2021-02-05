#include "neural_network.h"

static bool                neural_network_alloc(neural_network_t *nn)
{
    if (alloc_2d(&(nn->first_hidden_layer_weights), nn->nbr_of_neurons_per_hidden_layer, nn->nbr_of_inputs) == false && neural_network_free(nn, 0))
        return (false);
    if (alloc_3d(&(nn->hidden_layers_weights), nn->nbr_of_hidden_layers - 1, nn->nbr_of_neurons_per_hidden_layer, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 1))
        return (false);
    if (alloc_2d(&(nn->output_layer_weights), nn->nbr_of_outputs, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 2))
        return (false);
    
    if (alloc_1d(&(nn->first_hidden_layer_biases), nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 3))
        return (false);
    if (alloc_2d(&(nn->hidden_layers_biases), nn->nbr_of_hidden_layers - 1, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 4))
        return (false);
    if (alloc_1d(&(nn->output_layer_biases), nn->nbr_of_outputs) == false && neural_network_free(nn, 5))
        return (false);

    if (alloc_2d(&(nn->first_hidden_layer_weights_delta), nn->nbr_of_neurons_per_hidden_layer, nn->nbr_of_inputs) == false && neural_network_free(nn, 6))
        return (false);
    if (alloc_3d(&(nn->hidden_layers_weights_delta), nn->nbr_of_hidden_layers - 1, nn->nbr_of_neurons_per_hidden_layer, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 7))
        return (false);
    if (alloc_2d(&(nn->output_layer_weights_delta), nn->nbr_of_outputs, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 8))
        return (false);

    if (alloc_1d(&(nn->first_hidden_layer_biases_delta), nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 9))
        return (false);
    if (alloc_2d(&(nn->hidden_layers_biases_delta), nn->nbr_of_hidden_layers - 1, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 10))
        return (false);
    if (alloc_1d(&(nn->output_layer_biases_delta), nn->nbr_of_outputs) == false && neural_network_free(nn, 11))
        return (false);

    if (alloc_2d(&(nn->z), nn->nbr_of_hidden_layers + 1, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 12))
        return (false);
    if (alloc_2d(&(nn->a), nn->nbr_of_hidden_layers + 1, nn->nbr_of_neurons_per_hidden_layer) == false && neural_network_free(nn, 13))
        return (false);
    return (true);
}

static void neural_network_fill(neural_network_t nn)
{
    fill_randomly_2d(nn.first_hidden_layer_weights, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs);
    fill_randomly_3d(nn.hidden_layers_weights, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer);
    fill_randomly_2d(nn.output_layer_weights, nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer);

    fill_randomly_1d(nn.first_hidden_layer_biases, nn.nbr_of_neurons_per_hidden_layer);
    fill_randomly_2d(nn.hidden_layers_biases, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer);
    fill_randomly_1d(nn.output_layer_biases, nn.nbr_of_outputs);

    fill_of_zeros_2d(nn.first_hidden_layer_weights_delta, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs);
    fill_of_zeros_3d(nn.hidden_layers_weights_delta, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.output_layer_weights_delta, nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer);

    fill_of_zeros_1d(nn.first_hidden_layer_biases_delta, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.hidden_layers_biases_delta, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_1d(nn.output_layer_biases_delta, nn.nbr_of_outputs);
    
    fill_of_zeros_2d(nn.z, nn.nbr_of_hidden_layers + 1, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.a, nn.nbr_of_hidden_layers + 1, nn.nbr_of_neurons_per_hidden_layer);
}

neural_network_t    *neural_network_init()
{
    neural_network_t    *nn;

    if ((nn = malloc(sizeof(neural_network_t))) == NULL)
        return (NULL);
    nn->nbr_of_hidden_layers = NBR_OF_HIDDEN_LAYERS;
    nn->nbr_of_neurons_per_hidden_layer = NBR_OF_NEURONS_PER_HIDDEN_LAYER;
    nn->nbr_of_inputs = NBR_OF_INPUTS;
    nn->nbr_of_outputs = NBR_OF_OUTPUTS;
    if (neural_network_alloc(nn) == false)
        return (NULL);
    neural_network_fill(*nn);
    return (nn);
}

static bool csv_to_array(const char *dataset_path, double **array, uint x, uint y)
{
    dataframe_t *df;

    if ((df = df_from_csv(dataset_path,  NULL, false, y)) == NULL)
        return (false);
    // df_print(*df);
    copy_2d(array, df->data, x, y);
    df_free(df);
    return (true);
}

static bool csv_to_1d_array(const char *dataset_path, double *array, uint x)
{
    dataframe_t *df;

    if ((df = df_from_csv(dataset_path,  NULL, false, x)) == NULL)
        return (false);
    // df_print(*df);
    copy_1d(array, df->data[0], x);
    df_free(df);
    return (true);
}

static bool neural_network_fill_from_backup(neural_network_t nn)
{
    char    str[100];

    if (csv_to_array("./neural_network/backup/first_hidden_layer_weights.csv", nn.first_hidden_layer_weights, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs) == false)
        return (false);
    for (uint i = 0; i < nn.nbr_of_hidden_layers - 1; ++i)
    {
        sprintf(str, "./neural_network/backup/hidden_layers_weights.csv_%d.csv", i);
        if (csv_to_array(str, nn.hidden_layers_weights[i], nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer) == false)
            return (false);
    }
    if (csv_to_array("./neural_network/backup/output_layer_weights.csv", nn.output_layer_weights, nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer) == false)
        return (false);
    if (csv_to_1d_array("./neural_network/backup/first_hidden_layer_biases.csv", nn.first_hidden_layer_biases, nn.nbr_of_neurons_per_hidden_layer) == false)
        return (false);
    if (csv_to_array("./neural_network/backup/hidden_layers_biases.csv", nn.hidden_layers_biases, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer) == false)
        return (false);
    if (csv_to_1d_array("./neural_network/backup/output_layer_biases.csv", nn.output_layer_biases, nn.nbr_of_outputs) == false)
        return (false);

    fill_of_zeros_2d(nn.first_hidden_layer_weights_delta, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs);
    fill_of_zeros_3d(nn.hidden_layers_weights_delta, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.output_layer_weights_delta, nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer);

    fill_of_zeros_1d(nn.first_hidden_layer_biases_delta, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.hidden_layers_biases_delta, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_1d(nn.output_layer_biases_delta, nn.nbr_of_outputs);
    
    fill_of_zeros_2d(nn.z, nn.nbr_of_hidden_layers + 1, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.a, nn.nbr_of_hidden_layers + 1, nn.nbr_of_neurons_per_hidden_layer);
    return (true);
}

neural_network_t    *neural_network_init_from_backup()
{
    neural_network_t    *nn;

    if ((nn = malloc(sizeof(neural_network_t))) == NULL)
        return (NULL);
    nn->nbr_of_hidden_layers = NBR_OF_HIDDEN_LAYERS;
    nn->nbr_of_neurons_per_hidden_layer = NBR_OF_NEURONS_PER_HIDDEN_LAYER;
    nn->nbr_of_inputs = NBR_OF_INPUTS;
    nn->nbr_of_outputs = NBR_OF_OUTPUTS;
    if (neural_network_alloc(nn) == false)
        return (NULL);
    if (neural_network_fill_from_backup(*nn) == false)
        return (NULL);
    return (nn);
}