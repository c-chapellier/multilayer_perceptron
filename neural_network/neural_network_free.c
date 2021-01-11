#include "neural_network.h"

bool    neural_network_free(neural_network_t *nn, int flag)
{
    if (flag > 0)
        free_2d(nn->first_hidden_layer_weights, nn->nbr_of_neurons_per_hidden_layer);
    if (flag > 1)
        free_3d(nn->hidden_layers_weights, nn->nbr_of_hidden_layers - 1, nn->nbr_of_neurons_per_hidden_layer);
    if (flag > 2)
        free_2d(nn->output_layer_weights, nn->nbr_of_outputs);

    if (flag > 3)
        free(nn->first_hidden_layer_biases);
    if (flag > 4)
        free_2d(nn->hidden_layers_biases, nn->nbr_of_hidden_layers - 1);
    if (flag > 5)
        free(nn->output_layer_biases);

    if (flag > 6)
        free_2d(nn->first_hidden_layer_weights_delta, nn->nbr_of_neurons_per_hidden_layer);
    if (flag > 7)
        free_3d(nn->hidden_layers_weights_delta, nn->nbr_of_hidden_layers - 1, nn->nbr_of_neurons_per_hidden_layer);
    if (flag > 8)
        free_2d(nn->output_layer_weights_delta, nn->nbr_of_outputs);

    if (flag > 9)
        free(nn->first_hidden_layer_biases_delta);
    if (flag > 10)
        free_2d(nn->hidden_layers_biases_delta, nn->nbr_of_hidden_layers - 1);
    if (flag > 11)
        free(nn->output_layer_biases_delta);

    if (flag > 12)
        free_2d(nn->z, nn->nbr_of_hidden_layers + 1);
    if (flag > 13)
        free_2d(nn->a, nn->nbr_of_hidden_layers + 1);
    free(nn);
    return (true);
}