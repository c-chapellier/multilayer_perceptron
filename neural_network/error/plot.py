import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

if __name__ == "__main__":
    df = pd.read_csv("./neural_network/error/error.csv")
    plt.plot(np.arange(1, len(df) + 1), df, color='red', linewidth=5)
    plt.ylabel("error")
    plt.xlabel("iterations")
    plt.show()