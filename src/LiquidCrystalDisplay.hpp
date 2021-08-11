/**
********************************************************************************
 *  @file		LiquidCrystalDisplay.hpp
 *  @brief		Arduino lib for Hunda Tech I2C touchscreens
 *  @details	Tested on a H28A-IC, 
 *  @author		Efratror
 *  @version	1.0
 *  @date		3-8-2021
 ********************************************************************************/

#ifndef LiquidCrystalDisplay_h
#define LiquidCrystalDisplay_h

#include <inttypes.h>
#include "Print.h"
#include "I2C.h"

#pragma region commands
//! @addtogroup sendHex
//! @{

//! Basic hex commands
enum BasicCmd {
	CMD_ICON_DISPLAY		= 0x49,		//!< Show icon display
	CMD_CUT_ICON			= 0x4F,		//!< Show cut icon
	CMD_CLEAR_SCREEN		= 0x43,		//!< Clear screen
	CMD_STR_DISPLAY			= 0x53,		//!< Show string
	CMD_DRAW_FILL_RECT		= 0x46,		//!< Draw a filled rectangle
	CMD_DRAW_RECT			= 0x52,		//!< Draw a rectangle
	CMD_DRAW_LINE			= 0x4C,		//!< Draw a line
	CMD_DRAW_NUMBER			= 0x4E,		//!< Draw a number
	CMD_SET_LCD_BRIGHTNESS	= 0x42,		//!< Adjust backlight brightness
	CMD_TP_CALIBRATION		= 0x50,		//!< Calibrate resistance touchscreen
	CMD_TP_TEST				= 0x54,		//!< Touchscreen test
	CMD_TP_SET				= 0x51,		//!< Touchscreen enable/disable
	CMD_MESSAGE_DISPLAY		= 0x4D,		//!< Show a message
	CMD_CONNECT				= 0x48,		//!< Touchscreen connection status
	CMD_RESET				= 0x4A,		//!< Touchscreen reset
	CMD_SET_PAGE_ID			= 0x70,		//!< Set page
	CMD_GET_PAGE_ID			= 0x71		//!< Get page
};
 
//! Object mannipulation hex commands
enum ObjectCmd
{
	RECT			= 0x72,	//!< Not tested @todo check command
	LINE			= 0x6C,	//!< Not tested @todo check command
	LABLE			= 0x74,	//!< Set text value of lable control
	BOX				= 0x78,	//!< Not tested @todo check command
	SET_EDIT		= 0x65,	//!< Set value of Edit control
	GET_EDIT		= 0x75,	//!< Get value of Edit control
	PROGRESS		= 0x6F,	//!< Set value of Progress control
	SET_CHECKBOX	= 0x63,	//!< Set value of Checkbox control
	GET_CHECKBOX	= 0x64,	//!< Get value of Checkbox control
	IMAGE			= 0x69,	//!< Not tested @todo check command
	CIRCLEGAUGE		= 0x7A,	//!< Set value of Circlegauge control
	BARGAUGE		= 0x61,	//!< Set value of Bargauge
	WATERGAUGE		= 0x77,	//!< Set value of Watergauge
	THERMOMETER		= 0x6D,	//!< Set value of Thermometer
	WAVEFORM		= 0x66,	//!< Set value of Waveform
	BATTERY			= 0x79,	//!< Set value of Battery control
	FORM			= 0x73,	//!< Not tested @todo check command
	NUMBER			= 0x6E,	//!< Set value of Number control

	GET_SLIDER_VALUE	= 0x68,	//!< Get value of slider
	SET_SLIDER_VALUE	= 0x5F, //!< Set value of slider

	TOUCH_EDIT_SEND_VALUE	= 0x7B,	//!< not tested @todo check command
	GET_TOUCH_EDIT_VALUE	= 0x7C, //!< not tested @todo check command

	CLEAR_WAVEFORM		= 0x76,	//!< Clears the Waveform control
	INSER_WAVEFORM_DATA = 0x6A, //!< Inserts a data point in the Waveform control
	REFRESH_WAVEFORM	= 0x6B  //!< Refreshes the Waveform control
};
//! @}

//! @addtogroup returnmsg
//! @{

//! Generic control return message
typedef struct _CTRL_MSG
{
	uint8_t    cmd_type;		//!< Returned command @see returnCmd
	uint8_t    control_id;		//!< Id of the control
	uint8_t    page_id;			//!< Page number where the control is located
	uint8_t    status;			//!< Status command @see readState, buttonState or checkboxState
	uint8_t    button_type;		//!< Button control type @see buttonTypes
	uint8_t    button_value;	//!< Value of the button control
}CTRL_MSG, * PCTRL_MSG;

