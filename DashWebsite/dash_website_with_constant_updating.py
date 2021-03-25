import flask
import dash
import dash_html_components as html
import dash_core_components as dcc
from dash.dependencies import Output, Input
from flask import request

text_style = {
    'color': 'blue',
}

children_text = {
    'content' : "fuckdig",
}

server = flask.Flask(__name__)
app = dash.Dash(__name__, server=server)

app.layout = html.Div([
    html.Div(id='dummy', children=children_text['content'], style=text_style),
    dcc.Interval(
        id='interval-component',
        interval=1 * 1000,  # in milliseconds
    ),
    html.Div(id='none', style={'display': 'none'}),
])


@app.callback(Output('dummy', 'children'),
              [Input('interval-component', 'n_intervals')])
def timer(n_intervals):
    return children_text['content']


@server.route('/test', methods=['POST'])
def req():
    print(request.json)
    print('Request triggered!')  # For debugging purposes, prints to console

    if request.json['color'] == 'red':  # Toggle textcolor between red and blue
        text_style['color'] = 'red'
        children_text['content'] = "FUCKDETHERPIS"

    if request.json['color'] == 'gray':  # Toggle textcolor between red and blue
        text_style['color'] = 'gray'

    return flask.redirect(flask.request.url)


if __name__ == '__main__':
    app.run_server(debug=True)
