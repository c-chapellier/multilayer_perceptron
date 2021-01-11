#include "neural_network.h"

double  dot(double *x, double *y, uint len)
{
    double  ret = 0.0;

    for (uint i = 0; i < len; ++i)
    {
        ret += x[i] * y[i]; 
    }
    return (ret);
}

double  cost(double *preds, double label, uint nbr_of_outputs)
{
    double  loss;
    double  labels[nbr_of_outputs];

    labels[0] = label;
    loss = 0.0;
    for (uint i = 0; i < nbr_of_outputs; ++i)
    {
        loss += pow(labels[i] - preds[i], 2); 
    }
    return (loss);
}

void    normalize(double **features, uint len, double **base)
{
    double max, min;

    for (uint i = 0; i < len; ++i)
    {
        max = base[0][i], min = base[0][i];
        for (uint j = 0; base[j] != NULL; ++j)
        {
            if (base[j][i] > max)
                max = base[j][i];
            if (base[j][i] < min)
                min = base[j][i];
        }
        for (uint j = 0; features[j] != NULL; ++j)
        {
            features[j][i] = 2 * ((features[j][i] - min) / (max - min)) - 1;
        }
    }
}