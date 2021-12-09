# -*- encoding: utf-8 -*-
# DELETE FROM Data WHERE dataID IN(1,2)
"""
Copyright (c) 2019 - present AppSeed.us
"""

from sqlite3.dbapi2 import Cursor
from flask.json import jsonify
from app.home import blueprint
from flask import render_template, redirect, url_for, request, Flask,request, make_response
from jinja2 import TemplateNotFound
import random
import json
from time import time
import sqlite3


def db_connection():
    conn = None
    try:
        conn = sqlite3.connect('Database.db')
    except sqlite3.error as e:
        print(e)
    return conn

@blueprint.route('/index')
def index():
    return render_template('play.html', segment='index')

@blueprint.route('/<template>')
def route_template(template):
    try:
        if not template.endswith( '.html' ):
            template += '.html'

        # Detect the current page
        segment = get_segment( request )

        # Serve the file (if exists) from app/templates/FILE.html
        return render_template( template, segment=segment )

    except TemplateNotFound:
        return render_template('page-404.html'), 404
    
    except:
        return render_template('page-500.html'), 500

# Helper - Extract current page name from request 
def get_segment( request ): 
    try:
        segment = request.path.split('/')[-1]
        if segment == '':
            segment = 'index'
        return segment    
    except:
        return None  

@blueprint.route('/getCommands', methods=["GET" , "POST"])
def getCommands():
    if request.method == "POST":
        commands = request.form['queue']
        # Remove the brackets "" and coma
        commands = commands.replace("[","")
        commands = commands.replace("]","")
        commands = commands.replace("\"","")
        commands = commands.replace(",","")
        return jsonify(commands)
    return jsonify("")

@blueprint.route('/submitCommands', methods=["GET", "POST"])
def submitQueue():
    if request.method == "GET":
        try:
            # Establish database Connection
            conn = sqlite3.connect('Database.db')
            c = conn.cursor()
        except:
            return "Fail to connect to database"
        try:
            #Get the queue from AJAX GET request
            queue = request.args.get('qCommands')
            for q in queue:
                #Insert data to DB here
                c.execute("INSERT INTO Queue(Commands) VALUES(?)", (q,))
                c.execute("INSERT INTO QueueCar(Commands) VALUES(?)", (q,))
            conn.commit()
            conn.close()
            return "Success"
        except:
            return "Fail to submit queue commands"
    return "Fail"

'''
This function is used to get the queue from the database, dequeue it and return it to the client to execute it for the virtual map
Input: None
Output: String of commands
'''
@blueprint.route('/api/commands/dequeue', methods=["GET", "POST"])
def dequeue():
    if request.method == "GET":
        # Establish database Connection
        try:
            conn = sqlite3.connect('Database.db')
            c = conn.cursor()
        except:
            return "Fail to connect to database"
        try:
            #Get the queue from AJAX GET request
            c.execute("SELECT commands FROM Queue ORDER BY QueueID ASC LIMIT 1")
            data = c.fetchone()
            print(data);
            if (data):
                #Remove the first element from the queue
                c.execute("DELETE FROM Queue WHERE QueueID = (SELECT QueueID FROM Queue ORDER BY QueueID ASC LIMIT 1)")
            conn.commit()
            conn.close()
            return data[0][0] + ''
        except:
            # flash("No commands in queue")
            # return render_template('page-500.html'), 500
            return "No commands in queue" + '\0'
    return "Fail"

'''
This function is used to get the queue from the database, dequeue it and return it to the client to execute it for the car
Input: None
Output
'''
@blueprint.route('/api/commands/dequeueCar', methods=["GET", "POST"])
def dequeueCar():
    if request.method == "GET":
        # Establish database Connection
        try:
            conn = sqlite3.connect('Database.db')
            c = conn.cursor()
        except:
            return "Fail to connect to database"
        try:
            #Get the queue from AJAX GET request
            c.execute("SELECT commands FROM QueueCar ORDER BY QueueID ASC LIMIT 1")
            data = c.fetchone()
            print(data)
            if (data):
                #Remove the first element from the queue
                c.execute("DELETE FROM QueueCar WHERE QueueID = (SELECT QueueID FROM QueueCar ORDER BY QueueID ASC LIMIT 1)")
            conn.commit()
            conn.close()
            return data[0][0] + ''
        except:
            # flash("No commands in queue")
            # return render_template('page-500.html'), 500
            return "No commands in queue" + '\0'
    return "Fail"

