import datetime
from Utilities.Utilities import constructDataFrame
from dash.dependencies import Output, Input


def callback_handler(webapp, data_list):
    dataCallback(webapp, data_list)
    graphCallback(webapp)


def dataCallback(webapp, data_list):
    @webapp.callback(
        Output("date-range", "start_date"),
        Output("date-range", "end_date"),
        Output('table', 'data'),
        Input('interval-component', 'n_intervals'),
        Input("technology-filter", "value"),
        Input("type-filter", "value"),
    )
    def updateTable(n_intervals, technology, type):
        if technology is None:
            filterByTechnology = data_list
        else:
            filterByTechnology = [datapoint for datapoint in data_list if datapoint['technology'] == technology]

        if type is None:
            returninglist = filterByTechnology
        else:
            returninglist = [datapoint for datapoint in filterByTechnology if datapoint['type'] == type]

        if len(data_list) > 0:
            start_date = data_list[0]['Date']
            end_data = data_list[-1]['Date']
            return start_date, end_data, returninglist
        else:
            return datetime.datetime.now(), \
                   datetime.datetime.now(), \
                   returninglist


def graphCallback(webapp):
    @webapp.callback(
        Output("content-chart", "figure"),
        Input("technology-filter", "value"),
        Input("type-filter", "value"),
        Input("date-range", "start_date"),
        Input("date-range", "end_date"),
        Input('table', 'data')
    )
    def update_charts(technology, security_type, start_date, end_date, data_input):
        graph_data = constructDataFrame(data_input)

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
