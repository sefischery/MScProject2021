from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import scipy.stats as st

result_path = str(Path.cwd().parents[0])
result_path_graphs = f"{result_path}\\GraphResults"
result_path_graphs_hist = f"{result_path_graphs}\\Histogram"
result_path_graphs_box = f"{result_path_graphs}\\Boxplot"
microseconds_label_str = "[µs]"


def unique(input_list):
    x = np.array(input_list)
    return np.unique(x)


def generate_histogram(deviceType, encryptionStandard, isEncryption, save):
    if isEncryption:
        read = pd.read_csv(f"{deviceType}/{encryptionStandard}-encryption.csv")
        operating_values = read[f'{encryptionStandard}-encryption'].values
    else:
        read = pd.read_csv(f"{deviceType}/{encryptionStandard}-decryption.csv")
        operating_values = read[f'{encryptionStandard}-decryption'].values

    fig, axe = plt.subplots(figsize=(9, 5))
    axe.hist(operating_values, histtype='stepfilled', alpha=0.5, color='#86bf91', zorder=2, rwidth=0.9)
    axe.set_xlabel(microseconds_label_str)
    axe.set_ylabel('Samples count')
    axe.grid(color='white')  # True, axis='y', alpha=0.2)
    axe.set_facecolor('#e6e8e7')

    # Get mean as standard deviation
    std = mpatches.Patch(color='#86bf91', label=f'std: {round(operating_values.std(), 4)}', alpha=0.5)
    mean = mpatches.Patch(color='r', label=f'mean: {round(operating_values.mean(), 2)}', alpha=0.3)
    axe.legend(handles=[std, mean])

    mean = operating_values.mean()

    # the problem comes here
    plt.axvline(mean, color='r', linestyle='dashed', linewidth=2, alpha=0.3)

    if isEncryption:
        naming = "encryption"
    else:
        naming = "decryption"

    if save:
        plt.savefig(
            f'{result_path_graphs_hist}\\{deviceType}-{encryptionStandard}-{naming}.pdf')


def box_plot_coloring(ax, data, edge_color, fill_color):
    bp = ax.boxplot(data, patch_artist=True)

    for element in ['boxes', 'whiskers', 'fliers', 'means', 'medians', 'caps']:
        plt.setp(bp[element], color=edge_color, alpha=0.75)

    for patch in bp['boxes']:
        patch.set(facecolor=fill_color)


def generate_boxplot(cryptographicOperation, deviceTypes, encryptionStandards, save):
    columns = []
    number_array = []
    namings = []
    counter = 0
    for device in deviceTypes:
        for index, standard in enumerate(encryptionStandards):
            counter += 1
            readEncryption = pd.read_csv(f"{device}/{standard}-{cryptographicOperation}.csv")
            columns.append(readEncryption[f'{standard}-{cryptographicOperation}'].values)
            number_array.append(counter)
            namings.append(f'{device}-{standard}')

    fig, axe = plt.subplots(figsize=(9, 5))
    axe.boxplot(columns)
    plt.xticks(number_array, namings, rotation=10)
    axe.set_ylabel(microseconds_label_str)
    axe.grid(color='white')  # True, axis='y', alpha=0.2)
    axe.set_facecolor('#e6e8e7')

    if save:
        plt.savefig(f'{result_path_graphs_box}\\one-in-all-{cryptographicOperation}.pdf')


