#include "LiquidCrystalDisplay.hpp"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

// When the display powers up, it is configured as follows:

LiquidCrystal::LiquidCrystal(uint8_t rst, bool verbose = false)
{
	I2c.begin();
	TFTInit(rst);
	_verbose = verbose;
}


void LiquidCrystal::TFTInit(uint8_t rst)
{
	_rst_pin = rst;  //Reset LCDs PIN

	pinMode(_rst_pin, OUTPUT);

	digitalWrite(_rst_pin, LOW);//Reset LCDs
	delayMicroseconds(1000);
	digitalWrite(_rst_pin, HIGH);
}

inline void LiquidCrystal::command(uint8_t value) {
}

inline size_t LiquidCrystal::write(uint8_t value) {
	return 1; // assume sucess
}

/***********Basic Commands */
#pragma region BasicCommands
void LiquidCrystal::SetReset(void)
{
	sendBuf[0] = CMD_RESET;
	sendBuf[1] = 0x52;
	sendBuf[2] = 0x65;
	sendBuf[3] = 0x73;
	sendBuf[4] = 0x65;
	sendBuf[5] = 0x74;
	I2C_Send(sendBuf, 6);
}

void LiquidCrystal::ClrScreen(uint16_t Color)
{
	sendBuf[0] = CMD_CLEAR_SCREEN;
	sendBuf[1] = (Color >> 8);
	sendBuf[2] = Color;
	I2C_Send(sendBuf, 3);
}

void LiquidCrystal::Display_Image(uint16_t x, uint16_t y, uint8_t image_id)
{
	sendBuf[0] = CMD_ICON_DISPLAY;
	sendBuf[1] = (x >> 8);
	sendBuf[2] = x;
	sendBuf[3] = (y >> 8);
	sendBuf[4] = y;
	sendBuf[5] = image_id;

	I2C_Send(sendBuf, 6);
}

void LiquidCrystal::DisplayCut_Image(uint16_t image_x, uint16_t image_y, uint16_t image_w, uint16_t image_h, uint8_t  image_id)
{
	sendBuf[0] = CMD_CUT_ICON;
	sendBuf[1] = (image_x >> 8);
	sendBuf[2] = image_x;
	sendBuf[3] = (image_y >> 8);
	sendBuf[4] = image_y;
	sendBuf[5] = (image_w >> 8);
	sendBuf[6] = image_w;
	sendBuf[7] = (image_h >> 8);
	sendBuf[8] = image_h;
	sendBuf[9] = image_id;

	I2C_Send(sendBuf, 10);
}


void LiquidCrystal::PutString(uint16_t x, uint16_t y, displayType DisplayType, fontSize FontSize, uint8_t ImageNo, uint16_t BackColor, uint16_t ForeColor, uint8_t* strings)
{
	uint8_t i = 0;

	sendBuf[0] = CMD_STR_DISPLAY;
	sendBuf[1] = (x >> 8);
	sendBuf[2] = x;
	sendBuf[3] = (y >> 8);
	sendBuf[4] = y;
	sendBuf[5] = (uint8_t)DisplayType;
	sendBuf[6] = (uint8_t)FontSize;
	sendBuf[7] = ImageNo;
	sendBuf[8] = (BackColor >> 8);
	sendBuf[9] = BackColor;
	sendBuf[10] = (ForeColor >> 8);
	sendBuf[11] = ForeColor;

	while (*strings != 0)
	{
		sendBuf[i + 12] = *(strings++);
		i++;
	}
	I2C_Send(sendBuf, 12 + i);
}

void LiquidCrystal::Display_Message(uint8_t FontSize, uint8_t time, uint8_t* strings)
{
	uint8_t i = 0;
	sendBuf[0] = CMD_MESSAGE_DISPLAY;
	sendBuf[1] = FontSize;
	sendBuf[2] = time;
	while (*strings != 0)
	{
		sendBuf[i + 3] = *(strings++);
		i++;
	}
	I2C_Send(sendBuf, 3 + i);

}

