#include "neural_network.h"

void    neural_network_save(neural_network_t nn)
{
    dataframe_t *df;
    char        *tmp = NULL, str[20];

    if ((df = df_from_array(nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_inputs, nn.first_hidden_layer_weights)) == NULL)
        return ;
    df_to_csv(*df, "./neural_network/backup/first_hidden_layer_weights.csv");
    df_free(df);
    for (uint i = 0; i < nn.nbr_of_hidden_layers - 1; ++i)
    {
        if ((df = df_from_array(nn.nbr_of_neurons_per_hidden_layer, nn.nbr_of_neurons_per_hidden_layer, nn.hidden_layers_weights[i])) == NULL)
            return ;
        sprintf(str, "%d.csv", i);
        if ((tmp = strjoin("./neural_network/backup/hidden_layers_weights.csv", str)) == NULL)
        {
            df_free(df);
            return ;
        }
        df_to_csv(*df, tmp);
        df_free(df);
    }
    if ((df = df_from_array(nn.nbr_of_outputs, nn.nbr_of_neurons_per_hidden_layer, nn.output_layer_weights)) == NULL)
        return ;
    df_to_csv(*df, "./neural_network/backup/output_layer_weights.csv");
    df_free(df);

    if ((df = df_from_array(1, nn.nbr_of_neurons_per_hidden_layer, &nn.first_hidden_layer_biases)) == NULL)
        return ;
    df_to_csv(*df, "./neural_network/backup/first_hidden_layer_biases.csv");
    df_free(df);
    if ((df = df_from_array(nn.nbr_of_hidden_layers - 1, nn.nbr_of_neurons_per_hidden_layer, nn.hidden_layers_biases)) == NULL)
        return ;
    df_to_csv(*df, "./neural_network/backup/hidden_layers_biases.csv");
    df_free(df);
    if ((df = df_from_array(1, nn.nbr_of_outputs, &nn.output_layer_biases)) == NULL)
        return ;
    df_to_csv(*df, "./neural_network/backup/output_layer_biases.csv");
    df_free(df);
}