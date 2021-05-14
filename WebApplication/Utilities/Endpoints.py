import datetime
from flask import request
import flask


def createMessageEndpoint(server, data_array_of_dicts):

    @server.route('/message', methods=['POST'])
    def req():
        json_data = request.json  # this is a dictionary!!

        date = json_data['date']
        payload_size = json_data['payload-size']
        content = json_data['content']

        if json_data['technology'] == "Sigfox":
            content = bytes.fromhex(content[:len(content) - 2]).decode('utf-8')
            date = datetime.datetime.fromtimestamp(int(json_data['date'])).strftime("%Y-%m-%d %H:%M:%S")
            payload_size = len(content)

        data_array_of_dicts.insert(0,
            {
                'Date': date,
                'Payload Size': payload_size,
                'Content': content,
                'type': json_data['type'],
                'technology': json_data['technology']
            })

        return flask.redirect(flask.request.url, code=200)