//! Edit control return message
typedef struct _EDIT_MSG
{
	uint8_t    cmd_type;		//!< Returned command @see returnCmd
	uint8_t    control_id;		//!< Id of the control
	uint8_t    page_id;			//!< Page where the control is located
	uint8_t    status;			//!< Status command @see readState, buttonState or checkboxState
	uint8_t    button_type;		//!< Button control type @see buttonTypes
	uint8_t    string_length;	//!< Length of the Edit control returned text
	uint8_t    text[57];		//!< The text returned by the Edit control
}EDIT_MSG, * PEDIT_MSG;
//!@}

//! @addtogroup returnhex
//! @{

//! Return hex-codes
enum returnType {
	RETURN_TOUCH_BUTTON		= 0x62,	//!< Button control was touched on screen
	RETURN_TOUCH_CHECKBOX	= 0x60,	//!< Checkbox control was touched on screen
	RETURN_TOUCH_SLIDER		= 0x67,	//!< Slider control was touched on screen
	RETURN_TOUCH_EDIT		= 0x7B,	//!< Edit control was touched on screen
									
	RETURN_GET_TOUCH_EDIT	= 0x7C,	//!< Not tested @todo check command
	RETURN_GET_EDIT			= 0x75,	//!< Not tested @todo check command
	RETURN_GET_PAGE			= 0x71,	//!< Get page information is returned
	RETURN_GET_CHECKBOX		= 0x64,	//!< Get Checkbox control information is returned
	RETURN_GET_SLIDER		= 0x68  //!< Get Slider control information is returned
};

//! Key type hex-codes
enum buttonTypes {
	CHAR				= 0X30,	//!< Button adds a character to the Edit control
	CLEAR				= 0X31,	//!< Button cleares the Edit control
	BACK_SPACE			= 0X32,	//!< Button preforms a backspace to the Edit control
	SPACE				= 0X33,	//!< Button adds a space to the Edit Control
	ENTER				= 0X34,	//!< Button preforms a enter in the Edit control
	CHANGE_PAGE			= 0X35,	//!< Button preforms a page change
	UPLOAD_CONTROL_ID	= 0X36  //!< Unknown @todo check command
};

//! Edit control readstate hex-codes
enum readState {
	SUCCESS = 0x6f,	//!< Edit control value is read succesfully
	INVALID = 0x6E,	//!< Edit control value is Invalid
	FAILURE = 0x65  //!< Unknown
};

//! Buttonstate hex-codes
enum buttonState {
	BUTTON_PRESSED	= 0x44,	//!< Button control is pressed
	BUTTON_RELEASED = 0x55  //!< Button control is released
};

//! Checkboxstate hex-codes
enum checkboxState {
	SELECTED	= 0X53,	//!< Checkbox control is selected
	UNSELECTED	= 0X55  //!< Checkbox control is unselected
};
//!@}
#pragma endregion


//! @addtogroup parameters
//! @{

//! Display type
enum class displayType {
	transparent = 0x00, //!< Transparent background
	color = 0x01,		//!< With color of the background
	cutImage = 0x02		//!< With cut image
};

//! Font size
enum class fontSize {
	Height16 = 0x10, //!< 16 Pixels
	Height24 = 0x18, //!< 24 Pixels
	Height32 = 0x20, //!< 32 Pixels
	Height48 = 0x30, //!< 48 Pixels
	Height64 = 0x40  //!< 64 Pixels
};
//! @}

//! @addtogroup LCDLib
//! @{

//! Hunda Tech LCD-touchscreen display
class LiquidCrystal : public Print {
private:

	static const uint8_t cmdMaxSize = 65;		//!< Maximum size (in bytes) of the command send or returned
	static const uint8_t QUEUE_MAX_SIZE = 256;	//!< Maximum size of the Queue
	static const uint8_t address = 0x77;		//!< i2c adress;
	static const uint8_t CMD_DATA_LENGTH = 6;

	bool _verbose;					//!< Verbose output of the returend command
	uint8_t _rst_pin;				//!< Reset LCDs, not used
	uint8_t _int_pin;				//!< LCD interrupt pin
	uint8_t sendBuf[cmdMaxSize];	//!< Send buffer
	uint8_t cmdBuf[cmdMaxSize];		//!< Receive buffer

	typedef uint8_t qdata;
	typedef uint16_t qsize;

	typedef struct
	{
		qsize _head;
		qsize _tail;
		qdata _data[QUEUE_MAX_SIZE];
	}QUEUE;

	qsize cmd_pos;
	qdata _length;
	qdata cmd_backup;
	qdata cmd_statu;
	qdata cmd_length;

	QUEUE que;
public:
	/** @brief Constructor of the liquidcrystal class
		@todo check reset pin
		@param rst The number of the reset pin (unused)
		@param verbose Verbose output of the received command
	*/
	LiquidCrystal(uint8_t rst, bool verbose = false);

