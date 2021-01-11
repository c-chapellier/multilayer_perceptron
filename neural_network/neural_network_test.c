#include "neural_network.h"

double  neural_network_test(neural_network_t nn, double **features, double **labels, bool print_info)
{
    uint    size = 0;
    double  mean_cost = 0.0, error;

    for (uint i = 0; features[i] != NULL; ++i)
        ++size;
    for (uint i = 0; features[i] != NULL; ++i)
    {
        if (print_info)
        {
            printf("inputs =");
            for (uint j = 0; j < nn.nbr_of_inputs; ++j)
                printf(" %f", features[i][j]);
            printf("\n");
        }
        neural_network_feedforward(nn, features[i]);

        for (uint j = 0; j < nn.nbr_of_outputs; ++j)
        {
            // error = (labels[j][i] * log(nn.a[nn.nbr_of_hidden_layers][j])) + ((1 - labels[j][i]) * log(1 - nn.a[nn.nbr_of_hidden_layers][j]));
            error = sqrt(pow(nn.a[nn.nbr_of_hidden_layers][j] - labels[j][i], 2));
            if (print_info)
            {
                if (error > 0.1)
                    C_PRINTF(RED, "error(%f, %f) = %f\x1b[0m\n", nn.a[nn.nbr_of_hidden_layers][j], labels[j][i], error);
                else
                    C_PRINTF(GREEN, "error(%f, %f) = %f\x1b[0m\n", nn.a[nn.nbr_of_hidden_layers][j], labels[j][i], error);
            }
            mean_cost += error / (size * nn.nbr_of_outputs);
        }
    }
    if (print_info)
        printf("\033[0;35mmean_cost_test = %f\x1b[0m\n", mean_cost);
    return (mean_cost);
}