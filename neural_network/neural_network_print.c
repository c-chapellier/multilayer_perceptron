#include "neural_network.h"
#include <sys/ioctl.h>

static void header(struct winsize w, uint size)
{
    char *title[] = {
        "  /\\/\\  _   _| | |_(_) | __ _ _   _  ___ _ __    / _ \\___ _ __ ___ ___ _ __ | |_ _ __ ___  _ __  ",
        " /    \\| | | | | __| | |/ _` | | | |/ _ \\ '__|  / /_)/ _ \\ '__/ __/ _ \\ '_ \\| __| '__/ _ \\| '_ \\ ",
        "/ /\\/\\ \\ |_| | | |_| | | (_| | |_| |  __/ |    / ___/  __/ | | (_|  __/ |_) | |_| | | (_) | | | |",
        "\\/    \\/\\__,_|_|\\__|_|_|\\__,_|\\__, |\\___|_|    \\/    \\___|_|  \\___\\___| .__/ \\__|_|  \\___/|_| |_|",
        "                              |___/                                   |_|                        "
    };

    printf("/-------");
    for (uint i = 0; i < w.ws_col - 16; ++i)
        printf(" ");
    printf("-------\\");
    printf("\n");
    for (uint i = 0; i < 5; ++i)
    {
        printf("|");
        for (uint i = 0; i < (w.ws_col - 98) / 2; ++i)
            printf(" ");
        printf("%s", title[i]);
        for (uint i = 0; i < (w.ws_col - 100) / 2; ++i)
            printf(" ");
        printf("|\n");
    }
    for (uint i = 0; i < (w.ws_row - 13 - size) / 2; ++i)
        printf("\n");
    if ((w.ws_row - 13 - size) % 2 == 1)
        printf("\n");
}

static void footer(struct winsize w, uint size)
{
    for (uint i = 0; i < (w.ws_row - 13 - size) / 2; ++i)
        printf("\n");
    for (uint i = 0; i < 4; ++i)
    {
        printf("|");
        for (uint i = 0; i < w.ws_col - 2; ++i)
            printf(" ");
        printf("|\n");
    }
    printf("\\-------");
    for (uint i = 0; i < w.ws_col - 16; ++i)
        printf(" ");
    printf("-------/");
    printf("\n");
}

void    neural_network_print(neural_network_t nn)
{
    struct winsize  w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    header(w, 2 * nn.nbr_of_neurons_per_hidden_layer);

    for (uint i = 0; i < nn.nbr_of_neurons_per_hidden_layer; ++i)
    {
        for (uint j = 0; j < nn.nbr_of_inputs; ++j)
            C_PRINTF(CYAN, "\t%f", nn.first_hidden_layer_weights[i][j]);
        C_PRINTF(RED, "\t%f", nn.first_hidden_layer_biases[i]);
        printf("\t->");
        for (uint j = 0; j < nn.nbr_of_hidden_layers - 1; ++j)
        {
            for (uint k = 0; k < nn.nbr_of_neurons_per_hidden_layer; ++k)
                C_PRINTF(CYAN, "\t%f", nn.hidden_layers_weights[j][i][k]);
            C_PRINTF(BLUE, "\t%f", nn.hidden_layers_biases[j][i]);
            printf("\t->");
        }
        if (i < nn.nbr_of_outputs)
        {
            for (uint j = 0; j < nn.nbr_of_neurons_per_hidden_layer; ++j)
                C_PRINTF(CYAN, "\t%f", nn.output_layer_weights[i][j]);
            C_PRINTF(YELLOW, "\t%f", nn.output_layer_biases[i]);
        }
        printf("\n\n");
    }

    footer(w, 2 * nn.nbr_of_neurons_per_hidden_layer);
}