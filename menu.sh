#!/bin/sh
set -x
dialog --checklist 'Choose the desired patches' 0 0 0 \
	fn-swap			'Swap Fn and Ctrl keys'					off \
	lcd-brightness		'Allow lowering LCD backlight brightness to 1%'		on \
	lcd-backlight-925hz	'Increase the LCD backlight PWM frequency to ~925Hz'	on \
	fan-72deg		'Make fan blow at 100% speed on 72+°C CPU temperature'	on \
	enable-hotkeys		'Generate scancodes for all hotkeys'			on \
	ec-hotpatch		'Enable EC hotpatching'					off \
	battery-bugfixes	'Vladisslav2001 battery bugfixes'			off \
	2> selected
for p in $(cat selected); do
	fn="patches/$p.rapatch"
	if [ ! -e "$fn" ]; then
		echo "The patch \"$fn\" doesn't exist!"
		exit 1
	fi
	r2 -w -q -P "$fn" ec.bin || true	# see https://github.com/radare/radare2/issues/15002
done
