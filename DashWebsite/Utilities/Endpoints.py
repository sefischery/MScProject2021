from flask import request
import flask
import pandas as pd


def createMessageEndpoint(server, data_array_of_dics):

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

        return flask.redirect(flask.request.url)
