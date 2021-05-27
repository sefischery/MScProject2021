from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import scipy.stats as st

result_path = str(Path.cwd().parents[0])
print(result_path)
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
    #axe.hist(operating_values, histtype='stepfilled', alpha=0.5, color='#86bf91', zorder=2, rwidth=0.9)
    yAxisArray = [x for x in range(0, len(operating_values))]
    axe.scatter(operating_values, yAxisArray, color='#86bf91', marker='x')
    axe.set_xlabel(microseconds_label_str)
    axe.set_ylabel('Samples count')
    axe.grid(color='white')  # True, axis='y', alpha=0.2)
    axe.set_facecolor('#e6e8e7')

    # Get mean as standard deviation
    std = mpatches.Patch(color='b', label=f'std: {round(operating_values.std(), 4)}', alpha=0.5)
    mean = mpatches.Patch(color='r', label=f'mean: {round(operating_values.mean(), 4)}', alpha=0.3)
    axe.legend(handles=[std, mean])

    mean = operating_values.mean()
    std = operating_values.std()

    std_minus = mean - std
    std_plus = mean + std

    # the problem comes here
    plt.axvline(mean, color='r', linestyle='dashed', linewidth=2, alpha=0.3)
    if std > 0.00005:
        plt.axvline(std_minus, color='b', linestyle='dashed', linewidth=2, alpha=0.5)
        plt.axvline(std_plus, color='b', linestyle='dashed', linewidth=2, alpha=0.5)

    if isEncryption:
        naming = "encryption"
    else:
        naming = "decryption"

    if save:
        plt.savefig(
            f'{result_path_graphs_hist}\\{deviceType}-{encryptionStandard}-{naming}.pdf', bbox_inches="tight", pad_inches=0.1)


def box_plot_coloring(ax, data, positions, edge_color, fill_color):
    bp = ax.boxplot(data, positions=positions, patch_artist=True)

    for element in ['boxes', 'whiskers', 'fliers', 'means', 'medians', 'caps']:
        plt.setp(bp[element], color=edge_color, alpha=0.75)

    for patch in bp['boxes']:
        patch.set(facecolor=fill_color)


def generate_boxplot(cryptographicOperation, deviceTypes, encryptionStandards, save):
    columns = []
    number_array = []
    namings = []
    counter = 0
    fig, axe = plt.subplots(figsize=(9, 5))
    for deviceIndex, device in enumerate(deviceTypes):
        for index, standard in enumerate(encryptionStandards):
            counter += 1
            readEncryption = pd.read_csv(f"{device}/{standard}-{cryptographicOperation}.csv")
            columns.append(readEncryption[f'{standard}-{cryptographicOperation}'].values)
            number_array.append(counter)
            namings.append(f'{device}-{standard}')
    box_plot_coloring(axe, columns[:3], [1,2,3], "red", "white")
    box_plot_coloring(axe, columns[3:6], [4,5,6], "blue", "white")
    box_plot_coloring(axe, columns[6:9], [7,8,9], "green", "white")
    plt.xticks(number_array, namings, rotation=10)
    axe.set_ylabel(microseconds_label_str)
    axe.grid(color='white')  # True, axis='y', alpha=0.2)
    axe.set_facecolor('#e6e8e7')
    uno = mpatches.Patch(color='red', label=f'Uno', alpha=0.3)
    esp8266 = mpatches.Patch(color='blue', label=f'ESP8266', alpha=0.3)
    esp32 = mpatches.Patch(color='green', label=f'ESP32', alpha=0.3)
    axe.legend(handles=[esp32, esp8266, uno])

    if save:
        plt.savefig(f'{result_path_graphs_box}\\one-in-all-{cryptographicOperation}.pdf', bbox_inches="tight", pad_inches=0.1)


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
            box_plot_coloring(axe, crypto, [1, 2, 3], 'red', 'white')
        elif index == 1:
            box_plot_coloring(axe, crypto, [1, 2, 3], 'blue', 'white')
        else:
            box_plot_coloring(axe, crypto, [1, 2, 3], 'green', 'white')
    plt.xticks([1, 2, 3], ['ACORN', 'ASCON', 'AES'], rotation=10)
    axe.set_ylabel(microseconds_label_str)
    axe.grid(color='white')  # True, axis='y', alpha=0.2)
    axe.set_facecolor('#e6e8e7')
    uno = mpatches.Patch(color='red', label=f'Uno', alpha=0.3)
    esp8266 = mpatches.Patch(color='blue', label=f'ESP8266', alpha=0.3)
    esp32 = mpatches.Patch(color='green', label=f'ESP32', alpha=0.3)
    axe.legend(handles=[esp32, esp8266, uno])

    if save:
        plt.savefig(f'{result_path_graphs_box}\\comparable-boxplot-{cryptographicOperation}.pdf', bbox_inches="tight", pad_inches=0.1)


def generate_boxplot_individual(data, label, save):
    fig, axe = plt.subplots(figsize=(9, 5))
    box_plot_coloring(axe, data, [1], 'black', 'white')
    #plt.xticks([1], [label], rotation=10)
    plt.tick_params(
        axis='x',  # changes apply to the x-axis
        which='both',  # both major and minor ticks are affected
        bottom=False,  # ticks along the bottom edge are off
        top=False,  # ticks along the top edge are off
        labelbottom=False)  # labels along the bottom edge are off

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
        plt.savefig(f'{result_path_graphs_box}\\{label}-boxplot.pdf', bbox_inches="tight", pad_inches=0.1)


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
    triggerHistogram(display=False, save=True)
    #triggerBoxplot1("encryption", display=False, save=True)
    #triggerBoxplot1("decryption", display=True, save=True)
    #triggerBoxplot2("decryption", display=False, save=True)
    #triggerBoxplot3("encryption", display=False, save=True)
    #triggerBoxplot3("decryption", display=False, save=True)
