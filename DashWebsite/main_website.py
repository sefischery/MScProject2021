import dash
import pandas as pd
from flask import Flask
from flask_restful import Api

from Utilities.Callbacks import graphCallback, dataCallback
from Utilities.Endpoints import createMessageEndpoint
from Utilities.Utilities import formatToList
from assets.Layout import defineHtmlLayout

pd.set_option("display.max_rows", None, "display.max_columns", None)

external_stylesheets = [
    {
        "href": "https://fonts.googleapis.com/css2?"
                "family=Lato:wght@400;700&display=swap",
        "rel": "stylesheet",
    },
]

data_array_of_dics = []

data = pd.DataFrame([], columns=["Date", "Payload Size", "Content", "type", "technology"])
data["Date"] = pd.to_datetime(data["Date"], format="%Y-%m-%d")
data.sort_values("Date", inplace=True)

server = Flask('my_app')
ourapp = dash.Dash(server=server, external_stylesheets=external_stylesheets)
ourapp.title = "Sebastian & Magnus MSc Thesis"
api = Api(server)

ourapp.layout = defineHtmlLayout(ourapp, data, formatToList(data))

dataCallback(ourapp, data_array_of_dics)
graphCallback(ourapp)

data = createMessageEndpoint(server, data_array_of_dics, data)

if __name__ == "__main__":
    ourapp.run_server(host='localhost', debug=True, port=8050)
