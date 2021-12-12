# ICT2101-2201-Project

## Lab Group P5-2
Groups Members
1. Eddie Tan DeJun (2002226) EddieTanDJ
2. Chow Wen Jun (2000530) Lyc4on
3. Chua Xue Ning Joey (2002230) 170joeychua
4. Ong Jia Yan, Celeste (2001882) potatopootie
5. Foo Yong Jian Daniel (2000864) DanielFYJ

## Install Python Venv and the Python Libs
1. Clone the repository
2. Open terminal in the cloned repository and run the script below
```sh
pip3 install virtualenv
cd src
python -m venv venv-win
venv-win\Scripts\Activate
pip install -r requirements.txt
```

## Notes
Ensure virtual environment (venv) is enabled before starting the project or installng dependencies. 
```venv-win\Scripts\Activate```. Ensure a .env file is present in the root folder with correct configurations to be able to connect to the database. The variables are as shown below.

## Development Workflow
```master```: Only codes that are ready to deploy will be merged here.
<br>
```dev```: This is where completed features are being merged

<br />

## Project Structure
```
<Project Root>
|── Docs/
|   |── Class Diagram
|   |   |── M2 Class Diagram Remove Web Portal Entity.png
|   |   |── M2 Class Diagram.png
|   |── Communication Diagram
|   |   |── Project Communication-Diagram-UC1.png
|   |   |── Project Communication-Diagram-UC8.png
|   |── Component Diagram
|   |   |── M2 Components Diagram.png
|   |── Sequence Diagram
|   |   |── Project Sequence Diagram UC1.png
|   |   |── Project Sequence Diagram UC1
|   |   |── Project Sequence Diagram UC2.png
|   |   |── Project Sequence Diagram UC2
|   |   |── Project Sequence Diagram UC3.png
|   |   |── Project Sequence Diagram UC3
|   |   |── Project Sequence Diagram UC4.png
|   |   |── Project Sequence Diagram UC4
|   |   |── Project Sequence Diagram UC5.png
|   |   |── Project Sequence Diagram UC5
|   |   |── Project Sequence Diagram UC6.png
|   |   |── Project Sequence Diagram UC6
|   |   |── Project Sequence Diagram UC7.png
|   |   |── Project Sequence Diagram UC7
|   |   |── Project Sequence Diagram UC8.png
|   |   |── Project Sequence Diagram UC8
|   |   |── Project Sequence Diagram UC9.png
|   |   |── Project Sequence Diagram UC9
|   |   |── Project Sequence Diagram UC10.png
|   |   |── Project Sequence Diagram UC10
|   |   |── Project Sequence Diagram UC11.png
|   |   |── Project Sequence Diagram UC11
|   |   |── Project Sequence Diagram UC12PART1.png
|   |   |── Project Sequence Diagram UC12PART1
|   |   |── Project Sequence Diagram UC12PART2.png
|   |   |── Project Sequence Diagram UC12PART2
|   |   |── Project Sequence Diagram UC13PART1.png
|   |   |── Project Sequence Diagram UC13PART1
|   |   |── Project Sequence Diagram UC13PART2.png
|   |   |── Project Sequence Diagram UC13PART2
|   |   |── Project Sequence Diagram UC14.png
|   |   |── Project Sequence Diagram UC14
|   |   |── Project Sequence Diagram UC15PART1.png
|   |   |── Project Sequence Diagram UC15PART1
|   |   |── Project Sequence Diagram UC15PART2.png
|   |   |── Project Sequence Diagram UC15PART2
|   |   |── Project Sequence Diagram UC15PART3.png
|   |   |── Project Sequence Diagram UC15PART3
|   |── System State Diagram
|   |   |── M2 System State Diagram without highlight use case.png
|   |   |── M2 System State Diagram.png
|   |── Use Case Diagram
|   |   |── Robotic Car
|   |   |   |── 2104_Car_UCD_Lucid.png
|   |   |   |── 2104_Car_UCD.png
|   |   |   |── 2104_Car_UCD.txt
|   |   |── Web Portal
|   |   |   |── Project Use Case Diagram.png
|   |   |   |── Project Use Case Diagram.txt
|   |── Burn Down Chart.pptx
├── Robotic Car
│   ├── ProjectFINAL
│   │   ├── .launches
|   |   |   |── ProjectFINAL.launch
│   │   ├── ccs
│   │   │   |── startup_msp432p401r_ccs.c
│   │   ├── Devices
│   │   │   |── ESP8266.c
│   │   │   |── ESP8266.h
│   │   │   |── MSPIO.c
│   │   │   |── MSPIO.h
│   │   ├── Hardware
│   │   │   |── CS_Driver.c
│   │   │   |── CS_Driver.h
│   │   │   |── GPIO_Driver.c
│   │   │   |── GPIO_Driver.h
│   │   │   |── UART_Driver.c
│   │   │   |── UART_Driver.h
│   │   ├── Motor
│   │   │   |── Motor_Driver.c
│   │   │   |── Motor_Driver.h
│   │   ├── Sensor
│   │   │   |── encoder.c
│   │   │   |── encoder.h
│   │   │   |── init_Ultrasonic.c
│   │   │   |── init_Ultrasonic.h
│   │   │   |── Linetracker.c
│   │   │   |── Linetracker.h
│   │   │   |── Ultrasonic.c
│   │   │   |── Ultrasonic.h
│   │   ├── Wifi
│   │   │   |── Wifi.c
│   │   │   |── Wifi.h
│   │   ├── .cssproject
│   │   ├── .cproject
│   │   ├── msp432p401r.cmd
│   │   ├── system_msp432p401r.c
│   │   └── main.c
├── src
│   ├── app
│   │   ├── base
│   │   │   ├── static
│   │   │   │   ├── assets
│   │   │   │   │   ├── css
│   │   │   │   │   |   ├── plugins
│   │   │   │   │   |   │   ├── animate.min.css
│   │   │   │   │   |   │   ├── bootstrap.min.css
│   │   │   │   │   |   │   ├── perfect-scrollbar.min.css
│   │   │   │   │   |   │   ├── prism.coy.css
│   │   │   │   │   |   ├── Dashboard.css
│   │   │   │   │   |   ├── Play.css
│   │   │   │   │   |   ├── Style.css
│   │   │   │   │   ├── js
│   │   │   │   │   │   ├── pages
│   │   │   │   │   │   │   ├── dashboard-main.js
│   │   │   │   │   │   │   ├── Play.js
│   │   │   │   │   │   ├── plugins
│   │   │   │   │   │   │   ├── apexchart.min.js
│   │   │   │   │   │   │   ├── bootstrap.min.js
│   │   │   │   │   │   │   ├── perfect-scrollbar.jquery.min.js
│   │   │   │   │   │   │   ├── prism.js
│   │   │   │   │   │   ├── p5.min.js
│   │   │   │   │   │   ├── p5.sound.min.js
│   │   │   │   │   │   ├── pcoded.js
│   │   │   │   │   │   ├── pcoded.min.js
│   │   │   │   │   │   ├── sketch.js
│   │   │   │   │   │   ├── sketch.test.js
│   │   │   │   │   │   ├── uikit.js
│   │   │   │   │   │   ├── uikit.min.js
│   │   │   │   │   │   ├── vendor-all.js
│   │   │   │   │   │   ├── vendor-all.min.js
│   │   │   │   │   │   ├── waves.min.js
│   │   │   │   │   ├── json
│   │   │   │   │   │   ├── map.txt
|   │   │   │   ├── template
|   │   │   │   │   ├── includes
|   │   │   │   │   │   ├── navigation.html
|   │   │   │   │   │   ├── scripts.html
|   │   │   │   │   │   ├── sidebar.html
|   │   │   │   │   ├── layouts
|   │   │   │   │   │   ├── base.html
|   │   │   ├── init.py
|   │   │   ├── forms.py
|   │   │   ├── modules.py
|   │   │   ├── routes.py
|   │   │   ├── util.py
│   │   ├── home
│   │   │   ├── templates
│   │   │   │   ├── Dashboard.html
│   │   │   │   ├── page-403.html
│   │   │   │   ├── page-404.html
│   │   │   │   ├── page-500.html
│   │   │   │   ├── Play.html
│   │   |   ├── init.py
│   │   |   ├── routes.py
│   │   ├── init.py
│   ├── .gitignore
│   ├── config.py
│   ├── Database.db
│   ├── requriements-mysql.txt
│   ├── requirements.txt
│   ├── run.py
│   ├── start_server.sh
├── .gitingore
├── README.md
├── WORKFLOW.md
```

## User Acceptance Test (UAT)
Please kindly refer to the [UAT Video](https://www.youtube.com/watch?v=ay578-uKb1Y) for the system test cases.
<br />
[![UAT video](https://j.gifs.com/vQ9nDm.gif)](https://www.youtube.com/watch?v=ay578-uKb1Y)  
<br />

## Start the Web Server
```sh
start_server.bat
```
## Whitebox Testing
1. install and initialize npm
```sh
npm install --save-dev jest
npm init -y
```
2. Change to "test": "jest" in package.json file.
3. run the test
```sh
npm run test  
```

Please kindly refer to the [WhiteBox Testing](https://www.youtube.com/watch?v=kV2nuk3d0X0) video. 
<br />
[![White Box video](https://j.gifs.com/WPjx1W.gif)](https://www.youtube.cyouom/watch?v=kV2nuk3d0X0)  


