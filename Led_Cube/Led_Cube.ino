#include <SPI.h>
#include <MsTimer2.h>

unsigned long Timer = 0;
int layer = 0;
int increment = 1;

#include "LedCubeData.h"
#include "ButtonDebounce.h"

void NextDisplay()
{
  // Send next pattern to Shift Registers.
  PORTC |= 0x0f;// set bottom bit low, turning off display.
  SPI.transfer16( ~LedCube_NextPlane() );
  // Turn on the current plane.
  PORTC &= ~(1 << LedCube_CurrentPlane );

} // End of NextDisplay

// As a test, each plane is given a location (row,column)
// and that plane is moved through all its locations.
int ZeroRow  = 0, ZeroColumn  = 0;
void MoveZero()
{
  ZeroColumn++;
  if ( ZeroColumn >= 4 )
  {
    ZeroColumn = 0;
    ZeroRow++;
    if ( ZeroRow >= 4 )
      ZeroRow = 0;
  }
} // End of MoveZero

int OneRow   = 1, OneColumn   = 1;
void MoveOne()
{
  OneColumn++;
  if ( OneColumn >= 4 )
  {
    OneColumn = 0;
    OneRow++;
    if ( OneRow >= 4 )
      OneRow = 0;
  }
} // End of MoveOne

int TwoRow   = 2, TwoColumn   = 2;
void MoveTwo()
{
  TwoColumn++;
  if ( TwoColumn >= 4 )
  {
    TwoColumn = 0;
    TwoRow++;
    if ( TwoRow >= 4 )
      TwoRow = 0;
  }
} // End of MoveTwo

int ThreeRow = 3, ThreeColumn = 3;
void MoveThree()
{
  ThreeColumn++;
  if ( ThreeColumn >= 4 )
  {
    ThreeColumn = 0;
    ThreeRow++;
    if ( ThreeRow >= 4 )
      ThreeRow = 0;
  }
}  // End of MoveThree

void Mode0()
{
  LedCube_ClearData();
  MoveZero();
  LedCube_SetLed( ZeroRow,  ZeroColumn,  0 );
  MoveOne();
  LedCube_SetLed( OneRow,   OneColumn,   1 );
  MoveTwo();
  LedCube_SetLed( TwoRow,   TwoColumn,   2 );
  MoveThree();
  LedCube_SetLed( ThreeRow, ThreeColumn, 3 );
}

void MoveLayer() {
  if (layer == 0) {
    increment = 1;
  } else if (layer == 3) {
    increment = -1;
  }

  layer += increment;
}

void Chase() {
  LedCube_ClearData();
  LedCube_SetLed(ChaseRow, ChaseCol, ChaseLayer);
  MoveChase();
}

void Layers() {
  LedCube_ClearData();

  LedCube_SetLed(0, 0, layer);
  LedCube_SetLed(0, 1, layer);
  LedCube_SetLed(0, 2, layer);
  LedCube_SetLed(0, 3, layer);

  LedCube_SetLed(1, 0, layer);
  LedCube_SetLed(1, 1, layer);
  LedCube_SetLed(1, 2, layer);
  LedCube_SetLed(1, 3, layer);

  LedCube_SetLed(2, 0, layer);
  LedCube_SetLed(2, 1, layer);
  LedCube_SetLed(2, 2, layer);
  LedCube_SetLed(2, 3, layer);

  LedCube_SetLed(3, 0, layer);
  LedCube_SetLed(3, 1, layer);
  LedCube_SetLed(3, 2, layer);
  LedCube_SetLed(3, 3, layer);

  MoveLayer();
}

// setup code, run once:
void setup()
{
  MsTimer2::set(4, NextDisplay ); // 4ms period
  MsTimer2::start();

  // A3-A0 to outputs.
  DDRC |= 0x0f;
  // Set up display data.
  LedCube_ClearData();
  // Start up the SPI
  SPI.begin();
  // Set the parameters for the transfers.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  ButtonInitialize();

  // Timer for moving the ON led's
  Timer = millis();

} // End of setup

int Mode = 0;

// main code, run repeatedly:
void loop()
{
  // 2000 millisecond timer to update display
  if ( millis() - Timer >= 200 )
  {
    // Extra modes could be done here.
    switch (Mode) {
      case 0:
        Mode0();
        break;
      case 1:
        Chase();
        break;
      case 2:
        Layers();
        break;
    }

    Timer += 200; // Update timer

  } // End of timer if.

  if ( ButtonNextState(digitalRead(4)) == 1 )
  {
    Mode = (Mode + 1) % 3;

    // Mode Reset
  }
} // End of loop.
