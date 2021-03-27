import datetime
import os

import dash
import dash_core_components as dcc
import dash_html_components as html
import dash_table
import flask
import pandas as pd
from dash.dependencies import Output, Input
from flask import Flask, request
from flask_restful import Api

external_stylesheets = [
    {
        "href": "https://fonts.googleapis.com/css2?"
                "family=Lato:wght@400;700&display=swap",
        "rel": "stylesheet",
    },
]

data_array_of_dics = []

data = pd.read_csv("nbiottest.csv")
data["Date"] = pd.to_datetime(data["Date"], format="%Y-%m-%d")
data.sort_values("Date", inplace=True)

server = Flask('my_app')
ourapp = dash.Dash(server=server, external_stylesheets=external_stylesheets)
ourapp.title = "Sebastian & Magnus MSc Thesis"
api = Api(server)

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
                                    "label": 'NB-IoT',
                                    "value": 'NB-IoT'
                                }

                                # for technology in np.sort(
                                # data.technology.unique())
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
                                    "label": 'plaintext',
                                    "value": 'plaintext',
                                },
                                {
                                    "label": 'encryption',
                                    "value": 'encryption',
                                }
                                # for security_type in data.type.unique()

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
                    dash_table.DataTable(
                        id='table',
                        columns=[{"name": i, "id": i}
                                 for i in
                                 ["Date", "Payload Size", "Content", "type",
                                  "technology"]],
                        data=data_array_of_dics,
                        style_cell=dict(textAlign='left', padding='10px')
                    ),
                    className="card-listview",
                ),
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
                dcc.Interval(
                    id='interval-component',
                    interval=1 * 2000,  # in milliseconds
                ),
            ],
            className="wrapper",
        ),
    ]
)


@ourapp.callback(
    Output("date-range", "start_date"),
    Output("date-range", "end_date"),
    Output('table', 'data'),
    Input('interval-component', 'n_intervals'),
    Input('table', 'data')
)
def updateTable(n_intervals, data):
    try:
        start_date = data[0]['Date']
        end_data = data[-1]['Date']
        return start_date, end_data, data_array_of_dics
    except Exception as e:
        return datetime.datetime.now(), datetime.datetime.now(), \
               data_array_of_dics


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
            "title": {"text": "UDP Packets Received on Port XYZ", "x": 0.05,
                      "xanchor": "left"},
            "xaxis": {"fixedrange": True},
            "yaxis": {"fixedrange": True},
            "colorway": ["#E12D39"],
        },
    }
    return price_chart_figure, volume_chart_figure


@server.route('/message', methods=['POST'])
def req():
    global data
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
    data = data.append(dataFrame1, ignore_index=True)

    data["Date"] = pd.to_datetime(data["Date"], format="%Y-%m-%d")
    data.sort_values("Date", inplace=True)

    return flask.redirect(flask.request.url)


@server.route("/hello")
def hello():
    return {'hello': 'world'}


if __name__ == "__main__":
    ourapp.run_server(host='0.0.0.0', debug=True, port=8050)
