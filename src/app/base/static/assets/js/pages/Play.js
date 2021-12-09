/* This file is for 'queue commands' and 'robo car status' section in the play page*/

//#region Queue Commands
/* There are 2 queues created. 
One is used to insert to the basic table and 
the other is used to send the commands to the robot over the socket. */

let isBlackTileDetected = false;
let isObstacleDetected = false;
let movement = '';

// Create Queue in OOP using ES6
class Queue {
  constructor() {
    this.items = [];
  }
  enqueue(element) {
    this.items.push(element);
  }
  dequeue() {
    if (this.isEmpty()) return "Underflow";
    return this.items.shift();
  }
  front() {
    if (this.isEmpty()) return "No elements in Queue";
    return this.items[0];
  }
  isEmpty() {
    return this.items.length == 0;
  }

  clear() {
    return this.items = [];
  }

  removebyIndex(index) {
    return this.items.splice(index, 1);
  }

  convertToString() {
    return JSON.stringify(this.items);
  }

  getCommands(data) {
    var queue = data;
    //console.log(queue);
    $.ajax({
      type: "POST",
      url: "/getCommands",
      data: {
        queue: queue
      },
      success: function (commands) {
        for (var i = 0; i < commands.length; i++) {
          switch (commands[i]) {
            case "W":
              // console.log("up");
              $("#tableCommands").append("<tr><td>" + "<input type=\"checkbox\" aria-label=\"Checkbox to select following commands\" name=\"record\">" + "</td>" + "<td>" + "</td>" + "<td>" + "Forward" + "</td></tr>");
              addNo();
              break;
            case "S":
              // console.log("down");
              $("#tableCommands").append("<tr><td>" + "<input type=\"checkbox\" aria-label=\"Checkbox to select following commands\" name=\"record\">" + "</td>" + "<td>" + "</td>" + "<td>" + "Backward" + "</td></tr>");
              addNo();
              break;
            case "A":
              // console.log("left");
              $("#tableCommands").append("<tr><td>" + "<input type=\"checkbox\" aria-label=\"Checkbox to select following commands\" name=\"record\">" + "</td>" + "<td>" + "</td>" + "<td>" + "Left" + "</td></tr>");
              addNo();
              break;
            case "D":
              // console.log("right");
              $("#tableCommands").append("<tr><td>" + "<input type=\"checkbox\" aria-label=\"Checkbox to select following commands\" name=\"record\">" + "</td>" + "<td>" + "</td>" + "<td>" + "Right" + "</td></tr>");
              addNo();
              break;
            case "R":
              // console.log("rotate");
              $("#tableCommands").append("<tr><td>" + "<input type=\"checkbox\" aria-label=\"Checkbox to select following commands\" name=\"record\">" + "</td>" + "<td>" + "</td>" + "<td>" + "Rotate" + "</td></tr>");
              addNo();
              break;
            case "B":
              // console.log("black_tiles");
              $("#tableCommands").append("<tr><td>" + "<input type=\"checkbox\" aria-label=\"Checkbox to select following commands\" name=\"record\">" + "</td>" + "<td>" + "</td>" + "<td>" + "When On black tiles" + "</td></tr>");
              addNo();
              break;
            case "*":
              // console.log("star");
              $("#tableCommands").append("<tr><td>" + "<input type=\"checkbox\" aria-label=\"Checkbox to select following commands\" name=\"record\">" + "</td>" + "<td>" + "</td>" + "<td>" + "When " + "<i class=\"fas fa-star\"></i> " + " On Black Tiles" + "</td></tr>");
              addNo();
              break;
            default:
              break;
          }
          //$("#tableCommands").append("<tr><td>" +  +"</td>" +"<td>" + row + "</td>" +  "<td>" + commands[i] + "</td></tr>");
        }
      }
    });
  }
}

// This queue is for the table
let q = new Queue();
// This queue is to send the commands to the car
let qCommands = new Queue();

// Enable click function for all buttons when document is ready.
$(document).ready(function () {
  $("#up").click(up);
  $("#down").click(down);
  $("#left").click(left);
  $("#right").click(right);
  $("#rotate").click(rotate);
  $("#black_tiles").click(black_tiles);
  $("#stars").click(star);
  $("#stop").click(stop);
  $("#cfmSubmitDelete").click(deleteQueue);
  $("#cfmSubmitDeleteAll").click(deleteAllQueue);
  $("#cfmSubmit").click(submitQueue);
  checkFeedback();
  display();
  checkFirstCommand();
  setInterval(function () { checkFeedback() }, 10000);
  setInterval(function () { display() }, 10000);
  setInterval(function () { checkFirstCommand() }, 10000);

});

//When up button is click, store W in queue
function up() {
  q.enqueue("W");
  qCommands.enqueue("W");
  data = q.convertToString();
  q.getCommands(data);
  console.log(qCommands.front);
  q.clear();
};

// when down button is click, store S in queue
function down() {
  // console.log("down");
  q.enqueue("S");
  qCommands.enqueue("S");
  data = q.convertToString();
  q.getCommands(data);
  q.clear();
};

// when left button is click, store A in queue
function left() {
  // console.log("left");
  q.enqueue("A");
  qCommands.enqueue("A");
  // console.log(q.commands);
  data = q.convertToString();
  q.getCommands(data);
  q.clear();
};

