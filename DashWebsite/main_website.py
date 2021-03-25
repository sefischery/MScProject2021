import json

import dash
import dash_core_components as dcc
import dash_html_components as html
import dash_table
import flask
import pandas as pd
import numpy as np
from dash.dependencies import Output, Input
from flask import Flask, request
from flask_restful import Resource, Api

data = pd.read_csv("nbiottest.csv")
data["Date"] = pd.to_datetime(data["Date"], format="%Y-%m-%d")
data.sort_values("Date", inplace=True)

external_stylesheets = [
    {
        "href": "https://fonts.googleapis.com/css2?"
                "family=Lato:wght@400;700&display=swap",
        "rel": "stylesheet",
    },
]

data_array_of_dics = [{'Source': 2,
                       'Payload': 3}, {'Source': 2,
                                       'Payload': 3}]

server = Flask('my_app')
ourapp = dash.Dash(server=server, external_stylesheets=external_stylesheets)
ourapp.title = "Sebastian & Magnus MSc Thesis"
api = Api(server)

dataList = [{'name': "Magnus", 'age': 25}, {'name': "Sebastian", 'age': 24}]


def addMessageItem(name, age):
    return html.Div(
        children=[
            html.H2(children=name),
            html.P(children=age),
        ],
        className="message-item",
    )


ourapp.layout = html.Div(
    children=[
        dcc.Location(id='url', refresh=False),
        html.Div(
            children=[
                html.H1(
                    children=ourapp.title, className="header-title"
                ),
                html.P(
                    children="Showcase NB-IoT received messages",
                    className="header-description",
                ),
            ],
            className="header",
        ),
        html.Div(
            children=[
                html.Div(
                    children=[
                        html.Div(children="Technology", className="menu-title"),
                        dcc.Dropdown(
                            id="technology-filter",
                            options=[
                                {
                                    "label": technology,
                                    "value": technology
                                }

                                for technology in np.sort(data.technology.unique())
                            ],
                            value="NB-IoT",
                            clearable=False,
                            className="dropdown",
                        ),
                    ]
                ),
                html.Div(
                    children=[
                        html.Div(children="Type", className="menu-title"),
                        dcc.Dropdown(
                            id="type-filter",
                            options=[
                                {
                                    "label": security_type,
                                    "value": security_type
                                }

                                for security_type in data.type.unique()

                            ],
                            value="organic",
                            clearable=False,
                            searchable=False,
                            className="dropdown",
                        ),
                    ],
                ),
                html.Div(
                    children=[
                        html.Div(
                            children="Date Range", className="menu-title"
                        ),
                        dcc.DatePickerRange(
                            id="date-range",
                            min_date_allowed=data.Date.min().date(),
                            max_date_allowed=data.Date.max().date(),
                            start_date=data.Date.min().date(),
                            end_date=data.Date.max().date(),
                        ),
                    ]
                ),
            ],
            className="menu",
        ),
        html.Div(
            children=[
                html.Div(
                    children=dcc.Graph(
                        id="price-chart",
                        config={"displayModeBar": False},
                    ),
                    className="card",
                ),
                html.Div(
                    children=dcc.Graph(
                        id="volume-chart",
                        config={"displayModeBar": False},
                    ),
                    className="card",
                ),

                html.Div(
                    children=html.Div(

                        id="message-list",
                        children=[addMessageItem(message['name'], message['age']) for message in dataList],
                        className="message-list"
                    ),

                    className="card",
                ),
                dash_table.DataTable(
                    id='table',
                    columns=[{"name": i, "id": i}
                             for i in ["Source", "Payload"]],
                    data=data_array_of_dics,
                    style_cell=dict(textAlign='left'),
                    style_header=dict(backgroundColor="paleturquoise"),
                    style_data=dict(backgroundColor="lavender")
                ),
                dcc.Interval(
                    id='interval-component',
                    interval=1 * 1000,  # in milliseconds
                )
                ,
            ],
            className="wrapper",
        ),
    ]
)


@ourapp.callback(Output('table', 'data'),
                 [Input('interval-component', 'n_intervals')])
def updateTable(n_intervals):
    return data_array_of_dics


@server.route('/test', methods=['POST'])
def req():
    json_data = request.json  # this is a dictionary!!
    data_array_of_dics.append({'Source': json_data['Source'], 'Payload': json_data['Payload']})
    return flask.redirect(flask.request.url)


@server.route("/hello")
def hello():
    return {'hello': 'world'}


if __name__ == "__main__":
    ourapp.run_server(host='localhost', port=8050, debug=True)
