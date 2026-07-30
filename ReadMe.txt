Function	Description
begin()	Initializes pins and board setup
BoardName()	Returns the board selected in Arduino IDE
BoardType()	Returns REDNOVA_V2, REDNOVA_MEGA, or REDNOVA_MICRO
ReadSwitch()	Reads DIP switch states
ReadButton()	Reads button state
ReadTrimpot()	Reads trimpot analog value
ColorFunction(r, g, b)	Sets RGB LED color (0-100)
DualDirection(left, right, delay)	Controls left/right motors with delay in ms
PlayStartup()	Plays startup LED + buzzer effect
StartMelody()	Plays optional startup melody
PlayMario()	Plays Mario melody
Jump()	Plays a short jump sound effect
Buzzer(state, duration)	Plays buzzer: state = 1 ON, 0 OFF, duration in ms
