import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import seaborn as sns


def unique(input_list):
    x = np.array(input_list)
    return np.unique(x)


def generate_graph(deviceType, encryptionStandard, isEncryption):
    if isEncryption:
        read = pd.read_csv(f"{deviceType}/{encryptionStandard}-encryption.csv")
        operatingValues = read[f'{encryptionStandard}-encryption'].values
    else:
        read = pd.read_csv(f"{deviceType}/{encryptionStandard}-decryption.csv")
        operatingValues = read[f'{encryptionStandard}-decryption'].values

    fig, axe = plt.subplots(figsize=(9,5))
    axe.hist(operatingValues, histtype='stepfilled', alpha=0.5, color='#86bf91', zorder=2, rwidth=0.9)
    axe.set_xlabel("µs")
    axe.set_ylabel('Samples count')
    axe.grid(True, axis='y', alpha=0.2)

    # Get mean as standard deviation
    std = mpatches.Patch(color='#86bf91', label=f'std: {round(operatingValues.std(), 4)}', alpha=0.5)
    mean = mpatches.Patch(color='r', label=f'mean: {round(operatingValues.mean(),2)}', alpha=0.3)
    axe.legend(handles=[std, mean])

    mean = operatingValues.mean()

    # the problem comes here
    plt.axvline(mean, color='r', linestyle='dashed', linewidth=2, alpha=0.3)

    if isEncryption:
        naming = "encryption"
    else:
        naming = "decryption"

    """ This is a specific Path to Magnus' Computer. For testing change it to your desired location """
    plt.savefig(f'C:\\Users\\glasd\\CLionProjects\\MScProject2021\\Results\\GraphResults\\Histogram/{deviceType}-{encryptionStandard}-{naming}.pdf')


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    devices = ['Uno', 'Esp8266', 'Esp32']
    encryptions = ['acorn', 'ascon', 'aes']

    for device in devices:
        for encryption in encryptions:
            generate_graph(device, encryption, True)
            generate_graph(device, encryption, False)
