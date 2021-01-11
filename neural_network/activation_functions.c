#include "neural_network.h"

double  softmax(double *outputs, double *inputs, uint nbr_of_inputs)
{
    double  denominator = 0.0;

    if (nbr_of_inputs == 1)
    {
        outputs[0] = inputs[0];
        return (1.0);
    }
    for (uint i = 0; i < nbr_of_inputs; ++i)
        denominator += exp(inputs[i]);
    if (denominator == 0.0)
    {
        denominator = 1.0;
        printf("ERROR: softmax: div by 0\n");
    }
    for (uint i = 0; i < nbr_of_inputs; ++i)
        outputs[i] = exp(inputs[i]) / denominator;
    return (denominator);
}

static double  sigmoid(double x)
{
    return (1 / (1 + exp(-x)));
}

static double  reLU(double x)
{
    if (x <= 0.0)
        return (0.0);
    return (x);
}

static double  step(double x)
{
    if (x <= 0.0)
        return (0.0);
    return (1.0);
}

double  activation_function(double x)
{
    if (ACTIVATION_FUNCTION == SIGMOID_FUNCTION)
        return (sigmoid(x));
    if (ACTIVATION_FUNCTION == RELU_FUNCTION)
        return (reLU(x));
    if (ACTIVATION_FUNCTION == STEP_FUNCTION)
        return (step(x));
    if (ACTIVATION_FUNCTION == TANH_FUNCTION)
        return (tanh(x));
    printf("ERROR: macro ACTIVATION_FUNCTION: bad value\n");
    return (0.0);
}

double          softmax_derivative(double z, double denominator)
{
    return ((z * (denominator - z)) / pow(denominator, 2));
}

static double  sigmoid_derivative(double x)
{
    return (sigmoid(x) * (1 - sigmoid(x)));
}

static double  reLU_derivative(double x)
{
    if (x <= 0.0)
        return (0.0);
    return (1.0);
}

static double  step_derivative(double x)
{
    (void)x;
    return (0.0);
}

static double  tanh_derivative(double x)
{
   return (1 / pow(cosh(x), 2));
}

double  activation_function_derivative(double x)
{
    if (ACTIVATION_FUNCTION == SIGMOID_FUNCTION)
        return (sigmoid_derivative(x));
    if (ACTIVATION_FUNCTION == RELU_FUNCTION)
        return (reLU_derivative(x));
    if (ACTIVATION_FUNCTION == STEP_FUNCTION)
        return (step_derivative(x));
    if (ACTIVATION_FUNCTION == TANH_FUNCTION)
        return (tanh_derivative(x));
    printf("ERROR: macro ACTIVATION_FUNCTION: bad value\n");
    return (0.0);
}