	void TFTInit(uint8_t rst);

	virtual size_t write(uint8_t);
	void command(uint8_t);
	
	//! @addtogroup basic
	//! @{

	/** @brief Reset the LCD
	*	@details Clears internal data and display
	*/
	void SetReset(void);

	/** @brief Clears the LCD-screen
	*	Uses a RGB565 Color (16-bit)
	*	@todo test function
	*	@param Color 16-bit RGB
	*/
	void ClrScreen(uint16_t Color);


	/** @brief Display image with specified coordinates
		@todo test function
		@param x X-coordinate (2 bytes)
		@param y Y-coordiante (2 bytes)
		@param image_id Number of the image (from visual lcd studio)
	*/
	void Display_Image(uint16_t x, uint16_t y, uint8_t image_id);


	/** @brief Cutting full area of the full screen image
		@todo test function
		@param x X-coordinate (2 bytes)
		@param y Y-coordiante (2 bytes)
		@param width Image width  (2 bytes)
		@param height Image height (2 bytes)
		@param image_id Number of the image (from visual lcd studio)
	*/
	void DisplayCut_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t  image_id);


	/** @brief Display a string
		@todo test function
		@param x X-coordinate (2 bytes)
		@param y Y-coordiante (2 bytes)
		@param DisplayType @link displayType Display type @endlink
		@param FontSize @link fontSize Size of the font @endlink
		@param ImageNo NUmber of the image (from visual lcd studio)
		@param BackColor Background color
		@param ForeColor Forground color
		@param strings String to be displayed (Maximum size 47 char)

	*/
	void PutString(uint16_t x, uint16_t y, displayType DisplayType, fontSize FontSize, uint8_t ImageNo, uint16_t BackColor, uint16_t ForeColor, uint8_t* strings);

	/** @brief Display a pop-up like message
		@param FontSize @link fontSize Size of the font @endlink
		@param time Delay-time in seconds
		@param strings Message String (58 Char)
	*/
	void Display_Message(uint8_t FontSize, uint8_t time, uint8_t* strings);

	/** @brief Display a line
		@todo test function
		@param x1 Starting X-coordinate (2 bytes)
		@param y1 Starting Y-coordiante (2 bytes)
		@param x2 Ending X-coordinate (2 bytes)
		@param y2 Ending Y-coordiante (2 bytes)
		@param ForeColor Forground color
	*/
	void Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t ForeColor);



	/** @brief Display a rectangle
		@todo test function
		@param x X-coordinate (2 bytes)
		@param y Y-coordiante (2 bytes)
		@param width Width of the rectangle
		@param height Height of the rectangle
		@param ForeColor Forground color
	*/
	void Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t ForeColor);


	/** @brief Display a filled rectangle
		@todo test function
		@param x X-coordinate (2 bytes)
		@param y Y-coordiante (2 bytes)
		@param width Width of the rectangle
		@param height Height of the rectangle
		@param ForeColor Forground color
	*/
	void RectangleFill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t ForeColor);


	/** @brief Set backlight brightness
	*	@param brightness 0 = off, 100 = fully on
	*	@note When the backlight is turned of and the touch functions is activated.
	*	The backlight will automatically turn on after touch
	*/
	void SetBackLight(uint8_t brightness);


	/** @brief Enables or disables the touchscreen
	*	@param enable true or false
	*	@note power-up enables the touchscreen by default
	*/
	void EnableTouchScreen(bool enable);

	/** @brief Calibrates a resistance touchscreen
	*	@note capacitive touchscreens don't require calibration
	*/
	void CalibrateTouchPane(void);

	/** @brief test the touchscreen
	*	@param enable true or false
	*/
	void TestTouchPanel(bool enable);


	/** @brief Set the page on the touchscreen
	*	@param page_id Number of the page to show up
	*/
	void SetPage(uint8_t page_id);

	/** @brief Get the number of the current page on the touchscreen.
	*/
	void GetPage(void);

	//! @}


	//! @addtogroup object
	//! @{

	/** @brief Set value of a Lable-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param strings Text value to be set to the Lable-control.
	*/
	void SetLableValue(uint8_t page_id, uint8_t control_id, uint8_t* strings);

	/** @brief Set value of a Number-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param number value to be set to the Number-control.
	*/
	void SetNumberValue(uint8_t page_id, uint8_t control_id, uint16_t number);


	/** @brief Set value of a Edit-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param strings Text value to be set to the Edit-control.
	*/
	void SetEditValue(uint8_t page_id, uint8_t control_id, uint8_t* strings);


	/** @brief Request the current value of a Edit-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*/
	void GetEditValue(uint8_t page_id, uint8_t control_id);


	/** @brief Set value of a Edit-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@todo Check function
	*/
	void GetTouchEditValue(uint8_t page_id, uint8_t control_id);

		
	/** @brief Set value of a Progressbar-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param value value to be set to the Progressbar-control.
	*/
	void SetProgressbarValue(uint8_t page_id, uint8_t control_id, uint8_t value);


	/** @brief Set state of a Checkbox-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param state value to be set to the Checkbox-control.
	*/
	void SetCheckboxValue(uint8_t page_id, uint8_t control_id, checkboxState state);


	/** @brief Request current state of a Checkbox-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*/
	void GetCheckboxValue(uint8_t page_id, uint8_t control_id);


	/** @brief Set value of a CircleGauge-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param value Value to be set to the CircleGauge-control. In radians
	*/
	void SetCircleGaugeValue(uint8_t page_id, uint8_t control_id, uint16_t value);


	/** @brief Set value of a BarGauge-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param value Value to be set to the BarGauge-control. Range: 0-100%
	*/
	void SetBarGaugeValue(uint8_t page_id, uint8_t control_id, uint16_t value);


	/** @brief Set value of a WaterGauge-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param value Value to be set to the WaterGauge-control. Range: 0-100%
	*/
	void SetWaterGaugeValue(uint8_t page_id, uint8_t control_id, uint16_t value);


	/** @brief Set value of a Thermometer-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param value Value to be set to the Thermometer-control.Range: 0-120°C
	*/
	void SetThermometerValue(uint8_t page_id, uint8_t control_id, uint16_t value);


	/** @brief Set value of a Battery-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param value Value to be set to the Battery-control. Range: 0-100%
	*/
	void SetBatteryValue(uint8_t page_id, uint8_t control_id, uint16_t value);


	/** @brief Write data to Waveform-control and refresh the control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param channelNo Number for the channel (1 or 2)
	*	@param value Value to be set to the Waveform-control. Range: 0-250
	*/
	void SetWaveformValue(uint8_t page_id, uint8_t control_id, uint8_t channelNo, uint8_t value);


	/** @brief Clear all data from of a Waveform-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*/
	void WaveformDataClear(uint8_t page_id, uint8_t control_id);


	/** @brief Write data to a channel of the Waveform-control. Doensn't refresh the control.
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param channelNo Number for the channel (1 or 2)
	*	@param value Value to be set to the Waveform-control. Range: 0-250
	*/
	void WaveformDataInsert(uint8_t page_id, uint8_t control_id, uint8_t channelNo, uint8_t value);


	/** @brief Refreshes the Waveform-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param channelNo Number for individual refressing a channel (1 or 2). Or use 3 for a refresh of both channels.
	*/
	void WaveformDataRefresh(uint8_t page_id, uint8_t control_id, uint8_t channelNo);
	

	/** @brief Set value of a Slider-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*	@param value value to be set to the Slider-control.
	*/
	void SetSliderValue(uint8_t page_id, uint8_t control_id, uint8_t value);


	/** @brief Request the current value of a Slider-control
	*	@param page_id Number of the page where the control is located
	*	@param control_id Number of the control itself
	*/
	void GetSliderValue(uint8_t page_id, uint8_t control_id);

	//! @}

	//! @addtogroup return
	//! @{
	
	/**	@brief Read a new message from the touchscreen
	*	@details When a interupt is signaled use this function the translate the message. The message shall be added to the recievBuffer.
	*/
	void readMessage(void);


	/** @brief check if there is a message in the recievBuffer
	    @return true if there is a message in the reviev buffer
	*/
	bool checkNewMessage(void);


	/** @brief Return the last message from the revievBuffer
	*	@return PCTRL_MSG the last message in the buffer
	*/
	PCTRL_MSG getMessage();

	/** @brief Prints the message to the serial interface. The message is printed in human-readable form.
	*	@details
	*	The layout of the message shows the following:
	*	- @link returnType Return type @endlink
	*	- control ID, number of the control (from Visual LCD studio)
	*	- Page ID, number of the page
	*	- Status of type: @ref readState, @ref buttonState or @ref checkboxState
	*	- @link buttonTypes Button Type @endlink
	*	- Button Value
	*	@param msg PCTRL_MSG
	* 
	*/
	void verboseCommand(PCTRL_MSG msg);

	//! @}

	using Print::write;
private:
	/*!
	 * @brief Send a byte 
	 * @param buf byte(s) to be send
	 * @param len length of the buffer
	*/
	void I2C_Send(uint8_t* buf, uint8_t len);

	/**
	 * @brief Reads a byte from the i2c-bus
	 * @return byte read from i2c-bus
	*/
	uint8_t I2C_Read(void);

	void queue_reset(void);
	void queue_push(qdata _data);
	void queue_pop(qdata* _data);
	uint8_t queue_size();
	uint8_t queue_find_cmd(qdata* cmd, qsize buf_len);
};
//! @}
#endif
