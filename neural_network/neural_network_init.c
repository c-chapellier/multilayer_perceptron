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

static void neural_network_fill_from_backup(neural_network_t nn)
{
    dataframe_t *df;
    char        *tmp = NULL, str[20];

    if ((df = df_from_csv("./neural_network/backup/first_hidden_layer_weights.csv", NULL, false, nn.nbr_of_inputs)) == NULL)
        return ;
    df_print(*df);
    copy_2d(nn.first_hidden_layer_weights, df->data, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs);
    df_free(df);

    // fill_randomly_3d(nn.hidden_layers_weights, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer);
    for (uint i = 0; i < nn.nbr_of_hidden_layers - 1; ++i)
    {
        sprintf(str, "%d.csv", i);
        if ((tmp = strjoin("./neural_network/backup/hidden_layers_weights.csv", str)) == NULL)
            return ;
        if ((df = df_from_csv(tmp, NULL, false, nn.nbr_of_neurons_per_hidden_layer)) == NULL)
        {
            free(tmp);
            return ;
        }
        free(tmp);
        df_print(*df);
        copy_2d(nn.hidden_layers_weights[i], df->data, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer);
        df_free(df);
    }

    if ((df = df_from_csv("./neural_network/backup/output_layer_weights.csv", NULL, false, nn.nbr_of_neurons_per_hidden_layer)) == NULL)
        return ;
    df_print(*df);
    copy_2d(nn.output_layer_weights, df->data, nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer);
    df_free(df);
    

    if ((df = df_from_csv("./neural_network/backup/first_hidden_layer_biases.csv", NULL, false, nn.nbr_of_neurons_per_hidden_layer)) == NULL)
        return ;
    df_print(*df);
    copy_1d(nn.first_hidden_layer_biases, df->data[0], nn.nbr_of_neurons_per_hidden_layer);
    df_free(df);
    if ((df = df_from_csv("./neural_network/backup/hidden_layers_biases.csv", NULL, false, nn.nbr_of_neurons_per_hidden_layer)) == NULL)
        return ;
    df_print(*df);
    copy_2d(nn.hidden_layers_biases, df->data, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer);
    df_free(df);
    if ((df = df_from_csv("./neural_network/backup/output_layer_biases.csv", NULL, false, nn.nbr_of_outputs)) == NULL)
        return ;
    df_print(*df);
    copy_1d(nn.output_layer_biases, df->data[0], nn.nbr_of_outputs);
    df_free(df);

    fill_of_zeros_2d(nn.first_hidden_layer_weights_delta, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs);
    fill_of_zeros_3d(nn.hidden_layers_weights_delta, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.output_layer_weights_delta, nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer);

    fill_of_zeros_1d(nn.first_hidden_layer_biases_delta, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.hidden_layers_biases_delta, nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_1d(nn.output_layer_biases_delta, nn.nbr_of_outputs);
    
    fill_of_zeros_2d(nn.z, nn.nbr_of_hidden_layers + 1, nn.nbr_of_neurons_per_hidden_layer);
    fill_of_zeros_2d(nn.a, nn.nbr_of_hidden_layers + 1, nn.nbr_of_neurons_per_hidden_layer);
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
    neural_network_fill_from_backup(*nn);
    return (nn);
}