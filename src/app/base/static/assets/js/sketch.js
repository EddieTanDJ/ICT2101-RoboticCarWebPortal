let GameCar;
let car;
var cols = 6;
var rows = 4;
var colors = [];
var rectwstars = [];
var star_obj = [];
var slice_index = 0;
var cmd = "";
var first_cmd = "";
var star_count = 4;
var game_map = [];
var map_string;

function setup() {
    var canvas = createCanvas(500, 450);
    canvas.parent('canvas');
    startGame();
    createRestartButton();
}

function preload(){
  car = loadImage ('/static/assets/images/car.png');
  star = loadImage ('/static/assets/images/star.png');
  map_string= loadStrings ('/static/assets/json/map.txt'); 
}

function draw() {
  clear();  
  draw2Darray();
  GenerateGameText();
  storeFirstCmd();
  GameCar.keyPressed(cmd[0]);
  GameCar.display();
  displayAllStars();
  EndGame(GameCar);
}
function displayAllStars() {
for (var x = 0; x < star_obj.length; x++) {
    star_obj[x].display();
  }
}

function storeFirstCmd(){
  cmd = retrieveQueue();
  if (first_cmd == "" && cmd[0] != "N") {
    first_cmd = cmd[0];
  }
}

function GenerateGameText() {
  textSize(20);
  fill(51);
  if (rectwstars.length != 0) {
    text("Star Counter : "+star_count, 0,20);
  } else {
    star_count = 0;
  }
  text('END', 20,50);
  fill(51);
  text('START', 410, 410);
}

function createRestartButton() {
  button = createButton('Restart Game');
  button.parent('canvas');
  let col = color('#4099ff');
  button.style('background-color', col);
  button.style('color', 'white');
  button.style('display', 'block');
  button.style('font-weight','400');
  button.style('padding', "0.375rem 0.95rem");
  button.style('border-color', '#4099ff');
  button.mousePressed(startGame);
}

class gameStar {
  constructor(x, y, col, row) {
    this.x = x;
    this.y = y;
    this.row = row;
    this.col = col;
  }
  display() {
    // fill("red");
    // image(star, this.x, this.y, 60, 50);
    image(star, this.x, this.y, 60, 50);
  }
}
class gameCar {
  constructor(width, height) {
    this.x = width;
    this.y = height;
    this.diameter = 20;
  }

  async keyPressed(cmd) {
      noLoop();
      await sleep(1000);
      if (cmd == "W" && first_cmd == "W") {
        this.move(0,-80);
      } else if (cmd == "A" && first_cmd == "A") {
        this.move(-80,0);
      } else if (cmd == "S" && first_cmd == "S") {
        this.move(0,80);;
      } else if (cmd == "D" && first_cmd == "D") {
        this.move(80,0);
      } else if (cmd == "R" && first_cmd == "*") {
        console.log(star_obj.length);
        // console.log(dist(GameCar.x, GameCar.y,star_obj[slice_index].x,star_obj[slice_index].y));
        if ((star_obj.length != 0) && (dist(GameCar.x, GameCar.y,star_obj[slice_index].x,star_obj[slice_index].y) < 85)) {
          star_obj.splice(slice_index,1);
          rectwstars.splice(slice_index,1);
          star_count -= 1;
        }
       
      } else if (cmd == "B") {
        first_cmd = "B";
      } else if (cmd == "*") {
        for (var x = 0; x < rectwstars.length; x++) {
          if (rectwstars[x][2] == 1) {
            if (dist(GameCar.x, GameCar.y, 10+rectwstars[x][0]*80, 70+rectwstars[x][1]*80) < 25) {
              slice_index = x;
              first_cmd = "*";
              break;
            }
          }
        }
      
      } else if (first_cmd == "*" || first_cmd == "B" ) {
        switch(cmd) {
          case "W":
            first_cmd = "W";
            this.move(0,-80);        
            break;
          case "A":
            first_cmd = "A";
            this.move(-80,0);
            break;
          case "D":
            first_cmd = "D";
            this.move(80,0);       
            break;
          case "S":
            first_cmd = "S";
            this.move(0,80);
            break;
        }
      } else {
        this.move(0,0);
      }
      loop();
  }
  move(x, y) {
    this.x += x;
    this.y += y;
    if(this.x < 0+(this.diameter/2)){
      this.x = 10;
    }
    if(this.y < 80+(this.diameter/2)){
      this.y = 90;
    }
    if(this.x > 420-(this.diameter/2)){
      this.x = 410;
    }
    if(this.y > 340-(this.diameter/2)){
      this.y = 330;
    }
  }
  
  display() {
    image(car, this.x, this.y, 60, 30);
  }
}

function make2Darray(cols, rows) {
  var arr = new Array(cols);
  for (var i = 0; i < arr.length; i++) {
    arr[i] = new Array(rows);
  }
  return arr;
}

function color2Darray(game_map) {
  for (var i = 0; i < cols; i++) {
    for (var j = 0; j < rows; j++) {
      colors[i][j] = 255;
    }
  }
  for(var i = 0; i < game_map.length; i++) {
    colors[game_map[i][0]][game_map[i][1]] = 0;
  }
}

function draw2Darray() {
  for (var i = 0; i < cols; i++) {
    for (var j = 0; j < rows; j++) {
      let x = i * 80;
      let y = 60 + (j * 80);
      fill(colors[i][j]);
      stroke(0)
      rect(x, y, 80, 80);
    }
  }
}

function retrieveQueue() {
  var result = "";
  $.ajax({
    type: "GET",
    async: false,
    url: "/api/commands/dequeue",
    success: function (data) {
      result = data;
    }
  });
  return result;
}

function sleep(millisecondsDuration)
{
  return new Promise((resolve) => {
    setTimeout(resolve, millisecondsDuration);
  })
}

function EndGame(GameCar) {
  if ((dist(GameCar.x, GameCar.y, 10, 70) < 25) && (star_count == 0)) {
    clear();
    background(255);
    text('Game Success', 150,180);
  }
}

function startGame() {
  clear();
  background(255);
  colors = make2Darray(cols, rows); 
  let i = 0;
  for (var x = 0; x < map_string[0].length; x+=3) {
    game_map.push(new Array(map_string[0][x],map_string[0][x+1],map_string[0][x+2]));
    i++;
  }
  console.log(game_map.length);
  color2Darray(game_map);
  i = 0;
  for (var x = 0; x < game_map.length; x++) {
    if (game_map[x][2] == 1) {
      star_obj[i] = new gameStar(10 + game_map[x][0] * 80, 70 + (game_map[x][1] * 80));
      i++;
      rectwstars.push(new Array(game_map[x][0],game_map[x][1],game_map[x][2]));
    }
  }
  GameCar = new gameCar(410, 330);
}

//testing
module.exports = make2Darray;

    