import sqlite3
from flask import Flask, render_template, request, url_for, flash, redirect, json
import requests 

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from matplotlib.dates import date2num
from dateutil import parser


def get_db_connection():
    conn = sqlite3.connect('database.db')
    conn.row_factory = sqlite3.Row
    return conn

app = Flask(__name__)
app.config['SECRET_KEY'] = 'my secret key'

@app.route('/')
def index():
    conn = get_db_connection()
    weatherdata = conn.execute('SELECT * FROM weatherdata ORDER BY created DESC LIMIT 10').fetchall()

    plotweatherdata = conn.execute('SELECT * FROM weatherdata').fetchall()
    conn.close()

    timenow = [] 
    temperature = []

    for row in plotweatherdata:
        timenow.append(parser.parse(row['created']))
        temperature.append(row['temperature'])

    
    # Convert datetime.datetime to float days since 0001-01-01 UTC.
    dates = [date2num(t) for t in timenow]


    fig = plt.figure()
    ax1 = fig.add_subplot(111)
    ax1.set_title("My environmental data")

    # Configure x-ticks
    ax1.xaxis.set_major_formatter(mdates.DateFormatter('%d.%m.%Y %H:%M'))

    # Plot temperature data on left Y axis
    ax1.set_ylabel("Temperature [°C]")
    ax1.plot_date(dates, temperature, '-', label="Temperature", color='r')

    plt.savefig("figure.png")


    return render_template('index.html', weatherdata=weatherdata)


@app.route('/postweatherdata', methods=['POST'])
def weatherdata():

    request_data = request.get_json()
    #print(request_data["ip"])
    #print(request_data["temperature"])


    ip = request_data['ip']
    temperature = request_data['temperature']
    
    if not ip or not temperature:
            
        flash('ip and temperature are required!')
 
    else:
        conn = get_db_connection()
        conn.execute('INSERT INTO weatherdata (ip, temperature) VALUES (?, ?)',
                         (ip, temperature))
        conn.commit()
        conn.close()
        return redirect(url_for('index'))

    return render_template('index.html')

newTemperatureChanged = 18

@app.route('/create', methods=('GET', 'POST'))
def create():
    if request.method == 'POST':
        newtemperature = request.form['newtemp']
        global newTemperatureChanged 
        newTemperatureChanged = newtemperature

        if not newtemperature:
            flash('temp is required!')

        else:
            data = { 'temperature': newtemperature }
            data = json.dumps(data)
            #y = json.loads(data)

            url = "http://192.168.43.83/update"

            response = requests.post(url, data)
            
            return redirect(url_for('create'))

    return render_template('create.html')


@app.route('/gettemperature')
def getTemperature():

    x = newTemperatureChanged

    data = { 'temperature': x }

    return data