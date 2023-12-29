/*
 * Title:         AGON VDPAUDIO - VDP audio C interface implementation
 * Author:        James Higgs
 * Created:       2023-05-06
 * Last Updated:  2023-12-26
 * 
 * Modinfo:
 * 2023-05-06:		Initial version
 * 2023-12-26:		Updated for MOS 1.04
 */

#include <defines.h>
#include "mos-interface.h"


// MOS/VDP 1.04 audio
// https://github.com/breakintoprogram/agon-docs/wiki/VDP-%E2%80%90-Enhanced-Audio-API
// ("wave" parameter now seems to be used as a "command id")

// Audio "command" IDs
enum {
	CMD_PLAYNOTE = 0,			// 0 to 7 - Player and player bullet bitmap id's
	CMD_STATUS,
	CMD_SETVOLUME,
	CMD_SETFREQ,
	CMD_SETWAVEFORM,
	CMD_SAMPLE,
	CMD_VOLUMEENVELOPE,
	CMD_FREQENVELOPE,
	CMD_ENABLECHANNEL,
	CMD_DISABLECHANNEL,
	CMD_RESETCHANNEL
};

// Command 0 - Play note
UINT8 audio_playNote(UINT8 channel, UINT8 volume, UINT16 frequency, UINT16 duration)
{
	// Notes:
	// VDU 23, 0, 0x85, channel8, 0, vol8, freq16, duration16

	unsigned int delay = 1000;

	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_PLAYNOTE);			// Command ID 0 - "Play Note"
	putch(volume);
	putch(frequency & 0xFF);
	putch(frequency >> 8);
	putch(duration & 0xFF);
	putch(duration >> 8);

	// wait for VDP to process and return status
	while (delay--);
	return getsysvar_audioSuccess();
}

// Command 1 - Status
UINT8 audio_status(UINT8 channel)
{
	unsigned int delay = 1000;

	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_STATUS);

	// wait for VDP to process and return status
	while (delay--);
	return getsysvar_audioChannel();			// TODO - Check
}

// Command 2 - Set Volume
void audio_setVolume(UINT8 channel, UINT8 volume)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_SETVOLUME);
	putch(volume);
}

// Command 3 - Set frequency
void audio_setFreq(UINT8 channel, UINT16 frequency)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_SETFREQ);
	putch(frequency & 0xFF);
	putch(frequency >> 8);
}

// Command 4 - Set waveform
void audio_setWaveform(UINT8 channel, INT8 waveform)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_SETWAVEFORM);
	putch(waveform);
}

// Command 5 - Sample management
// VDU 23, 0, &85, sample, 5, 0, length; lengthHighByte, <sampleData>
UINT8 audio_loadSample(INT8 sampleId, UINT24 length, UINT8 *data)
{
	unsigned int delay = 1000;
	UINT24 count = 0;
	UINT8 *p = data;

	putch(23);
	putch(0);
	putch(0x85);
	putch(sampleId);
	putch(CMD_SAMPLE);
	putch(0);							// sample command "load sample"
	
	putch(length & 0xFF);
	putch(length >> 8);
	putch(length >> 16);

	for (count = 0; count < length; count++)
		{
		putch(*p);
		p++;
		}

	// wait for VDP to process and return status
	while (delay--);
	return getsysvar_audioSuccess();			// TODO - Check
}

UINT8 audio_clearSample(INT8 sampleId)
{
	unsigned int delay = 1000;

	putch(23);
	putch(0);
	putch(0x85);
	putch(sampleId);
	putch(CMD_SAMPLE);
	putch(1);							// sample command "clear sample"

	// wait for VDP to process and return status
	while (delay--);
	return getsysvar_audioSuccess();			// TODO - Check
}

// Command 6 - Volume envelope
void audio_setVolumeEnvelope(UINT8 channel, UINT16 attack, UINT16 decay, UINT8 sustain, UINT16 release)
{
// TODO
}

void audio_disableVolumeEnvelope(UINT8 channel)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_VOLUMEENVELOPE);
	putch(0);										// disable vol env
}
 
// Command 7 - Frequency envelope
// TODO !
//void audio_setFreqEnvelope(UINT8 channel, ...);

void audio_disableFreqEnvelope(UINT8 channel)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_FREQENVELOPE);
	putch(0);										// disable freq env
}

// Command 8 - Enable channel
void audio_enableChannel(UINT8 channel)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_ENABLECHANNEL);
}

// Command 9 - Disable channel
void audio_disableChannel(UINT8 channel)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_DISABLECHANNEL);
}

// Command 10 - Reset channel
void audio_resetChannel(UINT8 channel)
{
	putch(23);
	putch(0);
	putch(0x85);
	putch(channel);
	putch(CMD_RESETCHANNEL);
}

