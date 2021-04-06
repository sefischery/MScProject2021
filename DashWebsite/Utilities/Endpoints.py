import datetime

from flask import request
import flask
import pandas as pd


def createMessageEndpoint(server, data_array_of_dics):

    @server.route('/message', methods=['POST'])
    def req():
        json_data = request.json  # this is a dictionary!!

        date = json_data['date']
        payloadSize = json_data['payload-size']
        content = json_data['content']

        if json_data['technology'] == "Sigfox":
            date = datetime.datetime.fromtimestamp(int(json_data['date'])).strftime("%Y-%m-%d %H:%M:%S")
            payloadSize = len(json_data['content'])

        data_array_of_dics.append(
            {
                'Date': date,
                'Payload Size': payloadSize,
                'Content': content,
                'type': json_data['type'],
                'technology': json_data['technology']
            })

        return flask.redirect(flask.request.url, code=200)
