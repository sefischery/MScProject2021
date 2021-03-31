import datetime

from Utilities.Utilities import contructDataFrame
from dash.dependencies import Output, Input


def dataCallback(ourapp, data_array_of_dics):
    @ourapp.callback(
        Output("date-range", "start_date"),
        Output("date-range", "end_date"),
        Output('table', 'data'),
        Input('interval-component', 'n_intervals'),
    )
    def updateTable(n_intervals):
        if len(data_array_of_dics) > 0:
            start_date = data_array_of_dics[0]['Date']
            end_data = data_array_of_dics[-1]['Date']
            return start_date, end_data, data_array_of_dics
        else:
            return datetime.datetime.now(), \
                   datetime.datetime.now(), \
                   data_array_of_dics


def graphCallback(ourapp):
    @ourapp.callback(
        Output("content-chart", "figure"),
        Input("technology-filter", "value"),
        Input("type-filter", "value"),
        Input("date-range", "start_date"),
        Input("date-range", "end_date"),
        Input('table', 'data')
    )
    def update_charts(technology, security_type, start_date, end_date, dataInput):
        graph_data = contructDataFrame(dataInput)

        mask = (
                (graph_data.technology == technology)
                & (graph_data.type == security_type)
                & (graph_data.Date >= start_date)
                & (graph_data.Date <= end_date)
        )
        filtered_data = graph_data.loc[mask, :]

        content_chart_figure = {
            "data": [
                {
                    "x": filtered_data["Date"],
                    "y": filtered_data["Payload Size"],
                    "hovertemplate": filtered_data["Content"] + "<extra></extra>",
                    "mode": "markers",
                },
            ],
            "layout": {
                "title": {
                    "text": "Average Payload Size",
                    "x": 0.05,
                    "xanchor": "left",
                },
                "xaxis": {"fixedrange": True},
                "yaxis": {"fixedrange": True},
                "colorway": ["#17B897"],
            },
        }
        return content_chart_figure
