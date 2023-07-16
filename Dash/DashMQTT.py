import dash
from dash.dependencies import Output, Input, State
from dash import dcc
from dash import html
from dash import dash_table
import plotly.graph_objs as go
import serial
from collections import deque
import time
import paho.mqtt.client as mqtt
import json

dispense_values1 = deque(maxlen=20)
dispense_values2 = deque(maxlen=20)
dispense_values3 = deque(maxlen=20)

data_count = 0  # Contador global para los datos recibidos
read_n = 0
mensajes_debug = []

BROKER = '10.159.13.199 ' #IP 
PORT = 1883
TOPIC = 'datos_planta'
USERNAME = 'usuario_publicador'
PASSWORD = 'fuga_publicador'

# Crear cliente
client = mqtt.Client()
# Configurar usuario y contraseña
client.username_pw_set(USERNAME, PASSWORD)




debug = open('debug.txt', 'a')

def start_serial_connection():
    puerto_com = 'COM7'
    ser = None
    while ser is None:
        try:
            ser = serial.Serial(puerto_com, 9600, dsrdtr = False) 
            print(f"Datos recibidos: {data}")


        except Exception as e:
            print(f"Error en start_serial_connection: {e}")
            print(f"Reconectar Arduino en puerto {puerto_com}")
            time.sleep(1)  # Espera un segundo antes de intentar nuevamente
    return ser

# Variable global para la conexión serial
global ser
ser = start_serial_connection()


# Conexión MQTT
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n", rc)
client.on_connect = on_connect
client.connect(BROKER, PORT)
client.loop_start() # Para manejar la conexión en segundo plano




last_valid_data = []

def read_from_serial(): # Para los valores numéricos
    global ser
    global mensajes_debug  # Se almacenan las líneas que no son numéricas
    global last_valid_data  # Se almacena la última data válida aquí

    try:
        if ser is not None:
            line = ser.readline().decode('utf-8')
            
            # Comprobamos si la línea empieza con "Debug"
            if line.startswith("Debug"):
                # Si es así, añadimos la línea completa a mensajes_debug
                mensajes_debug.append(line)
                # Se limita el tamaño de la lista de mensajes de depuración a 15 elementos
                mensajes_debug = mensajes_debug[-15:]
                return last_valid_data
            else:
                # Si no, se intenta convertir cada elemento a float
                data = line.split(',')
                try:
                    cleaned_data = [float(item) for item in data]
                    last_valid_data = cleaned_data  # Se guarda la data válida como la última válida
                    # transforma los datos a un diccionario para el envío por mqtt
                    cleaned_data_dict = {
                        "tiempo": float(cleaned_data[0]),
                        "vol_descargas": float(cleaned_data[1]),
                        "vol_almacenamiento": float(cleaned_data[2]),
                        "vol_ventas1": float(cleaned_data[3]),
                        "vol_ventas2": float(cleaned_data[4]),
                        "vol_fugas": float(cleaned_data[5]),
                        "prox_vol_descarga": float(cleaned_data[6]),
                        "prox_vol_ventas1": float(cleaned_data[7]),
                        "prox_vol_ventas2": float(cleaned_data[8]),
                        "vol_acum_descargas": float(cleaned_data[9]),
                        "vol_acum_ventas1": float(cleaned_data[10]),
                        "vol_acum_ventas2": float(cleaned_data[11]),
                        "vol_acum_fugas": float(cleaned_data[12]),
                        "prox_inst_descarga": float(cleaned_data[13]),
                        "prox_inst_ventas1": float(cleaned_data[14]),
                        "prox_inst_ventas2": float(cleaned_data[15]),
                        "cantidad_descargas": float(cleaned_data[16]),
                        "cantidad_ventas1": float(cleaned_data[17]),
                        "cantidad_ventas2": float(cleaned_data[18]),
                        "altura_fuga": float(cleaned_data[19]),
                        "dimensiones_fuga": float(cleaned_data[20])
                    }
                    # enviar los datos a través de MQTT
                    client.publish(TOPIC, json.dumps(cleaned_data_dict))
                    return cleaned_data
                except ValueError:
                    print(f"Unable to convert data to float. Data: {data}")
                    return last_valid_data  # Si hay un error, devolvemos la última data válida
                
        else:
            print("Serial connection not established.")
            return last_valid_data  # Si no hay conexión, devolvemos la última data válida
    except Exception as e:
        print(f"Error en read_from_serial: {e}")
        return last_valid_data  # Si hay una excepción, devolvemos la última data válida
    
    finally:
        # Escribe en el archivo
        for mensaje in mensajes_debug:
            debug.write(mensaje)