// when right button is click, store D in queue
function right() {
  // console.log("right");
  q.enqueue("D");
  qCommands.enqueue("D");
  // console.log(q.commands);
  data = q.convertToString();
  q.getCommands(data);
  q.clear();
};

// when rotate button is click, store R in queue
function rotate() {
  // console.log("rotate");
  q.enqueue("R");
  qCommands.enqueue("R");
  // console.log(q.commands);
  data = q.convertToString();
  q.getCommands(data);
  q.clear();
};

// When black tiles button is click, store B in queue
function black_tiles() {
  q.enqueue("B");
  qCommands.enqueue("B");
  data = q.convertToString();
  q.getCommands(data);
  q.clear();
}

// When star is click, store * in queue
function star() {
  q.enqueue("*");
  qCommands.enqueue("*");
  data = q.convertToString();
  q.getCommands(data);
  q.clear();
}

/* TODO: Submit the stop command directly to the server */
function stop() {
  qCommands.clear();
  qCommands.enqueue("S");
  submitQueue();
}

// Add serial number to table
function addNo() {
  var table = document.getElementById('tableCommands');
  for (var i = 1, row; row = table.rows[i]; i++) {
    row.cells[1].innerHTML = i;
  }
}

// Delete  row in  table
function deleteQueue() {
  if (qCommands.isEmpty()) {
    alert("There are no commands in queue to delete. Please add a command by selecting a button in the Controls section");
  }
  else {
    $("table tbody").find('input[name="record"]').each(function () {
      if ($(this).is(":checked")) {
        // Extract the row index
        var row = $(this).closest("tr").index();
        //console.log(row);
        qCommands.removebyIndex(row);
        //console.log(qCommands.items);
        $(this).parents("tr").remove();
      }
    });
    addNo();
  }
}

function deleteAllQueue() {
  if (qCommands.isEmpty()) {
    alert("There are no commands in queue to delete. Please add a command by selecting a button in the Controls section");
  }
  else {
    // Delete all the rows in the table
    qCommands.clear();
    $('#tableCommands').find("tr:gt(0)").remove();
    //console.log(qCommands.items);
  }
}

// Submit the qCommands to the server via GET
function submitQueue() {
  if (qCommands.isEmpty()) {
    alert("Please add a command by selecting a button in the Controls section");
  }
  else {
    var qCommandsString = qCommands.convertToString();
    // Formatting the string
    qCommandsString = qCommandsString.replace(/[\[\]']+/g, '');
    qCommandsString = qCommandsString.replace(/["]+/g, '');
    qCommandsString = qCommandsString.replace(/[,]+/g, '');
    console.log(qCommandsString);
    $.ajax({
      type: "GET",
      url: "/submitCommands",
      data: {
        "qCommands": qCommandsString
      },
      success: function (data) {
        //console.log(data);
        deleteAllQueue();
      }
    });
  }
}


//#region illustration to dispaly robo car status
/**/
function display() {
  var bottomRect = document.getElementById("bottomRect").getContext('2d');
  if (isBlackTileDetected == false) {
    // display dotted rectangle
    bottomRect.clearRect(0, 5, 60, 12);
    bottomRect.beginPath();
    bottomRect.setLineDash([5]);
    bottomRect.rect(0, 5, 60, 12);
    bottomRect.stroke();

  } else {
    // display filled rectangle
    bottomRect.clearRect(0, 5, 60, 12);
    bottomRect.fillRect(0, 5, 60, 12);
  }

  var rightRect = document.getElementById("rightRect").getContext('2d');
  if (isObstacleDetected == false) {
    // display dotted rectangle
    rightRect.clearRect(20, 0, 12, 44);
    rightRect.beginPath();
    rightRect.setLineDash([3]);
    rightRect.rect(20, 0, 12, 44);
    rightRect.stroke();
  } else {
    // display filled rectangle
    rightRect.clearRect(20, 0, 12, 44);
    rightRect.fillRect(20, 0, 12, 44);
  }
}

// AJAX call to check if the car detect obstacle
function checkFeedback() {
  $.ajax({
    type: "POST",
    url: "/checkFeedback",
    async: false,
    success: function (data) {
      console.log(data);
      if (data.includes("obstacle") == true) {
        isObstacleDetected = true;
      }
      else {
        isObstacleDetected = false;
      }
      if (data.includes("black") == true) {
        isBlackTileDetected = true;
      }
      else {
        isBlackTileDetected = false;
      }
      console.log("Obstacle detected: " + isObstacleDetected);
      console.log("Black Tiles detected: " + isBlackTileDetected);
    }
  });
}

// AJAX call to check if the car detect obstacle
function checkFirstCommand() {
  $.ajax({
    type: "GET",
    url: "/api/commands/getFirstCommands",
    async: false,
    success: function (data) {
      console.log(data);
      console.log(data[0]);
      switch (data[0]) {
        case 'W':
          // Move foward
          movement = "Forward"
          break;

        case 'A':
          // Turn left
          movement = "Left"
          break;

        case 'S':
          // Move backward
          movement = "Backward"
          break;

        case 'D':
          // Turn right
          movement = "Right"
          break;

        case 'R':
          // Rotate 360 degrees clockwise
          movement = "Rotate"
          break;

        default:
          // Return dash to indicate null
          movement = "-"
          break;
      }
      console.log("Current Movement: " + movement);
      // Writing data back to HTML
      $("#Movement").html(movement);
      // document.getElementById("Movement").innerHTML = movement;
    }
  });
}

//#endregion
//#endregion