def generate_boxplot2(cryptographicOperation, devices, standardTypes, save):
    algorithm = []
    for device in devices:
        containment_list = []
        for standard in standardTypes:
            read_encryption = pd.read_csv(f"{device}/{standard}-{cryptographicOperation}.csv")
            containment_list.append(read_encryption[f'{standard}-{cryptographicOperation}'].values)
        algorithm.append(containment_list)

    fig, axe = plt.subplots(figsize=(9, 5))
    for index, crypto in enumerate(algorithm):
        if index == 0:
            box_plot_coloring(axe, crypto, 'red', 'white')
        elif index == 1:
            box_plot_coloring(axe, crypto, 'blue', 'white')
        else:
            box_plot_coloring(axe, crypto, 'green', 'white')
    plt.xticks([1, 2, 3], ['ACORN', 'ASCON', 'AES'], rotation=10)
    axe.set_ylabel(microseconds_label_str)
    axe.grid(color='white')  # True, axis='y', alpha=0.2)
    axe.set_facecolor('#e6e8e7')
    uno = mpatches.Patch(color='red', label=f'Uno', alpha=0.3)
    esp8266 = mpatches.Patch(color='blue', label=f'ESP8266', alpha=0.3)
    esp32 = mpatches.Patch(color='green', label=f'ESP32', alpha=0.3)
    axe.legend(handles=[esp32, esp8266, uno])

    if save:
        plt.savefig(f'{result_path_graphs_box}\\comparable-boxplot-{cryptographicOperation}.pdf')


def generate_boxplot_individual(data, label, save):
    fig, axe = plt.subplots(figsize=(9, 5))
    box_plot_coloring(axe, data, 'black', 'white')
    plt.xticks([1], [label], rotation=10)
    axe.set_ylabel(microseconds_label_str)
    axe.grid(color='white')  # True, axis='y', alpha=0.2)
    axe.set_facecolor('#e6e8e7')
    if "Esp8266-acorn-decryption" == label:
        confidence_interval = (0.74, 0.74)
    elif "Esp8266-acorn-encryption" == label:
        confidence_interval = (0.72, 0.72)
    else:
        confidence_interval = st.t.interval(alpha=0.95, df=len(data) - 1, loc=np.mean(data), scale=st.sem(data))
    print(f'{label}: {confidence_interval}')
    firstnumber = confidence_interval[0]
    sencondnumber = confidence_interval[1]
    confidence = mpatches.Patch(label=f'Confidence: ({round(firstnumber, 5)},{round(sencondnumber, 5)})', alpha=0.3)
    axe.legend(handles=[confidence])

    if save:
        plt.savefig(f'{result_path_graphs_box}\\{label}-boxplot.pdf')


def triggerHistogram(display, save):
    devices = ['Uno', 'Esp8266', 'Esp32']
    encryptions = ['acorn', 'ascon', 'aes']

    for device in devices:
        for encryption in encryptions:
            generate_histogram(device, encryption, True, save)
            generate_histogram(device, encryption, False, save)

    if display:
        plt.show()


def triggerBoxplot1(cryptographicOperation, display, save):
    devices = ['Uno', 'Esp8266', 'Esp32']
    encryptions = ['acorn', 'ascon', 'aes']
    generate_boxplot(cryptographicOperation, devices, encryptions, save)

    if display:
        plt.show()


def triggerBoxplot2(cryptographicOperation, display, save):
    devices = ['Uno', 'Esp8266', 'Esp32']
    encryptions = ['acorn', 'ascon', 'aes']
    generate_boxplot2(cryptographicOperation, devices, encryptions, save)
    if display:
        plt.show()


def triggerBoxplot3(cryptographicOperation, display, save):
    devices = ['Uno', 'Esp8266', 'Esp32']
    encryptions = ['acorn', 'ascon', 'aes']
    for device in devices:
        for standard in encryptions:
            readEncryption = pd.read_csv(f"{device}/{standard}-{cryptographicOperation}.csv")
            data = readEncryption[f'{standard}-{cryptographicOperation}'].values
            label = f'{device}-{standard}-{cryptographicOperation}'
            generate_boxplot_individual(data, label, save)

    if display:
        plt.show()


if __name__ == '__main__':
    #triggerHistogram(display=False, save=True)
    triggerBoxplot1("encryption", display=False, save=True)
    triggerBoxplot2("encryption", display=False, save=True)
    triggerBoxplot3("encryption", display=False, save=True)