def close_serial_connection(ser):
    try:
        if ser is not None:
            ser.close()
    except Exception as e:
        print(f"Error en close_serial_connection: {e}")







def ms_to_time(ms):
    seconds = (ms / 1000) % 60
    seconds = int(seconds)
    minutes = (ms / (1000 * 60)) % 60
    minutes = int(minutes)
    hours = (ms / (1000 * 60 * 60)) % 24

    return "%02d:%02d:%02d" % (hours, minutes, seconds)



app = dash.Dash(__name__)

tank_dimensions = [
    {"width": 100, "height": 125, "max_value": 33000},
    {"width": 200, "height": 300, "max_value": 125000},
    {"width": 50, "height": 100, "max_value": 2500},
    {"width": 50, "height": 100, "max_value": 2500},
    {"width": 50, "height": 100, "max_value": 2500},
]






app.layout = html.Div([

    dcc.Store(id='live-update-data'),

    dcc.Store(id='mensajes-debug-store'),

    dcc.Interval(
                id='interval-component',
                interval=1 * 500,  #500
                n_intervals=0
            ),

    dcc.Tabs(
        id='tabs-example',
        value='tab-1',
        children=[
            dcc.Tab(label='Monitorización', value='tab-1', style={'width': '250px', 'height': '15px', 'display': 'flex', 'justify-content': 'center', 'align-items': 'center'}),
            dcc.Tab(label='Mensajes de Debug', value='tab-2', style={'width': '250px', 'height': '15px', 'display': 'flex', 'justify-content': 'center', 'align-items': 'center'}),
    ],
    style={'height': '25px', 'width': '100%', 'display': 'flex', 'justify-content': 'flex-end', 'align-items': 'center', "fontFamily": "system-ui"}),
    
    html.Div(id='tabs-content-example')
])






@app.callback(Output('serial-output', 'children'),
              Input('message-store', 'data'))
def display_output(data):
    if data is None:
        return 'Esperando mensajes...'
    else:
        return '\n'.join(data)







@app.callback(Output('tabs-content-example', 'children'),
              Input('tabs-example', 'value'))
