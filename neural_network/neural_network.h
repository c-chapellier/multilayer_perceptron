#ifndef NEURAL_NETWORK_H
# define NEURAL_NETWORK_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <stdbool.h>
# include "../dataframe/df.h"

# define uint unsigned int

# define SIGMOID_FUNCTION 0
# define RELU_FUNCTION 1
# define STEP_FUNCTION 2
# define TANH_FUNCTION 3

# define ACTIVATION_FUNCTION 0
# define NBR_OF_ITERATIONS 100
# define BATCH_SIZE 100
# define LEARNING_RATE 0.1
# define NBR_OF_INPUTS 2
# define NBR_OF_OUTPUTS 2
# define NBR_OF_HIDDEN_LAYERS 1
# define NBR_OF_NEURONS_PER_HIDDEN_LAYER 2

// # define DEBUG true
# define DEBUG false
# define PRINTF(f_, ...) (DEBUG) ? printf((f_), ##__VA_ARGS__) : 0

# define RED "[0;31m"
# define BLACK "[0;30m"
# define GREEN "[0;32m"
# define YELLOW "[0;33m"
# define BLUE "[0;34m"
# define PURPLE "[0;35m"
# define CYAN "[0;36m"
# define C_PRINTF(color, f_, ...) printf("\033") && printf(color) && printf((f_), ##__VA_ARGS__) && printf("\033[0m")

extern char         *columns[];

typedef struct      neural_network_s
{
    uint    nbr_of_hidden_layers;
    uint    nbr_of_neurons_per_hidden_layer;

    uint    nbr_of_inputs;
    uint    nbr_of_outputs;

    double  **first_hidden_layer_weights;
    double  ***hidden_layers_weights;
    double  **output_layer_weights;

    double  *first_hidden_layer_biases;
    double  **hidden_layers_biases;
    double  *output_layer_biases;

    double  **first_hidden_layer_weights_delta;
    double  ***hidden_layers_weights_delta;
    double  **output_layer_weights_delta;

    double  *first_hidden_layer_biases_delta;
    double  **hidden_layers_biases_delta;
    double  *output_layer_biases_delta;

    double  **z;
    double  **a;

    double  softmax_denominator;
}                   neural_network_t;

/* -----------   neural_network  ----------- */
neural_network_t    *neural_network_init();
neural_network_t    *neural_network_init_from_backup();
void                neural_network_feedforward(neural_network_t nn, double *inputs);
void                neural_network_train(neural_network_t nn, double **features, double **labels, double **features_test, double **labels_test);
void                neural_network_print(neural_network_t nn);
double              neural_network_test(neural_network_t nn, double **features, double **labels, bool print_info);
bool                neural_network_save(neural_network_t nn);
bool                neural_network_free(neural_network_t *nn, int flag);

/* -----------   array  ----------- */
void    fill_randomly_1d(double *to_fill, uint x);
void    fill_randomly_2d(double **to_fill, uint x, uint y);
void    fill_randomly_3d(double ***to_fill, uint x, uint y, uint z);
void    fill_of_zeros_1d(double *to_fill, uint x);
void    fill_of_zeros_2d(double **to_fill, uint x, uint y);
void    fill_of_zeros_3d(double ***to_fill, uint x, uint y, uint z);
bool    alloc_1d(double **to_alloc, uint x);
bool    alloc_2d(double ***to_alloc, uint x, uint y);
bool    alloc_3d(double ****to_alloc, uint x, uint y, uint z);
void    free_2d(double **to_free, uint x);
void    free_3d(double ***to_free, uint x, uint y);
void    copy_1d(double *dst, double *src, uint x);
void    copy_2d(double **dst, double **src, uint x, uint y);
void    copy_3d(double ***dst, double ***src, uint x, uint y, uint z);
void    print_1d(double *array, uint x);
void    print_2d(double **array, uint x, uint y);
void    print_3d(double ***array, uint x, uint y, uint z);
double  random_double();

/* -----------   activation_functions  ----------- */
double  softmax(double *outputs, double *inputs, uint nbr_of_inputs);
double  activation_function(double x);
double  softmax_derivative(double z, double denominator);
double  activation_function_derivative(double x);

/* -----------   formulas  ----------- */
double  dot(double *x, double *y, uint len);
double  cost(double *preds, double label, uint nbr_of_outputs);
void    normalize(double **features, uint len, double **base);

#endif