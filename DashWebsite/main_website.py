import dash
import pandas as pd
from flask import Flask
from flask_restful import Api

from Utilities.Callbacks import callback_handler
from Utilities.Endpoints import createMessageEndpoint
from assets.Layout import defineHtmlLayout

pd.set_option("display.max_rows", None, "display.max_columns", None)

external_stylesheets = [
    {
        "href": "https://fonts.googleapis.com/css2?"
                "family=Lato:wght@400;700&display=swap",
        "rel": "stylesheet",
    },
]

data = []

""" Initial setup of web application """
server = Flask('my_app')
ourapp = dash.Dash(server=server, external_stylesheets=external_stylesheets)
ourapp.title = "Sebastian & Magnus MSc Thesis"
api = Api(server)

""" This method defined the html layout of the web application """
ourapp.layout = defineHtmlLayout(ourapp, data)

""" This method creates and handle the callback functionality of the web application """
callback_handler(ourapp, data)

""" This method creates the message endpoint, which is used for adding datas """
createMessageEndpoint(server, data)

if __name__ == "__main__":
    ourapp.run_server(host='0.0.0.0', debug=True, port=80)
