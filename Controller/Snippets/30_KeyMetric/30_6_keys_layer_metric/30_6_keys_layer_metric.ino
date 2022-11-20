/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the key metric layer idea
* 
* Requried libs:
* - None
*********************************************************************/

// CONSTANTS
#define ROW_COUNT 2  // number of rows
#define COL_COUNT 3  // number of columns

// PINS
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

// LAYER
const byte layerRowIdx = 0;         // define the layer row index
const byte layerBackColIdx = 2;     // define the back button column index
const byte layerHomeColIdx = 1;     // define the home button column index
const byte layerForwardColIdx = 0;  // define the forward button column index
const byte maxLayer = 5;
const byte homeLayer = 0;

byte currentLayer = 0;

void setup() {
  Serial.begin(9600);

  // declare all cols as input and activate internal pullup resistor
  for (int x = 0; x < COL_COUNT; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }

  // declare all rows as output and set them high
  for (int x = 0; x < ROW_COUNT; x++) {
    pinMode(rows[x], OUTPUT);
    digitalWrite(rows[x], HIGH);
  }
}

void loop() {
  readMatrix();
  delay(100);
}

void readMatrix() {
  // scan matrix
  for (int rowIndex = 0; rowIndex < ROW_COUNT; rowIndex++) {
    // row: set output to low
    byte curRow = rows[rowIndex];
    digitalWrite(curRow, LOW);

    // col: interate through the columns
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      byte colRow = cols[colIndex];
      if (digitalRead(colRow) == LOW) {
        keyPressed(rowIndex, colIndex);
      }
    }

    digitalWrite(curRow, HIGH);
  }
}

void keyPressed(byte rowIdx, byte colIdx) {

  if (rowIdx == layerRowIdx) {
    // it's the row where the layer buttons are located
    switch (colIdx) {
      case layerBackColIdx:
        Serial.println("Layer back");
        if (currentLayer == 0) {
          currentLayer = maxLayer - 1;
        } else {
          currentLayer--;
        }
        Serial.println(currentLayer);
        break;
      case layerForwardColIdx:
        Serial.println("Layer forward");
        currentLayer = (currentLayer + 1) % maxLayer;
        Serial.println(currentLayer);
        break;
      case layerHomeColIdx:
      default:
        Serial.println("Home");
        currentLayer = homeLayer;
        Serial.println(currentLayer);
    }
  } else {
    // the button rows with functions
    Serial.print("pressed: ");
    Serial.print("R");
    Serial.print(rowIdx);
    Serial.print("C");
    Serial.print(colIdx);
    Serial.println(" ");
  }
}