import dash
import dash_core_components as dcc
import dash_html_components as html
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
            ],
            className="wrapper",
        ),
    ]
)


@ourapp.callback(
    Output("message-list", "children")
)
def update_charts(name, age):
    dataList.append({'name': name, 'age': age})
    return dataList


@ourapp.callback(
    Output("price-chart", "figure"),
    Output("volume-chart", "figure"),
    Input("technology-filter", "value"),
    Input("type-filter", "value"),
    Input("date-range", "start_date"),
    Input("date-range", "end_date"),
)
def update_charts(technology, security_type, start_date, end_date):
    mask = (
            (data.technology == technology)
            & (data.type == security_type)
            & (data.Date >= start_date)
            & (data.Date <= end_date)
    )
    filtered_data = data.loc[mask, :]
    price_chart_figure = {
        "data": [
            {
                "x": filtered_data["Date"],
                "y": filtered_data["Payload Size"],
                "type": "lines",
                "hovertemplate": "$%{y:.2f}<extra></extra>",
            },
        ],
        "layout": {
            "title": {
                "text": "Average Payload Size",
                "x": 0.05,
                "xanchor": "left",
            },
            "xaxis": {"fixedrange": True},
            "yaxis": {"tickprefix": "$", "fixedrange": True},
            "colorway": ["#17B897"],
        },
    }

    volume_chart_figure = {
        "data": [
            {
                "x": filtered_data["Date"],
                "y": filtered_data["Content"],
                "type": "lines",
            },
        ],
        "layout": {
            "title": {"text": "UDP Packets Received on Port XYZ", "x": 0.05, "xanchor": "left"},
            "xaxis": {"fixedrange": True},
            "yaxis": {"fixedrange": True},
            "colorway": ["#E12D39"],
        },
    }
    return price_chart_figure, volume_chart_figure


@server.route("/message", methods=['POST'])
def message():
    request_data = request.get_json()
    name = request_data['name']
    age = request_data['age']
    dataList.append({'name': name, 'age': age})
    return { 'name': name, 'age': age }


@server.route("/hello")
def hello():
    return {'hello': 'world'}


if __name__ == "__main__":
    ourapp.run_server(host='localhost', port=80, debug=True)
