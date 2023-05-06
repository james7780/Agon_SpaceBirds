// vdp_audio
/*
 * Title:		AGON VDPAUDIO - VDP audio c header interface
 * Author:		James Higgs
 * Created:		2023-05-06
 * Last Updated:	2023-05-06
 * 
 * Modinfo:
 * 2023-05-06:		Initial version
 */

#include <defines.h>
//#include "mos-interface.h"
//#include "stdint.h"

#ifndef VDPAUDIO_H
#define VDPAUDIO_H

// Notes:
// VDU 23, 0, 0x85, channel8, wave8, vol8, freq16, duration16
// vdu_sys_audio: void vdu_sys_audio() {
//	byte channel			// 0 to 2 (3 channels)
//	byte waveform			// ignored! (tri wave or square wave only)
//	byte volume
//	word frequency
//	word duration

UINT8 audio_play(UINT8 channel, UINT8 volume, UINT16 frequencty, UINT16 duration);


#endif VDPAUDIO_H