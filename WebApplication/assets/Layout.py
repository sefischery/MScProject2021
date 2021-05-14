import dash_core_components as dcc
import dash_html_components as html
import dash_table

from Utilities.Utilities import constructDataFrame


def defineHtmlLayout(webapp, data_list):
    data = constructDataFrame(data_list)

    return html.Div(
        children=[
            dcc.Location(id='url', refresh=False),
            html.Div(
                html.Div(
                    children=[
                        html.H1(
                            children=webapp.title, className="header-title"
                        ),
                        html.P(
                            children="IoT communication protocol backend",
                            className="header-description",
                        ),
                    ],
                    className="title-container"
                ),
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
                                        "label": 'MQTT',
                                        "value": 'MQTT'
                                    },
                                    {
                                        "label": 'NB-IoT',
                                        "value": 'NB-IoT'
                                    },
                                    {
                                        "label": 'Sigfox',
                                        "value": 'Sigfox'
                                    },
                                ],
                                value=None,
                                clearable=True,
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
                                value=None,
                                clearable=True,
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
                            data=data_list,
                            style_cell=dict(textAlign='left', padding='10px'),
                            style_table={'max-height': '300px', 'overflowY': 'auto'}
                        ),
                        className="card-listview",
                    ),
                    html.Div(
                        children=dcc.Graph(
                            id="content-chart",
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
