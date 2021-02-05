#include "neural_network.h"

void                neural_network_update_weights(neural_network_t nn)
{
    for (uint i = 0; i < nn.nbr_of_outputs; ++i)
    {
        for (uint j = 0; j < nn.nbr_of_neurons_per_hidden_layer; ++j)
        {
            PRINTF("1 up weight %f -= %f\n", nn.output_layer_weights[i][j], LEARNING_RATE * nn.output_layer_weights_delta[i][j]);
            nn.output_layer_weights[i][j] -= LEARNING_RATE * nn.output_layer_weights_delta[i][j];
            nn.output_layer_weights_delta[i][j] = 0.0;
        }
        PRINTF("1 up bias %f -= %f\n", nn.output_layer_biases[i], LEARNING_RATE * nn.output_layer_biases_delta[i]);
        nn.output_layer_biases[i] -= LEARNING_RATE * nn.output_layer_biases_delta[i];
        nn.output_layer_biases_delta[i] = 0.0;
    }

    for (int i = nn.nbr_of_hidden_layers - 2; i >= 0; --i)
    {
        for (uint j = 0; j < nn.nbr_of_neurons_per_hidden_layer; ++j)
        {
            for (uint k = 0; k < nn.nbr_of_neurons_per_hidden_layer; ++k)
            {
                PRINTF("2 up weight %f -= %f\n", nn.hidden_layers_weights[i][j][k], LEARNING_RATE * nn.hidden_layers_weights_delta[i][j][k]);
                nn.hidden_layers_weights[i][j][k] -= LEARNING_RATE * nn.hidden_layers_weights_delta[i][j][k];
                nn.hidden_layers_weights_delta[i][j][k] = 0.0;
            }
            PRINTF("2 up bias %f -= %f\n", nn.hidden_layers_biases[i][j], LEARNING_RATE * nn.hidden_layers_biases_delta[i][j]);
            nn.hidden_layers_biases[i][j] -= LEARNING_RATE * nn.hidden_layers_biases_delta[i][j];
            nn.hidden_layers_biases_delta[i][j] = 0.0;
        }
    }

    for (uint i = 0; i < nn.nbr_of_neurons_per_hidden_layer; ++i)
    {
        for (uint j = 0; j < nn.nbr_of_inputs; ++j)
        {
            PRINTF("3 up weight %f -= %f\n", nn.first_hidden_layer_weights[i][j], LEARNING_RATE * nn.first_hidden_layer_weights_delta[i][j]);
            nn.first_hidden_layer_weights[i][j] -= LEARNING_RATE * nn.first_hidden_layer_weights_delta[i][j];
            nn.first_hidden_layer_weights_delta[i][j] = 0.0;
        }
        PRINTF("3 up bias %f -= %f\n", nn.first_hidden_layer_biases[i], LEARNING_RATE * nn.first_hidden_layer_biases_delta[i]);
        nn.first_hidden_layer_biases[i] -= LEARNING_RATE * nn.first_hidden_layer_biases_delta[i];
        nn.first_hidden_layer_biases_delta[i] = 0.0;
    }
}