void LiquidCrystal::Line(uint16_t s_x, uint16_t s_y, uint16_t e_x, uint16_t e_y, uint16_t ForeColor)
{
	sendBuf[0] = CMD_DRAW_LINE;
	sendBuf[1] = (s_x >> 8);
	sendBuf[2] = s_x;
	sendBuf[3] = (s_y >> 8);
	sendBuf[4] = s_y;
	sendBuf[5] = (e_x >> 8);
	sendBuf[6] = e_x;
	sendBuf[7] = (e_y >> 8);
	sendBuf[8] = e_y;
	sendBuf[9] = (ForeColor >> 8);
	sendBuf[10] = ForeColor;
	I2C_Send(sendBuf, 11);
}


void LiquidCrystal::Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t ForeColor)
{
	sendBuf[0] = CMD_DRAW_RECT;
	sendBuf[1] = (x >> 8);
	sendBuf[2] = x;
	sendBuf[3] = (y >> 8);
	sendBuf[4] = y;
	sendBuf[5] = (width >> 8);
	sendBuf[6] = width;
	sendBuf[7] = (height >> 8);
	sendBuf[8] = height;
	sendBuf[9] = (ForeColor >> 8);
	sendBuf[10] = ForeColor;
	I2C_Send(sendBuf, 11);
}

void LiquidCrystal::RectangleFill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t ForeColor)
{
	sendBuf[0] = CMD_DRAW_FILL_RECT;
	sendBuf[1] = (x0 >> 8);
	sendBuf[2] = x0;
	sendBuf[3] = (y0 >> 8);
	sendBuf[4] = y0;
	sendBuf[5] = (x1 >> 8);
	sendBuf[6] = x1;
	sendBuf[7] = (y1 >> 8);
	sendBuf[8] = y1;
	sendBuf[9] = (ForeColor >> 8);
	sendBuf[10] = ForeColor;
	I2C_Send(sendBuf, 11);
}


void LiquidCrystal::SetBackLight(uint8_t brightness)
{
	sendBuf[0] = CMD_SET_LCD_BRIGHTNESS;
	sendBuf[1] = brightness;
	I2C_Send(sendBuf, 2);
}

void LiquidCrystal::EnableTouchScreen(bool enable)
{
	sendBuf[0] = CMD_TP_SET;
	if (enable)
		sendBuf[1] = 0x01;
	else
		sendBuf[1] = 0x00;
	I2C_Send(sendBuf, 2);
}

void LiquidCrystal::CalibrateTouchPane(void)
{
	sendBuf[0] = CMD_TP_CALIBRATION;
	sendBuf[1] = 0x50;
	I2C_Send(sendBuf, 2);
}

void LiquidCrystal::TestTouchPanel(bool enable)
{
	sendBuf[0] = CMD_TP_TEST;
	if (enable)
		sendBuf[1] = 0x01;
	else
		sendBuf[1] = 0x00;
	I2C_Send(sendBuf, 2);
}
#pragma endregion

/***********Object Commands */
#pragma region ObjectCommands
void LiquidCrystal::SetPage(uint8_t page_id)
{
	sendBuf[0] = CMD_SET_PAGE_ID;
	sendBuf[1] = page_id;
	I2C_Send(sendBuf, 2);
}

void LiquidCrystal::GetPage(void)
{
	sendBuf[0] = CMD_GET_PAGE_ID;
	I2C_Send(sendBuf, 1);
}

void LiquidCrystal::SetLableValue(uint8_t page_id, uint8_t control_id, uint8_t* strings)
{
	uint8_t i = 0;
	sendBuf[0] = LABLE;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;

	for (i = 0; i < sizeof(strings); i++)
	{
		sendBuf[i + 3] = strings[i];
	}

	I2C_Send(sendBuf, 3 + i);
}

