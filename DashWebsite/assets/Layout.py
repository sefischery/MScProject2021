import dash_core_components as dcc
import dash_html_components as html
import dash_table


def defineHtmlLayout(ourapp, data, data_array_of_dics):
    return html.Div(
        children=[
            dcc.Location(id='url', refresh=False),
            html.Div(
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
                                        "label": 'NB-IoT',
                                        "value": 'NB-IoT'
                                    }
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
