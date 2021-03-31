import pandas as pd


def formatToList(data):
    resultingList = []

    if data.values.any():
        return resultingList

    for item in data.values:
        resultingList.append({
            'Date': item[0],
            'Payload Size': item[1],
            'Content': item[2],
            'type': item[3],
            'technology': item[4]})

    return resultingList


def constructDataFrame(input_list):
    data = pd.DataFrame([], columns=["Date", "Payload Size", "Content", "type", "technology"])
    data["Date"] = pd.to_datetime(data["Date"], format="%Y-%m-%d")
    data.sort_values("Date", inplace=True)

    for item in input_list:
        dataFrame = pd.DataFrame(
            {
                'Date': [item['Date']],
                'Payload Size': [item['Payload Size']],
                'Content': [item['Content']],
                'type': [item['type']],
                'technology': [item['technology']]
            }
        )
        data = data.append(dataFrame, ignore_index=True)

    return data