void LiquidCrystal::SetNumberValue(uint8_t page_id, uint8_t control_id, uint16_t number)
{
	sendBuf[0] = NUMBER;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = (number >> 8);
	sendBuf[4] = number;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::SetEditValue(uint8_t page_id, uint8_t control_id, uint8_t* strings)
{
	uint8_t i = 0;
	sendBuf[0] = SET_EDIT;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	while (*strings != 0)
	{
		sendBuf[i + 3] = *(strings++);
		i++;
	}
	I2C_Send(sendBuf, 3 + i);
}

void LiquidCrystal::GetEditValue(uint8_t page_id, uint8_t control_id)
{
	sendBuf[0] = GET_EDIT;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	I2C_Send(sendBuf, 3);
}

void LiquidCrystal::GetTouchEditValue(uint8_t page_id, uint8_t control_id)
{
	sendBuf[0] = GET_TOUCH_EDIT_VALUE;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	I2C_Send(sendBuf, 3);
}

void LiquidCrystal::SetProgressbarValue(uint8_t page_id, uint8_t control_id, uint8_t value)
{
	sendBuf[0] = PROGRESS;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = value;
	I2C_Send(sendBuf, 4);
}

void LiquidCrystal::SetCheckboxValue(uint8_t page_id, uint8_t control_id, checkboxState state)
{
	sendBuf[0] = SET_CHECKBOX;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = (uint8_t)state;
	I2C_Send(sendBuf, 4);
}

void LiquidCrystal::GetCheckboxValue(uint8_t page_id, uint8_t control_id)
{
	sendBuf[0] = GET_CHECKBOX;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	I2C_Send(sendBuf, 3);
}

void LiquidCrystal::SetCircleGaugeValue(uint8_t page_id, uint8_t control_id, uint16_t value)
{
	sendBuf[0] = CIRCLEGAUGE;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = (value >> 8);
	sendBuf[4] = value;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::SetBarGaugeValue(uint8_t page_id, uint8_t control_id, uint16_t value)
{
	sendBuf[0] = BARGAUGE;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = (value >> 8);
	sendBuf[4] = value;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::SetWaterGaugeValue(uint8_t page_id, uint8_t control_id, uint16_t value)
{
	sendBuf[0] = WATERGAUGE;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = (value >> 8);
	sendBuf[4] = value;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::SetThermometerValue(uint8_t page_id, uint8_t control_id, uint16_t value)
{
	sendBuf[0] = THERMOMETER;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = (value >> 8);
	sendBuf[4] = value;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::SetBatteryValue(uint8_t page_id, uint8_t control_id, uint16_t value)
{
	sendBuf[0] = BATTERY;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = (value >> 8);
	sendBuf[4] = value;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::SetWaveformValue(uint8_t page_id, uint8_t control_id, uint8_t channelNo, uint8_t value)
{
	sendBuf[0] = WAVEFORM;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = channelNo;
	sendBuf[4] = value;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::WaveformDataClear(uint8_t page_id, uint8_t control_id)
{
	sendBuf[0] = CLEAR_WAVEFORM;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	I2C_Send(sendBuf, 3);
}

void LiquidCrystal::WaveformDataInsert(uint8_t page_id, uint8_t control_id, uint8_t channelNo, uint8_t value)
{
	sendBuf[0] = INSER_WAVEFORM_DATA;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = channelNo;
	sendBuf[4] = value;
	I2C_Send(sendBuf, 5);
}

void LiquidCrystal::WaveformDataRefresh(uint8_t page_id, uint8_t control_id, uint8_t channelNo)
{
	sendBuf[0] = REFRESH_WAVEFORM;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = channelNo;
	I2C_Send(sendBuf, 4);
}

void LiquidCrystal::SetSliderValue(uint8_t page_id, uint8_t control_id, uint8_t value)
{
	sendBuf[0] = SET_SLIDER_VALUE;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	sendBuf[3] = value;
	I2C_Send(sendBuf, 4);
}

void LiquidCrystal::GetSliderValue(uint8_t page_id, uint8_t control_id)
{
	sendBuf[0] = GET_SLIDER_VALUE;
	sendBuf[1] = control_id;
	sendBuf[2] = page_id;
	I2C_Send(sendBuf, 3);
}
#pragma endregion

void LiquidCrystal::readMessage(void)
{
	uint8_t   identifier, cnt;
	uint8_t   command_cmd;
	uint8_t   command_status;
	uint8_t   command_length;
	command_cmd = I2C_Read();
	queue_push(command_cmd);

	for (cnt = 0; cnt < 2; cnt++)
	{
		identifier = I2C_Read();
		queue_push(identifier);

#ifdef DEBUG
		Serial.println(identifier, HEX);
#endif // DEBUG
	}
	command_status = I2C_Read();
	identifier = I2C_Read();
	command_length = I2C_Read();

	queue_push(command_status);
	queue_push(identifier);
	queue_push(command_length);

	if ((command_cmd == GET_EDIT && command_status == SUCCESS) || (command_cmd == GET_TOUCH_EDIT_VALUE && command_status == SUCCESS))
	{
		for (cnt = 0; cnt < command_length; cnt++)
		{
			identifier = I2C_Read();
			queue_push(identifier);
#ifdef DEBUG
			Serial.println(identifier, HEX);
#endif // DEBUG
		}
	}
}

bool LiquidCrystal::checkNewMessage(void)
{
	if (queue_size() > 0)
		return true;
	return false;
}

PCTRL_MSG LiquidCrystal::getMessage()
{
	uint8_t   dataSize;
	dataSize = queue_find_cmd(cmdBuf, cmdMaxSize);
	if (dataSize > 0)
	{
		if (_verbose)
			verboseCommand((PCTRL_MSG)cmdBuf);
		return (PCTRL_MSG)cmdBuf;
	}

	return NULL;
}

void LiquidCrystal::verboseCommand(PCTRL_MSG msg)
{
	uint8_t cmd_type = msg->cmd_type;
	uint8_t control_id = msg->control_id;
	uint8_t page_id = msg->page_id;
	uint8_t _status = msg->status;
	uint8_t button_type = msg->button_type;
	uint8_t key_value = msg->button_value;
	
	char temp[255];
	String cmdType = "Cmd Type     : ";
	String status = "Status       : ";
	String keyType = "Key type     : ";
	String keyValue = "Key value    : ";

	switch (cmd_type)
	{
	case RETURN_GET_TOUCH_EDIT:
		cmdType += "Return touch edit";
		break;
	case RETURN_TOUCH_BUTTON:
		cmdType += "Button Touched";
		break;
	case RETURN_TOUCH_CHECKBOX:
		cmdType += "Checkbox Touched";
		break;
	case RETURN_TOUCH_SLIDER:
		cmdType += "Slider Touched";
		keyValue = "Slider value : ";
		break;
	case RETURN_GET_EDIT:
		cmdType += "Return edit";
		break;
	case RETURN_GET_PAGE:
		cmdType += "Return get page";
		break;
	case RETURN_GET_CHECKBOX:
		cmdType += "Return checkbox value";
		break;
	case RETURN_GET_SLIDER:
		cmdType += "Return slider value";
		break;
	default:
		Serial.print("0x");
		Serial.println(cmd_type, HEX);
		break;
	}

	switch (_status)
	{
	case BUTTON_PRESSED:
		status += "Key pressed";
		break;
	case BUTTON_RELEASED:
		status += "Key released";
		break;
	case SUCCESS:
		status += "Succes";
		break;
	case INVALID:
		status += "Invalid";
		break;
	case FAILURE:
		status += "Failure";
		break;
	default:
		sprintf(temp, "0x%02x", _status);
		status += temp;
		break;
	}

	switch (button_type)
	{
	case CHAR:
		keyType += "Character";
		break;
	case CLEAR:
		keyType += "Clear";
		break;
	case BACK_SPACE:
		keyType += "Back Space";
		break;
	case SPACE:
		keyType += "Space";
		break;
	case ENTER:
		keyType += "Enter";
		break;
	case CHANGE_PAGE:
		keyType += "Change page";
		keyValue = "To page      : ";
		break;
	case UPLOAD_CONTROL_ID:
		keyType += "Upload control id";
		break;
	case 0xff:
		keyType += "Fixed value";
		break;
	default:
		sprintf(temp, "0x%02x", button_type);
		keyType += temp;
		break;
	}

	switch (key_value)
	{
	case SELECTED:
		keyValue += "Checked";
		break;
	case UNSELECTED:
		keyValue += "Unchecked";
		break;
	default:
		if (button_type == CHAR) {
			keyValue += (char)key_value;
		}
		else if (button_type == CHANGE_PAGE){
			keyValue += key_value;
		}
		else {
			sprintf(temp, "0x%02x", key_value);
			keyValue += temp;
		}
		break;
	}

	Serial.println(cmdType);
	Serial.print("Control id   : ");
	Serial.println(control_id );
	Serial.print("Page id      : ");
	Serial.println(page_id);
	Serial.println(status);
	Serial.println(keyType);
	Serial.println(keyValue);

	Serial.println();
}


void LiquidCrystal::I2C_Send(uint8_t* buf, uint8_t len)
{
	I2c.write(address, buf, len);

}

uint8_t LiquidCrystal::I2C_Read(void)
{
	uint8_t _data;
	_data = I2c.read(address, (uint8_t)1);
	return _data;
}


/***********Queue data */
#pragma region MyRegion

void LiquidCrystal::queue_reset()
{
	que._head = que._tail = 0;
	cmd_pos = 0;
}

void LiquidCrystal::queue_push(qdata _data)
{
	qsize pos = (que._head + 1) % QUEUE_MAX_SIZE;
	if (pos != que._tail)
	{
		que._data[que._head] = _data;
		que._head = pos;
	}
}

void LiquidCrystal::queue_pop(qdata* _data)
{
	if (que._tail != que._head)
	{
		*_data = que._data[que._tail];
		que._tail = (que._tail + 1) % QUEUE_MAX_SIZE;
	}
}

uint8_t LiquidCrystal::queue_size()
{
	return ((que._head + QUEUE_MAX_SIZE - que._tail) % QUEUE_MAX_SIZE);
}

uint8_t LiquidCrystal::queue_find_cmd(qdata* buffer, qsize buf_len)
{
	qsize cmd_size = 0;
	qdata _data = 0;

	cmd_length = CMD_DATA_LENGTH - 1;

	while (queue_size() > 0)
	{
		//get 1 data
		queue_pop(&_data);

		if (cmd_pos == 0 && _data != RETURN_TOUCH_BUTTON && _data != RETURN_TOUCH_CHECKBOX && _data != RETURN_TOUCH_SLIDER
			&& _data != RETURN_GET_EDIT && _data != RETURN_GET_PAGE && _data != RETURN_GET_CHECKBOX && _data != RETURN_GET_SLIDER
			&& _data != RETURN_TOUCH_EDIT && _data != RETURN_GET_TOUCH_EDIT)
			continue;

		if (cmd_pos < buf_len)
		{
			if (cmd_pos == 0)
			{
				cmd_backup = _data;
			}

			if (cmd_pos == (CMD_DATA_LENGTH - 1))
			{
				if (cmd_backup == RETURN_GET_EDIT || cmd_backup == RETURN_GET_TOUCH_EDIT)
					cmd_length = CMD_DATA_LENGTH + _data - 1;

			}

			if (cmd_pos > (cmdMaxSize - 1))
			{
				cmd_pos = 0;
				cmd_backup = 0;
			}

			buffer[cmd_pos++] = _data;
		}

		//get whole cmd data
		if (cmd_pos > cmd_length)
		{
			cmd_size = cmd_pos;
			cmd_pos = 0;
			return cmd_size;
		}
	}

	return 0;
}
#pragma endregion