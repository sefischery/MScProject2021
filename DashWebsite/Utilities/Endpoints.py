from flask import request
import flask
import pandas as pd


def createMessageEndpoint(server, data_array_of_dics, data_input):
    data_handler = pd.DataFrame([], columns=["Date", "Payload Size", "Content", "type", "technology"])

    @server.route('/message', methods=['POST'])
    def req():
        json_data = request.json  # this is a dictionary!!
        data_array_of_dics.append(
            {
                'Date': json_data['date'],
                'Payload Size': json_data['payload-size'],
                'Content': json_data['content'],
                'type': json_data['type'],
                'technology': json_data['technology']})

        dataFrame1 = pd.DataFrame(
            {
                'Date': [json_data['date']],
                'Payload Size': [json_data['payload-size']],
                'Content': [json_data['content']],
                'type': [json_data['type']],
                'technology': [json_data['technology']]
            }
        )
        data_handler = data_input.append(dataFrame1, ignore_index=True)

        data_handler["Date"] = pd.to_datetime(data_handler["Date"], format="%Y-%m-%d")
        data_handler.sort_values("Date", inplace=True)

        return flask.redirect(flask.request.url)

    return data_handler
