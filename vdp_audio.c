// vdp_audio
/*
 * Title:			AGON VDPAUDIO - VDP audio c interface implementation
 * Author::		James Higgs
 * Created:		2023-05-06
 * Last Updated:	2023-05-06
 * 
 * Modinfo:
 * 2023-05-06:		Initial version
 */

#include <defines.h>
#include "mos-interface.h"
//#include "stdint.h"

// Notes:
// VDU 23, 0, 0x85, channel8, wave8, vol8, freq16, duration16
// vdu_sys_audio: void vdu_sys_audio() {
//	byte channel			// 0 to 2 (3 channels)
//	byte waveform			// ignored! (tri wave or square wave only)
//	byte volume
//	word frequency
//	word duration

UINT8 audio_play(UINT8 channel, UINT8 volume, UINT16 frequency, UINT16 duration)
{
	unsigned int delay = 1000;

    putch(23);
    putch(0);
	putch(0x85);
	putch(channel);
	putch(0);			// waveform (does not matter)
	putch(volume);
    putch(frequency & 0xFF);
    putch(frequency >> 8);
    putch(duration & 0xFF);
    putch(duration >> 8);

	// wait for VDP to process and return status
	while (delay--);
	return getsysvar_audioSuccess();
}

/* For reference (from video.ino):
void vdu_sys_audio() {
	int channel = readByte_t();		if(channel == -1) return;
	int waveform = readByte_t();	if(waveform == -1) return;
	int volume = readByte_t();		if(volume == -1) return;
	int frequency = readWord_t();	if(frequency == -1) return;
	int duration = readWord_t();	if(duration == -1) return;

	sendPlayNote(channel, play_note(channel, volume, frequency, duration));
}
*/