void    neural_network_backpropagation(neural_network_t nn, double *inputs, double cost_d[])
{
    double  a_d;
    double  n_d[nn.nbr_of_hidden_layers][nn.nbr_of_neurons_per_hidden_layer];

    for (uint i = 0; i < nn.nbr_of_outputs; ++i)
    {
        a_d = activation_function_derivative(nn.z[nn.nbr_of_hidden_layers][i]);
        // a_d = softmax_derivative(nn.z[nn.nbr_of_hidden_layers][i], nn.softmax_denominator);
        PRINTF("[0] a = %f\n", a_d);
        for (uint j = 0; j < nn.nbr_of_neurons_per_hidden_layer; ++j)
        {
            PRINTF("[0] w = %f\n", nn.a[nn.nbr_of_hidden_layers - 1][j] * a_d * cost_d[i]);
            nn.output_layer_weights_delta[i][j] += nn.a[nn.nbr_of_hidden_layers - 1][j] * a_d * cost_d[i];
        }
        PRINTF("[0] b = %f\n", a_d * cost_d[i]);
        nn.output_layer_biases_delta[i] += a_d * cost_d[i];
    }
    for (uint i = 0; i < nn.nbr_of_neurons_per_hidden_layer; ++i)
    {
        double sum = 0.0;
         for (uint j = 0; j < nn.nbr_of_outputs; ++j)
        {
            PRINTF("[1] z = %f\n", nn.z[nn.nbr_of_hidden_layers][j]);
            a_d = activation_function_derivative(nn.z[nn.nbr_of_hidden_layers][j]);
            PRINTF("[1] a = %f\n", nn.output_layer_weights[j][i] * a_d * cost_d[j]);
            sum += nn.output_layer_weights[j][i] * a_d * cost_d[j];
        }
        PRINTF("[1] s = %f\n", sum);
        n_d[nn.nbr_of_hidden_layers - 1][i] = sum;
    }
    for (int h = nn.nbr_of_hidden_layers - 2; h >= 0; --h)
    {
        for (uint i = 0; i < nn.nbr_of_neurons_per_hidden_layer; ++i)
        {
            double sum = 0.0;
            for (uint j = 0; j < nn.nbr_of_neurons_per_hidden_layer; ++j)
            {
                a_d = activation_function_derivative(nn.z[h + 1][j]);
                PRINTF("[2] a = %f\n", nn.hidden_layers_weights[h][j][i] * a_d * n_d[h + 1][j]);
                sum += nn.hidden_layers_weights[h][j][i] * a_d * n_d[h + 1][j];
            }
            PRINTF("[2] s = %f\n", sum);
            n_d[h][i] = sum;
        }
    }
    for (int i = nn.nbr_of_hidden_layers - 1; i >= 1; --i)
    {
        for (uint j = 0; j < nn.nbr_of_neurons_per_hidden_layer; ++j)
        {
            a_d = activation_function_derivative(nn.z[i][j]);
            for (uint k = 0; k < nn.nbr_of_neurons_per_hidden_layer; ++k)
            {
                PRINTF("[3] a = %f\n", nn.a[i - 1][k] * a_d * n_d[i][j]);
                nn.hidden_layers_weights_delta[i - 1][j][k] += nn.a[i - 1][k] * a_d * n_d[i][j];
            }
            PRINTF("[3] s = %f\n", a_d * n_d[i][j]);
            nn.hidden_layers_biases_delta[i - 1][j] += a_d * n_d[i][j];
        }
    }
    for (uint i = 0; i < nn.nbr_of_neurons_per_hidden_layer; ++i)
    {
        a_d = activation_function_derivative(nn.z[0][i]);
        PRINTF("[4] z = %f\n", a_d);
        for (uint j = 0; j < nn.nbr_of_inputs; ++j)
        {
            PRINTF("[4] a = %f\n", inputs[j] * a_d * n_d[0][i]);
            nn.first_hidden_layer_weights_delta[i][j] += inputs[j] * a_d * n_d[0][i];
        }
        PRINTF("[4] s = %f\n", a_d * n_d[0][i]);
        nn.first_hidden_layer_biases_delta[i] += a_d * n_d[0][i];
    }
}

void    neural_network_feedforward(neural_network_t nn, double *inputs)
{
    for (uint i = 0; i < nn.nbr_of_neurons_per_hidden_layer; ++i)
    {
        nn.z[0][i] = dot(inputs, nn.first_hidden_layer_weights[i], nn.nbr_of_inputs) + nn.first_hidden_layer_biases[i];
        nn.a[0][i] = activation_function(nn.z[0][i]);
    }
    for (uint i = 1; i < nn.nbr_of_hidden_layers; ++i)
    {
        for (uint j = 0; j < nn.nbr_of_neurons_per_hidden_layer; ++j)
        {
            nn.z[i][j] = dot(nn.a[i - 1], nn.hidden_layers_weights[i - 1][j], nn.nbr_of_neurons_per_hidden_layer) + nn.hidden_layers_biases[i - 1][j];
            nn.a[i][j] = activation_function(nn.z[i][j]);
        }
    }
    for (uint i = 0; i < nn.nbr_of_outputs; ++i)
    {
        nn.z[nn.nbr_of_hidden_layers][i] = dot(nn.a[nn.nbr_of_hidden_layers - 1], nn.output_layer_weights[i], nn.nbr_of_neurons_per_hidden_layer) + nn.output_layer_biases[i];
        nn.a[nn.nbr_of_hidden_layers][i] = activation_function(nn.z[nn.nbr_of_hidden_layers][i]);
    }
    // nn.softmax_denominator = softmax(nn.a[nn.nbr_of_hidden_layers], nn.z[nn.nbr_of_hidden_layers], nn.nbr_of_outputs);
}

