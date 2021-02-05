#include "neural_network.h"

static bool array_to_csv(uint x, uint y, double **array, char *filename)
{
    dataframe_t *df;

    if ((df = df_from_array(x, y, array)) == NULL)
        return (false);
    df_to_csv(*df, filename);
    df_free(df);
    return (true);
}

bool    neural_network_save(neural_network_t nn)
{
    char    str[100];

    if (array_to_csv(nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs, nn.first_hidden_layer_weights, "./neural_network/backup/first_hidden_layer_weights.csv") == false)
        return (false);
    for (uint i = 0; i < nn.nbr_of_hidden_layers - 1; ++i)
    {
        sprintf(str, "./neural_network/backup/hidden_layers_weights_%d.csv", i);
        if (array_to_csv(nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer, nn.hidden_layers_weights[i], str) == false)
            return (false);
    }
    if (array_to_csv(nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer, nn.output_layer_weights, "./neural_network/backup/output_layer_weights.csv") == false)
        return (false);
    if (array_to_csv(1, nn.nbr_of_neurons_per_hidden_layer, &nn.first_hidden_layer_biases, "./neural_network/backup/first_hidden_layer_biases.csv") == false)
        return (false);
    if (nn.nbr_of_hidden_layers > 1 && array_to_csv(nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer, nn.hidden_layers_biases, "./neural_network/backup/hidden_layers_biases.csv") == false)
        return (false);
    if (array_to_csv(1, nn.nbr_of_outputs, &nn.output_layer_biases, "./neural_network/backup/output_layer_biases.csv") == false)
        return (false);
    return (true);
}