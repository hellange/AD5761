//References: https://wiki.analog.com/resources/tools-software/uc-drivers/ad5761r

/* Input Shift Register Commands */
#define CMD_NOP              0x0
#define CMD_WR_TO_INPUT_REG       0x1
#define CMD_UPDATE_DAC_REG        0x2
#define CMD_WR_UPDATE_DAC_REG     0x3
#define CMD_WR_CTRL_REG         0x4
#define CMD_NOP_ALT_1         0x5
#define CMD_NOP_ALT_2         0x6
#define CMD_SW_DATA_RESET       0x7
#define CMD_RESERVED          0x8
#define CMD_DIS_DAISY_CHAIN       0x9
#define CMD_RD_INPUT_REG        0xA
#define CMD_RD_DAC_REG          0xB
#define CMD_RD_CTRL_REG         0xC
#define CMD_NOP_ALT_3         0xD
#define CMD_NOP_ALT_4         0xE
#define CMD_SW_FULL_RESET       0xF


#include <SPI.h> // Include the Arduino SPI library

const int ssPin = 10;

static byte SPI_Buff[3];  // read data ???


void setup()
{
  Serial.begin(9600);
  
  pinMode(ssPin, OUTPUT);  // Set the SS pin as an output
  digitalWrite(ssPin, HIGH);  // Set the SS pin HIGH
  SPI.begin();  // Begin SPI hardware
  SPI.setClockDivider(SPI_CLOCK_DIV64);  // Slow down SPI clock

  // software reset
  ad5761r_readwrite(CMD_SW_FULL_RESET, 0);

  // read control register
  ad5761r_readwrite(CMD_RD_CTRL_REG, 0);
  ad5761r_readwrite(CMD_RD_CTRL_REG, 0);

  Serial.println("Readback from control register before setting:");
  printRegisterData();

  // write control register
  ad5761r_readwrite(CMD_WR_CTRL_REG, 0b0000000001101011);

  // read control register
  ad5761r_readwrite(CMD_RD_CTRL_REG, 0);
  ad5761r_readwrite(CMD_RD_CTRL_REG, 0);

  Serial.println("Readback from control register before setting:");
  printRegisterData();

}

void printRegisterData() {
  Serial.print(SPI_Buff[0], HEX);
  Serial.print(SPI_Buff[1], HEX);
  Serial.println(SPI_Buff[2], HEX);
}

void loop()
{
  //ad5761r_readwrite(CMD_WR_TO_INPUT_REG, 0);
  ad5761r_readwrite(CMD_UPDATE_DAC_REG, 0);
  printRegisterData();
  delay(100);  // This will make the display update at 100Hz.*/
  
  //ad5761r_readwrite(CMD_WR_TO_INPUT_REG, 0xffff);
  ad5761r_readwrite(CMD_UPDATE_DAC_REG, 0xffff);
  printRegisterData();
  delay(100);  // This will make the display update at 100Hz.*/
}

void ad5761r_readwrite(uint8_t reg_addr_cmd,
            uint16_t reg_data)
{
  uint8_t data[3];
  
  digitalWrite(ssPin, LOW);
  data[0] = reg_addr_cmd;
  data[1] = (reg_data & 0xFF00) >> 8;
  data[2] = (reg_data & 0x00FF) >> 0;
  for (int i=0; i<3; i++)
  {
    SPI_Buff[i] = SPI.transfer(data);
  }
  digitalWrite(ssPin, HIGH);
}