def render_content(tab):
    if tab == 'tab-1':
        return html.Div([
            
            # Fila arriba del todo: título a la izquierda.
            html.Div([
                html.H2('MONITORIZACIÓN DE LA PLANTA',
                    style={
                        "marginLeft": "100px", 
                        "fontFamily": "system-ui",
                        "fontSize": "40px" 
                    }),
            ]),
            
            # Fila debajo de la anterior: temporizador a la derecha.
            html.Div([
                html.H2(id='time',
                        
                        
                        style={
                            "position": "absolute",   # posicionar el elemento
                            "top": "75px",            # distancia desde la parte superior
                            "right": "600px",          # distancia desde la derecha
                            "border": "2px solid",    # define el estilo y ancho del borde
                            "padding": "10px",        # espacio interior del borde, para que el texto no toque el borde
                            "background": "#ffffff",  # color de fondo del cuadrado
                            "fontFamily": "system-ui", 
                            "fontSize": "30px",
                        }),
            ]),

            # Debajo de la fila anterior, dos columnas.
            html.Div([  
                # Columna izquierda: los dibujos de los 5 tanques
                html.Div([
                    html.Div(id='tanks', 
                        style={
                            "display": "flex", 
                            "flexDirection": "column", 
                            "alignItems": "center", 
                            "gap": "50px",
                            "marginTop": "75px"     #100px
                        }),

                html.Div(       #Interruptor de emergencia
                    html.Button(
                        'Emergencia',
                        id='emergencia',
                        n_clicks=0,
                        style={
                            'background-color': '#F51919',  # Rojo en hexadecimal
                            'border': '10px solid #FCEA33',  # Amarillo en hexadecimal
                            'box-shadow': '0px 0px 1px 3px black',  # Sombra de caja negra que actúa como un segundo borde
                            'color': 'white',

                            'padding': '10px',
                            'text-align': 'center',
                            'font-size': '25px',
                            'width': '300px',
                            'position': 'absolute',
                            'bottom': '50px',
                            'left': '250px'
                        }
                    )
                )
                    
                ], style={"flex": "0.6", "display": "flex", "flexDirection": "column"}),
                
                # Columna derecha: dividida a su vez en dos filas.
                html.Div([
                    # Fila superior: Tabla a la izquierda y gráfico de anillos a la derecha.
                    html.Div([
                        html.Div([
                            dash_table.DataTable(
                                id='table',
                                columns=[{"name": i, "id": i} for i in ["Depósito", "Prox. Dispensación", "Dispensaciones"]],
                                data=[{" ": "", "Prox. Dispensación": "Depósito", "Dispensaciones": ""} for _ in range(3)],  # placeholder 
                                style_cell={'textAlign': 'center', 'fontFamily': 'system-ui', 'fontSize': '17px', 'padding': '10px'},  
                                style_header={'font-size': '20px', 'fontWeight': 'bold'},
                                style_table={
                                    "width": "80%",
                                    "marginTop": "150px",
                                    "marginLeft": "100px",
                                },
                                
                                style_cell_conditional=[
                                    {'if': {'column_id': 'Depósito'},
                                    'font-size': '17px', 'fontWeight': 'bold'},
                                    {'if': {'column_id': 'Prox. Dispensación'},
                                    'font-size': '17px'},
                                    {'if': {'column_id': 'Dispensaciones'},
                                    'font-size': '17px'}
                                ]
                            )

                        ], style={"flex": "0.5"}), 
                        html.Div([
                            dcc.Graph(
                                id='ring-chart',
                                figure=go.Figure(
                                    data=[go.Pie(
                                        labels=['Label 1', 'Label 2', 'Label 3'],
                                        values=[10, 15, 7],
                                        hole=.3
                                    )],
                                    layout=go.Layout(
                                        title='Volumen acumulado',
                                        title_font=dict(size=20),
                                        title_x=1,  # Centra el título en el gráfico
                                        title_y=0.75,
                                        legend=dict(
                                            yanchor="bottom",
                                            y=0.5,
                                            xanchor="right",
                                            x=1.5
                                        )
                                    )
                                )
                            )

                        ], style={"flex": "0.4"})
                    ], style={"display": "flex"}),

                    # Fila inferior: Gráfico de descargas a la izquierda y Gráfico de Ventas a la derecha.
                    html.Div([
                        html.Div([
                            dcc.Graph(id='dispense-next-volumes-1')
                        ], style={"flex": "0.45"}),
                        html.Div([
                            dcc.Graph(id='dispense-next-volumes-2-3')
                        ], style={"flex": "0.45"})
                    ], style={"flex": "1", "display": "flex"})
                ], style={"flex": "1", "display": "flex", "flexDirection": "column"}),

            ], style={"display": "flex"}),

            # Fila abajo del todo: datos recibidos.
            html.Div([
                html.H2(id='data-count-display', style={"textAlign": "right", "fontSize": "10px"})
            ], style={"position": "absolute", "bottom": "0", "right": "0"})

            
       




        ],
        style={"fontFamily": "system-ui"})
    
    elif tab == 'tab-2':
        return html.Div([
            html.H2('Mensajes recibidos por comunicación serial',
                    style={
                        "marginLeft": "100px", 
                        "fontFamily": "system-ui",
                        "fontSize": "40px"  
                    }),

            #Aquí los mensajes de depuración
            
            html.Pre(id='debug-pantalla', style={"whiteSpace": "pre-wrap","fontSize": "25px",}),
        ])








# Variable global para almacenar los mensajes
global serial_messages
serial_messages = []






@app.callback(Output('live-update-data', 'data'),
              [Input('interval-component', 'n_intervals')])
def update_data(n):
    global data_count
    try:
        data = read_from_serial()
        data_count += 1
        print(f"Contador de datos: {data_count}")
        return data
    except Exception as e:
        print(f"Error en update_data: {e}")
        return []
    


@app.callback(Output('data-count-display', 'children'),
              [Input('interval-component', 'n_intervals')])
def update_data_count(n):
    global data_count
    return f'Datos recibidos: {data_count}'


@app.callback(Output('time', 'children'),
              [Input('live-update-data', 'data')])
def update_time(data):
    return "T = " + ms_to_time(data[0])




