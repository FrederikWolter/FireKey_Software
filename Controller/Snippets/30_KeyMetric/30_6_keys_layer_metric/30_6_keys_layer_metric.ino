/*********************************************************************
* FIREKEY - PROJECT
* 
* Code snippet to test the key metric layer idea
* 
* Required libs:
* - None
*********************************************************************/

// CONSTANTS
#define ROW_COUNT 2  // number of rows
#define COL_COUNT 3  // number of columns
//Layers
#define LAYER_ROW_IDX 0          // define the layer row index
#define LAYER_BACK_COL_IDX 2     // define the back button column index
#define LAYER_HOME_COL_IDX 1     // define the home button column index
#define LAYER_FORWARD_COL_IDX 0  // define the forward button column index
#define MAX_LAYER 5
#define HOME_LAYER 0

// PINS
byte rows[ROW_COUNT] = { 5, 6 };        // define the row pins
byte cols[COL_COUNT] = { 10, 16, 14 };  // define the column pins

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
    // pull output row to low
    byte curRow = rows[rowIndex];
    digitalWrite(curRow, LOW);

    // is any column pulled to low due to a button being pressed?
    for (int colIndex = 0; colIndex < COL_COUNT; colIndex++) {
      byte colRow = cols[colIndex];
      if (digitalRead(colRow) == LOW) {
        keyPressed(rowIndex, colIndex);
      }
    }

    // pull output row high again
    digitalWrite(curRow, HIGH);
  }
}

void keyPressed(byte rowIdx, byte colIdx) {

  if (rowIdx == LAYER_ROW_IDX) {
    // it's the row where the layer buttons are located
    switch (colIdx) {
      case LAYER_BACK_COL_IDX:
        Serial.println("Layer back");
        if (currentLayer == 0) {
          currentLayer = MAX_LAYER - 1;
        } else {
          currentLayer--;
        }
        Serial.println(currentLayer);
        break;
      case LAYER_FORWARD_COL_IDX:
        Serial.println("Layer forward");
        currentLayer = (currentLayer + 1) % MAX_LAYER;
        Serial.println(currentLayer);
        break;
      case LAYER_HOME_COL_IDX:
      default:
        Serial.println("Home");
        currentLayer = HOME_LAYER;
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