
$$ Message 0.0 definition as command set purposes:

For the format of head part,

BIT7: Request message flag
BIT6: Set target type 
BIT5: Set system command
BIT4: Set working mode (auto / manual)
BIT3: Set target channel
BIT2: Set voltage
BIT1: Set current
BIT0: Set temperature

For the format of body part,

Higher bytes: Extended and Reserved.
Higher bytes: Extended and Reserved. 
1 byte: Channel number if BIT3 is set in head part
1 byte: Auto (1) / Manual (0) is set if BIT4 is set in head part
1 byte: Command type (pre-defined.) is set if BIT5 is set in head part
1 byte: Target type if BIT6 is set in head part 


