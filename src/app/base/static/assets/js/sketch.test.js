const make2Darray = require('./sketch');
const retrieveQueue = require('./sketch');
const gameCar = require('./sketch');
const gameStar = require('./sketch');
const color2Darray = require('./sketch');
const draw2Darray = require('./sketch');
const sleep = require('./sketch');
const createRestartButton = require('./sketch');
const GenerateGameText = require('./sketch');
const storeFirstCmd = require('./sketch');
const displayAllStars = require('./sketch');


test('make 2D array', () => {
  expect(make2Darray(1,1)).toBe([[]])
});

test('color 2D array', () => {
    expect(typeof color2Darray([2,1],[3,1])).toBe("object")
  });

test('draw 2D array', () => {
expect(typeof draw2Darray()).toBe("object")
});

test('retrieve queue from DB', () => {
    expect(typeof retrieveQueue()).toBe("object")
  });

test('initialise gameCar', () => {
    expect(typeof new gameCar(10,10)).toBe("object")
  });

test('initialise gameStar', () => {
expect(typeof new gameStar(10,10)).toBe("object")
});

test('sleep function', () => {3
    expect(typeof sleep(3000)).toBe("object")
    });

test('create start button', () => {
    expect(typeof createRestartButton()).toBe("object")
    });

test('Generate Text for the Game', () => {
    expect(typeof GenerateGameText()).toBe("object")
    });

test('Store first command', () => {
    expect(typeof storeFirstCmd()).toBe("object")
    });

test('Display All stars', () => {
    expect(typeof displayAllStars()).toBe("object")
    });
    

// test('initialise queue', () => {
//     expect(typeof new Queue()).toBe("object")
//     });

// test('enqueue black tiles', () => {
//     expect(black_tiles()).toBe("B")
//     });


    