@app.callback(
    Output(component_id='mensajes-debug-store', component_property='data'),
    Input(component_id='interval-component', component_property='n_intervals')
)
def update_mensajes_store(n):
    global mensajes_debug
    return mensajes_debug

@app.callback(
    Output(component_id='debug-pantalla', component_property='children'),
    Input(component_id='mensajes-debug-store', component_property='data')
)
def update_debug(mensajes):
    if mensajes:
        return '\n'.join(mensajes)
    else:
        return "No hay mensajes de debug."















@app.callback(Output('tanks', 'children'),
              [Input('live-update-data', 'data')])
def update_tanks(data):
    tank_values = data[1:6]
    dashed_line_height = data[19]  
    value_1 = data[19]
    value_2 = data[20]  
    tanks = []
    for i, value in enumerate(tank_values):
        water_level = (value / tank_dimensions[i]["max_value"]) * tank_dimensions[i]["height"]
        children = [
            html.Div(
                style={
                    "position": "absolute",
                    "bottom": "0",
                    "left": "0",
                    "height": f"{water_level}px",
                    "width": "100%",
                    "backgroundColor": "#22E1DE",
                    "zIndex": "1"
                }
            ),
            html.Div(
                style={
                    "position": "absolute",
                    "height": f"{tank_dimensions[i]['height']}px",
                    "width": f"{tank_dimensions[i]['width']}px",
                    "backgroundImage": f"url(assets/tank{i + 1}.png)",
                    "backgroundSize": "contain",
                    "zIndex": "2"
                }
            ),
            html.P(f"{value/1000:.2f}",     #Volumen de cada tanque
                style={
                    "position": "absolute", 
                    "bottom": "5px", #bottom o top
                    "left": "0", 
                    "width": "100%", 
                    "text-align": "center", 
                    "margin": "0", 
                    "fontFamily": "system-ui",       #Para cambiar fuente
                    "font-size": "20px",
                    "zIndex": "3"
                })

        ]

        if i == 1:
            dashed_line_position = (dashed_line_height / tank_dimensions[i]["max_value"]) * tank_dimensions[i]["height"]
            dashed_line = html.Div(
                style={
                    "position": "absolute",
                    "bottom": f"{dashed_line_position}px",
                    "left": "0",
                    "height": "2px",
                    "width": "100%",
                    "borderTop": "2px dashed gray",
                    "zIndex": "1"
                }
            )
            children.append(dashed_line)

            text1 = html.P(f"Altura umbral: {value_1 / 1000 :.0f} l",
                        style={"position": "absolute",
                                "bottom": f"{dashed_line_position + 20}px",
                                "left": f"{tank_dimensions[i]['width'] + 10}px",
                                "margin": "0",
                                "zIndex": "3",
                                "white-space": "nowrap"})
            children.append(text1)

            text2 = html.P(f"Ritmo de fuga: {value_2} %",
                        style={"position": "absolute",
                                "bottom": f"{dashed_line_position}px",
                                "left": f"{tank_dimensions[i]['width'] + 10}px",
                                "margin": "0",
                                "zIndex": "3",
                                "white-space": "nowrap"})
            children.append(text2)


        tank = html.Div(
            style={
                "position": "relative",
                "height": f"{tank_dimensions[i]['height']}px",
                "width": f"{tank_dimensions[i]['width']}px",
            },
            children=children
        )
        tanks.append(tank)

    tanks_row_1 = html.Div(tanks[0:1],
                           style={"display": "flex", "justifyContent": "center", "gap": "20px"})
    tanks_row_2 = html.Div(tanks[1:2],
                           style={"display": "flex", "justifyContent": "center", "gap": "20px"})
    tanks_row_3 = html.Div(tanks[2:],
                           style={"display": "flex", "justifyContent": "center", "gap": "20px"})

    return [tanks_row_1, tanks_row_2, tanks_row_3]






@app.callback(Output('table', 'data'),
              [Input('live-update-data', 'data')])
def update_table(data):
    next_dispense_times = data[13:16]
    print(f"Datos para next_dispense_times: {next_dispense_times}")
    dispense_counts = data[16:19]
    print(f"Datos para dispense_counts: {dispense_counts}")

    row_titles = ["Descargas", "Ventas1", "Ventas2"]
    table_data = [{"Depósito": row_titles[i], "Prox. Dispensación": ms_to_time(next_dispense_times[i]), "Dispensaciones": dispense_counts[i]} for i in range(3)]

    return table_data






