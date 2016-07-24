#*** mutually-assured-destruction
#**    MIDI to FLAME converter

fs = require "fs"
MIDIEvents = require "midievents"
MIDIFile = require "midifile"

toArrayBuffer = (buffer) ->
	ab = new ArrayBuffer buffer.length
	view = new Uint8Array ab
	view[i] = val for val,i in buffer
	ab

changeState = (channel, active, time) ->
	return if channel.active is active
	channel.active = active

	diff = time-channel.since
	
	console.log "#{if active then "starting" else "stopping"} fire after #{diff}ms"
	channel.chain.push Math.round diff/10

	channel.since = event.playTime


midi = new MIDIFile toArrayBuffer fs.readFileSync "5th.mid"

state = []
state[channel] = active: false, since: 0, chain: [] for channel in [0...16]

for event in midi.getMidiEvents()
	continue if event.playTime > 60000
	continue unless 13 > event.channel > 10

	channel = state[event.channel]

	switch event.subtype
		when MIDIEvents.EVENT_MIDI_NOTE_ON
			changeState channel, true, event.playTime			

		when MIDIEvents.EVENT_MIDI_NOTE_OFF
			changeState channel, false, event.playTime

console.log "left = [#{state[11].chain.join ","}];"
console.log "right = [#{state[12].chain.join ","}];"