void    features_shuffle(double **features)
{
    uint    j, size = 0;
    double  *tmp;

    for (uint i = 0; features[i] != NULL; ++i)
        ++size;
    for (uint i = 0; features[i] != NULL; ++i)
    {
        j = i + rand() / (RAND_MAX / (size - i) + 1);
        tmp = features[j];
        features[j] = features[i];
        features[i] = tmp;
    }
}

void    write_cost(double cost[])
{
    FILE    *fptr;
    char    content[NBR_OF_ITERATIONS * 10] = {0};

    for (uint i = 0; i < NBR_OF_ITERATIONS; ++i)
    {
        sprintf(content, "%s%f\n", content, cost[i]);
    }
    if ((fptr = fopen("./neural_network/error/error.csv", "w")) == NULL)
    {
        printf("ERROR: write_cost: fopen\n");
        return ;
    }
    fprintf(fptr, "%s", content);
    fclose(fptr);
}

void    neural_network_train(neural_network_t nn, double **features, double **labels, double **features_test, double **labels_test)
{
    double  mean_cost, mean_cost_tmp, mean_cost_it[NBR_OF_ITERATIONS];
    double  cost_d[nn.nbr_of_outputs];
    uint    index;
    uint    size = 0;

    for (uint i = 0; features[i] != NULL; ++i)
        ++size;
    for (uint it = 0; it < NBR_OF_ITERATIONS; ++it)
    {
        mean_cost_it[it] = 0.0;

        // features_shuffle(features);
        for (uint i = 0; i < (size / BATCH_SIZE); ++i)
        {
            mean_cost = 0.0;
            for (uint j = 0; j < BATCH_SIZE; ++j)
            {
                index = (i * BATCH_SIZE) + j;
                // printf("inputs[%d] = %f\n", j, features[index][0]);
                neural_network_feedforward(nn, features[index]);

                mean_cost_tmp = 0.0;
                for (uint k = 0; k < nn.nbr_of_outputs; ++k)
                {
                    // printf("cost(%f, %f) = %f\n", nn.a[nn.nbr_of_hidden_layers][k], labels[k][index], nn.a[nn.nbr_of_hidden_layers][k] - labels[k][index]);
                    mean_cost_tmp += fabs(nn.a[nn.nbr_of_hidden_layers][k] - labels[k][index]) / nn.nbr_of_outputs;
                    cost_d[k] = 2 * (nn.a[nn.nbr_of_hidden_layers][k] - labels[k][index]);
                }
                // printf("mean_cost_tmp = %f\n", mean_cost_tmp);
                mean_cost += mean_cost_tmp / BATCH_SIZE;

                neural_network_backpropagation(nn, features[index], cost_d);
            }
            // printf("\x1b[31mmean_cost = %f \x1b[0m\n", mean_cost);
            // neural_network_print(nn);
            neural_network_update_weights(nn);
            mean_cost_it[it] += mean_cost / (size / BATCH_SIZE);
        }
        C_PRINTF(YELLOW, "epoch %u/%u - loss: %f - val_loss: %f\n", it + 1, NBR_OF_ITERATIONS, mean_cost_it[it], neural_network_test(nn, features_test, labels_test, false));
    }

    write_cost(mean_cost_it);
    // system("gnuplot -e \"plot 'error.csv' using 0:1 smooth bezier; pause -1\"");
}