@blueprint.route('/api/commands/getFirstCommands', methods=["GET", "POST"])
def getFirstCommand():
    if request.method == "GET":
        # Establish database Connection
        conn = sqlite3.connect('Database.db')
        # conn.close()
        c = conn.cursor()
        try:
            #Get the queue from AJAX GET request
            c.execute("SELECT commands FROM QueueCar ORDER BY QueueID ASC LIMIT 1")
            data = c.fetchone()
            conn.close()
            #Indicate end of string
            return data[0][0] + '\0'
        except:
            # flash("No commands in queue")
            # return render_template('page-500.html'), 500
            return "No commands in queue" + '\0'
    return "Fail"

@blueprint.route('/Dashboard', methods=["GET", "POST"])
def main():
    return render_template('Dashboard.html')


count = 0 
@blueprint.route('/testdata', methods=["GET", "POST"])
def data():
    # This one is to 1 by 1 increment each run of the function
    # global count
    # count += 1
    # str(count)

    # Random value
    carid = random.randint(1, 4)
    # print(carid)
    if request.method == "GET":
        # Establish database Connection
        conn = sqlite3.connect('Database.db')
        # conn.close()
        c = conn.cursor()
        try:
            #Get the queue from AJAX GET request
            c.execute("SELECT commands FROM Queue ORDER BY QueueID ASC LIMIT 1")
            data = c.fetchone()
            conn.close()
            #Indicate end of string
            return "Commands:" + data[0][0] + '\0'
        except:
            # flash("No commands in queue")
            # return render_template('page-500.html'), 500
            return "No commands in queue" + '\0'
    return "Fail"
 

# Route for reciving feedback from ESP8266
@blueprint.route("/api/data/testConn", methods=['GET'])
def testConn():
    if request.method == 'GET':
        return 'Hello EcSP8266'

@blueprint.route('/retrieveGameMap', methods=["GET", "POST"])
def play():
    if request.method == "POST":
        # Establish database Connection
        # try:
        conn = sqlite3.connect('Database.db')
        c = conn.cursor()
        # except:
        #     return render_template('page-500.html'), 500
        # try:
        mid = request.form['mid']
        c.execute("SELECT map_string FROM GameMap WHERE map_id = ?", (mid,))
        data = c.fetchall()
        conn.close()
        print(data)
        print(data[0][0])
        with open('app/base/static/assets/json/map.txt', 'w') as outf:
            outf.write(str(data[0][0]))
        return redirect("play.html")
        # except:
        #     return render_template('page-500.html'), 500
    else:
        return render_template('play.html')

# Route for reciving feedback from ESP8266
@blueprint.route("/api/data/feedback", methods=['GET'])
def recieveData():
    if request.method == 'GET':
        # Get data from URL parameters
        feedback = request.args.get('feedback')
        print (feedback)
        if (feedback != None):
            #Store the data into the database
            try:
                # Establish database Connection
                conn = sqlite3.connect('Database.db')
                c = conn.cursor()
            except:
                return "Fail to connect to database"
            try:
                #Insert data to DB here
                c.execute("INSERT INTO Feedback(Data) VALUES (?)", (feedback,))
                conn.commit()
                conn.close()
                if (feedback ==  "obstacle" or feedback == "blackobstacle"): 
                    try:
                        # Establish database Connection
                        conn = sqlite3.connect('Database.db')
                        c = conn.cursor()
                    except:
                        return "Fail to connect to database"
                    try:
                        #Get the queue from AJAX GET request
                        c.execute("DELETE FROM Queue")
                        c.execute("DELETE FROM QueueCar")
                        conn.commit()
                        conn.close()
                        return "Success"
                    except:
                        # flash("No commands in queue")
                        # return render_template('page-500.html'), 500
                        return "No commands in queue" + '\0'
                commands = dequeueCar()
                print(commands)
                return commands
            except:
                return "Fail to store feedback into database"
        else:
            return "Fail to recieve feedback"
    return "Fail to connect to web portal"

# Route for check obstacle feedback from ESP8266
@blueprint.route("/checkFeedback", methods=['POST'])
def checkFeedback():
    if request.method == 'POST':
        #Store the data into the database
        try:
            # Establish database Connection
            conn = sqlite3.connect('Database.db')
            c = conn.cursor()
        except:
            return "Fail to connect to database"
        #Insert data to DB here
        try:  
            c.execute("SELECT Data FROM Feedback")
            feedback = c.fetchall()
            print(feedback)
            if (feedback):
                text = ''
                c.execute("DELETE FROM Feedback")
                conn.commit()
                conn.close()
                for f in feedback:
                    text += f[0]
                print(text)
                return text
            else:
                conn.close()
                return 'None'
        except:
            return "Fail to fetch data from the database"
    return "Fail to connect to web portal"
