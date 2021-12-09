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

##.env
The .env file will have the following variables. 
```
DEBUG=True
SECRET_KEY=S3cr3t_K#Key
DB_ENGINE=postgresql
DB_NAME=appseed-flask
DB_HOST=localhost
DB_PORT=5432
DB_USERNAME=appseed
DB_PASS=pass
```
## Development Workflow
```master```: Only codes that are ready to deploy will be merged here.
<br>
```dev```: This is where completed features are being merged

<br />

## System Test Cases
[![Watch the video](https://img.youtube.com/vi/ay578-uKb1Y.jpg)](https://youtu.be/ay578-uKb1Y)
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
[![Watch the video](https://img.youtube.com/vi/kV2nuk3d0X0.jpg)](https://www.youtube.com/watch?v=kV2nuk3d0X0)