@app.callback(Output('ring-chart', 'figure'),
              [Input('live-update-data', 'data')])
def update_ring_chart(data):
    accumulated_volumes = data[9:13]
    print(f"Datos para accumulated_volumes: {accumulated_volumes}")

    total = sum(accumulated_volumes)

    labels = ["Descargas", "Ventas1", "Ventas2", "Fugas"]

    values = [volume / total for volume in accumulated_volumes]

    trace = go.Pie(
        labels=labels,
        values=accumulated_volumes,
        hole=.6,
        textinfo='value', #textinfo='label+value'
        marker=dict(colors=['#FF5050', '#00CCCC', '#00FFCC', '#006666']) # Colores
    )

    layout = go.Layout(
        title={'text': 'Volumen acumulado', 'font': {'size': 20}},
        title_x=1,  # Centra el título en el gráfico
        title_y=0.75,
        legend=dict(
            yanchor="bottom",
            y=0.5,
            xanchor="right",
            x=1.5
        ),

        paper_bgcolor="rgba(0,0,0,0)"         #Para que el fondo sea transparente
    )

    return {"data": [trace], "layout": layout}






@app.callback(Output('dispense-next-volumes-1', 'figure'),
              [Input('live-update-data', 'data')])
def update_next_dispense_volumes_1(data):
    next_volumes = data[6:9]
    print(f"Datos para next_volumes: {next_volumes}")


    dispense_values1.append(next_volumes[0])
    
    trace1 = go.Scatter(
        x=list(range(len(dispense_values1))),
        y=list(dispense_values1),
        name='Descargas',
        mode='lines+markers',
        line=dict(color='#FF5050')
    )

    layout = go.Layout(
        title={'text': 'Volúmenes de descarga', 'y':0.85, 'x':0.5, 'xanchor': 'center', 'yanchor': 'top'},
        title_font=dict(size=20),
    )

    return {"data": [trace1], "layout": layout}


@app.callback(Output('dispense-next-volumes-2-3', 'figure'),
              [Input('live-update-data', 'data')])
def update_next_dispense_volumes_2_3(data):
    next_volumes = data[6:9]
    print(f"Datos para next_volumes: {next_volumes}")

    dispense_values2.append(next_volumes[1])
    dispense_values3.append(next_volumes[2])

    trace2 = go.Scatter(
        x=list(range(len(dispense_values2))),
        y=list(dispense_values2),
        name='Ventas 1',
        mode='lines+markers',
        line=dict(color='#00CCCC')
    )

    trace3 = go.Scatter(
        x=list(range(len(dispense_values3))),
        y=list(dispense_values3),
        name='Ventas 2',
        mode='lines+markers',
        line=dict(color='#00FFCC')
    )

    layout = go.Layout(
        title={'text': 'Volúmenes de venta', 'y':0.85, 'x':0.5, 'xanchor': 'center', 'yanchor': 'top'},  
        title_font=dict(size=20),
    )

    return {"data": [trace2, trace3], "layout": layout}





@app.callback(
    Output('emergencia', 'style'),
    Input('emergencia', 'n_clicks'),
    State('emergencia', 'style')
)
def switch_color(n_clicks, style):
    if n_clicks % 2 == 0:  # si el número de clics es par
        style['background-color'] = '#F51919'  # cambiar a rojo
        style['border'] = '10px solid #FCEA33'
        style['box-shadow'] = '0px 0px 1px 3px black'
        style['color'] = 'white'
        Emer_Dash_status = 0

    else:  # si el número de clics es impar
        style['background-color'] = '#C11212'  # cambiar a rojo oscuro
        style['border'] = '10px solid #FCEA33'
        style['box-shadow'] = '0px 0px 15px 10px orange'
        style['color'] = '#F9F9F9'

        Emer_Dash_status = 1
    

    #ser.write(str(Emer_Dash_status).encode())   #Para enviarlo sin formato, sólo 0 o 1
    ser.write(("Emergencia_Dash: " + str(Emer_Dash_status)).encode())   #Para enviarlo con el formato Emergencia_Dash: 0

    return style










if __name__ == '__main__':
    try:
        app.run_server(debug=True)
    finally:
        close_serial_connection(ser)
        debug.close()
        client.loop_stop()  # Detener la conexión MQTT