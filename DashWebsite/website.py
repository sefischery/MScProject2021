import dash
import dash_core_components as dcc
import dash_html_components as html
import dash_table
import pandas as pd

data = pd.read_csv("avocado.csv")
data = data.query("type == 'conventional' and region == 'Albany'")
data["Date"] = pd.to_datetime(data["Date"], format="%Y-%m-%d")
data.sort_values("Date", inplace=True)

app = dash.Dash(__name__)

app.layout = html.Div(
    children=[
        html.H1(children="Sebastian Fischer & Magnus Jakobsen", ),
        html.P(
            children="Number of NB-IoT messages received"
        ),
        dash_table.DataTable(
            id='table',
            columns=[{"name": i, "id": i}
                     for i in ["Source", "Payload"]],
            data=[{
                'Source': 2,
                'Payload': 3
            },
            ],
            style_cell=dict(textAlign='left'),
            style_header=dict(backgroundColor="paleturquoise"),
            style_data=dict(backgroundColor="lavender")
        )
    ]
)

if __name__ == "__main__":
    app.run_server(debug=True)
